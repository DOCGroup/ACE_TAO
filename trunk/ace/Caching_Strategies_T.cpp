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

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY>
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::ACE_LRU_Caching_Strategy (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                                                                                                                 int delete_cleanup_strategy,
                                                                                                                 CACHING_STRATEGY_UTILITY *utility_s,
                                                                                                                 int delete_caching_strategy_utility)
  : timer_ (0),
    purge_percent_ (10),
    entries_ (0),
    cleanup_strategy_ (0),
    delete_cleanup_strategy_ (1),
    caching_strategy_utility_ (0),
    delete_caching_strategy_utility_ (1)
{
  if (this->open (cleanup_s,
                  delete_cleanup_strategy,
                  utility_s,
                  delete_caching_strategy_utility) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_LRU_Caching_Strategy::ACE_LRU_Caching_Strategy")));

}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY>
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::~ACE_LRU_Caching_Strategy (void)
{
  if (this->delete_cleanup_strategy_ == 1)
    delete this->cleanup_strategy_;
  this->delete_cleanup_strategy_ = 0;
  this->cleanup_strategy_ = 0;

  if (this->delete_caching_strategy_utility_ == 1)
    delete this->caching_strategy_utility_;
  this->delete_caching_strategy_utility_ = 0;
  this->caching_strategy_utility_ = 0;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> int
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::open (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                                                                                             int delete_cleanup_strategy,
                                                                                             CACHING_STRATEGY_UTILITY *utility_s,
                                                                                             int delete_caching_strategy_utility)
{
  // Initialise the cleanup strategy.

  // First we decide whether we need to clean up.
   if (this->cleanup_strategy_ != 0 &&
      this->delete_cleanup_strategy_ == 1 &&
      cleanup_s != 0)
    {
      delete this->cleanup_strategy_;
      this->cleanup_strategy_ = 0;
      this->delete_cleanup_strategy_ = 0;
    }

  if (cleanup_s != 0)
    {
      this->cleanup_strategy_ = cleanup_s;
      this->delete_cleanup_strategy_ = delete_cleanup_strategy;
    }
  else if (this->cleanup_strategy_ == 0)
    {
      ACE_NEW_RETURN (this->cleanup_strategy_,
                      CLEANUP_STRATEGY,
                      -1);

      this->delete_cleanup_strategy_ = 1;
    }

  // Initialise the caching strategy utility.

  // First we decide whether we need to clean up.
  if (this->caching_strategy_utility_ != 0 &&
      this->delete_caching_strategy_utility_ == 1 &&
      utility_s != 0)
    {
      delete this->caching_strategy_utility_;
      this->caching_strategy_utility_ = 0;
      this->delete_caching_strategy_utility_ = 0;
    }

  if (utility_s != 0)
    {
      this->caching_strategy_utility_ = utility_s;
      this->delete_caching_strategy_utility_ = delete_caching_strategy_utility;
    }
  else if (this->caching_strategy_utility_ == 0)
    {
      ACE_NEW_RETURN (this->caching_strategy_utility_,
                      CACHING_STRATEGY_UTILITY,
                      -1);

      this->delete_caching_strategy_utility_ = 1;
    }

  return 0;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> int
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::clear_cache (CONTAINER &container)
{
  return this->caching_strategy_utility_->clear_cache (container,
                                                       this->cleanup_strategy_,
                                                       this->purge_percent_);
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> int
ACE_LRU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::clear_cache (CONTAINER &container,
                                                                                                    unsigned int &total_container_entries)
{

  return this->caching_strategy_utility_->clear_cache (container,
                                                       this->cleanup_strategy_,
                                                       this->purge_percent_,
                                                       total_container_entries);
}

////////////////////////////////////////////////////////////////////////////////////////////////

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY>
ACE_LFU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::ACE_LFU_Caching_Strategy (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                                                                                                                 int delete_cleanup_strategy,
                                                                                                                 CACHING_STRATEGY_UTILITY *utility_s,
                                                                                                                 int delete_caching_strategy_utility)
  : purge_percent_ (10),
    entries_ (0),
    cleanup_strategy_ (0),
    delete_cleanup_strategy_ (1),
    caching_strategy_utility_ (0),
    delete_caching_strategy_utility_ (1)
{
  if (this->open (cleanup_s,
                  delete_cleanup_strategy,
                  utility_s,
                  delete_caching_strategy_utility) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_LFU_Caching_Strategy::ACE_LFU_Caching_Strategy")));

}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY>
ACE_LFU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::~ACE_LFU_Caching_Strategy (void)
{
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> int
ACE_LFU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::open (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                                                                                             int delete_cleanup_strategy,
                                                                                             CACHING_STRATEGY_UTILITY *utility_s,
                                                                                             int delete_caching_strategy_utility)
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

  // Initialise the caching strategy utility.

  // First we decide whether we need to clean up.
  if (this->caching_strategy_utility_ != 0 &&
      this->delete_caching_strategy_utility_ == 1 &&
      utility_s != 0)
    {
      delete this->caching_strategy_utility_;
      this->caching_strategy_utility_ = 0;
      this->delete_caching_strategy_utility_ = delete_caching_strategy_utility;
    }

  if (utility_s != 0)
    this->caching_strategy_utility_ = utility_s;
  else if (this->caching_strategy_utility_ == 0)
    {
      ACE_NEW_RETURN (this->caching_strategy_utility_,
                      CACHING_STRATEGY_UTILITY,
                      -1);

      this->delete_caching_strategy_utility_ = delete_caching_strategy_utility;
    }

  return 0;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> int
ACE_LFU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::clear_cache (CONTAINER &container)
{
  return this->caching_strategy_utility_->clear_cache (container,
                                                       this->cleanup_strategy_,
                                                       this->purge_percent_);
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> int
ACE_LFU_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::clear_cache (CONTAINER &container,
                                                                                                    unsigned int &total_container_entries)
{
  return this->caching_strategy_utility_->clear_cache (container,
                                                       this->cleanup_strategy_,
                                                       this->purge_percent_,
                                                       total_container_entries);
}

////////////////////////////////////////////////////////////////////////////////////////////////

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY>
ACE_FIFO_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::ACE_FIFO_Caching_Strategy (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                                                                                                                   int delete_cleanup_strategy,
                                                                                                                   CACHING_STRATEGY_UTILITY *utility_s,
                                                                                                                   int delete_caching_strategy_utility)
  : order_ (0),
    purge_percent_ (10),
    entries_ (0),
    cleanup_strategy_ (0),
    delete_cleanup_strategy_ (1),
    caching_strategy_utility_ (0),
    delete_caching_strategy_utility_ (1)
{
  if (this->open (cleanup_s,
                  delete_cleanup_strategy,
                  utility_s,
                  delete_caching_strategy_utility) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_FIFO_Caching_Strategy::ACE_FIFO_Caching_Strategy")));
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY>
ACE_FIFO_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::~ACE_FIFO_Caching_Strategy (void)

{
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> int
ACE_FIFO_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::open (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                                                                                              int delete_cleanup_strategy,
                                                                                              CACHING_STRATEGY_UTILITY *utility_s,
                                                                                              int delete_caching_strategy_utility)
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

  // Initialise the caching strategy utility.

  // First we decide whether we need to clean up.
  if (this->caching_strategy_utility_ != 0 &&
      this->delete_caching_strategy_utility_ == 1 &&
      utility_s != 0)
    {
      delete this->caching_strategy_utility_;
      this->caching_strategy_utility_ = 0;
      this->delete_caching_strategy_utility_ = delete_caching_strategy_utility;
    }

  if (utility_s != 0)
    this->caching_strategy_utility_ = utility_s;
  else if (this->caching_strategy_utility_ == 0)
    {
      ACE_NEW_RETURN (this->caching_strategy_utility_,
                      CACHING_STRATEGY_UTILITY,
                      -1);

      this->delete_caching_strategy_utility_ = delete_caching_strategy_utility;
    }

  return 0;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> int
ACE_FIFO_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::clear_cache (CONTAINER &container)
{
  return this->caching_strategy_utility_->clear_cache (container,
                                                       this->cleanup_strategy_,
                                                       this->purge_percent_);
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> int
ACE_FIFO_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::clear_cache (CONTAINER &container,
                                                                                                    unsigned int &total_container_entries)
{
  return this->caching_strategy_utility_->clear_cache (container,
                                                       this->cleanup_strategy_,
                                                       this->purge_percent_,
                                                       total_container_entries);
}

////////////////////////////////////////////////////////////////////////////////////////////////


template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY>
ACE_Null_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::ACE_Null_Caching_Strategy (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                                                                                                                   int delete_cleanup_strategy,
                                                                                                                   CACHING_STRATEGY_UTILITY *utility_s,
                                                                                                                   int delete_caching_strategy_utility)
{
  ACE_UNUSED_ARG (cleanup_s);
  ACE_UNUSED_ARG (delete_cleanup_strategy);
  ACE_UNUSED_ARG (utility_s);
  ACE_UNUSED_ARG (delete_caching_strategy_utility);
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> int
ACE_Null_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::open (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                                                                                              int delete_cleanup_strategy,
                                                                                              CACHING_STRATEGY_UTILITY *utility_s,
                                                                                              int delete_caching_strategy_utility)
{
  ACE_UNUSED_ARG (cleanup_s);
  ACE_UNUSED_ARG (delete_cleanup_strategy);
  ACE_UNUSED_ARG (utility_s);
  ACE_UNUSED_ARG (delete_caching_strategy_utility);

  return 0;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY>
ACE_Null_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::~ACE_Null_Caching_Strategy (void)
{
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> int
ACE_Null_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::clear_cache (CONTAINER &container)
{
  ACE_UNUSED_ARG (container);

  return 0;
}

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY> int
ACE_Null_Caching_Strategy<KEY, VALUE, CONTAINER, ATTRIBUTES, CACHING_STRATEGY_UTILITY>::clear_cache (CONTAINER &container,
                                                                                                     unsigned int &total_container_entries)
{
  ACE_UNUSED_ARG (container);
  ACE_UNUSED_ARG (total_container_entries);

  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ACE_ALLOC_HOOK_DEFINE(ACE_LRU_Caching_Strategy)
ACE_ALLOC_HOOK_DEFINE(ACE_LFU_Caching_Strategy)
ACE_ALLOC_HOOK_DEFINE(ACE_FIFO_Caching_Strategy)
ACE_ALLOC_HOOK_DEFINE(ACE_Null_Caching_Strategy)

#endif /* CACHING_STRATEGIES_T_C */
