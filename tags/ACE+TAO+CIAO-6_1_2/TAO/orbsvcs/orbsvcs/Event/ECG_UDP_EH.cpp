// $Id$

#include "orbsvcs/Event/ECG_UDP_EH.h"
#include "ace/Reactor.h"
#include "ace/INET_Addr.h"

#if !defined(__ACE_INLINE__)
#include "orbsvcs/Event/ECG_UDP_EH.inl"
#endif /* __ACE_INLINE__ */



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ECG_UDP_EH::TAO_ECG_UDP_EH (TAO_ECG_Dgram_Handler *recv)
  :  receiver_ (recv)
{
  ACE_ASSERT (this->receiver_);
}

TAO_ECG_UDP_EH::~TAO_ECG_UDP_EH (void)
{
}

int
TAO_ECG_UDP_EH::open (const ACE_INET_Addr& ipaddr,
                      int reuse_addr)
{
  // Check that we haven't been closed already.
  if (!this->receiver_)
    return -1;

  if (this->dgram_.open (ipaddr, PF_INET, 0, reuse_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open udp handler: "
                       "error opening receiving dgram.\n"),
                       -1);

  if (!this->reactor ()
      || 0 != this->reactor ()->register_handler (this->dgram_.get_handle (),
                                                  this,
                                                  ACE_Event_Handler::READ_MASK))
    {
      this->dgram_.close ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot register handler with reactor.\n"),
                        -1);
    }

  return 0;
}

int
TAO_ECG_UDP_EH::shutdown (void)
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
                "on shutdown.\n"));

  result = this->dgram_.close ();
  if (result != 0)
    ACE_ERROR ((LM_ERROR,
                "Unable to close receiving dgram on shutdown.\n"));

  this->receiver_ = 0;

  return result;
}

int
TAO_ECG_UDP_EH::handle_input (ACE_HANDLE)
{
  return this->receiver_->handle_input (this->dgram_);
}

TAO_END_VERSIONED_NAMESPACE_DECL

// ****************************************************************
