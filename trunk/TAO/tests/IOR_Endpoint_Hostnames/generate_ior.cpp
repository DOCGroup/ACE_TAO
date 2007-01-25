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
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "");

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var rp =
        PortableServer::POA::_narrow(poa_object.in());
      if  (CORBA::is_nil (rp.in()))
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) panic: nil root poa\n"), 1);

      bogus* bogus_impl = new bogus();
      PortableServer::ServantBase_var owner_transfer(bogus_impl);
      Test::bogus_var b = bogus_impl->_this();
      CORBA::String_var ior =
        orb->object_to_string (b.in());

      printf ("%s\n", ior.in());

      orb->shutdown();
      orb->destroy();
    }
  catch (const CORBA::Exception& e)
    {
      e._tao_print_exception ("Caught exception:");
    }

  return 0;
}
