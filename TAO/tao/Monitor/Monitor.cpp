// -*- C++ -*-
// $Id$

#include "tao/Monitor/Monitor.h"
#include "tao/Monitor/Monitor_Impl.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

TAO_Monitor_Init::TAO_Monitor_Init (void)
{
}

CORBA::Object_ptr
TAO_Monitor_Init::create_object (CORBA::ORB_ptr orb,
                                 int,
                                 ACE_TCHAR *[])
{
  try
    {
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var mgr =
        poa->the_POAManager ();

      mgr->activate ();

      Monitor_Impl *servant = 0;
      ACE_NEW_RETURN (servant,
                      Monitor_Impl (orb),
                      CORBA::Object::_nil ());

      PortableServer::ObjectId_var id =
        poa->activate_object (servant);

      PortableServer::ServantBase_var safe_servant = servant;
      obj = servant->_this ();

      return obj._retn ();
    }
  catch (const CORBA::Exception&)
    {
    }
  
  return CORBA::Object::_nil ();
}

// This initializer is very similar to the ORBInitializer_Registry::init.
// This one is used during static initialization while the other is used
// when Monitor is a shared library.
int
TAO_Monitor_Init::Initializer (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_Monitor_Init);
}


ACE_STATIC_SVC_DEFINE (TAO_Monitor_Init,
                       ACE_TEXT ("Monitor_Init"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Monitor_Init),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_Monitor, TAO_Monitor_Init)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

