/* -*-C++-*- */
//$Id$

template<class CONTAINER> ACE_INLINE
ACE_LRU_Caching_Strategy<CONTAINER>::ACE_LRU_Caching_Strategy (CONTAINER &container, ATTRIBUTES timer, int purge_percent)
  : timer_ (timer),
    container_ (container),
    purge_percent_ (purge_percent),
    entries_ (0)
{
}

template<class CONTAINER> ACE_INLINE
ACE_LRU_Caching_Strategy<CONTAINER>::~ACE_LRU_Caching_Strategy (void)
{
}

template<class CONTAINER> ACE_INLINE ACE_LRU_Caching_Strategy<CONTAINER>::ATTRIBUTES
ACE_LRU_Caching_Strategy<CONTAINER>::attributes (void)
{
  return this->timer_;
}   

template<class CONTAINER> ACE_INLINE int
ACE_LRU_Caching_Strategy<CONTAINER>::purge_percent (void)
{
  return this->purge_percent_;
}

template<class CONTAINER> ACE_INLINE void
ACE_LRU_Caching_Strategy<CONTAINER>::purge_percent (int percentage)
{
  this->purge_percent_ = percentage;
}

template<class CONTAINER> ACE_INLINE int 
ACE_LRU_Caching_Strategy<CONTAINER>::notify_bind (int result, 
                                                  const ATTRIBUTES &attributes)
{
  if (result == 0)
    {
      ++this->timer_;
      ++ this->entries_;
    }
  
  return result;
}

template<class CONTAINER> ACE_INLINE int 
ACE_LRU_Caching_Strategy<CONTAINER>::notify_find (int result, 
                                                  ATTRIBUTES &attr)
{
  if (result == 0)
    {
      attr = this->timer_;
      ++this->timer_;
    }
  return result;
}

template<class CONTAINER> ACE_INLINE int 
ACE_LRU_Caching_Strategy<CONTAINER>::notify_unbind (int result, 
                                                          const ATTRIBUTES &attr)
{
  return result;
}

template<class CONTAINER> ACE_INLINE int 
ACE_LRU_Caching_Strategy<CONTAINER>::notify_trybind (int result, 
                                                           ATTRIBUTES &attr)
{
  return result;
}

template<class CONTAINER> ACE_INLINE int 
ACE_LRU_Caching_Strategy<CONTAINER>::notify_rebind (int result, 
                                                          const ATTRIBUTES &attr)
{ 
  if (result == 0)
    ++this->timer_;
  return result;
}

