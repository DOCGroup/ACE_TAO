//
// This initializes an ORB, a POA, an Object within that POA, and
// obtains and prints an IOR for that Object.
//
// $Id$

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"

#include "bogus_i.h"

ACE_RCSID (IOR_Endpoint_Hostnames, generate_ior, "$Id$")

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);

      PortableServer::POA_var rp =
        PortableServer::POA::_narrow(poa_object.in() ACE_ENV_ARG_PARAMETER);
      if  (CORBA::is_nil (rp.in()))
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) panic: nil root poa\n"), 1);

      bogus* bogus_impl = new bogus();
      PortableServer::ServantBase_var owner_transfer(bogus_impl);
      Test::bogus_var b = bogus_impl->_this();
      CORBA::String_var ior =
        orb->object_to_string (b.in() ACE_ENV_ARG_PARAMETER);

      printf ("%s\n", ior.in());

      orb->shutdown();
      orb->destroy();
    }
  ACE_CATCH (CORBA::Exception, e)
    {
      ACE_PRINT_EXCEPTION (e, "Caught exception:");
    }
  ACE_ENDTRY;

  return 0;
}
