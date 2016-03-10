
#include <SDD_JSON.h>

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


SDD_JSON::SDD_JSON(std::string filename) 
	: SDD(filename)
{

}


SDD_JSON::~SDD_JSON(){

}


int SDD_JSON::parse(MoniPoints& mpCollection){

	//## Read file
	if(readFile()<0){
		cerr<<"SDD::parse(): ERROR: Reading input file failed!"<<endl;
		return -1;
	}

	//## Parse json string
	Json::Reader reader;
	bool isValidParsing = reader.parse(fSDDString, fSDDRoot);
	if (!isValidParsing) {// report to the user the failure and their locations in the document.
  	cerr<<"SDD::parse(): ERROR Failed to parse configuration ("<< reader.getFormattedErrorMessages()<<")"<< endl;
    return -1;
	}

	//## Nvigate the DOM

	Json::Value mp_array= fSDDRoot.get("MonitoringPoints",0);
	if(!mp_array.isArray()) return -1;

	bool hasFailed= false;

	for( Json::ValueIterator it= mp_array.begin();it!=mp_array.end();it++){
		//std::string device_name= it->
		char* member_name= (char*)it.memberName();
		int arg_value= (it.key()).asInt();
		
		
		//Json::Value plugins = it->get["device"];
		std::string device_name= it->get("device","").asString();
		if(device_name=="") {
			hasFailed= true;
			break;
		}

		cout<<"member_name="<<member_name<<" arg_value="<<arg_value<<" device_name="<<device_name<<endl;

		//Get access to list of monitoring point info
		Json::Value mplist= it->get("mplist",0);
		if(!mplist){
			hasFailed= true;
			break;
		}
		for( Json::ValueIterator mpIter= mplist.begin();mpIter!=mplist.end();mpIter++){
			int id= mpIter->get("id",-1).asInt();
			std::string description= mpIter->get("description","").asString();
			std::string name= mpIter->get("name","").asString();
			std::string strategy= mpIter->get("strategy","").asString();
			cout<<"MP no. "<<id<<", info="<<description<<" name="<<name<<" strategy="<<strategy<<endl;
		}//end loop mni points

	}//end loop array
	
	if(hasFailed){
		cerr<<"SDD::parse(): ERROR: Parsing failed!"<<endl;
		return -1;
	}

	return 0;

}//close SDD_JSON::parse()


std::string SDD_JSON::getJSON(bool isMinified) {

	if(fSDDString=="") return fSDDString;

	std::string jsonString= "";
	if(isMinified){// write in a minified way
		Json::FastWriter fastWriter;
		jsonString= fastWriter.write(fSDDRoot);
	}
	else{	// write in a nice readible way
		Json::StyledWriter formattedWriter;
		jsonString= formattedWriter.write(fSDDRoot);
	}

	jsonString.erase(jsonString.size()-1);
	
	return jsonString;

}//close SDD_JSON::getJSON()


}//close namespace
