// $Id$

#ifndef ACE_REFCOUNTABLE_T_CPP
#define ACE_REFCOUNTABLE_T_CPP

#include "ace/Refcountable_T.h"

ACE_RCSID (ace,
           Refcountable_T,
           "$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <class ACE_LOCK>
ACE_Refcountable_T<ACE_LOCK>::~ACE_Refcountable_T (void)
{
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif  /* !ACE_REFCOUNTABLE_T_CPP */
