
#ifndef ScopedLogger_H
#define ScopedLogger_H

#include <tango.h>
#include <log4tango.h>

//#include <LMCLogger.h>

//== LOG4CXX HEADERS ==
#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/simplelayout.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/net/syslogappender.h>
#include <log4cxx/helpers/exception.h>

//Boost headers
//#define BOOST_LOG_DYN_LINK 1 // necessary when linking the boost_log library dynamically
#include <boost/log/core/core.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks/sink.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <boost/log/trivial.hpp>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

namespace LMCLogger_ns {

struct ScopedLogger {
			
	//Constructor
	ScopedLogger(std::string device_name,log4tango::Level::Value level,std::string prefix="")
  	: m_deviceName(device_name), m_level(level), m_msgprefix(prefix) 
	{
		//Transform to lower case
		std::transform(m_deviceName.begin(), m_deviceName.end(), m_deviceName.begin(), ::tolower);
	}
			
	//Get stream
  std::stringstream& stream(){ 	
		return m_sstream; 
	}

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
		//Prepend prefix
		
	
		//Tango logging	
		Tango::DeviceImpl* ds_impl= Tango::Util::instance()->get_device_by_name(m_deviceName);
		if(ds_impl && ds_impl->get_logger()->is_level_enabled(m_level) ) {
			//ds_impl->get_logger()->log(m_level, m_sstream.str()); 
			ds_impl->get_logger()->log(m_level, m_msgprefix+m_sstream.str());
		}
			
		//Syslog logging
		//log4cxx::LoggerPtr syslogger = log4cxx::Logger::getLogger("sysLogger");
		log4cxx::LoggerPtr syslogger = log4cxx::Logger::getLogger(m_deviceName.c_str());
		log4cxx::LevelPtr syslevel= GetSysLogLevel(m_level);
					
		if(syslogger && syslogger->isEnabledFor(syslevel) ){
			log4cxx::helpers::MessageBuffer oss_; \
      syslogger->forcedLog(syslevel, oss_.str(oss_ << m_msgprefix << m_sstream.str()), log4cxx::spi::LocationInfo(m_deviceName.c_str(),__LOG4CXX_FUNC__,__LINE__) );
			//syslogger->log (syslevel, m_sstream.str(),log4cxx::spi::LocationInfo(m_deviceName.c_str(),__LOG4CXX_FUNC__,__LINE__) );
		}
	}//close destructor

	private:
		std::string m_deviceName;
  	std::stringstream m_sstream;
  	log4tango::Level::Value m_level;
		std::string m_msgprefix;
		
};//close ScopedLogger struct


enum boost_severity_level {	
	boost_off= 0,
	boost_fatal= 1,
	boost_error= 2,
  boost_warn= 3,
	boost_info= 4,
  boost_debug= 5
};
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity",boost_severity_level)


struct BoostScopedLogger {
			
	//Constructor
	BoostScopedLogger(std::string device_name,log4tango::Level::Value level,std::string prefix="")
  	: m_deviceName(device_name), m_level(level), m_msgprefix(prefix) 
	{
		//Transform to lower case
		std::transform(m_deviceName.begin(), m_deviceName.end(), m_deviceName.begin(), ::tolower);
	}
			
	//Get stream
  std::stringstream& stream(){ return m_sstream; }

	//Get syslog level
	boost_severity_level GetSysLogLevel(log4tango::Level::Value level){			

		boost_severity_level syslevel= boost_off;		
		switch(level){
			case log4tango::Level::OFF :
				syslevel= boost_off;
				break;
			case log4tango::Level::INFO :
				syslevel= boost_info;	
				break;
			case log4tango::Level::DEBUG :
				syslevel= boost_debug;	
				break;
			case log4tango::Level::FATAL :
				syslevel= boost_fatal;	
				break;
			case log4tango::Level::WARN :
				syslevel= boost_warn;	
				break;
			case log4tango::Level::ERROR :
				syslevel= boost_error;	
				break;
			default :
				syslevel= boost_off;	
				break;
		}//close switch
				
		return syslevel;
	}//close GetSysLogLevel()

	//Destructor
  ~BoostScopedLogger(){ 
		//Tango logging	
		Tango::DeviceImpl* ds_impl= Tango::Util::instance()->get_device_by_name(m_deviceName);
		if(ds_impl && ds_impl->get_logger()->is_level_enabled(m_level) ) {
			ds_impl->get_logger()->log(m_level, m_msgprefix+m_sstream.str()); 
		}
			
		//Syslog logging
		boost::log::core_ptr syslogger= boost::log::core::get();
		boost_severity_level syslevel= GetSysLogLevel(m_level);
					
		if(syslogger && syslogger->get_logging_enabled() ){
			boost::log::sources::severity_logger_mt<boost_severity_level> lg(boost::log::keywords::severity = boost_off);
			BOOST_LOG_SEV(lg, syslevel) << m_msgprefix << m_sstream.str();
		}
	}//close destructor

	private:
		std::string m_deviceName;
  	std::stringstream m_sstream;
  	log4tango::Level::Value m_level;
		std::string m_msgprefix;
		
};//close BoostScopedLogger struct

}//close namespace


