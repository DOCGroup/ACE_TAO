/* -*-C++-*- */
//$Id$

//////////////////////////////////////////////////////////////////////////////////

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY, class IMPLEMENTATION>
ACE_Caching_Strategy_Adapter<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY, IMPLEMENTATION>::ACE_Caching_Strategy_Adapter (IMPLEMENTATION *implementation,
                                                                                                                                         int delete_implementation)
  : implementation_ (implementation),
    delete_implementation_ (delete_implementation)
{
  if (this->implementation_ == 0)
    {
      ACE_NEW (this->implementation_,
               IMPLEMENTATION);
      this->delete_implementation_ = 1;
    }
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY, class IMPLEMENTATION>
ACE_Caching_Strategy_Adapter<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY, IMPLEMENTATION>::~ACE_Caching_Strategy_Adapter (void)
{
  if (this->delete_implementation_)
    delete this->implementation_;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY, class IMPLEMENTATION> int
ACE_Caching_Strategy_Adapter<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY, IMPLEMENTATION>::open (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                                                                                                                 int delete_cleanup_strategy,
                                                                                                                 CACHING_STRATEGY_UTILITY *utility_s,
                                                                                                                 int delete_caching_strategy_utility)
{
  return this->implementation_->open (cleanup_s,
                                      delete_cleanup_strategy,
                                      utility_s,
                                      delete_caching_strategy_utility);
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY, class IMPLEMENTATION> ATTRIBUTES
ACE_Caching_Strategy_Adapter<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY, IMPLEMENTATION>::attributes (void)
{
  return this->implementation_->attributes ();
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY, class IMPLEMENTATION> double
ACE_Caching_Strategy_Adapter<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY, IMPLEMENTATION>::purge_percent (void)
{
  return this->implementation_->purge_percent ();
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY, class IMPLEMENTATION> void
ACE_Caching_Strategy_Adapter<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY, IMPLEMENTATION>::purge_percent (double percentage)
{
  this->implementation_->purge_percent (percentage);
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY, class IMPLEMENTATION> int
ACE_Caching_Strategy_Adapter<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY, IMPLEMENTATION>::notify_bind (int result,
                                                                                                                        const ATTRIBUTES &attr)
{
  return this->implementation_->notify_bind (result,
                                             attr);
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY, class IMPLEMENTATION> int
ACE_Caching_Strategy_Adapter<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY, IMPLEMENTATION>::notify_find (int result,
                                                                                                                        ATTRIBUTES &attr)
{
  return this->implementation_->notify_find (result,
                                             attr);
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY, class IMPLEMENTATION> int
ACE_Caching_Strategy_Adapter<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY, IMPLEMENTATION>::notify_unbind (int result,
                                                                                                                          const ATTRIBUTES &attr)
{
  return this->implementation_->notify_unbind (result,
                                               attr);
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY, class IMPLEMENTATION> int
ACE_Caching_Strategy_Adapter<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY, IMPLEMENTATION>::notify_trybind (int result,
                                                                                                                           ATTRIBUTES &attr)
{
  return this->implementation_->notify_trybind (result,
                                                attr);
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY, class IMPLEMENTATION> int
ACE_Caching_Strategy_Adapter<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY, IMPLEMENTATION>::notify_rebind (int result,
                                                                                                                          const ATTRIBUTES &attr)
{
  return this->implementation_->notify_rebind (result,
                                               attr);
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY, class IMPLEMENTATION> int
ACE_Caching_Strategy_Adapter<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY, IMPLEMENTATION>::clear_cache (CONTAINER &container)
{
  return this->implementation_->clear_cache (container);
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY, class IMPLEMENTATION> IMPLEMENTATION &
ACE_Caching_Strategy_Adapter<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY, IMPLEMENTATION>::implementation (void)
{
  return *this->implementation_;
}

//////////////////////////////////////////////////////////////////////////////////

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE ATTRIBUTES
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::attributes (void)
{
  return this->timer_;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE double
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::purge_percent (void)
{
  return this->purge_percent_;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE void
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::purge_percent (double percentage)
{
  this->purge_percent_ = percentage;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_bind (int result,
                                                                                                    const ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  if (result == 0)
    {
      ++this->timer_;
      ++this->entries_;
    }

  return result;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_find (int result,
                                                                                                    ATTRIBUTES &attr)
{
  if (result == 0)
    {
      attr = this->timer_;
      ++this->timer_;
    }

  return result;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_unbind (int result,
                                                                                                      const ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);
  return result;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_trybind (int result,
                                                                                                       ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  return result;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_rebind (int result,
                                                                                                      const ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  if (result == 0)
    ++this->timer_;

  return result;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE void
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::dump (void) const
{
  ACE_TRACE ("ACE_LRU_Caching_Strategy::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("timer_ = %d "), this->timer_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

//////////////////////////////////////////////////////////////////////////////////

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE ATTRIBUTES
ACE_LFU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::attributes (void)
{
  return 0;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE double
ACE_LFU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::purge_percent (void)
{
  return this->purge_percent_;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE void
ACE_LFU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::purge_percent (double percentage)
{
  this->purge_percent_ = percentage;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_LFU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_bind (int result,
                                                                                                    const ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  if (result == 0)
    ++this->entries_;

  return result;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_LFU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_find (int result,
                                                                                                    ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  if (result == 0)
    ++attr;

  return result;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_LFU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_trybind (int result,
                                                                                                       ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  return result;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_LFU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_rebind (int result,
                                                                                                      const ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  if (result == 0)
    ++this->entries_;

  return result;
}
template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_LFU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_unbind (int result,
                                                                                                      const ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  return result;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE void
ACE_LFU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::dump (void) const
{
  ACE_TRACE ("ACE_LFU_Caching_Strategy::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

//////////////////////////////////////////////////////////////////////////////////////

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE ATTRIBUTES
ACE_FIFO_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::attributes (void)
{
  return this->order_;
}
template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE double
ACE_FIFO_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::purge_percent (void)
{
  return this->purge_percent_;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE void
ACE_FIFO_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::purge_percent (double percentage)
{
  this->purge_percent_ = percentage;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_FIFO_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_bind (int result,
                                                                                                     const ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  if (result == 0)
    {
      ++this->order_;
      ++this->entries_;
    }

  return result;
}


template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_FIFO_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_find (int result,
                                                                                                     ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  return result;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_FIFO_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_unbind (int result,
                                                                                                       const ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  return result;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_FIFO_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_trybind (int result,
                                                                                                        ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  return result;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_FIFO_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_rebind (int result,
                                                                                                       const ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  if (result == 0)
    {
      ++this->order_;
      ++this->entries_;
    }

  return result;
}


template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE void
ACE_FIFO_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::dump (void) const
{
  ACE_TRACE ("ACE_FIFO_Caching_Strategy::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("order_ = %d "), this->order_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

//////////////////////////////////////////////////////////////////////////////////

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE ATTRIBUTES
ACE_Null_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::attributes (void)
{
  return 0;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE double
ACE_Null_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::purge_percent (void)
{
  return 0;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE void
ACE_Null_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::purge_percent (double percentage)
{
  ACE_UNUSED_ARG (percentage);
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_Null_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_bind (int result,
                                                                                                     const ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  return result;
}
template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_Null_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_find (int result,
                                                                                                     ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  return result;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_Null_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_unbind (int result,
                                                                                                       const ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  return result;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_Null_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_trybind (int result,
                                                                                                        ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  return result;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE int
ACE_Null_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::notify_rebind (int result,
                                                                                                       const ATTRIBUTES &attr)
{
  ACE_UNUSED_ARG (attr);

  return result;
}


template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE void
ACE_Null_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::dump (void) const
{
  ACE_TRACE ("ACE_Null_Caching_Strategy::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

//////////////////////////////////////////////////////////////////////////////////
