/**
* @file Argument.h
* @class Argument
* @brief Class storing argument 
* 
* @author S. Riggi
* @date 22/02/2016
*/

#ifndef Argument_h
#define Argument_h 1

#include <SysUtils.h>

#include <tango.h>
#include <json/json.h>

#include <vector>
#include <string>
#include <map>
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <type_traits>
#include <cstdint>

using namespace std;

namespace MessageParser_ns {


class Argument {
	public:
		virtual ~Argument() {}
	public:
		virtual std::string GetName() const = 0;
		virtual std::string GetType() const = 0;
		
		virtual void SetTypeCode() = 0;
		virtual void SetValueFromStream(std::stringstream& sstream) = 0;
		virtual void SetValueFromString(std::string const& s) = 0;
		virtual Json::Value GetJson() = 0;
		virtual int GetJsonString(std::string&,bool) = 0;
		virtual Argument* Clone() const = 0;
		virtual std::shared_ptr<Tango::DevicePipeBlob> GetPipeBlobPtr() = 0;
};
typedef std::vector<Argument*> Arguments;
typedef Argument* ArgumentPtr;
typedef std::map<std::string, ArgumentPtr> ArgumentMap;


 

template <typename T>
struct ArgumentHelper {
	static void SetArgValueFromStream(std::stringstream& sstream,T& value) {
		sstream>>value; 
	}
	static void GetJson(T& value,Json::Value& root) {
		root["value"]= value;
	}
};


template <>
struct ArgumentHelper<long int> {
	static void SetArgValueFromStream(std::stringstream& sstream,long int& value) {
		sstream>>value; 
	}
	static void GetJson(long int& value,Json::Value& root) {
		root["value"]= Json::Value( static_cast<Json::Int64>(value) );
	}
};//close template specialization for long int

template <>
struct ArgumentHelper<bool> {
	static void SetArgValueFromStream(std::stringstream& sstream,bool& value) {
		sstream >> std::boolalpha >> value;
	}
	static void GetJson(bool& value,Json::Value& root) {
		root["value"]= value;
	}
};//close template specialization for long int


template <typename T>
struct ArgumentHelper< std::vector<T> > {
	static void SetArgValueFromStream(std::stringstream& sstream,std::vector<T>& value) {
		do {
    		for (T thisVal; sstream >> thisVal;) {
      		value.push_back(thisVal);
        }
        if (sstream.fail()) {
        	sstream.clear();
        	std::string token;
        	sstream >> token;
        }
    	}
    	while (!sstream.eof());
	}//close function

	static void GetJson(std::vector<T>& value,Json::Value& root) {
		for(unsigned int i=0;i<value.size();i++){	
			root["value"].append( Json::Value(value[i]) );
		}
	}
};


template <>
struct ArgumentHelper< std::vector<long int> > {
	static void SetArgValueFromStream(std::stringstream& sstream,std::vector<long int>& value) {	
		do {
    	for (long int thisVal; sstream >> thisVal;) {
      	value.push_back(thisVal);
      }
     	if (sstream.fail()) {
      	sstream.clear();
       	std::string token;
        sstream >> token;
    	}
    }
    while (!sstream.eof());
	}//close function

	static void GetJson(std::vector<long int>& value,Json::Value& root) {
		for(unsigned int i=0;i<value.size();i++){	
			root["value"].append( Json::Value( static_cast<Json::Int64>(value[i]) ) );
		}
	}
};//close template specialization for std::vector<bool>


template <>
struct ArgumentHelper< std::vector<bool> > {
	static void SetArgValueFromStream(std::stringstream& sstream,std::vector<bool>& value) {
		do {
    	for (bool thisVal; sstream >> std::boolalpha >> thisVal;) {
      	value.push_back(thisVal);
      }
      if (sstream.fail()) {
      	sstream.clear();
       	std::string token;
        sstream >> token;
      }
    }
    while (!sstream.eof());
	}//close function

	static void GetJson(std::vector<bool>& value,Json::Value& root) {
		for(unsigned int i=0;i<value.size();i++){	
			root["value"].append( Json::Value(value[i]) );
		}
	}
};//close template specialization for std::vector<bool>

template <typename T> 
class ArgumentImpl : public Argument {

