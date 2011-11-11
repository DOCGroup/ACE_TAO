// $Id$

#include "tao/Wait_Strategy.h"

//@@ WAIT_STRATEGY_SPL_COPY_HOOK_START

#if !defined (__ACE_INLINE__)
# include "tao/Wait_Strategy.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Constructor.
TAO_Wait_Strategy::TAO_Wait_Strategy (TAO_Transport *transport)
  : transport_ (transport),
    is_registered_ (false)
{
}

// Destructor.
TAO_Wait_Strategy::~TAO_Wait_Strategy (void)
{
}

int
TAO_Wait_Strategy::defer_upcall (ACE_Event_Handler*)
{
  return -1;
}

/*
 * Hook to comment out this method in the lf wait strategy
 */
//@@ LF_WAIT_STRATEGY_SPL_COMMENT_HOOK_START
int
TAO_Wait_Strategy::sending_request (TAO_ORB_Core *, int)
{
  return 0;
}
//@@ LF_WAIT_STRATEGY_SPL_COMMENT_HOOK_END

/*
 * Hook to specialize the Wait Strategy
 */
//@@ TAO_WAIT_STRATEGY_SPL_ADD_HOOK

TAO_END_VERSIONED_NAMESPACE_DECL
