//$Id$
/*
 * @file WatchSettingManager_exec.cpp
 *
 * @author Tao Lu <lu@dre.vanderbilt.edu>
 */

#include "WatchSettingManager_exec.h"
#include "CIAO_common.h"

WatchSettingManager_Impl::WatchSettingManager_exec_i::WatchSettingManager_exec_i ()
{
}

WatchSettingManager_Impl::WatchSettingManager_exec_i::~WatchSettingManager_exec_i ()
{
}


RobotAssembly::CCM_WorkOrderResponses_ptr 
WatchSettingManager_Impl::WatchSettingManager_exec_i::get_DisplayResponse (ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException)) {
	ACE_DEBUG ((LM_DEBUG, "WatchSettingManager_Impl::WatchSettingManager_exec.i::get_DisplayResponse called\n "));
	return (new WorkOrderResponses_Impl (*this));
}



// provides WorkOrderResponses DisplayResponse;
//    void AcceptWorkOrderResponse(in WorkOrder Order, in StatusType Status);
// 1f
void 
WatchSettingManager_Impl::WorkOrderResponses_Impl::AcceptWorkOrderResponse (const RobotAssembly::WorkOrder & Order, RobotAssembly::StatusType Status ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException)) 
{
	printf("WatchSettingManager  - In AcceptWorkOrderResponse (1f)\n");
	if (Status == RobotAssembly::accepted) {
		printf("\tAccept work order Product\n");
		//   eventtype  ProductionStatus {
		//			public  StatusType Status;
		//			public WorkOrder Order;
		//	  };
		// Follow 3e - publishes ProductionStatus ProductionReport
		RobotAssembly::ProductionStatus_var productionStatus_event = new OBV_RobotAssembly::ProductionStatus;
		productionStatus_event->Order(Order);
		productionStatus_event->Status(RobotAssembly::accepted);
		this->component_.context_->push_ProductionReport(productionStatus_event ACE_ENV_ARG_PARAMETER);

		//	  eventtype MovePalletRequest {
		//			public  MoveRequestType MoveRequest;
		//			public WorkOrder Order;
		//		};

		// Follow 4e - publishes MovePalletRequest MovePallet
		RobotAssembly::MovePalletRequest_var movePalletRequest_event = new OBV_RobotAssembly::MovePalletRequest;
		movePalletRequest_event->Order(Order);
		movePalletRequest_event->MoveRequest(RobotAssembly::GetPalletProductToProcess);
		this->component_.context_->push_MovePallet(movePalletRequest_event ACE_ENV_ARG_PARAMETER);


	} else if (Status == RobotAssembly::rejected) {
		printf("\tReject work order Product\n");
	} else {
		printf("\tInvalid response to Accept Work Order Question\n");
	}
}

//    void SetTimeResponse(in WorkOrder Order, in StatusType Status);
// 2f
void 
WatchSettingManager_Impl::WorkOrderResponses_Impl::SetTimeResponse (const RobotAssembly::WorkOrder & Order, RobotAssembly::StatusType Status ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	printf("WatchSettingManager  - In SetTimeResponse (2f)\n");
	if (Status == RobotAssembly::ready) {
		printf("\tReady to set time\n");

		//	 eventtype ProcessPallet {
		//		public ProcessingOrderType ProcessingOrder;
		//		public WorkOrder Order;
		//	 };
		// Follow 7e - publishes ProcessPallet ProductionControl
		RobotAssembly::ProcessPallet_var processPallet_event = new OBV_RobotAssembly::ProcessPallet;
		processPallet_event->Order(Order);
		processPallet_event->ProcessingOrder(RobotAssembly::BeginProcessingPallet);
		this->component_.context_->push_ProductionControl(processPallet_event ACE_ENV_ARG_PARAMETER);

	} else if (Status == RobotAssembly::rejected) {
		printf("\tRejected Setting the time\n");
	} else {
		printf("\tInvalid response to Set Time Question\n");
	}
}			

//    void AcceptFinalProductResponse(in WorkOrder, in StatusType Status);
// 3f
void 
WatchSettingManager_Impl::WorkOrderResponses_Impl::AcceptFinalProductResponse (const RobotAssembly::WorkOrder & Order, RobotAssembly::StatusType Status ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException)) 
{
	printf("WatchSettingManager  - In AcceptFinalProductResponse (3f)\n");
	if (Status == RobotAssembly::complete) {
		printf("\tAccept final Product\n");
		//	  eventtype MovePalletRequest {
		//			public MoveRequestType MoveRequest;
		//			public WorkOrder Order;
		//		};
		// Follow 10e - publishes MovePalletRequest MovePallet
		RobotAssembly::MovePalletRequest_var movePalletRequest_event = new OBV_RobotAssembly::MovePalletRequest;
		movePalletRequest_event->Order(Order);
		movePalletRequest_event->MoveRequest(RobotAssembly::MovePalletToFinishingArea);
		this->component_.context_->push_MovePallet(movePalletRequest_event ACE_ENV_ARG_PARAMETER);

	} else if (Status == RobotAssembly::rejected) {
		printf("\tReject final Product\n");
	} else {
		printf("\tInvalid response to Accept Final Product Question\n");
	}
}






