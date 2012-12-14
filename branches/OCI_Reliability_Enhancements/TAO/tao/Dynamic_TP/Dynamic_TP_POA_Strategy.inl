// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
/// Constructor.
TAO_Dynamic_TP_POA_Strategy::TAO_Dynamic_TP_POA_Strategy
(ACE_CString tp_config_name,
 bool ss)
  : serialize_servants_ (ss)
{
  this->dynamic_tp_config_name_ = tp_config_name;
}

ACE_INLINE
TAO_Dynamic_TP_POA_Strategy::TAO_Dynamic_TP_POA_Strategy
(TAO_DTP_Definition * tp_config,
 bool ss)
  : serialize_servants_ (ss)
{
  //Need to govern the rules of the parameters coming in and set the
  //appropriate values.

  // Check to see if a null config got sent in.
  if (tp_config != 0 )
    {

      // min_pool_threads_

      if ( tp_config->min_threads_ <= 0)
        {
          this->min_pool_threads_ = 1;
        }
      else
        {
          this->min_pool_threads_ = tp_config->min_threads_;
        }

      // max_pool_threads_

      if ( tp_config->max_threads_ < this->min_pool_threads_)
        {
          this->max_pool_threads_ =  -1;   // Set to -1 so that max is unbounded.
        }
      else
        {
          this->max_pool_threads_ = tp_config->max_threads_;
        }


      // initial_pool_threads_
      if (tp_config->init_threads_ <= this->min_pool_threads_)
        {
          this->initial_pool_threads_ = this->min_pool_threads_;
        }
      else if ((tp_config->init_threads_ > this->max_pool_threads_) &&
               (this->max_pool_threads_ > 0))
        {
          this->initial_pool_threads_ = this->max_pool_threads_;
        }
      else
        {
          this->initial_pool_threads_ = tp_config->init_threads_; // set to default
        }

      // thread_stack_size_

      if ( tp_config->stack_size_ <= 0 )
        {
          this->thread_stack_size_ = ACE_DEFAULT_THREAD_STACKSIZE;
        }
      else
        {
          this->thread_stack_size_ = tp_config->stack_size_;
        }

      // max_request_queue_depth_

      if ( tp_config->queue_depth_ <= 0 )
        {
          this->max_request_queue_depth_ = -1;
        }
      else
        {
          this->max_request_queue_depth_ = tp_config->queue_depth_;
        }

      // thread_idle_time_
      this->thread_idle_time_ = tp_config->timeout_;

      if (TAO_debug_level > 4)
      {
            ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Strategy: ")
            ACE_TEXT ("Initialized with:\n")
            ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Strategy initial_pool_threads_=[%d]\n")
            ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Strategy min_pool_threads_=[%d]\n")
            ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Strategy max_pool_threads_=[%d]\n")
            ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Strategy max_request_queue_depth_=[%d]\n")
            ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Strategy thread_stack_size_=[%d]\n")
            ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Strategy thread_idle_time_=[%d]\n"),
            this->initial_pool_threads_,
            this->min_pool_threads_,
            this->max_pool_threads_,
            this->max_request_queue_depth_,
            this->thread_stack_size_,
            this->thread_idle_time_.sec()));
      }
    }
  else
    {
      // Configuration was sent in null, so leave constructor defaults in place
      if (TAO_debug_level > 4)
      {

        ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Strategy: ")
            ACE_TEXT ("Null configuration provided, setting to defaults:\n")
            ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Strategy initial_pool_threads_=[%d]\n")
            ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Strategy min_pool_threads_=[%d]\n")
            ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Strategy max_pool_threads_=[%d]\n")
            ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Strategy max_request_queue_depth_=[%d]\n")
            ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Strategy thread_stack_size_=[%d]\n")
            ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Strategy thread_idle_time_=[%d]\n"),
            this->initial_pool_threads_,
            this->min_pool_threads_,
            this->max_pool_threads_,
            this->max_request_queue_depth_,
            this->thread_stack_size_,
            this->thread_idle_time_.sec ()));
      }
    }
}

ACE_INLINE
void
TAO_Dynamic_TP_POA_Strategy::set_servant_serialization(bool serialize_servants)
{
  // Simple Mutator.
  this->serialize_servants_ = serialize_servants;
}


TAO_END_VERSIONED_NAMESPACE_DECL
