// $Id$
// PalletConveyorManager_exec.h
//============================================================
/**
* @file PalletConveyorManager_exec.h
*
* Header file for the Executor implementation.
*/
//============================================================

//#ifndef PALLETCONVEYORMANAGER_EXEC_H
//#define PALLETCONVEYORMANAGER_EXEC_H

#include "PalletConveyorManagerEC.h"
#include "PalletConveyorManager_exec_export.h"

#include "tao/LocalObject.h"

namespace PalletConveyorManager_Impl
{
/**
* @class PalletConveyorManager_exec_i
*
* PalletConveyorManager executor implementation class.
	*/
	
	class PALLETCONVEYORMANAGER_EXEC_Export PalletConveyorManager_exec_i :
	public virtual PalletConveyorManager_Exec, 
		public virtual TAO_Local_RefCounted_Object
	{
		
	public:
		/// Default constructor.
		PalletConveyorManager_exec_i () { }
		
		/// Default destructor.
		virtual ~PalletConveyorManager_exec_i ();
	

		//consumes MovePalletRequest PalletRequests;
		virtual void push_PalletRequests (RobotAssembly::MovePalletRequest *ev ACE_ENV_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException));

		virtual RobotAssembly::CCM_PCMController_ptr get_Controller (ACE_ENV_SINGLE_ARG_DECL)
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
		PalletConveyorManager_Exec_Context_var context_;
		friend class PCMController_Impl;
		
	};

//	provides PCMController Controller;
	class PCMController_Impl : public virtual RobotAssembly::CCM_PCMController,
		public virtual TAO_Local_RefCounted_Object
	{
	public:
		PCMController_Impl(PalletConveyorManager_exec_i& component) : component_ (component)
		{  }
		//    void SetDiscrete( in Discretes discrete);
		virtual void SetPalletStatus (ACE_ENV_SINGLE_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));

		//    DiscreteValue GetDiscreteStatus( in Discretes discrete);
		virtual RobotAssembly::MovePalletRequestType GetOrderStatus (ACE_ENV_SINGLE_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));

		//    MovePalletRequestType GetOrderStatus();
		virtual RobotAssembly::DiscreteValue GetDiscreteStatus (RobotAssembly::Discretes discrete ACE_ENV_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));

		//    void SetPalletStatus();
		virtual void SetDiscrete (RobotAssembly::Discretes discrete ACE_ENV_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));

		//    string getHost();	
		virtual char * getHost (ACE_ENV_SINGLE_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));
		
	private:
		PalletConveyorManager_exec_i& component_;
	};


	/**
	* @class PalletConveyorManagerHome_exec_i
	*
* PalletConveyorManager home executor implementation class.
*/
class PALLETCONVEYORMANAGER_EXEC_Export PalletConveyorManagerHome_exec_i :
public virtual PalletConveyorManagerHome_Exec,
public virtual TAO_Local_RefCounted_Object
{
public:
	/// Default ctor.
	PalletConveyorManagerHome_exec_i ();
	
	/// Default dtor.
	virtual ~PalletConveyorManagerHome_exec_i ();
	
	// Implicit home operations.
	
	virtual ::Components::EnterpriseComponent_ptr
		create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
		ACE_THROW_SPEC ((CORBA::SystemException,
		Components::CCMException));
};

}

extern "C" PALLETCONVEYORMANAGER_EXEC_Export ::Components::HomeExecutorBase_ptr
createPalletConveyorManagerHome_Impl (void);

//#endif /* PALLETCONVEYORMANAGER_EXEC_H */
