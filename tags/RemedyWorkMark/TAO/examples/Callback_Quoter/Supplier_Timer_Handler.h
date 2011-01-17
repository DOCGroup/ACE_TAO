/* -*- C++ -*- */
// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Consumer_Input_Handler.h
//
// = DESCRIPTION
//    Definition of the Supplier_Timer_Handler class.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#ifndef SUPPLIER_TIMER_HANDLER_H
#define SUPPLIER_TIMER_HANDLER_H
#include "ace/Reactor.h"
#include "ace/Timer_Queue.h"
#include "ace/Event_Handler.h"
#include "Supplier_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Supplier;

class Supplier_Timer_Handler : public ACE_Event_Handler
{
  // = TITLE
  //    Feeds stock information to the Callback Quoter notifier
  //    periodically.
  //
  // = Description
  //   Create a class to handle timer events.  Since only timer events
  // need to be handled, only the handle_timeout method is overlaoded.
public:
  Supplier_Timer_Handler (Supplier *supplier,
                          ACE_Reactor *reactor,
                          FILE *file_ptr);
  // Initilization.

  ~Supplier_Timer_Handler (void);
  // Destructor.

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);
  // Method which will be called by the Reactor when timeout occurs.

private:

  int get_stock_information (void);
  // The values of the stock and its rate are got from the file.

  Supplier *supplier_obj_;
  // The supplier instance.

  ACE_Reactor *reactor_;
  // Reactor used by the supplier.

  FILE* file_ptr_;
  // The file handle of the file from where the stock input is obtained.

  char stockname_[BUFSIZ];
  // The name of the stock.

  long value_;
  // The market value of the stock.It will be typecasted to long later.
};

#endif /* SUPPLIER_TIMER_HANDLER_H */
