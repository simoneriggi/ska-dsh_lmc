/**
* @file Message.h
* @class Message
* @brief Class storing message info 
* 
* @author S. Riggi
* @date 22/02/2016
*/
#ifndef Message_H
#define Message_H

#include <Argument.h>

#include <tango.h>

//# JSON CPP
#include <json/json.h>

#include <string>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <iomanip>

#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <map>

using namespace std;

namespace MessageParser_ns {


struct MatchArg {
	MatchArg(const std::string& name) : m_name(name) {}
 	bool operator()(const Argument* obj) const {
		std::string arg_name= obj->GetName();
  	return (arg_name==m_name);
 	}
 	private:
  	const std::string& m_name;
};


class Message {

	public:
		Message(){
			m_id= "";
			m_args.clear();	
			m_HasFieldMap.clear();
			m_argMap.clear();
		};
		virtual ~Message(){
			cout<<"~Message(): Calling destructor..."<<endl;
			ClearArgs();
		};

	public: 
		/** 
		\brief Check whether message has field set
 		*/
		bool HasField(std::string name){
			std::map<std::string,bool>::iterator it= m_HasFieldMap.find(name);
			if ( m_HasFieldMap.empty() || it==m_HasFieldMap.end() ) return false;
			return true;
		}

		/** 
		\brief Set id field
 		*/
		void SetId(std::string id){
			if(id=="") return;
			m_id= id;
			SetField(std::string("id"));
		}//close SetId()
		/** 
		\brief Get id field
 		*/
		std::string GetId(){return m_id;}
		

		/** 
		\brief Set arguments
 		*/
		void SetArguments(std::vector<Argument*>& args){
			if(args.size()<=0) return;
			m_args= args;
			SetField(std::string("arguments"));
		}//close SetArguments()

		/** 
		\brief Copy arguments
 		*/
		void CopyArguments(std::vector<Argument*>& args){
			if(args.size()<=0) return;
			ClearArgs();
		
			for(unsigned int i=0;i<args.size();i++){
				m_args.push_back( args[i]->Clone() );	
			}//end loop args
			SetField(std::string("arguments"));
		}//close SetArguments()
		
		/** 
		\brief Get arguments
 		*/
		std::vector<Argument*> GetArguments(){return m_args;}	
		/** 
		\brief Get argument
 		*/
		Argument* GetArgument(int index){
			if(m_args.empty() || index<0 || index>=(int)(m_args.size())) return 0;
			return m_args[index];
		}
		/** 
		\brief Has arguments?
 		*/
		bool HasArguments(){return (m_args.size()>0 && HasField(std::string("arguments")));}


		/** 
		\brief Find argument by name
 		*/
		Argument* FindArgument(int& index,std::string name){
			if(m_args.empty()) return 0;
			std::vector<Argument*>::iterator it = std::find_if(m_args.begin(), m_args.end(), MatchArg(name));
			if (it==m_args.end()) return 0;//not found in collection
			size_t pos = it-m_args.begin();
			index= pos;
			return GetArgument(index);
		}//close FindArgument()


		/** 
		\brief Parse from string
 		*/
		virtual int Parse(std::string& msg){return 0;}

		/** 
		\brief Get json object from class fields
 		*/
		virtual int GetJson(Json::Value&){return 0;}
		/** 
		\brief Get json string from class fields
 		*/
		int GetJsonString(std::string& jsonString,bool isMinified=true){
			//Get json object
			Json::Value root;
			if(GetJson(root)<0){
				return -1;
			}

			//Encode to string
			if(GetJsonString(jsonString,root,isMinified)<0){
				return -1;
			}	
			return 0;
		};
		/** 
		\brief Get json string from json obj
 		*/
		int GetJsonString(std::string& jsonString,Json::Value const& jsonObj,bool isMinified=true) {
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
				cerr<<"Message::GetJsonString(): ERROR: Failed to encode argument to json string!"<<endl;
				return -1;
			}
			return 0;
		};
		/** 
		\brief Validate fields (e.g. mandatory or not, etc)
 		*/
		virtual bool Validate(){
			if( !HasField(std::string("id")) ) {	
				cerr<<"Message::Validate(): ERROR: Missing id field!"<<endl;
				return false;
			}
			return true;
		};
		
