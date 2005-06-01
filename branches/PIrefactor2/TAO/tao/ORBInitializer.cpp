#include "ORBInitializer.h"
#include "PortableInterceptorC.h"
#include "ORB.h"
#include "ORB_Constants.h"
#include "TAO_Singleton_Manager.h"
#include "SystemException.h"

#include "ace/Static_Object_Lock.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Log_Msg.h"

ACE_RCSID (tao,
           ORBInitializer_Registry,
           "$Id$")


// ****************************************************************

void
PortableInterceptor::register_orb_initializer (
  PortableInterceptor::ORBInitializer_ptr init
  ACE_ENV_ARG_DECL)
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
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) register_orb_initializer: ")
                    ACE_TEXT ("Unable to pre-initialize TAO\n")));
      }

    TAO::ORB::init_orb_globals (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }

  // Make sure the following is done after the global ORB
  // initialization since we need to have exceptions initialized.
    // @todo
  // use the adapter
//  TAO::ORBInitializer_Registry::instance ()->register_orb_initializer (
 ///   init
 //   ACE_ENV_ARG_PARAMETER);
}


