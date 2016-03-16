/*----- PROTECTED REGION ID(TMEmulator.cpp) ENABLED START -----*/
static const char *RcsId = "$Id:  $";
//=============================================================================
//
// file :        TMEmulator.cpp
//
// description : C++ source for the TMEmulator class and its commands.
//               The class is derived from Device. It represents the
//               CORBA servant object which will be accessed from the
//               network. All commands which can be executed on the
//               TMEmulator are implemented in this file.
//
// project :     TMEmulator
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


#include <TMEmulator.h>
#include <TMEmulatorClass.h>

/*----- PROTECTED REGION END -----*/	//	TMEmulator.cpp

/**
 *  TMEmulator class description:
 *    Emulator device for TM
 */

//================================================================
//  The following table gives the correspondence
//  between command and method names.
//
//  Command name          |  Method name
//================================================================
//  State                 |  Inherited (no method)
//  Status                |  Inherited (no method)
//  SendPointingCoords    |  send_pointing_coords
//  SendPointingSchedule  |  send_pointing_schedule
//================================================================

//================================================================
//  Attributes managed is:
//================================================================
//================================================================

namespace TMEmulator_ns
{
/*----- PROTECTED REGION ID(TMEmulator::namespace_starting) ENABLED START -----*/

//	static initializations

/*----- PROTECTED REGION END -----*/	//	TMEmulator::namespace_starting

//--------------------------------------------------------
/**
 *	Method      : TMEmulator::TMEmulator()
 *	Description : Constructors for a Tango device
 *                implementing the classTMEmulator
 */
//--------------------------------------------------------
TMEmulator::TMEmulator(Tango::DeviceClass *cl, string &s)
 : TANGO_BASE_CLASS(cl, s.c_str())
{
	/*----- PROTECTED REGION ID(TMEmulator::constructor_1) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	TMEmulator::constructor_1
}
//--------------------------------------------------------
TMEmulator::TMEmulator(Tango::DeviceClass *cl, const char *s)
 : TANGO_BASE_CLASS(cl, s)
{
	/*----- PROTECTED REGION ID(TMEmulator::constructor_2) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	TMEmulator::constructor_2
}
//--------------------------------------------------------
TMEmulator::TMEmulator(Tango::DeviceClass *cl, const char *s, const char *d)
 : TANGO_BASE_CLASS(cl, s, d)
{
	/*----- PROTECTED REGION ID(TMEmulator::constructor_3) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	TMEmulator::constructor_3
}

//--------------------------------------------------------
/**
 *	Method      : TMEmulator::delete_device()
 *	Description : will be called at device destruction or at init command
 */
//--------------------------------------------------------
void TMEmulator::delete_device()
{
	DEBUG_STREAM << "TMEmulator::delete_device() " << device_name << endl;
	/*----- PROTECTED REGION ID(TMEmulator::delete_device) ENABLED START -----*/
	
	//	Delete device allocated objects
	
	/*----- PROTECTED REGION END -----*/	//	TMEmulator::delete_device
}

//--------------------------------------------------------
/**
 *	Method      : TMEmulator::init_device()
 *	Description : will be called at device initialization.
 */
//--------------------------------------------------------
void TMEmulator::init_device()
{
	DEBUG_STREAM << "TMEmulator::init_device() create device " << device_name << endl;
	/*----- PROTECTED REGION ID(TMEmulator::init_device_before) ENABLED START -----*/
	
	//	Initialization before get_device_property() call
	
	/*----- PROTECTED REGION END -----*/	//	TMEmulator::init_device_before
	
	//	No device property to be read from database
	
	/*----- PROTECTED REGION ID(TMEmulator::init_device) ENABLED START -----*/
	
	//	Initialize device
	
	/*----- PROTECTED REGION END -----*/	//	TMEmulator::init_device
}


//--------------------------------------------------------
/**
 *	Method      : TMEmulator::always_executed_hook()
 *	Description : method always executed before any command is executed
 */
//--------------------------------------------------------
void TMEmulator::always_executed_hook()
{
	DEBUG_STREAM << "TMEmulator::always_executed_hook()  " << device_name << endl;
	/*----- PROTECTED REGION ID(TMEmulator::always_executed_hook) ENABLED START -----*/
	
	//	code always executed before all requests
	
	/*----- PROTECTED REGION END -----*/	//	TMEmulator::always_executed_hook
}

//--------------------------------------------------------
/**
 *	Method      : TMEmulator::read_attr_hardware()
 *	Description : Hardware acquisition for attributes
 */
//--------------------------------------------------------
void TMEmulator::read_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	DEBUG_STREAM << "TMEmulator::read_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(TMEmulator::read_attr_hardware) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	TMEmulator::read_attr_hardware
}


//--------------------------------------------------------
/**
 *	Method      : TMEmulator::add_dynamic_attributes()
 *	Description : Create the dynamic attributes if any
 *                for specified device.
 */
//--------------------------------------------------------
void TMEmulator::add_dynamic_attributes()
{
	/*----- PROTECTED REGION ID(TMEmulator::add_dynamic_attributes) ENABLED START -----*/
	
	//	Add your own code to create and add dynamic attributes if any
	
	/*----- PROTECTED REGION END -----*/	//	TMEmulator::add_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Command SendPointingCoords related method
 *	Description: Send pointing coordinates to LMC interface, e.g.
 *               a set of (Az, El, timestamp), encoded in the Response
 *               message string.
 *
 *	@param argin 
 *	@returns 
 */
//--------------------------------------------------------
Tango::DevVarLongStringArray *TMEmulator::send_pointing_coords(Tango::DevString argin)
{
	Tango::DevVarLongStringArray *argout;
	DEBUG_STREAM << "TMEmulator::SendPointingCoords()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(TMEmulator::send_pointing_coords) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	TMEmulator::send_pointing_coords
	return argout;
}
//--------------------------------------------------------
/**
 *	Command SendPointingSchedule related method
 *	Description: Send pointing schedule encoded in the Request format
 *               string.
 *
 *	@param argin 
 *	@returns 
 */
//--------------------------------------------------------
Tango::DevVarLongStringArray *TMEmulator::send_pointing_schedule(Tango::DevString argin)
{
	Tango::DevVarLongStringArray *argout;
	DEBUG_STREAM << "TMEmulator::SendPointingSchedule()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(TMEmulator::send_pointing_schedule) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	TMEmulator::send_pointing_schedule
	return argout;
}
//--------------------------------------------------------
/**
 *	Method      : TMEmulator::add_dynamic_commands()
 *	Description : Create the dynamic commands if any
 *                for specified device.
 */
//--------------------------------------------------------
void TMEmulator::add_dynamic_commands()
{
	/*----- PROTECTED REGION ID(TMEmulator::add_dynamic_commands) ENABLED START -----*/
	
	//	Add your own code to create and add dynamic commands if any
	
	/*----- PROTECTED REGION END -----*/	//	TMEmulator::add_dynamic_commands
}

/*----- PROTECTED REGION ID(TMEmulator::namespace_ending) ENABLED START -----*/

//	Additional Methods

/*----- PROTECTED REGION END -----*/	//	TMEmulator::namespace_ending
} //	namespace