		template<typename T>
			void AddArgument(std::string name,T value){
				if(name=="") return;
				ArgumentPtr aNewArg= ArgumentFactory::Instance().Create<T>(name,value);
				if(!aNewArg) return;
				m_args.push_back(aNewArg);
				m_argMap.insert( std::pair<std::string,ArgumentPtr>(name,aNewArg) );
				SetField(std::string("arguments"));
			}

		template<typename T>
			void AddArgument(std::string name,std::string type,T value){
				if(name=="") return;
				ArgumentPtr aNewArg= ArgumentFactory::Instance().Create<T>(name,type,value);
				if(!aNewArg) return;
				m_args.push_back(aNewArg);
				m_argMap.insert( std::pair<std::string,ArgumentPtr>(name,aNewArg) );
				SetField(std::string("arguments"));
			}

		int AddArgumentFromJson(Json::Value& root){
				if(root.empty()) return -1;
				std::string name= root["name"].asString();
				ArgumentPtr aNewArg= ArgumentFactory::Instance().CreateFromJson(root);
				if(!aNewArg) return -1;
				m_args.push_back(aNewArg);
				m_argMap.insert( std::pair<std::string,ArgumentPtr>(name,aNewArg) );
				SetField(std::string("arguments"));
				return 0;
			}

		/** 
		\brief Get argument base pointer
 		*/
		ArgumentPtr GetArgumentBase(std::string name){
			ArgumentMap::iterator it= m_argMap.find(name);
			if ( m_argMap.empty() || it==m_argMap.end() ) return 0;
			return it->second;
		}

		/** 
		\brief Get argument 
 		*/
		template <typename T>
		//std::shared_ptr<ArgumentImpl<T>> GetArgument(std::string name){
			ArgumentImpl<T>* GetArgument(std::string name){
				//Try to dynamical cast to template type
				ArgumentPtr thisArgument= GetArgumentBase(name);
				if(!thisArgument){
					cerr<<"Message::GetArgument(): INFO: Argument "<<name<<" not found!"<<endl;
					return 0;
				}
				//std::shared_ptr<Option<T>> thisCastedArgument= std::dynamic_pointer_cast<ArgumentImpl<T>>(thisArgument);
				ArgumentImpl<T>* thisCastedArgument= dynamic_cast<ArgumentImpl<T>*>(thisArgument);
			
				if(!thisCastedArgument){
					cerr<<"Message::GetArgument(): ERROR: Failed to cast argument to given data type!"<<endl;
					return 0;
				}
				return thisCastedArgument;
			}

		/** 
		\brief Get argument value
 		*/
		template <typename T>
			int GetArgumentValue(std::string name,T& value){
				ArgumentImpl<T>* thisArgument= GetArgument<T>(name);
				if(!thisArgument) return -1;
				value= thisArgument->value;
				return 0;
			}//close GetArgumentValue()		

