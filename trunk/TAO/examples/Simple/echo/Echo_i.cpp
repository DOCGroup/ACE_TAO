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

void Echo_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}

// Return the mesg string from the server

char *
Echo_i::echo (const char *mesg,
	      CORBA::Environment &env)
{
  // @@ Once you're done with getting your program to compile and run,
  // I want you to use Purify on your program to find out where the
  // memory management problems are.

  // @@ Please read the ACE-guidelines.html and follow the programming
  // style.

  if (mesg == 0)
    return 0;
    // The pointer mesg was NULL, return.

  // @@ If you raise an exception for whatever reason, you need to
  // return 0.

  CORBA::String_var str = CORBA::string_dup (mesg);

  // if CORBA::string_dup() returns a 0 pointer, an exception is
  // raised.

  if (str == 0)
    env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));

  // @@ Make sure that you test out an version of this test where you
  // intentionally raise an exception to make sure that your client
  // handles it properly.

  // @@ You need to take a look at how to handle memory failures,
  // i.e., when CORBA::string_dup() returns a 0 pointer.  In this
  // case, you'll need to transform this into an exception and raise
  // the exception.  I recommend that you check out
  // $TAO_ROOT/orbsvcs/orbsvcs/Log/Logger_i.cpp and see how it is
  // handled there.

  // Got thru! now, make a deep copy of the mesg string and send it
  // back to the client.

  return str._retn ();
  // The _retn is used as it allows the conversion of
  // CORBA::String_var to char* without causing any compiler errors.
}

// Shutdown the server application.

void Echo_i::shutdown (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n%s\n",
              "The echo server is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}
