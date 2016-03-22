
#include <ZMQUtils.h>
#include <zmq.h>

#include <string>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <sstream>

#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <map>
#include <chrono>


using namespace std;

namespace Utils_ns {

ZMQUtils::ZMQUtils() {	
	
}   


ZMQUtils::~ZMQUtils(){

}

int ZMQUtils::sendrcv_message(std::vector<SBuffer>& recvBuffers,void* socket,SBuffer& sndBuffer,ZMQSendOptions SndOptions,ZMQRcvOptions RcvOptions,int nMaxMsg,int poll_timeout,int read_timeout){

	//Send message
	if(send_message(socket,sndBuffer,SndOptions)<0){
		cerr<<"ZMQUtils::sendrcv_message(): ERROR: Send failed!"<<endl;
		return -1;
	}
	
	//Receive message(s)
	zmq_pollitem_t items[] = {
  	{ socket, 0, ZMQ_POLLIN, 0 }
  };
	int errflag;
	int nMsg= 0;

	auto start = std::chrono::system_clock::now();

	while(1) {
		//Check end conditions
		if(nMsg>=nMaxMsg) {
			cout<<"ZMQUtils::sendrcv_message(): INFO: Max number of reply messages (n="<<nMsg<<") reached!"<<endl;
			break;
		}
		auto now = std::chrono::system_clock::now();
		auto tdiff = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
		if(tdiff>=read_timeout) {
			cerr<<"ZMQUtils::sendrcv_message(): WARN: Read timeout (timeout="<<read_timeout<<") exceeded!"<<endl;
			break;
		}

		//Check if there are messages from main thread
		int poll_status = zmq_poll (items, 1, poll_timeout*ZMQ_POLL_MSEC);
    if (poll_status == -1) {//poll returns with error
			cerr<<"ZMQUtils::sendrcv_message(): ERROR: Polling error ..."<<endl;     
			if(errno==EINTR || errno==ETERM) {
				cerr<<"ZMQUtils::sendrcv_message(): ERROR: EINTR/ETERM state..."<<endl;
        return -1;
      }
      continue;
    }

		if (items[0].revents & ZMQ_POLLIN) {	
			SBuffer recvBuffer= recv_message(socket,RcvOptions,errflag);
      if(recvBuffer.data.empty() || recvBuffer.size==-1){
        cerr<<"ZMQUtils::sendrcv_message(): WARN: Empty message received..."<<endl;
    		continue;
      }
			cout<<"ZMQUtils::sendrcv_message(): recvBuffer: "<<recvBuffer.data<<endl;
			recvBuffers.push_back(recvBuffer);
			nMsg++;
		}//close if
	}//end loop

	if(nMsg<=0) {
		cerr<<"ZMQUtils::sendrcv_message(): WARN: No replies received from the socket!"<<endl;
		return -1;
	}

	return 0;
	
}//close ZMQUtils::send_rcv()


ZMQUtils::SBuffer ZMQUtils::recv_buffer(void* socket,int& errflag){
	
	ZMQUtils::SBuffer buffer;
	buffer.data= "";
	buffer.size= 0;
	buffer.identity= "";
	buffer.identity_size= 0;
	buffer.topic= "";
	buffer.topic_size= 0;

	zmq_msg_t msg;
	zmq_msg_init (&msg);
	int size= zmq_msg_recv (&msg, socket, 0);
	if (size == -1) {
		buffer.size= size;
		errflag= errno;	
		zmq_msg_close (&msg);
		return buffer;
	}

	std::string data = std::string(static_cast<char*>(zmq_msg_data(&msg)), zmq_msg_size(&msg));

	buffer.data= data;
	buffer.size= size;
	buffer.identity= "";
	buffer.identity_size= 0;
	buffer.topic= "";
	buffer.topic_size= 0;

	zmq_msg_close (&msg);
	
	return buffer;
	
}//close recv_buffer()


ZMQUtils::SBuffer ZMQUtils::recv_message(void* socket,ZMQRcvOptions options,int& errflag){
	
	ZMQUtils::SBuffer buffer;
	buffer.data= "";
	buffer.size= 0;
	buffer.identity= "";
	buffer.identity_size= 0;
	buffer.topic= "";
	buffer.topic_size= 0;

	//## Receive identity?
	SBuffer identity_buffer;
	identity_buffer.data= "";
	identity_buffer.size= 0;
	identity_buffer.identity= "";
	identity_buffer.identity_size= 0;
	identity_buffer.topic= "";
	identity_buffer.topic_size= 0;

	if(options.rcv_identity){
		identity_buffer= recv_buffer(socket,errflag);	
		if(identity_buffer.size==-1) {//failure in receive
			cerr<<"ZMQUtils::recv_message(): ERROR: Identity message recv failed"<<endl;			
			return buffer;
		}
		buffer.identity= identity_buffer.data;
		buffer.identity_size= identity_buffer.size;		
	}	

	//## Receive empty frame?
	ZMQUtils::SBuffer empty_buffer;
	empty_buffer.data= "";
	empty_buffer.size= 0;
 	empty_buffer.identity= "";
	empty_buffer.identity_size= 0;

	if(options.rcv_empty){
		empty_buffer= recv_buffer(socket,errflag);
		if(empty_buffer.size==-1) {//failure in receive
			cerr<<"ZMQUtils::recv_message(): ERROR: Empty message recv failed"<<endl;			
			return buffer;
		}
		if(empty_buffer.data!=""){
			cerr<<"ZMQUtils::recv_message(): WARNING: Empty message is not empty ("<<empty_buffer.data<<")"<<endl;			
		}
	}

	//## Receive topic?
	ZMQUtils::SBuffer topic_buffer;
	topic_buffer.data= "";
	topic_buffer.size= 0;
 	topic_buffer.identity= "";
	topic_buffer.identity_size= 0;
	topic_buffer.topic= "";
	topic_buffer.topic_size= 0;

	if(options.rcv_topic){
		topic_buffer= recv_buffer(socket,errflag);
		if(topic_buffer.size==-1) {//failure in receive
			cerr<<"ZMQUtils::recv_message(): ERROR: Topic message recv failed"<<endl;			
			return buffer;
		}
		buffer.topic= topic_buffer.data;
		buffer.topic_size= topic_buffer.size;		
	}

	//## Receive message body
	ZMQUtils::SBuffer body_buffer;
	body_buffer.data= "";
	body_buffer.size= 0;
 	body_buffer.identity= "";
	body_buffer.identity_size= 0;
	body_buffer.topic= "";
	body_buffer.topic_size= 0;

	body_buffer= recv_buffer(socket,errflag);
	if(body_buffer.size==-1) {//failure in receive
		cerr<<"ZMQUtils::recv_message(): ERROR: Body message recv failed"<<endl;			
		return buffer;
	}
	
	buffer.data= body_buffer.data;
	buffer.size= body_buffer.size;
	
	return buffer;

}//close recv_message()


int ZMQUtils::send_buffer (void* socket,ZMQUtils::SBuffer buffer,bool sendmore){
	
	int rc= 0;

	//## Buffer data
	std::string buffer_data= buffer.data;
	int buffer_size= buffer.size;

	//## Create zmq message from buffer
	zmq_msg_t msg;
	zmq_msg_init_size(&msg,buffer_size);	
	memcpy(zmq_msg_data(&msg), buffer_data.c_str(), buffer_size);//replace this part with zero-copy for faster execution!
	
	//## Send buffer
	if(sendmore) rc= zmq_msg_send(&msg,socket,ZMQ_SNDMORE+ZMQ_DONTWAIT);
	else rc= zmq_msg_send(&msg,socket,ZMQ_DONTWAIT);		

	if(rc==-1) cerr<<"ZMQUtils::send_buffer(): ERROR: Send zmq message failed"<<endl;

	zmq_msg_close(&msg);

	return rc;

}//close send_buffer()

int ZMQUtils::send_message (void* socket,ZMQUtils::SBuffer buffer,ZMQSendOptions options) {

	int rc= 0;

	//## Send identity?
	if(options.send_identity){
		ZMQUtils::SBuffer identity_buffer;
		identity_buffer.data= options.identity;
		identity_buffer.size= (identity_buffer.data).length();
		identity_buffer.identity= "";
		identity_buffer.identity_size= 0;
		identity_buffer.topic= "";
		identity_buffer.topic_size= 0;

		rc= send_buffer(socket,identity_buffer,true);
		if(rc==-1){
			cerr<<"ZMQUtils::send_message(): ERROR: Send zmq identity buffer message failed"<<endl;
			return -1;
		}
	}	

	//## Send empty buffer?
	if(options.send_empty) {
		ZMQUtils::SBuffer empty_buffer;
		empty_buffer.data= "";
		empty_buffer.size= 0;
		empty_buffer.identity= "";
		empty_buffer.identity_size= 0;
		empty_buffer.topic= "";
		empty_buffer.topic_size= 0;

		rc= send_buffer(socket,empty_buffer,true);
		if(rc==-1){
			cerr<<"ZMQUtils::send_message(): ERROR: Send zmq empty buffer message failed"<<endl;
			return -1;
		}
	}

	//## Send topic?
	if(options.send_topic) {
		ZMQUtils::SBuffer topic_buffer;
		topic_buffer.data= options.topic;
		topic_buffer.size= (topic_buffer.data).length();
		topic_buffer.identity= "";
		topic_buffer.identity_size= 0;
		topic_buffer.topic= "";
		topic_buffer.topic_size= 0;

		rc= send_buffer(socket,topic_buffer,true);
		if(rc==-1){
			cerr<<"ZMQUtils::send_message(): ERROR: Send zmq topic buffer message failed"<<endl;
			return -1;
		}
	}

	//## Send message body	
	if(options.sendmore) rc= send_buffer(socket,buffer,true);
	else rc= send_buffer(socket,buffer,false);		
	
	if(rc==-1) cerr<<"ZMQUtils::send_message(): ERROR: Send zmq message failed"<<endl;
	
	return rc;

}//close send_message()


int ZMQUtils::proxy(void* frontend,void* backend,std::string identity){

	zmq_pollitem_t items[] = {
  	{ backend, 0, ZMQ_POLLIN, 0 },
    { frontend, 0, ZMQ_POLLIN, 0 }
  };

	Utils_ns::ZMQUtils::ZMQSendOptions SndOptions;
  SndOptions.send_empty= false;
  SndOptions.send_topic= false;
  SndOptions.topic= "";
  SndOptions.send_identity= true;
  SndOptions.sendmore= false;
  SndOptions.identity= identity;

	int errflag= 0;
	Utils_ns::ZMQUtils::ZMQRcvOptions RcvOptions;
  RcvOptions.rcv_empty= false;
  RcvOptions.rcv_topic= false;
  RcvOptions.rcv_identity= true;
  RcvOptions.rcvmore= false;
	
	bool isTerminated= false;

	while(!isTerminated){
		//Receive from dispatcher
    int poll_status = zmq_poll (items, 2, 100*ZMQ_POLL_MSEC);
    
    //## Check for poll return code and exit in case of failure
    if (poll_status == -1) {//poll returns with error
			cout << "ZMQUtils::proxy(): WARNING: Polling return failure (errno=" <<errno<<", description="<< zmq_strerror(errno)<<")";	
			if(errno==EINTR || errno==ETERM) {
        cout<<"ZMQUtils::proxy(): INFO: Context shut down, exit ..."<<endl;
				errflag= -1;
        break;
      }
      continue;
    }//close if
	
    //## Receive task from broker
    if (items[1].revents & ZMQ_POLLIN) {
			RcvOptions.rcv_identity= true;
      Utils_ns::ZMQUtils::SBuffer recvBuffer= Utils_ns::ZMQUtils::recv_message(frontend,RcvOptions,errflag);
 
      if(recvBuffer.data.empty() || recvBuffer.size==-1){
        cout << "ZMQUtils::proxy(): WARNING: Receive return failure (errno=" <<errflag<<", description="<< zmq_strerror(errflag)<<")"<<endl;
				continue;
      }

			if(recvBuffer.data=="#TERMINATE#" && recvBuffer.size==11) {
      	cout<<"ZMQUtils::proxy(): INFO: Stop thread signal received, ending thread!"<<endl;
      	isTerminated= true;

				//Reply to dispatcher	
				SndOptions.send_identity= true;
      	Utils_ns::ZMQUtils::send_message(frontend,recvBuffer,SndOptions);
				break; 
    	}

      cout<<"ZMQUtils::proxy(): INFO: Received request: "<<recvBuffer.data<<endl;
      
			//Reply to dispatcher	
			SndOptions.send_identity= true;
      Utils_ns::ZMQUtils::send_message(frontend,recvBuffer,SndOptions);
						
			//Send task to workers
			SndOptions.send_identity= false;
			Utils_ns::ZMQUtils::send_message(backend,recvBuffer,SndOptions);

		}//close if

		//##  Handle worker activity on backend
    if (items[0].revents & ZMQ_POLLIN) {	
			RcvOptions.rcv_identity= false; 
			Utils_ns::ZMQUtils::SBuffer recvBuffer= Utils_ns::ZMQUtils::recv_message(backend,RcvOptions,errflag);
 
      if(recvBuffer.data.empty() || recvBuffer.size==-1){
        cout<< "ZMQUtils::proxy(): WARNING: Receive return failure (errno=" <<errflag<<", description="<< zmq_strerror(errflag)<<")"<<endl;
				continue;
      }

			cout << "ZMQUtils::proxy(): INFO: Received response: "<<recvBuffer.data<<endl;			
		}//close if

	}//end forever loop

	return errflag;

}//clse proxy()

}//close namespace

