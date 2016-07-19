/*----- PROTECTED REGION ID(PointingMgr.cpp) ENABLED START -----*/
static const char *RcsId = "$Id:  $";
//=============================================================================
//
// file :        PointingMgr.cpp
//
// description : C++ source for the PointingMgr class and its commands.
//               The class is derived from Device. It represents the
//               CORBA servant object which will be accessed from the
//               network. All commands which can be executed on the
//               PointingMgr are implemented in this file.
//
// project :     Pointing Manager
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


#include <PointingMgr.h>
#include <PointingMgrClass.h>

/*----- PROTECTED REGION END -----*/	//	PointingMgr.cpp

/**
 *  PointingMgr class description:
 *    Device server for controlling pointing operations of SKA Dish Antenna
 */

//================================================================
//  The following table gives the correspondence
//  between command and method names.
//
//  Command name      |  Method name
//================================================================
//  State             |  Inherited (no method)
//  Status            |  Inherited (no method)
//  DevConfigure      |  Inherited (no method)
//  RestoreDevConfig  |  Inherited (no method)
//  RemoveAttr        |  Inherited (no method)
//  RemoveAttrs       |  Inherited (no method)
//  SubscribeAttr     |  Inherited (no method)
//  UnsubscribeAttr   |  Inherited (no method)
//  SubscribeAttrs    |  Inherited (no method)
//  UnsubscribeAttrs  |  Inherited (no method)
//  TrackAzEl         |  track_az_el
//================================================================

//================================================================
//  Attributes managed is:
//================================================================
//================================================================

