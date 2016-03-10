/*----- PROTECTED REGION ID(SchedulerClass.cpp) ENABLED START -----*/
static const char *RcsId      = "$Id:  $";
static const char *TagName    = "$Name:  $";
static const char *CvsPath    = "$Source:  $";
static const char *SvnPath    = "$HeadURL:  $";
static const char *HttpServer = "http://www.esrf.eu/computing/cs/tango/tango_doc/ds_doc/";
//=============================================================================
//
// file :        SchedulerClass.cpp
//
// description : C++ source for the SchedulerClass.
//               A singleton class derived from DeviceClass.
//               It implements the command and attribute list
//               and all properties and methods required
//               by the Scheduler once per process.
//
// project :     Scheduler
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


#include <SchedulerClass.h>

/*----- PROTECTED REGION END -----*/	//	SchedulerClass.cpp

//-------------------------------------------------------------------
/**
 *	Create SchedulerClass singleton and
 *	return it in a C function for Python usage
 */
//-------------------------------------------------------------------
extern "C" {
#ifdef _TG_WINDOWS_

__declspec(dllexport)

#endif

	Tango::DeviceClass *_create_Scheduler_class(const char *name) {
		return Scheduler_ns::SchedulerClass::init(name);
	}
}

namespace Scheduler_ns
{
//===================================================================
//	Initialize pointer for singleton pattern
//===================================================================
SchedulerClass *SchedulerClass::_instance = NULL;

//--------------------------------------------------------
/**
 * method : 		SchedulerClass::SchedulerClass(string &s)
 * description : 	constructor for the SchedulerClass
 *
 * @param s	The class name
 */
//--------------------------------------------------------
SchedulerClass::SchedulerClass(string &s):BaseDevice_ns::BaseDeviceClass(s)
{
	cout2 << "Entering SchedulerClass constructor" << endl;
	set_default_property();
	write_class_property();

	/*----- PROTECTED REGION ID(SchedulerClass::constructor) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	SchedulerClass::constructor

	cout2 << "Leaving SchedulerClass constructor" << endl;
}

//--------------------------------------------------------
/**
 * method : 		SchedulerClass::~SchedulerClass()
 * description : 	destructor for the SchedulerClass
 */
//--------------------------------------------------------
SchedulerClass::~SchedulerClass()
{
	/*----- PROTECTED REGION ID(SchedulerClass::destructor) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	SchedulerClass::destructor

	_instance = NULL;
}


//--------------------------------------------------------
/**
 * method : 		SchedulerClass::init
 * description : 	Create the object if not already done.
 *                  Otherwise, just return a pointer to the object
 *
 * @param	name	The class name
 */
//--------------------------------------------------------
SchedulerClass *SchedulerClass::init(const char *name)
{
	if (_instance == NULL)
	{
		try
		{
			string s(name);
			_instance = new SchedulerClass(s);
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
 * method : 		SchedulerClass::instance
 * description : 	Check if object already created,
 *                  and return a pointer to the object
 */
//--------------------------------------------------------
SchedulerClass *SchedulerClass::instance()
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
 * method : 		ScheduleTaskClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *ScheduleTaskClass::execute(Tango::DeviceImpl *device, const CORBA::Any &in_any)
{
	cout2 << "ScheduleTaskClass::execute(): arrived" << endl;
	const Tango::DevVarStringArray *argin;
	extract(in_any, argin);
	return insert((static_cast<Scheduler *>(device))->schedule_task(argin));
}

//--------------------------------------------------------
/**
 * method : 		TestScheduleClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *TestScheduleClass::execute(Tango::DeviceImpl *device, const CORBA::Any &in_any)
{
	cout2 << "TestScheduleClass::execute(): arrived" << endl;
	Tango::DevString argin;
	extract(in_any, argin);
	return insert((static_cast<Scheduler *>(device))->test_schedule(argin));
}

//--------------------------------------------------------
/**
 * method : 		FlushTaskQueueClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *FlushTaskQueueClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "FlushTaskQueueClass::execute(): arrived" << endl;
	return insert((static_cast<Scheduler *>(device))->flush_task_queue());
}

//--------------------------------------------------------
/**
 * method : 		RevokeTaskClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *RevokeTaskClass::execute(Tango::DeviceImpl *device, const CORBA::Any &in_any)
{
	cout2 << "RevokeTaskClass::execute(): arrived" << endl;
	Tango::DevString argin;
	extract(in_any, argin);
	return insert((static_cast<Scheduler *>(device))->revoke_task(argin));
}

//--------------------------------------------------------
/**
 * method : 		GetNQueuedTasksClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *GetNQueuedTasksClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "GetNQueuedTasksClass::execute(): arrived" << endl;
	return insert((static_cast<Scheduler *>(device))->get_nqueued_tasks());
}

//--------------------------------------------------------
/**
 * method : 		GetNRunningTasksClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *GetNRunningTasksClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "GetNRunningTasksClass::execute(): arrived" << endl;
	return insert((static_cast<Scheduler *>(device))->get_nrunning_tasks());
}

//--------------------------------------------------------
/**
 * method : 		PrintTasksClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *PrintTasksClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "PrintTasksClass::execute(): arrived" << endl;
	((static_cast<Scheduler *>(device))->print_tasks());
	return new CORBA::Any();
}


//===================================================================
//	Properties management
//===================================================================
//--------------------------------------------------------
/**
 *	Method      : SchedulerClass::get_class_property()
 *	Description : Get the class property for specified name.
 */
//--------------------------------------------------------
Tango::DbDatum SchedulerClass::get_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_prop.size() ; i++)
		if (cl_prop[i].name == prop_name)
			return cl_prop[i];
	//	if not found, returns  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method      : SchedulerClass::get_default_device_property()
 *	Description : Return the default value for device property.
 */
//--------------------------------------------------------
Tango::DbDatum SchedulerClass::get_default_device_property(string &prop_name)
{
	for (unsigned int i=0 ; i<dev_def_prop.size() ; i++)
		if (dev_def_prop[i].name == prop_name)
			return dev_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method      : SchedulerClass::get_default_class_property()
 *	Description : Return the default value for class property.
 */
//--------------------------------------------------------
Tango::DbDatum SchedulerClass::get_default_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_def_prop.size() ; i++)
		if (cl_def_prop[i].name == prop_name)
			return cl_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}


//--------------------------------------------------------
/**
 *	Method      : SchedulerClass::set_default_property()
 *	Description : Set default property (class and device) for wizard.
 *                For each property, add to wizard property name and description.
 *                If default value has been set, add it to wizard property and
 *                store it in a DbDatum.
 */
//--------------------------------------------------------
void SchedulerClass::set_default_property()
{
	string	prop_name;
	string	prop_desc;
	string	prop_def;
	vector<string>	vect_data;

	//	Set Default Class Properties

	//	Set Default device Properties
	prop_name = "max_queueable_tasks";
	prop_desc = "Max size of task queue, e.g. maximum number of tasks that \ncan be inserted in the task queue";
	prop_def  = "100";
	vect_data.clear();
	vect_data.push_back("100");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "thread_heartbeat_period";
	prop_desc = "Polling period used for thread heartbeat  in seconds";
	prop_def  = "1";
	vect_data.clear();
	vect_data.push_back("1");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "max_trackable_tasks";
	prop_desc = "Maximum  number of tasks tracked, e.g. inserted and monitored \nin the task collection.";
	prop_def  = "1000";
	vect_data.clear();
	vect_data.push_back("1000");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "max_cacheable_device_proxies";
	prop_desc = "Maximum number of cacheable device proxies. \nIf the number of cached device proxies exceeds the limit all \nrequested command to be scheduled will be refused.";
	prop_def  = "10";
	vect_data.clear();
	vect_data.push_back("10");
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
 *	Method      : SchedulerClass::write_class_property()
 *	Description : Set class description fields as property in database
 */
//--------------------------------------------------------
void SchedulerClass::write_class_property()
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
	string	str_title("Scheduler");
	title << str_title;
	data.push_back(title);

	//	Put Description
	Tango::DbDatum	description("Description");
	vector<string>	str_desc;
	str_desc.push_back("Device server providing command scheduling functionalities");
	description << str_desc;
	data.push_back(description);

	//	put cvs or svn location
	string	filename("Scheduler");
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
 *	Method      : SchedulerClass::device_factory()
 *	Description : Create the device object(s)
 *                and store them in the device list
 */
//--------------------------------------------------------
void SchedulerClass::device_factory(const Tango::DevVarStringArray *devlist_ptr)
{
	/*----- PROTECTED REGION ID(SchedulerClass::device_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	SchedulerClass::device_factory_before

	//	Create devices and add it into the device list
	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		cout4 << "Device name : " << (*devlist_ptr)[i].in() << endl;
		device_list.push_back(new Scheduler(this, (*devlist_ptr)[i]));
	}

	//	Manage dynamic attributes if any
	erase_dynamic_attributes(devlist_ptr, get_class_attr()->get_attr_list());

	//	Export devices to the outside world
	for (unsigned long i=1 ; i<=devlist_ptr->length() ; i++)
	{
		//	Add dynamic attributes if any
		Scheduler *dev = static_cast<Scheduler *>(device_list[device_list.size()-i]);
		dev->add_dynamic_attributes();

		//	Check before if database used.
		if ((Tango::Util::_UseDb == true) && (Tango::Util::_FileDb == false))
			export_device(dev);
		else
			export_device(dev, dev->get_name().c_str());
	}

	/*----- PROTECTED REGION ID(SchedulerClass::device_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	SchedulerClass::device_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : SchedulerClass::attribute_factory()
 *	Description : Create the attribute object(s)
 *                and store them in the attribute list
 */
//--------------------------------------------------------
void SchedulerClass::attribute_factory(vector<Tango::Attr *> &att_list)
{
	/*----- PROTECTED REGION ID(SchedulerClass::attribute_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	SchedulerClass::attribute_factory_before
	//	Call atribute_factory for inherited class
	BaseDevice_ns::BaseDeviceClass::attribute_factory(att_list);


	//	Create a list of static attributes
	create_static_attribute_list(get_class_attr()->get_attr_list());
	/*----- PROTECTED REGION ID(SchedulerClass::attribute_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	SchedulerClass::attribute_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : SchedulerClass::pipe_factory()
 *	Description : Create the pipe object(s)
 *                and store them in the pipe list
 */
//--------------------------------------------------------
void SchedulerClass::pipe_factory()
{
	/*----- PROTECTED REGION ID(SchedulerClass::pipe_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	SchedulerClass::pipe_factory_before
	Tango::UserDefaultPipeProp udpp;
	FinalResponseClass	*pFinalResponse = new FinalResponseClass("FinalResponse",Tango::OPERATOR);
	udpp.set_description("Task final response data structure");
	udpp.set_label("FinalResponse");
	pFinalResponse->set_default_properties(udpp);
	pipe_list.push_back(pFinalResponse);

	IntermediateResponseClass	*pIntermediateResponse = new IntermediateResponseClass("IntermediateResponse",Tango::OPERATOR);
	udpp.set_description("intermediate task response data structure");
	udpp.set_label("intermediateResponse");
	pIntermediateResponse->set_default_properties(udpp);
	pipe_list.push_back(pIntermediateResponse);

	queued_tasksClass	*pqueued_tasks = new queued_tasksClass("queued_tasks",Tango::OPERATOR);
	udpp.set_description("Queued tasks");
	udpp.set_label("queued_tasks");
	pqueued_tasks->set_default_properties(udpp);
	pipe_list.push_back(pqueued_tasks);

	tasksClass	*ptasks = new tasksClass("tasks",Tango::OPERATOR);
	udpp.set_description("Task collection pipe");
	udpp.set_label("tasks");
	ptasks->set_default_properties(udpp);
	pipe_list.push_back(ptasks);

	/*----- PROTECTED REGION ID(SchedulerClass::pipe_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	SchedulerClass::pipe_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : SchedulerClass::command_factory()
 *	Description : Create the command object(s)
 *                and store them in the command list
 */
//--------------------------------------------------------
void SchedulerClass::command_factory()
{
	/*----- PROTECTED REGION ID(SchedulerClass::command_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	SchedulerClass::command_factory_before
	//	Call command_factory for inherited class
	BaseDevice_ns::BaseDeviceClass::command_factory();



	//	Command ScheduleTask
	ScheduleTaskClass	*pScheduleTaskCmd =
		new ScheduleTaskClass("ScheduleTask",
			Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_LONGSTRINGARRAY,
			"[0]: Request arg \n[1]: Proxy device name to be contacted for request\n[2]: Command name in proxy device (optional)",
			"Long arg\n[0]: ack\n\nString arg\n[0]: err/info description",
			Tango::OPERATOR);
	command_list.push_back(pScheduleTaskCmd);

	//	Command TestSchedule
	TestScheduleClass	*pTestScheduleCmd =
		new TestScheduleClass("TestSchedule",
			Tango::DEV_STRING, Tango::DEVVAR_LONGSTRINGARRAY,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pTestScheduleCmd);

	//	Command FlushTaskQueue
	FlushTaskQueueClass	*pFlushTaskQueueCmd =
		new FlushTaskQueueClass("FlushTaskQueue",
			Tango::DEV_VOID, Tango::DEVVAR_LONGSTRINGARRAY,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pFlushTaskQueueCmd);

	//	Command RevokeTask
	RevokeTaskClass	*pRevokeTaskCmd =
		new RevokeTaskClass("RevokeTask",
			Tango::DEV_STRING, Tango::DEVVAR_LONGSTRINGARRAY,
			"Task id",
			"",
			Tango::OPERATOR);
	command_list.push_back(pRevokeTaskCmd);

	//	Command GetNQueuedTasks
	GetNQueuedTasksClass	*pGetNQueuedTasksCmd =
		new GetNQueuedTasksClass("GetNQueuedTasks",
			Tango::DEV_VOID, Tango::DEVVAR_LONGSTRINGARRAY,
			"",
			"Long arg\n[0]: ack code \n[1]: current queue size",
			Tango::OPERATOR);
	command_list.push_back(pGetNQueuedTasksCmd);

	//	Command GetNRunningTasks
	GetNRunningTasksClass	*pGetNRunningTasksCmd =
		new GetNRunningTasksClass("GetNRunningTasks",
			Tango::DEV_VOID, Tango::DEVVAR_LONGSTRINGARRAY,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pGetNRunningTasksCmd);

	//	Command PrintTasks
	PrintTasksClass	*pPrintTasksCmd =
		new PrintTasksClass("PrintTasks",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pPrintTasksCmd);

	/*----- PROTECTED REGION ID(SchedulerClass::command_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	SchedulerClass::command_factory_after
}

//===================================================================
//	Dynamic attributes related methods
//===================================================================

//--------------------------------------------------------
/**
 * method : 		SchedulerClass::create_static_attribute_list
 * description : 	Create the a list of static attributes
 *
 * @param	att_list	the ceated attribute list
 */
//--------------------------------------------------------
void SchedulerClass::create_static_attribute_list(vector<Tango::Attr *> &att_list)
{
	for (unsigned long i=0 ; i<att_list.size() ; i++)
	{
		string att_name(att_list[i]->get_name());
		transform(att_name.begin(), att_name.end(), att_name.begin(), ::tolower);
		defaultAttList.push_back(att_name);
	}

	cout2 << defaultAttList.size() << " attributes in default list" << endl;

	/*----- PROTECTED REGION ID(SchedulerClass::create_static_att_list) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	SchedulerClass::create_static_att_list
}


//--------------------------------------------------------
/**
 * method : 		SchedulerClass::erase_dynamic_attributes
 * description : 	delete the dynamic attributes if any.
 *
 * @param	devlist_ptr	the device list pointer
 * @param	list of all attributes
 */
//--------------------------------------------------------
void SchedulerClass::erase_dynamic_attributes(const Tango::DevVarStringArray *devlist_ptr, vector<Tango::Attr *> &att_list)
{
	Tango::Util *tg = Tango::Util::instance();

	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		Tango::DeviceImpl *dev_impl = tg->get_device_by_name(((string)(*devlist_ptr)[i]).c_str());
		Scheduler *dev = static_cast<Scheduler *> (dev_impl);

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
	/*----- PROTECTED REGION ID(SchedulerClass::erase_dynamic_attributes) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	SchedulerClass::erase_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Method      : SchedulerClass::get_attr_by_name()
 *	Description : returns Tango::Attr * object found by name
 */
//--------------------------------------------------------
Tango::Attr *SchedulerClass::get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname)
{
	vector<Tango::Attr *>::iterator it;
	for (it=att_list.begin() ; it<att_list.end() ; ++it)
		if ((*it)->get_name()==attname)
			return (*it);
	//	Attr does not exist
	return NULL;
}


/*----- PROTECTED REGION ID(SchedulerClass::Additional Methods) ENABLED START -----*/

/*----- PROTECTED REGION END -----*/	//	SchedulerClass::Additional Methods
} //	namespace
