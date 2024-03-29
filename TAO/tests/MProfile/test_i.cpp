#include "test_i.h"

Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    val_ (1)
{
}

Simple_Server_i::Simple_Server_i ()
  : orb_ (0)
{
}

CORBA::Long
Simple_Server_i::remote_call ()
{
  ACE_DEBUG ((LM_DEBUG,
              "Print out from process id (%P) hosting the servant\n"));

  return this->val_++;
}


void
Simple_Server_i::shutdown ()
{
  this->orb_->shutdown (false);
}