		/** 
		\brief Get argument pipe blob
 		*/
		std::shared_ptr<Tango::DevicePipeBlob> GetArgumentPipeBlobPtr(){

			
			std::shared_ptr<Tango::DevicePipeBlob> blob= 0;	
			try{				

				//Init blob
				blob= std::make_shared<Tango::DevicePipeBlob>("arguments");
				blob->set_name("arguments");
				std::vector<std::string> field_names;
				for(unsigned int i=0;i<m_args.size();i++){
					std::string arg_name= m_args[i]->GetName();
					field_names.push_back(arg_name);
				}		
				blob->set_data_elt_names(field_names);
	
				//Fill pipe
				blob->set_data_elt_nb(m_args.size());
				for(unsigned int i=0;i<m_args.size();i++){		
					std::string arg_name= m_args[i]->GetName();	
					auto arg_blob= m_args[i]->GetPipeBlobPtr();				
					if(!arg_blob){
						throw std::runtime_error("Failed to get arg pipe blob from task");
					}
					(*blob) << *arg_blob;					
				}//end loop tasks
			}//close try
			catch(Tango::DevFailed& e){
				Tango::Except::print_exception(e);
				cerr<<"Message::GetArgumentPipeBlobPtr(): ERROR: Exception occurred while creating and filling task pipe!"<<endl;
				return 0;
			}
			catch(...){
				cerr<<"Message::GetArgumentPipeBlobPtr(): ERROR: Exception occurred while filling arg collection pipe!"<<endl;
				return 0;
			}

			return blob;	
		}//close GetArgumentPipeBlobPtr()

		/** 
		\brief Get argument pipe blob
 		*/
		/*
		int GetArgumentPipeBlob(Tango::Pipe& pipe){

			//Init arg pipe
			pipe.set_root_blob_name("arguments");
			std::vector<std::string> field_names;
			for(unsigned int i=0;i<m_args.size();i++){
				std::string arg_name= m_args[i].name;
				field_names.push_back(arg_name);
			}		
			pipe.set_data_elt_names(field_names);
				
			//Fill pipe
			try{					
				for(unsigned int i=0;i<m_args.size();i++){					
					auto pipe_blob= m_args[i].GetPipeBlobPtr();				
					if(!pipe_blob){
						throw std::runtime_error("Failed to get arg pipe blob from task");
					}
					pipe << *pipe_blob;					
				}//end loop tasks
			}//close try
			catch(...){
				cerr<<"Message::GetArgumentPipe(): ERROR: Exception occurred while filling arg collection pipe!"<<endl;
				return -1;
			}
			return 0;
		}//close GetPipeBlob()
		*/


	protected:
		void SetField(std::string field_name){
			if(field_name=="") return;
			m_HasFieldMap[field_name]= true;		
		}//close SetField()


	private:
		void ClearArgs(){
			cout<<"Message::ClearArgs(): Clearing args (n="<<m_args.size()<<")"<<endl;
			for(unsigned int i=0;i<m_args.size();i++){
				if(m_args[i]){
					cout<<"Message::ClearArgs(): Clearing arg no "<<i+1<<" ..."<<endl;		
					delete m_args[i];
					m_args[i]= 0;
				}
			}	
			m_args.clear();
		}//close ClearArgs()
	   
		
	protected:
		
		std::string m_id;	
		
		std::vector<Argument*> m_args;	
		std::map<std::string,bool> m_HasFieldMap;
		ArgumentMap m_argMap;
		
};

enum ResponseType {
	ACK_RESPONSE= 0,
	INTERMEDIATE_RESPONSE= 1,
	FINAL_RESPONSE= 2
};
enum ReturnCode {
	ACK= 0,
	NACK= -1
};

class Response : public Message {
	public:
		Response();
		~Response();

	public:
		int GetJson(Json::Value&);
		bool Validate();
		int Parse(std::string& msg);

		/** 
		\brief Set response type field
 		*/
		void SetType(ResponseType code){
			m_type= code;
			SetField(std::string("type"));
		}
		/** 
		\brief Get response type field
 		*/
		short GetType(){return m_type;}


		/** 
		\brief Set return code field
 		*/
		void SetReturnCode(short code){
			m_return_code= code;
			SetField(std::string("return_code"));
		}
		/** 
		\brief Get return code field
 		*/
		short GetReturnCode(){return m_return_code;}
		/** 
		\brief Set response message
 		*/
		void SetResponseMessage(std::string msg){
			if(msg=="") return;
			m_response_msg= msg;
			SetField(std::string("response_msg"));
		}
		/** 
		\brief Get response message
 		*/
		std::string GetResponseMessage(){return m_response_msg;}


