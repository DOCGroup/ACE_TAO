// $Id$

// ===========================================================
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Consumer_i.cpp
//
// = DESCRIPTION
//    Implements the Consumer_i class, which is used by the
//    callback quoter client.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#include "Consumer_i.h"

Consumer_i::Consumer_i (void)
{
}

Consumer_i::~Consumer_i (void)
{
}

void
Consumer_i::push (const Callback_Quoter::Info &data,
                  CORBA::Environment &)
{
  // On getting the needed information you now proceed to the next
  // step, which could be obtaining the shares.

  ACE_DEBUG ((LM_DEBUG,
              "Selling 10,000 %s shares at %d!!\n",
	            data.stock_name.in (),
              data.value));
}

void
Consumer_i::shutdown (CORBA::Environment &)
{

  // Instruct the ORB to shutdown.

  ACE_DEBUG ((LM_DEBUG,
              " consumer shutting down \n "));

  this->orb_->shutdown ();
}

void
Consumer_i::orb (CORBA::ORB_ptr o)
{
  // Makes a copy of the ORB pointer.

  this->orb_ = CORBA::ORB::_duplicate (o);
}
