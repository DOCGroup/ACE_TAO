// This is a simple test of an ImR using the corba interfaces
// Start with -orbinitref Test=...

#include "testC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

ACE_CString server = "TestObject_0";

int parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("s:m:"));
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 's':
          server = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage:  %s"
                             " [-s <servername>]"
                             "\n",
                             argv[0]),1);
        }
    }

  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
      if (parse_args (argc, argv) != 0)
        return -1;

      CORBA::Object_var nsobj = orb->resolve_initial_references ("NameService");
      CosNaming::NamingContext_var nc =
        CosNaming::NamingContext::_narrow (nsobj.in ());
      CosNaming::Name name(1);
      name.length (1);
      name[0].id = CORBA::string_dup (server.c_str ());

      try
        {
          nsobj = nc->resolve (name);
        }
      catch (CosNaming::NamingContext::NotFound &)
        {
          return 1;
        }

      test_var test = test::_narrow(nsobj.in());
      ACE_ASSERT(! is_nil(test.in()));

      CORBA::Long n = test->get();
      CORBA::Long m = test->get();
      if (m == n + 1)
        return 0;
      return 1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client:");
    }
  return -1;
}
