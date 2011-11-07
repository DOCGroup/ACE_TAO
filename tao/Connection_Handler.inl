// -*- C++ -*-
//
//$Id$

#include "tao/Transport.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_ORB_Core *
TAO_Connection_Handler::orb_core (void)
{
  return this->orb_core_;
}

ACE_INLINE TAO_Transport *
TAO_Connection_Handler::transport (void)
{
  return this->transport_;
}

ACE_INLINE bool
TAO_Connection_Handler::is_closed (void) const
{
  return (this->state_ == TAO_LF_Event::LFS_CONNECTION_CLOSED ||
          this->state_ == TAO_LF_Event::LFS_TIMEOUT || this->is_closed_);
}

ACE_INLINE bool
TAO_Connection_Handler::is_timeout (void) const
{
  return (this->state_ == TAO_LF_Event::LFS_TIMEOUT);
}

ACE_INLINE bool
TAO_Connection_Handler::is_open (void) const
{
  return this->state_ == TAO_LF_Event::LFS_SUCCESS;
}

ACE_INLINE bool
TAO_Connection_Handler::is_connecting (void) const
{
  return this->state_ == TAO_LF_Event::LFS_CONNECTION_WAIT;
}

ACE_INLINE void
TAO_Connection_Handler::connection_pending (void)
{
  if (!this->connection_pending_)
    {
      this->connection_pending_ = true;
      this->transport()->add_reference();
    }
}

ACE_INLINE void
TAO_Connection_Handler::cancel_pending_connection (void)
{
  if (this->connection_pending_)
    {
      this->connection_pending_ = false;
      this->transport()->remove_reference();
    }
}

/// Derived classes should implement this for proper support with the
/// Blocking Flushing Strategy.
ACE_INLINE int
TAO_Connection_Handler::handle_write_ready (const ACE_Time_Value *)
{
  return 0;
}

//@@ CONNECTION_HANDLER_SPL_METHODS_ADD_HOOK

TAO_END_VERSIONED_NAMESPACE_DECL
