#include <MoniPoint.h>

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

namespace SDD_ns 
{


MoniPoint::MoniPoint() {

	//Init values	
	//--> General
	id= 0;
	has_description= false;
	description= "";
	name= "";
	alias_name= "";
	data_type= -1;
	data_format= -1;
	rw_type= Tango::READ;
	has_unit= false;
	unit= "";
	component_name= "";
	component_id= -1;

	device_address= "";

	//--> Data range
	has_data_range= false;
	has_alarm_range= false;
	min_data_value= 0;
	max_data_value= 0;
	min_data_alarm= 0;
	max_data_alarm= 0;	
	has_enum_labels= false;
	enum_labels.clear();
	
	//--> Polling
	is_polled= false;	
	poll_period= 3000;
	
	//--> Events
	is_subscribed= false;
	event_ids.clear();
	subscribe_events= false;
	has_events= false;
	has_rel_change_event= false;
	rel_change_value= 0;
	has_abs_change_event= false;
	abs_change_value= 0;
	has_push_change_event= false;
	has_periodic_event= false;
	event_period= 0;

}//close constructor


MoniPoint::~MoniPoint(){

}


int MoniPoint::ParseFromXML(pugi::xml_node mp,bool filter,std::string component_filter,int componentId_filter){
			
	//Get monitoring point name & alias from attributes
	pugi::xml_attribute name_attr= mp.attribute("name");
	if(name_attr.empty()){
		cerr<<"ParseFromXML(): WARN: Invalid monitoring point detected (missing name attr), skip it!"<<endl;
		return -1;
	}	
	name= name_attr.value();

	pugi::xml_attribute alias_attr= mp.attribute("alias");
	alias_name= name;
	if(!alias_attr.empty()){
		alias_name= alias_attr.value();
	}

	pugi::xml_attribute id_attr= mp.attribute("id");
	if(!id_attr.empty()){
		id= id_attr.as_int();
	}	
			
	//Get component info child
	pugi::xml_node componentTag = mp.child("Component");
	pugi::xml_node componentIdTag = mp.child("ComponentId");
	if(componentTag.empty() || componentIdTag.empty() ){
		cerr<<"ParseFromXML(): WARN: Missing component info ...skip it!"<<endl;
		return -1;
	}
	component_name= componentTag.text().as_string();
	component_id= componentIdTag.text().as_int();

	//Filter by component?
	if(filter && (component_name!=component_filter || component_id!=componentId_filter) ){
		cout<<"ParseFromXML(): INFO: Filter not passed..."<<endl;
		return -1;
	}
			
	//Get data type & format
	pugi::xml_node dataTypeTag= mp.child("DataType");
	pugi::xml_node dataFormatTag= mp.child("DataFormat");
	pugi::xml_node dataRWTypeTag= mp.child("RWType");
	if(dataTypeTag.empty()){
		cerr<<"ParseFromXML(): WARN: Missing data type info ...skip it!"<<endl;
		return -1;
	}
	data_type= GetAttrDataType(dataTypeTag.text().as_string());
	if(data_type<0){
		cerr<<"ParseFromXML: WARN: Failed to determine data type ...skip it!"<<endl;
		return -1;
	}
	
	if(dataFormatTag.empty()){
		cerr<<"ParseFromXML(): WARN: Missing data format info ...skip it!"<<endl;
		return -1;
	}
	data_format= GetAttrDataFormat(dataFormatTag.text().as_string());
		
	if(dataRWTypeTag.empty()){
		cerr<<"ParseFromXML(): WARN: Missing data rw type info ...skip it!"<<endl;
		return -1;
	}
	rw_type= GetAttrRWType(dataRWTypeTag.text().as_string());
			
	//Get description prperty
	if(!mp.child("Description").empty()) {
		has_description= true;
		description= mp.child("Description").text().as_string();
	}

	//Unit property
	pugi::xml_node unitTag= mp.child("Unit");
	has_unit= !unitTag.empty();
	if(has_unit) unit= unitTag.text().as_string(); 	

	//Value range property
	pugi::xml_node dataRangeTag= mp.child("ValueRange");
	bool hasDataRange= !dataRangeTag.empty();
	std::string min_value= "";
	std::string max_value= "";
	std::string min_alarm= "";
	std::string max_alarm= "";
	bool hasRangeValue= false;
	bool hasAlarmValue= false;
	if(hasDataRange){
		pugi::xml_node minValueTag= dataRangeTag.child("MinValue");
		pugi::xml_node maxValueTag= dataRangeTag.child("MaxValue");
		pugi::xml_node minAlarmTag= dataRangeTag.child("MinAlarm");
		pugi::xml_node maxAlarmTag= dataRangeTag.child("MaxAlarm");
		has_data_range= ( !minValueTag.empty() && !maxValueTag.empty() );
		has_alarm_range= ( !minAlarmTag.empty() && !maxAlarmTag.empty() );
				
		if(has_data_range){
			min_data_value= minValueTag.text().as_double();
			max_data_value= maxValueTag.text().as_double();
		}
		if(has_alarm_range){
			min_data_alarm= minAlarmTag.text().as_double();
			max_data_alarm= maxAlarmTag.text().as_double();
		}
	}//close has data range
			
	//Enum values?
	pugi::xml_node possibleValuesTag= mp.child("PossibleValues");
	bool hasPossibleValues= !possibleValuesTag.empty();
	
	if(hasPossibleValues){
		for (pugi::xml_node it = possibleValuesTag.child("Value"); it; it = it.next_sibling("Value")){
			std::string enum_label= it.text().as_string();
			enum_labels.push_back(enum_label);
		}//end loop enum labels
		if(enum_labels.size()>0) has_enum_labels= true;
	}//close if

	//Is Polled attr?
	pugi::xml_node pollingTag= mp.child("Polling");
	is_polled= !pollingTag.empty();
	bool hasPollingPeriod= false;
	if(is_polled){
		pugi::xml_node pollingPeriodTag= pollingTag.child("Period");
		hasPollingPeriod= !pollingPeriodTag.empty();
		if(hasPollingPeriod) poll_period= pollingPeriodTag.text().as_llong();
		else {
			cerr<<"ParseFromXML(): ERROR: Missing polling period!"<<endl;
			return -1;
		}
	}//close if
	
	//Get event info
	pugi::xml_node eventsTag= mp.child("Events");
	has_events= !eventsTag.empty();	
	bool hasRelChangeEvent= false;
	bool hasRelChangeValue= false;
	std::string relChangeValue= "";
	bool hasAbsChangeEvent= false;
	bool hasAbsChangeValue= false;
	std::string absChangeValue= "";
	bool hasPushChangeEvent= false;		
	bool hasPeriodicEvent= false;
	bool hasEventPeriod= false;
	std::string periodValue= "";
	if(has_events){
		pugi::xml_node relChangeEventTag= eventsTag.child("RelChangeEvent");
		has_rel_change_event= !relChangeEventTag.empty();
		if(has_rel_change_event){
			pugi::xml_node relChangeValueTag= relChangeEventTag.child("ChangeValue");
			hasRelChangeValue= !relChangeValueTag.empty();
			if(hasRelChangeValue) {
				rel_change_value= relChangeValueTag.text().as_double();	
			}
			else{
				cerr<<"ParseFromXML(): ERROR: Missing rel_change_value!"<<endl;
				return -1;
			}
		}//close if rel change

		pugi::xml_node absChangeEventTag= eventsTag.child("AbsChangeEvent");
		has_abs_change_event= !absChangeEventTag.empty();
		if(has_abs_change_event){
			pugi::xml_node absChangeValueTag= absChangeEventTag.child("ChangeValue");
			hasAbsChangeValue= !absChangeValueTag.empty();
			if(hasAbsChangeValue) {
				abs_change_value= absChangeValueTag.text().as_double();
			}
			else{
				cerr<<"ParseFromXML(): ERROR: Missing abs_change_value!"<<endl;
				return -1;
			}
		}//close if abs change
			
		pugi::xml_node periodicEventTag= eventsTag.child("PeriodicEvent");
		has_periodic_event= !periodicEventTag.empty();
		if( has_periodic_event){
			pugi::xml_node periodValueTag= periodicEventTag.child("Period");
			hasEventPeriod= !periodValueTag.empty();
			if(hasEventPeriod) {
				event_period= periodValueTag.text().as_double();
			}
			else {
				cerr<<"ParseFromXML(): ERROR: Missing period value!"<<endl;
				return -1;
			}
		}	

		pugi::xml_node pushChangeEventTag= eventsTag.child("PushEvent");
		has_push_change_event= !pushChangeEventTag.empty();
	}//close if has events
			
	return 0;

}//close ParseFromXML()

		
void MoniPoint::DumpProperties(){
	/*
	if(!multi_attr_prop) return;
			cout<<"*** MP PROPERTIES ***"<<endl;
			cout<<"description: "<<multi_attr_prop->description<<endl;
			cout<<"unit: "<<multi_attr_prop->unit<<endl;
			cout<<"range: ("<<multi_attr_prop->min_value<<","<<multi_attr_prop->max_value<<")"<<endl;
			cout<<"alarm: ("<<multi_attr_prop->min_alarm<<","<<multi_attr_prop->max_alarm<<")"<<endl;
			if(data_type==Tango::DEV_ENUM){
				cout<<"enum_labels(";
				for(unsigned int i=0;i<(multi_attr_prop->enum_labels).size();i++){
					cout<<(multi_attr_prop->enum_labels)[i]<<",";
				}
				cout<<")"<<endl;
			}
			if(has_periodic_event) cout<<"event_period: "<<multi_attr_prop->event_period<<endl;
			if(has_abs_change_event) cout<<"abs_change: "<<multi_attr_prop->abs_change<<endl;
			if(has_rel_change_event) cout<<"rel_change: "<<multi_attr_prop->rel_change<<endl;
			cout<<"*********************"<<endl;
	*/
}//close DumpProperties()	

int MoniPoint::GetAttrDataFormat(std::string dataFormatTag){
	long data_format= 1;
	if(dataFormatTag=="SCALAR") data_format= 1;
	else if(dataFormatTag=="SPECTRUM") data_format= 2;
	else if(dataFormatTag=="IMAGE") data_format= 3;
	return data_format;
}//close GetAttrDataFormat()

Tango::AttrWriteType MoniPoint::GetAttrRWType(std::string dataRWTag){			
	Tango::AttrWriteType data_rw_type= Tango::READ;
	if(dataRWTag=="READ") data_rw_type= Tango::READ;
	else if(dataRWTag=="WRITE") data_rw_type= Tango::WRITE;
	else if(dataRWTag=="READ_WRITE") data_rw_type= Tango::READ_WRITE;
	return data_rw_type;
}//close GetAttrRWType()

long MoniPoint::GetAttrDataType(std::string dataTypeTag){
	long int data_type= -1;
	if(dataTypeTag=="DOUBLE") data_type= static_cast<long int>(Tango::DEV_DOUBLE);
	else if(dataTypeTag=="FLOAT") data_type= static_cast<long int>(Tango::DEV_FLOAT);
	else if(dataTypeTag=="SHORT") data_type= static_cast<long int>(Tango::DEV_SHORT);
	else if(dataTypeTag=="LONG") data_type= static_cast<long int>(Tango::DEV_LONG);	
	else if(dataTypeTag=="ENUM") data_type= static_cast<long int>(Tango::DEV_ENUM);
	return data_type;
}//close SetAttrDataType()

void MoniPoint::Dump(){		
			
	cout<<"*** MONI POINT ID "<<id<<" ***"<<endl;
	cout<<"device_address: "<<device_address<<" component_name="<<component_name<<" component_id="<<component_id<<endl;
	cout<<"name: "<<name<<" alias: "<<alias_name<<endl;
	cout<<"data_type: "<<data_type<<" rw_type: "<<rw_type<<" data_format: "<<data_format<<endl;
	cout<<"events? "<<has_events<<" ";
	if(has_rel_change_event) cout<<" (REL_CHANGE x="<<rel_change_value<<") ";
	if(has_abs_change_event) cout<<" (ABS_CHANGE x="<<abs_change_value<<") ";
	if(has_periodic_event) cout<<" (PERIODIC t="<<event_period<<") ";
	if(has_push_change_event) cout<<" (PUSH_CHANGE) ";
	cout<<endl;
	cout<<"******************************"<<endl;

}//close Dump()


}//close namespace
