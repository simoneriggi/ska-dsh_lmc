/*----- PROTECTED REGION ID(SelfMonitoring.h) ENABLED START -----*/
//=============================================================================
//
// file :        SelfMonitoring.h
//
// description : Include file for the SelfMonitoring class
//
// project :     SelfMonitoring
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


#ifndef SelfMonitoring_H
#define SelfMonitoring_H

#include <tango.h>
#include <BaseDevice.h>
#include <NagiosListenerThread.h>


/*----- PROTECTED REGION END -----*/	//	SelfMonitoring.h

/**
 *  SelfMonitoring class description:
 *    Device server to perform host/service self-monitoring
 */

namespace SelfMonitoring_ns
{
/*----- PROTECTED REGION ID(SelfMonitoring::Additional Class Declarations) ENABLED START -----*/

//	Additional Class Declarations
	class NagiosListenerThread;
/*----- PROTECTED REGION END -----*/	//	SelfMonitoring::Additional Class Declarations

class SelfMonitoring : public BaseDevice_ns::BaseDevice
{

/*----- PROTECTED REGION ID(SelfMonitoring::Data Members) ENABLED START -----*/

//	Add your own data members

/*----- PROTECTED REGION END -----*/	//	SelfMonitoring::Data Members

//	Device property data members
public:
	//	nagios_query_handler:	Address of Nagios Query Handler in filesystem
	string	nagios_query_handler;

//	Attribute data members
public:

//	Constructors and destructors
public:
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	SelfMonitoring(Tango::DeviceClass *cl,string &s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	SelfMonitoring(Tango::DeviceClass *cl,const char *s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device name
	 *	@param d	Device description.
	 */
	SelfMonitoring(Tango::DeviceClass *cl,const char *s,const char *d);
	/**
	 * The device object destructor.
	 */
	~SelfMonitoring() {delete_device();};


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
	 *	Method      : SelfMonitoring::read_attr_hardware()
	 *	Description : Hardware acquisition for attributes.
	 */
	//--------------------------------------------------------
	virtual void read_attr_hardware(vector<long> &attr_list);
	//--------------------------------------------------------
	/*
	 *	Method      : SelfMonitoring::write_attr_hardware()
	 *	Description : Hardware writing for attributes.
	 */
	//--------------------------------------------------------
	virtual void write_attr_hardware(vector<long> &attr_list);



	//--------------------------------------------------------
	/**
	 *	Method      : SelfMonitoring::add_dynamic_attributes()
	 *	Description : Add dynamic attributes if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_attributes();




//	Command related methods
public:
	/**
	 *	Command SubscribeToServiceChecks related method
	 *	Description: Subscribe to receive service checks from Nagios 
	 *               NERD
	 *
	 *	@returns 
	 */
	virtual Tango::DevVarLongStringArray *subscribe_to_service_checks();
	virtual bool is_SubscribeToServiceChecks_allowed(const CORBA::Any &any);
	/**
	 *	Command SubscribeToHostChecks related method
	 *	Description: Subscribe to receive host checks from Nagios NERD
	 *
	 *	@returns 
	 */
	virtual Tango::DevVarLongStringArray *subscribe_to_host_checks();
	virtual bool is_SubscribeToHostChecks_allowed(const CORBA::Any &any);
	/**
	 *	Command UnsubscribeFromServiceChecks related method
	 *	Description: Unsubscribe from Nagios service check channel
	 *
	 *	@returns 
	 */
	virtual Tango::DevVarLongStringArray *unsubscribe_from_service_checks();
	virtual bool is_UnsubscribeFromServiceChecks_allowed(const CORBA::Any &any);
	/**
	 *	Command UnsubscribeFromHostChecks related method
	 *	Description: Unsubscribe from host check channel
	 *
	 *	@returns 
	 */
	virtual Tango::DevVarLongStringArray *unsubscribe_from_host_checks();
	virtual bool is_UnsubscribeFromHostChecks_allowed(const CORBA::Any &any);
	/**
	 *	Command ListChannels related method
	 *	Description: List available Nagios channels
	 *
	 *	@returns 
	 */
	virtual Tango::DevVarLongStringArray *list_channels();
	virtual bool is_ListChannels_allowed(const CORBA::Any &any);


	//--------------------------------------------------------
	/**
	 *	Method      : SelfMonitoring::add_dynamic_commands()
	 *	Description : Add dynamic commands if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_commands();

/*----- PROTECTED REGION ID(SelfMonitoring::Additional Method prototypes) ENABLED START -----*/

//	Additional Method prototypes
	protected:
		int InitSocket();
		int CloseSocket();
		ssize_t WriteSocket(const char* buffer, size_t bufsize);
		ssize_t ReadSocket(char* buffer, size_t bufsize, int timeout);
		int WaitForSocketReadEvent(int timeout);

	protected:
		enum SockStatus {
    	eConnectionClosed = 0,
      eConnectionReset = -1,
      eConnectionTimedOut = -2
    };

		enum HostStatus {
			UP= 0,
			DOWN= 1,
			UNREACHABLE= 2
		};
		enum ServiceStatus {
			OK= 0,
			WARNING= 1,
			CRITICAL= 2,
			UNKNOWN= 3
		};

		bool m_StopThreadFlag;	
		omni_mutex* m_mutex;
		NagiosListenerThread* m_NagiosListenerThread;
		
		//Socket vars
		int m_sockfd;
		void* m_ZmqContext;
		void* m_ZmqSock;
		

	friend class NagiosListenerThread;

/*----- PROTECTED REGION END -----*/	//	SelfMonitoring::Additional Method prototypes
};

/*----- PROTECTED REGION ID(SelfMonitoring::Additional Classes Definitions) ENABLED START -----*/

//	Additional Classes Definitions

/*----- PROTECTED REGION END -----*/	//	SelfMonitoring::Additional Classes Definitions

}	//	End of namespace

#endif   //	SelfMonitoring_H