		/** 
		\brief Set LMCId
 		*/
		void SetLMCId(std::string id){
			if(id=="") return;
			m_lmc_id= id;
			SetField(std::string("lmc_id"));
		}
		/** 
		\brief Get LMCId
 		*/
		std::string GetLMCId(){return m_lmc_id;}

		/** 
		\brief Print()
 		*/
		void Print(){
			cout<<"== RESPONSE =="<<endl;
			cout<<"id: "<<m_id<<", lmc_id: "<<m_lmc_id<<", type: "<<m_type<<endl;
			cout<<"return_code: "<<m_return_code<<endl;
			cout<<"=============="<<endl;
		}

		/** 
		\brief Get pipe
 		*/
		int GetPipeBlob(Tango::DevicePipeBlob& pipe);

	protected:
		ResponseType m_type;
		short m_return_code;
		std::string m_response_msg;
		std::string m_lmc_id;

	private:

};//close Response class

class Request : public Message {
	public:
		Request();
		~Request();

	public:
		int GetJson(Json::Value&);
		bool Validate();
		int Parse(std::string& msg);

		
		/** 
		\brief Set type field
 		*/
		void SetType(std::string type){
			if(type=="") return;
			m_type= type;
			SetField(std::string("type"));
		}
		/** 
		\brief Get type field
 		*/
		std::string GetType(){return m_type;}
		/** 
		\brief Set source id field
 		*/
		void SetSourceId(std::string id){
			if(id=="") return;
			m_source_id= id;
			SetField(std::string("source_id"));
		}//close SetSourceId()
		/** 
		\brief Get source id field
 		*/
		std::string GetSourceId(){return m_source_id;}
		
		/** 
		\brief Set subarray id field
 		*/
		void SetSubArrayId(short id){
			m_subarray_id= id;
			SetField(std::string("subarray_id"));
		}
		/** 
		\brief Get subarray id field
 		*/
		short GetSubArrayId(){return m_subarray_id;}
		/** 
		\brief Set activation time field
 		*/
		void SetActivationTime(std::string act_time){
			if(act_time=="") return;
			m_activation_time= act_time;
			SetField(std::string("activation_time"));
		}
		/** 
		\brief Get activation time field
 		*/
		std::string GetActivationTime(){return m_activation_time;}
		/** 
		\brief Set expiration time field
 		*/
		void SetExpirationTime(std::string exp_time){
			if(exp_time=="") return;
			m_expiration_time= exp_time;
			SetField(std::string("expiration_time"));
		}
		/** 
		\brief Get expiration time field
 		*/
		std::string GetExpirationTime(){return m_expiration_time;}
		/** 
		\brief Set retry number field
 		*/
		void SetRetryNumber(short retry){
			m_retry_number= retry;
			SetField(std::string("retry_number"));
		}
		/** 
		\brief Get subarray id field
 		*/
		short GetRetryNumber(){return m_retry_number;}

	
	protected:
		std::string m_type;//mandatory
		std::string m_source_id;//mandatory
		short m_retry_number;//mandatory
		short m_subarray_id;//optional
		std::string m_activation_time;//optional
		std::string m_expiration_time;//optional

};//close Request class


class MessageUtils {
	public:
		MessageUtils();
		~MessageUtils();

	public:
		static int MakeEchoResponse(Response& res,Request& req,std::string lmc_id,std::string msg);
		static int MakeEchoResponse(std::string& res,Request& req,std::string lmc_id,std::string msg);
		static int MakeErrorResponse(Response& res,Request& req,std::string lmc_id,std::string msg);
		static int MakeErrorResponse(std::string& res,Request& req,std::string lmc_id,std::string msg);
		static int MakeSuccessResponse(Response& res,Request& req,std::string lmc_id,std::string msg);
		static int MakeSuccessResponse(std::string& res,Request& req,std::string lmc_id,std::string msg);
		

};//close class


}//close namespace

#endif



