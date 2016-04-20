/*----- PROTECTED REGION ID(BaseDeviceClass.cpp) ENABLED START -----*/
static const char *RcsId      = "$Id:  $";
static const char *TagName    = "$Name:  $";
static const char *CvsPath    = "$Source:  $";
static const char *SvnPath    = "$HeadURL:  $";
static const char *HttpServer = "http://www.esrf.eu/computing/cs/tango/tango_doc/ds_doc/";
//=============================================================================
//
// file :        BaseDeviceClass.cpp
//
// description : C++ source for the BaseDeviceClass.
//               A singleton class derived from DeviceClass.
//               It implements the command and attribute list
//               and all properties and methods required
//               by the BaseDevice once per process.
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


#include <BaseDeviceClass.h>

/*----- PROTECTED REGION END -----*/	//	BaseDeviceClass.cpp

//-------------------------------------------------------------------
/**
 *	Create BaseDeviceClass singleton and
 *	return it in a C function for Python usage
 */
//-------------------------------------------------------------------
extern "C" {
#ifdef _TG_WINDOWS_

__declspec(dllexport)

#endif

	Tango::DeviceClass *_create_BaseDevice_class(const char *name) {
		return BaseDevice_ns::BaseDeviceClass::init(name);
	}
}

namespace BaseDevice_ns
{
//===================================================================
//	Initialize pointer for singleton pattern
//===================================================================
BaseDeviceClass *BaseDeviceClass::_instance = NULL;

//--------------------------------------------------------
/**
 * method : 		BaseDeviceClass::BaseDeviceClass(string &s)
 * description : 	constructor for the BaseDeviceClass
 *
 * @param s	The class name
 */
//--------------------------------------------------------
BaseDeviceClass::BaseDeviceClass(string &s):Tango::DeviceClass(s)
{
	cout2 << "Entering BaseDeviceClass constructor" << endl;
	set_default_property();
	write_class_property();

	/*----- PROTECTED REGION ID(BaseDeviceClass::constructor) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	BaseDeviceClass::constructor

	cout2 << "Leaving BaseDeviceClass constructor" << endl;
}

//--------------------------------------------------------
/**
 * method : 		BaseDeviceClass::~BaseDeviceClass()
 * description : 	destructor for the BaseDeviceClass
 */
//--------------------------------------------------------
BaseDeviceClass::~BaseDeviceClass()
{
	/*----- PROTECTED REGION ID(BaseDeviceClass::destructor) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	BaseDeviceClass::destructor

	_instance = NULL;
}


//--------------------------------------------------------
/**
 * method : 		BaseDeviceClass::init
 * description : 	Create the object if not already done.
 *                  Otherwise, just return a pointer to the object
 *
 * @param	name	The class name
 */
//--------------------------------------------------------
BaseDeviceClass *BaseDeviceClass::init(const char *name)
{
	if (_instance == NULL)
	{
		try
		{
			string s(name);
			_instance = new BaseDeviceClass(s);
		}
		catch (bad_alloc &)
		{
			throw;
		}
	}
	return _instance;
}

//--------------------------------------------------------
/**
 * method : 		BaseDeviceClass::instance
 * description : 	Check if object already created,
 *                  and return a pointer to the object
 */
//--------------------------------------------------------
BaseDeviceClass *BaseDeviceClass::instance()
{
	if (_instance == NULL)
	{
		cerr << "Class is not initialised !!" << endl;
		exit(-1);
	}
	return _instance;
}



//===================================================================
//	Command execution method calls
//===================================================================
//--------------------------------------------------------
/**
 * method : 		DevConfigureClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *DevConfigureClass::execute(Tango::DeviceImpl *device, const CORBA::Any &in_any)
{
	cout2 << "DevConfigureClass::execute(): arrived" << endl;
	Tango::DevString argin;
	extract(in_any, argin);
	return insert((static_cast<BaseDevice *>(device))->dev_configure(argin));
}

//--------------------------------------------------------
/**
 * method : 		RestoreDevConfigClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *RestoreDevConfigClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "RestoreDevConfigClass::execute(): arrived" << endl;
	return insert((static_cast<BaseDevice *>(device))->restore_dev_config());
}

//--------------------------------------------------------
/**
 * method : 		RemoveAttrClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *RemoveAttrClass::execute(Tango::DeviceImpl *device, const CORBA::Any &in_any)
{
	cout2 << "RemoveAttrClass::execute(): arrived" << endl;
	Tango::DevString argin;
	extract(in_any, argin);
	return insert((static_cast<BaseDevice *>(device))->remove_attr(argin));
}

//--------------------------------------------------------
/**
 * method : 		RemoveAttrsClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *RemoveAttrsClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "RemoveAttrsClass::execute(): arrived" << endl;
	return insert((static_cast<BaseDevice *>(device))->remove_attrs());
}

//--------------------------------------------------------
/**
 * method : 		SubscribeAttrClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *SubscribeAttrClass::execute(Tango::DeviceImpl *device, const CORBA::Any &in_any)
{
	cout2 << "SubscribeAttrClass::execute(): arrived" << endl;
	Tango::DevString argin;
	extract(in_any, argin);
	return insert((static_cast<BaseDevice *>(device))->subscribe_attr(argin));
}

//--------------------------------------------------------
/**
 * method : 		UnsubscribeAttrClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *UnsubscribeAttrClass::execute(Tango::DeviceImpl *device, const CORBA::Any &in_any)
{
	cout2 << "UnsubscribeAttrClass::execute(): arrived" << endl;
	Tango::DevString argin;
	extract(in_any, argin);
	return insert((static_cast<BaseDevice *>(device))->unsubscribe_attr(argin));
}

//--------------------------------------------------------
/**
 * method : 		SubscribeAttrsClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *SubscribeAttrsClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "SubscribeAttrsClass::execute(): arrived" << endl;
	return insert((static_cast<BaseDevice *>(device))->subscribe_attrs());
}

//--------------------------------------------------------
/**
 * method : 		UnsubscribeAttrsClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *UnsubscribeAttrsClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "UnsubscribeAttrsClass::execute(): arrived" << endl;
	return insert((static_cast<BaseDevice *>(device))->unsubscribe_attrs());
}


//===================================================================
//	Properties management
//===================================================================
//--------------------------------------------------------
/**
 *	Method      : BaseDeviceClass::get_class_property()
 *	Description : Get the class property for specified name.
 */
//--------------------------------------------------------
Tango::DbDatum BaseDeviceClass::get_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_prop.size() ; i++)
		if (cl_prop[i].name == prop_name)
			return cl_prop[i];
	//	if not found, returns  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method      : BaseDeviceClass::get_default_device_property()
 *	Description : Return the default value for device property.
 */
//--------------------------------------------------------
Tango::DbDatum BaseDeviceClass::get_default_device_property(string &prop_name)
{
	for (unsigned int i=0 ; i<dev_def_prop.size() ; i++)
		if (dev_def_prop[i].name == prop_name)
			return dev_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method      : BaseDeviceClass::get_default_class_property()
 *	Description : Return the default value for class property.
 */
//--------------------------------------------------------
Tango::DbDatum BaseDeviceClass::get_default_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_def_prop.size() ; i++)
		if (cl_def_prop[i].name == prop_name)
			return cl_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}


//--------------------------------------------------------
/**
 *	Method      : BaseDeviceClass::set_default_property()
 *	Description : Set default property (class and device) for wizard.
 *                For each property, add to wizard property name and description.
 *                If default value has been set, add it to wizard property and
 *                store it in a DbDatum.
 */
//--------------------------------------------------------
void BaseDeviceClass::set_default_property()
{
	string	prop_name;
	string	prop_desc;
	string	prop_def;
	vector<string>	vect_data;

	//	Set Default Class Properties

	//	Set Default device Properties
	prop_name = "configFile";
	prop_desc = "XML configuration file";
	prop_def  = "";
	vect_data.clear();
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "syslog_level";
	prop_desc = "Log level used for logging to syslog (OFF,TRACE,DEBUG,INFO,WARN,ERROR,FATAL)";
	prop_def  = "OFF";
	vect_data.clear();
	vect_data.push_back("OFF");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "syslog_facility";
	prop_desc = "LOG_USER: A miscellaneous user process\nLOG_MAIL: Mail\nLOG_DAEMON: A miscellaneous system daemon\nLOG_AUTH:  Security (authorization)\nLOG_SYSLOG: Syslog\nLOG_LPR: Central printer\nLOG_NEWS: Network news (e.g. Usenet)\nLOG_UUCP: UUCP\nLOG_CRON: Cron and At\nLOG_AUTHPRIV: Private security (authorization)\nLOG_FTP: Ftp server\nLOG_LOCAL0,1,2,3,4,5,6,7: Locally defined\n\n``auth``: LOG_AUTH\n``authpriv``: LOG_AUTHPRIV\n``cron``: LOG_CRON \n``daemon``: LOG_DAEMON\n``ftp``: LOG_FTP\n``kern``: LOG_KERN\n``lpr``: LOG_LPR\n``mail``: LOG_MAIL\n``mark``: INTERNAL_MARK\n``news``: LOG_NEWS\n``security``: LOG_AUTH,\n``syslog``: LOG_SYSLOG\n``user``: LOG_USER\n``uucp``: LOG_UUCP\n``local0``: LOG_LOCAL0\n``local1``: LOG_LOCAL1 \n``local2``: LOG_LOCAL2 \n``local3``: LOG_LOCAL3 \n``local4``: LOG_LOCAL4 \n``local5``: LOG_LOCAL5 \n``local6``: LOG_LOCAL6 \n``local7``: LOG_LOCAL7";
	prop_def  = "local6";
	vect_data.clear();
	vect_data.push_back("local6");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
}

//--------------------------------------------------------
/**
 *	Method      : BaseDeviceClass::write_class_property()
 *	Description : Set class description fields as property in database
 */
//--------------------------------------------------------
void BaseDeviceClass::write_class_property()
{
	//	First time, check if database used
	if (Tango::Util::_UseDb == false)
		return;

	Tango::DbData	data;
	string	classname = get_name();
	string	header;
	string::size_type	start, end;

	//	Put title
	Tango::DbDatum	title("ProjectTitle");
	string	str_title("Base device");
	title << str_title;
	data.push_back(title);

	//	Put Description
	Tango::DbDatum	description("Description");
	vector<string>	str_desc;
	str_desc.push_back("Base device used to export basic functionalities to all other devices.");
	description << str_desc;
	data.push_back(description);

	//	put cvs or svn location
	string	filename("BaseDevice");
	filename += "Class.cpp";

	// check for cvs information
	string	src_path(CvsPath);
	start = src_path.find("/");
	if (start!=string::npos)
	{
		end   = src_path.find(filename);
		if (end>start)
		{
			string	strloc = src_path.substr(start, end-start);
			//	Check if specific repository
			start = strloc.find("/cvsroot/");
			if (start!=string::npos && start>0)
			{
				string	repository = strloc.substr(0, start);
				if (repository.find("/segfs/")!=string::npos)
					strloc = "ESRF:" + strloc.substr(start, strloc.length()-start);
			}
			Tango::DbDatum	cvs_loc("cvs_location");
			cvs_loc << strloc;
			data.push_back(cvs_loc);
		}
	}

	// check for svn information
	else
	{
		string	src_path(SvnPath);
		start = src_path.find("://");
		if (start!=string::npos)
		{
			end = src_path.find(filename);
			if (end>start)
			{
				header = "$HeadURL: ";
				start = header.length();
				string	strloc = src_path.substr(start, (end-start));
				
				Tango::DbDatum	svn_loc("svn_location");
				svn_loc << strloc;
				data.push_back(svn_loc);
			}
		}
	}

	//	Get CVS or SVN revision tag
	
	// CVS tag
	string	tagname(TagName);
	header = "$Name: ";
	start = header.length();
	string	endstr(" $");
	
	end   = tagname.find(endstr);
	if (end!=string::npos && end>start)
	{
		string	strtag = tagname.substr(start, end-start);
		Tango::DbDatum	cvs_tag("cvs_tag");
		cvs_tag << strtag;
		data.push_back(cvs_tag);
	}
	
	// SVN tag
	string	svnpath(SvnPath);
	header = "$HeadURL: ";
	start = header.length();
	
	end   = svnpath.find(endstr);
	if (end!=string::npos && end>start)
	{
		string	strloc = svnpath.substr(start, end-start);
		
		string tagstr ("/tags/");
		start = strloc.find(tagstr);
		if ( start!=string::npos )
		{
			start = start + tagstr.length();
			end   = strloc.find(filename);
			string	strtag = strloc.substr(start, end-start-1);
			
			Tango::DbDatum	svn_tag("svn_tag");
			svn_tag << strtag;
			data.push_back(svn_tag);
		}
	}

	//	Get URL location
	string	httpServ(HttpServer);
	if (httpServ.length()>0)
	{
		Tango::DbDatum	db_doc_url("doc_url");
		db_doc_url << httpServ;
		data.push_back(db_doc_url);
	}

	//  Put inheritance
	Tango::DbDatum	inher_datum("InheritedFrom");
	vector<string> inheritance;
	inheritance.push_back("TANGO_BASE_CLASS");
	inher_datum << inheritance;
	data.push_back(inher_datum);

	//	Call database and and values
	get_db_class()->put_property(data);
}

//===================================================================
//	Factory methods
//===================================================================

//--------------------------------------------------------
/**
 *	Method      : BaseDeviceClass::device_factory()
 *	Description : Create the device object(s)
 *                and store them in the device list
 */
//--------------------------------------------------------
void BaseDeviceClass::device_factory(const Tango::DevVarStringArray *devlist_ptr)
{
	/*----- PROTECTED REGION ID(BaseDeviceClass::device_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	BaseDeviceClass::device_factory_before

	//	Create devices and add it into the device list
	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		cout4 << "Device name : " << (*devlist_ptr)[i].in() << endl;
		device_list.push_back(new BaseDevice(this, (*devlist_ptr)[i]));
	}

	//	Manage dynamic attributes if any
	erase_dynamic_attributes(devlist_ptr, get_class_attr()->get_attr_list());

	//	Export devices to the outside world
	for (unsigned long i=1 ; i<=devlist_ptr->length() ; i++)
	{
		//	Add dynamic attributes if any
		BaseDevice *dev = static_cast<BaseDevice *>(device_list[device_list.size()-i]);
		dev->add_dynamic_attributes();

		//	Check before if database used.
		if ((Tango::Util::_UseDb == true) && (Tango::Util::_FileDb == false))
			export_device(dev);
		else
			export_device(dev, dev->get_name().c_str());
	}

	/*----- PROTECTED REGION ID(BaseDeviceClass::device_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	BaseDeviceClass::device_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : BaseDeviceClass::attribute_factory()
 *	Description : Create the attribute object(s)
 *                and store them in the attribute list
 */
//--------------------------------------------------------
void BaseDeviceClass::attribute_factory(vector<Tango::Attr *> &att_list)
{
	/*----- PROTECTED REGION ID(BaseDeviceClass::attribute_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	BaseDeviceClass::attribute_factory_before

	//	Create a list of static attributes
	create_static_attribute_list(get_class_attr()->get_attr_list());
	/*----- PROTECTED REGION ID(BaseDeviceClass::attribute_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	BaseDeviceClass::attribute_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : BaseDeviceClass::pipe_factory()
 *	Description : Create the pipe object(s)
 *                and store them in the pipe list
 */
//--------------------------------------------------------
void BaseDeviceClass::pipe_factory()
{
	/*----- PROTECTED REGION ID(BaseDeviceClass::pipe_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	BaseDeviceClass::pipe_factory_before
	/*----- PROTECTED REGION ID(BaseDeviceClass::pipe_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	BaseDeviceClass::pipe_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : BaseDeviceClass::command_factory()
 *	Description : Create the command object(s)
 *                and store them in the command list
 */
//--------------------------------------------------------
void BaseDeviceClass::command_factory()
{
	/*----- PROTECTED REGION ID(BaseDeviceClass::command_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	BaseDeviceClass::command_factory_before


	//	Command DevConfigure
	DevConfigureClass	*pDevConfigureCmd =
		new DevConfigureClass("DevConfigure",
			Tango::DEV_STRING, Tango::DEVVAR_LONGSTRINGARRAY,
			"A string with SDD configuration",
			"",
			Tango::OPERATOR);
	command_list.push_back(pDevConfigureCmd);

	//	Command RestoreDevConfig
	RestoreDevConfigClass	*pRestoreDevConfigCmd =
		new RestoreDevConfigClass("RestoreDevConfig",
			Tango::DEV_VOID, Tango::DEVVAR_LONGSTRINGARRAY,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pRestoreDevConfigCmd);

	//	Command RemoveAttr
	RemoveAttrClass	*pRemoveAttrCmd =
		new RemoveAttrClass("RemoveAttr",
			Tango::DEV_STRING, Tango::DEVVAR_LONGSTRINGARRAY,
			"Attr name",
			"",
			Tango::OPERATOR);
	command_list.push_back(pRemoveAttrCmd);

	//	Command RemoveAttrs
	RemoveAttrsClass	*pRemoveAttrsCmd =
		new RemoveAttrsClass("RemoveAttrs",
			Tango::DEV_VOID, Tango::DEVVAR_LONGSTRINGARRAY,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pRemoveAttrsCmd);

	//	Command SubscribeAttr
	SubscribeAttrClass	*pSubscribeAttrCmd =
		new SubscribeAttrClass("SubscribeAttr",
			Tango::DEV_STRING, Tango::DEVVAR_LONGSTRINGARRAY,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pSubscribeAttrCmd);

	//	Command UnsubscribeAttr
	UnsubscribeAttrClass	*pUnsubscribeAttrCmd =
		new UnsubscribeAttrClass("UnsubscribeAttr",
			Tango::DEV_STRING, Tango::DEVVAR_LONGSTRINGARRAY,
			"Attr name",
			"",
			Tango::OPERATOR);
	command_list.push_back(pUnsubscribeAttrCmd);

	//	Command SubscribeAttrs
	SubscribeAttrsClass	*pSubscribeAttrsCmd =
		new SubscribeAttrsClass("SubscribeAttrs",
			Tango::DEV_VOID, Tango::DEVVAR_LONGSTRINGARRAY,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pSubscribeAttrsCmd);

	//	Command UnsubscribeAttrs
	UnsubscribeAttrsClass	*pUnsubscribeAttrsCmd =
		new UnsubscribeAttrsClass("UnsubscribeAttrs",
			Tango::DEV_VOID, Tango::DEVVAR_LONGSTRINGARRAY,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pUnsubscribeAttrsCmd);

	/*----- PROTECTED REGION ID(BaseDeviceClass::command_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	BaseDeviceClass::command_factory_after
}

//===================================================================
//	Dynamic attributes related methods
//===================================================================

//--------------------------------------------------------
/**
 * method : 		BaseDeviceClass::create_static_attribute_list
 * description : 	Create the a list of static attributes
 *
 * @param	att_list	the ceated attribute list
 */
//--------------------------------------------------------
void BaseDeviceClass::create_static_attribute_list(vector<Tango::Attr *> &att_list)
{
	for (unsigned long i=0 ; i<att_list.size() ; i++)
	{
		string att_name(att_list[i]->get_name());
		transform(att_name.begin(), att_name.end(), att_name.begin(), ::tolower);
		defaultAttList.push_back(att_name);
	}

	cout2 << defaultAttList.size() << " attributes in default list" << endl;

	/*----- PROTECTED REGION ID(BaseDeviceClass::create_static_att_list) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	BaseDeviceClass::create_static_att_list
}


//--------------------------------------------------------
/**
 * method : 		BaseDeviceClass::erase_dynamic_attributes
 * description : 	delete the dynamic attributes if any.
 *
 * @param	devlist_ptr	the device list pointer
 * @param	list of all attributes
 */
//--------------------------------------------------------
void BaseDeviceClass::erase_dynamic_attributes(const Tango::DevVarStringArray *devlist_ptr, vector<Tango::Attr *> &att_list)
{
	Tango::Util *tg = Tango::Util::instance();

	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		Tango::DeviceImpl *dev_impl = tg->get_device_by_name(((string)(*devlist_ptr)[i]).c_str());
		BaseDevice *dev = static_cast<BaseDevice *> (dev_impl);

		vector<Tango::Attribute *> &dev_att_list = dev->get_device_attr()->get_attribute_list();
		vector<Tango::Attribute *>::iterator ite_att;
		for (ite_att=dev_att_list.begin() ; ite_att != dev_att_list.end() ; ++ite_att)
		{
			string att_name((*ite_att)->get_name_lower());
			if ((att_name == "state") || (att_name == "status"))
				continue;
			vector<string>::iterator ite_str = find(defaultAttList.begin(), defaultAttList.end(), att_name);
			if (ite_str == defaultAttList.end())
			{
				cout2 << att_name << " is a UNWANTED dynamic attribute for device " << (*devlist_ptr)[i] << endl;
				Tango::Attribute &att = dev->get_device_attr()->get_attr_by_name(att_name.c_str());
				dev->remove_attribute(att_list[att.get_attr_idx()], true, false);
				--ite_att;
			}
		}
	}
	/*----- PROTECTED REGION ID(BaseDeviceClass::erase_dynamic_attributes) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	BaseDeviceClass::erase_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Method      : BaseDeviceClass::get_attr_by_name()
 *	Description : returns Tango::Attr * object found by name
 */
//--------------------------------------------------------
Tango::Attr *BaseDeviceClass::get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname)
{
	vector<Tango::Attr *>::iterator it;
	for (it=att_list.begin() ; it<att_list.end() ; ++it)
		if ((*it)->get_name()==attname)
			return (*it);
	//	Attr does not exist
	return NULL;
}


/*----- PROTECTED REGION ID(BaseDeviceClass::Additional Methods) ENABLED START -----*/

/*----- PROTECTED REGION END -----*/	//	BaseDeviceClass::Additional Methods
} //	namespace
