//
// $Id$
//

template<class TARGET> ACE_INLINE
ACE_PushConsumer_Adapter<TARGET>::ACE_PushConsumer_Adapter (TARGET *t)
  :  target_ (t)
{
}

template<class TARGET> ACE_INLINE
ACE_PushSupplier_Adapter<TARGET>::ACE_PushSupplier_Adapter (TARGET *t)
  :  target_ (t)
{
}


