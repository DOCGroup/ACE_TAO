// $Id$

#include "Echo_i.h"

ACE_RCSID(Echo, Echo_i, "$Id$")

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
Echo_i::echo_list (const char *message,
                   CORBA::Environment &)
{
  Echo::List *list;
  
  ACE_NEW_RETURN (list,
                  Echo::List (3),
                  0);

  list->length (3);

  // Just do something to get a list of object references.
  (*list)[0] = orb_->resolve_initial_references ("NameService");
  (*list)[1] = orb_->resolve_initial_references ("NameService");;
  (*list)[2] = orb_->resolve_initial_references ("NameService");;

  return list;
}

// Return the mesg string from the server

char *
Echo_i::echo_string (const char *mesg,
                     CORBA::Environment &ACE_TRY_ENV)
{
  // The pointer mesg was NULL, return.
  if (mesg == 0)
    return 0;

  CORBA::String_var str = CORBA::string_dup (mesg);

  // if <CORBA::string_dup> returns a 0 pointer, an exception is
  // raised.

  if (str.in () == 0)
    ACE_THROW_RETURN (CORBA::NO_MEMORY (), 0);

  // Got thru! now, make a deep copy of the mesg string and send it
  // back to the client.

  return str._retn ();
  // The _retn is used as it allows the conversion of
  // CORBA::String_var to char* without causing any compiler errors.
}

// Shutdown the server application.

void 
Echo_i::shutdown (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n%s\n",
              "The echo server is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}
