//$Id$

// ============================================================================
//
// = FILENAME
//   DD_servant_i.h
//
// = AUTHOR
//   Yuanfang Zhang
//
// ============================================================================

#ifndef TAO_DD_SERVANT_I_H
#define TAO_DD_SERVANT_I_H

#include "DD_servantS.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "Task_Stats.h"

class DD_Servant_i : public POA_DD_Servant
{
public:
	DD_Servant_i(CORBA::ORB_ptr orb, 
		RTScheduling::Current_ptr current,
		DD_Servant_ptr servant_ptr,
	     	Task_Stats&,
	  	int,
		int);

	void exec_method (CORBA::Long value ACE_ENV_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC ((CORBA::SystemException));

	void shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC ((CORBA::SystemException));

private:
	CORBA::ORB_var orb_;
	RTScheduling::Current_var current_;
	DD_Servant_ptr servant_ptr_;
	Task_Stats& task_stats_;
	int enable_yield_;
	int exec_time_;
};

#if defined(__ACE_INLINE__)
#include "DD_servant_i.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_MT_SERVER_METHOD_I_H */
