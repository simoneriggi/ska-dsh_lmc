/*----- PROTECTED REGION ID(SchedulerStateMachine.cpp) ENABLED START -----*/
static const char *RcsId = "$Id:  $";
//=============================================================================
//
// file :        SchedulerStateMachine.cpp
//
// description : State machine file for the Scheduler class
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

#include <Scheduler.h>

/*----- PROTECTED REGION END -----*/	//	Scheduler::SchedulerStateMachine.cpp

//================================================================
//  States  |  Description
//================================================================


namespace Scheduler_ns
{
//=================================================
//		Attributes Allowed Methods
//=================================================

//--------------------------------------------------------
/**
 *	Method      : Scheduler::is_finalResponse_allowed()
 *	Description : Execution allowed for finalResponse attribute
 */
//--------------------------------------------------------
bool Scheduler::is_finalResponse_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for finalResponse attribute in read access.
	/*----- PROTECTED REGION ID(Scheduler::finalResponseStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Scheduler::finalResponseStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Scheduler::is_intermediateResponse_allowed()
 *	Description : Execution allowed for intermediateResponse attribute
 */
//--------------------------------------------------------
bool Scheduler::is_intermediateResponse_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for intermediateResponse attribute in read access.
	/*----- PROTECTED REGION ID(Scheduler::intermediateResponseStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Scheduler::intermediateResponseStateAllowed_READ
	return true;
}

//=================================================
//		pipe Allowed Methods
//=================================================
//--------------------------------------------------------
/**
 *	Method      : Scheduler::is_FinalResponsePipe_allowed()
 *	Description : Execution allowed for FinalResponsePipe pipe
 */
//--------------------------------------------------------
bool Scheduler::is_FinalResponsePipe_allowed(TANGO_UNUSED(Tango::PipeReqType type))
{
	//	Not any excluded states for FinalResponsePipe pipe in read access.
	/*----- PROTECTED REGION ID(Scheduler::FinalResponsePipeStateAllowed_READ) ENABLED START -----*/
	return true;
	/*----- PROTECTED REGION END -----*/	//	Scheduler::FinalResponsePipeStateAllowed_READ
}
//--------------------------------------------------------
/**
 *	Method      : Scheduler::is_IntermediateResponsePipe_allowed()
 *	Description : Execution allowed for IntermediateResponsePipe pipe
 */
//--------------------------------------------------------
bool Scheduler::is_IntermediateResponsePipe_allowed(TANGO_UNUSED(Tango::PipeReqType type))
{
	//	Not any excluded states for IntermediateResponsePipe pipe in read access.
	/*----- PROTECTED REGION ID(Scheduler::IntermediateResponsePipeStateAllowed_READ) ENABLED START -----*/
	return true;
	/*----- PROTECTED REGION END -----*/	//	Scheduler::IntermediateResponsePipeStateAllowed_READ
}
//--------------------------------------------------------
/**
 *	Method      : Scheduler::is_queued_tasks_allowed()
 *	Description : Execution allowed for queued_tasks pipe
 */
//--------------------------------------------------------
bool Scheduler::is_queued_tasks_allowed(TANGO_UNUSED(Tango::PipeReqType type))
{
	//	Not any excluded states for queued_tasks pipe in read access.
	/*----- PROTECTED REGION ID(Scheduler::queued_tasksStateAllowed_READ) ENABLED START -----*/
	return true;
	/*----- PROTECTED REGION END -----*/	//	Scheduler::queued_tasksStateAllowed_READ
}
//--------------------------------------------------------
/**
 *	Method      : Scheduler::is_tasks_allowed()
 *	Description : Execution allowed for tasks pipe
 */
//--------------------------------------------------------
bool Scheduler::is_tasks_allowed(TANGO_UNUSED(Tango::PipeReqType type))
{
	//	Not any excluded states for tasks pipe in read access.
	/*----- PROTECTED REGION ID(Scheduler::tasksStateAllowed_READ) ENABLED START -----*/
	return true;
	/*----- PROTECTED REGION END -----*/	//	Scheduler::tasksStateAllowed_READ
}
//--------------------------------------------------------
/**
 *	Method      : Scheduler::is_myPipe_allowed()
 *	Description : Execution allowed for myPipe pipe
 */
//--------------------------------------------------------
bool Scheduler::is_myPipe_allowed(TANGO_UNUSED(Tango::PipeReqType type))
{
	//	Not any excluded states for myPipe pipe in read access.
	/*----- PROTECTED REGION ID(Scheduler::myPipeStateAllowed_READ) ENABLED START -----*/
	return true;
	/*----- PROTECTED REGION END -----*/	//	Scheduler::myPipeStateAllowed_READ
}

//=================================================
//		Commands Allowed Methods
//=================================================

//--------------------------------------------------------
/**
 *	Method      : Scheduler::is_ScheduleTask_allowed()
 *	Description : Execution allowed for ScheduleTask attribute
 */
//--------------------------------------------------------
bool Scheduler::is_ScheduleTask_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Not any excluded states for ScheduleTask command.
	/*----- PROTECTED REGION ID(Scheduler::ScheduleTaskStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Scheduler::ScheduleTaskStateAllowed
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Scheduler::is_TestSchedule_allowed()
 *	Description : Execution allowed for TestSchedule attribute
 */
//--------------------------------------------------------
bool Scheduler::is_TestSchedule_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Not any excluded states for TestSchedule command.
	/*----- PROTECTED REGION ID(Scheduler::TestScheduleStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Scheduler::TestScheduleStateAllowed
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Scheduler::is_FlushTaskQueue_allowed()
 *	Description : Execution allowed for FlushTaskQueue attribute
 */
//--------------------------------------------------------
bool Scheduler::is_FlushTaskQueue_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Not any excluded states for FlushTaskQueue command.
	/*----- PROTECTED REGION ID(Scheduler::FlushTaskQueueStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Scheduler::FlushTaskQueueStateAllowed
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Scheduler::is_RevokeTask_allowed()
 *	Description : Execution allowed for RevokeTask attribute
 */
//--------------------------------------------------------
bool Scheduler::is_RevokeTask_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Not any excluded states for RevokeTask command.
	/*----- PROTECTED REGION ID(Scheduler::RevokeTaskStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Scheduler::RevokeTaskStateAllowed
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Scheduler::is_GetNQueuedTasks_allowed()
 *	Description : Execution allowed for GetNQueuedTasks attribute
 */
//--------------------------------------------------------
bool Scheduler::is_GetNQueuedTasks_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Not any excluded states for GetNQueuedTasks command.
	/*----- PROTECTED REGION ID(Scheduler::GetNQueuedTasksStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Scheduler::GetNQueuedTasksStateAllowed
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Scheduler::is_GetNRunningTasks_allowed()
 *	Description : Execution allowed for GetNRunningTasks attribute
 */
//--------------------------------------------------------
bool Scheduler::is_GetNRunningTasks_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Not any excluded states for GetNRunningTasks command.
	/*----- PROTECTED REGION ID(Scheduler::GetNRunningTasksStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Scheduler::GetNRunningTasksStateAllowed
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Scheduler::is_PrintTasks_allowed()
 *	Description : Execution allowed for PrintTasks attribute
 */
//--------------------------------------------------------
bool Scheduler::is_PrintTasks_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Not any excluded states for PrintTasks command.
	/*----- PROTECTED REGION ID(Scheduler::PrintTasksStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Scheduler::PrintTasksStateAllowed
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Scheduler::is_ClearTasks_allowed()
 *	Description : Execution allowed for ClearTasks attribute
 */
//--------------------------------------------------------
bool Scheduler::is_ClearTasks_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Not any excluded states for ClearTasks command.
	/*----- PROTECTED REGION ID(Scheduler::ClearTasksStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Scheduler::ClearTasksStateAllowed
	return true;
}


/*----- PROTECTED REGION ID(Scheduler::SchedulerStateAllowed.AdditionalMethods) ENABLED START -----*/

//	Additional Methods

/*----- PROTECTED REGION END -----*/	//	Scheduler::SchedulerStateAllowed.AdditionalMethods

}	//	End of namespace
