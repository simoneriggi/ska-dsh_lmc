#include <SchedulerThread.h>
#include <Scheduler.h>
#include <ZMQUtils.h>
#include <CodeUtils.h>
#include <TaskManager.h>

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
#include <chrono>

using namespace std;


namespace Scheduler_ns {

SchedulerThread::SchedulerThread(Scheduler* dev) : Tango::LogAdapter(dev), m_thread() {
			
	device = dev;
	m_logger= 0;
	if(dev) m_logger= Tango::LogAdapter(dev).get_logger();
	m_stopThread= false;
}//close constructor
    
SchedulerThread::~SchedulerThread(){
	
	DEBUG_STREAM<<"SchedulerThread::~SchedulerThread(): INFO: Deleting scheduler thread..."<<endl;
	//{
  //	std::lock_guard<std::mutex> lock( mutex );
    m_stopThread = true;
  //}
  if(m_thread.joinable()) m_thread.join();
	DEBUG_STREAM<<"SchedulerThread::~SchedulerThread(): INFO: done!"<<endl;
	
}//close destructor


void SchedulerThread::Run(){
    
	INFO_STREAM << "SchedulerThread::Run(): INFO: Starting dispatcher thread..."<<endl;
	 
	Task task;

	while(!m_stopThread){
    DEBUG_STREAM<<"SchedulerThread::Run(): INFO: Scheduler thread looping (exit? "<<m_stopThread<<")..."<<endl;
		if(m_stopThread){
			DEBUG_STREAM<<"SchedulerThread::Run(): INFO: Exit from infinite loop..."<<endl;
			break;
		}

		//Pop task	
		if( (device->m_TaskManager)->PopTask(task)<0 ) continue;
  
		//Execute task
		if( (device->m_TaskManager)->ExecuteTask(task)<0 ) continue;

  }//end infinite loop
    
	DEBUG_STREAM<<"SchedulerThread::Run(): INFO: End thread run."<<endl;

}//close Run()


//#######################################################


MonitorThread::MonitorThread(Scheduler* dev) : Tango::LogAdapter(dev), m_thread() {
			
	device = dev;
	m_logger= 0;
	if(dev) m_logger= Tango::LogAdapter(dev).get_logger();
	m_stopThread= false;
}//close constructor
    
MonitorThread::~MonitorThread(){

	DEBUG_STREAM<<"MonitorThread::~MonitorThread(): INFO: Deleting monitor thread..."<<endl;
	//{
  //	std::lock_guard<std::mutex> lock( mutex );
    m_stopThread = true;
  //}
  if(m_thread.joinable()) m_thread.join();
	DEBUG_STREAM<<"MonitorThread::~MonitorThread(): INFO: done!"<<endl;
	
}//close destructor


void MonitorThread::Run(){
    
	INFO_STREAM << "MonitorThread::Run(): INFO: Starting monitor thread..."<<endl;
	 
	Task task;

	while(!m_stopThread){
    DEBUG_STREAM<<"MonitorThread::Run(): INFO: Monitor thread looping (exit? "<<m_stopThread<<")..."<<endl;
		if(m_stopThread){
			DEBUG_STREAM<<"MonitorThread::Run(): INFO: Exit from infinite loop..."<<endl;
			break;
		}

		//Pop task	
		if( (device->m_TaskManager)->PopMonitorTask(task)<0 ) continue;
  
		//Execute task
		if( (device->m_TaskManager)->SetTaskStatus(task,eCOMPLETED,eEXPIRED)<0 ) {
			ERROR_STREAM<<"MonitorThread::Run(): ERROR: Failed to set task status!"<<endl;
			continue;
		}


  }//end infinite loop
    
	DEBUG_STREAM<<"MonitorThread::Run(): INFO: End thread run."<<endl;

}//close Run()

}//close namespace 


