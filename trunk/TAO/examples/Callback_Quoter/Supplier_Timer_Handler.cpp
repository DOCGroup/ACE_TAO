// $Id$
// ===========================================================
//
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//  Supplier_Time _Handler.cpp
//
// = DESCRIPTION
//    Implementation of the Supplier_Time_Handler class.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#include "ace/OS.h"
#include "Supplier_Timer_Handler.h"


ACE_RCSID(Callback_Quoter, Supplier, "$Id$")

  
// The supplier refernce is got so that the mathods in the
// supplier can be accessed.
  Supplier_Timer_Handler:: Supplier_Timer_Handler (Supplier * supplier,
                                                   ACE_Reactor *reactor)
    :supplier_obj_ (supplier),
     reactor_ (reactor)
{
}

//destructor.
 Supplier_Timer_Handler::~Supplier_Timer_Handler (void)
{
  // No-op.
}


// Method which will be called by the reactor on timeout.
int
Supplier_Timer_Handler:: handle_timeout (const ACE_Time_Value &tv,
                                         const void *arg)
{
  long value;
  char stock_name[BUFSIZ];

  value = 10;
  ACE_OS::strcpy(stock_name,"Ernst&Young");

  ACE_DEBUG ((LM_DEBUG,
              "sending data from handle_timeout to notifier!\n"));

  if (this->supplier_obj_->send_market_status (stock_name, value) < 0)
    {
      this->reactor_->end_event_loop ();
    ACE_ERROR_RETURN ((LM_ERROR,
                "handle_timeout: send_market_status failed! %p\n",
                       "send_market_status"),
                      -1);
    }
  return 0;
}

