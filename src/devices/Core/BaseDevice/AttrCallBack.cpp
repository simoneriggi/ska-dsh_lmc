
#include <AttrCallBack.h>
#include <BaseDevice.h>

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


namespace BaseDevice_ns {


BaseDevice* AttrCallBack::device;
omni_mutex* AttrCallBack::mutex;
log4tango::Logger* AttrCallBack::logger;

AttrCallBack::AttrCallBack (BaseDevice* dev,omni_mutex* m) : Tango::LogAdapter(dev){
   
	mutex= m;
	device = dev;
	logger= Tango::LogAdapter(dev).get_logger();

}//close constructor
	

AttrCallBack::~AttrCallBack(){

}//destructor


void AttrCallBack::push_event(Tango::EventData* event){
  
  //## Timestamp event receipt
  //struct timeval timestamp_end;
  //gettimeofday(&timestamp_end, NULL);

	//## EventData info
  //[device] : The DeviceProxy object on which the call was executed (Tango::DeviceProxy *)
  //[attr_name] : The attribute name (std::string &)
  //[event] : The event name (std::string &)
  //[attr_value] : The attribute data (DeviceAttribute *)
  //[err] : A boolean flag set to true if the request failed. False otherwise (bool)
  //[errors] : The error stack (DevErrorList &)
   
	//Check event
	INFO_STREAM << "AttrCallBack::push_event(): INFO: Received event "<<event->event<<" for attr: "<<event->attr_name<<" (errflag="<<event->err<<")"<<endl; 
	Tango::Except::print_error_stack(event->errors);
	if (event->err) {
		ERROR_STREAM<<"AttrCallBack::push_event(): ERROR: Invalid event!"<<endl;
		return;
	}

	//Get access to event info
	int status= event->err;	
  std::string attr_full_name= event->attr_name;
	std::string event_name= event->event;
	

	//Get access to event attr info
	DeviceAttribute* device_attr= 0;
	Tango::TimeVal timestamp;
	std::string attr_name= "";
	int attr_type= -1;
	Tango::AttrQuality attr_quality;
  try { 
		device_attr= event->attr_value;
		timestamp= device_attr->get_date(); 
		attr_name= device_attr->get_name();
		attr_type= device_attr->get_type();
		attr_quality= device_attr->get_quality();

		INFO_STREAM << "AttrCallBack::push_event(): INFO: Event attr info: attr_name="<<attr_name<<", attr_type="<<attr_type<<", attr_quality="<<attr_quality<<", time=("<<timestamp.tv_sec<<","<<timestamp.tv_usec<<")"<<endl;
	}//close try block
	catch(Tango::DevFailed &e){
		Tango::Except::print_exception(e);
		ERROR_STREAM<<"AttrCallBack::push_event(): ERROR: Failed to retrieve event data!"<<endl;
		return;
	}
  catch(...){
		ERROR_STREAM<<"AttrCallBack::push_event(): ERROR: Failed to retrieve event data!"<<endl;
		return;
	}
		
	
	//## Update device attr from event
	if(UpdateDeviceAttr(event)<0){
		WARN_STREAM<<"AttrCallBack::push_event(): ERROR: Failed to update device attribute from event data!"<<endl;
	}
	
}//close AttrCallBack::push_event()


int AttrCallBack::UpdateDeviceAttr(Tango::EventData* event){

	if(!event) {
		WARN_STREAM<<"AttrCallBack::UpdateDeviceAttr(): WARN: Null ptr to given event!"<<endl;
		return -1;
	}

	DeviceAttribute* device_attr= event->attr_value;
	if(!device_attr) {
		WARN_STREAM<<"AttrCallBack::UpdateDeviceAttr(): WARN: Null ptr to event device attribute!"<<endl;
		return -1;
	}

	Tango::DeviceProxy* remote_device= event->device;
	if(!remote_device) {
		WARN_STREAM<<"AttrCallBack::UpdateDeviceAttr(): WARN: Null ptr to remote proxy device!"<<endl;
		return -1;
	}

	Tango::TimeVal timestamp= device_attr->get_date(); 
	std::string attr_name= device_attr->get_name();
	int attr_type= device_attr->get_type();
	Tango::AttrQuality attr_quality= device_attr->get_quality();
	


	std::string alias_attr_name= attr_name;
	/*
	//## Find attribute in moni point list
	size_t pos= -1;
	std::string alias_attr_name= "";
	bool isFound= false;
	
	try{	
		std::vector<SSD_ns::MoniPoint*>::iterator it = std::find_if((device->mpCollection).begin(), (device->mpCollection).end(),SSD_ns::MoniPointFinder(attr_name,false));
		isFound= (it!=device->mpCollection.end() && device->mpCollection.size()>0);
		if(!isFound){
			ERROR_STREAM<<"SEModuleAttrCallBack::UpdateDeviceAttr(): WARN: Cannot find event attr in moni point list!"<<endl;
			return -1;	
		}
		pos= (it - device->mpCollection.begin());
		alias_attr_name= (device->mpCollection)[pos]->alias_name; 
		INFO_STREAM<<"SEModuleAttrCallBack::UpdateDeviceAttr(): INFO: Found attr in device with alias name="<<alias_attr_name<<endl;
	}
	catch(...){
		ERROR_STREAM<<"SEModuleAttrCallBack::UpdateDeviceAttr(): ERROR: Failed to retrieve attr from moni point list!"<<endl;
		return -1;
	}
	*/

	//## Set device attribute value to event value
	try{
		
		switch( attr_type ){
			case Tango::DEV_FLOAT :
			{
				float event_attr_value;
    		*device_attr >> event_attr_value;	
				INFO_STREAM << "AttrCallBack::UpdateDeviceAttr(): INFO: Event attr info: attr_name="<<attr_name<<", attr_value="<<event_attr_value<<", attr_type="<<attr_type<<", attr_quality="<<attr_quality<<", time=("<<timestamp.tv_sec<<","<<timestamp.tv_usec<<")"<<endl;

				Tango::DevFloat* attr_value = device->get_dynFloatAttr_data_ptr(alias_attr_name);
				mutex->lock();
				*attr_value= event_attr_value;
				mutex->unlock();
				break;
			}
			case Tango::DEV_DOUBLE :
			{
				double event_attr_value;
     		*device_attr >> event_attr_value;
				INFO_STREAM << "AttrCallBack::UpdateDeviceAttr(): INFO: Event attr info: attr_name="<<attr_name<<", attr_value="<<event_attr_value<<", attr_type="<<attr_type<<", attr_quality="<<attr_quality<<", time=("<<timestamp.tv_sec<<","<<timestamp.tv_usec<<")"<<endl;

				//Tango::DevDouble	*attr_value = device->get_dynDoubleAttr_data_ptr(alias_attr_name);				
				//mutex->lock();
				//*attr_value= event_attr_value;
				//mutex->unlock();
				break;
			}
			case Tango::DEV_LONG :
			{
				Tango::DevLong event_attr_value;
     		*device_attr >> event_attr_value;
				INFO_STREAM << "AttrCallBack::UpdateDeviceAttr(): INFO: Long event attr: attr_name="<<attr_name<<", attr_value="<<event_attr_value<<", attr_type="<<attr_type<<", attr_quality="<<attr_quality<<", time=("<<timestamp.tv_sec<<","<<timestamp.tv_usec<<")"<<endl;

				Tango::DevLong	*attr_value = device->get_dynLongAttr_data_ptr(alias_attr_name);				
				mutex->lock();
				*attr_value= event_attr_value;
				mutex->unlock();
				break;
			}
			case Tango::DEV_SHORT :
			{
				short event_attr_value;
     		*device_attr >> event_attr_value;
				INFO_STREAM << "AttrCallBack::UpdateDeviceAttr(): INFO: Short event attr: attr_name="<<attr_name<<", attr_value="<<event_attr_value<<", attr_type="<<attr_type<<", attr_quality="<<attr_quality<<", time=("<<timestamp.tv_sec<<","<<timestamp.tv_usec<<")"<<endl;

				Tango::DevShort	*attr_value = device->get_dynShortAttr_data_ptr(alias_attr_name);				
				mutex->lock();
				*attr_value= event_attr_value;
				mutex->unlock();
				break;
			}
			case Tango::DEV_STRING :
			{
				std::string event_attr_value;
     		*device_attr >> event_attr_value;
				INFO_STREAM << "AttrCallBack::UpdateDeviceAttr(): INFO: String event attr: attr_name="<<attr_name<<", attr_value="<<event_attr_value<<", attr_type="<<attr_type<<", attr_quality="<<attr_quality<<", time=("<<timestamp.tv_sec<<","<<timestamp.tv_usec<<")"<<endl;

				//Tango::DevString	*attr_value = device->get_dynStringAttr_data_ptr(alias_attr_name);				
				//mutex->lock();
				//*attr_value= event_attr_value;
				//mutex->unlock();
				break;
			}
			case Tango::DEV_ENUM :
			{
				
				short event_attr_value;
     		*device_attr >> event_attr_value;
				
				INFO_STREAM << "AttrCallBack::UpdateDeviceAttr(): INFO: Enum event attr: attr_name="<<attr_name<<", attr_value="<<event_attr_value<<", attr_type="<<attr_type<<", attr_quality="<<attr_quality<<", time=("<<timestamp.tv_sec<<","<<timestamp.tv_usec<<")"<<endl;

				Tango::DevEnum* attr_value = device->get_dynEnumAttr_data_ptr(alias_attr_name);

				//--> Check the received enum code
				//Retrieve the enum property values of attribute of this device
				Tango::MultiAttrProp<Tango::DevEnum> multi_attr_prop;
				device->get_device_attr()->get_attr_by_name(alias_attr_name.c_str()).get_properties(multi_attr_prop);
				std::vector<std::string> attr_enum_labels= multi_attr_prop.enum_labels;

				//Retrieve the enum property values of attribute of remote device
				AttributeInfoEx attr_info= remote_device->get_attribute_config(attr_name);
				std::vector<std::string> remote_attr_enum_labels= attr_info.enum_labels;
 				std::string remote_attr_enum_label= remote_attr_enum_labels[event_attr_value];//need to check index as well (not done here)!
				std::vector<std::string>::iterator it= std::find(attr_enum_labels.begin(),attr_enum_labels.end(),remote_attr_enum_label);
				bool isValidEnum= (attr_enum_labels.size()>0 && it!=attr_enum_labels.end());
				if(isValidEnum){
					short enum_val= static_cast<short>(it- attr_enum_labels.begin());
					mutex->lock();
					*attr_value= enum_val;
					mutex->unlock();
				}
				else {
					TangoSys_OMemStream o;
					o<<"AttrCallBack::push_event(): Attempted to set invalid enum code  ("<<event_attr_value<<") to device attribute!"<<endl;
					Tango::Except::throw_exception("API_WrongData",o.str(),"SEModuleAttrCallBack::UpdateDeviceAttr");
				} 
			
				break;
			}
			case Tango::DEV_STATE :
			{
				Tango::DevState event_attr_value;
     		*device_attr >> event_attr_value;
				INFO_STREAM << "AttrCallBack::UpdateDeviceAttr(): INFO: State event attr: attr_name="<<attr_name<<", attr_value="<<event_attr_value<<", attr_type="<<attr_type<<", attr_quality="<<attr_quality<<", time=("<<timestamp.tv_sec<<","<<timestamp.tv_usec<<")"<<endl;

				//Tango::DevShort	*attr_value = device->get_dynShortAttr_data_ptr(alias_attr_name);				
				//mutex->lock();
				//*attr_value= event_attr_value;
				//mutex->unlock();
				break;
			}
			default : 
			{	
				TangoSys_OMemStream o;
				o<<"AttrCallBack::UpdateDeviceAttr()(): Unsupported data type ("<<attr_type<<")!"<<endl;
				Tango::Except::throw_exception("API_IncompatibleAttrArgumentType",o.str(),"SEModuleAttrCallBack::UpdateDeviceAttr");
				break;
			}
		}//close switch
	  
  }//close try 
	catch(Tango::DevFailed &e){
		Tango::Except::print_exception(e);
		ERROR_STREAM << "AttrCallBack::UpdateDeviceAttr(): ERROR: Failed to set attribute to event data!"<<endl; 
		return -1;
	}
  catch (...) {
    ERROR_STREAM << "AttrCallBack::UpdateDeviceAttr(): ERROR: Failed to set attribute to event data!"<<endl; 
		return -1;
  }

	return 0;

}//close AttrCallBack::UpdateDeviceAttr()



}//close namespace


