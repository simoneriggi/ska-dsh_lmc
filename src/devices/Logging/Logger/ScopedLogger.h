
#ifndef ScopedLogger_H
#define ScopedLogger_H

#include <tango.h>
#include <log4tango.h>

//== LOG4CXX HEADERS ==
#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/simplelayout.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/net/syslogappender.h>
#include <log4cxx/helpers/exception.h>

namespace LMCLogger_ns {

struct ScopedLogger {
			
	//Constructor
	ScopedLogger(std::string device_name,log4tango::Level::Value level)
  	: m_deviceName(device_name), m_level(level) 
	{
		//
	}
			
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
			//cout<<"~ScopedLogger(): Tango logging @ level "<<m_level<<endl;
			ds_impl->get_logger()->log(m_level, m_sstream.str()); 
		}
			
		//Syslog logging
		log4cxx::LoggerPtr syslogger = log4cxx::Logger::getLogger("sysLogger");
		log4cxx::LevelPtr syslevel= GetSysLogLevel(m_level);
		//cout<<"~ScopedLogger(): syslogger?"<<syslogger<<", syslevel="<<syslevel->toString()<<" enabled? "<<syslogger->isEnabledFor(syslevel)<<endl;
					
		if(syslogger && syslogger->isEnabledFor(syslevel) ){
			//cout<<"~ScopedLogger(): SysLog logging @ level "<<syslevel->toString()<<endl;
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

}//close namespace

#endif

