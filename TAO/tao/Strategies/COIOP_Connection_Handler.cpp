// $Id$

#include "tao/Strategies/COIOP_Connection_Handler.h"

#if defined (TAO_HAS_COIOP) && (TAO_HAS_COIOP != 0)

#include "tao/Timeprobe.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/CDR.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Resume_Handle.h"

#include "tao/Strategies/COIOP_Transport.h"
#include "tao/Strategies/COIOP_Endpoint.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_COIOP_Connection_Handler::TAO_COIOP_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_COIOP_SVC_HANDLER (t, 0 , 0),
    TAO_Connection_Handler (0)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (0);
}


TAO_COIOP_Connection_Handler::TAO_COIOP_Connection_Handler (TAO_ORB_Core *orb_core)
  : TAO_COIOP_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core)
{
  TAO_COIOP_Transport* specific_transport = 0;
  ACE_NEW (specific_transport,
           TAO_COIOP_Transport(this, orb_core));

  // store this pointer (indirectly increment ref count)
  this->transport (specific_transport);
}


TAO_COIOP_Connection_Handler::~TAO_COIOP_Connection_Handler (void)
{
  delete this->transport ();

  int const result =
    this->release_os_resources ();

  if (result == -1 && TAO_debug_level)
    {
      TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT("TAO (%P|%t) - COIOP_Connection_Handler::")
                  ACE_TEXT("~COIOP_Connection_Handler, ")
                  ACE_TEXT("release_os_resources() failed %m\n")));
    }
  delete this->transport ();
}

int
TAO_COIOP_Connection_Handler::open_handler (void *v)
{
  return this->open (v);
}

int
TAO_COIOP_Connection_Handler::open (void*)
{
  return 0;
}

int
TAO_COIOP_Connection_Handler::resume_handler (void)
{
  return ACE_Event_Handler::ACE_APPLICATION_RESUMES_HANDLER;
}

int
TAO_COIOP_Connection_Handler::close_connection (void)
{
  return this->close_connection_eh (this);
}

int
TAO_COIOP_Connection_Handler::handle_input (ACE_HANDLE h)
{
  return this->handle_input_eh (h, this);
}

int
TAO_COIOP_Connection_Handler::handle_output (ACE_HANDLE handle)
{
  int const result =
    this->handle_output_eh (handle, this);

  if (result == -1)
    {
      this->close_connection ();
      return 0;
    }

  return result;
}

int
TAO_COIOP_Connection_Handler::handle_timeout (const ACE_Time_Value &,
                                              const void *)
{
  // We don't use this upcall from the Reactor.  However, we should
  // override this since the base class returns -1 which will result
  // in handle_close() getting called.
  return 0;
}

int
TAO_COIOP_Connection_Handler::handle_close (ACE_HANDLE,
                                            ACE_Reactor_Mask)
{
  // No asserts here since the handler is registered with the Reactor
  // and the handler ownership is given to the Reactor.  When the
  // Reactor closes, it will call handle_close() on the handler.  It
  // is however important to overwrite handle_close() to do nothing
  // since the base class does too much.
  return 0;
}

int
TAO_COIOP_Connection_Handler::close (u_long flags)
{
  return this->close_handler (flags);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_COIOP && TAO_HAS_COIOP != 0 */
