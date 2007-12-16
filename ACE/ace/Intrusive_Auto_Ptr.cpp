// $Id$

#ifndef ACE_INTRUSIVE_AUTO_PTR_CPP
#define ACE_INTRUSIVE_AUTO_PTR_CPP


#include "ace/Intrusive_Auto_Ptr.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <class X>
ACE_Intrusive_Auto_Ptr<X>::~ACE_Intrusive_Auto_Ptr (void)
{
  reset (0);
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif  /* !ACE_INTRUSIVE_AUTO_PTR_CPP */
