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

	m_stopThread = true;
  if(m_thread.joinable()) m_thread.join();

}//close destructor


void SchedulerThread::Run(){
    
	INFO_STREAM << "SchedulerThread::Run(): INFO: Starting dispatcher thread..."<<endl;
	 
	Task task;

	while(!m_stopThread){
    DEBUG_STREAM<<"SchedulerThread::Run(): INFO: Scheduler thread looping..."<<endl;

		//Pop task	
		if( (device->m_TaskManager)->PopTask(task)<0 ) continue;
  
		//Execute task
		if( (device->m_TaskManager)->ExecuteTask(task)<0 ) continue;
  }//end infinite loop
    
}//close Run()


//#######################################################


MonitorThread::MonitorThread(Scheduler* dev) : Tango::LogAdapter(dev), m_thread() {
			
	device = dev;
	m_logger= 0;
	if(dev) m_logger= Tango::LogAdapter(dev).get_logger();
	m_stopThread= false;
}//close constructor
    
MonitorThread::~MonitorThread(){

	m_stopThread = true;
  if(m_thread.joinable()) m_thread.join();

}//close destructor


void MonitorThread::Run(){
    
	INFO_STREAM << "MonitorThread::Run(): INFO: Starting dispatcher thread..."<<endl;
	 
	Task task;

	while(!m_stopThread){
    DEBUG_STREAM<<"MonitorThread::Run(): INFO: Monitor thread looping..."<<endl;

		//Pop task	
		if( (device->m_TaskManager)->PopMonitorTask(task)<0 ) continue;
  
		//Execute task
		if( (device->m_TaskManager)->SetTaskStatus(task,eCOMPLETED,eEXPIRED)<0 ) {
			ERROR_STREAM<<"MonitorThread::Run(): ERROR: Failed to set task status!"<<endl;
			continue;
		}
  }//end infinite loop
    
}//close Run()

}//close namespace 


