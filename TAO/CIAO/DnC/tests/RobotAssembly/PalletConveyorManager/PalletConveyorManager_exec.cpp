// $Id$
// PalletConveyorManager_exec.cpp

#include "PalletConveyorManager_exec.h"

// MangementWorkInstructions_exec_i
//
//
PalletConveyorManager_Impl::PalletConveyorManager_exec_i::~PalletConveyorManager_exec_i ()
{
}

//consumes MovePalletRequest PalletRequests;
void 
PalletConveyorManager_Impl::PalletConveyorManager_exec_i::push_PalletRequests (RobotAssembly::MovePalletRequest *ev ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	ACE_DEBUG ((LM_DEBUG, "PalletConveyorManager - In push_PalletStatus event sink (4e, 10e)\n"));
		//	  eventtype MovePalletRequest {
		//			public  MoveRequestType MoveRequest;
		//			public WorkOrder Order;
		//		};

	RobotAssembly::WorkOrder myWorkOrder = ev->Order();
	RobotAssembly::MoveRequestType myMoveRequestType = ev->MoveRequest();
	if (myMoveRequestType == RobotAssembly::GetPalletProductToProcess) {
		printf("\tMoveRequestType = GetPalletProductToProcess (4e)\n");
		// Simulate Pallet Ready - this code should be moved to the PCMController once the GUIs are attached
		// eventtype PalletStatusResponse {
		//		public StatusType Status;
		//		public WorkOrder Order;
		// };
		// 5e
		// publishes PalletStatusResponse PalletStatus;
		RobotAssembly::PalletStatusResponse_var palletStatusResponse_event = new OBV_RobotAssembly::PalletStatusResponse;
		palletStatusResponse_event->Order(myWorkOrder);
		palletStatusResponse_event->Status(RobotAssembly::palletInPlace);
		this->context_->push_PalletStatus(palletStatusResponse_event ACE_ENV_ARG_PARAMETER);

	} else if (myMoveRequestType == RobotAssembly::MovePalletToFinishingArea) {
		printf("\tMoveRequestType = MovePalletToFinishingArea (10e)\n");
		// Simulate Pallet Ready - this code should be moved to the PCMController once the GUIs are attached
		// eventtype PalletStatusResponse {
		//		public StatusType Status;
		//		public WorkOrder Order;
		// };
		// 11e
		// publishes PalletStatusResponse PalletStatus;
		RobotAssembly::PalletStatusResponse_var palletStatusResponse_event = new OBV_RobotAssembly::PalletStatusResponse;
		palletStatusResponse_event->Order(myWorkOrder);
		palletStatusResponse_event->Status(RobotAssembly::complete);
		this->context_->push_PalletStatus(palletStatusResponse_event ACE_ENV_ARG_PARAMETER);

	}
}

RobotAssembly::CCM_PCMController_ptr 
PalletConveyorManager_Impl::PalletConveyorManager_exec_i::get_Controller (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	ACE_DEBUG ((LM_DEBUG, "PalletConveyorManager_Impl::PalletConveyorManager_exec.i::get_Controller called\n "));
	return (new PCMController_Impl (*this));
}

//    void SetDiscrete( in Discretes discrete);
void 
PalletConveyorManager_Impl::PCMController_Impl::SetPalletStatus (ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
}

//    DiscreteValue GetDiscreteStatus( in Discretes discrete);
RobotAssembly::MovePalletRequestType
PalletConveyorManager_Impl::PCMController_Impl::GetOrderStatus (ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	RobotAssembly::MovePalletRequestType returnVal;
	return returnVal;
}

//    MovePalletRequestType GetOrderStatus();
RobotAssembly::DiscreteValue 
PalletConveyorManager_Impl::PCMController_Impl::GetDiscreteStatus (RobotAssembly::Discretes discrete ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	RobotAssembly::DiscreteValue returnVal;
	return returnVal;
}

//    void SetPalletStatus();
void 
PalletConveyorManager_Impl::PCMController_Impl::SetDiscrete (RobotAssembly::Discretes discrete ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
}
//    string getHost();	
char * 
PalletConveyorManager_Impl::PCMController_Impl::getHost (ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	return "getHost";
}








// Operations from Components::SessionComponen
void
PalletConveyorManager_Impl::PalletConveyorManager_exec_i::set_session_context (Components::SessionContext_ptr ctx
																						 ACE_ENV_ARG_DECL)
																						 ACE_THROW_SPEC ((CORBA::SystemException,
																						 Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG, "PalletConveyorManager_Impl::PalletConveyorManager_exec_i::set_session_context\n"));
	
	this->context_ = PalletConveyorManager_Impl::PalletConveyorManager_Exec_Context::_narrow (ctx ACE_ENV_ARG_PARAMETER);
	
	ACE_CHECK;
	
	if (CORBA::is_nil (this->context_.in ()))
		
		ACE_THROW (CORBA::INTERNAL ());
	// Urm, we actually discard exceptions thown from this operation.
}

void
PalletConveyorManager_Impl::PalletConveyorManager_exec_i::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG,"PalletConveyorManager_Impl::PalletConveyorManager_exec_i::ciao_preactivate\n"));
}

void
PalletConveyorManager_Impl::PalletConveyorManager_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG,"PalletConveyorManager_Impl::PalletConveyorManager_exec_i::ccm_activate\n"));
}

void
PalletConveyorManager_Impl::PalletConveyorManager_exec_i::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG,"PalletConveyorManager_Impl::PalletConveyorManager_exec_i::ciao_postactivate\n"));
}


void
PalletConveyorManager_Impl::PalletConveyorManager_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG, "PalletConveyorManager_Impl::PalletConveyorManager_exec_i::ccm_passivate\n"));
}

void
PalletConveyorManager_Impl::PalletConveyorManager_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG, "PalletConveyorManager_Impl::PalletConveyorManager_exec_i::ccm_remove\n"));
}


// PalletConveyorManagerHome_exec_i
//
//
PalletConveyorManager_Impl::PalletConveyorManagerHome_exec_i::PalletConveyorManagerHome_exec_i ()
{
}

PalletConveyorManager_Impl::PalletConveyorManagerHome_exec_i::~PalletConveyorManagerHome_exec_i ()
{
}

::Components::EnterpriseComponent_ptr
PalletConveyorManager_Impl::PalletConveyorManagerHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	return new PalletConveyorManager_Impl::PalletConveyorManager_exec_i;
}


extern "C" PALLETCONVEYORMANAGER_EXEC_Export ::Components::HomeExecutorBase_ptr
createPalletConveyorManagerHome_Impl (void)
{
	return new PalletConveyorManager_Impl::PalletConveyorManagerHome_exec_i ();
}
