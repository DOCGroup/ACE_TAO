//
// $Id$
//
#include "BGML_Base/BGML_Task_Base.h"
#include "ace/Sched_Params.h"
#include "ace/High_Res_Timer.h"

BGML_Task_Base::BGML_Task_Base (int priority)
: priority_ (priority)
{
}

BGML_Task_Base::BGML_Task_Base ()
{
	this->priority_ = (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
		+ ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
	this->set_priority ();
}

BGML_Task_Base::~BGML_Task_Base ()
{


}

int
BGML_Task_Base::set_priority ()
{

	if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
		this->priority_,
		ACE_SCOPE_PROCESS)) != 0)
	{
		if (ACE_OS::last_error () == EPERM)
		{
			ACE_DEBUG ((LM_DEBUG,
				"Client_Task_Base (%P|%t): user is not superuser, "
				"test runs in time-shared class\n"));
			return 1;
		}
		else
		{
			ACE_ERROR ((LM_ERROR,
				"Client_Task_Base (%P|%t): sched_params failed\n"));
			return -1;
		}
	}
	return 1;
}

int
BGML_Task_Base::set_priority (int priority)
{
	int temp = this->priority_;
	this->priority_ = priority;
	if (this->set_priority () == -1)
	{
		this->priority_ = temp;
		return -1;
	}

	return 1;
}
