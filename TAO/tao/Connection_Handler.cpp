//$Id$

#include "tao/Connection_Handler.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/debug.h"
#include "tao/Object.h"
#include "tao/Messaging_Policy_i.h"

#if !defined (__ACE_INLINE__)
#include "tao/Connection_Handler.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Connection_Handler, "$Id$")

TAO_Connection_Handler::TAO_Connection_Handler (TAO_ORB_Core *orb_core)
  :orb_core_ (orb_core),
   tss_resources_ (orb_core->get_tss_resources ()),
   ref_count_ (1),
   cache_map_entry_ (0),
   is_registered_ (0)
{
}


TAO_Connection_Handler::~TAO_Connection_Handler (void)
{
  // Set some of the pointers that we hold to zero explicitly, so that
  // nobody tries to access these
  this->orb_core_ = 0;
  this->tss_resources_ = 0;
  this->cache_map_entry_ = 0;
}


int
TAO_Connection_Handler::purge_entry (void)
{
  int retval =
    this->orb_core_->connection_cache ().purge_entry (this->cache_map_entry_);

  // Decrement our reference count as we have been removed from the
  // cache map.
  this->decr_ref_count ();

  return retval;
}

int
TAO_Connection_Handler::make_idle (void)
{
  return
    this->orb_core_->connection_cache ().make_idle (this->cache_map_entry_);
}


int
TAO_Connection_Handler::set_socket_option (ACE_SOCK &sock,
                                           int snd_size,
                                           int rcv_size)
{
#if !defined (ACE_LACKS_SOCKET_BUFSIZ)

  if (sock.set_option (SOL_SOCKET,
                       SO_SNDBUF,
                       (void *) &snd_size,
                       sizeof (snd_size)) == -1
      && errno != ENOTSUP)
    return -1;
  else if (sock.set_option (SOL_SOCKET,
                            SO_RCVBUF,
                            (void *) &rcv_size,
                            sizeof (int)) == -1
           && errno != ENOTSUP)
    return -1;
#endif /* !ACE_LACKS_SOCKET_BUFSIZ */

  (void) sock.enable (ACE_CLOEXEC);
  // Set the close-on-exec flag for that file descriptor. If the
  // operation fails we are out of luck (some platforms do not support
  // it and return -1).

  return 0;
}

int
TAO_Connection_Handler::svc_i (void)
{
  int result = 0;

  // Inheriting the ORB_Core tss stuff from the parent thread.
  this->orb_core_->inherit_from_parent_thread (this->tss_resources_);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) TAO_Connection_Handler::svc_i begin\n")));

  // Here we simply synthesize the "typical" event loop one might find
  // in a reactive handler, except that this can simply block waiting
  // for input.

  ACE_Time_Value *max_wait_time = 0;
  ACE_Time_Value timeout;
  ACE_Time_Value current_timeout;

  if (this->orb_core_->thread_per_connection_timeout (timeout))
    {
      current_timeout = timeout;
      max_wait_time = &current_timeout;
    }

  while (!this->orb_core_->has_shutdown ()
         && result >= 0)
    {
      result = this->handle_input_i (ACE_INVALID_HANDLE, max_wait_time);

      if (result == -1 && errno == ETIME)
        {
          // Ignore timeouts, they are only used to wake up and
          // shutdown.
          result = 0;

          // Reset errno to make sure we don't trip over an old value
          // of errno in case it is not reset when the recv() call
          // fails if the socket has been closed.
          errno = 0;
        }

      current_timeout = timeout;

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) TAO_Connection_Handler::svc_i - ")
                    ACE_TEXT ("loop <%d>\n"), current_timeout.msec ()));
    }

  if (TAO_debug_level > 0)
    ACE_DEBUG  ((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) TAO_Connection_Handler::svc_i end\n")));

  return result;
}
