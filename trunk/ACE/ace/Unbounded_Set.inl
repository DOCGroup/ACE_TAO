// -*- C++ -*-
//
// $Id$

#include "ace/Global_Macros.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename T> ACE_INLINE bool
ACE_Unbounded_Set_Default_Comparator<T>::operator () (const T  &lhs, const T &rhs) const
{
  return lhs == rhs;
}

template<typename T>
ACE_Unbounded_Set_Iterator<T>::ACE_Unbounded_Set_Iterator (ACE_Unbounded_Set<T> &s, 
                                                           bool end)
  : BASE (s, end)
{
}

template<typename T>
ACE_Unbounded_Set_Iterator<T>::ACE_Unbounded_Set_Iterator (const BASE & s)
  : BASE  (s)
{
}

template<typename T>
ACE_Unbounded_Set_Const_Iterator<T>::
ACE_Unbounded_Set_Const_Iterator (const ACE_Unbounded_Set<T> &s,
                                  bool end)
  : BASE (s, end)
{
}

template<typename T>
ACE_Unbounded_Set_Const_Iterator<T>::ACE_Unbounded_Set_Const_Iterator (const BASE & s)
  : BASE  (s)
{
}

template<typename T>
ACE_Unbounded_Set<T>::ACE_Unbounded_Set (ACE_Allocator *alloc)
  : ACE_Unbounded_Set_Ex<T, ACE_Unbounded_Set_Default_Comparator<T> > (alloc)
{
}


ACE_END_VERSIONED_NAMESPACE_DECL
