#include "Echo_i.h"

constexpr size_t BIG_LENGTH = 4 * 1024;

// Constructor.

Echo_i::Echo_i (CORBA::ORB_ptr o)
   : orb_(o)
{
}

// Return a list of strings.
Echo::List *
Echo_i::return_list ()
{
  Echo::List_var list;

  {
    Echo::List *tmp {};
    ACE_NEW_RETURN (tmp,
                    Echo::List (2),
                    {});
    list = tmp;
  }

  list->length (2);

  // Just do something to get a 'big' list of strings.
  CORBA::Char big[BIG_LENGTH + 1];
  for (size_t i = 0; i < BIG_LENGTH; ++i)
     big[i] = 'A';
  big[BIG_LENGTH] = 0;
  list[CORBA::ULong(0)] = CORBA::string_dup(big);
  list[CORBA::ULong(1)] = CORBA::string_dup("Hello World");

  return list._retn ();
}

Echo::WList *
Echo_i::return_wlist ()
{
  Echo::WList_var list;

  {
    Echo::WList *tmp {};
    ACE_NEW_RETURN (tmp,
                    Echo::WList (2),
                    {});
    list = tmp;
  }

  list->length (2);

  // Just do something to get a 'big' list of wide strings.
  CORBA::WChar big[BIG_LENGTH + 1];
  for (size_t i = 0; i < BIG_LENGTH; ++i)
     big[i] = 'A';
  big[BIG_LENGTH] = 0;
  CORBA::WChar small[17 + 1];
  for (size_t i = 0; i < 17; ++i)
     small[i] = 'B';
  small[17] = 0;
  list[CORBA::ULong(0)] = CORBA::wstring_dup(big);
  list[CORBA::ULong(1)] = CORBA::wstring_dup(small);

  return list._retn ();
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
