// $Id$

#include "test_i.h"

Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
  // no-op
}

void
Simple_Server_i::remote_call ()
{
  ACE_DEBUG ((LM_DEBUG,
              "Print out from process id (%P) hosting the servant\n"));
}

void
Simple_Server_i::shutdown ()
{
  ACE_DEBUG ((LM_DEBUG,
              "Servant in process id (%P) received shutdown request. Complying...\n"));
  this->orb_->shutdown (0);
}
