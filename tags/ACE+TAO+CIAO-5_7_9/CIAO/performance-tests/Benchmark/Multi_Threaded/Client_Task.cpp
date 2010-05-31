// $Id$

#include "Client_Task.h"

Client_Task::Client_Task (Benchmark::RoundTripClient_ptr roundtrip)
  : roundtrip_(Benchmark::RoundTripClient::_duplicate(roundtrip))
{
}

int
Client_Task::svc (void)
{
  // Create the Controller
  CORBA::Object_var obj = this->roundtrip_->provide_facet ("controller");
  Benchmark::Controller_var controller = Benchmark::Controller::_narrow (obj.in ());

  // Check if controller is non nil
  if (CORBA::is_nil (controller.in ()))
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "Nil Benchmark::Controller reference\n"),
                        1);

  ACE_DEBUG ((LM_DEBUG, "Sending Trigger\n"));

  // Send out the trigger message
  controller->start();

  return 0;
}
