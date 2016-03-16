/*----- PROTECTED REGION ID(MoniDataAggregator.cpp) ENABLED START -----*/
static const char *RcsId = "$Id:  $";
//=============================================================================
//
// file :        MoniDataAggregator.cpp
//
// description : C++ source for the MoniDataAggregator class and its commands.
//               The class is derived from Device. It represents the
//               CORBA servant object which will be accessed from the
//               network. All commands which can be executed on the
//               MoniDataAggregator are implemented in this file.
//
// project :     MoniDataAggregator
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


#include <MoniDataAggregator.h>
#include <MoniDataAggregatorClass.h>

/*----- PROTECTED REGION END -----*/	//	MoniDataAggregator.cpp

/**
 *  MoniDataAggregator class description:
 *    Monitoring data aggregator
 */

//================================================================
//  The following table gives the correspondence
//  between command and method names.
//
//  Command name  |  Method name
//================================================================
//  State         |  Inherited (no method)
//  Status        |  Inherited (no method)
//================================================================

//================================================================
//  Attributes managed is:
//================================================================
//================================================================

namespace MoniDataAggregator_ns
{
/*----- PROTECTED REGION ID(MoniDataAggregator::namespace_starting) ENABLED START -----*/

//	static initializations

/*----- PROTECTED REGION END -----*/	//	MoniDataAggregator::namespace_starting

//--------------------------------------------------------
/**
 *	Method      : MoniDataAggregator::MoniDataAggregator()
 *	Description : Constructors for a Tango device
 *                implementing the classMoniDataAggregator
 */
//--------------------------------------------------------
MoniDataAggregator::MoniDataAggregator(Tango::DeviceClass *cl, string &s)
 : BaseDevice(cl, s.c_str())
{
	/*----- PROTECTED REGION ID(MoniDataAggregator::constructor_1) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	MoniDataAggregator::constructor_1
}
//--------------------------------------------------------
MoniDataAggregator::MoniDataAggregator(Tango::DeviceClass *cl, const char *s)
 : BaseDevice(cl, s)
{
	/*----- PROTECTED REGION ID(MoniDataAggregator::constructor_2) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	MoniDataAggregator::constructor_2
}
//--------------------------------------------------------
MoniDataAggregator::MoniDataAggregator(Tango::DeviceClass *cl, const char *s, const char *d)
 : BaseDevice(cl, s, d)
{
	/*----- PROTECTED REGION ID(MoniDataAggregator::constructor_3) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	MoniDataAggregator::constructor_3
}

//--------------------------------------------------------
/**
 *	Method      : MoniDataAggregator::delete_device()
 *	Description : will be called at device destruction or at init command
 */
//--------------------------------------------------------
void MoniDataAggregator::delete_device()
{
	DEBUG_STREAM << "MoniDataAggregator::delete_device() " << device_name << endl;
	/*----- PROTECTED REGION ID(MoniDataAggregator::delete_device) ENABLED START -----*/
	
	//	Delete device allocated objects
	
	/*----- PROTECTED REGION END -----*/	//	MoniDataAggregator::delete_device

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
 *	Method      : MoniDataAggregator::init_device()
 *	Description : will be called at device initialization.
 */
//--------------------------------------------------------
void MoniDataAggregator::init_device()
{
	DEBUG_STREAM << "MoniDataAggregator::init_device() create device " << device_name << endl;
	/*----- PROTECTED REGION ID(MoniDataAggregator::init_device_before) ENABLED START -----*/
	
	//	Initialization before get_device_property() call
	
	/*----- PROTECTED REGION END -----*/	//	MoniDataAggregator::init_device_before
	
	if (Tango::Util::instance()->is_svr_starting() == false  &&
		Tango::Util::instance()->is_device_restarting(device_name)==false)
	{
		//	If not starting up call init device for inherited object
		BaseDevice_ns::BaseDevice::init_device();
	}
	//	No device property to be read from database
	
	/*----- PROTECTED REGION ID(MoniDataAggregator::init_device) ENABLED START -----*/
	
	//	Initialize device
	
	/*----- PROTECTED REGION END -----*/	//	MoniDataAggregator::init_device
}


//--------------------------------------------------------
/**
 *	Method      : MoniDataAggregator::always_executed_hook()
 *	Description : method always executed before any command is executed
 */
//--------------------------------------------------------
void MoniDataAggregator::always_executed_hook()
{
	DEBUG_STREAM << "MoniDataAggregator::always_executed_hook()  " << device_name << endl;
	/*----- PROTECTED REGION ID(MoniDataAggregator::always_executed_hook) ENABLED START -----*/
	
	//	code always executed before all requests
	
	/*----- PROTECTED REGION END -----*/	//	MoniDataAggregator::always_executed_hook
}

//--------------------------------------------------------
/**
 *	Method      : MoniDataAggregator::read_attr_hardware()
 *	Description : Hardware acquisition for attributes
 */
//--------------------------------------------------------
void MoniDataAggregator::read_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	DEBUG_STREAM << "MoniDataAggregator::read_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(MoniDataAggregator::read_attr_hardware) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	MoniDataAggregator::read_attr_hardware
}


//--------------------------------------------------------
/**
 *	Method      : MoniDataAggregator::add_dynamic_attributes()
 *	Description : Create the dynamic attributes if any
 *                for specified device.
 */
//--------------------------------------------------------
void MoniDataAggregator::add_dynamic_attributes()
{
	/*----- PROTECTED REGION ID(MoniDataAggregator::add_dynamic_attributes) ENABLED START -----*/
	
	//	Add your own code to create and add dynamic attributes if any
	
	/*----- PROTECTED REGION END -----*/	//	MoniDataAggregator::add_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Method      : MoniDataAggregator::add_dynamic_commands()
 *	Description : Create the dynamic commands if any
 *                for specified device.
 */
//--------------------------------------------------------
void MoniDataAggregator::add_dynamic_commands()
{
	/*----- PROTECTED REGION ID(MoniDataAggregator::add_dynamic_commands) ENABLED START -----*/
	
	//	Add your own code to create and add dynamic commands if any
	
	/*----- PROTECTED REGION END -----*/	//	MoniDataAggregator::add_dynamic_commands
}

/*----- PROTECTED REGION ID(MoniDataAggregator::namespace_ending) ENABLED START -----*/

//	Additional Methods

/*----- PROTECTED REGION END -----*/	//	MoniDataAggregator::namespace_ending
} //	namespace