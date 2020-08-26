#include <string>
#include "Echo_i.h"

// Constructor.

Echo_i::Echo_i (CORBA::ORB_ptr o)
   : orb_(o)
{
}

// Destructor.

Echo_i::~Echo_i ()
{
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
    list = tmp;
  }

  list->length (2);

  // Just do something to get a 'big' list of strings.
  std::string big(4 * 1024, 'A');
  std::string small("Hello World");
  list[CORBA::ULong(0)] = CORBA::string_dup(big.c_str());
  list[CORBA::ULong(1)] = CORBA::string_dup(small.c_str());

  return list._retn ();
}

Echo::WList *
Echo_i::return_wlist ()
{
  Echo::WList_var list;

  {
    Echo::WList *tmp = 0;
    ACE_NEW_RETURN (tmp,
                    Echo::WList (2),
                    0);
    list = tmp;
  }

  list->length (2);

  // Just do something to get a 'big' list of wide strings.
  std::wstring big(4 * 1024, 'A');
  std::wstring small(17, 'B');
  list[CORBA::ULong(0)] = CORBA::wstring_dup(big.c_str());
  list[CORBA::ULong(1)] = CORBA::wstring_dup(small.c_str());

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