//-------------------------------------------------------------
// LOGGING MACROS (FOR DEVICE DEVELOPERS)
//-------------------------------------------------------------
/*
inline std::string getClassName(const char* fullFuncName,const char* funcName=""){
	std::string fullFuncNameStr(fullFuncName);
	std::string funcNameStr= "::";
	funcNameStr+= std::string(funcName);
	//size_t pos = fullFuncNameStr.find_last_of("::");
	size_t pos = fullFuncNameStr.find_last_of(funcNameStr.c_str());
  if (pos == std::string::npos) {
  	return "";
  }
  return fullFuncNameStr.substr(0, pos-1);
}
*/

inline std::string getClassName(std::string fullFuncName,std::string funcName){

	//Init pattern to be searched
	std::string result= "";
	std::string pattern("::(.*)::");//FIX ME!!!
	pattern+= funcName;

	//Create regex
	boost::regex expression;
	try {
  	expression = pattern;
  }
  catch (boost::regex_error& e) {
  	return result;
  }

	//Find match
	boost::smatch matches;
	if (boost::regex_search(fullFuncName, matches, expression) && matches.size()>1) {
		result= std::string(matches[1].first, matches[1].second);
  }
	
	return result;

}//close function

#define __CLASS__ getClassName(__PRETTY_FUNCTION__,__FUNCTION__)
#define __DEVICE_CLASS(deviceInstance) deviceInstance->get_device_class()->get_name()
//#define LOG_PREFIX(deviceInstance) __DEVICE_CLASS(deviceInstance) + std::string("::") + __FUNCTION__ + std::string("() - ")
#define LOG_PREFIX(deviceInstance) __CLASS__ + std::string("::") + __FUNCTION__ + std::string("() - ")

//== LOG4CXX MACROS
#define DLOG(deviceInstance,Level, What) \
  ScopedLogger(deviceInstance->get_name(),Level,LOG_PREFIX(deviceInstance)).stream() << What

//#define LOG(Level, What) ScopedLogger(get_name(),Level,LOG_PREFIX).stream() << What
#define LOG(Level, What) \
  DLOG(this,Level,What)

//#define INFO_LOG(What) ScopedLogger(get_name(),log4tango::Level::INFO,LOG_PREFIX).stream() << What
#define INFO_LOG(What) \
  DLOG(this,log4tango::Level::INFO,What)

//#define WARN_LOG(What) ScopedLogger(get_name(),log4tango::Level::WARN,LOG_PREFIX).stream() << What
#define WARN_LOG(What) \
  DLOG(this,log4tango::Level::WARN,What)

//#define DEBUG_LOG(What) ScopedLogger(get_name(),log4tango::Level::DEBUG,LOG_PREFIX).stream() << What
#define DEBUG_LOG(What) \
  DLOG(this,log4tango::Level::DEBUG,What)

//#define ERROR_LOG(What) ScopedLogger(get_name(),log4tango::Level::ERROR,LOG_PREFIX).stream() << What
#define ERROR_LOG(What) \
  DLOG(this,log4tango::Level::ERROR,What)

//#define FATAL_LOG(What) ScopedLogger(get_name(),log4tango::Level::FATAL,LOG_PREFIX).stream() << What
#define FATAL_LOG(What) \
  DLOG(this,log4tango::Level::FATAL,What)


//== BOOST MACROS
#define DLOG_BOOST(deviceInstance,Level, What) \
  BoostScopedLogger(deviceInstance->get_name(),Level,LOG_PREFIX(deviceInstance)).stream() << What

//#define LOG_BOOST(Level, What) BoostScopedLogger(get_name(),Level,LOG_PREFIX).stream() << What
#define LOG_BOOST(Level, What) \
  DLOG_BOOST(this,Level,What)

//#define INFO_LOG_BOOST(What) BoostScopedLogger(get_name(),log4tango::Level::INFO,LOG_PREFIX(deviceInstance)).stream() << What
#define INFO_LOG_BOOST(Level, What) \
  DLOG_BOOST(this,log4tango::Level::INFO,What)

//#define WARN_LOG_BOOST(What) BoostScopedLogger(get_name(),log4tango::Level::WARN,LOG_PREFIX(deviceInstance)).stream() << What
#define WARN_LOG_BOOST(Level, What) \
  DLOG_BOOST(this,log4tango::Level::WARN,What)

//#define DEBUG_LOG_BOOST(What) BoostScopedLogger(get_name(),log4tango::Level::DEBUG,LOG_PREFIX(deviceInstance)).stream() << What
#define DEBUG_LOG_BOOST(Level, What) \
  DLOG_BOOST(this,log4tango::Level::DEBUG,What)

//#define ERROR_LOG_BOOST(What) BoostScopedLogger(get_name(),log4tango::Level::ERROR,LOG_PREFIX(deviceInstance)).stream() << What
#define ERROR_LOG_BOOST(Level, What) \
  DLOG_BOOST(this,log4tango::Level::ERROR,What)

//#define FATAL_LOG_BOOST(What) BoostScopedLogger(get_name(),log4tango::Level::FATAL,LOG_PREFIX(deviceInstance)).stream() << What
#define FATAL_LOG_BOOST(Level, What) \
  DLOG_BOOST(this,log4tango::Level::FATAL,What)

#endif

