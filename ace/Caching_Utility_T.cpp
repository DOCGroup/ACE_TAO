//$Id$

#ifndef CACHING_UTILITY_T_C
#define CACHING_UTILITY_T_C

#include "ace/Caching_Utility_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Cleanup_Strategies_T.h"

#if !defined (__ACE_INLINE__)
#include "ace/Caching_Utility_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Caching_Utility_T, "$Id$")

/////////////////////////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER, class ITERATOR, class ATTRIBUTES> int
ACE_Pair_Caching_Utility<KEY, VALUE, CONTAINER, ITERATOR, ATTRIBUTES>::clear_cache (CONTAINER &container,
                                                                                    ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                                                                                    double purge_percent)
{
  // Check that the purge_percent is non-zero.
  if (purge_percent == 0)
    return 0;

  // Get the number of entries in the container.
  size_t current_map_size = container.current_size ();

  // Also whether the number of entries in the cache is just one!
  // Oops! then there is no way out but exiting. So return an error.
  if (current_map_size <= 1)
    return 0;

  // Calculate the no of entries to remove from the cache depending
  // upon the <purge_percent>.
  size_t entries_to_remove = (purge_percent / 100 * current_map_size) + 0.5;

  KEY *key_to_remove = 0;
  VALUE *value_to_remove = 0;

  for (size_t i = 0; i < entries_to_remove ; ++i)
    {
      this->minimum (container,
                     key_to_remove,
                     value_to_remove);

      if (cleanup_s->cleanup (container,
                              key_to_remove,
                              value_to_remove) == -1)
        return -1;
    }

  return 0;
}

template <class KEY, class VALUE, class CONTAINER, class ITERATOR, class ATTRIBUTES> void
ACE_Pair_Caching_Utility<KEY, VALUE, CONTAINER, ITERATOR, ATTRIBUTES>::minimum (CONTAINER &container,
                                                                                KEY *&key_to_remove,
                                                                                VALUE *&value_to_remove)
{
  // Starting values.
  ITERATOR iter = container.begin ();
  ITERATOR end = container.end ();
  ATTRIBUTES min = (*iter).int_id_.second ();
  key_to_remove = &(*iter).ext_id_;
  value_to_remove = &(*iter).int_id_;

  // The iterator moves thru the container searching for the entry
  // with the lowest ATTRIBUTES.
  for (++iter;
       iter != end;
       ++iter)
    {
      if (min > (*iter).int_id_.second ())
        {
          // Ah! an item with lower ATTTRIBUTES...
          min = (*iter).int_id_.second ();
          key_to_remove = &(*iter).ext_id_;
          value_to_remove = &(*iter).int_id_;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER, class ITERATOR, class ATTRIBUTES> int
ACE_Handler_Caching_Utility<KEY, VALUE, CONTAINER, ITERATOR, ATTRIBUTES>::clear_cache (CONTAINER &container,
                                                                                       ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                                                                                       double purge_percent)
{
  // Check that the purge_percent is non-zero.
  if (purge_percent == 0)
    return 0;

  // Get the number of entries in the container.
  size_t current_map_size = container.current_size ();

  // Also whether the number of entries in the cache is just one!
  // Oops! then there is no way out but exiting. So return an error.
  if (current_map_size <= 1)
    return 0;

  // Calculate the no of entries to remove from the cache depending
  // upon the <purge_percent>.
  size_t entries_to_remove = (purge_percent / 100 * current_map_size) + 0.5;

  KEY *key_to_remove = 0;
  VALUE *value_to_remove = 0;

  for (size_t i = 0; i < entries_to_remove ; ++i)
    {
      this->minimum (container,
                     key_to_remove,
                     value_to_remove);

      if (cleanup_s->cleanup (container,
                              key_to_remove,
                              value_to_remove) == -1)
        return -1;
    }

  return 0;
}

template <class KEY, class VALUE, class CONTAINER, class ITERATOR, class ATTRIBUTES> void
ACE_Handler_Caching_Utility<KEY, VALUE, CONTAINER, ITERATOR, ATTRIBUTES>::minimum (CONTAINER &container,
                                                                                   KEY *&key_to_remove,
                                                                                   VALUE *&value_to_remove)
{
  // Starting values.
  ITERATOR iter = container.begin ();
  ITERATOR end = container.end ();
  ATTRIBUTES min = (*iter).int_id_->caching_attributes ();
  key_to_remove = &(*iter).ext_id_;
  value_to_remove = &(*iter).int_id_;

  // The iterator moves thru the container searching for the entry
  // with the lowest ATTRIBUTES.
  for (++iter;
       iter != end;
       ++iter)
    {
      if (min > (*iter).int_id_->caching_attributes () &&
          (*iter).int_id_->active () != 1)
        {
          // Ah! an item with lower ATTTRIBUTES...
          min = (*iter).int_id_->caching_attributes ();
          key_to_remove = &(*iter).ext_id_;
          value_to_remove = &(*iter).int_id_;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER, class ITERATOR, class ATTRIBUTES> int
ACE_Null_Caching_Utility<KEY, VALUE, CONTAINER, ITERATOR, ATTRIBUTES>::clear_cache (CONTAINER &container,
                                                                                    ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                                                                                    double purge_percent)
{
  ACE_UNUSED_ARG (container);
  ACE_UNUSED_ARG (cleanup_s);
  ACE_UNUSED_ARG (purge_percent);

  return 0;
}

template <class KEY, class VALUE, class CONTAINER, class ITERATOR, class ATTRIBUTES> void
ACE_Null_Caching_Utility<KEY, VALUE, CONTAINER, ITERATOR, ATTRIBUTES>::minimum (CONTAINER &container,
                                                                                KEY *&key_to_remove,
                                                                                VALUE *&value_to_remove)
{
  ACE_UNUSED_ARG (container);
  ACE_UNUSED_ARG (key_to_remove);
  ACE_UNUSED_ARG (value_to_remove);
}

#endif /* CACHING_UTILITY_T_C */
