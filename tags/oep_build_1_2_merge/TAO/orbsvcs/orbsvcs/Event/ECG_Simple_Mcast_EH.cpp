// $Id$

#include "ECG_Simple_Mcast_EH.h"
#include "ace/Log_Msg.h"
#include "ace/Reactor.h"

#if !defined(__ACE_INLINE__)
#include "ECG_Simple_Mcast_EH.i"
#endif /* __ACE_INLINE__ */

TAO_ECG_Simple_Mcast_EH::TAO_ECG_Simple_Mcast_EH (TAO_ECG_Dgram_Handler *recv)
  : receiver_ (recv)
{
  ACE_ASSERT (this->receiver_);
}

TAO_ECG_Simple_Mcast_EH::~TAO_ECG_Simple_Mcast_EH (void)
{
}

int
TAO_ECG_Simple_Mcast_EH::open (const char * mcast_addr,
                               const ACE_TCHAR *net_if)
{
  // Check that we haven't been closed already.
  if (!this->receiver_)
    return -1;

  if (mcast_addr == 0)
    return -1;

  ACE_INET_Addr mcast_group;
  if (mcast_group.set (mcast_addr) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open mcast handler: "
                       "error using specified address %s "
                       "in ACE_INET.set ().",
                       mcast_addr),
                      -1);

  if (this->dgram_.subscribe (mcast_group, 1, net_if) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                      "Unable to open mcast handler: error "
                      "subscribing to %s",
                       mcast_addr),
                      -1);

  (void) dgram_.enable(ACE_NONBLOCK);

  if (!this->reactor ()
      || 0 != this->reactor ()->register_handler (this->dgram_.get_handle (),
                                                  this,
                                                  ACE_Event_Handler::READ_MASK))
    {
      this->dgram_.close ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot register handler with reactor."),
                        -1);
    }

  return 0;
}

int
TAO_ECG_Simple_Mcast_EH::shutdown (void)
{
  // Already shut down.
  if (!this->receiver_)
    return -1;

  int result = 0;
  if (this->reactor ())
    {
      result = this->reactor ()->remove_handler (this->dgram_.get_handle (),
                                                 ACE_Event_Handler::READ_MASK);
    }
  if (result != 0)
    ACE_ERROR ((LM_ERROR,
                "Unable to deregister handler from reactor "
                "on shutdown."));

  result = this->dgram_.close ();
  if (result != 0)
    ACE_ERROR ((LM_ERROR,
                "Unable to close mcast receiving dgram "
                "on shutdown."));

  this->receiver_ = 0;

  return result;
}

int
TAO_ECG_Simple_Mcast_EH::handle_input (ACE_HANDLE /* fd */)
{
  return this->receiver_->handle_input (this->dgram_);
}
