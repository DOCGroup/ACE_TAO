//$Id$

#ifndef CACHING_STRATEGIES_T_C
#define CACHING_STRATEGIES_T_C

#include "ace/Caching_Strategies_T.h"

#if !defined (__ACE_INLINE__)
#include "ace/Caching_Strategies_T.i"
#endif /* __ACE_INLINE__ */

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_RCSID(ace, Caching_Strategies_T, "$Id$")

template<class CONTAINER> 
ACE_LRU_Caching_Strategy<CONTAINER>::ACE_LRU_Caching_Strategy (ACE_Cleanup_Strategy<CONTAINER> *cleanup_s,
                                                               int delete_cleanup_strategy)
  : timer_ (0),
    purge_percent_ (10),
    entries_ (0),
    cleanup_strategy_ (0),
    delete_cleanup_strategy_ (1)
{
  if (this->open (cleanup_s, delete_cleanup_strategy) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_LRU_Caching_Strategy::ACE_LRU_Caching_Strategy")));
  
}

template<class CONTAINER> 
ACE_LRU_Caching_Strategy<CONTAINER>::~ACE_LRU_Caching_Strategy (void)
{
}

template<class CONTAINER> int 
ACE_LRU_Caching_Strategy<CONTAINER>::open (ACE_Cleanup_Strategy<CONTAINER> *cleanup_s,
                                           int delete_cleanup_strategy)
{
  // Initialise the cleanup strategy.

  // First we decide whether we need to clean up.
  if (this->cleanup_strategy_ != 0 &&
      this->delete_cleanup_strategy_ == 1 &&
      cleanup_s != 0)

    {

      delete this->cleanup_strategy_;

      this->cleanup_strategy_ = 0;

      this->delete_cleanup_strategy_ = delete_cleanup_strategy;

    }

  if (cleanup_s != 0)
    this->cleanup_strategy_ = cleanup_s;
  else if (this->cleanup_strategy_ == 0)
    {

      ACE_NEW_RETURN (this->cleanup_strategy_,
                      CLEANUP_STRATEGY,
                      -1);

      this->delete_cleanup_strategy_ = delete_cleanup_strategy;

    }
    
  return 0;  
}

template<class CONTAINER> int
ACE_LRU_Caching_Strategy<CONTAINER>::clear_cache (CONTAINER &container)
{
  return this->caching_strategy_utility_.clear_cache (container,
                                                      this->cleanup_strategy_,
                                                      this->purge_percent_,
                                                      this->entries_);
}
  
////////////////////////////////////////////////////////////////////////////////////////////////

template<class CONTAINER> 
ACE_LFU_Caching_Strategy<CONTAINER>::ACE_LFU_Caching_Strategy (ACE_Cleanup_Strategy<CONTAINER> *cleanup_s,
                                                               int delete_cleanup_strategy)
  : purge_percent_ (10),
    entries_ (0),
    cleanup_strategy_ (0),
    delete_cleanup_strategy_ (1)
{
  if (this->open (cleanup_s, delete_cleanup_strategy) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_LFU_Caching_Strategy::ACE_LFU_Caching_Strategy")));
  
}

template<class CONTAINER> 
ACE_LFU_Caching_Strategy<CONTAINER>::~ACE_LFU_Caching_Strategy (void)
{
}

template<class CONTAINER> int 
ACE_LFU_Caching_Strategy<CONTAINER>::open (ACE_Cleanup_Strategy<CONTAINER> *cleanup_s,
                                           int delete_cleanup_strategy)
{
  // Initialise the cleanup strategy.

  // First we decide whether we need to clean up.
  if (this->cleanup_strategy_ != 0 &&
      this->delete_cleanup_strategy_ == 1 &&
      cleanup_s != 0)
    {

      delete this->cleanup_strategy_;

      this->cleanup_strategy_ = 0;

      this->delete_cleanup_strategy_ = delete_cleanup_strategy;

    }

  if (cleanup_s != 0)
    this->cleanup_strategy_ = cleanup_s;
  else if (this->cleanup_strategy_ == 0)
    {

      ACE_NEW_RETURN (this->cleanup_strategy_,
                      CLEANUP_STRATEGY,
                      -1);

      this->delete_cleanup_strategy_ = delete_cleanup_strategy;

    }
    
  return 0;
}

template<class CONTAINER> int
ACE_LFU_Caching_Strategy<CONTAINER>::clear_cache (CONTAINER &container)
{

  return this->caching_strategy_utility_.clear_cache (container,
                                                      this->cleanup_strategy_,
                                                      this->purge_percent_,
                                                      this->entries_);
}

////////////////////////////////////////////////////////////////////////////////////////////////

template<class CONTAINER> 
ACE_FIFO_Caching_Strategy<CONTAINER>::ACE_FIFO_Caching_Strategy (ACE_Cleanup_Strategy<CONTAINER> *cleanup_s,
                                                                 int delete_cleanup_strategy)
  : order_ (0),
    purge_percent_ (10),
    entries_ (0),
    cleanup_strategy_ (0),
    delete_cleanup_strategy_ (1)
{
  if (this->open (cleanup_s, delete_cleanup_strategy) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_FIFO_Caching_Strategy::ACE_FIFO_Caching_Strategy")));
}

template<class CONTAINER> 
ACE_FIFO_Caching_Strategy<CONTAINER>::~ACE_FIFO_Caching_Strategy (void)

{
}

template<class CONTAINER> int 
ACE_FIFO_Caching_Strategy<CONTAINER>::open (ACE_Cleanup_Strategy<CONTAINER> *cleanup_s,
                                           int delete_cleanup_strategy)
{
  // Initialise the cleanup strategy.

  // First we decide whether we need to clean up.
  if (this->cleanup_strategy_ != 0 &&
      this->delete_cleanup_strategy_ == 1 &&
      cleanup_s != 0)
    {

      delete this->cleanup_strategy_;

      this->cleanup_strategy_ = 0;

      this->delete_cleanup_strategy_ = delete_cleanup_strategy;

    }

  if (cleanup_s != 0)
    this->cleanup_strategy_ = cleanup_s;
  else if (this->cleanup_strategy_ == 0)
    {

      ACE_NEW_RETURN (this->cleanup_strategy_,
                      CLEANUP_STRATEGY, 
                      -1);

      this->delete_cleanup_strategy_ = delete_cleanup_strategy;

    }
    
  return 0;  
}




template<class CONTAINER> int
ACE_FIFO_Caching_Strategy<CONTAINER>::clear_cache (CONTAINER &container)
{

  return this->caching_strategy_utility_.clear_cache (container,
                                                      this->cleanup_strategy_,
                                                      this->purge_percent_,
                                                      this->entries_);
}


////////////////////////////////////////////////////////////////////////////////////////////////


template<class CONTAINER>  
ACE_Null_Caching_Strategy<CONTAINER>::ACE_Null_Caching_Strategy (ACE_Cleanup_Strategy<CONTAINER> *cleanup_s,
                                                                 int delete_cleanup_strategy)
{
}

template<class CONTAINER> int 
ACE_Null_Caching_Strategy<CONTAINER>::open (ACE_Cleanup_Strategy<CONTAINER> *cleanup_s,
                                            int delete_cleanup_strategy)
{
}

template<class CONTAINER> 
ACE_Null_Caching_Strategy<CONTAINER>::~ACE_Null_Caching_Strategy (void)
{
}

template<class CONTAINER> int
ACE_Null_Caching_Strategy<CONTAINER>::clear_cache (CONTAINER &container)
{
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ACE_ALLOC_HOOK_DEFINE(ACE_LRU_Caching_Strategy)
ACE_ALLOC_HOOK_DEFINE(ACE_LFU_Caching_Strategy)
ACE_ALLOC_HOOK_DEFINE(ACE_FIFO_Caching_Strategy)
ACE_ALLOC_HOOK_DEFINE(ACE_Null_Caching_Strategy)
#endif /* CACHING_STRATEGIES_T_C */



