#ifndef SchedulerThread_H
#define SchedulerThread_H

#include <Scheduler.h>
#include <tango.h>


#include <thread>

namespace Scheduler_ns {
  
class Scheduler;
class TaskCallBack;
class Task;


class TaskManagerThread : public Tango::LogAdapter {
	public:
    // Explicitly using the default constructor to underline the fact that it does get called
    TaskManagerThread(Scheduler* dev);
    ~TaskManagerThread();

	public:
		void Init();

    void Start(){
    	// This will start the thread. Notice move semantics!
      m_thread = std::thread(&TaskManagerThread::Run,this);
    }

		void Run();
		int ExecuteTask(Task&);

	private:	
		bool m_stopThread;
		Scheduler* device;	
		log4tango::Logger* m_logger;
    std::thread m_thread;
		TaskCallBack* m_taskCallBack;
    Tango::DeviceProxy* m_deviceProxy;

};//close class







class SchedulerThread : public omni_thread, public Tango::LogAdapter {
  
	private:
  	static Scheduler* device;	
			
  public :
    SchedulerThread(Scheduler* dev);
    ~SchedulerThread();

	public:
     		
	private:
		/** 
		\brief The thread infinite run loop
 		*/
    void* run_undetached(void *arg);
    /** 
		\brief Dispatch a task
 		*/
		//int DispatchTask(std::string cmd_name,std::string cmd_id);
		int ExecuteTask(Task&);
		int ExecuteTask(std::shared_ptr<Task>);
		int ExecuteTask(std::string name,std::string id){};


	private:
  	static log4tango::Logger* logger;		
		TaskCallBack* taskCallBack;
		Tango::DeviceProxy* device_proxy;
			
	friend class Scheduler;

};//close class


}//close namespace

#endif
