/* -*- C++ -*- */

// =================================================================
/**
 * @file Current_Loader.cpp
 *
 * $Id$
 *
 * @author Iliyan Jeliazkov <iliyan@ociweb.com>
 *
 */
// =================================================================


#include "ace/Service_Config.h"

#include "tao/ORB_Constants.h"
#include "tao/ORBInitializer_Registry.h"

#include "tao/PI/PI.h"

#include "tao/TransportCurrent/Current_ORBInitializer.h"
#include "tao/TransportCurrent/Current_Loader.h"
#include "tao/TransportCurrent/Current_Impl.h"

ACE_RCSID (Transport_Current,
           Current_Loader,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Transport
  {

    /// A helper method for obtaining access to the the Transport
    /// Factory Manager instance.

    Current_Loader*
    Current_Loader::instance (void)
    {
      Current_Loader *cl =
        ACE_Dynamic_Service <Current_Loader>::instance (ACE_TEXT ("TAO_Transport_Current_Loader"));

      if (cl == 0)
#if defined (TAO_AS_STATIC_LIBS)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%P|%t) Unable to obtain Current::Loader instance: %m\n")),
                              0);
          return 0;
        }
#else
      {
        // In case we build shared, try to load the appropriate library.
        ACE_Service_Config::process_directive
          (ACE_DYNAMIC_SERVICE_DIRECTIVE("TAO_Transport_Current_Loader",
                                         "TAO_TC",
                                         "_make_TAO_Transport_Current_Loader",
                                         ""));
        cl = ACE_Dynamic_Service<Current_Loader>::instance (ACE_TEXT ("TAO_Transport_Current_Loader"));
      }
#endif /* TAO_AS_STATIC_LIBS */

      if (cl != 0)
        return cl;

      if (TAO_debug_level > 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Unable to obtain Current::Loader instance: %m\n")),
                          0);
      return 0;
    }


    /// dtor

    Current_Loader::~Current_Loader (void)
    {
    }



    /// Initializes object when dynamic linking occurs.

    int
    Current_Loader::init (int, ACE_TCHAR *[])
    {
      PortableInterceptor::ORBInitializer_ptr tmp = 0;
      ACE_NEW_THROW_EX (tmp,
                        Current_ORBInitializer<Current_Impl>
                          (ACE_TEXT ("TAO::Transport::Current")),
                        CORBA::NO_MEMORY
                          (CORBA::SystemException::_tao_minor_code (TAO::VMCID,
                                                                    ENOMEM),
                           CORBA::COMPLETED_NO));
      ACE_TRY_CHECK;

      PortableInterceptor::ORBInitializer_var initializer (tmp);

      PortableInterceptor::register_orb_initializer (initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      return 0;
    }

  } /* namespace Transport */

} /* namespace TAO */

TAO_END_VERSIONED_NAMESPACE_DECL





#if defined (TAO_AS_STATIC_LIBS)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_REQUIRE (TAO_Transport_Current_Loader);

namespace TAO
{
  namespace Transport
  {
    int current_static_initializer (void)
    {
      ACE_STATIC_SVC_REGISTER (TAO_Transport_Current_Loader);
      return 0;
    }

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* defined (TAO_AS_STATIC_LIBS) */




ACE_STATIC_SVC_DEFINE (TAO_Transport_Current_Loader,
                       ACE_TEXT ("TAO_Transport_Current_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Transport_Current_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_NAMESPACE_DEFINE (TAO_Transport_Current,
                              TAO_Transport_Current_Loader,
                              TAO::Transport::Current_Loader)
