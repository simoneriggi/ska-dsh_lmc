/*----- PROTECTED REGION ID(BaseDevice.h) ENABLED START -----*/
//=============================================================================
//
// file :        BaseDevice.h
//
// description : Include file for the BaseDevice class
//
// project :     Base device
//
// This file is part of Tango device class.
// 
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
// 
// $Author:  $
//
// $Revision:  $
// $Date:  $
//
// $HeadURL:  $
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#ifndef BaseDevice_H
#define BaseDevice_H

#include <AttrCallBack.h>

#include <tango.h>

#include <MoniPoint.h>
#include <SDD.h>
#include <SDD_XML.h>

#include <boost/log/sinks/sink.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
using namespace boost::log;

#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/simplelayout.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/net/syslogappender.h>
#include <log4cxx/helpers/exception.h>

//-------------------------------------------------------------
// LOGGING MACROS (FOR DEVICE DEVELOPERS)
//-------------------------------------------------------------
#define LOG(Level, What) \
  ScopedLogger(get_name(),Level).stream() << What

#define INFO_LOG(What) \
  ScopedLogger(get_name(),log4tango::Level::INFO).stream() << What

#define WARN_LOG(What) \
  ScopedLogger(get_name(),log4tango::Level::WARN).stream() << What

#define DEBUG_LOG(What) \
  ScopedLogger(get_name(),log4tango::Level::DEBUG).stream() << What

#define ERROR_LOG(What) \
  ScopedLogger(get_name(),log4tango::Level::ERROR).stream() << What

#define FATAL_LOG(What) \
  ScopedLogger(get_name(),log4tango::Level::FATAL).stream() << What

/*----- PROTECTED REGION END -----*/	//	BaseDevice.h

/**
 *  BaseDevice class description:
 *    Base device used to export basic functionalities to all other devices.
 */

