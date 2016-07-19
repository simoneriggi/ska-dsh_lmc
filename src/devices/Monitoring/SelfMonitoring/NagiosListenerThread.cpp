#include <NagiosListenerThread.h>
#include <SelfMonitoring.h>
#include <ZMQUtils.h>
#include <SocketUtils.h>

#include <tango.h>

//## Boost
#include <boost/regex.hpp>

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
#include <regex>

using namespace std;


namespace SelfMonitoring_ns {


SelfMonitoring* NagiosListenerThread::device;
log4tango::Logger* NagiosListenerThread::logger;

NagiosListenerThread::NagiosListenerThread (SelfMonitoring* dev,void* ctx) : Tango::LogAdapter(dev), omni_thread() {

  device = dev;
  logger= Tango::LogAdapter(dev).get_logger();
 	m_ZmqContext= ctx;
	m_SocketClient= 0;

}//close constructor
  
NagiosListenerThread::~NagiosListenerThread(){
	
	
	if(m_SocketClient){
		DEBUG_STREAM<<"NagiosListenerThread::~NagiosListenerThread(): Deleting unix socket client..."<<endl;
		delete m_SocketClient;
		m_SocketClient= 0;
	}

	if(m_ZmqSocket) {
		DEBUG_STREAM<<"NagiosListenerThread::~NagiosListenerThread(): Closing zmq socket..."<<endl;
		zmq_close (m_ZmqSocket);
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
void* NagiosListenerThread::run_undetached(void*) {

  INFO_STREAM << "NagiosListenerThread::run_undetached(): INFO: Starting listener thread..."<<endl;

	//## Init socket client and buffer
	m_SocketClient= new Utils_ns::SocketClient(device->nagios_query_handler,false);
	if(m_SocketClient->Init()<0){
		ERROR_STREAM<<"NagiosListenerThread::run_undetached(): ERROR: Failed to init socket to talk to Nagios!"<<endl;
	}
	int bufsize= 8192;
	char buffer[8192];
	int timeout= 1;

	//## Init ZMQ socket to talk to main thread loop
	//m_ZmqSocket = zmq_socket (m_ZmqContext, ZMQ_REP);
	m_ZmqSocket = zmq_socket (m_ZmqContext, ZMQ_ROUTER);

	char* identity= "NagiosListenerThread";
	zmq_setsockopt (m_ZmqSocket, ZMQ_IDENTITY, identity, strlen (identity));

	int val = 0;//in ms (0=destroy messages immediately, -1=wait foreever)
  int rc = zmq_setsockopt(m_ZmqSocket, ZMQ_LINGER, &val, sizeof(val));
  zmq_bind (m_ZmqSocket, "inproc://dispatcher");
	
	zmq_pollitem_t items[] = {
  	{ m_ZmqSocket, 0, ZMQ_POLLIN, 0 }
  };
	std::stringstream sstream;

	int errflag= 0;
	Utils_ns::ZMQUtils::ZMQRcvOptions RcvOptions;
  RcvOptions.rcv_empty= false;
  RcvOptions.rcv_topic= false;
  //RcvOptions.rcv_identity= false;
	RcvOptions.rcv_identity= true;
  RcvOptions.rcvmore= false;

	
	std::string client_identity= "";
	Utils_ns::ZMQUtils::ZMQSendOptions SndOptions;
  SndOptions.send_empty= false;
  SndOptions.send_topic= false;
  SndOptions.topic= "";
  SndOptions.send_identity= true;
  SndOptions.sendmore= false;
  SndOptions.identity= client_identity;
	

	//## Read loop
	while(1) {
		//================================================
		//==  Check if thread stop flag is set
		//================================================
		if(device->m_StopThreadFlag) {
      DEBUG_STREAM<<"NagiosListenerThread::run_undetached(): DEBUG: Stop thread signal received, ending thread!"<<endl;
			break; 
    }

		//=====================================================
		//==  Check if there are messages from main thread
		//=====================================================
		int poll_status = zmq_poll (items, 1, 100*ZMQ_POLL_MSEC);
    if (poll_status == -1) {//poll returns with error
			sstream.str(std::string());
      sstream << "NagiosListenerThread::run_undetached(): WARNING: Polling return failure (errno=" <<errno<<", description="<< zmq_strerror(errno)<<")";	
			logger->debug(sstream.str().c_str());
      if(errno==EINTR || errno==ETERM) {
        DEBUG_STREAM<<"NagiosListenerThread::run_undetached(): INFO: Context shut down, exit ..."<<endl;
        break;
      }
      continue;
    }//close if

		if (items[0].revents & ZMQ_POLLIN) {	
			Utils_ns::ZMQUtils::SBuffer recvBuffer= Utils_ns::ZMQUtils::recv_message(m_ZmqSocket,RcvOptions,errflag);
 
      if(recvBuffer.data.empty() || recvBuffer.size==-1){
        sstream.str(std::string());
        sstream << "NagiosListenerThread::run_undetached(): WARNING: Receive return failure (errno=" <<errflag<<", description="<< zmq_strerror(errflag)<<")";
				logger->warn(sstream.str().c_str());
				continue;
      }

			sstream.str(std::string());
      sstream << "NagiosListenerThread::run_undetached(): INFO: Received message from main thread: "<<recvBuffer.data<<endl;
      logger->info(sstream.str().c_str());

			client_identity= recvBuffer.identity;

			//=========================================
			//==   Write cmd to Unix Nagios socket
			//=========================================
			DEBUG_STREAM<<"NagiosListenerThread::run_undetached(): INFO: Sending list request to Nagios NERD..."<<endl;
			ssize_t recvBytes= m_SocketClient->Write((recvBuffer.data).c_str(),recvBuffer.size);
			if(recvBytes<0){
				if(recvBytes!=Utils_ns::SocketClient::eConnectionTimedOut){
					ERROR_STREAM<<"NagiosListenerThread::run_undetached(): ERROR: Failed to write to socket, close and reopening socket..."<<endl;
					m_SocketClient->Reset();
				}
					
				//Send back reply to dispatcher
				Utils_ns::ZMQUtils::SBuffer reply;
				reply.data= "NACK";
				reply.size= 4;
				reply.identity= client_identity;	
				SndOptions.identity= client_identity;
				Utils_ns::ZMQUtils::send_message(m_ZmqSocket,reply,SndOptions);
			}//close if check recv
		}//close if zmq poll
	
		//=========================================
		//===      Read Unix Nagios socket
		//=========================================
		ssize_t nbytes= m_SocketClient->Read(buffer,bufsize,timeout);
		if(nbytes<0){
			ERROR_STREAM<<"NagiosListenerThread::run_undetached(): ERROR: Failed to read from socket..."<<endl;
			if(nbytes!=Utils_ns::SocketClient::eConnectionTimedOut){
				ERROR_STREAM<<"NagiosListenerThread::run_undetached(): ERROR: Failed to read from socket, close and reopening socket..."<<endl;
				m_SocketClient->Reset();
			}
			continue;
		}
		INFO_STREAM<<"NagiosListenerThread::run_undetached(): INFO: Received message (n="<<nbytes<<"): "<< buffer <<endl;
		
		//=========================================
		//===    Process received message 
		//=========================================
		try{
			m_eventBuffer.data= std::string(buffer,nbytes);
		}
		catch (std::exception const &e) {
  		ERROR_STREAM<<"NagiosListenerThread::run_undetached(): ERROR: Exception caught (e="<<e.what()<<")"<<endl;
			continue;
  	}
  	catch (...) {
    	ERROR_STREAM<<"NagiosListenerThread::ProcessEvent(): ERROR: Unknown Exception caught!"<<endl;
			continue;
 	 	}
		m_eventBuffer.size= nbytes;
		m_eventBuffer.identity= client_identity;
		ProcessEvent(m_eventBuffer);

	}//end foreever loop
	
	      
	INFO_STREAM << "NagiosListenerThread::run_undetached(): INFO: End listener thread..."<<endl;

	//Close zmq socket
	if(m_ZmqSocket) {
		DEBUG_STREAM<<"NagiosListenerThread::run_undetached(): INFO: Closing zmq socket..."<<endl;
		zmq_close (m_ZmqSocket);
	}

  return 0;

}//close run()


int NagiosListenerThread::ProcessEvent(Utils_ns::ZMQUtils::SBuffer& sbuffer){

	//localhost from 0 -> 0: PING OK - Packet loss = 0%, RTA = 0.03 ms|rta=0.034000ms;3000.000000;5000.000000;0.000000 pl=0%;80;100;0

	DEBUG_STREAM<<"NagiosListenerThread::ProcessEvent(): INFO: Processing event (n="<<sbuffer.size<<",id="<<sbuffer.identity<<"): "<<sbuffer.data<<endl;
	boost::regex serviceCheckExpr("(.*?);(.*?) from ([0-9]+) -> ([0-9]+): (.*)$");
	boost::regex hostCheckExpr("(.*?) from ([0-9]+) -> ([0-9]+): (.*)$");
	boost::regex cmdErrorExpr("([0-9]+): (.*)$");
	boost::match_results<std::string::const_iterator> results;

	
	Utils_ns::ZMQUtils::ZMQSendOptions SndOptions;
  SndOptions.send_empty= false;
  SndOptions.send_topic= false;
  SndOptions.topic= "";
  SndOptions.send_identity= true;
  SndOptions.sendmore= false;
  SndOptions.identity= sbuffer.identity;
	

	if( boost::regex_match(sbuffer.data, results, serviceCheckExpr) ){//Service Check data received
		DEBUG_STREAM<<"NagiosListenerThread::ProcessEvent(): INFO: Service Check data received..."<<endl;
		std::string parsedData= results[0].str();
		std::string serviceHost= results[1].str();
		std::string serviceName= results[2].str();
		int parsedOldState= atoi(results[3].str().c_str());
		int parsedState= atoi(results[4].str().c_str());
		int serviceOldState= SelfMonitoring::UNKNOWN;
		int serviceState= SelfMonitoring::UNKNOWN;
		switch(parsedOldState){
			case SelfMonitoring::OK :
				serviceOldState= SelfMonitoring::OK;
				break;
			case SelfMonitoring::WARNING :
				serviceOldState= SelfMonitoring::WARNING;
				break;
			case SelfMonitoring::CRITICAL :
				serviceOldState= SelfMonitoring::CRITICAL;
				break;
			default:
				serviceOldState= SelfMonitoring::UNKNOWN;
				break;
		}//close switch

		switch(parsedState){
			case SelfMonitoring::OK :
				serviceState= SelfMonitoring::OK;
				break;
			case SelfMonitoring::WARNING :
				serviceState= SelfMonitoring::WARNING;
				break;
			case SelfMonitoring::CRITICAL :
				serviceState= SelfMonitoring::CRITICAL;
				break;
			default:
				serviceState= SelfMonitoring::UNKNOWN;
				break;
		}//close switch

		bool hasChangedState= (serviceState!=serviceOldState);
		std::string serviceCheckData= results[5].str();
		
		DEBUG_STREAM<<"NagiosListenerThread::ProcessEvent(): INFO: Host="<<serviceHost<<", Service="<<serviceName<<", State("<<serviceOldState<<"-->"<<serviceState<<"), CheckData="<<serviceCheckData<<endl;

		//Send back reply to dispatcher
		Utils_ns::ZMQUtils::SBuffer reply;
		reply.data= "subscribe servicechecks OK";
		reply.size= 26;
		reply.identity= sbuffer.identity;
		Utils_ns::ZMQUtils::send_message(m_ZmqSocket,reply,SndOptions);

		
	}//close if service check
	else if( boost::regex_match(sbuffer.data, results, hostCheckExpr) ){//Host Check data received
		DEBUG_STREAM<<"NagiosListenerThread::ProcessEvent(): INFO: Host Check data received..."<<endl;
		std::string parsedData= results[0].str();
		std::string Host= results[1].str();
		int parsedOldState= atoi(results[2].str().c_str());
		int parsedState= atoi(results[3].str().c_str());
		int hostOldState= SelfMonitoring::UNREACHABLE;
		int hostState= SelfMonitoring::UNREACHABLE;
		switch(parsedOldState){
			case SelfMonitoring::UP :
				hostOldState= SelfMonitoring::UP;
				break;
			case SelfMonitoring::DOWN :
				hostOldState= SelfMonitoring::DOWN;
				break;
			default:
				hostOldState= SelfMonitoring::UNREACHABLE;
				break;
		}//close switch

		switch(parsedState){
			case SelfMonitoring::UP :
				hostState= SelfMonitoring::UP;
				break;
			case SelfMonitoring::DOWN :
				hostState= SelfMonitoring::DOWN;
				break;
			default:
				hostState= SelfMonitoring::UNREACHABLE;
				break;
		}//close switch

		bool hasChangedState= (hostState!=hostOldState);
		std::string hostCheckData= results[4].str();
		
		DEBUG_STREAM<<"NagiosListenerThread::ProcessEvent(): INFO: Host="<<Host<<", State("<<hostOldState<<"-->"<<hostState<<"), CheckData="<<hostCheckData<<endl;
		
		
	}//close else if Host check
	else if(boost::regex_match(sbuffer.data, results, cmdErrorExpr)){
		DEBUG_STREAM<<"NagiosListenerThread::ProcessEvent(): INFO: Error message received..."<<endl;
		std::string parsedData= results[0].str();
		int ErrorCode= atoi(results[1].str().c_str());
		std::string ErrorMsg= results[2].str();
		DEBUG_STREAM<<"NagiosListenerThread::ProcessEvent(): INFO: ErrorCode="<<ErrorCode<<", ErrorMsg("<<ErrorMsg<<endl;
		
		//Send back reply to dispatcher
		Utils_ns::ZMQUtils::SBuffer reply;
		reply.data= parsedData;
		reply.size= parsedData.size();
		reply.identity= sbuffer.identity;
		Utils_ns::ZMQUtils::send_message(m_ZmqSocket,reply,SndOptions);

	}//close else if Error
	else{
		//Other data received, send them back to dispatcher
		DEBUG_STREAM<<"NagiosListenerThread::ProcessEvent(): INFO: Send back data to dispatcher..."<<endl;
		Utils_ns::ZMQUtils::send_message(m_ZmqSocket,sbuffer,SndOptions);
	}


	return 0;

}//close ProcessEvent()


}//close namespace

