// $Id$

#include "tao/ORB_Core.h"
#include "tao/PortableServer/PortableServer.h"
#include "fooS.h"

namespace
{
class Foo_i : public virtual POA_foo
{
public:
  virtual void check ()
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) checking\n"));
  }
};

int vc_check(foo_ptr ff, bool active=true)
{
  try
  {
    CORBA::PolicyList_var policies;
    CORBA::Boolean rv=ff->_validate_connection (policies.out());
    if (!rv)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) got false from _validate_connection\n"),
                          1);
    }
  }
  catch (const CORBA::INV_POLICY&)
  {
    if (!active)
      ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) unexpect inlaid policies\n"),
                              2);
  }
  catch (const CORBA::OBJECT_NOT_EXIST&)
  {
    if (active)
      ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) unexpect object not exists\n"),
                              3);
  }
  catch (const CORBA::Exception&)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) unexpect exception\n"),
                          4);
  }
  return 0;
}
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb;
  try
  {
    orb = CORBA::ORB_init (argc, argv);

    CORBA::Object_var root_poa_o =
      orb->resolve_initial_references ("RootPOA");

    PortableServer::POA_var rootPOA =
      PortableServer::POA::_narrow (root_poa_o.in ());

    if (CORBA::is_nil (rootPOA.in ()))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
        " (%P|%t) Panic: nil RootPOA\n"), 1);
      }

    PortableServer::POAManager_var poaMgr = rootPOA->the_POAManager ();

    poaMgr->activate ();

    CORBA::PolicyList policies;
    policies.length (3);
    policies[0] = rootPOA->create_id_assignment_policy (
      PortableServer::SYSTEM_ID);
    policies[1] = rootPOA->create_implicit_activation_policy (
      PortableServer::NO_IMPLICIT_ACTIVATION);
    policies[2] = rootPOA->create_lifespan_policy (
      PortableServer::TRANSIENT);

    PortableServer::POA_var fooPoa = rootPOA->create_POA (
      "FOO_POA", poaMgr.in (), policies );

    for (CORBA::ULong i = 0; i < policies.length (); ++i)
    {
      policies[i]->destroy ();
    }

    Foo_i servant;
    PortableServer::ObjectId_var oid = fooPoa->activate_object( &servant );

    CORBA::Object_var obj = fooPoa->id_to_reference (oid.in ());

    foo_var client = foo::_narrow (obj.in());

    client->check();

    if (vc_check(client.in()))
    {
      orb->destroy();
      return 1;
    }

    fooPoa->deactivate_object (oid.in () );  //servant is gone

    if (vc_check(client.in(), false))  //exception expected
    {
      orb->destroy();
      return 2;
    }
  }
  catch(...)
  {
    return 3;
  }
  return 0;
}
