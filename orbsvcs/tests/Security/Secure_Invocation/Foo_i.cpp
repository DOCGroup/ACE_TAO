// -*- C++ -*-
// $Id$

#include "Foo_i.h"
#include "ace/OS_NS_string.h"

Foo_i::Foo_i (CORBA::ORB_ptr orb,
              SecurityLevel3::SecurityCurrent_ptr current)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    current_ (SecurityLevel3::SecurityCurrent::_duplicate (current))
{
}

void
Foo_i::baz (void)
{
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
