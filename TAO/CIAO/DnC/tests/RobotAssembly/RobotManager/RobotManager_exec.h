// $Id$
// RobotManager_exec.h
//============================================================
/**
* @file RobotManager_exec.h
*
* Header file for the Executor implementation.
*/
//============================================================

//#ifndef ROBOTMANAGER_EXEC_H
//#define ROBOTMANAGER_EXEC_H

#include "RobotManagerEC.h"
#include "RobotManager_exec_export.h"

#include "tao/LocalObject.h"

namespace RobotManager_Impl
{
/**
* @class RobotManager_exec_i
*
* RobotManager executor implementation class.
*/
	
	class ROBOTMANAGER_EXEC_Export RobotManager_exec_i :
	public virtual RobotManager_Exec, 
        public virtual TAO_Local_RefCounted_Object
	{
		
	public:
		/// Default constructor.
		RobotManager_exec_i () { }
		
		/// Default destructor.
		virtual ~RobotManager_exec_i ();

		// consumes  ProcessPallet ProcessPalletCommands;
		virtual void push_ProcessPalletCommands (RobotAssembly::ProcessPallet *ev ACE_ENV_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException));

		virtual RobotAssembly::CCM_RMController_ptr get_Controller (ACE_ENV_SINGLE_ARG_DECL)
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
		RobotManager_Exec_Context_var context_;
		friend class RMController_Impl;
	
	};
	//	provides RMController Controller;
	class RMController_Impl : public virtual RobotAssembly::CCM_RMController,
		public virtual TAO_Local_RefCounted_Object
	{
	public:
		RMController_Impl(RobotManager_exec_i& component) : component_ (component)
		{  }
		
		//    void SetProcessingStatus(in ProcessingOrderType Status);
		virtual void SetProcessingStatus (RobotAssembly::ProcessingOrderType Status ACE_ENV_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));

		//    ProcessingOrderType GetProcessingStatus();
		RobotAssembly::ProcessingOrderType GetProcessingStatus (ACE_ENV_SINGLE_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));

		//    string getHost();
		virtual char * getHost (ACE_ENV_SINGLE_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));
		
	private:
		RobotManager_exec_i& component_;
	};





	/**
	* @class RobotManagerHome_exec_i
	*
* RobotManager home executor implementation class.
*/
class ROBOTMANAGER_EXEC_Export RobotManagerHome_exec_i :
public virtual RobotManagerHome_Exec,
public virtual TAO_Local_RefCounted_Object
{
public:
	/// Default ctor.
	RobotManagerHome_exec_i ();
	
	/// Default dtor.
	virtual ~RobotManagerHome_exec_i ();
	
	// Implicit home operations.
	
	virtual ::Components::EnterpriseComponent_ptr
		create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
		ACE_THROW_SPEC ((CORBA::SystemException,
		Components::CCMException));
};

}

extern "C" ROBOTMANAGER_EXEC_Export ::Components::HomeExecutorBase_ptr
createRobotManagerHome_Impl (void);

//#endif /* ROBOTMANAGER_EXEC_H */