void
WatchSettingManager_Impl::WatchSettingManager_exec_i::push_recvWorkOrder (RobotAssembly::ProductionWorkOrder *ev
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //Get the message from the Sender first.
  ACE_DEBUG ((LM_DEBUG,"WatchSettingManager - In recvWorkOrder \n"));

  RobotAssembly::WorkOrder myWorkOrder = ev->Order();
  printf("Work Order Number %d, ProductNumber = %d\n", myWorkOrder.OrderNumber, myWorkOrder.ProductNumber);

  
 
 // RobotAssembly::ProductionWorkOrder_var event = new OBV_RobotAssembly::ProductionWorkOrder;
	
//	event->Order(testOrder);

//	this->component_.context_->push_sendWorkOrder(event ACE_ENV_ARG_PARAMETER);

  // Send work order to HMI to prompt user as to if it should be accepted
	RobotAssembly::DisplayWork_var displayWork_event = new OBV_RobotAssembly::DisplayWork;
	displayWork_event->Display(RobotAssembly::DisplayWorkOrder);
	displayWork_event->Order(myWorkOrder);
	displayWork_event->Status(RobotAssembly::unacknowledged);
	// 2e
	this->context_->push_Display(displayWork_event ACE_ENV_ARG_PARAMETER);
/*    eventtype DisplayWork {
      public DisplayRequest Display;
      public WorkOrder Order;
      public StatusType Status;
   };
*/

}


//	consumes PalletProcessingStatus ProcessingStatus;
void
WatchSettingManager_Impl::WatchSettingManager_exec_i::push_ProcessingStatus (RobotAssembly::PalletProcessingStatus *ev ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException)) 
{

	RobotAssembly::WorkOrder myWorkOrder = ev->Order();
	RobotAssembly::StatusType myStatus = ev->Status();
	//  eventtype PalletProcessingStatus {
    //		public StatusType Status;
	//		public WorkOrder Order;
	//	};
	ACE_DEBUG ((LM_DEBUG, "WatchSettingManager - In push_ProcessingStatus event sink (8e) \n"));
	// if (palletProcessed (call was 5e))  9e
	// eventtype DisplayWork {
	//		public DisplayRequest Display;
	//		public WorkOrder Order;
	//		public StatusType Status;
	// };
	// 9e publishes DisplayWork Display;
	RobotAssembly::DisplayWork_var displayWork_event = new OBV_RobotAssembly::DisplayWork;
	displayWork_event->Display(RobotAssembly::DisplayWorkProductionComplete);
	displayWork_event->Order(myWorkOrder);
	displayWork_event->Status(RobotAssembly::ready);
	this->context_->push_Display(displayWork_event ACE_ENV_ARG_PARAMETER);

}
//	consumes PalletStatusResponse PalletStatus;
void
WatchSettingManager_Impl::WatchSettingManager_exec_i::push_PalletStatus (RobotAssembly::PalletStatusResponse *ev ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
	// eventtype PalletStatusResponse {
	//		public StatusType Status;
	//		public WorkOrder Order;
	// };
	RobotAssembly::WorkOrder myWorkOrder = ev->Order();
	RobotAssembly::StatusType myStatus = ev->Status();

	ACE_DEBUG ((LM_DEBUG, "WatchSettingManager - In push_PalletStatus event sink \n"));
	if (myStatus == RobotAssembly::palletInPlace) {
		printf("\tStatus = palletInPlace (call was 5e)\n");
		// if (palletInPlace (call was 5e))  6e
		//6e
		// eventtype DisplayWork {
		//		public DisplayRequest Display;
		//		public WorkOrder Order;
		//		public StatusType Status;
		// };
		// 6e publishes DisplayWork Display;
		RobotAssembly::DisplayWork_var displayWork_event = new OBV_RobotAssembly::DisplayWork;
		displayWork_event->Display(RobotAssembly::ReadytoProduce);
		displayWork_event->Order(myWorkOrder);
		displayWork_event->Status(RobotAssembly::accepted);
		this->context_->push_Display(displayWork_event ACE_ENV_ARG_PARAMETER);

	} else if (myStatus == RobotAssembly::complete) {
		printf("\tStatus = complete (call was 11e)\n");
		// else if (complete(call was 11e)) 12e
		// 12e
		// eventtype  ProductionStatus {
		//		public  StatusType Status;
		//		public WorkOrder Order;
		// };
		// 12e publishes ProductionStatus ProductionReport
		RobotAssembly::ProductionStatus_var productionStatus_event = new OBV_RobotAssembly::ProductionStatus;
		productionStatus_event->Order(myWorkOrder);
		productionStatus_event->Status(RobotAssembly::complete);
		this->context_->push_ProductionReport(productionStatus_event ACE_ENV_ARG_PARAMETER);

	} else {
		printf("\tInvalid status value\n");
	}

	/*

	if (myMoveRequestType == RobotAssembly::GetPalletProductToProcess) {
		// Simulate Pallet Ready - this code should be moved to the PCMController once the GUIs are attached

		// 5e
		// publishes PalletStatusResponse PalletStatus;
		RobotAssembly::PalletStatusResponse_var palletStatusResponse_event = new OBV_RobotAssembly::PalletStatusResponse;
		palletStatusResponse_event->Order(myWorkOrder);
		palletStatusResponse_event->Status(RobotAssembly::palletInPlace);
		this->context_->push_PalletStatus(palletStatusResponse_event ACE_ENV_ARG_PARAMETER);

	} else if (myMoveRequestType == RobotAssembly::MovePalletToFinishingArea) {
		// Simulate Pallet Ready - this code should be moved to the PCMController once the GUIs are attached
		// eventtype PalletStatusResponse {
		//		public StatusType Status;
		//		public WorkOrder Order;
		// };
		// 11e
		// publishes PalletStatusResponse PalletStatus;
		RobotAssembly::PalletStatusResponse_var palletStatusResponse_event = new OBV_RobotAssembly::PalletStatusResponse;
		palletStatusResponse_event->Order(myWorkOrder);
		palletStatusResponse_event->Status(RobotAssembly::palletProcessed);
		this->context_->push_PalletStatus(palletStatusResponse_event ACE_ENV_ARG_PARAMETER);

	}
  */
}



