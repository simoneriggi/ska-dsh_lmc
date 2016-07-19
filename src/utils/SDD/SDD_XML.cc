#include <SDD_XML.h>

#include <CodeUtils.h>

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


SDD_XML::SDD_XML(){

}

SDD_XML::SDD_XML(std::string filename) 
	: SDD(filename)
{

}


SDD_XML::~SDD_XML(){

}

int SDD_XML::readFile(){

	//## Open file and load to doc object
	if ( !fDoc.load_file(fFileName.c_str(),pugi::parse_trim_pcdata) ) {
		cerr<<"SDD::parse(): ERROR: Reading input file failed!"<<endl;
		return -1;
	}

	return 0;

}//close readFile()

int SDD_XML::ReadFromFile(pugi::xml_document& doc,std::string& filename){

	//## Open file and load to doc object
	pugi::xml_parse_result result= doc.load_file(filename.c_str(),pugi::parse_trim_pcdata);
	if (!result) {
		cerr<<"SDD::ReadFromFile(): ERROR: Reading input file failed (status="<<result.description()<<")"<<endl;
		return -1;
	}
	
	return 0;

}//close ReadFile()

int SDD_XML::ReadFromString(pugi::xml_document& doc,std::string& input_string){
	
	std::ifstream sstream(input_string);	
	pugi::xml_parse_result result= doc.load(sstream);
	if (!result) {
		cerr<<"SDD::ReadFromString(): ERROR: Reading input file failed (status="<<result.description()<<")"<<endl;
		return -1;
	}
	return 0;

}//close ReadFromString()


int SDD_XML::Parse(std::vector<DeviceAttr*>& attrCollection,std::string& input,bool readFromFile){
//int SDD_XML::Parse(std::vector<Tango::Attribute*>& attrCollection,std::string& input,bool readFromFile,std::string dev_name){

	//===========================================
	//==     READ
	//===========================================
	pugi::xml_document doc;
	int status= -1;
	if(readFromFile){//read as file
		status= ReadFromFile(doc,input);
	}
	else{//Read as string
		status= ReadFromString(doc,input);
	}
	if(status<0){
		cerr<<"SDD_XML::Parse(): ERROR: Failed to read XML tree!"<<endl;
		return -1;
	}

	//===========================================
	//==    LOAD MONI POINTS
	//===========================================
	//--> Check if MonitoringPointsList tag is present
	std::string topTag= "DeviceConfiguration";
	std::string mpListRootTag= "MonitoringPointsList";
	std::string mpRootTag= "MonitoringPoint";

	pugi::xml_node rootNode= doc.child(topTag.c_str());
	if(rootNode.empty()){
		cerr<<"SDD_XML::Parse(): ERROR: Failed to parse document (missing "<<topTag<<" tag)!"<<endl;
		return -1;
	}

	pugi::xml_node mpRoot = rootNode.child(mpListRootTag.c_str());
	if(mpRoot.empty()){
		cerr<<"SDD_XML::Parse(): ERROR: Failed to parse document (missing "<<mpListRootTag<<" tag)!"<<endl;
		return -1;
	}

	//--> Iterate over monitoring points list
	int nMPs= 0;
	
	for (pugi::xml_node mp = mpRoot.child(mpRootTag.c_str()); mp; mp = mp.next_sibling(mpRootTag.c_str())){
   
		//Parse attr
		DeviceAttr* aDeviceAttr= ParseAttrNode(mp);
		//Tango::Attribute* aDeviceAttr= ParseAttrNode(mp,dev_name);		
		if(!aDeviceAttr){
			cerr<<"SDD_XML::Parse(): WARN: Failed to parse moni point from XML node...skip it!"<<endl;
			continue;
		}

		//Add to collection
		attrCollection.push_back(aDeviceAttr);
		
		nMPs++;   
	}//end loop monitoring points
	

 	return 0;

}//close SDD_XML::Parse()


