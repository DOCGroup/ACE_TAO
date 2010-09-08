// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "TestS.h"
#include "Foo_i.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
static int n = 10;
int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        n = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (parse_args (argc, argv) != 0)
        return 1;

      Foo_i *foo_i;
      ACE_NEW_RETURN (foo_i,
                      Foo_i (orb.in ()),
                      1);

      PortableServer::ServantBase_var owner_transfer(foo_i);

      Test::Foo_var foo =
        foo_i->_this ();

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Memory_Growth_var mem =
        Test::Memory_Growth::_narrow(tmp.in ());

      if (CORBA::is_nil (mem.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      // Make a sequence..
      Test::PayLoad pl (50000);
      pl.length (50000);

      for (CORBA::ULong l = 0;
           l != pl.length ();
           l++)
        {
          pl[l] = foo;
        }

      // Make a few calls to the remote object
      for (int iter = 0; iter != n; iter++)
        {
          mem->send_objref (pl);
        }

      // Let us run the event loop. This way we will not exit
      orb->run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
