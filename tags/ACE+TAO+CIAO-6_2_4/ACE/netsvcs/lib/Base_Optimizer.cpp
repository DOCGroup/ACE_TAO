// $Id$

#if !defined (BASE_OPTIMIZER_CPP)
#define BASE_OPTIMIZER_CPP

#include "Base_Optimizer.h"

template<class Base, class Member>
Base_Optimizer<Base, Member>::Base_Optimizer (void)
{
}

template<class Base, class Member>
Base_Optimizer<Base, Member>::Base_Optimizer (const Base &base,
                                              const Member &member)
  : Base (base),
    m_ (member)
{
}

template<class Base, class Member>
Base_Optimizer<Base, Member>::Base_Optimizer (const Base &base)
  : Base (base)
{
}

#endif /* BASE_OPTIMIZER_CPP */