namespace BaseDevice_ns
{
enum _dynEnumAttrEnum {
} ;
typedef _dynEnumAttrEnum dynEnumAttrEnum;

/*----- PROTECTED REGION ID(BaseDevice::Additional Class Declarations) ENABLED START -----*/

//	Additional Class Declarations
	class AttrCallBack;
/*----- PROTECTED REGION END -----*/	//	BaseDevice::Additional Class Declarations

class BaseDevice : public TANGO_BASE_CLASS
{

/*----- PROTECTED REGION ID(BaseDevice::Data Members) ENABLED START -----*/

//	Add your own data members

/*----- PROTECTED REGION END -----*/	//	BaseDevice::Data Members

//	Device property data members
public:
	//	configFile:	XML configuration file
	string	configFile;
	//	syslog_level:	Log level used for logging to syslog (OFF,TRACE,DEBUG,INFO,WARN,ERROR,FATAL)
	string	syslog_level;
	//	syslog_facility:	LOG_USER: A miscellaneous user process
	//  LOG_MAIL: Mail
	//  LOG_DAEMON: A miscellaneous system daemon
	//  LOG_AUTH:  Security (authorization)
	//  LOG_SYSLOG: Syslog
	//  LOG_LPR: Central printer
	//  LOG_NEWS: Network news (e.g. Usenet)
	//  LOG_UUCP: UUCP
	//  LOG_CRON: Cron and At
	//  LOG_AUTHPRIV: Private security (authorization)
	//  LOG_FTP: Ftp server
	//  LOG_LOCAL0,1,2,3,4,5,6,7: Locally defined
	//  
	//  ``auth``: LOG_AUTH
	//  ``authpriv``: LOG_AUTHPRIV
	//  ``cron``: LOG_CRON 
	//  ``daemon``: LOG_DAEMON
	//  ``ftp``: LOG_FTP
	//  ``kern``: LOG_KERN
	//  ``lpr``: LOG_LPR
	//  ``mail``: LOG_MAIL
	//  ``mark``: INTERNAL_MARK
	//  ``news``: LOG_NEWS
	//  ``security``: LOG_AUTH,
	//  ``syslog``: LOG_SYSLOG
	//  ``user``: LOG_USER
	//  ``uucp``: LOG_UUCP
	//  ``local0``: LOG_LOCAL0
	//  ``local1``: LOG_LOCAL1 
	//  ``local2``: LOG_LOCAL2 
	//  ``local3``: LOG_LOCAL3 
	//  ``local4``: LOG_LOCAL4 
	//  ``local5``: LOG_LOCAL5 
	//  ``local6``: LOG_LOCAL6 
	//  ``local7``: LOG_LOCAL7
	string	syslog_facility;


//	Constructors and destructors
public:
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	BaseDevice(Tango::DeviceClass *cl,string &s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	BaseDevice(Tango::DeviceClass *cl,const char *s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device name
	 *	@param d	Device description.
	 */
	BaseDevice(Tango::DeviceClass *cl,const char *s,const char *d);
	/**
	 * The device object destructor.
	 */
	~BaseDevice() {delete_device();};


//	Miscellaneous methods
public:
	/*
	 *	will be called at device destruction or at init command.
	 */
	void delete_device();
	/*
	 *	Initialize the device
	 */
	virtual void init_device();
	/*
	 *	Read the device properties from database
	 */
	void get_device_property();
	/*
	 *	Always executed method before execution command method.
	 */
	virtual void always_executed_hook();


//	Attribute methods
public:
	//--------------------------------------------------------
	/*
	 *	Method      : BaseDevice::read_attr_hardware()
	 *	Description : Hardware acquisition for attributes.
	 */
	//--------------------------------------------------------
	virtual void read_attr_hardware(vector<long> &attr_list);

//	Dynamic attribute methods
public:

	/**
	 *	Attribute dynFloatAttr related methods
	 *	Description: This is a template float dyn attr
	 *
	 *	Data type:	Tango::DevFloat
	 *	Attr type:	Scalar
	 */
	virtual void read_dynFloatAttr(Tango::Attribute &attr);
	virtual bool is_dynFloatAttr_allowed(Tango::AttReqType type);
	void add_dynFloatAttr_dynamic_attribute(string attname);
	void remove_dynFloatAttr_dynamic_attribute(string attname);
	Tango::DevFloat *get_dynFloatAttr_data_ptr(string &name);
	map<string,Tango::DevFloat>	   dynFloatAttr_data;

	/**
	 *	Attribute dynDoubleAttr related methods
	 *	Description: 
	 *
	 *	Data type:	Tango::DevDouble
	 *	Attr type:	Scalar
	 */
	virtual void read_dynDoubleAttr(Tango::Attribute &attr);
	virtual bool is_dynDoubleAttr_allowed(Tango::AttReqType type);
	void add_dynDoubleAttr_dynamic_attribute(string attname);
	void remove_dynDoubleAttr_dynamic_attribute(string attname);
	Tango::DevDouble *get_dynDoubleAttr_data_ptr(string &name);
	map<string,Tango::DevDouble>	   dynDoubleAttr_data;

	/**
	 *	Attribute dynStringAttr related methods
	 *	Description: 
	 *
	 *	Data type:	Tango::DevString
	 *	Attr type:	Scalar
	 */
	virtual void read_dynStringAttr(Tango::Attribute &attr);
	virtual bool is_dynStringAttr_allowed(Tango::AttReqType type);
	void add_dynStringAttr_dynamic_attribute(string attname);
	void remove_dynStringAttr_dynamic_attribute(string attname);
	Tango::DevString *get_dynStringAttr_data_ptr(string &name);
	map<string,Tango::DevString>	   dynStringAttr_data;

	/**
	 *	Attribute dynEnumAttr related methods
	 *	Description: 
	 *
	 *	Data type:	Tango::DevEnum
	 *	Attr type:	Scalar
	 */
	virtual void read_dynEnumAttr(Tango::Attribute &attr);
	virtual bool is_dynEnumAttr_allowed(Tango::AttReqType type);
	void add_dynEnumAttr_dynamic_attribute(string attname);
	void remove_dynEnumAttr_dynamic_attribute(string attname);
	Tango::DevEnum *get_dynEnumAttr_data_ptr(string &name);
	map<string,Tango::DevEnum>	   dynEnumAttr_data;

	/**
	 *	Attribute dynLongAttr related methods
	 *	Description: 
	 *
	 *	Data type:	Tango::DevLong
	 *	Attr type:	Scalar
	 */
	virtual void read_dynLongAttr(Tango::Attribute &attr);
	virtual bool is_dynLongAttr_allowed(Tango::AttReqType type);
	void add_dynLongAttr_dynamic_attribute(string attname);
	void remove_dynLongAttr_dynamic_attribute(string attname);
	Tango::DevLong *get_dynLongAttr_data_ptr(string &name);
	map<string,Tango::DevLong>	   dynLongAttr_data;

	/**
	 *	Attribute dynShortAttr related methods
	 *	Description: 
	 *
	 *	Data type:	Tango::DevShort
	 *	Attr type:	Scalar
	 */
	virtual void read_dynShortAttr(Tango::Attribute &attr);
	virtual bool is_dynShortAttr_allowed(Tango::AttReqType type);
	void add_dynShortAttr_dynamic_attribute(string attname);
	void remove_dynShortAttr_dynamic_attribute(string attname);
	Tango::DevShort *get_dynShortAttr_data_ptr(string &name);
	map<string,Tango::DevShort>	   dynShortAttr_data;

	/**
	 *	Attribute dynFloatArrayAttr related methods
	 *	Description: 
	 *
	 *	Data type:	Tango::DevFloat
	 *	Attr type:	Spectrum max = 1000
	 */
	virtual void read_dynFloatArrayAttr(Tango::Attribute &attr);
	virtual bool is_dynFloatArrayAttr_allowed(Tango::AttReqType type);
	void add_dynFloatArrayAttr_dynamic_attribute(string attname, Tango::DevFloat *ptr=NULL);
	void remove_dynFloatArrayAttr_dynamic_attribute(string attname, bool free_it=true);
	Tango::DevFloat *get_dynFloatArrayAttr_data_ptr(string &name);
	map<string,Tango::DevFloat *>	   dynFloatArrayAttr_data;

	/**
	 *	Attribute dynDoubleArrayAttr related methods
	 *	Description: 
	 *
	 *	Data type:	Tango::DevDouble
	 *	Attr type:	Spectrum max = 1000
	 */
	virtual void read_dynDoubleArrayAttr(Tango::Attribute &attr);
	virtual bool is_dynDoubleArrayAttr_allowed(Tango::AttReqType type);
	void add_dynDoubleArrayAttr_dynamic_attribute(string attname, Tango::DevDouble *ptr=NULL);
	void remove_dynDoubleArrayAttr_dynamic_attribute(string attname, bool free_it=true);
	Tango::DevDouble *get_dynDoubleArrayAttr_data_ptr(string &name);
	map<string,Tango::DevDouble *>	   dynDoubleArrayAttr_data;

	/**
	 *	Attribute dynStringArrayAttr related methods
	 *	Description: 
	 *
	 *	Data type:	Tango::DevString
	 *	Attr type:	Spectrum max = 1000
	 */
	virtual void read_dynStringArrayAttr(Tango::Attribute &attr);
	virtual bool is_dynStringArrayAttr_allowed(Tango::AttReqType type);
	void add_dynStringArrayAttr_dynamic_attribute(string attname, Tango::DevString *ptr=NULL);
	void remove_dynStringArrayAttr_dynamic_attribute(string attname, bool free_it=true);
	Tango::DevString *get_dynStringArrayAttr_data_ptr(string &name);
	map<string,Tango::DevString *>	   dynStringArrayAttr_data;

	/**
	 *	Attribute dynLongArrayAttr related methods
	 *	Description: 
	 *
	 *	Data type:	Tango::DevLong
	 *	Attr type:	Spectrum max = 1000
	 */
	virtual void read_dynLongArrayAttr(Tango::Attribute &attr);
	virtual bool is_dynLongArrayAttr_allowed(Tango::AttReqType type);
	void add_dynLongArrayAttr_dynamic_attribute(string attname, Tango::DevLong *ptr=NULL);
	void remove_dynLongArrayAttr_dynamic_attribute(string attname, bool free_it=true);
	Tango::DevLong *get_dynLongArrayAttr_data_ptr(string &name);
	map<string,Tango::DevLong *>	   dynLongArrayAttr_data;

	/**
	 *	Attribute dynShortArrayAttr related methods
	 *	Description: 
	 *
	 *	Data type:	Tango::DevShort
	 *	Attr type:	Spectrum max = 1000
	 */
	virtual void read_dynShortArrayAttr(Tango::Attribute &attr);
	virtual bool is_dynShortArrayAttr_allowed(Tango::AttReqType type);
	void add_dynShortArrayAttr_dynamic_attribute(string attname, Tango::DevShort *ptr=NULL);
	void remove_dynShortArrayAttr_dynamic_attribute(string attname, bool free_it=true);
	Tango::DevShort *get_dynShortArrayAttr_data_ptr(string &name);
	map<string,Tango::DevShort *>	   dynShortArrayAttr_data;

	//--------------------------------------------------------
	/**
	 *	Method      : BaseDevice::add_dynamic_attributes()
	 *	Description : Add dynamic attributes if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_attributes();




//	Command related methods
public:
	/**
	 *	Command DevConfigure related method
	 *	Description: Configure device using the input configuration string
	 *
	 *	@param argin A string with SDD configuration
	 *	@returns 
	 */
	virtual Tango::DevVarLongStringArray *dev_configure(Tango::DevString argin);
	virtual bool is_DevConfigure_allowed(const CORBA::Any &any);
	/**
	 *	Command RestoreDevConfig related method
	 *	Description: Restore device configuration to the default
	 *
	 *	@returns 
	 */
	virtual Tango::DevVarLongStringArray *restore_dev_config();
	virtual bool is_RestoreDevConfig_allowed(const CORBA::Any &any);
	/**
	 *	Command RemoveAttr related method
	 *	Description: Remove an attribute from the list
	 *
	 *	@param argin Attr name
	 *	@returns 
	 */
	virtual Tango::DevVarLongStringArray *remove_attr(Tango::DevString argin);
	virtual bool is_RemoveAttr_allowed(const CORBA::Any &any);
	/**
	 *	Command RemoveAttrs related method
	 *	Description: Remove all attributes from the device
	 *
	 *	@returns 
	 */
	virtual Tango::DevVarLongStringArray *remove_attrs();
	virtual bool is_RemoveAttrs_allowed(const CORBA::Any &any);
	/**
	 *	Command SubscribeAttr related method
	 *	Description: Subscribe attribute to remote monitoring point
	 *
	 *	@param argin 
	 *	@returns 
	 */
	virtual Tango::DevVarLongStringArray *subscribe_attr(Tango::DevString argin);
	virtual bool is_SubscribeAttr_allowed(const CORBA::Any &any);
	/**
	 *	Command UnsubscribeAttr related method
	 *	Description: Unsubscribe attr from remote monitoring point
	 *
	 *	@param argin Attr name
	 *	@returns 
	 */
	virtual Tango::DevVarLongStringArray *unsubscribe_attr(Tango::DevString argin);
	virtual bool is_UnsubscribeAttr_allowed(const CORBA::Any &any);
	/**
	 *	Command SubscribeAttrs related method
	 *	Description: Subscribe all attributes to remote monitoring points
	 *
	 *	@returns 
	 */
	virtual Tango::DevVarLongStringArray *subscribe_attrs();
	virtual bool is_SubscribeAttrs_allowed(const CORBA::Any &any);
	/**
	 *	Command UnsubscribeAttrs related method
	 *	Description: Unsubscribe all attributes from remote monitoring points
	 *
	 *	@returns 
	 */
	virtual Tango::DevVarLongStringArray *unsubscribe_attrs();
	virtual bool is_UnsubscribeAttrs_allowed(const CORBA::Any &any);


	//--------------------------------------------------------
	/**
	 *	Method      : BaseDevice::add_dynamic_commands()
	 *	Description : Add dynamic commands if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_commands();

/*----- PROTECTED REGION ID(BaseDevice::Additional Method prototypes) ENABLED START -----*/

//	Additional Method prototypes
	protected:
		int ConfigureDevice(std::string& config,bool readFromFile);
		int AddScalarAttr(SDD_ns::DeviceAttr* device_attr);
		int AddSpectrumAttr(SDD_ns::DeviceAttr* device_attr);
		int SubscribeAttr(SDD_ns::DeviceAttr* device_attr);

		int InitBoostSysLogger();
		virtual int InitSysLogger();

		
		/**
	 	*	Define scoped logger
	 	*/
		struct ScopedLogger {
			//Constructor
			ScopedLogger(std::string device_name,log4tango::Level::Value level)
    		: m_deviceName(device_name), m_level(level)
  		{}
			//Get stream
  		std::stringstream& stream(){ return m_sstream; }

			//Get syslog level
			log4cxx::LevelPtr GetSysLogLevel(log4tango::Level::Value level){
				log4cxx::LevelPtr syslevel= log4cxx::Level::toLevel("OFF");
				switch(level){
					case log4tango::Level::OFF :
						syslevel= log4cxx::Level::toLevel("OFF");
						break;
					case log4tango::Level::INFO :
						syslevel= log4cxx::Level::toLevel("INFO");
						break;
					case log4tango::Level::DEBUG :
						syslevel= log4cxx::Level::toLevel("DEBUG");
						break;
					case log4tango::Level::FATAL :
						syslevel= log4cxx::Level::toLevel("FATAL");
						break;
					case log4tango::Level::WARN :
						syslevel= log4cxx::Level::toLevel("WARN");
						break;
					case log4tango::Level::ERROR :
						syslevel= log4cxx::Level::toLevel("ERROR");
						break;
					default :
						syslevel= log4cxx::Level::toLevel("OFF");
						break;
				}//close switch
				return syslevel;
			}//close GetSysLogLevel()

			//Destructor
  		~ScopedLogger(){ 
				//Tango logging	
				Tango::DeviceImpl* ds_impl= Tango::Util::instance()->get_device_by_name(m_deviceName);
				if(ds_impl && ds_impl->get_logger()->is_level_enabled(m_level) ) {
					cout<<"~ScopedLogger(): Tango logging @ level "<<m_level<<endl;
					ds_impl->get_logger()->log(m_level, m_sstream.str()); 
				}
			
				//Syslog logging
				log4cxx::LoggerPtr syslogger = log4cxx::Logger::getLogger("sysLogger");
				log4cxx::LevelPtr syslevel= GetSysLogLevel(m_level);
				cout<<"~ScopedLogger(): syslogger?"<<syslogger<<", syslevel="<<syslevel->toString()<<" enabled? "<<syslogger->isEnabledFor(syslevel)<<endl;
					
				if(syslogger && syslogger->isEnabledFor(syslevel) ){
					cout<<"~ScopedLogger(): SysLog logging @ level "<<syslevel->toString()<<endl;
					log4cxx::helpers::MessageBuffer oss_; \
          syslogger->forcedLog(syslevel, oss_.str(oss_ << m_sstream.str()), log4cxx::spi::LocationInfo(m_deviceName.c_str(),__LOG4CXX_FUNC__,__LINE__) );
					//syslogger->log (syslevel, m_sstream.str(),log4cxx::spi::LocationInfo(m_deviceName.c_str(),__LOG4CXX_FUNC__,__LINE__) );
				}
			}//close destructor

			private:
				std::string m_deviceName;
  			std::stringstream m_sstream;
  			log4tango::Level::Value m_level;
		};//close ScopedLogger struct


	protected:
		enum severity_levels {normal,warning,error};
		typedef sinks::synchronous_sink< sinks::syslog_backend > sink_t;


		omni_mutex* mutex;		
		Tango::DeviceProxy* interface_device;
		AttrCallBack* attr_callback;

	friend class AttrCallBack;

/*----- PROTECTED REGION END -----*/	//	BaseDevice::Additional Method prototypes
};

/*----- PROTECTED REGION ID(BaseDevice::Additional Classes Definitions) ENABLED START -----*/

//	Additional Classes Definitions

/*----- PROTECTED REGION END -----*/	//	BaseDevice::Additional Classes Definitions

}	//	End of namespace

#endif   //	BaseDevice_H
