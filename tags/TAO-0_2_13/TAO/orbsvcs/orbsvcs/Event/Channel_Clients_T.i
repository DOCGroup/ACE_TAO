/* -*- C++ -*- */
// $Id$

template <class TARGET> ACE_INLINE
ACE_PushConsumer_Adapter<TARGET>::ACE_PushConsumer_Adapter (TARGET *target)
  : target_ (target)
{
}

template <class TARGET> ACE_INLINE void
ACE_PushConsumer_Adapter<TARGET>::push (const RtecEventComm::EventSet& events,
					CORBA::Environment &_env)
{
  target_->_push (events, _env);
}

template <class TARGET> ACE_INLINE void
ACE_PushConsumer_Adapter<TARGET>::disconnect_push_consumer (CORBA::Environment &_env)
{
  target_->_disconnect_push_consumer (_env);
}

// ************************************************************

template <class TARGET> ACE_INLINE
ACE_PushSupplier_Adapter<TARGET>::ACE_PushSupplier_Adapter (TARGET *target)
  : target_ (target)
{
}

template <class TARGET> ACE_INLINE void
ACE_PushSupplier_Adapter<TARGET>::disconnect_push_supplier (CORBA::Environment &_env)
{
  target_->_disconnect_push_supplier (_env);
}
