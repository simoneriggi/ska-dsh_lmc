/*----- PROTECTED REGION ID(LogConsumer.cpp) ENABLED START -----*/
static const char *RcsId = "$Id:  $";
//=============================================================================
//
// file :        LogConsumer.cpp
//
// description : C++ source for the LogConsumer class and its commands.
//               The class is derived from Device. It represents the
//               CORBA servant object which will be accessed from the
//               network. All commands which can be executed on the
//               LogConsumer are implemented in this file.
//
// project :     LogConsumer
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


#include <LogConsumer.h>
#include <LogConsumerClass.h>

/*----- PROTECTED REGION END -----*/	//	LogConsumer.cpp

/**
 *  LogConsumer class description:
 *    Device server to handle logs produced by sub-elements
 */

//================================================================
//  The following table gives the correspondence
//  between command and method names.
//
//  Command name  |  Method name
//================================================================
//  State         |  Inherited (no method)
//  Status        |  Inherited (no method)
//  log           |  log
//================================================================

//================================================================
//  Attributes managed is:
//================================================================
//================================================================

namespace LogConsumer_ns
{
/*----- PROTECTED REGION ID(LogConsumer::namespace_starting) ENABLED START -----*/

//	static initializations

/*----- PROTECTED REGION END -----*/	//	LogConsumer::namespace_starting

//--------------------------------------------------------
/**
 *	Method      : LogConsumer::LogConsumer()
 *	Description : Constructors for a Tango device
 *                implementing the classLogConsumer
 */
//--------------------------------------------------------
LogConsumer::LogConsumer(Tango::DeviceClass *cl, string &s)
 : BaseDevice(cl, s.c_str())
{
	/*----- PROTECTED REGION ID(LogConsumer::constructor_1) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	LogConsumer::constructor_1
}
//--------------------------------------------------------
LogConsumer::LogConsumer(Tango::DeviceClass *cl, const char *s)
 : BaseDevice(cl, s)
{
	/*----- PROTECTED REGION ID(LogConsumer::constructor_2) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	LogConsumer::constructor_2
}
//--------------------------------------------------------
LogConsumer::LogConsumer(Tango::DeviceClass *cl, const char *s, const char *d)
 : BaseDevice(cl, s, d)
{
	/*----- PROTECTED REGION ID(LogConsumer::constructor_3) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	LogConsumer::constructor_3
}

//--------------------------------------------------------
/**
 *	Method      : LogConsumer::delete_device()
 *	Description : will be called at device destruction or at init command
 */
//--------------------------------------------------------
void LogConsumer::delete_device()
{
	DEBUG_STREAM << "LogConsumer::delete_device() " << device_name << endl;
	/*----- PROTECTED REGION ID(LogConsumer::delete_device) ENABLED START -----*/
	
	//	Delete device allocated objects
	
	/*----- PROTECTED REGION END -----*/	//	LogConsumer::delete_device

	if (Tango::Util::instance()->is_svr_shutting_down()==false  &&
		Tango::Util::instance()->is_device_restarting(device_name)==false &&
		Tango::Util::instance()->is_svr_starting()==false)
	{
		//	If not shutting down call delete device for inherited object
		BaseDevice_ns::BaseDevice::delete_device();
	}
}

//--------------------------------------------------------
/**
 *	Method      : LogConsumer::init_device()
 *	Description : will be called at device initialization.
 */
//--------------------------------------------------------
void LogConsumer::init_device()
{
	DEBUG_STREAM << "LogConsumer::init_device() create device " << device_name << endl;
	/*----- PROTECTED REGION ID(LogConsumer::init_device_before) ENABLED START -----*/
	
	//	Initialization before get_device_property() call
	
	/*----- PROTECTED REGION END -----*/	//	LogConsumer::init_device_before
	
	if (Tango::Util::instance()->is_svr_starting() == false  &&
		Tango::Util::instance()->is_device_restarting(device_name)==false)
	{
		//	If not starting up call init device for inherited object
		BaseDevice_ns::BaseDevice::init_device();
	}
	//	No device property to be read from database
	
	/*----- PROTECTED REGION ID(LogConsumer::init_device) ENABLED START -----*/
	
	//	Initialize device
	
	/*----- PROTECTED REGION END -----*/	//	LogConsumer::init_device
}


//--------------------------------------------------------
/**
 *	Method      : LogConsumer::always_executed_hook()
 *	Description : method always executed before any command is executed
 */
//--------------------------------------------------------
void LogConsumer::always_executed_hook()
{
	DEBUG_STREAM << "LogConsumer::always_executed_hook()  " << device_name << endl;
	/*----- PROTECTED REGION ID(LogConsumer::always_executed_hook) ENABLED START -----*/
	
	//	code always executed before all requests
	
	/*----- PROTECTED REGION END -----*/	//	LogConsumer::always_executed_hook
}

//--------------------------------------------------------
/**
 *	Method      : LogConsumer::read_attr_hardware()
 *	Description : Hardware acquisition for attributes
 */
//--------------------------------------------------------
void LogConsumer::read_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	DEBUG_STREAM << "LogConsumer::read_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(LogConsumer::read_attr_hardware) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	LogConsumer::read_attr_hardware
}


//--------------------------------------------------------
/**
 *	Method      : LogConsumer::add_dynamic_attributes()
 *	Description : Create the dynamic attributes if any
 *                for specified device.
 */
//--------------------------------------------------------
void LogConsumer::add_dynamic_attributes()
{
	/*----- PROTECTED REGION ID(LogConsumer::add_dynamic_attributes) ENABLED START -----*/
	
	//	Add your own code to create and add dynamic attributes if any
	
	/*----- PROTECTED REGION END -----*/	//	LogConsumer::add_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Command log related method
 *	Description: Command to handle logs received from devices
 *
 *	@param argin Log info
 */
//--------------------------------------------------------
void LogConsumer::log(const Tango::DevVarStringArray *argin)
{
	DEBUG_STREAM << "LogConsumer::log()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(LogConsumer::log) ENABLED START -----*/
	
	//	Add your own code

	INFO_STREAM<<"LogConsumer::log(): INFO: Log received (n="<<argin->length()<<") @ time="<<(*argin)[0]<<", level="<<(*argin)[1]<<", source="<<(*argin)[2]<<", msg="<<(*argin)[3]<<endl;
	
	/*----- PROTECTED REGION END -----*/	//	LogConsumer::log
}
//--------------------------------------------------------
/**
 *	Method      : LogConsumer::add_dynamic_commands()
 *	Description : Create the dynamic commands if any
 *                for specified device.
 */
//--------------------------------------------------------
void LogConsumer::add_dynamic_commands()
{
	/*----- PROTECTED REGION ID(LogConsumer::add_dynamic_commands) ENABLED START -----*/
	
	//	Add your own code to create and add dynamic commands if any
	
	/*----- PROTECTED REGION END -----*/	//	LogConsumer::add_dynamic_commands
}

/*----- PROTECTED REGION ID(LogConsumer::namespace_ending) ENABLED START -----*/

//	Additional Methods

/*----- PROTECTED REGION END -----*/	//	LogConsumer::namespace_ending
} //	namespace
