// $Id$
// ManagementWorkInstructions_exec.cpp

#include "ManagementWorkInstructions_exec.h"

// MangementWorkInstructions_exec_i
//
//
ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec_i::~ManagementWorkInstructions_exec_i ()
{
}

// consumes ProductionStatus ProductionReport;
// 3e, 12e
//  eventtype  ProductionStatus {
//	  public StatusType Status;
//	  public WorkOrder Order;
//  };

void 
ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec_i::push_ProductionReport (RobotAssembly::ProductionStatus *ev ACE_ENV_ARG_DECL_WITH_DEFAULTS)
ACE_THROW_SPEC ((CORBA::SystemException)) {
	printf("ManagmentWorkInstructions - In consumes ProductionStatus Production Report\n");
	// 3e, 12e
  /*eventtype  ProductionStatus {
	  public  StatusType Status;
	  public WorkOrder Order;
  };*/

	RobotAssembly::StatusType myStatus = ev->Status();
	RobotAssembly::WorkOrder myOrder = ev->Order();

	if (myStatus == RobotAssembly::accepted) {
		printf("\tWorkOrder %d was accepted (3e) \n", myOrder.OrderNumber);
	} /*else if (myStatus == RobotAssembly::rejected) {
		printf("\tWorkOrder %d was rejected\n", myOrder.OrderNumber);
	} */else if (myStatus == RobotAssembly::complete) {
		printf("\tYEAH WE REACHED THE END\n\tWorkOrder %d was complete (12e)\n", myOrder.OrderNumber);
	} /*else if (myStatus == RobotAssembly::failed) {
		printf("\tWorkOrder %d failed\n", myOrder.OrderNumber);
	} */else {
		printf("\tERROR: WorkOrder %d has an invalid status value\n", myOrder.OrderNumber);
	}
}


RobotAssembly::CCM_MWIController_ptr 
ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec_i::get_Controller (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	ACE_DEBUG ((LM_DEBUG, "ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec.i::get_Controller called\n "));
	return (new MWIController_Impl (*this));
}


void
ManagementWorkInstructions_Impl::MWIController_Impl::SendStringTest (const char *testString ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	ACE_DEBUG ((LM_DEBUG, "ManagementWorkInstructions initiates the process.\n"));
	printf("********************************************\n");
	ACE_DEBUG ((LM_DEBUG, "testString = %s\n", testString));
	printf("********************************************\n");

//	RobotAssembly::ProductionWorkOrder_var event = new OBV_RobotAssembly::ProductionWorkOrder;
//	this->component_.context_->push_sendWorkOrder(event ACE_ENV_ARG_PARAMETER);
}


void 
ManagementWorkInstructions_Impl::MWIController_Impl::SendOrder(const ::RobotAssembly::WorkOrder & testOrder ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException)) 
{
	printf("ManagementWorkInstructions - In Send Order\n");
	RobotAssembly::ProductionWorkOrder_var event = new OBV_RobotAssembly::ProductionWorkOrder;
	
	event->Order(testOrder);

	this->component_.context_->push_sendWorkOrder(event ACE_ENV_ARG_PARAMETER);

}

RobotAssembly::ProductionStatusType ManagementWorkInstructions_Impl::MWIController_Impl::GetOrderStatus (ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	// FILL IN IMPLEMENTATION CODE
	printf("Inside MWI:GetOrderStatus\n");
	RobotAssembly::ProductionStatusType pType;
	pType.OrderNumber = 1;
	pType.ProductNumber = 2;
	pType.Status = RobotAssembly::accepted;
	//	pType.TimeComplete = 5;
	pType.RequestedProduction = 333;
	pType.ActualProduction = 342; 
	
	return pType;
}

char * ManagementWorkInstructions_Impl::MWIController_Impl::getHost (ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	// FILL IN IMPLEMENTATION CODE
	printf("Inside MWI: getHost\n");
	return "";
}

// Operations from Components::SessionComponen
void
ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec_i::set_session_context (Components::SessionContext_ptr ctx
																						 ACE_ENV_ARG_DECL)
																						 ACE_THROW_SPEC ((CORBA::SystemException,
																						 Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG, "ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec_i::set_session_context\n"));
	
	this->context_ = ManagementWorkInstructions_Impl::ManagementWorkInstructions_Exec_Context::_narrow (ctx ACE_ENV_ARG_PARAMETER);
	
	ACE_CHECK;
	
	if (CORBA::is_nil (this->context_.in ()))
		
		ACE_THROW (CORBA::INTERNAL ());
	// Urm, we actually discard exceptions thown from this operation.
}

void
ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec_i::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG,"ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec_i::ciao_preactivate\n"));
}

void
ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG,"ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec_i::ccm_activate\n"));
}

void
ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec_i::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG,"ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec_i::ciao_postactivate\n"));
}


void
ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG, "ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec_i::ccm_passivate\n"));
}

void
ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	ACE_DEBUG ((LM_DEBUG, "ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec_i::ccm_remove\n"));
}


// ManagementWorkInstructionsHome_exec_i
//
//
ManagementWorkInstructions_Impl::ManagementWorkInstructionsHome_exec_i::ManagementWorkInstructionsHome_exec_i ()
{
}

ManagementWorkInstructions_Impl::ManagementWorkInstructionsHome_exec_i::~ManagementWorkInstructionsHome_exec_i ()
{
}

::Components::EnterpriseComponent_ptr
ManagementWorkInstructions_Impl::ManagementWorkInstructionsHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
				Components::CCMException))
{
	return new ManagementWorkInstructions_Impl::ManagementWorkInstructions_exec_i;
}


extern "C" MANAGEMENTWORKINSTRUCTIONS_EXEC_Export ::Components::HomeExecutorBase_ptr
createManagementWorkInstructionsHome_Impl (void)
{
	return new ManagementWorkInstructions_Impl::ManagementWorkInstructionsHome_exec_i ();
}
