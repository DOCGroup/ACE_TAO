//$Id$

#ifndef CLEANUP_STRATEGIES_T_C
#define CLEANUP_STRATEGIES_T_C

#include "ace/Cleanup_Strategies_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_RCSID(ace, Cleanup_Strategies_T, "$Id$")

////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER> int
ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER>::cleanup (CONTAINER &container,
                                                      KEY *key,
                                                      VALUE *value)
{
  ACE_UNUSED_ARG (value);

  return container.unbind (*key);
}

////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER> int
ACE_Recyclable_Handler_Cleanup_Strategy<KEY, VALUE, CONTAINER>::cleanup (CONTAINER &container,
                                                                         KEY *key,
                                                                         VALUE *)
{
  VALUE value;

  if (container.unbind (*key, value) == -1)
    return -1;

  value.first ()->recycler (0, 0);

  value.first ()->close ();

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER> int
ACE_Refcounted_Recyclable_Handler_Cleanup_Strategy<KEY, VALUE, CONTAINER>::cleanup (CONTAINER &,
                                                                                    KEY *,
                                                                                    VALUE *value)
{
  return value->first ()->handle_close_i ();
}

////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER> int
ACE_Handler_Cleanup_Strategy<KEY, VALUE, CONTAINER>::cleanup (CONTAINER &container,
                                                              KEY *key,
                                                              VALUE *value)
{
  // Remove the item from cache only if the handler isnt in use.
  if ((*value)->active () == 0)
    {
      (*value)->close ();

      if (container.unbind (*key) == -1)
        return -1;

    }

  return 0;
}

////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER> int
ACE_Null_Cleanup_Strategy<KEY, VALUE, CONTAINER>::cleanup (CONTAINER &container,
                                                           KEY *key,
                                                           VALUE *value)
{
  ACE_UNUSED_ARG (container);
  ACE_UNUSED_ARG (key);
  ACE_UNUSED_ARG (value);

  return 0;
}

#endif /* CLEANUP_STRATEGIES_T_C */
