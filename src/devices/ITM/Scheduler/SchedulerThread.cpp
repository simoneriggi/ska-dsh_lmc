#include <SchedulerThread.h>
#include <Scheduler.h>
#include <ZMQUtils.h>
#include <CodeUtils.h>

#include <TaskManager.h>

#include <tango.h>

//# ZMQ
#include <zmq.h>
#include <zmq_utils.h>

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
#include <chrono>

using namespace std;


namespace Scheduler_ns {

TaskManagerThread::TaskManagerThread(Scheduler* dev) : Tango::LogAdapter(dev), m_thread() {
			
	device = dev;
	m_logger= 0;
	if(dev) m_logger= Tango::LogAdapter(dev).get_logger();
	m_deviceProxy= 0;	
	m_taskCallBack= 0;	
	m_stopThread= false;
}//close constructor
    
TaskManagerThread::~TaskManagerThread(){
	
	if(m_taskCallBack){	
		DEBUG_STREAM<<"SchedulerThread::~SchedulerThread(): DEBUG: Deleting callback..."<<endl;
		delete m_taskCallBack;
		m_taskCallBack= 0;
	}

	if(m_deviceProxy){
		DEBUG_STREAM<<"SchedulerThread::~SchedulerThread(): DEBUG: Deleting device proxy..."<<endl;
		delete m_deviceProxy;
		m_deviceProxy= 0;
	}

	m_stopThread = true;
  if(m_thread.joinable()) m_thread.join();

}//close destructor

void TaskManagerThread::Init(){

	if(!m_taskCallBack) m_taskCallBack= new TaskCallBack(device);
	m_stopThread= false;

}//close Init()

void TaskManagerThread::Run(){
    
	INFO_STREAM << "TaskManagerThread::Run(): INFO: Starting dispatcher thread..."<<endl;
	std::stringstream sstream; 
   
	//## Init 
	Init();

	Task task;

	while(!m_stopThread){
    DEBUG_STREAM<<"TaskManagerThread::Run(): INFO: Scheduler thread looping..."<<endl;

		//(device->m_TaskManager)->PrintQueueTasks();
		      
		//Pop task	
		if( (device->m_TaskManager)->PopTask(task)<0 ) continue;
  
		//Execute task
		//int status= ExecuteTask(task);
		if( (device->m_TaskManager)->ExecuteTask(task)<0 ) continue;

  }//end infinite loop
    
}//close Run()


int TaskManagerThread::ExecuteTask(Task& task){

	//## Get task info
	bool hasArgin= task.hasArgin;
	std::string cmd_name= task.name;
	std::string cmd_id= task.id;
	std::string device_name= task.deviceName;
	int status= task.status;
	int exec_status= task.status;
	DEBUG_STREAM<<"TaskManager::ExecuteTask(): INFO: Executing task (cmd:"<<cmd_name<<",id:"<<cmd_id<<",status:"<<exec_status<<")..."<<endl;
	
	
	//Connect to remote device (BETTER TO MAINTAIN A CACHE OF DEVICE PROXY!!!)
	DEBUG_STREAM<<"TaskManager::ExecuteTask(): INFO: Connect to remote device "<<device_name<<" ..."<<endl;
	//Tango::DeviceProxy* device_proxy= 0;
	try{
		Tango::ApiUtil::instance()->set_asynch_cb_sub_model(Tango::PUSH_CALLBACK);
		m_deviceProxy= new Tango::DeviceProxy(device_name);
	}
	catch(Tango::DevFailed& e){
		ERROR_STREAM<<"ERROR: Failed to connect to device "<<device_name<<endl;
		Tango::Except::print_exception(e);
		/*
		if(device_proxy){
			delete device_proxy;
			device_proxy= 0;
		}
		*/
		return -1;
	}
	    
		
	
	// Execute command asynchronously	
	try{
		DEBUG_STREAM<<"TaskManager::ExecuteTask(): INFO: Execute command "<<cmd_name<<" asynchronously ..."<<endl;
		if(hasArgin){
			Tango::DeviceData din= task.argin;
			m_deviceProxy->command_inout_asynch(cmd_name,din,*(device->m_TaskCallBack));
		}
		else {
			m_deviceProxy->command_inout_asynch(cmd_name,*(device->m_TaskCallBack));
		}
	}//close try block
	catch (Tango::DevFailed &e) { 
		ERROR_STREAM<<"ERROR: Failed to execute async command to device "<<device_name<<endl;
  	Tango::Except::print_exception(e); 
		/*
		if(device_proxy){
			delete device_proxy;
			device_proxy= 0;
		}
		*/
		return -1;
  }

	
	//Find task in list and mark as 'RUNNING'	
	DEBUG_STREAM<<"TaskManager::ExecuteTask(): INFO: Set task as RUNNING ..."<<endl;
	int task_index= -1;	
	
	if(!((device->m_TaskManager)->FindTask(task_index,cmd_id,cmd_name))){
		WARN_STREAM<<"ERROR: Failed to find task (name="<<cmd_name<<", id="<<cmd_id<<") in collection"<<endl;
		return -1;
	}
	
	(device->m_TaskManager)->SetTaskExecStatus(task_index,eRUNNING,false);
	
	DEBUG_STREAM<<"TaskManager::ExecuteTask(): INFO: End task dispatch..."<<endl;
	
	return 0;

}//close ExecuteTask()


//######################################################################

Scheduler* SchedulerThread::device;
log4tango::Logger* SchedulerThread::logger;

SchedulerThread::SchedulerThread (Scheduler* dev) : Tango::LogAdapter(dev), omni_thread() {

  device = dev;
  logger= Tango::LogAdapter(dev).get_logger();
	
	DEBUG_STREAM<<"SchedulerThread::SchedulerThread(): DEBUG: Init callback..."<<endl;
	taskCallBack= 0;
	taskCallBack= new TaskCallBack(device);
  DEBUG_STREAM<<"SchedulerThread::SchedulerThread(): DEBUG: done!"<<endl;

	device_proxy= 0;
	
}//close constructor
  
SchedulerThread::~SchedulerThread(){
	
	if(taskCallBack){	
		DEBUG_STREAM<<"SchedulerThread::~SchedulerThread(): DEBUG: Deleting callback..."<<endl;
		delete taskCallBack;
		taskCallBack= 0;
	}

	if(device_proxy){
		DEBUG_STREAM<<"SchedulerThread::~SchedulerThread(): DEBUG: Deleting device proxy..."<<endl;
		delete device_proxy;
		device_proxy= 0;
	}

}//close destructor


//+------------------------------------------------------------------
/**
  *	method:	run()
  *
  *	description:	
  *
  */
//+------------------------------------------------------------------


void* SchedulerThread::run_undetached(void*) {

  INFO_STREAM << "SchedulerThread::run_undetached(): INFO: Starting dispatcher thread..."<<endl;

	std::stringstream sstream;

	
	//================================
	//==    TASK MONITOR LOOP 
	//================================
  
	//Start the infinite loop to monitor task queue and schedule tasks
	
	while(1) {  
		//Check if thread stop signal is set
		DEBUG_STREAM<<"SchedulerThread::run_undetached(): INFO: Scheduler thread looping..."<<endl;
			
		if(device->m_StopThreadFlag) {
    	DEBUG_STREAM<<"SchedulerThread::run_undetached(): INFO: Stop thread signal received, ending thread..."<<endl;
			break; 
    }


		(device->m_TaskManager)->PrintQueueTasks();


		/*
		//Consume task
		int status= (device->m_TaskManager)->ConsumeTask();
		if(status<0){
			ERROR_STREAM<<"SchedulerThread::run_undetached(): ERROR: Task consume failed..."<<endl;
		}
		*/

		
		//Pop task
		Task task;
		if( (device->m_TaskManager)->PopTask(task)<0 ) {
			WARN_STREAM<<""<<endl;
			continue;
		}

		/*
		//Pop task
		std::shared_ptr<Task> task= (device->m_TaskManager)->PopTask();
		*/

		//Execute task
		int status= ExecuteTask(task);
		//int status= ExecuteTask(cmd_name,cmd_id);
		
	}//end infinite loop

}//close run()


int SchedulerThread::ExecuteTask(std::shared_ptr<Task> task){

	cout<<"TaskManager::ExecuteTask(): INFO: Executing task..."<<endl;
		
	//Get task info
	bool hasArgin= task->hasArgin;
	std::string cmd_name= task->name;
	std::string cmd_id= task->id;
	std::string device_name= task->deviceName;
	int status= task->status;
	int exec_status= task->status;

	
	//Connect to remote device (BETTER TO MAINTAIN A CACHE OF DEVICE PROXY!!!)
	INFO_STREAM<<"TaskManager::ExecuteTask(): INFO: Connect to remote device "<<device_name<<" ..."<<endl;
	//Tango::DeviceProxy* device_proxy= 0;
	try{
		Tango::ApiUtil::instance()->set_asynch_cb_sub_model(Tango::PUSH_CALLBACK);
		device_proxy= new Tango::DeviceProxy(device_name);
	}
	catch(Tango::DevFailed& e){
		cerr<<"ERROR: Failed to connect to device "<<device_name<<endl;
		Tango::Except::print_exception(e);
		if(device_proxy){
			delete device_proxy;
			device_proxy= 0;
		}
		return -1;
	}
	    	
	
	// Execute command asynchronously	
	INFO_STREAM<<"TaskManager::ExecuteTask(): INFO: Execute command "<<cmd_name<<" asynchronously ..."<<endl;
	try{
		//(device->m_mutex)->lock();	
		if(hasArgin){
			Tango::DeviceData din= task->argin;
			//device_proxy->command_inout_asynch(cmd_name,din,*(device->m_TaskCallBack));
			device_proxy->command_inout_asynch(cmd_name,din,*taskCallBack);
		}
		else {
			//device_proxy->command_inout_asynch(cmd_name,*(device->m_TaskCallBack));
			device_proxy->command_inout_asynch(cmd_name,*taskCallBack);
		}
		//(device->m_mutex)->unlock();	
	
	}
	catch (Tango::DevFailed &e) { 
		ERROR_STREAM<<"ERROR: Failed to execute async command to device "<<device_name<<endl;
  	Tango::Except::print_exception(e); 
		if(device_proxy){
			delete device_proxy;
			device_proxy= 0;
		}
		return -1;
  }

	
	/*
	//Find task in list and mark as 'RUNNING'	
	INFO_STREAM<<"TaskManager::ExecuteTask(): INFO: Finding task ..."<<endl;
	int task_index= -1;	
	if(!(device->m_TaskManager)->FindTask(task_index,cmd_id,cmd_name)){
		cerr<<"ERROR: Failed to find top task in collection"<<endl;
		return -1;
	}

	INFO_STREAM<<"TaskManager::ExecuteTask(): INFO: Set task as RUNNING ..."<<endl;
	(device->m_TaskManager)->SetTaskExecStatus(task_index,eRUNNING,false);
	*/


	/*
	//Clear up
	INFO_STREAM<<"TaskManager::ExecuteTask(): INFO: Clearup ..."<<endl;
	if(device_proxy){
		delete device_proxy;
		device_proxy= 0;
	}
	*/
	INFO_STREAM<<"TaskManager::ExecuteTask(): INFO: End task dispatch..."<<endl;
	
	return 0;

}//close ExecuteTask()




int SchedulerThread::ExecuteTask(Task& task){
//int SchedulerThread::ExecuteTask(Task& task){

		
	//Get task info
	bool hasArgin= task.hasArgin;
	std::string cmd_name= task.name;
	std::string cmd_id= task.id;
	std::string device_name= task.deviceName;
	int status= task.status;
	int exec_status= task.status;
	INFO_STREAM<<"TaskManager::ExecuteTask(): INFO: Executing task (cmd:"<<cmd_name<<",id:"<<cmd_id<<",status:"<<exec_status<<")..."<<endl;
	
	
	//Connect to remote device (BETTER TO MAINTAIN A CACHE OF DEVICE PROXY!!!)
	INFO_STREAM<<"TaskManager::ExecuteTask(): INFO: Connect to remote device "<<device_name<<" ..."<<endl;
	Tango::DeviceProxy* device_proxy= 0;
	try{
		Tango::ApiUtil::instance()->set_asynch_cb_sub_model(Tango::PUSH_CALLBACK);
		device_proxy= new Tango::DeviceProxy(device_name);
	}
	catch(Tango::DevFailed& e){
		cerr<<"ERROR: Failed to connect to device "<<device_name<<endl;
		Tango::Except::print_exception(e);
		if(device_proxy){
			delete device_proxy;
			device_proxy= 0;
		}
		return -1;
	}
	    
	INFO_STREAM<<"pto 3"<<endl;
		
	
	// Execute command asynchronously	
	cout<<"TaskManager::ExecuteTask(): INFO: Execute command "<<cmd_name<<" asynchronously ..."<<endl;
	try{
		//(device->m_mutex)->lock();	
		if(hasArgin){
			Tango::DeviceData din= task.argin;
			device_proxy->command_inout_asynch(cmd_name,din,*(device->m_TaskCallBack));
		}
		else {
			device_proxy->command_inout_asynch(cmd_name,*(device->m_TaskCallBack));
		}
		//(device->m_mutex)->unlock();	
	
	}
	catch (Tango::DevFailed &e) { 
		cerr<<"ERROR: Failed to execute async command to device "<<device_name<<endl;
  	Tango::Except::print_exception(e); 
		if(device_proxy){
			delete device_proxy;
			device_proxy= 0;
		}
		return -1;
  }

	
	//Find task in list and mark as 'RUNNING'	
	cout<<"TaskManager::ExecuteTask(): INFO: Set task as RUNNING ..."<<endl;
	//(device->m_mutex)->lock();	
	int task_index= -1;	
	INFO_STREAM<<"pto 1"<<endl;
	
	if(!((device->m_TaskManager)->FindTask(task_index,cmd_id,cmd_name))){
		cerr<<"ERROR: Failed to find top task in collection"<<endl;
		return -1;
	}
	INFO_STREAM<<"pto 2"<<endl;
	(device->m_TaskManager)->SetTaskExecStatus(task_index,eRUNNING,false);
	//(device->m_mutex)->unlock();	
	
	//Clear up
	cout<<"TaskManager::ExecuteTask(): INFO: Clearup ..."<<endl;
	if(device_proxy){
		delete device_proxy;
		device_proxy= 0;
	}
	
	cout<<"TaskManager::ExecuteTask(): INFO: End task dispatch..."<<endl;
	
	return 0;

}//close ExecuteTask()



/*
void* SchedulerThread::run_undetached(void*) {

  INFO_STREAM << "SchedulerThread::run_undetached(): INFO: Starting dispatcher thread..."<<endl;

	std::stringstream sstream;

	int MAX_QUEUE_SIZE= device->queue_size;	
	
	
	//================================
	//==    TASK MONITOR LOOP 
	//================================
  
	//Start the infinite loop to monitor task queue and schedule tasks
	
	while(1) {  
		//Check if thread stop signal is set
		if(device->m_StopThreadFlag) {
      logger->debug("SchedulerThread::run_undetached(): INFO: Stop thread signal received, ending thread...");
			break; 
    }

		(device->m_mutex)->lock();

		//Check action to be performed
		bool hasTasksInQueue= !(device->m_TaskQueue).empty();
		
		if(hasTasksInQueue){

			//(device->m_mutex)->lock();
			Task task= (device->m_TaskQueue).top();
			
			auto task_start= task.start_time;
			auto task_end= task.end_time;
			std::string cmd_id= task.id;
			std::string cmd_name= task.name;
			
			//Dispatch task?
			auto now = std::chrono::system_clock::now();
			//double tdiff= std::chrono::duration<double>(task->start_time-now).count();
			double tdiff= std::chrono::duration<double>(task.start_time-now).count();

			sstream.str(std::string());
			//sstream<<"SchedulerThread::run_undetached(): DEBUG: Task@top: cmd_name="<<cmd_name<<", cmd_device="<<cmd_device<<" cmd_id="<<cmd_id<<", tdiff="<<tdiff<<" t["<<task_start.time_since_epoch().count()<<","<<task_end.time_since_epoch().count()<<"]";
			sstream<<"SchedulerThread::run_undetached(): DEBUG: Task@top: cmd_name="<<cmd_name<<" cmd_id="<<cmd_id<<", tdiff="<<tdiff<<" t["<<task_start.time_since_epoch().count()<<","<<task_end.time_since_epoch().count()<<"]";
			logger->debug(sstream.str().c_str());

			if (tdiff<=0 && now<task.end_time) {
				logger->debug("SchedulerThread::run_undetached(): DEBUG: Dispatching task...");
				(device->m_TaskQueue).pop();
				//if(DispatchTask(task)<0){
				if(DispatchTask(cmd_name,cmd_id)<0){
					logger->error("SchedulerThread::run_undetached(): ERROR: Failed to dispatch task!");
				}
			}
			else{
				//Sleeping up to the time of the top task or until a wake-up signal is received
				
				double tdiff_sec, tdiff_nsec;
				tdiff_nsec= modf (tdiff, &tdiff_sec);
				long int thisSleepSec= static_cast<long int>(tdiff_sec);
				long int thisSleepNSec= static_cast<long int>(tdiff_nsec*1000000000L);
				unsigned long sleepSec, sleepNSec;
				get_time( &sleepSec, &sleepNSec, thisSleepSec, thisSleepNSec);
				

				sstream.str(std::string());
				sstream<<"DispatcherThread::run_undetached(): DEBUG: Sleeping for tdiff("<<thisSleepSec<<","<<thisSleepNSec<<") t("<<sleepSec<<","<<sleepNSec<<") until next task or signal received...";
				logger->debug(sstream.str().c_str());
	
				//(device->m_mutex)->lock();	
				(device->m_wait_condition)->timedwait(sleepSec, sleepNSec);
				//(device->m_mutex)->unlock();	
				logger->debug("DispatcherThread::run_undetached(): DEBUG: Sleep time over...");
	
			}//close else

			
		}//close if hasTasksInQueue
		else{
			//Queue is empty, sleeping until a wake up signal is given (when a task is pushed in the queue)
			logger->debug("SchedulerThread::run_undetached(): INFO: Sleeping until wake up signal...");
			//(device->m_mutex)->lock();		
			(device->m_wait_condition)->wait();	
			//(device->m_mutex)->unlock();	
				
			logger->debug("DispatcherThread::run_undetached(): DEBUG: Sleep time over...");
		}

		(device->m_mutex)->unlock();

	}//end foreever loop

	INFO_STREAM << "SchedulerThread::run_undetached(): INFO: End scheduler thread..."<<endl;

  return 0;

}//close run()
*/

/*
int SchedulerThread::DispatchTask(std::string cmd_name,std::string cmd_id){

	logger->info("SchedulerThread::DispatchTask(): INFO: Starting task...");
	std::stringstream sstream;
	
	//Finding task
	CmdTask* task= (device->m_CmdCollection).FindTask(cmd_id,cmd_name);
	if(!task){
		logger->error("SchedulerThread::DispatchTask(): ERROR: Null ptr to task given!");
		return -1;
	}
		
	//Get task info
	CORBA::Any* argin_any= task->argin;
	//std::string cmd_name= task->name;
	//std::string cmd_id= task->id;
	std::string device_name= task->device_name;
	int status= task->status;
	int exec_status= task->status;

	//Connect to remote device (BETTER TO MAINTAIN A CACHE OF DEVICE PROXY!!!)
	logger->info("SchedulerThread::DispatchTask(): INFO: Connect to remote device...");
	Tango::DeviceProxy* device_proxy= 0;
	try{
		device_proxy= new Tango::DeviceProxy(device_name);
	}
	catch(Tango::DevFailed& e){
		Tango::Except::print_exception(e);
		sstream.str(std::string());
		sstream << "SchedulerThread::DispatchTask(): ERROR: Failed to connect to proxy device "<<device_name;
		logger->error(sstream.str().c_str());
		if(device_proxy){
			delete device_proxy;
			device_proxy= 0;
		}
		return -1;
	}
	
	//Set command argument data  
  Tango::DeviceData din; 
	logger->info("SchedulerThread::DispatchTask(): INFO: Set command argument data ...");
  try { 
		//Tango::DevString argin_string;
		//*argin_any >>= argin_string;
  	//din<<argin_string;
		din<<argin_any;
  } 
  catch(Tango::DevFailed& e){
		Tango::Except::print_exception(e);
		logger->error("SchedulerThread::DispatchTask(): ERROR: Failed to set command argument data!");
		if(device_proxy){
			delete device_proxy;
			device_proxy= 0;
		}
		return -1;
	}      

	// Execute command asynchronously	
	logger->info("SchedulerThread::DispatchTask(): INFO: Execute command asynchronously	 ...");
	try{
		device_proxy->command_inout_asynch(cmd_name,din,device->m_TaskCallBack);
	}
	catch (Tango::DevFailed &e) { 
  	Tango::Except::print_exception(e); 
		logger->error("SchedulerThread::DispatchTask(): ERROR: Failed to execute async command!");
		if(device_proxy){
			delete device_proxy;
			device_proxy= 0;
		}
		return -1;
  }

	//Set task as 'RUNNING'
	logger->info("SchedulerThread::DispatchTask(): INFO: Set task as RUNNING ...");
	//(device->m_mutex)->lock();
	task->exec_status= eRUNNING;
	logger->info("SchedulerThread::DispatchTask(): INFO: Set task as RUNNING after...");
	//(device->m_mutex)->unlock();

	//Clear up
	logger->info("SchedulerThread::DispatchTask(): INFO: Clearup ...");
	if(device_proxy){
		delete device_proxy;
		device_proxy= 0;
	}

	logger->info("SchedulerThread::DispatchTask(): INFO: End task dispatch...");
	

	return 0;

}//close DispatchTask()
*/

}//close namespace 


