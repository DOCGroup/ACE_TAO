#include <string>
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

// Return a list of strings.

Echo::List *
Echo_i::return_list ()
{
  Echo::List_var list;

  {
    Echo::List *tmp = 0;
    ACE_NEW_RETURN (tmp,
                    Echo::List (2),
                    0);
    // Pass ownership to the _var, pitty that ACE_NEW_RETURN cannot
    // assign to T_vars directly.
    list = tmp;
  }

  list->length (2);

  // Just do something to get a 'big' list of strings.
  std::string big(4 * 1024 * 1024, 'A');
  std::string small("Hello World");
  list[CORBA::ULong(0)] = CORBA::string_dup(big.c_str());
  list[CORBA::ULong(1)] = CORBA::string_dup(small.c_str());

  return list._retn ();
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
