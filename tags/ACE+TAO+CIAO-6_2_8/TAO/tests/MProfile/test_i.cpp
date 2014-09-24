// $Id$

#include "test_i.h"

Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    val_ (1)
{
}

Simple_Server_i::Simple_Server_i (void)
  : orb_ (0)
{
  // no-op
}

CORBA::Long
Simple_Server_i::remote_call (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Print out from process id (%P) hosting the servant\n"));

  return this->val_++;
}


void
Simple_Server_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
