//$Id$:
//============================================================
/**
* @file WatchSettingManager_exec.h
*
* Header file for the Executor implementation.
* @author Tao Lu <lu@dre.vanderbilt.edu>
*/
//============================================================

#ifndef WATCHSETTINGMANAGER_EXEC_H
#define WATCHSETTINGMANAGER_EXEC_H

#include "WatchSettingManagerEC.h"
#include "WatchSettingManager_exec_export.h"
#include "tao/LocalObject.h"

namespace WatchSettingManager_Impl
{
	
/**
* @class WatchSettingManager_exec_i
*
* WatchSettingManager executor implementation class.
	*/
	
	class WATCHSETTINGMANAGER_EXEC_Export WatchSettingManager_exec_i :
	public virtual WatchSettingManager_Exec,
		public virtual TAO_Local_RefCounted_Object
	{
		
	public:
		/// Default constructor.
		WatchSettingManager_exec_i ();
		
		/// Default destructor.
		~WatchSettingManager_exec_i ();

//	publishes DisplayWork Display;
//	publishes ProcessPallet ProductionControl;
//	publishes MovePalletRequest MovePallet;
//	publishes ProductionStatus ProductionReport;


//	provides WorkOrderResponses DisplayResponse;


		virtual RobotAssembly::CCM_WorkOrderResponses_ptr get_DisplayResponse (ACE_ENV_SINGLE_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));
		
//	consumes ProductionWorkOrder recvWorkOrder;
		virtual void
			push_recvWorkOrder (RobotAssembly::ProductionWorkOrder *ev
			ACE_ENV_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException));
//	consumes PalletProcessingStatus ProcessingStatus;
		virtual void
			push_ProcessingStatus (RobotAssembly::PalletProcessingStatus *ev
			ACE_ENV_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException));
//	consumes PalletStatusResponse PalletStatus;
		virtual void
			push_PalletStatus (RobotAssembly::PalletStatusResponse *ev
			ACE_ENV_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException));
		
		// Operations from Components::SessionComponent
		virtual void set_session_context (Components::SessionContext_ptr ctx
			ACE_ENV_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException,
			Components::CCMException));
		
		virtual void ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException,
			Components::CCMException));
		
		virtual void ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException,
			Components::CCMException));
		
		virtual void ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException,
			Components::CCMException));
		
		
		virtual void ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException,
			Components::CCMException));
		
		virtual void ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException,
			Components::CCMException));
		
	protected:
		/// Copmponent specific context
		WatchSettingManager_Exec_Context_var context_;
		friend class WorkOrderResponses_Impl;
	};
	
	// provides WorkOrderResponses DisplayResponse;
	class WorkOrderResponses_Impl : public virtual RobotAssembly::CCM_WorkOrderResponses,
		public virtual TAO_Local_RefCounted_Object
	{
	public:
		WorkOrderResponses_Impl(WatchSettingManager_exec_i& component) : component_ (component)
		{  }

		// 1f
		//    void AcceptWorkOrderResponse(in WorkOrder Order, in StatusType Status);
		virtual void AcceptWorkOrderResponse(const RobotAssembly::WorkOrder & Order, RobotAssembly::StatusType Status ACE_ENV_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));

		// 2f
		//    void SetTimeResponse(in WorkOrder Order, in StatusType Status);
		virtual void SetTimeResponse (const RobotAssembly::WorkOrder & Order, RobotAssembly::StatusType Status ACE_ENV_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));

		// 3f
		//    void AcceptFinalProductResponse(in WorkOrder, in StatusType Status);
		virtual void AcceptFinalProductResponse (const RobotAssembly::WorkOrder & Order,::RobotAssembly::StatusType Status ACE_ENV_ARG_DECL)
			ACE_THROW_SPEC ((CORBA::SystemException));

		
	private:
		WatchSettingManager_exec_i& component_;
	};  


	/**
	* @class WatchSettingManagerHome_exec_i
	*
	* WatchSettingManager home executor implementation class.
	*/
	class WATCHSETTINGMANAGER_EXEC_Export WatchSettingManagerHome_exec_i :
    public virtual WatchSettingManagerHome_Exec,
		public virtual TAO_Local_RefCounted_Object
	{
	public:
		/// Default ctor.
		WatchSettingManagerHome_exec_i ();
		
		/// Default dtor.
		~WatchSettingManagerHome_exec_i ();
		
		// Implicit home operations.
		
		virtual ::Components::EnterpriseComponent_ptr
			create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
			ACE_THROW_SPEC ((CORBA::SystemException,
			Components::CCMException));
	};
	
}

extern "C" WATCHSETTINGMANAGER_EXEC_Export ::Components::HomeExecutorBase_ptr
createWatchSettingManagerHome_Impl (void);

#endif /* WATCHSETTINGMANAGER_EXEC_H */
