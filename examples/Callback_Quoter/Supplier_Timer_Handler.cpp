//=============================================================================
/**
 *  Implementation of the Supplier_Time_Handler class.
 *
 *  $Id$
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#include "ace/ACE.h"

#include "Supplier_Timer_Handler.h"

// The supplier refernce is got so that the mathods in the supplier
// can be accessed.

Supplier_Timer_Handler:: Supplier_Timer_Handler (Supplier *supplier,
                                                 ACE_Reactor *reactor,
                                                 FILE *file_ptr)
    :supplier_obj_ (supplier),
     reactor_ (reactor),
     file_ptr_ (file_ptr)
{
  // No-op.
}

// Destructor.

 Supplier_Timer_Handler::~Supplier_Timer_Handler (void)
{
  // No-op.
}

// Method which will be called by the reactor on timeout.

int
Supplier_Timer_Handler:: handle_timeout (const ACE_Time_Value & /* tv */,
                                         const void * /* arg */)
{

  ACE_DEBUG ((LM_DEBUG,
              "Sending Stock Market Information to Notifier...\n"));

  // The next current stock rates are obtained from a file.
  if (this->get_stock_information () == -1)
    return 0;


  // Send the stock information to the notifier.  Graceful exit when
  // the notifier doesnt accept the information.
  if (this->supplier_obj_->send_market_status (stockname_,
                                               value_) < 0)
    {
      this->reactor_->end_event_loop ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "handle_timeout: send_market_status failed! %p\n",
                         "send_market_status"),
                        -1);
    }

  return 0;
}

// Get the stock information from a file.

int
Supplier_Timer_Handler::get_stock_information (void)
{
  // Scan the file and obtain the stock information.
  if (fscanf (file_ptr_,
              "%s %ld\n",
              stockname_,
              &value_) != EOF)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Stockname: %s, Stockvalue: %d\n",
                  stockname_,
                  value_));
      return 0;
    }
  else
    {
      // Close down the Reactor.
      this->reactor_->end_event_loop ();
      return -1;
    }
}

