
#include <TaskCallBack.h>
#include <Scheduler.h>
#include <Message.h>

#include <TaskManager.h>

#include <tango.h>


//## Standard headers
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <signal.h>
#include <ctime>
#include <stdexcept>
#include <stdlib.h>

#include <ratio>
#include <chrono>
#include <map>
#include <vector>

using namespace std;


namespace Scheduler_ns {


Scheduler* TaskCallBack::device;
log4tango::Logger* TaskCallBack::logger;

TaskCallBack::TaskCallBack (Scheduler* dev) : Tango::LogAdapter(dev){
   
	device = dev;
	logger= Tango::LogAdapter(dev).get_logger();
	m_TaskResponse= std::make_shared<MessageParser_ns::Response>();

}//close constructor
	

TaskCallBack::~TaskCallBack(){

}//destructor

void TaskCallBack::cmd_ended(Tango::CmdDoneEvent* event){
	/*
	[device] : The DeviceProxy object on which the call was executed (Tango::DeviceProxy *)
	[cmd_name] : The command name (string &)
	[argout] : The command argout (DeviceData &)
	[err] : A boolean flag set to true if the command failed. False otherwise (bool)
	[errors] : The error stack (DevErrorList &)
	*/

	if(!event){
		INFO_STREAM << "TaskCallBack::cmd_ended(): INFO: Received nullptr CmdDoneEvent!"<<endl; 
		return;
	}

	//Check event
	try {
		INFO_STREAM << "TaskCallBack::cmd_ended(): INFO: Received cmd_end_event for command "<<event->cmd_name<<" (errflag="<<event->err<<")"<<endl; 
		if (event->err) {	
			Tango::Except::print_error_stack(event->errors);	
		}
	}
	catch(Tango::DevFailed& e){
		Tango::Except::print_exception(e);
		ERROR_STREAM<<"TaskCallBack::cmd_ended(): ERROR: Failed to check event data!"<<endl;
		return;
	}
	catch(...){
		ERROR_STREAM<<"TaskCallBack::cmd_ended(): ERROR: Unknown exception occurred while checking event data!"<<endl;
		return;
	}

	//Get access to event info
	std::string cmd_name= "";
	Tango::DeviceData cmd_argout_data;
	try {
		INFO_STREAM << "TaskCallBack::cmd_ended(): INFO: Access to event info..."<<endl; 
		cmd_name= event->cmd_name;
		cmd_argout_data= event->argout;
	}
	catch(Tango::DevFailed& e){
		Tango::Except::print_exception(e);
		ERROR_STREAM<<"TaskCallBack::cmd_ended(): ERROR: Failed to access to event data!"<<endl;
		return;
	}	
	catch(...){
		ERROR_STREAM<<"TaskCallBack::cmd_ended(): ERROR: Unknown exception occurred while accessing to event data!"<<endl;
		return;
	}

	INFO_STREAM << "TaskCallBack::cmd_ended(): INFO: cmd_name:"<<cmd_name<<endl; 
	
	//Extract cmd argout data
	std::string cmd_argout= "";	
	try{
		INFO_STREAM << "TaskCallBack::cmd_ended(): INFO: Extract cmd argout data..."<<endl; 
		cmd_argout_data>>cmd_argout;
	}
	catch(Tango::DevFailed& e){
		Tango::Except::print_exception(e);
		ERROR_STREAM<<"TaskCallBack::cmd_ended(): ERROR: Failed to extract cmd argout data!"<<endl;
		return;
	}
	catch(...){
		ERROR_STREAM<<"TaskCallBack::cmd_ended(): ERROR: Unknown exception occurred while extracting cmd argout data!"<<endl;
		return;
	}

	
	//Parse argout data to Response
	//if(m_TaskResponse->Parse(cmd_argout)<0){
	if(MessageParser_ns::MessageUtils::ParseResponse(*m_TaskResponse,cmd_argout)<0){
		ERROR_STREAM<<"TaskCallBack::cmd_ended(): ERROR: Failed to parse argout data to Response (malformed response?)!"<<endl;
		return;
	}

	//Get access to response fields and access to task list
	int index= -1;
	std::string cmd_id= m_TaskResponse->GetId();
	if(!((device->m_TaskManager)->FindTask(index,cmd_id,cmd_name)) ){
		ERROR_STREAM << "TaskCallBack::cmd_ended(): ERROR: Failed to find task (name="<<cmd_name<<", id="<<cmd_id<<") in list"<<endl;
		return;
	}
	
	//Update cmd status in task list
	if(event->err){
		(device->m_TaskManager)->SetTaskStatus(index,eCOMPLETED,eFAILED,false);
	}
	else {
		(device->m_TaskManager)->SetTaskStatus(index,eCOMPLETED,eSUCCESS,false);
	}

	// Set a response pipe and push event
	(device->m_mutex)->lock();
	*(device->attr_finalResponse_read)= CORBA::string_dup(cmd_argout.c_str());
	device->push_change_event ("FinalResponse", device->attr_finalResponse_read);
	(device->m_mutex)->unlock();
	


	
	// Set a response pipe and push event
	(device->m_mutex)->lock();
	try {	
		INFO_STREAM<<"TaskCallBack::cmd_ended(): INFO: Encoding response msg to pipe..."<<endl;
		if(m_TaskResponse->GetPipeBlob((device->m_FinalResponsePipeBlob))<0){
			throw std::runtime_error("Failed to encode response message to Tango pipe");
		}	
	
		INFO_STREAM<<"TaskCallBack::cmd_ended(): INFO: Setting device pipe from blob..."<<endl;
		(device->m_FinalResponsePipeBlob).set_name("FinalResponsePipeBlob");
		INFO_STREAM<<"TaskCallBack::cmd_ended(): INFO: after pipe blob set"<<endl;
			
		//Push event pipe
		device->push_pipe_event	("FinalResponsePipe",&(device->m_FinalResponsePipeBlob),true);
	
	}
	catch(Tango::DevFailed& e){
		Tango::Except::print_exception(e);
		ERROR_STREAM << "TaskCallBack::cmd_ended(): ERROR: Tango exception occurred while setting pipe!"<<endl;	
	}
	catch(std::exception &e){
		ERROR_STREAM << "TaskCallBack::cmd_ended(): ERROR: Run time exception occurred (e="<<e.what()<<") while setting pipe!"<<endl;	
	}
	catch(...){
		ERROR_STREAM << "TaskCallBack::cmd_ended(): ERROR: Unknown exception occurred while setting pipe!"<<endl;	
	}

	
	(device->m_mutex)->unlock();
	



	/*
	// Set a dummy pipe
	(device->m_mutex)->lock();
	try {	
		INFO_STREAM<<"TaskCallBack::cmd_ended(): INFO: Retrieve pipe from device class..."<<endl;
		Tango::Pipe& pipe= device->get_device_class()->get_pipe_by_name("myPipe");//guess this will throw a Tango exception if pipe does not exist on device
		
		INFO_STREAM<<"TaskCallBack::cmd_ended(): INFO: Setting device pipe from blob..."<<endl;
	
		pipe.set_root_blob_name("myPipe");
		std::vector<std::string> de_names {"x","y"};
		pipe.set_data_elt_names(de_names);

		device->x= 4.5;
		device->y= 6.7;
		pipe << device->x << device->y;
		INFO_STREAM<<"TaskCallBack::cmd_ended(): INFO: after pipe set"<<endl;
	}
	catch(Tango::DevFailed& e){
		Tango::Except::print_exception(e);
		ERROR_STREAM << "TaskCallBack::cmd_ended(): ERROR: Tango exception occurred while setting pipe!"<<endl;	
	}
	catch(std::exception &e){
		ERROR_STREAM << "TaskCallBack::cmd_ended(): ERROR: Run time exception occurred (e="<<e.what()<<") while setting pipe!"<<endl;	
	}
	catch(...){
		ERROR_STREAM << "TaskCallBack::cmd_ended(): ERROR: Unknown exception occurred while setting pipe!"<<endl;	
	}
	(device->m_mutex)->unlock();
	*/


	
	/*
	// Set a dummy pipe
	(device->m_mutex)->lock();
	try {	
		INFO_STREAM<<"TaskCallBack::cmd_ended(): INFO: Retrieve pipe from device class..."<<endl;
		//Tango::Pipe& pipe= device->get_device_class()->get_pipe_by_name("myPipe");//guess this will throw a Tango exception if pipe does not exist on device
		
		INFO_STREAM<<"TaskCallBack::cmd_ended(): INFO: Setting device pipe from blob..."<<endl;
	
		//Set blob
		(device->myPipeBlob).set_name("myPipeBlob");
		std::vector<std::string> de_names {"x","y"};
		(device->myPipeBlob).set_data_elt_names(de_names);

		device->x= 4.5;
		device->y= 6.7;
		(device->myPipeBlob) << device->x << device->y;
		INFO_STREAM<<"TaskCallBack::cmd_ended(): INFO: after pipe blob set"<<endl;

		//Set pipe
		//pipe.set_root_blob_name("myPipe");
		//std::vector<std::string> blob_names {"myPipeBlob"};
		//pipe.set_data_elt_names(blob_names);
		//pipe << device->myPipeBlob;
		//INFO_STREAM<<"TaskCallBack::cmd_ended(): INFO: after pipe set"<<endl;
	}
	catch(Tango::DevFailed& e){
		Tango::Except::print_exception(e);
		ERROR_STREAM << "TaskCallBack::cmd_ended(): ERROR: Tango exception occurred while setting pipe!"<<endl;	
	}
	catch(std::exception &e){
		ERROR_STREAM << "TaskCallBack::cmd_ended(): ERROR: Run time exception occurred (e="<<e.what()<<") while setting pipe!"<<endl;	
	}
	catch(...){
		ERROR_STREAM << "TaskCallBack::cmd_ended(): ERROR: Unknown exception occurred while setting pipe!"<<endl;	
	}

	//Push pipe event
	device->push_pipe_event	("myPipe",&(device->myPipeBlob),true);

	(device->m_mutex)->unlock();
	*/
		

}//close cmd_ended()


}//close namespace


