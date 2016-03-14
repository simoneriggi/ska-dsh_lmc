/*----- PROTECTED REGION ID(LogConsumerClass.h) ENABLED START -----*/
//=============================================================================
//
// file :        LogConsumerClass.h
//
// description : Include for the LogConsumer root class.
//               This class is the singleton class for
//                the LogConsumer device class.
//               It contains all properties and methods which the 
//               LogConsumer requires only once e.g. the commands.
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


#ifndef LogConsumerClass_H
#define LogConsumerClass_H

#include <tango.h>
#include <BaseDeviceClass.h>
#include <LogConsumer.h>


/*----- PROTECTED REGION END -----*/	//	LogConsumerClass.h


namespace LogConsumer_ns
{
/*----- PROTECTED REGION ID(LogConsumerClass::classes for dynamic creation) ENABLED START -----*/


/*----- PROTECTED REGION END -----*/	//	LogConsumerClass::classes for dynamic creation

//=========================================
//	Define classes for commands
//=========================================
//	Command log class definition
class logClass : public Tango::Command
{
public:
	logClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	logClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~logClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<LogConsumer *>(dev))->is_log_allowed(any);}
};


/**
 *	The LogConsumerClass singleton definition
 */

#ifdef _TG_WINDOWS_
class __declspec(dllexport)  LogConsumerClass : public BaseDevice_ns::BaseDeviceClass
#else
class LogConsumerClass : public BaseDevice_ns::BaseDeviceClass
#endif
{
	/*----- PROTECTED REGION ID(LogConsumerClass::Additionnal DServer data members) ENABLED START -----*/
	
	
	/*----- PROTECTED REGION END -----*/	//	LogConsumerClass::Additionnal DServer data members

	public:
		//	write class properties data members
		Tango::DbData	cl_prop;
		Tango::DbData	cl_def_prop;
		Tango::DbData	dev_def_prop;
	
		//	Method prototypes
		static LogConsumerClass *init(const char *);
		static LogConsumerClass *instance();
		~LogConsumerClass();
		Tango::DbDatum	get_class_property(string &);
		Tango::DbDatum	get_default_device_property(string &);
		Tango::DbDatum	get_default_class_property(string &);
	
	protected:
		LogConsumerClass(string &);
		static LogConsumerClass *_instance;
		void command_factory();
		void attribute_factory(vector<Tango::Attr *> &);
		void pipe_factory();
		void write_class_property();
		void set_default_property();
		void get_class_property();
		string get_cvstag();
		string get_cvsroot();
	
	private:
		void device_factory(const Tango::DevVarStringArray *);
		void create_static_attribute_list(vector<Tango::Attr *> &);
		void erase_dynamic_attributes(const Tango::DevVarStringArray *,vector<Tango::Attr *> &);
		vector<string>	defaultAttList;
		Tango::Attr *get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname);
};

}	//	End of namespace

#endif   //	LogConsumer_H
