// $Id$

#include "Assembly_Spec.h"

#if !defined (__ACE_INLINE__)
# include "Assembly_Spec.inl"
#endif /* __ACE_INLINE__ */


// ================================================================

int
CIAO::Assembly_Placement::Node::accept
(CIAO::Assembly_Placement::Visitor &
 ACE_ENV_ARG_DECL)
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
CIAO::Assembly_Placement::Container::accept
(CIAO::Assembly_Placement::Visitor &visitor
 ACE_ENV_ARG_DECL)
{
  return visitor.visit_Container (this
                                  ACE_ENV_ARG_PARAMETER);
}

// ================================================================

int
CIAO::Assembly_Placement::componentinstantiation::accept
(CIAO::Assembly_Placement::Visitor &visitor
 ACE_ENV_ARG_DECL)
{
  return visitor.visit_componentinstantiation (this
                                               ACE_ENV_ARG_PARAMETER);
}

CIAO::Assembly_Placement::componentinstantiation::~componentinstantiation ()
{
}

// ================================================================

int
CIAO::Assembly_Placement::homeplacement::accept
(CIAO::Assembly_Placement::Visitor &visitor
 ACE_ENV_ARG_DECL)
{
  return visitor.visit_homeplacement (this
                                      ACE_ENV_ARG_PARAMETER);
}

CIAO::Assembly_Placement::homeplacement::~homeplacement ()
{
}

// ================================================================

int
CIAO::Assembly_Placement::hostcollocation::accept
(CIAO::Assembly_Placement::Visitor &visitor
 ACE_ENV_ARG_DECL)
{
  return visitor.visit_hostcollocation (this
                                        ACE_ENV_ARG_PARAMETER);
}

CIAO::Assembly_Placement::hostcollocation::~hostcollocation ()
{
}

// ================================================================

int
CIAO::Assembly_Placement::processcollocation::accept
(CIAO::Assembly_Placement::Visitor &visitor
 ACE_ENV_ARG_DECL)
{
  return visitor.visit_processcollocation (this
                                           ACE_ENV_ARG_PARAMETER);
}

CIAO::Assembly_Placement::processcollocation::~processcollocation ()
{
}

// ================================================================

CIAO::Assembly_Placement::Visitor::~Visitor (void)
{
}
