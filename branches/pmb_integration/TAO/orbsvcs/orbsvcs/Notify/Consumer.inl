// $Id$

#include "ProxySupplier.h"

ACE_INLINE TAO_SYNCH_MUTEX*
TAO_Notify_Consumer::proxy_lock (void)
{
  return &this->proxy_->lock_;
}

ACE_INLINE TAO_Notify_ProxySupplier*
TAO_Notify_Consumer::proxy_supplier (void)
{
  return this->proxy_;
}

ACE_INLINE CORBA::Boolean
TAO_Notify_Consumer::is_suspended (void)
{
  return this->is_suspended_;
}

ACE_INLINE void
TAO_Notify_Consumer::suspend (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->is_suspended_ = 1;
}

ACE_INLINE void
TAO_Notify_Consumer::push (const TAO_Notify_Event_var &event ACE_ENV_ARG_DECL)
{
  if (this->is_suspended_ == 1) // If we're suspended, queue for later delivery.
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, *this->proxy_lock ());

      this->event_collection_->enqueue_head (event);

      return;
    }

  ACE_TRY
    {
      this->push_i (event ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, not_exist)
    {
      this->handle_dispatch_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      this->handle_dispatch_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
}

ACE_INLINE void
TAO_Notify_Consumer::push (const TAO_Notify_Event* event ACE_ENV_ARG_DECL)
{
  if (this->is_suspended_ == 1) // If we're suspended, queue for later delivery.
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, *this->proxy_lock ());

      TAO_Notify_Event* event_copy = event->copy (ACE_ENV_SINGLE_ARG_PARAMETER);
      TAO_Notify_Event_Copy_var event_var (event_copy);

      this->event_collection_->enqueue_head (event_var);

      return;
    }

  ACE_TRY
    {
      this->push_i (event ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, not_exist)
    {
      this->handle_dispatch_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      this->handle_dispatch_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
}
