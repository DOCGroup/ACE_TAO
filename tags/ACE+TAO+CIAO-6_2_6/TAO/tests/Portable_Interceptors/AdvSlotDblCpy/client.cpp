// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "InvokeMeC.h"

namespace
{
  class always
  {
    const InvokeMe_ptr server_;
  public:
    always(const InvokeMe_ptr x):server_(x) {}
    ~always()
    {
      ACE_DEBUG ((LM_DEBUG, "\n"));
      server_->shutdown ();
    }
  };
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (argc != 2)
        {
          ACE_DEBUG ((LM_ERROR, "Usage: %s <ior>\n", argv[0]));
          return -1;
        }

      CORBA::Object_var obj = orb->string_to_object (argv[1]);
      InvokeMe_var server = InvokeMe::_narrow (obj.in ());

      always Shutdown( server.in () ); // When goes out of scope!
      server->invoke_me ();
      ACE_DEBUG ((LM_DEBUG, "\n"));
      server->invoke_me ();
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("ERROR : unexpected CORBA exception caught : ");
      return 1;
    }

  return 0;
}
