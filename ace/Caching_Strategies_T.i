/* -*-C++-*- */
//$Id$

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE ATTRIBUTES
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::attributes (void)
{
  return this->timer_;
}   

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE unsigned int
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::purge_percent (void)
{
  return this->purge_percent_;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE void
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::purge_percent (unsigned int percentage)
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

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE unsigned int
ACE_LFU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::purge_percent (void)
{
  return this->purge_percent_;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE void
ACE_LFU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::purge_percent (unsigned int percentage)
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
template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE unsigned int
ACE_FIFO_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::purge_percent (void)
{
  return this->purge_percent_;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE void
ACE_FIFO_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::purge_percent (unsigned int percentage)
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

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE unsigned int
ACE_Null_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::purge_percent (void)
{
  return 0;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> ACE_INLINE void
ACE_Null_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::purge_percent (unsigned int percentage)
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
