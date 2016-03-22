#ifndef NagiosListenerThread_H
#define NagiosListenerThread_H

#include <SelfMonitoring.h>
#include <SocketUtils.h>
#include <ZMQUtils.h>

#include <tango.h>


namespace SelfMonitoring_ns {
  
class SelfMonitoring;
class SocketClient;

class NagiosListenerThread : public omni_thread, public Tango::LogAdapter {
  
	private:
  	static SelfMonitoring* device;	
			
  public :
    NagiosListenerThread(SelfMonitoring* dev,void* ctx);
    ~NagiosListenerThread();

	public:
     		
	private:
    void* run_undetached(void *arg);
		int ProcessEvent(Utils_ns::ZMQUtils::SBuffer& sbuffer);
    
	private:	
  	static log4tango::Logger* logger;		
    Utils_ns::SocketClient* m_SocketClient;
		void* m_ZmqContext;
		void* m_ZmqSocket;	
		Utils_ns::ZMQUtils::SBuffer m_eventBuffer;
			
	friend class SelfMonitoring;

};//close class


}//close namespace

#endif
