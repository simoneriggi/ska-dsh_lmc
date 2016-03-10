#include <SDD.h>

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

SDD::SDD(std::string filename) 
	: fFileName(filename)
{

}   


SDD::~SDD(){

}

int SDD::GetAttrDataFormat(std::string dataFormatTag){

	long data_format= 1;
	if(dataFormatTag=="SCALAR"){
		data_format= 1;
	}	
	else if(dataFormatTag=="SPECTRUM"){
		data_format= 2;
	}
	else if(dataFormatTag=="IMAGE"){
		data_format= 3;
	}
	
	return data_format;

}//close GetAttrDataFormat()


Tango::AttrWriteType SDD::GetAttrRWType(std::string dataRWTag){
		
	Tango::AttrWriteType data_rw_type= Tango::READ;
	if(dataRWTag=="READ"){
		data_rw_type= Tango::READ;
	}	
	else if(dataRWTag=="WRITE"){
		data_rw_type= Tango::WRITE;
	}
	else if(dataRWTag=="READ_WRITE"){
		data_rw_type= Tango::READ_WRITE;
	}

	return data_rw_type;

}//close GetAttrRWType()

long SDD::GetAttrDataType(std::string dataTypeTag){

	long data_type= -1;
	if(dataTypeTag=="DOUBLE"){
		data_type= static_cast<long>(Tango::DEV_DOUBLE);
	}	
	else if(dataTypeTag=="FLOAT"){
		data_type= static_cast<long>(Tango::DEV_FLOAT);
	}
	else if(dataTypeTag=="SHORT"){
		data_type= static_cast<long>(Tango::DEV_SHORT);
	}
	else if(dataTypeTag=="LONG"){
		data_type= static_cast<long>(Tango::DEV_LONG);
	}	
	else if(dataTypeTag=="ENUM"){
		data_type= static_cast<long>(Tango::DEV_ENUM);
	}
	else if(dataTypeTag=="STRING"){
		data_type= static_cast<long>(Tango::DEV_STRING);
	}

	return data_type;

}//close SetAttrDataType()

int SDD::readFile(){

	if(fFileName=="") return -1;

	std::ifstream inFile;
	inFile.open(fFileName);//open the input file
	if(!inFile.is_open()) return -1;

	std::stringstream sstream;
	sstream << inFile.rdbuf();//read the file
	fSDDString = sstream.str();//str holds the content of the file

	//cout<<"SDD::readFile(): INFO: SDD string: "<<fSDDString<<endl;

	return 0;

}//close readFile()

}//close namespace
