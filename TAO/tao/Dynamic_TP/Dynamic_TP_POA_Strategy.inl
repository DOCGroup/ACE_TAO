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
          this->dtp_task_.set_min_pool_threads(1);
        }
      else
        {
          this->dtp_task_.set_min_pool_threads(tp_config->min_threads_);
        }

      // max_pool_threads_

      if (tp_config->max_threads_ <= 0)
        {
          this->dtp_task_.set_max_pool_threads(0);   // Set to 0 so that max is unbounded.
        }
      else
        if (tp_config->max_threads_ < tp_config->min_threads_)
          {
            this->dtp_task_.set_max_pool_threads(this->dtp_task_.get_min_pool_threads());
          }
        else
          {
            this->dtp_task_.set_max_pool_threads(tp_config->max_threads_);
          }


      // initial_pool_threads_
      if (tp_config->init_threads_ <= 0)
        {
          this->dtp_task_.set_init_pool_threads(this->dtp_task_.get_min_pool_threads());
        }
      else
        {
          this->dtp_task_.set_init_pool_threads(tp_config->init_threads_);
        }

      // thread_stack_size_

      if (tp_config->stack_size_ <= 0)
        {
          this->dtp_task_.set_thread_stack_size(ACE_DEFAULT_THREAD_STACKSIZE);
        }
      else
        {
          this->dtp_task_.set_thread_stack_size(tp_config->stack_size_);
        }

      // max_request_queue_depth_

      if (tp_config->queue_depth_ < 0)
        {
          this->dtp_task_.set_max_request_queue_depth(0);
        }
      else
        {
          this->dtp_task_.set_max_request_queue_depth(tp_config->queue_depth_);
        }

      // thread_idle_time_
      this->dtp_task_.set_thread_idle_time(tp_config->timeout_);

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
            this->dtp_task_.get_init_pool_threads(),
            this->dtp_task_.get_min_pool_threads(),
            this->dtp_task_.get_max_pool_threads(),
            this->dtp_task_.get_max_request_queue_depth(),
            this->dtp_task_.get_thread_stack_size(),
            this->dtp_task_.get_thread_idle_time()));
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
            this->dtp_task_.get_init_pool_threads(),
            this->dtp_task_.get_min_pool_threads(),
            this->dtp_task_.get_max_pool_threads(),
            this->dtp_task_.get_max_request_queue_depth(),
            this->dtp_task_.get_thread_stack_size(),
            this->dtp_task_.get_thread_idle_time()));
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
