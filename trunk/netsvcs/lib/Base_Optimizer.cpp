// $Id$

#define ACE_BUILD_DLL

#include "Base_Optimizer.h"

template<class Base, class Member> ACE_INLINE
Base_Optimizer<Base, Member>::Base_Optimizer (void)
{
}

template<class Base, class Member> ACE_INLINE
Base_Optimizer<Base, Member>::Base_Optimizer (const Base &base,
                                              const Member &member)
  : Base (base),
    m_ (member)
{
}

template<class Base, class Member> ACE_INLINE
Base_Optimizer<Base, Member>::Base_Optimizer (const Base &base)
  : Base (base)
{
}

