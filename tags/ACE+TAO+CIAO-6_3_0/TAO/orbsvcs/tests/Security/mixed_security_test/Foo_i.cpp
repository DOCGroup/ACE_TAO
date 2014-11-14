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
  if (this->current_.in() == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "FOO (%P|%t) No ClientCredentials available\n"));
      return;
    }

  try
    {
      SecurityLevel3::ClientCredentials_var credentials =
        this->current_->client_credentials ();

      CORBA::String_var id = credentials->creds_id ();

      ACE_DEBUG ((LM_DEBUG,
                  "FOO (%P|%t) ClientCredentials ID: %s\n", id.in ()));
      return;
    }
  catch (const CORBA::BAD_INV_ORDER &ex)
    {
      ACE_DEBUG ((LM_INFO,
                  "FOO (%P|%t) Caught BAD_INV_ORDER exception trying to obtain "
                  "client credentials.  This is okay if the invocation was via "
                  "non-secured means.\n"));
      ex._tao_print_exception ("obtaining client credentials");
      // should we re-throw? No, because what we're testing is whether
      // we go into this method, not whether the method itself operates
      // properly.
    }
  catch (CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_ERROR, "FOO (%P|%t) Caught exception, trying to obtain ClientCredentials"));
      ex._tao_print_exception ("Exception");
    }

}

void
Foo_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
