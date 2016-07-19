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
	//cout<<"~Request(): Calling destructor..."<<endl;
}

bool Request::Validate(){

	//Check if mandatory fields are set
	if(!Message::Validate()) return false;

	if( !HasField(std::string("type")) ) return false;
	if( !HasField(std::string("source_id")) ) return false;
	if( !HasField(std::string("retry_number")) ) return false;

	return true;

}//close Validate()

bool Request::Validate(bool checkSchedulable){

	if(!Validate()) return false;

	//Check if this is a scheduled & schedulable request
	if( IsScheduled() && !IsSchedulable() ) {
		cerr<<"Request::Validate(): WARN: Schedulable task cannot be scheduled (activation time in the past?)"<<endl;
		return false;
	}

	return true;

}//close Validate()


int Request::ParseFromJsonObj(Json::Value& root){

	//Set class fields from json object
	if(root.isNull()) {
		cerr<<"Request::ParseFromJsonObj(): ERROR: Null json tree!"<<endl;
		return -1;
	}

	if(root.isMember("id")){
		std::string id= root["id"].asString();
		if(id=="") {
			cerr<<"Request::ParseFromJsonObj(): ERROR: Empty id field!"<<endl;
			return -1;
		}
		SetId(id);
	}
	if(root.isMember("source_id")){
		std::string source_id= root["source_id"].asString();
		if(source_id=="") {
			cerr<<"Request::ParseFromJsonObj(): ERROR: Empty source_id field!"<<endl;
			return -1;
		}
		SetSourceId(source_id);
	}
	if(root.isMember("type")){
		std::string type= root["type"].asString();
		if(type=="") {
			cerr<<"Request::ParseFromJsonObj(): ERROR: Empty type field!"<<endl;
			return -1;
		}
		SetType(type);
	}
	if(root.isMember("retry_number")){
		short retry_number= static_cast<short>(root["retry_number"].asInt());
		if(retry_number<0) {
			cerr<<"Request::ParseFromJsonObj(): ERROR: Invalid retry_number field (value="<<retry_number<<")"<<endl;
			return -1;
		}
		SetRetryNumber(retry_number);
	}
	if(root.isMember("subarray_id")){
		short subarray_id= static_cast<short>(root["subarray_id"].asInt());
		if(subarray_id<0) {
			cerr<<"Request::ParseFromJsonObj(): ERROR: Invalid subarray_id field (value="<<subarray_id<<")"<<endl;
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
			cerr<<"Request::ParseFromJsonObj(): ERROR: Empty activation time field!"<<endl;
			return -1;
		}

		std::string t_end= "";
		if(hasExpirationTime) {
			t_end= root["expiration_time"].asString();
			if(t_end==""){
				cerr<<"Request::ParseFromJsonObj(): ERROR: Empty expiration time field!"<<endl;
				return -1;
			}

			//Check time range
			std::time_t epoch_start= Utils_ns::SysUtils::TimeStringToLocalEpoch(t_start);
			std::time_t epoch_end= Utils_ns::SysUtils::TimeStringToLocalEpoch(t_end);
		
			if(epoch_start>=epoch_end) {
				cerr<<"Request::ParseFromJsonObj(): ERROR: Invalid time range requested (t_start is after t_end)!"<<endl;
				return -1;
			}
			SetExpirationTime(t_end);
			
		}//close if
		SetActivationTime(t_start);
	}//close if
	

	cout<<"Request::ParseFromJsonObj(): INFO: hasArgs? "<<root.isMember("arguments")<<endl;
	if(root.isMember("arguments")){
		Json::Value attr_root= root["arguments"];
		cout<<"--> nargs="<<attr_root.size()<<" attr_root.isArray()="<<attr_root.isArray()<<endl;
		if(attr_root.isArray()){
			for(unsigned int i=0;i<attr_root.size();i++){
				int status= AddArgumentFromJson(attr_root[i]);
				if(status<0) {
					cerr<<"Request::ParseFromJsonObj(): ERROR: Failed to parse argument no. "<<i+1<<endl;
					return -1;
				}
			}
		}
	}//close if

	//Validate to ensure that all fields are correctly set
	if(!Validate()) {
		cerr<<"Request::ParseFromJsonObj(): ERROR: Validation failed!"<<endl;
		return -1;
	}

	return 0;

}//close ParseFromJsonObj()

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
	
	//Parse from Json obj
	return ParseFromJsonObj(root);

}//close Parse()



