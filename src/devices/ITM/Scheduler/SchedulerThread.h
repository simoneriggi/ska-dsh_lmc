#ifndef SchedulerThread_H
#define SchedulerThread_H

#include <Scheduler.h>
#include <tango.h>

#include <thread>

namespace Scheduler_ns {
  
class Scheduler;
class TaskCallBack;
class Task;

class SchedulerThread : public Tango::LogAdapter {
	public:
    SchedulerThread(Scheduler* dev);
    ~SchedulerThread();

	public:
		/** 
		\brief Start the thread
 		*/		
		void Start(){
    	m_thread = std::thread(&SchedulerThread::Run,this);
    }
		
	private:
		/** 
		\brief Main thread function 
 		*/
		void Run();
		
	private:	
		bool m_stopThread;
		Scheduler* device;	
		log4tango::Logger* m_logger;
    std::thread m_thread;

		
};//close SchedulerThread class


class MonitorThread : public Tango::LogAdapter {
	public:
    MonitorThread(Scheduler* dev);
    ~MonitorThread();

	public:
		/** 
		\brief Start the thread
 		*/		
		void Start(){
    	m_thread = std::thread(&MonitorThread::Run,this);
    }
		
	private:
		/** 
		\brief Main thread function 
 		*/
		void Run();
		
	private:	
		bool m_stopThread;
		Scheduler* device;	
		log4tango::Logger* m_logger;
    std::thread m_thread;
		
	

};//close MonitorThread class

}//close namespace

#endif