namespace PointingMgr_ns
{
/*----- PROTECTED REGION ID(PointingMgr::namespace_starting) ENABLED START -----*/

//	static initializations

/*----- PROTECTED REGION END -----*/	//	PointingMgr::namespace_starting

//--------------------------------------------------------
/**
 *	Method      : PointingMgr::PointingMgr()
 *	Description : Constructors for a Tango device
 *                implementing the classPointingMgr
 */
//--------------------------------------------------------
PointingMgr::PointingMgr(Tango::DeviceClass *cl, string &s)
 : BaseDevice(cl, s.c_str())
{
	/*----- PROTECTED REGION ID(PointingMgr::constructor_1) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	PointingMgr::constructor_1
}
//--------------------------------------------------------
PointingMgr::PointingMgr(Tango::DeviceClass *cl, const char *s)
 : BaseDevice(cl, s)
{
	/*----- PROTECTED REGION ID(PointingMgr::constructor_2) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	PointingMgr::constructor_2
}
//--------------------------------------------------------
PointingMgr::PointingMgr(Tango::DeviceClass *cl, const char *s, const char *d)
 : BaseDevice(cl, s, d)
{
	/*----- PROTECTED REGION ID(PointingMgr::constructor_3) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	PointingMgr::constructor_3
}

//--------------------------------------------------------
/**
 *	Method      : PointingMgr::delete_device()
 *	Description : will be called at device destruction or at init command
 */
//--------------------------------------------------------
void PointingMgr::delete_device()
{
	DEBUG_STREAM << "PointingMgr::delete_device() " << device_name << endl;
	/*----- PROTECTED REGION ID(PointingMgr::delete_device) ENABLED START -----*/
	
	//	Delete device allocated objects
	
	/*----- PROTECTED REGION END -----*/	//	PointingMgr::delete_device

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
 *	Method      : PointingMgr::init_device()
 *	Description : will be called at device initialization.
 */
//--------------------------------------------------------
void PointingMgr::init_device()
{
	DEBUG_STREAM << "PointingMgr::init_device() create device " << device_name << endl;
	/*----- PROTECTED REGION ID(PointingMgr::init_device_before) ENABLED START -----*/
	
	//	Initialization before get_device_property() call
	
	/*----- PROTECTED REGION END -----*/	//	PointingMgr::init_device_before
	
	if (Tango::Util::instance()->is_svr_starting() == false  &&
		Tango::Util::instance()->is_device_restarting(device_name)==false)
	{
		//	If not starting up call init device for inherited object
		BaseDevice_ns::BaseDevice::init_device();
	}
	//	No device property to be read from database
	
	/*----- PROTECTED REGION ID(PointingMgr::init_device) ENABLED START -----*/
	
	//	Initialize device
	
	/*----- PROTECTED REGION END -----*/	//	PointingMgr::init_device
}


//--------------------------------------------------------
/**
 *	Method      : PointingMgr::always_executed_hook()
 *	Description : method always executed before any command is executed
 */
//--------------------------------------------------------
void PointingMgr::always_executed_hook()
{
	DEBUG_STREAM << "PointingMgr::always_executed_hook()  " << device_name << endl;
	/*----- PROTECTED REGION ID(PointingMgr::always_executed_hook) ENABLED START -----*/
	
	//	code always executed before all requests
	
	/*----- PROTECTED REGION END -----*/	//	PointingMgr::always_executed_hook
}

//--------------------------------------------------------
/**
 *	Method      : PointingMgr::read_attr_hardware()
 *	Description : Hardware acquisition for attributes
 */
//--------------------------------------------------------
void PointingMgr::read_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	DEBUG_STREAM << "PointingMgr::read_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(PointingMgr::read_attr_hardware) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	PointingMgr::read_attr_hardware
}


//--------------------------------------------------------
/**
 *	Method      : PointingMgr::add_dynamic_attributes()
 *	Description : Create the dynamic attributes if any
 *                for specified device.
 */
//--------------------------------------------------------
void PointingMgr::add_dynamic_attributes()
{
	/*----- PROTECTED REGION ID(PointingMgr::add_dynamic_attributes) ENABLED START -----*/
	
	//	Add your own code to create and add dynamic attributes if any
	
	/*----- PROTECTED REGION END -----*/	//	PointingMgr::add_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Command TrackAzEl related method
 *	Description: Receive a pointing request (Az,El,timestamp) and 
 *               send it to the antenna positioner system
 *
 *	@param argin Double Array
 *               [0]: Az
 *               [1]: El
 *               
 *               String arg
 *               [0]: timestamp
 *	@returns Long arg
 *           [0]: 
 *           
 *           String arg
 *           [0]: err/info description
 */
//--------------------------------------------------------
Tango::DevVarLongStringArray *PointingMgr::track_az_el(const Tango::DevVarDoubleStringArray *argin)
{
	Tango::DevVarLongStringArray *argout;
	DEBUG_STREAM << "PointingMgr::TrackAzEl()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(PointingMgr::track_az_el) ENABLED START -----*/
	
	//	Add your own code
	std::string reply= "Request executed with success";
	long int ack= 0;
	argout= new Tango::DevVarLongStringArray;
	argout->lvalue.length(1);
	argout->svalue.length(1);

	//============================
	//==   VALIDATE ARGS
	//============================
	//Validate request args
	int nargs_s= argin->svalue.length();
	int nargs_d= argin->dvalue.length();
	if(nargs_s<1 || nargs_d<2){
		ERROR_STREAM<<"PointingMgr::TrackAzEl(): ERROR: Missing arguments!"<<endl;
		ack= -1;
		reply= "Missing args";
		argout->lvalue[0]= ack;	
		argout->svalue[0]= CORBA::string_dup(reply.c_str());
		return argout;
	}
	std::string timestamp= std::string(argin->svalue[0]);
	double Az= argin->dvalue[0];
	double El= argin->dvalue[1];

	INFO_STREAM<<"PointingMgr::TrackAzEl(): INFO: Pointing req received (Az,El,timestamp)=("<<Az<<","<<El<<","<<timestamp<<")"<<endl;
	

	//## Put here Adriano code
	//...
	//..

	//## Return results
	argout->lvalue[0]= ack;
	argout->svalue[0] = CORBA::string_dup(reply.c_str());		

	/*----- PROTECTED REGION END -----*/	//	PointingMgr::track_az_el
	return argout;
}
//--------------------------------------------------------
/**
 *	Method      : PointingMgr::add_dynamic_commands()
 *	Description : Create the dynamic commands if any
 *                for specified device.
 */
//--------------------------------------------------------
void PointingMgr::add_dynamic_commands()
{
	/*----- PROTECTED REGION ID(PointingMgr::add_dynamic_commands) ENABLED START -----*/
	
	//	Add your own code to create and add dynamic commands if any
	
	/*----- PROTECTED REGION END -----*/	//	PointingMgr::add_dynamic_commands
}

/*----- PROTECTED REGION ID(PointingMgr::namespace_ending) ENABLED START -----*/

//	Additional Methods

/*----- PROTECTED REGION END -----*/	//	PointingMgr::namespace_ending
} //	namespace