// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
#include "InvokeMeC.h"

namespace
{
  class always
  {
  public:
    explicit always (InvokeMe_ptr server) : server_ (server) {}

    always (always const &) = delete;
    always (always &&) = delete;
    always & operator = (always const &) = delete;
    always & operator = (always &&) = delete;

    ~always ()
    {
      ACE_DEBUG ((LM_DEBUG, "\n"));
      server_->shutdown ();
    }

  private:
    // Borrowed from the client-side var; Shutdown is destroyed before it.
    InvokeMe_ptr const server_;
  };
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (argc != 2)
        {
          ACE_DEBUG ((LM_ERROR, "Usage: %s <ior>\n", argv[0]));
          return -1;
        }

      CORBA::Object_var const obj = orb->string_to_object (argv[1]);
      InvokeMe_var const server = InvokeMe::_narrow (obj.in ());

      always const Shutdown (server.in ()); // When goes out of scope!
      server->invoke_me ();
      ACE_DEBUG ((LM_DEBUG, "\n"));
      server->invoke_me ();
    }
  catch (::CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("ERROR : unexpected CORBA exception caught : ");
      return 1;
    }

  return 0;
}
