// $Id$

#include "Assembly_Spec.h"

#if !defined (__ACE_INLINE__)
# include "Assembly_Spec.inl"
#endif /* __ACE_INLINE__ */


// ================================================================

int
CIAO::Assembly_Placement::Node::accept
(CIAO::Assembly_Placement::Visitor &visitor)
{
  // This should have never gotten called.
  ACE_ASSERT (0);
  return -1;
}

CIAO::Assembly_Placement::Node::~Node ()
{
}

// ================================================================

int
CIAO::Assembly_Placement::componentinstantiation::accept
(CIAO::Assembly_Placement::Visitor &visitor)
{
  return visitor.visit_componentinstantiation (this);
}

CIAO::Assembly_Placement::componentinstantiation::~componentinstantiation ()
{
}

// ================================================================

int
CIAO::Assembly_Placement::homeplacement::accept
(CIAO::Assembly_Placement::Visitor &visitor)
{
  return visitor.visit_homeplacement (this);
}

CIAO::Assembly_Placement::homeplacement::~homeplacement ()
{
}

// ================================================================

int
CIAO::Assembly_Placement::hostcollocation::accept
(CIAO::Assembly_Placement::Visitor &visitor)
{
  return visitor.visit_hostcollocation (this);
}

CIAO::Assembly_Placement::hostcollocation::~hostcollocation ()
{
}

// ================================================================

int
CIAO::Assembly_Placement::processcollocation::accept
(CIAO::Assembly_Placement::Visitor &visitor)
{
  return visitor.visit_processcollocation (this);
}

CIAO::Assembly_Placement::processcollocation::~processcollocation ()
{
}

// ================================================================
