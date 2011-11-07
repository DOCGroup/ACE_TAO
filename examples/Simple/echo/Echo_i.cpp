// $Id$

#include "Echo_i.h"

// Constructor.

Echo_i::Echo_i (void)
{
}

// Destructor.

Echo_i::~Echo_i (void)
{
}

// Set the ORB pointer.

void
Echo_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}

// Return a list of object references.

Echo::List *
Echo_i::echo_list (const char *)
{
  Echo::List_var list;

  {
    Echo::List *tmp = 0;
    ACE_NEW_RETURN (tmp,
                    Echo::List (3),
                    0);
    // Pass ownership to the _var, pitty that ACE_NEW_RETURN cannot
    // assign to T_vars directly.
    list = tmp;
  }

  list->length (3);

  // Just do something to get a list of object references.
  list[CORBA::ULong(0)] =
    orb_->resolve_initial_references ("NameService");

  list[CORBA::ULong(1)] =
    orb_->resolve_initial_references ("NameService");;

  list[CORBA::ULong(2)] =
    orb_->resolve_initial_references ("NameService");

  return list._retn ();
}

// Return the mesg string from the server

char *
Echo_i::echo_string (const char *mesg)
{
  // The pointer mesg was NULL, return.
  if (mesg == 0)
    return 0;

  CORBA::String_var str = CORBA::string_dup (mesg);

  // if <CORBA::string_dup> returns a 0 pointer, an exception is
  // raised.

  if (str.in () == 0)
    throw CORBA::NO_MEMORY ();

  // Got thru! now, make a deep copy of the mesg string and send it
  // back to the client.

  return str._retn ();
  // The _retn is used as it allows the conversion of
  // CORBA::String_var to char* without causing any compiler errors.
}

// Shutdown the server application.

void
Echo_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nThe echo server is shutting down\n")));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}
