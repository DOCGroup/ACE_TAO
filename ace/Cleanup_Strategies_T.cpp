//$Id$

#ifndef CLEANUP_STRATEGIES_T_C
#define CLEANUP_STRATEGIES_T_C

#define ACE_BUILD_DLL

#include "ace/Cleanup_Strategies_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Cleanup_Strategies_T.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

ACE_RCSID(ace, Cleanup_Strategies_T, "$Id$")

template <class KEY, class VALUE, class CONTAINER>
ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER>::~ACE_Cleanup_Strategy (void)
{
}

////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER> int
ACE_Default_Cleanup_Strategy<KEY, VALUE, CONTAINER>::cleanup (CONTAINER &container, 
                                                              KEY *key, 
                                                              VALUE *value)
{
  return container.unbind (*key);
}

////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER> int
ACE_Svc_Cleanup_Strategy<KEY, VALUE, CONTAINER>::cleanup (CONTAINER &container, 
                                                          KEY *key, 
                                                          VALUE *value)
{
  (value->first ())->recycler (0, 0);

  (value->first ())->close ();
  
  if (container.unbind (*key) == -1)
    return -1;
   
  return 0; 
}

/////////////////////////////////////////////////////////////////////////////
template <class KEY, class VALUE, class CONTAINER> int
ACE_Handler_Cleanup_Strategy<KEY, VALUE, CONTAINER>::cleanup (CONTAINER &container, 
                                                              KEY *key, 
                                                              VALUE *value)
{
  // Remove the item from cache only if the handler isnt in use.
  if ((*value)->active () == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "handle %d\n",
                  (*value)->get_handle ()));
      (*value)->close ();

      ACE_DEBUG ((LM_DEBUG, "LRU: before unbind: current_size %d\n", container.current_size ()));
   
      if (container.unbind (*key) == -1)
        return -1;

      ACE_DEBUG ((LM_DEBUG, "LRU:after unbind: current_size %d\n", container.current_size ()));
    }

  return 0;

}

////////////////////////////////////////////////////////////////////////////
template <class KEY, class VALUE, class CONTAINER> int
ACE_Null_Cleanup_Strategy<KEY, VALUE, CONTAINER>::cleanup (CONTAINER &container, 
                                                           KEY *key, 
                                                           VALUE *value)
{
  return 0;
}

#endif /* CLEANUP_STRATEGIES_T_C */
