// $Id$
// RobotManager_exec.cpp

#include "RobotManager_exec.h"

// RobotManager_exec_i
//
//
RobotManager_Impl::RobotManager_exec_i::~RobotManager_exec_i ()
{
}

// consumes  ProcessPallet ProcessPalletCommands;
void 
RobotManager_Impl::RobotManager_exec_i::push_ProcessPalletCommands (RobotAssembly::ProcessPallet *ev ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{

	//	 eventtype ProcessPallet {
	//		public ProcessingOrderType ProcessingOrder;
	//		public WorkOrder Order;
	//	 };
	
	RobotAssembly::WorkOrder myWorkOrder = ev->Order();
	// myProcessingOrderType should have value BeginProcessingPallet
	RobotAssembly::ProcessingOrderType myProcessingOrderType = ev->ProcessingOrder(); 

	ACE_DEBUG ((LM_DEBUG, "RobotManager - In push_ProcessPalletCommands event sink (7e)\n"));

	// Simulate call 8e - this code should be move to the RMController once the GUI is added
	//  eventtype PalletProcessingStatus {
    //		public StatusType Status;
	//		public WorkOrder Order;
	//	};
	// 8e publishes PalletProcessingStatus ProcessingStatus;
	RobotAssembly::PalletProcessingStatus_var palletProcessingStatus_event = new OBV_RobotAssembly::PalletProcessingStatus;
	palletProcessingStatus_event->Order(myWorkOrder);
	palletProcessingStatus_event->Status(RobotAssembly::palletProcessed);
	this->context_->push_ProcessingStatus(palletProcessingStatus_event ACE_ENV_ARG_PARAMETER);

}

RobotAssembly::CCM_RMController_ptr 
RobotManager_Impl::RobotManager_exec_i::get_Controller (ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	ACE_DEBUG ((LM_DEBUG, "RobotManager_Impl::RobotManager_exec.i::get_Controller called\n "));
	return (new RMController_Impl (*this));
}
		
//	provides RMController Controller;
void 
RobotManager_Impl::RMController_Impl::SetProcessingStatus (RobotAssembly::ProcessingOrderType Status ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
}

//    ProcessingOrderType GetProcessingStatus();
RobotAssembly::ProcessingOrderType 
RobotManager_Impl::RMController_Impl::GetProcessingStatus (ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	RobotAssembly::ProcessingOrderType returnType;
	return returnType;
}

//    string getHost();
char * 
RobotManager_Impl::RMController_Impl::getHost (ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	return "getHost";
}





// Operations from Components::SessionComponen
void
RobotManager_Impl::RobotManager_exec_i::set_session_context (Components::SessionContext_ptr ctx
																						 ACE_ENV_ARG_DECL)
																						 ACE_THROW_SPEC ((CORBA::SystemException,
																						 Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG, "RobotManager_Impl::RobotManager_exec_i::set_session_context\n"));
	
	this->context_ = RobotManager_Impl::RobotManager_Exec_Context::_narrow (ctx ACE_ENV_ARG_PARAMETER);
	
	ACE_CHECK;
	
	if (CORBA::is_nil (this->context_.in ()))
		
		ACE_THROW (CORBA::INTERNAL ());
	// Urm, we actually discard exceptions thown from this operation.
}

void
RobotManager_Impl::RobotManager_exec_i::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG,"RobotManager_Impl::RobotManager_exec_i::ciao_preactivate\n"));
}

void
RobotManager_Impl::RobotManager_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG,"RobotManager_Impl::RobotManager_exec_i::ccm_activate\n"));
}

void
RobotManager_Impl::RobotManager_exec_i::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG,"RobotManager_Impl::RobotManager_exec_i::ciao_postactivate\n"));
}


void
RobotManager_Impl::RobotManager_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG, "RobotManager_Impl::RobotManager_exec_i::ccm_passivate\n"));
}

void
RobotManager_Impl::RobotManager_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG, "RobotManager_Impl::RobotManager_exec_i::ccm_remove\n"));
}


// RobotManagerHome_exec_i
//
//
RobotManager_Impl::RobotManagerHome_exec_i::RobotManagerHome_exec_i ()
{
}

RobotManager_Impl::RobotManagerHome_exec_i::~RobotManagerHome_exec_i ()
{
}

::Components::EnterpriseComponent_ptr
RobotManager_Impl::RobotManagerHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	return new RobotManager_Impl::RobotManager_exec_i;
}


extern "C" ROBOTMANAGER_EXEC_Export ::Components::HomeExecutorBase_ptr
createRobotManagerHome_Impl (void)
{
	return new RobotManager_Impl::RobotManagerHome_exec_i ();
}