DeviceAttr* SDD_XML::ParseAttrNode(pugi::xml_node& node){
//Tango::Attribute* SDD_XML::ParseAttrNode(pugi::xml_node& node,std::string dev_name){
	
	//Get attr name
	pugi::xml_attribute name_attr= node.attribute("name");
	if(name_attr.empty()){
		cerr<<"SDD_XML::ParseAttrNode(): WARN: Invalid monitoring point detected (missing name attr), skip it!"<<endl;
		return 0;
	}	
	std::string name= name_attr.value();

	//Get attr alias
	pugi::xml_attribute alias_attr= node.attribute("alias");
	std::string alias_name= name;
	if(!alias_attr.empty()){
		alias_name= alias_attr.value();
	}

	//Get attr id
	pugi::xml_attribute id_attr= node.attribute("id");
	int id= -1;
	if(!id_attr.empty()){
		id= id_attr.as_int();
	}	
			
	//Get data type & format
	pugi::xml_node dataTypeTag= node.child("DataType");
	pugi::xml_node dataFormatTag= node.child("DataFormat");
	pugi::xml_node dataRWTypeTag= node.child("RWType");
	if(dataTypeTag.empty()){
		cerr<<"SDD_XML::ParseAttrNode(): WARN: Missing data type info ...skip it!"<<endl;
		return 0;
	}
	long int data_type= GetAttrDataType(dataTypeTag.text().as_string());
	if(data_type<0){
		cerr<<"SDD_XML::ParseAttrNode(): WARN: Failed to determine data type ...skip it!"<<endl;
		return 0;
	}
	
	if(dataFormatTag.empty()){
		cerr<<"SDD_XML::ParseAttrNode(): WARN: Missing data format info ...skip it!"<<endl;
		return 0;
	}
	int data_format= GetAttrDataFormat(dataFormatTag.text().as_string());
		
	if(dataRWTypeTag.empty()){
		cerr<<"SDD_XML::ParseAttrNode(): WARN: Missing data rw type info ...skip it!"<<endl;
		return 0;
	}
	Tango::AttrWriteType rw_type= GetAttrRWType(dataRWTypeTag.text().as_string());
			
	//Get data max dim
	long int maxDimX= -1;
	long int maxDimY= -1;
	if(data_format==2){
		pugi::xml_node dimXTag= node.child("DimX");
		if(!dimXTag.empty()){
			maxDimX= dimXTag.text().as_llong();
		}
		else{
			cerr<<"SDD_XML::ParseAttrNode(): WARN: Missing data dimension info ...skip it!"<<endl;
			return 0;
		}
	}
	if(data_format==3){
		pugi::xml_node dimXTag= node.child("DimX");	
		pugi::xml_node dimYTag= node.child("DimY");
		if(!dimXTag.empty() && !dimYTag.empty()){
			maxDimX= dimXTag.text().as_llong();
			maxDimY= dimYTag.text().as_llong();
		}
		else{
			cerr<<"SDD_XML::ParseAttrNode(): WARN: Missing data dimension info ...skip it!"<<endl;
			return 0;
		}
	}	

	//Set polling info
	pugi::xml_node pollingTag= node.child("Polling");
	bool isPolled= false;	
	long int poll_period= -1;
	if(!pollingTag.empty()){
		pugi::xml_node pollingPeriodTag= pollingTag.child("Period");
		if(!pollingPeriodTag.empty()){
			isPolled= true;
			poll_period= pollingPeriodTag.text().as_llong();
		}
		else {
			cerr<<"SDD_XML::ParseAttrNode(): ERROR: Missing polling period!"<<endl;
		}
	}//close if


	
	//Create attribute
	Tango::Attr* attr= 0;

	if(data_format==1){
		attr= new Tango::Attr(alias_name.c_str(),data_type,rw_type);
	}
	else if(data_format==2){
		attr= new Tango::SpectrumAttr (alias_name.c_str(),data_type,rw_type,maxDimX);
	}
	else if(data_format==3){
		attr= new Tango::ImageAttr (alias_name.c_str(),data_type,rw_type,maxDimX,maxDimY);
	}
	else if(data_format==4){
		//...
		//...
	}
	else{
		cerr<<"SDD_XML::ParseAttrNode(): ERROR: Invalid attr format given!"<<endl;
		return 0;
	}

	cout<<"attr name="<<attr->get_name()<<endl;


	//Set polling
	if(isPolled) attr->set_polling_period(poll_period);

	
	//Set attr attr and properties
	int status= -1;
	DeviceAttr* device_attr= 0;
	//Tango::Attribute* device_attr= 0;
	//std::vector<Tango::AttrProperty> prop_list;
	//dev_name= "";

	if(data_type==Tango::DEV_DOUBLE){
		Tango::MultiAttrProp<Tango::DevDouble>* prop= new Tango::MultiAttrProp<Tango::DevDouble>;
		status= ParseMultiAttrProp(prop,node);

		device_attr= new DeviceAttrImpl<Tango::DevDouble>(attr,prop);
		device_attr->SetPolled(isPolled);
		//device_attr= new Tango::Attribute(prop_list,*attr,dev_name,id);
		//device_attr->set_properties(*prop);

	}
	else if(data_type==Tango::DEV_FLOAT){
		Tango::MultiAttrProp<Tango::DevFloat>* prop= new Tango::MultiAttrProp<Tango::DevFloat>;
		status= ParseMultiAttrProp(prop,node);

		device_attr= new DeviceAttrImpl<Tango::DevFloat>(attr,prop);
		device_attr->SetPolled(isPolled);
	
		//device_attr= new Tango::Attribute(prop_list,*attr,dev_name,id);
		//device_attr->set_properties(*prop);
	}
	else if(data_type==Tango::DEV_LONG){
		Tango::MultiAttrProp<Tango::DevLong>* prop= new Tango::MultiAttrProp<Tango::DevLong>;
		status= ParseMultiAttrProp(prop,node);
		
		device_attr= new DeviceAttrImpl<Tango::DevLong>(attr,prop);
		device_attr->SetPolled(isPolled);

		//device_attr= new Tango::Attribute(prop_list,*attr,dev_name,id);
		//device_attr->set_properties(*prop);
	}
	else if(data_type==Tango::DEV_SHORT){
		Tango::MultiAttrProp<Tango::DevShort>* prop= new Tango::MultiAttrProp<Tango::DevShort>;
		status= ParseMultiAttrProp(prop,node);
		
		device_attr= new DeviceAttrImpl<Tango::DevShort>(attr,prop);
		device_attr->SetPolled(isPolled);

		//device_attr= new Tango::Attribute(prop_list,*attr,dev_name,id);
		//device_attr->set_properties(*prop);
	}
	else if(data_type==Tango::DEV_ENUM){
		Tango::MultiAttrProp<Tango::DevEnum>* prop= new Tango::MultiAttrProp<Tango::DevEnum>;
		status= ParseMultiAttrProp(prop,node);
		prop->min_value= Tango::AlrmValueNotSpec;
		prop->max_value= Tango::AlrmValueNotSpec;
		prop->min_alarm= Tango::AlrmValueNotSpec;
		prop->max_alarm= Tango::AlrmValueNotSpec;
		prop->min_warning= Tango::AlrmValueNotSpec;
		prop->max_warning= Tango::AlrmValueNotSpec;
		prop->delta_t= Tango::AlrmValueNotSpec;
		prop->delta_val= Tango::AlrmValueNotSpec;
		prop->rel_change= Tango::AlrmValueNotSpec;
		prop->abs_change= Tango::AlrmValueNotSpec;
		prop->archive_rel_change= Tango::AlrmValueNotSpec;
		prop->archive_abs_change= Tango::AlrmValueNotSpec;
		
		device_attr= new DeviceAttrImpl<Tango::DevEnum>(attr,prop);
		device_attr->SetPolled(isPolled);

		//device_attr= new Tango::Attribute(prop_list,*attr,dev_name,id);
		//device_attr->set_properties(*prop);
	}
	else if(data_type==Tango::DEV_STRING){
		Tango::MultiAttrProp<Tango::DevString>* prop= new Tango::MultiAttrProp<Tango::DevString>;
		status= ParseMultiAttrProp(prop,node);
		prop->min_value= Tango::AlrmValueNotSpec;
		prop->max_value= Tango::AlrmValueNotSpec;
		prop->min_alarm= Tango::AlrmValueNotSpec;
		prop->max_alarm= Tango::AlrmValueNotSpec;
		prop->min_warning= Tango::AlrmValueNotSpec;
		prop->max_warning= Tango::AlrmValueNotSpec;
		prop->delta_t= Tango::AlrmValueNotSpec;
		prop->delta_val= Tango::AlrmValueNotSpec;
		prop->rel_change= Tango::AlrmValueNotSpec;
		prop->abs_change= Tango::AlrmValueNotSpec;
		prop->archive_rel_change= Tango::AlrmValueNotSpec;
		prop->archive_abs_change= Tango::AlrmValueNotSpec;
		
		device_attr= new DeviceAttrImpl<Tango::DevString>(attr,prop);
		device_attr->SetPolled(isPolled);
	}
	if(status<0) {
		cerr<<"SDD_XML::ParseAttrNode(): WARN: Failed to parse attr property!"<<endl;
		return 0;
	}

	//Set subscription fields
	pugi::xml_node subscribeTag= node.child("Subscribe");
	bool isSubscribe= false;	
	bool hasSubEndPoint= false;
	bool hasSubAttrName= false;
	std::string sub_endpoint= "";
	std::string sub_attrname= "";
	if(!subscribeTag.empty()){
		isSubscribe= true;

		//Check sub endpoint tag
		pugi::xml_node subEndPointTag= subscribeTag.child("SubEndPoint");
		if(!subEndPointTag.empty()){
			hasSubEndPoint= true;
			sub_endpoint= subEndPointTag.text().as_string();		
		}
		else {
			cerr<<"SDD_XML::ParseAttrNode(): ERROR: Missing subscription endpoint!"<<endl;
		}
		//Check sub attrname tag
		pugi::xml_node subAttrTag= subscribeTag.child("SubAttr");
		if(!subAttrTag.empty()){
			hasSubAttrName= true;
			sub_attrname= subAttrTag.text().as_string();		
		}
		else {
			cerr<<"SDD_XML::ParseAttrNode(): ERROR: Missing subscription attr name!"<<endl;
		}

		if(isSubscribe && hasSubEndPoint && hasSubAttrName){
			device_attr->SetHasSubscriptionPoint(true);
			device_attr->SetSubscriptionEndpoint(sub_endpoint);
			device_attr->SetSubscriptionAttr(sub_attrname);
		}
	}//close if

	

	return device_attr;

}//close ParseAttrNode()


