#ifndef ZMQ_UTILS_H
#define ZMQ_UTILS_H

#include <zmq.h>

#include <iostream>
#include <sstream>
#include <iomanip>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <vector>
#include <map>

#if (defined (WIN32))
#   include <time.h>
#else
#   include <sys/time.h>
#endif

#include <assert.h>
#include <signal.h>

//  Version checking, and patch up missing constants to match 2.1
#if ZMQ_VERSION_MAJOR == 2
#   error "Please upgrade to ZeroMQ/3.2 for these examples"
#endif

//  Provide random number from 0..(num-1)
#if (defined (WIN32))
#   define randof(num)  (int) ((float) (num) * rand () / (RAND_MAX + 1.0))
#else
#   define randof(num)  (int) ((float) (num) * random () / (RAND_MAX + 1.0))
#endif


#if ZMQ_VERSION_MAJOR == 4
#   define ZMQ_POLL_MSEC    1           //  zmq_poll is msec
#elif ZMQ_VERSION_MAJOR == 3
#   define ZMQ_POLL_MSEC    1           //  zmq_poll is msec
#elif ZMQ_VERSION_MAJOR == 2
#   define ZMQ_POLL_MSEC    1000 
#endif


#ifndef MULTIMSG_END_PATTERN
	#define MULTIMSG_END_PATTERN "#END#"
#endif

using namespace std;

namespace Utils_ns {

class ZMQUtils {
	public:

    ZMQUtils();
		~ZMQUtils();

		struct SBuffer{
			std::string data;
			int size;
			std::string identity;
			int identity_size;
			std::string topic;
			int topic_size;
		};
		typedef std::vector<SBuffer> SBufferCollection; 

		struct ZMQSendOptions{
			bool send_empty;
			bool send_topic;
			std::string topic;
			bool send_identity;
			std::string identity;
			bool sendmore;
		};

		struct ZMQRcvOptions{
			bool rcv_empty;
			bool rcv_topic;
			bool rcv_identity;
			bool rcvmore;
		};
		enum recv_status {eRecvOk=0,eRecvFault=1,eRecvCritical=-1};
		
	public:
		static SBuffer recv_buffer(void* socket,int& errflag);
		static SBuffer recv_message(void* socket,ZMQRcvOptions options,int& errflag);
		static int send_buffer (void* socket,SBuffer buffer,bool sendmore=false);
		static int send_message (void* socket,SBuffer buffer,ZMQSendOptions options);
	
		static int proxy(void* frontend,void* backend,std::string identity);

	protected:

	private:
		
};//close class

}//close namespace

#endif