// Operations from Components::SessionComponen
void
WatchSettingManager_Impl::WatchSettingManager_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "WatchSettingManager_Impl::WatchSettingManager_exec_i::set_session_context\n"));

  this->context_ =
    WatchSettingManager_Impl::WatchSettingManager_Exec_Context::_narrow (ctx
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
WatchSettingManager_Impl::WatchSettingManager_exec_i::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG,
              "WatchSettingManager_Impl::WatchSettingManager_exec_i::ciao_preactivate\n"));
}

void
WatchSettingManager_Impl::WatchSettingManager_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "WatchSettingManager_Impl::WatchSettingManager_exec_i::ccm_activate\n"));

  // @@ This hack work around a missing feature in CIAO's assembly
  // mechanism where a Softpkg descriptor can specify it's dependency
  // to a valuetype factory and instruct the deployment framework to
  // initialize and register the corresponding valuefactory in the
  // component server.  Here, we are registering the valuefactory
  // explicitly to work around this problem.
  char *argv[1] = { "WatchSettingManager_exec"};
  int argc = sizeof(argv)/sizeof(argv[0]);
  CORBA::ORB_var orb = CORBA::ORB_init (argc,argv, "" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CIAO_REGISTER_VALUE_FACTORY (orb.in(),RobotAssembly::ProductionWorkOrder_init,
                               RobotAssembly::ProductionWorkOrder);
}

void
WatchSettingManager_Impl::WatchSettingManager_exec_i::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG,
              "WatchSettingManager_Impl::WatchSettingManager_exec_i::ciao_postactivate\n"));
}

void
WatchSettingManager_Impl::WatchSettingManager_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "WatchSettingManager_Impl::WatchSettingManager_exec_i::ccm_passivate\n"));
}

void
WatchSettingManager_Impl::WatchSettingManager_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "WatchSettingManager_Impl::WatchSettingManager_exec_i::ccm_remove\n"));
}


WatchSettingManager_Impl::WatchSettingManagerHome_exec_i::WatchSettingManagerHome_exec_i ()
{
}

WatchSettingManager_Impl::WatchSettingManagerHome_exec_i::~WatchSettingManagerHome_exec_i ()
{
}

::Components::EnterpriseComponent_ptr
WatchSettingManager_Impl::WatchSettingManagerHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  Components::EnterpriseComponent_ptr tmp;
  ACE_NEW_THROW_EX (tmp,WatchSettingManager_Impl::WatchSettingManager_exec_i,
                    CORBA::NO_MEMORY ());
  return tmp;
}


extern "C" WATCHSETTINGMANAGER_EXEC_Export ::Components::HomeExecutorBase_ptr
createWatchSettingManagerHome_Impl (void)
{
  return new WatchSettingManager_Impl::WatchSettingManagerHome_exec_i ();
}
