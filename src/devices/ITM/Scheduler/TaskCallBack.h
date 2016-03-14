#ifndef TaskCallBack_H
#define TaskCallBack_H

#include <tango.h>
#include <Scheduler.h>

namespace MessageParser_ns {
class Response;
}

namespace Scheduler_ns {
  
class Scheduler;

class TaskCallBack : public Tango::CallBack, public Tango::LogAdapter { 
		
	public:
		TaskCallBack(Scheduler* dev);
		~TaskCallBack();
		   
	public:
		void cmd_ended(Tango::CmdDoneEvent*); 
			
	private:
		static log4tango::Logger* logger;
		static Scheduler* device;	

		std::shared_ptr<MessageParser_ns::Response> m_TaskResponse;
		
	
	friend class Scheduler;

};
}//close namespace

#endif
