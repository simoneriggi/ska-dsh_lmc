#include <Message.h>

#include <SysUtils.h>

//# JSON CPP
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>

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

namespace MessageParser_ns {


//#############################
//##    REQUEST CLASS  
//#############################
Request::Request() {	
	m_retry_number= 0;
	m_type= "";
	m_source_id= "";
	m_subarray_id= -1;
	m_activation_time= "";
	m_expiration_time= "";
}   

Request::~Request(){
	cout<<"~Request(): Calling destructor..."<<endl;
	
}

bool Request::Validate(){

	//Check if mandatory fields are set
	if(!Message::Validate()) return false;

	if( !HasField(std::string("type")) ) return false;
	if( !HasField(std::string("source_id")) ) return false;
	if( !HasField(std::string("retry_number")) ) return false;

	return true;

}//close Validate()

int Request::Parse(std::string& msg){

	//First parse input string msg into Json object
	Json::Value root;//root of json
	Json::Reader reader;
	try {
		reader.parse(msg,root);
	}
	catch(...){
		cerr<<"Request::Parse(): ERROR: Failed to parse input string!"<<endl;
		return -1;
	}

	//Set class fields from json object
	if(root.isNull()) {
		cerr<<"Request::Parse(): ERROR: Null json tree!"<<endl;
		return -1;
	}

	if(root.isMember("id")){
		std::string id= root["id"].asString();
		if(id=="") {
			cerr<<"Request::Parse(): ERROR: Empty id field!"<<endl;
			return -1;
		}
		SetId(id);
	}
	if(root.isMember("source_id")){
		std::string source_id= root["source_id"].asString();
		if(source_id=="") {
			cerr<<"Request::Parse(): ERROR: Empty source_id field!"<<endl;
			return -1;
		}
		SetSourceId(source_id);
	}
	if(root.isMember("type")){
		std::string type= root["type"].asString();
		if(type=="") {
			cerr<<"Request::Parse(): ERROR: Empty type field!"<<endl;
			return -1;
		}
		SetType(type);
	}
	if(root.isMember("retry_number")){
		short retry_number= static_cast<short>(root["retry_number"].asInt());
		if(retry_number<0) {
			cerr<<"Request::Parse(): ERROR: Invalid retry_number field (value="<<retry_number<<")"<<endl;
			return -1;
		}
		SetRetryNumber(retry_number);
	}
	if(root.isMember("subarray_id")){
		short subarray_id= static_cast<short>(root["subarray_id"].asInt());
		if(subarray_id<0) {
			cerr<<"Request::Parse(): ERROR: Invalid subarray_id field (value="<<subarray_id<<")"<<endl;
			return -1;
		}
		SetSubArrayId(subarray_id);
	}

	bool hasActivationTime= root.isMember("activation_time");
	bool hasExpirationTime= root.isMember("expiration_time");
	if(hasExpirationTime && !hasActivationTime) return -1;
	
	if(hasActivationTime){
		std::string t_start= root["activation_time"].asString();
		if(t_start==""){
			cerr<<"Request::Parse(): ERROR: Empty activation time field!"<<endl;
			return -1;
		}

		std::string t_end= "";
		if(hasExpirationTime) {
			t_end= root["expiration_time"].asString();
			if(t_end==""){
				cerr<<"Request::Parse(): ERROR: Empty expiration time field!"<<endl;
				return -1;
			}

			//Check time range
			std::time_t epoch_start= Utils_ns::SysUtils::TimeStringToLocalEpoch(t_start);
			std::time_t epoch_end= Utils_ns::SysUtils::TimeStringToLocalEpoch(t_end);
			/*
			std::tm tm_start;
			std::tm tm_end;
			
			strptime(t_start.c_str(), "%Y-%m-%dT%T%z", &tm_start);
			tm_start.tm_isdst = -1;
			std::time_t epoch_start= std::mktime(&tm_start);
			std::time_t epochFromUTC_start = timegm(tm_start);//Assume tm is expressed in UTC (neglects any timezone setting)

			strptime(t_end.c_str(), "%Y-%m-%dT%T%z", &tm_end);
			tm_end.tm_isdst = -1;
			std::time_t epoch_end= std::mktime(&tm_end);
			std::time_t epochFromUTC_end = timegm(tm_end);//Assume tm is expressed in UTC (neglects any timezone setting)
			*/


			if(epoch_start>=epoch_end) {
				cerr<<"Request::Parse(): ERROR: Invalid time range requested (t_start is after t_end)!"<<endl;
				return -1;
			}
			SetExpirationTime(t_end);
			
		}//close if
		SetActivationTime(t_start);
	}//close if
	

	if(root.isMember("arguments")){
		Json::Value attr_root= root["arguments"];
		if(attr_root.isArray()){
			for(unsigned int i=0;i<attr_root.size();i++){
				int status= AddArgumentFromJson(attr_root[i]);
				if(status<0) {
					cerr<<"Request::Parse(): ERROR: Failed to parse argument no. "<<i+1<<endl;
					return -1;
				}
			}
		}
	}//close if

	//Validate to ensure that all fields are correctly set
	if(!Validate()) {
		cerr<<"Request::Parse(): ERROR: Validation failed!"<<endl;
		return -1;
	}

	return 0;

}//close Parse()

int Request::GetJson(Json::Value& root){

	//Validate request message first		
	if(!Validate()) return -1;

	//Set mandatory fields
	root["id"]= m_id;
	root["source"]= m_source_id;	
	root["type"]= m_type;
	root["retry_number"]= m_retry_number;
	
	//Set optional fields
	if( HasField(std::string("subarray_id")) ) root["subarray_id"]= m_subarray_id;
	if( HasField(std::string("activation_time")) ) root["activation_time"]= m_activation_time;
	if( HasField(std::string("expiration_time")) ) root["expiration_time"]= m_expiration_time;
	
	if(HasField(std::string("arguments"))){
		Json::Value argObj;
		for(unsigned int i=0;i<m_args.size();i++){
			Json::Value thisArgObj= m_args[i]->GetJson();
			argObj.append(thisArgObj);
		}
		root["arguments"]= argObj;
	}
	
	return 0;

}//close GetJson()



//#############################
//##    RESPONSE CLASS  
//#############################
Response::Response() {	
	m_type= ACK_RESPONSE;
	m_return_code= -1;
	m_response_msg= "";
	m_lmc_id= "";
}   

Response::~Response(){
	
	cout<<"~Response(): Calling destructor..."<<endl;
			
}

bool Response::Validate(){

	//Check if mandatory fields are set
	if(!Message::Validate()) return false;
	
	if( !HasField(std::string("type")) ) {
		cerr<<"Response::Validate(): ERROR: Missing type field!"<<endl;
		return false;
	}
	if( !HasField(std::string("lmc_id")) ) {
		cerr<<"Response::Validate(): ERROR: Missing lmc_id field!"<<endl;
		return false;
	}
	if( !HasField(std::string("return_code")) ) {
		cerr<<"Response::Validate(): ERROR: Missing return_code field!"<<endl;
		return false;
	}
	if( !HasField(std::string("response_msg")) ) {
		cerr<<"Response::Validate(): ERROR: Missing response_msg field!"<<endl;		
		return false;
	}

	return true;

}//close validate()


int Response::Parse(std::string& msg){

	
	//First parse input string msg into Json object
	Json::Value root;//root of json
	Json::Reader reader;
	try {
		reader.parse(msg,root);
	}
	catch(...){
		cerr<<"Response::Parse(): ERROR: Failed to parse input string!"<<endl;
		return -1;
	}
	
	cout<<"Response::Parse(): INFO: msg="<<msg<<endl;

	//Set class fields from json object
	if(root.isNull()) {
		cerr<<"Response::Parse(): ERROR: Null json tree!"<<endl;
		return -1;
	}

	if(root.isMember("id")){
		std::string id= root["id"].asString();
		if(id=="") {
			cerr<<"Response::Parse(): ERROR: Empty id field!"<<endl;
			return -1;
		}
		SetId(id);
	}
	if(root.isMember("lmc_id")){
		std::string lmc_id= root["lmc_id"].asString();
		if(lmc_id=="") {
			cerr<<"Response::Parse(): ERROR: Empty lmc_id field!"<<endl;
			return -1;
		}
		SetLMCId(lmc_id);
	}	

	if(root.isMember("type")){
		int type_code= root["type"].asInt();
		switch(type_code){
			case ACK_RESPONSE:
				SetType(ACK_RESPONSE);
				break;
			case INTERMEDIATE_RESPONSE:
				SetType(INTERMEDIATE_RESPONSE);
				break;
			case FINAL_RESPONSE:
				SetType(FINAL_RESPONSE);
				break;
			default:
			{
				cerr<<"Response::Parse(): ERROR: Invalid response type given (value="<<type_code<<")!"<<endl;
				return -1;
				break;
			}
		}//close switch	
	}//close if
	
	if(root.isMember("return_code")){
		SetReturnCode( static_cast<short>(root["return_code"].asInt()) );
	}
	
	if(root.isMember("response_msg")){
		SetResponseMessage( root["response_msg"].asString() );
	}

	if(root.isMember("arguments")){
		Json::Value attr_root= root["arguments"];
		if(attr_root.isArray()){
			for(unsigned int i=0;i<attr_root.size();i++){
				AddArgumentFromJson(attr_root[i]);
			}
		}
	}//close if

	//Validate to ensure that all fields are correctly set
	if(!Validate()) {
		cerr<<"Response::Parse(): ERROR: Validation failed!"<<endl;
		return -1;
	}

	return 0;

}//close Parse()

int Response::GetJson(Json::Value& root){

	//Validate request message first		
	if(!Validate()) return -1;

	//Set mandatory fields
	root["id"]= m_id;
	root["lmc_id"]= m_lmc_id;
	root["type"]= m_type;
	root["return_code"]= m_return_code;
	root["response_msg"]= m_response_msg;

	//Set optional fields
	if(HasField(std::string("arguments"))){
		Json::Value argObj;
		for(unsigned int i=0;i<m_args.size();i++){
			Json::Value thisArgObj= m_args[i]->GetJson();
			argObj.append(thisArgObj);
		}
		root["arguments"]= argObj;
	}

	return 0;

}//close GetJson()


//int Response::GetPipe(Tango::Pipe& pipe){
int Response::GetPipeBlob(Tango::DevicePipeBlob& blob){

	bool hasArgs= HasArguments();
	Tango::DevShort type= static_cast<Tango::DevShort>(m_type);

	try {
		blob.set_name("FinalResponseBlob");
		std::vector<std::string> field_names {"CommandID","LMCID","ResponseType","ReturnCode","ResponseMsg"};
		if(hasArgs) field_names.push_back("Arguments");
		blob.set_data_elt_names(field_names);
				
		blob["CommandID"] << m_id;
		blob["LMCID"] << m_lmc_id;
		blob["ResponseType"] << type;
		blob["ReturnCode"] << m_return_code;
		blob["ResponseMsg"] << m_response_msg;

		
		if(hasArgs){
			auto args_blob= GetArgumentPipeBlobPtr();
			if(args_blob){
				throw std::runtime_error("Failed to get arguments pipe blob");
			}
			blob["Arguments"] << *args_blob;
		}
		

	}//close try block
	catch(Tango::DevFailed& e){
		Tango::Except::print_exception(e);
		cerr<<"Response::GetPipeBlob(): ERROR: Exception occurred while creating and filling response message pipe!"<<endl;
		return -1;
	}
	catch(std::exception &e){
		cerr << "Response::GetPipeBlob(): ERROR: Run time exception occurred (e="<<e.what()<<") while creating and filling response message pipe!"<<endl;	
		return -1;
	}
	catch(...){
		cerr<<"Response::GetPipeBlob(): ERROR: Unknown exception occurred while creating and filling response message pipe!"<<endl;
		return -1;
	}
	return 0;

}//close GetPipe()


//#############################
//##    MESSAGE UTILS
//#############################
MessageUtils::MessageUtils() {	
	
}   

MessageUtils::~MessageUtils(){

}

int MessageUtils::MakeSuccessResponse(Response& res,Request& req,std::string lmc_id,std::string msg){

	//Check request
	if(!req.Validate()){
		cerr<<"MessageUtils::MakeSuccessResponse(): ERROR: Invalid request object given!"<<endl;
		return -1;
	}
	
	//Fill response message
	res.SetId(req.GetId());
	res.SetLMCId(lmc_id);
	res.SetType(MessageParser_ns::ACK_RESPONSE);
	res.SetReturnCode(MessageParser_ns::ACK);
	res.SetResponseMessage(msg);

	//std::vector<Argument*> args= req.GetArguments();
	//if(req.HasArguments()) res.CopyArguments(args);

	if(!res.Validate()){
		cerr<<"MessageUtils::MakeSuccessResponse(): ERROR: Invalid response object created!"<<endl;
		return -1;
	}

	return 0;
}//close MakeSuccessResponse()

int MessageUtils::MakeSuccessResponse(std::string& jsonString,Request& req,std::string lmc_id,std::string msg){

	Response res;
	if(MakeErrorResponse(res,req,lmc_id,msg)<0){
		cerr<<"MessageUtils::MakeSuccessResponse(): ERROR: Failed to encode response to Response!"<<endl;	
		return -1;
	}
	res.Print();

	if(res.GetJsonString(jsonString,true)<0){
		cerr<<"MessageUtils::MakeSuccessResponse(): ERROR: Failed to encode response to json string!"<<endl;
		return -1;
	}

	return 0;

}//close MakeSuccessResponse()



int MessageUtils::MakeErrorResponse(Response& res,Request& req,std::string lmc_id,std::string msg){

	//Check request
	if(!req.Validate()){
		cerr<<"MessageUtils::MakeErrorResponse(): ERROR: Invalid request object given!"<<endl;
		return -1;
	}
	
	//Fill response message
	res.SetId(req.GetId());
	res.SetLMCId(lmc_id);
	res.SetType(MessageParser_ns::ACK_RESPONSE);
	res.SetReturnCode(MessageParser_ns::NACK);
	res.SetResponseMessage(msg);

	//std::vector<Argument*> args= req.GetArguments();
	//if(req.HasArguments()) res.CopyArguments(args);

	if(!res.Validate()){
		cerr<<"MessageUtils::MakeErrorResponse(): ERROR: Invalid response object created!"<<endl;
		return -1;
	}

	return 0;

}//close MakeErrorResponse()

int MessageUtils::MakeErrorResponse(std::string& jsonString,Request& req,std::string lmc_id,std::string msg){

	Response res;
	if(MakeErrorResponse(res,req,lmc_id,msg)<0){
		cerr<<"MessageUtils::MakeErrorResponse(): ERROR: Failed to encode response to Response!"<<endl;	
		return -1;
	}
	res.Print();

	if(res.GetJsonString(jsonString,true)<0){
		cerr<<"MessageUtils::MakeErrorResponse(): ERROR: Failed to encode response to json string!"<<endl;
		return -1;
	}

	return 0;

}//close MakeErrorResponse()

int MessageUtils::MakeEchoResponse(Response& res,Request& req,std::string lmc_id,std::string msg){

	//Check request
	if(!req.Validate()){
		cerr<<"MessageUtils::MakeEchoResponse(): ERROR: Invalid request object given!"<<endl;
		return -1;
	}
	
	//Fill response message
	res.SetId(req.GetId());
	res.SetLMCId(lmc_id);
	res.SetType(MessageParser_ns::ACK_RESPONSE);
	res.SetReturnCode(MessageParser_ns::ACK);
	res.SetResponseMessage(msg);

	std::vector<Argument*> args= req.GetArguments();
	if(req.HasArguments()) res.CopyArguments(args);

	if(!res.Validate()){
		cerr<<"MessageUtils::MakeEchoResponse(): ERROR: Invalid response object created!"<<endl;
		return -1;
	}

	return 0;

}//close MakeEchoResponse()
		
int MessageUtils::MakeEchoResponse(std::string& jsonString,Request& req,std::string lmc_id,std::string msg){

	Response res;
	if(MakeEchoResponse(res,req,lmc_id,msg)<0){
		cerr<<"MessageUtils::MakeEchoResponse(): ERROR: Failed to encode response to Response!"<<endl;	
		return -1;
	}
	res.Print();

	if(res.GetJsonString(jsonString,true)<0){
		cerr<<"MessageUtils::MakeEchoResponse(): ERROR: Failed to encode response to json string!"<<endl;
		return -1;
	}

	return 0;

}//close MakeEchoResponse()

}//close namespace

