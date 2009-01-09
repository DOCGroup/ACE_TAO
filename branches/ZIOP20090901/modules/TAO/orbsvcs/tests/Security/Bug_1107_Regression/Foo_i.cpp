// -*- C++ -*-

#include "Foo_i.h"

#include "ace/OS_NS_string.h"


ACE_RCSID (Bug_1107_Regression,
           Foo_i,
           "$Id$")


Foo_i::Foo_i (CORBA::ORB_ptr orb,
              SecurityLevel3::SecurityCurrent_ptr current)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    current_ (SecurityLevel3::SecurityCurrent::_duplicate (current))
{
}

void
Foo_i::baz (void)
{
  // Make sure that logging will occur.
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);

  SecurityLevel3::ClientCredentials_var credentials =
    this->current_->client_credentials ();

  CORBA::String_var id = credentials->creds_id ();

  ACE_DEBUG ((LM_DEBUG,
              "\n"
              "ClientCredentials ID\n"
              "====================\n"
              "%s\n",
              id.in ()));
}

void
Foo_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
