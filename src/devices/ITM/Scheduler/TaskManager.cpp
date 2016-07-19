#include <TaskManager.h>
#include <TaskCallBack.h>


#include <tango.h>

//# JSON CPP
#include <json/json.h>

//## Standard headers
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ctime>
#include <stdexcept>
#include <unistd.h>
#include <getopt.h>
#include <math.h>
#include <time.h>

#include <map>
#include <vector>
#include <thread>
#include <memory>
#include <functional>
#include <condition_variable>
#include <mutex>

//#include <omnithread.h>

using namespace std;


namespace Scheduler_ns {

bool TaskManager::m_wakeUpSignal;
bool TaskManager::m_endSignal;	

TaskManager::TaskManager(Scheduler* dev,TaskCallBack* cb) 
	: Tango::LogAdapter(dev), m_taskCallBack(cb)
{

	m_logger= 0;
	if(dev) m_logger= Tango::LogAdapter(dev).get_logger();
	m_deviceProxy= 0;
	m_wakeUpSignal= false;
	m_endSignal= false;
	m_maxTasksInQueue= 30;
	m_maxTasksInCollection= 1000;
	m_maxCachedDevProxies= 10;

}//close constructor
  

TaskManager::~TaskManager(){


}//close destructor

int TaskManager::AddTask(Task& task){
	
	std::lock_guard<std::mutex> lock(m_mutex);

	//Set task status to IDLE
	task.execStatus= eIDLE;

	//Check queue/collection size
	if((int)m_taskQueue.size()>=m_maxTasksInQueue || m_tasks.GetN()>=m_maxTasksInCollection) {
		WARN_STREAM<<"TaskManager::AddTask(): ERROR: Maximum number of tasks in queue and/or in collection reached, cannot add task!"<<endl;
		return -1;
	}

	//Add task to queue & collection
	DEBUG_STREAM<<"TaskManager::AddTask(): Adding task: id="<<task.id<<endl;
	try {	
		m_taskQueue.insert(task);
		m_monitorQueue.insert(task);
		m_tasks.AddTask(task);
	}
	catch(...){
		WARN_STREAM<<"TaskManager::AddTask(): ERROR: Exception occurred while adding task in queue/collection..."<<endl;
		return -1;
	}
	
	//Emit signal to wake up any the waiting thread
	m_wakeUpSignal = true;
  m_cond.notify_all();

	return 0;

}//close AddTask()

void TaskManager::WakeUp(){
	
	std::lock_guard<std::mutex> lock(m_mutex);
	DEBUG_STREAM<<"TaskManager::WakeUp(): INFO: Wake up signal throw..."<<endl;
	m_wakeUpSignal = true;
  //m_cond.notify_one();
	m_cond.notify_all();
	DEBUG_STREAM<<"TaskManager::WakeUp(): INFO: done!"<<endl;
	
}//close WakeUp()

void TaskManager::End(){

	std::lock_guard<std::mutex> lock(m_mutex);
	DEBUG_STREAM<<"TaskManager::End(): INFO: End signal throw..."<<endl;
	m_endSignal = true;
  //m_cond.notify_one();
	m_cond.notify_all();
	DEBUG_STREAM<<"TaskManager::End(): INFO: done!"<<endl;
	
}//close End()


bool TaskManager::IsEmptyQueue() const {
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_taskQueue.empty();
}

void TaskManager::PrintQueueTasks(){
	std::lock_guard<std::mutex> lock(m_mutex);
	if(m_taskQueue.empty()) return;
	auto it = m_taskQueue.begin();
	Task topTask= *it;  
	topTask.Print();
}//close PrintQueueTasks()


int TaskManager::PopMonitorTask(Task& task) {
    
	std::unique_lock<std::mutex> mlock(m_mutex);

	//If queue is empty wait until signalled
	DEBUG_STREAM << "TaskManager::PopMonitorTask(): INFO: Waiting until signaled..."<<endl;
	m_cond.wait(mlock,[this]{return (!m_monitorQueue.empty() || m_wakeUpSignal || m_endSignal);});
	DEBUG_STREAM << "TaskManager::PopMonitorTask(): INFO: Awake..."<<endl;
  	
	if(m_endSignal || m_monitorQueue.empty()){
		DEBUG_STREAM << "TaskManager::PopMonitorTask(): INFO: End signal catched, exiting..."<<endl;
		return -1;
	}

	m_wakeUpSignal = false;

	//## Get task & info
	auto it= m_monitorQueue.begin();
	task = *it;

	auto task_start= task.startTime;
	auto task_end= task.endTime;
	int task_exec_status= task.execStatus;
	std::string task_id= task.id;
	std::string task_name= task.name;
	auto now = std::chrono::system_clock::now();
	auto tdiff= std::chrono::duration<double,std::milli>(task_end-now);
	DEBUG_STREAM<<"TaskManager::PopMonitorTask(): INFO: Task info: name="<<task_name<<", id="<<task_id<<", status="<<task_exec_status<<", texp_diff(s)="<<tdiff.count()/1000.<<endl;
	
	//## If expiration time is reached, remove task from queue and return the popped task
	if (tdiff.count()<=0) {
		DEBUG_STREAM<<"TaskManager::PopMonitorTask(): INFO: Popping task: name="<<task_name<<", id="<<task_id<<", texp_diff(s)="<<tdiff.count()/1000.<<endl;
		m_monitorQueue.erase(it);
		return 0;	
	}

	//## If expiration time is not reached, wait until task expiration or until any signal (wakeup/end)
	if( m_cond.wait_for(mlock,tdiff, [](){return (m_wakeUpSignal || m_endSignal);}) ){
		DEBUG_STREAM<<"TaskManager::PopMonitorTask(): INFO: Finished waiting (signaled)..."<<endl;
		if(m_wakeUpSignal) m_wakeUpSignal= false;
	}
	else {
		DEBUG_STREAM<<"TaskManager::PopMonitorTask(): INFO: Finished waiting (Timeout reached)..."<<endl;
	}
	
	DEBUG_STREAM << "TaskManager::PopMonitorTask(): INFO: Awake after timeout..."<<endl;
  
  return -1;

}//close PopMonitorTask()


int TaskManager::PopTask(Task& task) {
    
	std::unique_lock<std::mutex> mlock(m_mutex);
	
	//If queue is empty wait until signalled
	DEBUG_STREAM << "TaskManager::PopTask(): INFO: Waiting until signaled..."<<endl;
	m_cond.wait(mlock,[this]{return (!m_taskQueue.empty() || m_wakeUpSignal || m_endSignal);});
	DEBUG_STREAM << "TaskManager::PopTask(): INFO: Awake..."<<endl;
  	
	if(m_endSignal || m_taskQueue.empty()){
		DEBUG_STREAM << "TaskManager::PopTask(): INFO: End signal catched, exiting..."<<endl;
		return -1;
	}
	
	m_wakeUpSignal = false;

	//## Get task & info
  //task = m_taskQueue.top();
	//task= std::move(m_taskQueue.top());
	auto it= m_taskQueue.begin();
	task = *it;

	auto task_start= task.startTime;
	auto task_end= task.endTime;
	int task_exec_status= task.execStatus;
	std::string task_id= task.id;
	std::string task_name= task.name;
	auto now = std::chrono::system_clock::now();
	auto tdiff= std::chrono::duration<double,std::milli>(task_start-now);
	DEBUG_STREAM<<"TaskManager::PopTask(): INFO: Task info: name="<<task_name<<", id="<<task_id<<", status="<<task_exec_status<<", tdiff(s)="<<tdiff.count()/1000.<<endl;
	
	//## If activation time is reached, remove task from queue and return the popped task
	if (tdiff.count()<=0 && now<task_end && task_exec_status==eIDLE) {
		//Remove task from the queue
		DEBUG_STREAM<<"TaskManager::PopTask(): INFO: Popping task: name="<<task_name<<", id="<<task_id<<", tdiff(s)="<<tdiff.count()/1000.<<endl;
		//m_taskQueue.pop();
		m_taskQueue.erase(it);
		return 0;	
	}

	//## If activation time is not reached, wait until task activation or until any signal (wakeup/end)
	if( m_cond.wait_for(mlock,tdiff, [](){return (m_wakeUpSignal || m_endSignal);}) ){
		DEBUG_STREAM<<"TaskManager::PopTask(): INFO: Finished waiting (signaled)..."<<endl;
		if(m_wakeUpSignal) m_wakeUpSignal= false;
	}
	else {
		DEBUG_STREAM<<"TaskManager::PopTask(): INFO: Finished waiting (Timeout reached)..."<<endl;
	}
	
	DEBUG_STREAM << "TaskManager::PopTask(): INFO: Awake after timeout..."<<endl;
  
  return -1;

}//close PopTask()


int TaskManager::RemoveTask(std::string cmd_id){

	std::unique_lock<std::mutex> mlock(m_mutex);
	if(cmd_id=="") return -1;

	//Remove task from queue 
	if(RemoveTaskFromQueue(cmd_id)<0){
		WARN_STREAM<<"TaskManager::RemoveTask(): ERROR: Failed to remove task from queue (cannot find task or delete failed)!"<<endl;
		return -1;
	}

	if(RemoveTaskFromMonitorQueue(cmd_id)<0){
		WARN_STREAM<<"TaskManager::RemoveTask(): ERROR: Failed to remove task from monitor queue (cannot find task or delete failed)!"<<endl;
		return -1;
	}
	
	//Remove task from list
	if(m_tasks.RemoveTask(cmd_id)<0){
		WARN_STREAM<<"TaskManager::RemoveTask(): ERROR: Failed to remove task from list (cannot find task or delete failed)!"<<endl;
		return -1;
	}

	return 0;

}//close RemoveTask()


int TaskManager::FlushQueue(){
	std::unique_lock<std::mutex> mlock(m_mutex);
	
	//Erase all tasks in queue
	try{
		m_taskQueue.erase(m_taskQueue.begin(),m_taskQueue.end());
		m_monitorQueue.erase(m_monitorQueue.begin(),m_monitorQueue.end());
	}
	catch(...){
		WARN_STREAM<<"TaskManager::FlushQueue(): ERROR: Failed to remove all tasks from queue!"<<endl;
		return -1;
	}

	//Erase all IDLE tasks in list
	if(m_tasks.RemoveIdleTasks()<0){
		WARN_STREAM<<"TaskManager::RemoveTask(): ERROR: Failed to remove IDLE tasks from list!"<<endl;
		return -1;
	}

	return 0;

}//close FlushQueue()


TaskPtr TaskManager::FindTaskPtr(int& index,std::string id,std::string name){

	std::lock_guard<std::mutex> lock(m_mutex);
	return m_tasks.FindTaskPtr(index,id,name);
	
}//close FindTaskPtr()

Task* TaskManager::FindTask(int& index,std::string id,std::string name){

	std::lock_guard<std::mutex> lock(m_mutex);
	return m_tasks.FindTask(index,id,name);
	
}//close FindTask()


int TaskManager::ExecuteTask(Task& task){

	std::lock_guard<std::mutex> lock(m_mutex);
	DEBUG_STREAM<<"TaskManager::ExecuteTask(): INFO: Executing task..."<<endl;
		
	//Get task info
	Tango::DeviceData din= task.argin;
	std::string cmd_name= task.name;
	std::string cmd_id= task.id;
	std::string device_name= task.deviceName;
	int status= task.status;
	int exec_status= task.status;

	//Find task in list
	DEBUG_STREAM<<"TaskManager::ExecuteTask(): INFO: Find task in collection ..."<<endl;
	int task_index= -1;
	if(!m_tasks.FindTask(task_index,cmd_id,cmd_name)){
		ERROR_STREAM<<"TaskManager::ExecuteTask(): WARN: Failed to find top task in collection"<<endl;
		return -1;
	}

	//Find device among cached
	m_deviceProxy= 0;
	try {
		m_deviceProxy= GetCachedDeviceProxy(device_name);
		if(!m_deviceProxy){
			DEBUG_STREAM<<"TaskManager::ExecuteTask(): INFO: Device not found among cached, connecting to remote device "<<device_name<<" ..."<<endl;
			m_deviceProxy= new Tango::DeviceProxy(device_name);
			if(AddDeviceToCached(device_name,m_deviceProxy,false)<0){
				throw std::runtime_error("Failed to insert device proxy to cache!");
			}
		}//close if
	}//close try
	catch(Tango::DevFailed& e){
		Tango::Except::print_exception(e);
		ERROR_STREAM<<"ERROR: Failed to connect to device "<<device_name<<endl;
		m_tasks.SetTaskStatus(task_index,eCOMPLETED,eABORTED,false);
		return -1;
	}
	catch(std::exception& e){
		ERROR_STREAM<<"TaskManager::ExecuteTask(): INFO: Run time exception detected ("<<e.what()<<")"<<endl;
		m_tasks.SetTaskStatus(task_index,eCOMPLETED,eABORTED,false);
		return -1;
	}

	    
	// Execute command asynchronously	
	DEBUG_STREAM<<"TaskManager::ExecuteTask(): INFO: Execute command "<<cmd_name<<" asynchronously ..."<<endl;
	try{
		m_deviceProxy->command_inout_asynch(cmd_name,din,*m_taskCallBack);
	}
	catch (Tango::DevFailed &e) { 
		Tango::Except::print_exception(e); 
		ERROR_STREAM<<"ERROR: Failed to execute async command to device "<<device_name<<endl;
		m_tasks.SetTaskStatus(task_index,eCOMPLETED,eABORTED,false);
		return -1;
  }

	//Find task in list and mark as 'RUNNING'	
	DEBUG_STREAM<<"TaskManager::ExecuteTask(): INFO: Set task as RUNNING ..."<<endl;
	m_tasks.SetTaskStatus(task_index,eUNKNOWN,eRUNNING,false);
	
	DEBUG_STREAM<<"TaskManager::ExecuteTask(): INFO: End task dispatch..."<<endl;
	
	return 0;

}//close ExecuteTask()



int TaskManager::GetTaskPipe(Tango::Pipe& pipe){

	std::lock_guard<std::mutex> lock(m_mutex);
	if(m_tasks.GetPipe(pipe)<0){
		ERROR_STREAM<<"TaskManager::GetTaskPipe(): ERROR: Failed to get pipe from task collection!"<<endl;
		return -1;
	}
	return 0;
}//close GetTaskPipe()

int TaskManager::SetTaskStatus(int index,int exec_status,int status,bool check){
	
	std::unique_lock<std::mutex> mlock(m_mutex);
	return m_tasks.SetTaskStatus(index,exec_status,status,check);
	
}//close SetTaskStatus()


int TaskManager::SetTaskStatus(Task& task,int exec_status,int status){

	//Get access to response fields and access to task list
	int index= -1;
	std::string cmd_id= task.id;
	std::string cmd_name= task.name;
	if( !FindTask(index,cmd_id,cmd_name) ){
		ERROR_STREAM << "TaskManager::SetTaskStatus(): ERROR: Failed to find task (name="<<cmd_name<<", id="<<cmd_id<<") in list"<<endl;
		return -1;
	}
	
	//Update cmd status in task list
	return SetTaskStatus(index,exec_status,status,false);
	
}//close SetTaskStatus()


int TaskManager::ClearTasks(double historyTimeThr,bool clearRunningTasks){

	//## Clean the task list
	//## Completed tasks are removed if too old (>historyTimeThr)
	//## Running tasks are removed if flag clearRunningTasks is true
	std::lock_guard<std::mutex> lock(m_mutex);

	return m_tasks.ClearTasks(historyTimeThr,clearRunningTasks);

}//close CleanTasks()


}//close namespace

