// $Id$

#include "tao/ORBInitializer_Registry.h"
#include "tao/ORBInitializer_Registry_Adapter.h"
#include "tao/PortableInterceptorC.h"
#include "tao/ORB.h"
#include "tao/ORB_Constants.h"
#include "tao/TAO_Singleton_Manager.h"
#include "tao/SystemException.h"

#include "ace/Service_Config.h"
#include "ace/Dynamic_Service.h"
#include "ace/Static_Object_Lock.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace PortableInterceptor
{
  void
  register_orb_initializer (ORBInitializer_ptr init)
  {
    {
      // Using ACE_Static_Object_Lock::instance() precludes
      // register_orb_initializer() from being called within a static
      // object CTOR.
      ACE_MT (ACE_GUARD (TAO_SYNCH_RECURSIVE_MUTEX,
                         guard,
                         *ACE_Static_Object_Lock::instance ()));

      // Make sure TAO's singleton manager is initialized.
      if (TAO_Singleton_Manager::instance ()->init () == -1)
        {
          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - register_orb_initializer: ")
                      ACE_TEXT ("Unable to pre-initialize TAO\n")));
        }
    }

    // If not, look it up.
    TAO::ORBInitializer_Registry_Adapter *orbinitializer_registry_ =
      ACE_Dynamic_Service<TAO::ORBInitializer_Registry_Adapter>::instance
      ("ORBInitializer_Registry", true); // only look in the local repo

#if !defined (TAO_AS_STATIC_LIBS)
    if (orbinitializer_registry_ == 0)
      {
        ACE_Service_Config::process_directive (
          ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE("ORBInitializer_Registry",
                                        "TAO_PI",
                                        TAO_VERSION,
                                        "_make_ORBInitializer_Registry",
                                        ""));
        orbinitializer_registry_ =
          ACE_Dynamic_Service<TAO::ORBInitializer_Registry_Adapter>::instance
            ("ORBInitializer_Registry");
      }
#endif /* !TAO_AS_STATIC_LIBS */

    if (orbinitializer_registry_ != 0)
      {
        orbinitializer_registry_->register_orb_initializer (init);
      }
    else
      {
        TAOLIB_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) %p\n"),
                    ACE_TEXT ("ERROR: ORBInitializer Registry unable to find the ")
                    ACE_TEXT ("ORBInitializer Registry instance")));

        throw ::CORBA::INTERNAL ();
      }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
