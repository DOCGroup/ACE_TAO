//$Id$

#ifndef CLEANUP_STRATEGIES_T_C
#define CLEANUP_STRATEGIES_T_C

#define ACE_BUILD_DLL

#include "ace/Cleanup_Strategies_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Cached_Connect_Strategy_T.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

ACE_RCSID(ace, Cleanup_Strategies_T, "$Id$")

template <class CONTAINER>
ACE_Cleanup_Strategy<CONTAINER>::~ACE_Cleanup_Strategy (void)
{
}

////////////////////////////////////////////////////////////////////////////

template <class CONTAINER> int
ACE_Default_Cleanup_Strategy<CONTAINER>::cleanup (CONTAINER &container, 
                                                  KEY *key, 
                                                  VALUE *value)
{
  return container.unbind (*key);
}

////////////////////////////////////////////////////////////////////////////

template <class CONTAINER> int
ACE_Svc_Cleanup_Strategy<CONTAINER>::cleanup (CONTAINER &container, 
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

template <class CONTAINER> int
ACE_Null_Cleanup_Strategy<CONTAINER>::cleanup (CONTAINER &container, 
                                               KEY *key, 
                                               VALUE *value)
{
  return 0;
}

#endif /* CLEANUP_STRATEGIES_T_C */
