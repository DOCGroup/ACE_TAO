#include "Echo_i.h"

// Set the ORB pointer.
void
Echo_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}

// Return a list of object references.
Echo::List *
Echo_i::echo_list (char const *)
{
  Echo::List_var list;

  {
    Echo::List *tmp = nullptr;
    ACE_NEW_RETURN (tmp,
                    Echo::List (3),
                    nullptr);
    // Pass ownership to the _var, pitty that ACE_NEW_RETURN cannot
    // assign to T_vars directly.
    list = tmp;
  }

  list->length (3);

  // Just do something to get a list of object references.
  list[CORBA::ULong(0)] =
    orb_->resolve_initial_references ("NameService");

  list[CORBA::ULong(1)] =
    orb_->resolve_initial_references ("NameService");

  list[CORBA::ULong(2)] =
    orb_->resolve_initial_references ("NameService");

  return list._retn ();
}

// Return the mesg string from the server

char *
Echo_i::echo_string (char const *mesg)
{
  // The pointer mesg was null, return.
  if (mesg == nullptr)
    return nullptr;

  CORBA::String_var str = CORBA::string_dup (mesg);

  // if <CORBA::string_dup> returns a null pointer, an exception is
  // raised.

  if (str.in () == nullptr)
    throw CORBA::NO_MEMORY ();

  // Got thru! now, make a deep copy of the mesg string and send it
  // back to the client.

  return str._retn ();
  // The _retn is used as it allows the conversion of
  // CORBA::String_var to char* without causing any compiler errors.
}

// Shutdown the server application.

void
Echo_i::shutdown ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nThe echo server is shutting down\n")));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}
