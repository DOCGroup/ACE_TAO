// $Id$
// ManagementWorkInstructions_exec.h
//============================================================
/**
* @file ManagementWorkInstructions_exec.h
*
* Header file for the Executor implementation.
*/
//============================================================

//#ifndef MANAGEMENTWORKINSTRUCTIONS_EXEC_H
//#define MANAGEMENTWORKINSTRUCTIONS_EXEC_H

#include "ManagementWorkInstructionsEC.h"
#include "ManagementWorkInstructions_exec_export.h"

#include "tao/LocalObject.h"

namespace ManagementWorkInstructions_Impl
{
/**
* @class ManagementWorkInstructions_exec_i
*
* ManagementWorkInstructions executor implementation class.
	*/
	
	class MANAGEMENTWORKINSTRUCTIONS_EXEC_Export ManagementWorkInstructions_exec_i :
	public virtual ManagementWorkInstructions_Exec, 
		public virtual TAO_Local_RefCounted_Object
	{
		
	public:
		/// Default constructor.
		ManagementWorkInstructions_exec_i () { }
		
		/// Default destructor.
		virtual ~ManagementWorkInstructions_exec_i ();
		
		
		/* Operations for obtaining the interface reference. */
		/* This method will be used in the assembly face so the
		* ObjRef of this read_message facet will be sent to the
		* client side(receptacle).
		*/
		virtual RobotAssembly::CCM_MWIController_ptr get_Controller (ACE_ENV_SINGLE_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));
		
// consumes ProductionStatus ProductionReport;
		virtual void push_ProductionReport (RobotAssembly::ProductionStatus *ev ACE_ENV_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException));
		
		// Operations from Components::SessionComponent
		virtual void set_session_context (Components::SessionContext_ptr ctx
			ACE_ENV_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException,
			Components::CCMException));
		
		virtual void ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException,
			Components::CCMException));
		
		virtual void ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException,
			Components::CCMException));
		
		virtual void ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException,
			Components::CCMException));
		
		virtual void ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException,
			Components::CCMException));
		
		virtual void ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException,
			Components::CCMException));
		
		
	protected:
		/// Component specific context
		ManagementWorkInstructions_Exec_Context_var context_;
		
		friend class MWIController_Impl;
		
	};
	
	class MWIController_Impl : public virtual RobotAssembly::CCM_MWIController,
		public virtual TAO_Local_RefCounted_Object
	{
	public:
		MWIController_Impl(ManagementWorkInstructions_exec_i& component) : component_ (component)
		{  }
		
		virtual void SendStringTest (const char *testString ACE_ENV_SINGLE_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));
		
		virtual void SendOrder( const ::RobotAssembly::WorkOrder & testOrder ACE_ENV_SINGLE_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));
		
		virtual RobotAssembly::ProductionStatusType GetOrderStatus (ACE_ENV_SINGLE_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));
		
		virtual char * getHost (ACE_ENV_SINGLE_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));
		
	private:
		ManagementWorkInstructions_exec_i& component_;
	};
	
	/**
	* @class ManagementWorkInstructionsHome_exec_i
	*
	* ManagementWorkInstructions home executor implementation class.
	*/
	class MANAGEMENTWORKINSTRUCTIONS_EXEC_Export ManagementWorkInstructionsHome_exec_i :
	public virtual ManagementWorkInstructionsHome_Exec,
		public virtual TAO_Local_RefCounted_Object
	{
	public:
		/// Default ctor.
		ManagementWorkInstructionsHome_exec_i ();
		
		/// Default dtor.
		virtual ~ManagementWorkInstructionsHome_exec_i ();
		
		// Implicit home operations.
		
		virtual ::Components::EnterpriseComponent_ptr
			create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException,
			Components::CCMException));
	};
	
}

extern "C" MANAGEMENTWORKINSTRUCTIONS_EXEC_Export ::Components::HomeExecutorBase_ptr
createManagementWorkInstructionsHome_Impl (void);

//#endif /* MANAGEMENTWORKINSTRUCTONS_EXEC_H */
