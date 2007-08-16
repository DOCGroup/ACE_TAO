// $Id$

#include "fooC.h"
#include "ace/Log_Msg.h"

const char* ior = "file://server.ior";

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var tmp = orb->string_to_object(ior);

      foo_var server = foo::_narrow(tmp.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil foo reference <%s>\n",
                             ior),
                            1);
        }

      server->destroy ();

      server->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CORBA::Exception");
    }

  return 0;
}
