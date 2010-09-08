// $Id$

#include "Hello.h"
#include "ace/OS_NS_stdio.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int status = 1;
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      Hello *hello_impl = 0;
      ACE_NEW_RETURN (hello_impl,
                      Hello (),
                      1);
      PortableServer::ServantBase_var owner_transfer(hello_impl);

      Test::Hello_var hello = hello_impl->_this ();

      ACE_ERROR ((LM_ERROR,
                  "ERROR: Implicit activation should have "
                  "thrown an exception with CORBA e compact.\n"));

      root_poa->destroy (1, 1);
      orb->destroy ();
    }
  catch (const PortableServer::POA::WrongPolicy&)
    {
      ACE_DEBUG ((LM_DEBUG, "This test passes.\n"));
      status = 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }

  return status;
}