	public:
		/** 
		\brief Class constructor
 		*/
  	ArgumentImpl(std::string arg_name,T arg_value) : 
			name(arg_name), value(arg_value)
		{
			SetTypeCode();
		};
		ArgumentImpl(std::string arg_name,std::string arg_type,T arg_value) : 
			name(arg_name), type(arg_type), value(arg_value)
		{
		};
		/** 
		\brief Class destructor
 		*/
  	virtual ~ArgumentImpl(){};

		typedef T ArgumentType;

	public:

		std::string GetName() const {return name;}
		std::string GetType() const {return type;}


		Argument* Clone() const { 
			return new ArgumentImpl<T>(*this); 
			//return new Derived(static_cast<Derived const &>(*this));
		}

		void SetTypeCode() {
			cout<<"SetTypeCode()"<<endl;
			if( std::is_same<T, bool>::value ) type= "DevBoolean";
			else if( std::is_same<T, float>::value ) type= "DevFloat";
			else if( std::is_same<T, double>::value ) type= "DevDouble";
			else if( std::is_same<T, short>::value ) type= "DevShort";
			else if(std::is_same<T, int>::value) type= "DevLong";
			else if(std::is_same<T, long int>::value) type= "DevLong64";
			else if(std::is_same<T, char*>::value) type= "DevString";
			else if(std::is_same<T, std::string>::value) type= "DevString";
			else type= "UNKNOWN";
		}//close SetTypeCode()

		/** 
		\brief Set argument value from stringstream
 		*/
		void SetValueFromStream(std::stringstream& sstream){
			ArgumentHelper<T>::SetArgValueFromStream(sstream,value);			
		}
		
		/** 
		\brief Set argument value from string
 		*/
		void SetValueFromString(std::string const& s){
			std::stringstream sstream(s);
			return SetValueFromStream(sstream);
		}

		/** 
		\brief Set argument value from stringstream
 		*/
		int SetValueFromJson(Json::Value& root){
			return ArgumentHelper<T>::SetArgValueFromJson(root,value);			
		}

		/** 
		\brief Get json value
 		*/
		Json::Value GetJson(){
			Json::Value jsonObj;
			jsonObj["name"]= name;
			jsonObj["type"]= type;
			ArgumentHelper<T>::GetJson(value,jsonObj);	
			return jsonObj;
		}

		/** 
		\brief Get json string
 		*/
		int GetJsonString(std::string& jsonString,bool isMinified=true){
			Json::Value jsonObj= GetJson();
			try {
				if(isMinified){// write in a minified way
					Json::FastWriter fastWriter;
					jsonString= fastWriter.write(jsonObj);
				}
				else{	// write in a nice readible way
					Json::StyledWriter formattedWriter;
					jsonString= formattedWriter.write(jsonObj);
				}
			}
			catch(...){
				cerr<<"ArgumentImpl::GetJsonString(): ERROR: Failed to encode argument to json string!"<<endl;
				return -1;
			}
			return 0;
		};
		
		/** 
		\brief Get Tango pipe blob
 		*/
		std::shared_ptr<Tango::DevicePipeBlob> GetPipeBlobPtr(){
			auto blob= std::make_shared<Tango::DevicePipeBlob>();
			blob->set_name(name);
			std::vector<std::string> field_names {"name","type","value"};
				
			try {
				blob->set_data_elt_names(field_names);		
				(*blob)["name"] << name;
				(*blob)["type"] << type;
				(*blob)["value"] << value;
			}
			catch(Tango::DevFailed& e){
				Tango::Except::print_exception(e);
				cerr<<"ArgumentImpl::GetPipeBlob(): ERROR: Exception occurred while creating and filling task pipe!"<<endl;
				return 0;
			}
			catch(...){
				cerr<<"ArgumentImpl::GetPipeBlob(): ERROR: Unknown exception occurred while creating and filling task pipe!"<<endl;
				return 0;
			}
			return blob;
		}//close GetPipeBlob()

	public:	
		
		std::string name;
		std::string type;
		//Tango::CmdArgType type_code;
		T value;
		
};




class ArgumentFactory {

	public:
		static ArgumentFactory& Instance() {
    	// Since it's a static variable, if the class has already been created,
      // It won't be created again.
      // And it is thread-safe in C++11.
      static ArgumentFactory myInstance;
 
      // Return a reference to our instance.
      return myInstance;
    }
 
