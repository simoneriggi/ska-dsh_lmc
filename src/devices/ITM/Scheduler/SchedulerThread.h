#ifndef SchedulerThread_H
#define SchedulerThread_H

#include <Scheduler.h>
#include <tango.h>

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

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
		
		/** 
		\brief Stop
 		*/
		void Stop(){
			DEBUG_STREAM<<"SchedulerThread::Stop(): INFO: Called Stop()..."<<endl;
			std::lock_guard<std::mutex> lock( m_mutex );
			m_stopThread = true;
			DEBUG_STREAM<<"SchedulerThread::Stop(): INFO: done!"<<endl;
		}

	private:
		/** 
		\brief Main thread function 
 		*/
		void Run();

		
		
	private:	
		mutable std::mutex m_mutex;	
		std::atomic<bool> m_stopThread;
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
		/** 
		\brief Stop
 		*/
		void Stop(){
			DEBUG_STREAM<<"MonitorThread::Stop(): INFO: Called Stop()..."<<endl;
			std::lock_guard<std::mutex> lock( m_mutex );
			m_stopThread = true;
			DEBUG_STREAM<<"MonitorThread::Stop(): INFO: done!"<<endl;
		}

	private:
		/** 
		\brief Main thread function 
 		*/
		void Run();
		
	private:
		mutable std::mutex m_mutex;	
		std::atomic<bool> m_stopThread;
		Scheduler* device;	
		log4tango::Logger* m_logger;
    std::thread m_thread;
	
};//close MonitorThread class

}//close namespace

#endif