int Request::Parse(char* msg){

	//First parse input string msg into Json object
	Json::Value root;//root of json
	Json::Reader reader;
	size_t n= strlen(msg);
	try {
		reader.parse(msg,msg+n-1,root);
	}
	catch(...){
		cerr<<"Request::Parse(): ERROR: Failed to parse input string!"<<endl;
		return -1;
	}

	//Parse from Json obj
	return ParseFromJsonObj(root);

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


bool Request::IsSchedulable(){

	//Check if this is a scheduled task
	if(!IsScheduled()) return false;

	//Check activation time
	std::string t_start= m_activation_time;	
	auto now = std::chrono::system_clock::now();
	auto t_start_local= Utils_ns::SysUtils::TimeStringToUTCChronoTimeStamp(t_start);

	if(t_start_local<now){
		cerr<<"Request::IsSchedulable(): WARN: Activation time is in the past!"<<endl;
		return false;
	}

	//Check expiration time 
	std::string t_end= "";
	if( HasField("expiration_time") ) {
		t_end= m_expiration_time;
		auto t_end_local= Utils_ns::SysUtils::TimeStringToUTCChronoTimeStamp(t_end);
		if(t_end_local<t_start_local) {
			cerr<<"Request::IsSchedulable(): WARN: Expiration time is before activation!"<<endl;
			return false;	
		}
	}//close if
	
	return true;

}//close IsSchedulable()

bool Request::IsScheduled(){
		
	//Check request args
	if( !HasField("activation_time") ){
		return false;
	}

	return true;

}//close IsScheduled()


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
	
	//cout<<"~Response(): Calling destructor..."<<endl;
			
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


int Response::ParseFromJsonObj(Json::Value& root){

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

}//close ParseFromJsonObj()


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
	
	return ParseFromJsonObj(root);


	/*
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
	*/

}//close Parse()

int Response::Parse(char* msg){

	//First parse input string msg into Json object
	Json::Value root;//root of json
	Json::Reader reader;
	size_t n= strlen(msg);
	
	try {
		reader.parse(msg,msg+n-1,root);
	}
	catch(...){
		cerr<<"Response::Parse(): ERROR: Failed to parse input string!"<<endl;
		return -1;
	}
	
	return ParseFromJsonObj(root);

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
			cout<<"Response::GetPipeBlob(): INFO: Getting arg pipe blob..."<<endl;
			auto args_blob= GetArgumentPipeBlobPtr();
			if(!args_blob){
				throw std::runtime_error("Failed to get arguments pipe blob");
			}
			cout<<"Response::GetPipeBlob(): INFO: Filling arg pipe blob..."<<endl;
			blob["Arguments"] << *args_blob;
			cout<<"Response::GetPipeBlob(): INFO: done!"<<endl;
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

int MessageUtils::ParseRequest(Request& reqObj,std::string& req){

	return reqObj.Parse(req);
	
}//close ParseRequest()
		
int MessageUtils::ParseRequest(Request& reqObj,char* req){

	return reqObj.Parse(req);
	
}//close ParseRequest()


int MessageUtils::ParseResponse(Response& resObj,std::string& res){

	return resObj.Parse(res);
	
}//close ParseResponse()
		
int MessageUtils::ParseResponse(Response& resObj,char* res){

	return resObj.Parse(res);
	
}//close ParseResponse()

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

