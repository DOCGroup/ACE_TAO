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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var rp =
        PortableServer::POA::_narrow(poa_object.in());
      if  (CORBA::is_nil (rp.in()))
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) panic: nil root poa\n"), 1);

      bogus* bogus_impl = new bogus();
      PortableServer::ServantBase_var owner_transfer(bogus_impl);
      PortableServer::ObjectId_var id =
        rp->activate_object (bogus_impl);

      CORBA::Object_var object = rp->id_to_reference (id.in ());

      Test::bogus_var b = Test::bogus::_narrow (object.in ());
      CORBA::String_var ior =
        orb->object_to_string (b.in());

      ACE_OS::printf ("%s\n", ior.in());

      orb->shutdown();
      orb->destroy();
    }
  catch (const CORBA::Exception& e)
    {
      e._tao_print_exception ("Caught exception:");
    }

  return 0;
}