template<typename T>
//int SDD_XML::ParseMultiAttrProp(Tango::MultiAttrProp<T>& prop,pugi::xml_node& node) {
int SDD_XML::ParseMultiAttrProp(Tango::MultiAttrProp<T>* prop,pugi::xml_node& node) {

	//Set description property
	std::string description= "";
	if(!node.child("Description").empty()) {
		description= node.child("Description").text().as_string();
	}
	prop->description= description;

	//Set label property
	std::string label= "";
	if(!node.child("Label").empty()) {
		label= node.child("Label").text().as_string();
	}
	prop->label= label;

	//Set unit property
	std::string unit= "";
	if(!node.child("Unit").empty()) {
		unit= node.child("Unit").text().as_string();
	}
	prop->unit= unit;		

	//Set value range property
	pugi::xml_node dataRangeTag= node.child("ValueRange");
	if(!dataRangeTag.empty()){
		pugi::xml_node minValueTag= dataRangeTag.child("MinValue");
		pugi::xml_node maxValueTag= dataRangeTag.child("MaxValue");
		pugi::xml_node minAlarmTag= dataRangeTag.child("MinAlarm");
		pugi::xml_node maxAlarmTag= dataRangeTag.child("MaxAlarm");
		bool hasDataRange= ( !minValueTag.empty() && !maxValueTag.empty() );
		bool hasAlarmRange= ( !minAlarmTag.empty() && !maxAlarmTag.empty() );
		if(hasDataRange){
			std::string smin= minValueTag.text().as_string();
			std::string smax= maxValueTag.text().as_string();
			T xmin= Utils_ns::CodeUtils::getValueFromString<T>(smin);
			T xmax= Utils_ns::CodeUtils::getValueFromString<T>(smax);
			prop->min_value= xmin;
			prop->max_value= xmax;
		}
		else{
			prop->min_value= Tango::AlrmValueNotSpec;
			prop->max_value= Tango::AlrmValueNotSpec;
		}
		if(hasAlarmRange){
			cout<<"--> alarm range..."<<endl;
			std::string smin= minAlarmTag.text().as_string();
			std::string smax= maxAlarmTag.text().as_string();
			T xmin= Utils_ns::CodeUtils::getValueFromString<T>(smin);
			T xmax= Utils_ns::CodeUtils::getValueFromString<T>(smax);
			prop->min_alarm= xmin;
			prop->max_alarm= xmax;
		}
		else{
			prop->min_alarm= Tango::AlrmValueNotSpec;
			prop->max_alarm= Tango::AlrmValueNotSpec;
		}
	}//close if value range

	//Set Enum values
	pugi::xml_node possibleValuesTag= node.child("PossibleValues");
	std::vector<std::string> enum_labels;
	if(!possibleValuesTag.empty()){
		for (pugi::xml_node it = possibleValuesTag.child("Value"); it; it = it.next_sibling("Value")){
			std::string enum_label= it.text().as_string();
			enum_labels.push_back(enum_label);
		}//end loop enum labels
		if(enum_labels.size()>0) {
			prop->enum_labels= enum_labels;
		}
	}//close if

	//Get event info
	pugi::xml_node eventsTag= node.child("Events");
	bool hasEvents= !eventsTag.empty();	
	bool hasRelChangeEvent= false;
	bool hasRelChangeValue= false;
	//std::string relChangeValue= "";
	double relChangeValue= -1;
	bool hasAbsChangeEvent= false;
	bool hasAbsChangeValue= false;
	//std::string absChangeValue= "";
	double absChangeValue= -1;
	bool hasPushChangeEvent= false;		
	bool hasPeriodicEvent= false;
	bool hasEventPeriod= false;
	//std::string periodValue= "";
	long int periodValue= -1;
	
	if(hasEvents){
		
		//Rel Change Event
		pugi::xml_node relChangeEventTag= eventsTag.child("RelChangeEvent");
		hasRelChangeEvent= !relChangeEventTag.empty();

		if(hasRelChangeEvent){
			pugi::xml_node relChangeValueTag= relChangeEventTag.child("ChangeValue");
			hasRelChangeValue= !relChangeValueTag.empty();
			if(hasRelChangeValue) {
				relChangeValue= relChangeValueTag.text().as_double();	
			}
			else{
				cerr<<"ParseFromXML(): WARN: Missing rel_change_value!"<<endl;
				hasRelChangeEvent= false;
			}
		}//close if rel change

		//Abs Change Event
		pugi::xml_node absChangeEventTag= eventsTag.child("AbsChangeEvent");
		hasAbsChangeEvent= !absChangeEventTag.empty();
		if(hasAbsChangeEvent){
			pugi::xml_node absChangeValueTag= absChangeEventTag.child("ChangeValue");
			hasAbsChangeValue= !absChangeValueTag.empty();
			if(hasAbsChangeValue) {
				absChangeValue= absChangeValueTag.text().as_double();
			}
			else{
				cerr<<"ParseFromXML(): ERROR: Missing abs_change_value!"<<endl;
				hasAbsChangeValue= false;
			}
		}//close if abs change
				
		//Periodic event
		pugi::xml_node periodicEventTag= eventsTag.child("PeriodicEvent");
		hasPeriodicEvent= !periodicEventTag.empty();
		if(hasPeriodicEvent){
			pugi::xml_node periodValueTag= periodicEventTag.child("Period");
			hasEventPeriod= !periodValueTag.empty();
			if(hasEventPeriod) {
				periodValue= periodValueTag.text().as_llong();
			}
			else {
				cerr<<"ParseFromXML(): ERROR: Missing period value!"<<endl;
				hasEventPeriod= false;
			}
		}	

		//Pushed event
		pugi::xml_node pushChangeEventTag= eventsTag.child("PushEvent");
		hasPushChangeEvent= !pushChangeEventTag.empty();


		//Set properties
		if(hasPeriodicEvent && hasEventPeriod){
			prop->event_period= periodValue;
		}
		if(hasRelChangeEvent && hasRelChangeValue){
			prop->rel_change= relChangeValue;
		}
		if(hasAbsChangeEvent && hasAbsChangeValue){
			prop->abs_change= absChangeValue;
		}

	}//close if has events


	return 0;

}//close ParseMultiAttrProp()




