/* -*- C++ -*- */
//$Id$


template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> ACE_INLINE int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::make_svc_handler
  (SVC_HANDLER *&sh)
{
  return this->creation_strategy_->make_svc_handler (sh);
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> ACE_INLINE int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::activate_svc_handler
  (SVC_HANDLER *svc_handler)
{
  return this->concurrency_strategy_->activate_svc_handler (svc_handler);
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> ACE_INLINE int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::assign_recycler
  (SVC_HANDLER *svc_handler,
   ACE_Connection_Recycling_Strategy *recycler,
   const void *recycling_act)
{
  return this->recycling_strategy_->assign_recycler (svc_handler,
                                                     recycler,
                                                     recycling_act);
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> ACE_INLINE int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::prepare_for_recycling
  (SVC_HANDLER *svc_handler)
{
  return this->recycling_strategy_->prepare_for_recycling (svc_handler);
}
