#ifndef MONI_POINT_H
#define MONI_POINT_H

#include <tango.h>

#include <pugixml.h>

#include <string>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <iomanip>

#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <map>

namespace SDD_ns
{

class DeviceAttr {
	public:
		virtual ~DeviceAttr() {}
	public:
		virtual Tango::Attr* GetAttr() const = 0;
		virtual void SetAttr(Tango::Attr* a) = 0;
		virtual void SetSubscriptionEndpoint(std::string endpoint) = 0;
		virtual void SetSubscriptionAttr(std::string subattrname) = 0;
		virtual bool IsPolled() const = 0;
		virtual void SetPolled(bool) = 0;
		virtual bool HasSubscriptionPoint() const = 0;
		virtual void SetHasSubscriptionPoint(bool) = 0;
		virtual std::string GetSubscriptionEndPoint() const = 0;
		virtual std::string GetSubscriptionAttrName() const = 0;
};



template <typename T>
class DeviceAttrImpl : public DeviceAttr {
	
	typedef Tango::MultiAttrProp<T> MultiProp;
	
	public:
		DeviceAttrImpl(){
			subscribe_endpoint= "";
			subscribe_attr= "";
			isPolled= false;
			hasSubscriptionPoint= false;
		};
		DeviceAttrImpl(Tango::Attr* a,MultiProp* p) 
			: attr(a), prop(p)
		{
			subscribe_endpoint= "";
			subscribe_attr= "";
			isPolled= false;
			hasSubscriptionPoint= false;
		}
		
		virtual ~DeviceAttrImpl(){
			if(attr){
				delete attr;
				attr= 0;
			}
			if(prop){
				delete prop;
				prop= 0;
			}
		};

		
	public:
		Tango::Attr* GetAttr() const {return attr;} 
		MultiProp* GetProp() const {return prop;}

		virtual void SetAttr(Tango::Attr* aAttr){attr=aAttr;}
		virtual void SetProp(MultiProp* aProp){prop=aProp;}
		virtual void SetSubscriptionEndpoint(std::string endpoint){subscribe_endpoint=endpoint;}
		virtual void SetSubscriptionAttr(std::string subattrname){subscribe_attr=subattrname;}
		bool IsPolled() const {return isPolled;}
		void SetPolled(bool value){isPolled=value;}
		bool HasSubscriptionPoint() const {return hasSubscriptionPoint;}
		void SetHasSubscriptionPoint(bool value){hasSubscriptionPoint=value;}
		std::string GetSubscriptionEndPoint() const {return subscribe_endpoint;}
		std::string GetSubscriptionAttrName() const {return subscribe_attr;}

	public:
		Tango::Attr* attr;
		MultiProp* prop;
		bool hasSubscriptionPoint;
		std::string subscribe_endpoint;
		std::string subscribe_attr;
		bool isPolled;		

};//close DeviceAttr


class MoniPoint {
   	
	public:

		MoniPoint();
		~MoniPoint();


	public:	
		
		int GetAttrDataFormat(std::string dataFormatTag);
		Tango::AttrWriteType GetAttrRWType(std::string dataRWTag);
		long GetAttrDataType(std::string dataTypeTag);
		void Dump();
		void DumpProperties();

		int ParseFromXML(pugi::xml_node mp,bool filter=false,std::string component_filter="",int componentId_filter=-1);

		template<typename T>
			int setMultiAttrProp(Tango::MultiAttrProp<T>& prop)
			{
				
				if(has_description) prop.description= description;
				prop.label= name;
				if(has_unit) prop.unit= unit;	
				
				
				if(has_data_range){
					prop.min_value= static_cast<T>(min_data_value);
					prop.max_value= static_cast<T>(max_data_value);
				}
				if(has_alarm_range){
					prop.min_alarm= static_cast<T>(min_data_alarm);
					prop.max_alarm= static_cast<T>(max_data_alarm);
				}
				
				if(has_rel_change_event) prop.rel_change= rel_change_value;
				if(has_abs_change_event) prop.abs_change= abs_change_value;
				if(has_periodic_event) prop.event_period= event_period;
				
				if(has_enum_labels && data_type==Tango::DEV_ENUM) prop.enum_labels= enum_labels;
				
				return 0;				
			}

		/*
		template<typename T>
			T getValueFromString(std::string input){
				T value;
				std::stringstream sstream;
				sstream.str(input);
				sstream>>value;
				return value;
			}
		*/
		
		
	public:
		//General
		int id;
		bool has_description;
		std::string description;
		bool has_unit;
		std::string unit;
		std::string name;
		std::string alias_name;
		long int data_type;
		Tango::AttrWriteType rw_type;
		int data_format;

		//Source device
		std::string device_address;
		std::string component_name;
		int component_id;

		//Data range
		bool has_data_range;
		double min_data_value;
		double max_data_value;
		bool has_alarm_range;
		double min_data_alarm;
		double max_data_alarm;
		bool has_enum_labels;
		std::vector<std::string> enum_labels;

		//Events
		bool subscribe_events;	
		bool is_subscribed;
		std::vector<int> event_ids;
		bool has_events;	
		bool has_rel_change_event;
		double rel_change_value;
		bool has_abs_change_event;
		double abs_change_value;
		bool has_push_change_event;
		bool has_periodic_event;
		double event_period;

		//Polling
		bool is_polled;
		int poll_period;
};

typedef std::vector<MoniPoint*> MoniPoints;
	

class MoniPointFinder {
	std::string name_;
	bool useAlias_;

	public:
  	MoniPointFinder(const std::string& name,const bool& useAlias=true)
			: name_(name),useAlias_(useAlias) {}

  	bool operator()(const SDD_ns::MoniPoint* mp){ 
			if(!useAlias_) return mp->name == name_;
			return mp->alias_name == name_; 
		}
};


}//close namespace

#endif