int SDD_XML::parse(SDD_ns::MoniPoints& mpCollection,bool filter,std::string component_filter,int componentId_filter){

	//## Read file
	if(readFile()<0){
		cerr<<"SDD_XML::parse(): ERROR: Reading input file failed!"<<endl;
		return -1;
	}
	

	//## Navigate the DOM
	//Check if MonitoringPointsList tag is present
	std::string topTag= "SEInterface";
	std::string mpListRootTag= "MonitoringPointsList";
	std::string mpRootTag= "MonitoringPoint";
	pugi::xml_node mpRoot = fDoc.child(topTag.c_str()).child(mpListRootTag.c_str());
	if(mpRoot.empty()){
		cerr<<"SDD_XML::parse(): ERROR: Failed to parse document (missing "<<mpRootTag<<" tag)!"<<endl;
		return -1;
	}

	//Check if device address is given
	pugi::xml_node deviceAddressTag= fDoc.child("DeviceAddress");
	bool hasDeviceAddress= !deviceAddressTag.empty();
	std::string device_address= "";
	if(hasDeviceAddress){
		device_address= deviceAddressTag.text().as_string();
	}

	//Iterate over monitoring points list
	int nMPs= 0;
	MoniPoint* aMoniPoint= 0;
	
	for (pugi::xml_node mp = mpRoot.child(mpRootTag.c_str()); mp; mp = mp.next_sibling(mpRootTag.c_str())){
   
		//Get data type
		pugi::xml_node dataTypeTag= mp.child("DataType");
		if(dataTypeTag.empty()){
			cerr<<"SDD_XML::parse(): WARN: Missing data type info ...skip it!"<<endl;
			continue;
		}
		long mp_data_type= GetAttrDataType(dataTypeTag.text().as_string());
		if(mp_data_type<0){
			cerr<<"SDD_XML::parse(): WARN: Failed to determine data type ...skip it!"<<endl;
			continue;
		}
		
		//Create moni point
		aMoniPoint= new MoniPoint;
		if(aMoniPoint->ParseFromXML(mp,filter,component_filter,componentId_filter)<0){
			cerr<<"SDD_XML::parse(): WARN: Failed to parse moni point from XML node...skip it!"<<endl;
			delete aMoniPoint;
			aMoniPoint= 0;
			continue;
		}

		//Set global info
		aMoniPoint->device_address= device_address;
		
		//Add to collection
		mpCollection.push_back(aMoniPoint);
		
		nMPs++;   
	}//end loop monitoring points
	
 	return 0;

}//close SDD_XML::parse()







}//close namespace

