// $Id$
#ifndef APPHELPER_H
#define APPHELPER_H

#include "CSD_TP_Test_Export.h"
#include "TestAppExceptionC.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/ORB.h"
#include "ace/Log_Msg.h"


template <typename T>
struct RefHelper
{
  typedef typename T::_ptr_type T_ptr;
  typedef typename T::_var_type T_var;

  static T_ptr string_to_ref(CORBA::ORB_ptr orb,
                             const ACE_TCHAR* ior)
  {
    CORBA::Object_var obj = orb->string_to_object(ior);

    if (CORBA::is_nil(obj.in()))
      {
        ACE_ERROR((LM_ERROR,
                   "(%P|%t) Failed to convert IOR string to obj ref.\n"));
        throw TestAppException();
      }

    T_var t_obj = T::_narrow(obj.in());

    if (CORBA::is_nil(t_obj.in()))
      {
        ACE_ERROR((LM_ERROR,
                   "(%P|%t) Failed to narrow obj ref to T interface.\n"));
        throw TestAppException();
      }

    return t_obj._retn();
  }

  static T_ptr resolve_initial_ref(CORBA::ORB_ptr orb,
                                   const char* name)
  {
    CORBA::Object_var obj
      = orb->resolve_initial_references(name);

    if (CORBA::is_nil(obj.in()))
      {
        ACE_ERROR((LM_ERROR,
                   "(%P|%t) Failed to resolve initial ref for '%s'.\n",
                   name));
        throw TestAppException();
      }

    T_var t_obj = T::_narrow(obj.in());


    if (CORBA::is_nil(t_obj.in()))
      {
        ACE_ERROR((LM_ERROR,
                   "(%P|%t) Failed to narrow resolved initial ref '%s'.\n",
                   name));
        throw TestAppException();
      }

    return t_obj._retn();
  }

};

struct CSD_TP_Test_Export AppHelper
{

  static void ref_to_file(CORBA::ORB_ptr    orb,
                          CORBA::Object_ptr obj,
                          const ACE_TCHAR*  filename);

  static PortableServer::POA_ptr create_poa
                              (const char*                    name,
                               PortableServer::POA_ptr        root_poa,
                               PortableServer::POAManager_ptr mgr,
                               CORBA::PolicyList&             policies);

  static CORBA::Object_ptr activate_servant(PortableServer::POA_ptr poa,
                                            PortableServer::Servant servant);

  // This helper method is used because there is a chance that the
  // initial CORBA request made to the target ORB will fail during
  // connection establishment with a TRANSIENT CORBA SystemException.
  // This occurs for some platforms (ie, windows) when several clients
  // make their initial CORBA request to the same ORB at the same time,
  // causing the ORB to attempt to handle several connection establishments
  // at one time.  Apparently, under certain conditions, it will throw the
  // TRANSIENT exception to tell the client application to "try again later".
  // The analogy is making a phone call.  Sometimes you get a busy tone.
  // This means "try again later".
  // This helper function will retry until the connection establishment
  // works - or until it decides that enough is enough.
  static bool validate_connection (CORBA::Object_ptr obj);
};

#endif

