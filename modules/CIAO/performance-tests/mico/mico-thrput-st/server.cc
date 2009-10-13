//
//$Id$
//
// FUZZ: disable check_for_missing_rir_env
#include <CORBA.h>
#include <coss/CosNaming.h>

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS

#include "RoundTrip_impl.h"
int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

    //Obtain reference to Naming service
    CORBA::Object_var naming_context_object =
      orb->resolve_initial_references ("NameService");
    CosNaming::NamingContext_var naming_context =
      CosNaming::NamingContext::_narrow (naming_context_object.in ());

    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (obj);
    RoundTrip_impl* rt_impl = new RoundTrip_impl();
    PortableServer::ObjectId_var oid = poa->activate_object (rt_impl);
    Roundtrip_var rt_obj = rt_impl->_this();

    //Add to Naming Service
    CosNaming::Name name (1);
    name.length (1);
    name[0].id = CORBA::string_dup ("RoundTrip");
    naming_context->bind (name, rt_obj.in());

    //allow incoming requests
    poa->the_POAManager()->activate();

    orb->run ();
    return 0;
}
