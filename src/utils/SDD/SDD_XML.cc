#include <SDD_XML.h>

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


SDD_XML::SDD_XML(std::string filename) 
	: SDD(filename)
{

}


SDD_XML::~SDD_XML(){

}



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


int SDD_XML::readFile(){

	//## Open file and load to doc object
	if ( !fDoc.load_file(fFileName.c_str(),pugi::parse_trim_pcdata) ) {
		cerr<<"SDD::parse(): ERROR: Reading input file failed!"<<endl;
		return -1;
	}

	return 0;

}//close readFile()


}//close namespace

