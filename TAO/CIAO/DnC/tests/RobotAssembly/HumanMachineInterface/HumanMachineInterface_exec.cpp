// $Id$
// HumanMachineInterface_exec.cpp

#include "HumanMachineInterface_exec.h"

// MangementWorkInstructions_exec_i
//
//
HumanMachineInterface_Impl::HumanMachineInterface_exec_i::~HumanMachineInterface_exec_i ()
{
}

// consumes DisplayWork WorkDisplayUpdate;
void 
HumanMachineInterface_Impl::HumanMachineInterface_exec_i::push_WorkDisplayUpdate (RobotAssembly::DisplayWork *ev ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	ACE_DEBUG ((LM_DEBUG, "HumanMachineInterface - In push_WorkDisplayUpdate event sink \n"));
	/*    eventtype DisplayWork {
      public DisplayRequest Display;
      public WorkOrder Order;
      public StatusType Status;
   };
*/
	// Following Code Simulates what will happen when the gui is attached
	// This codes should eventually be removed and replaced

	RobotAssembly::StatusType myStatus = ev->Status();
	RobotAssembly::WorkOrder myOrder = ev->Order();
	RobotAssembly::DisplayRequest myDisplay = ev->Display();

	if (myStatus == RobotAssembly::unacknowledged) {
		printf("\tWorkOrder %d was unacknowledged and is changed to accepted (2e) \n", myOrder.OrderNumber);
		myStatus = RobotAssembly::accepted;
		// 1f call
		//    void AcceptWorkOrderResponse(in WorkOrder Order, in StatusType Status);
		RobotAssembly::WorkOrderResponses_var rev 
			 = this->context_->get_connection_HumanResponse (ACE_ENV_SINGLE_ARG_PARAMETER);
		ACE_CHECK;

		if (CORBA::is_nil (rev.in ()))
			ACE_THROW (CORBA::BAD_INV_ORDER ());

		rev->AcceptWorkOrderResponse(myOrder, myStatus ACE_ENV_SINGLE_ARG_PARAMETER);

			
	} else if (myStatus == RobotAssembly::accepted) {
		printf("\tWorkOrder %d was accepted and is changed to ready (6e) \n", myOrder.OrderNumber);
		myStatus = RobotAssembly::ready;
		// 2f call
		//    void SetTimeResponse(in WorkOrder Order, in StatusType Status);
		RobotAssembly::WorkOrderResponses_var rev 
			 = this->context_->get_connection_HumanResponse (ACE_ENV_SINGLE_ARG_PARAMETER);
		ACE_CHECK;

		if (CORBA::is_nil (rev.in ()))
			ACE_THROW (CORBA::BAD_INV_ORDER ());

		rev->SetTimeResponse(myOrder, myStatus ACE_ENV_SINGLE_ARG_PARAMETER);

	} else if (myStatus == RobotAssembly::ready) {
		printf("\tWorkOrder %d was ready and is changed to completed (9e) \n", myOrder.OrderNumber);
		myStatus = RobotAssembly::complete;
		// 3f call
		//    void AcceptFinalProductResponse(in WorkOrder, in StatusType Status);
		RobotAssembly::WorkOrderResponses_var rev 
			 = this->context_->get_connection_HumanResponse (ACE_ENV_SINGLE_ARG_PARAMETER);
		ACE_CHECK;

		if (CORBA::is_nil (rev.in ()))
			ACE_THROW (CORBA::BAD_INV_ORDER ());

		rev->AcceptFinalProductResponse(myOrder, myStatus ACE_ENV_SINGLE_ARG_PARAMETER);
	} else {
		printf("\tERROR: WorkOrder %d has an invalid status value\n", myOrder.OrderNumber);
	}

}

RobotAssembly::CCM_HMIController_ptr 
HumanMachineInterface_Impl::HumanMachineInterface_exec_i::get_Controller (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	ACE_DEBUG ((LM_DEBUG, "HumanMachineInterface_Impl::HumanMachineInterface_exec.i::get_Controller called\n "));
	return (new HMIController_Impl (*this));
}

// provides HMIController Controller;
//    string TextToDisplay();
char * 
HumanMachineInterface_Impl::HMIController_Impl::TextToDisplay (ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	return "TextToDisplay";
}

//    void  KeyResponse( in short key);
void 
HumanMachineInterface_Impl::HMIController_Impl::KeyResponse (CORBA::Short key ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
}

//    string getHost();
char * 
HumanMachineInterface_Impl::HMIController_Impl::getHost (ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	return "getHost";
}



// Operations from Components::SessionComponen
void
HumanMachineInterface_Impl::HumanMachineInterface_exec_i::set_session_context (Components::SessionContext_ptr ctx ACE_ENV_ARG_DECL)
																						 ACE_THROW_SPEC ((CORBA::SystemException,
																						 Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG, "HumanMachineInterface_Impl::HumanMachineInterface_exec_i::set_session_context\n"));
	
	this->context_ = HumanMachineInterface_Impl::HumanMachineInterface_Exec_Context::_narrow (ctx ACE_ENV_ARG_PARAMETER);
	
	ACE_CHECK;
	
	if (CORBA::is_nil (this->context_.in ()))
		
		ACE_THROW (CORBA::INTERNAL ());
	// Urm, we actually discard exceptions thown from this operation.
}

void
HumanMachineInterface_Impl::HumanMachineInterface_exec_i::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG,"HumanMachineInterface_Impl::HumanMachineInterface_exec_i::ciao_preactivate\n"));
}

void
HumanMachineInterface_Impl::HumanMachineInterface_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG,"HumanMachineInterface_Impl::HumanMachineInterface_exec_i::ccm_activate\n"));
}

void
HumanMachineInterface_Impl::HumanMachineInterface_exec_i::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG,"HumanMachineInterface_Impl::HumanMachineInterface_exec_i::ciao_postactivate\n"));
}


void
HumanMachineInterface_Impl::HumanMachineInterface_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG, "HumanMachineInterface_Impl::HumanMachineInterface_exec_i::ccm_passivate\n"));
}

void
HumanMachineInterface_Impl::HumanMachineInterface_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG, "HumanMachineInterface_Impl::HumanMachineInterface_exec_i::ccm_remove\n"));
}


// HumanMachineInterfaceHome_exec_i
//
//
HumanMachineInterface_Impl::HumanMachineInterfaceHome_exec_i::HumanMachineInterfaceHome_exec_i ()
{
}

HumanMachineInterface_Impl::HumanMachineInterfaceHome_exec_i::~HumanMachineInterfaceHome_exec_i ()
{
}

::Components::EnterpriseComponent_ptr
HumanMachineInterface_Impl::HumanMachineInterfaceHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	return new HumanMachineInterface_Impl::HumanMachineInterface_exec_i;
}


extern "C" HUMANMACHINEINTERFACE_EXEC_Export ::Components::HomeExecutorBase_ptr
createHumanMachineInterfaceHome_Impl (void)
{
	return new HumanMachineInterface_Impl::HumanMachineInterfaceHome_exec_i ();
}
