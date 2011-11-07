
//=============================================================================
/**
 *  @file    Consumer_i.cpp
 *
 *  $Id$
 *
 *  Implements the Consumer_i class, which is used by the
 *  callback quoter client.
 *
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#include "Consumer_i.h"

Consumer_i::Consumer_i (void)
{
}

Consumer_i::~Consumer_i (void)
{
}

void
Consumer_i::push (const Callback_Quoter::Info &data)
{
  // On getting the needed information you now proceed to the next
  // step, which could be obtaining the shares.

  ACE_DEBUG ((LM_DEBUG,
              "Selling 10,000 %s shares at %d!!\n",
              data.stock_name.in (),
              data.value));
}

void
Consumer_i::shutdown (void)
{

  // Instruct the ORB to shutdown.

  ACE_DEBUG ((LM_DEBUG,
              " consumer shutting down \n"));

  this->orb_->shutdown ();
}

void
Consumer_i::orb (CORBA::ORB_ptr o)
{
  // Makes a copy of the ORB pointer.

  this->orb_ = CORBA::ORB::_duplicate (o);
}