template<class CONTAINER> ACE_INLINE void
ACE_LRU_Caching_Strategy<CONTAINER>::dump (void) const 
{
  ACE_TRACE ("ACE_LRU_Caching_Strategy::dump");
      
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("timer_ = %d "), this->timer_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

//////////////////////////////////////////////////////////////////////////////////

template<class CONTAINER> ACE_INLINE 
ACE_LFU_Caching_Strategy<CONTAINER>::ACE_LFU_Caching_Strategy (void)
{
}

template<class CONTAINER> ACE_INLINE 
ACE_LFU_Caching_Strategy<CONTAINER>::~ACE_LFU_Caching_Strategy (void)
{
}

template<class CONTAINER> ACE_INLINE ACE_LFU_Caching_Strategy<CONTAINER>::ATTRIBUTES
ACE_LFU_Caching_Strategy<CONTAINER>::attributes (void)
{
  return 0;
}

template<class CONTAINER> ACE_INLINE int 
ACE_LFU_Caching_Strategy<CONTAINER>::notify_bind (int result, 
                                                  const ATTRIBUTES &attr)
{
  return result;
}

template<class CONTAINER> ACE_INLINE int
ACE_LFU_Caching_Strategy<CONTAINER>::notify_find (int result, 
                                                  ATTRIBUTES &attr)
{
  if (result == 0)
    attr += 1; 
  return result;    
}

template<class CONTAINER> ACE_INLINE int 
ACE_LFU_Caching_Strategy<CONTAINER>::notify_trybind (int result, 
                                                     ATTRIBUTES &attr)
{
  return result;
}

template<class CONTAINER> ACE_INLINE int 
ACE_LFU_Caching_Strategy<CONTAINER>::notify_rebind (int result, 
                                                    const ATTRIBUTES &attr)
{ 
  return result;
}
template<class CONTAINER> ACE_INLINE int 
ACE_LFU_Caching_Strategy<CONTAINER>::notify_unbind (int result, 
                                                    const ATTRIBUTES &attr)
{
  return result;
}

template<class CONTAINER> ACE_INLINE void
ACE_LFU_Caching_Strategy<CONTAINER>::dump (void) const
{
  ACE_TRACE ("ACE_LFU_Caching_Strategy::dump");
      
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

//////////////////////////////////////////////////////////////////////////////////////

template<class CONTAINER> ACE_INLINE 
ACE_FIFO_Caching_Strategy<CONTAINER>::ACE_FIFO_Caching_Strategy (ATTRIBUTES order)
  : order_ (order)
{
}

template<class CONTAINER> ACE_INLINE 
ACE_FIFO_Caching_Strategy<CONTAINER>::~ACE_FIFO_Caching_Strategy (void)
{
}

template<class CONTAINER> ACE_INLINE ACE_FIFO_Caching_Strategy<CONTAINER>::ATTRIBUTES
ACE_FIFO_Caching_Strategy<CONTAINER>::attributes (void)
{
  return this->order_;
}

template<class CONTAINER> ACE_INLINE int
ACE_FIFO_Caching_Strategy<CONTAINER>::notify_bind (int result, 
                                                   const ATTRIBUTES &attr)
{
  if (result == 0)
    ++this->order_;
  return result;
}
template<class CONTAINER> ACE_INLINE int 
ACE_FIFO_Caching_Strategy<CONTAINER>::notify_find (int result, 
                                                   ATTRIBUTES &attr)
{
  return result;
}

template<class CONTAINER> ACE_INLINE int 
ACE_FIFO_Caching_Strategy<CONTAINER>::notify_unbind (int result, 
                                                     const ATTRIBUTES &attr)
{
  return result;
}

template<class CONTAINER> ACE_INLINE int 
ACE_FIFO_Caching_Strategy<CONTAINER>::notify_trybind (int result, 
                                                      ATTRIBUTES &attr)
{
  return result;
}

template<class CONTAINER> ACE_INLINE int
ACE_FIFO_Caching_Strategy<CONTAINER>::notify_rebind (int result, 
                                                     const ATTRIBUTES &attr)  
{
  if (result == 0)
    ++this->order_;
  return result;
}    


template<class CONTAINER> ACE_INLINE void
ACE_FIFO_Caching_Strategy<CONTAINER>::dump (void) const
{
  ACE_TRACE ("ACE_FIFO_Caching_Strategy::dump");
      
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("order_ = %d "), this->order_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

//////////////////////////////////////////////////////////////////////////////////
template<class CONTAINER> ACE_INLINE 
ACE_Null_Caching_Strategy<CONTAINER>::ACE_Null_Caching_Strategy (void)
  : dummy_ (0)
{
}

template<class CONTAINER> ACE_INLINE 
ACE_Null_Caching_Strategy<CONTAINER>::~ACE_Null_Caching_Strategy (void)
{
}

template<class CONTAINER> ACE_INLINE ACE_Null_Caching_Strategy<CONTAINER>::ATTRIBUTES
ACE_Null_Caching_Strategy<CONTAINER>::attributes (void)
{
  return this->dummy_;
}

template<class CONTAINER> ACE_INLINE int
ACE_Null_Caching_Strategy<CONTAINER>::notify_bind (int result, 
                                                   const ATTRIBUTES &attr)
{
  return result;
}
template<class CONTAINER> ACE_INLINE int 
ACE_Null_Caching_Strategy<CONTAINER>::notify_find (int result, 
                                                   ATTRIBUTES &attr)
{
  return result;
}

template<class CONTAINER> ACE_INLINE int 
ACE_Null_Caching_Strategy<CONTAINER>::notify_unbind (int result, 
                                                     const ATTRIBUTES &attr)
{
  return result;
}

template<class CONTAINER> ACE_INLINE int 
ACE_Null_Caching_Strategy<CONTAINER>::notify_trybind (int result, 
                                                      ATTRIBUTES &attr)
{
  return result;
}

template<class CONTAINER> ACE_INLINE int
ACE_Null_Caching_Strategy<CONTAINER>::notify_rebind (int result, 
                                                     const ATTRIBUTES &attr)  
{
  return result;
}    


template<class CONTAINER> ACE_INLINE void
ACE_Null_Caching_Strategy<CONTAINER>::dump (void) const
{
  ACE_TRACE ("ACE_Null_Caching_Strategy::dump");
      
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

//////////////////////////////////////////////////////////////////////////////////
