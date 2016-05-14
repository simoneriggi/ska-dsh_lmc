/*----- PROTECTED REGION ID(LMCLogger.h) ENABLED START -----*/
//=============================================================================
//
// file :        LMCLogger.h
//
// description : Include file for the LMCLogger class
//
// project :     LMCLogger
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


#ifndef LMCLogger_H
#define LMCLogger_H

#include <tango.h>

#include <ScopedLogger.h>


//== LOG4CXX HEADERS ==
#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/simplelayout.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/net/syslogappender.h>
#include <log4cxx/helpers/exception.h>


/*----- PROTECTED REGION END -----*/	//	LMCLogger.h

/**
 *  LMCLogger class description:
 *    LMCLogger device server
 */

namespace LMCLogger_ns
{
/*----- PROTECTED REGION ID(LMCLogger::Additional Class Declarations) ENABLED START -----*/

//	Additional Class Declarations
	

/*----- PROTECTED REGION END -----*/	//	LMCLogger::Additional Class Declarations

class LMCLogger : public TANGO_BASE_CLASS
{

/*----- PROTECTED REGION ID(LMCLogger::Data Members) ENABLED START -----*/

//	Add your own data members

/*----- PROTECTED REGION END -----*/	//	LMCLogger::Data Members

//	Device property data members
public:
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
	//	default_syslog_level:	Default log level to be used at startup for logging to syslog
	string	default_syslog_level;
	//	startup_enable_logfw:	Enable log forwarding
	Tango::DevShort	startup_enable_logfw;

//	Attribute data members
public:
	Tango::DevString	*attr_syslog_level_read;
	Tango::DevShort	*attr_enable_logfw_read;

//	Constructors and destructors
public:
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	LMCLogger(Tango::DeviceClass *cl,string &s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	LMCLogger(Tango::DeviceClass *cl,const char *s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device name
	 *	@param d	Device description.
	 */
	LMCLogger(Tango::DeviceClass *cl,const char *s,const char *d);
	/**
	 * The device object destructor.
	 */
	~LMCLogger() {delete_device();};


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
	 *	Method      : LMCLogger::read_attr_hardware()
	 *	Description : Hardware acquisition for attributes.
	 */
	//--------------------------------------------------------
	virtual void read_attr_hardware(vector<long> &attr_list);

/**
 *	Attribute syslog_level related methods
 *	Description: Log level used for logging to syslog (OFF,TRACE,DEBUG,INFO,WARN,ERROR,FATAL)
 *
 *	Data type:	Tango::DevString
 *	Attr type:	Scalar
 */
	virtual void read_syslog_level(Tango::Attribute &attr);
	virtual bool is_syslog_level_allowed(Tango::AttReqType type);
/**
 *	Attribute enable_logfw related methods
 *	Description: Flag signalling if log forwarding is active/inactive
 *
 *	Data type:	Tango::DevShort
 *	Attr type:	Scalar
 */
	virtual void read_enable_logfw(Tango::Attribute &attr);
	virtual bool is_enable_logfw_allowed(Tango::AttReqType type);


	//--------------------------------------------------------
	/**
	 *	Method      : LMCLogger::add_dynamic_attributes()
	 *	Description : Add dynamic attributes if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_attributes();




//	Command related methods
public:
	/**
	 *	Command log related method
	 *	Description: Command to handle logs received from devices
	 *
	 *	@param argin Logging info received from emitter devices
	 */
	virtual void log(const Tango::DevVarStringArray *argin);
	virtual bool is_log_allowed(const CORBA::Any &any);
	/**
	 *	Command SetFileLogLevel related method
	 *	Description: Set log level used for logging to file(s)
	 *
	 *	@param argin The requested log level
	 *               (0=OFF, 1=FATAL, 2=ERROR, 3=WARNING, 4=INFO, 5=DEBUG)
	 */
	virtual void set_file_log_level(Tango::DevLong argin);
	virtual bool is_SetFileLogLevel_allowed(const CORBA::Any &any);
	/**
	 *	Command SetDeviceLogLevel related method
	 *	Description: Set log level used for logging to device(s)
	 *
	 *	@param argin The requested log level
	 *               (0=OFF, 1=FATAL, 2=ERROR, 3=WARNING, 4=INFO, 5=DEBUG)
	 */
	virtual void set_device_log_level(Tango::DevLong argin);
	virtual bool is_SetDeviceLogLevel_allowed(const CORBA::Any &any);
	/**
	 *	Command SetConsoleLogLevel related method
	 *	Description: Set log level used for logging to console
	 *
	 *	@param argin The requested log level
	 *               (0=OFF, 1=FATAL, 2=ERROR, 3=WARNING, 4=INFO, 5=DEBUG)
	 */
	virtual void set_console_log_level(Tango::DevLong argin);
	virtual bool is_SetConsoleLogLevel_allowed(const CORBA::Any &any);
	/**
	 *	Command SetSysLogLevel related method
	 *	Description: Set the log level for logging to syslog
	 *
	 *	@param argin The requested log level
	 *               (0=OFF, 1=FATAL, 2=ERROR, 3=WARNING, 4=INFO, 5=DEBUG)
	 */
	virtual void set_sys_log_level(Tango::DevString argin);
	virtual bool is_SetSysLogLevel_allowed(const CORBA::Any &any);
	/**
	 *	Command EnableLogForward related method
	 *	Description: Enable/disable log forwarding
	 *
	 *	@param argin 0=disable
	 *               1=enable
	 */
	virtual void enable_log_forward(Tango::DevShort argin);
	virtual bool is_EnableLogForward_allowed(const CORBA::Any &any);
	/**
	 *	Command TestLog related method
	 *	Description: Generate a log in the device according to the given level.
	 *               Used for testing purposes.
	 *
	 *	@param argin Long arg
	 *               [0]: Level of log message
	 *               (0=OFF, 1=FATAL, 2=ERROR, 3=WARNING, 4=INFO, 5=DEBUG)
	 *               
	 *               String arg
	 *               [0]: log message
	 */
	virtual void test_log(const Tango::DevVarLongStringArray *argin);
	virtual bool is_TestLog_allowed(const CORBA::Any &any);


	//--------------------------------------------------------
	/**
	 *	Method      : LMCLogger::add_dynamic_commands()
	 *	Description : Add dynamic commands if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_commands();

/*----- PROTECTED REGION ID(LMCLogger::Additional Method prototypes) ENABLED START -----*/

//	Additional Method prototypes
	protected:
		virtual int InitSysLogger();
		log4tango::Level::Value GetLog4LevelFromString(std::string);
		//Tango::LogTarget GetAppenderType(log4tango::Appender*);
		int GetAppenderType(log4tango::Appender*);

/*----- PROTECTED REGION END -----*/	//	LMCLogger::Additional Method prototypes
};

/*----- PROTECTED REGION ID(LMCLogger::Additional Classes Definitions) ENABLED START -----*/

//	Additional Classes Definitions
//Tango::LogLevel GetTangoLogLevelFromString(std::string);

/*----- PROTECTED REGION END -----*/	//	LMCLogger::Additional Classes Definitions

}	//	End of namespace

#endif   //	LMCLogger_H
