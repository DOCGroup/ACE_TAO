// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Dynamic_TP_POA_StrategyImpl::TAO_Dynamic_TP_POA_StrategyImpl
(TAO_DTP_Definition * dynamic_tp_config,
 bool serialize_servants)
  : serialize_servants_(serialize_servants)
{
  //Need to govern the rules of the parameters coming in and set the
  //appropriate values.

  // Check to see if a null config got sent in.
  if (dynamic_tp_config != 0 )
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_StrategyImpl: ")
                  ACE_TEXT ("Initialized with:\n")
                  ACE_TEXT ("    initial_pool_threads_=[%d]\n")
                  ACE_TEXT ("    min_pool_threads_=[%d]\n")
                  ACE_TEXT ("    max_pool_threads_=[%d]\n")
                  ACE_TEXT ("    max_request_queue_depth_=[%d]\n")
                  ACE_TEXT ("    thread_stack_size_=[%d]\n")
                  ACE_TEXT ("    thread_idle_time_=[%d]\n"),
                  this->initial_pool_threads_,
                  this->min_pool_threads_,
                  this->max_pool_threads_,
                  this->max_request_queue_depth_,
                  this->thread_stack_size_,
                  this->thread_idle_time_.sec()));

      // initial_pool_threads_
      if (dynamic_tp_config->init_threads_ > 0)
        {
          this->initial_pool_threads_ = dynamic_tp_config->init_threads_;
        }
      else
        {
          this->initial_pool_threads_ = 5; // set to default
        }

      // min_pool_threads_

      if ( dynamic_tp_config->min_threads_ < this->initial_pool_threads_)
        {
          this->min_pool_threads_ = this->initial_pool_threads_;

        }
      else
        {
          this->min_pool_threads_ = dynamic_tp_config->min_threads_;
        }

      // max_pool_threads_

      if ( dynamic_tp_config->max_threads_ < this->min_pool_threads_)
        {
          this->max_pool_threads_ =  -1;   // Set to -1 so that max is unbounded.
        }
      else
        {
          this->max_pool_threads_ = dynamic_tp_config->max_threads_;
        }

      // thread_stack_size_

      if ( dynamic_tp_config->stack_size_ <= 0 )
        {
          this->thread_stack_size_ = ACE_DEFAULT_THREAD_STACKSIZE;
        }
      else
        {
          this->thread_stack_size_ = dynamic_tp_config->stack_size_;
        }

      // max_request_queue_depth_

      if ( dynamic_tp_config->queue_depth_ <= 0 )
        {
          this->max_request_queue_depth_ = -1;
        }
      else
        {
          this->max_request_queue_depth_ = dynamic_tp_config->queue_depth_;
        }

      // thread_idle_time_
      // TODO: Need functionality to trap for invalid ACE_Time_Value values

      this->thread_idle_time_ = dynamic_tp_config->timeout_;
    }
  else
    {
      // Configuration was sent in null, so leave constructor defaults in place
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_StrategyImpl: ")
                  ACE_TEXT ("Null configuration provided, setting to defaults:\n")
                  ACE_TEXT ("     initial_pool_threads_=[%d]\n")
                  ACE_TEXT ("     min_pool_threads_=[%d]\n")
                  ACE_TEXT ("     max_pool_threads_=[%d]\n")
                  ACE_TEXT ("     max_request_queue_depth_=[%d]\n")
                  ACE_TEXT ("     thread_stack_size_=[%d]\n")
                  ACE_TEXT ("     thread_idle_time_=[%d]\n"),
                  this->initial_pool_threads_,
                  this->min_pool_threads_,
                  this->max_pool_threads_,
                  this->max_request_queue_depth_,
                  this->thread_stack_size_,
                  this->thread_idle_time_.sec ()));
    }


}


ACE_INLINE
void
TAO_Dynamic_TP_POA_StrategyImpl::set_servant_serialization(bool serialize_servants)
{
  // Simple Mutator.
  this->serialize_servants_ = serialize_servants;
}


TAO_END_VERSIONED_NAMESPACE_DECL