    // delete copy and move constructors and assign operators
    ArgumentFactory(ArgumentFactory const&) = delete;             // Copy construct
    ArgumentFactory(ArgumentFactory&&) = delete;                  // Move construct
    ArgumentFactory& operator=(ArgumentFactory const&) = delete;  // Copy assign
    ArgumentFactory& operator=(ArgumentFactory &&) = delete;      // Move assign

	public:

		/** 
		\brief Create an argument
 		*/
		template<typename T>
		static ArgumentPtr Create(std::string name,T value) {	
			//Check name
			if(name=="") return 0;

			//Check attr type
			//if(type=="") return 0;
			//...

			return ArgumentPtr(new ArgumentImpl<T>(name,value));
		}//close Create()

		template<typename T>
		static ArgumentPtr Create(std::string name,std::string type,T value) {	
			//Check name
			if(name=="") return 0;

			//Check attr type
			if(type=="") return 0;
			//...

			return ArgumentPtr(new ArgumentImpl<T>(name,type,value));
		}//close Create()

		
		
		static ArgumentPtr CreateFromJson(Json::Value& root) {	
			//Check input json object
			if(root.empty()) return 0;
			if(!root.isMember("name")) return 0;
			if(!root.isMember("type")) return 0;
			if(!root.isMember("value")) return 0;

			//Check attr name
			std::string attr_name= root["name"].asString();
			if(attr_name=="") return 0;
			
			//Check attr type
			std::string attr_type= root["type"].asString();
			if(attr_type=="") return 0;

			//Check against registered attr type
			//...

			//Get value
			Json::Value attr_valueObj= root["value"];
	
			if( attr_type=="DevBoolean" ) {
				return CreateBoolFromJson(root);
			}
			else if(attr_type=="DevLong") {
				return CreateIntFromJson(root);
			}
			else if(attr_type=="DevShort") {
				return CreateShortFromJson(root);
			}
			else if(attr_type=="DevLong64") {
				return CreateLongFromJson(root);
			}
			else if(attr_type=="DevFloat") {
				return CreateFloatFromJson(root);
			}
			else if(attr_type=="DevDouble") {
				return CreateDoubleFromJson(root);
			}
			else if(attr_type=="DevString") {
				return CreateStringFromJson(root);
			}
			else return 0;
			
			return 0;

		}//close Create()


		template<typename T>
		static int GetTypeCode(std::string& type,T value){
			if( std::is_same<T, bool>::value ) type= "DevBoolean";
			else if( std::is_same<T, float>::value ) type= "DevFloat";
			else if( std::is_same<T, double>::value ) type= "DevDouble";
			else if( std::is_same<T, short>::value ) type= "DevShort";
			else if(std::is_same<T, int>::value) type= "DevLong";
			else if(std::is_same<T, long int>::value) type= "DevLong64";
			else if(std::is_same<T, std::string>::value) type= "DevString";
			else if(std::is_same<T, char*>::value) type= "DevString";
			else {
				type= "UNKNOWN";
				return -1;
			}
			return 0;
		}

		

	private:
		static ArgumentPtr CreateBoolFromJson(Json::Value& root){
			std::string attr_name= root["name"].asString();
			std::string attr_type= root["type"].asString();
			Json::Value attr_valueObj= root["value"];
			
			if(attr_valueObj.isArray()){
				std::vector<bool> attr_value;
				for(unsigned int i=0;i<attr_valueObj.size();i++){
					if(!attr_valueObj[i].isBool()) return 0;
					attr_value.push_back( attr_valueObj[i].asBool() );
				}//end loop item
				return Create<std::vector<bool>>(attr_name,attr_type,attr_value);
			}
			if(attr_valueObj.isBool()){
				bool attr_value= attr_valueObj.asBool();
				return Create<bool>(attr_name,attr_type,attr_value);
			}
			return 0;
		}//close CreateBool

		static ArgumentPtr CreateIntFromJson(Json::Value& root){
			std::string attr_name= root["name"].asString();
			std::string attr_type= root["type"].asString();
			Json::Value attr_valueObj= root["value"];
			
			if(attr_valueObj.isArray()){
				std::vector<int> attr_value;
				for(unsigned int i=0;i<attr_valueObj.size();i++){
					Json::Value thisItem= attr_valueObj[i];
					attr_value.push_back( attr_valueObj[i].asInt() );
				}//end loop item
				return Create<std::vector<int>>(attr_name,attr_type,attr_value);
			}
			if(attr_valueObj.isNumeric()){
				int attr_value= attr_valueObj.asInt();
				return Create<int>(attr_name,attr_type,attr_value);
			}
			return 0;
		}//close CreateInt()

