// -*- C++ -*-
//
// $Id:

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Dynamic_TP_POA_StrategyImpl::TAO_Dynamic_TP_POA_StrategyImpl(
								  TAO_DTP_Definition * dynamic_tp_config,
								  bool     serialize_servants = true)
  : initial_pool_threads_(dynamic_tp_config->init_threads_),
    min_pool_threads_(dynamic_tp_config->min_threads_),
	max_pool_threads_(dynamic_tp_config->max_threads_),
	max_request_queue_depth_(dynamic_tp_config->queue_depth_),
	thread_stack_size_(dynamic_tp_config->stack_size_),
	thread_idle_time_(dynamic_tp_config->timeout_),
    serialize_servants_(serialize_servants)
{
  // Assumes that num_threads > 0.
}


ACE_INLINE
void
TAO_Dynamic_TP_POA_StrategyImpl::set_servant_serialization(bool serialize_servants)
{
  // Simple Mutator.
  this->serialize_servants_ = serialize_servants;
}


TAO_END_VERSIONED_NAMESPACE_DECL
