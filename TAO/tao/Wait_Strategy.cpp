#include "tao/Wait_Strategy.h"

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
TAO_Wait_Strategy::~TAO_Wait_Strategy ()
{
}

int
TAO_Wait_Strategy::defer_upcall (ACE_Event_Handler*)
{
  return -1;
}

int
TAO_Wait_Strategy::sending_request (TAO_ORB_Core *, TAO_Message_Semantics)
{
  return 0;
}

void
TAO_Wait_Strategy::finished_request ()
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