		static ArgumentPtr CreateShortFromJson(Json::Value& root){
			std::string attr_name= root["name"].asString();
			std::string attr_type= root["type"].asString();
			Json::Value attr_valueObj= root["value"];
			
			if(attr_valueObj.isArray()){
				std::vector<short> attr_value;
				for(unsigned int i=0;i<attr_valueObj.size();i++){
					if(!attr_valueObj[i].isNumeric()) return 0;
					attr_value.push_back( static_cast<short>(attr_valueObj[i].asInt()) );
				}//end loop item
				return Create<std::vector<short>>(attr_name,attr_type,attr_value);
			}
			if(attr_valueObj.isNumeric()){
				short attr_value= static_cast<short>(attr_valueObj.asInt());
				return Create<short>(attr_name,attr_type,attr_value);
			}
			return 0;
		}//close CreateShort()

		static ArgumentPtr CreateLongFromJson(Json::Value& root){
			std::string attr_name= root["name"].asString();
			std::string attr_type= root["type"].asString();
			Json::Value attr_valueObj= root["value"];
			
			if(attr_valueObj.isArray()){
				std::vector<long int> attr_value;
				for(unsigned int i=0;i<attr_valueObj.size();i++){
					if(!attr_valueObj[i].isNumeric()) return 0;
					attr_value.push_back( static_cast<long int>(attr_valueObj[i].asInt64()) );
				}//end loop item
				return Create<std::vector<long int>>(attr_name,attr_type,attr_value);
			}
			if(attr_valueObj.isNumeric()){
				long int attr_value= static_cast<long int>(attr_valueObj.asInt64());
				return Create<long int>(attr_name,attr_type,attr_value);
			}
			return 0;
		}//close CreateInt()

		static ArgumentPtr CreateFloatFromJson(Json::Value& root){
			std::string attr_name= root["name"].asString();
			std::string attr_type= root["type"].asString();
			Json::Value attr_valueObj= root["value"];
			
			if(attr_valueObj.isArray()){
				std::vector<float> attr_value;
				for(unsigned int i=0;i<attr_valueObj.size();i++){
					if(!attr_valueObj[i].isNumeric()) return 0;
					attr_value.push_back( attr_valueObj[i].asFloat() );
				}//end loop item
				return Create<std::vector<float>>(attr_name,attr_type,attr_value);
			}
			if(attr_valueObj.isNumeric()){
				float attr_value= attr_valueObj.asFloat();
				return Create<float>(attr_name,attr_type,attr_value);
			}
			return 0;
		}//close CreateFloat()

		static ArgumentPtr CreateDoubleFromJson(Json::Value& root){
			std::string attr_name= root["name"].asString();
			std::string attr_type= root["type"].asString();
			Json::Value attr_valueObj= root["value"];
			if(attr_valueObj.isArray()){
				std::vector<double> attr_value;
				for(unsigned int i=0;i<attr_valueObj.size();i++){
					if(!attr_valueObj[i].isNumeric()) return 0;
					attr_value.push_back( attr_valueObj[i].asDouble() );
				}//end loop item
				return Create<std::vector<double>>(attr_name,attr_type,attr_value);
			}
			if(attr_valueObj.isNumeric()){
				double attr_value= attr_valueObj.asDouble();
				return Create<double>(attr_name,attr_type,attr_value);
			}
			return 0;
		}//close CreateDouble()
	
		static ArgumentPtr CreateStringFromJson(Json::Value& root){
			std::string attr_name= root["name"].asString();
			std::string attr_type= root["type"].asString();
			Json::Value attr_valueObj= root["value"];
			
			if(attr_valueObj.isArray()){
				std::vector<std::string> attr_value;
				for(unsigned int i=0;i<attr_valueObj.size();i++){
					if(!attr_valueObj[i].isString()) return 0;
					attr_value.push_back( attr_valueObj[i].asString() );
				}//end loop item
				return Create<std::vector<std::string>>(attr_name,attr_type,attr_value);
			}
			if(attr_valueObj.isString()){
				std::string attr_value= attr_valueObj.asString();
				return Create<std::string>(attr_name,attr_type,attr_value);
			}
			return 0;
		}//close CreateString()

	protected:
		ArgumentFactory(){};
		virtual ~ArgumentFactory(){};

	private:		
		
};

}//close namespace

#endif

