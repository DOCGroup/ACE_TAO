// $Id$
// HumanMachineInterface_exec.h
//============================================================
/**
* @file HumanMachineInterface_exec.h
*
* Header file for the Executor implementation.
*/
//============================================================

//#ifndef HUMANMACHINEINTERFACE_EXEC_H
//#define HUMANMACHINEINTERFACE_EXEC_H

#include "HumanMachineInterfaceEC.h"
#include "HumanMachineInterface_exec_export.h"

#include "tao/LocalObject.h"

namespace HumanMachineInterface_Impl
{
/**
* @class HumanMachineInterface_exec_i
*
* HumanMachineInterface executor implementation class.
	*/
	
	class HUMANMACHINEINTERFACE_EXEC_Export HumanMachineInterface_exec_i :
	public virtual HumanMachineInterface_Exec, 
		public virtual TAO_Local_RefCounted_Object
	{
		
	public:
		/// Default constructor.
		HumanMachineInterface_exec_i () { }
		
		/// Default destructor.
		virtual ~HumanMachineInterface_exec_i ();


// consumes DisplayWork WorkDisplayUpdate;
		virtual void push_WorkDisplayUpdate (RobotAssembly::DisplayWork *ev ACE_ENV_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException));

		virtual RobotAssembly::CCM_HMIController_ptr get_Controller (ACE_ENV_SINGLE_ARG_DECL)
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
		HumanMachineInterface_Exec_Context_var context_;
		friend class HMIController_Impl;
	};


// provides HMIController Controller;	
	class HMIController_Impl : public virtual RobotAssembly::CCM_HMIController,
		public virtual TAO_Local_RefCounted_Object
	{
	public:
		HMIController_Impl(HumanMachineInterface_exec_i& component) : component_ (component)
		{  }
		
		//    string TextToDisplay();
		virtual char * TextToDisplay (ACE_ENV_SINGLE_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));

		//    void  KeyResponse( in short key);
		virtual void KeyResponse (CORBA::Short key ACE_ENV_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));
		//    string getHost();
		virtual char * getHost (ACE_ENV_SINGLE_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));

		
	private:
		HumanMachineInterface_exec_i& component_;
	};
	
	

	
	/**
	* @class HumanMachineInterfaceHome_exec_i
	*
* HumanMachineInterface home executor implementation class.
*/
class HUMANMACHINEINTERFACE_EXEC_Export HumanMachineInterfaceHome_exec_i :
public virtual HumanMachineInterfaceHome_Exec,
public virtual TAO_Local_RefCounted_Object
{
public:
	/// Default ctor.
	HumanMachineInterfaceHome_exec_i ();
	
	/// Default dtor.
	virtual ~HumanMachineInterfaceHome_exec_i ();
	
	// Implicit home operations.
	
	virtual ::Components::EnterpriseComponent_ptr
		create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
		ACE_THROW_SPEC ((CORBA::SystemException,
		Components::CCMException));
};

}

extern "C" HUMANMACHINEINTERFACE_EXEC_Export ::Components::HomeExecutorBase_ptr
createHumanMachineInterfaceHome_Impl (void);

//#endif /* MANAGEMENTWORKINSTRUCTONS_EXEC_H */
