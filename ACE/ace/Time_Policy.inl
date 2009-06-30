// -*- C++ -*-
//
// $Id$

#include "ace/OS_NS_sys_time.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE ACE_Time_Value
ACE_Default_Time_Policy::operator()() const
{
  return ACE_OS::gettimeofday();
}

ACE_INLINE
ACE_FPointer_Time_Policy::ACE_FPointer_Time_Policy()
  : function_(ACE_OS::gettimeofday)
{
}

ACE_INLINE
ACE_FPointer_Time_Policy::
ACE_FPointer_Time_Policy(ACE_FPointer_Time_Policy::FPtr f)
  : function_(f)
{
}

ACE_INLINE ACE_Time_Value
ACE_FPointer_Time_Policy::operator()() const
{
  return (*this->function_)();
}

ACE_END_VERSIONED_NAMESPACE_DECL
