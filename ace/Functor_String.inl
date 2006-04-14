/* -*- C++ -*- */
// $Id$

#include "ace/ACE.h"
#include "ace/String_Base.h"

ACE_INLINE unsigned long
ACE_Hash<ACE_CString>::operator () (const ACE_CString &t) const
{
  return t.hash ();
}


ACE_INLINE int
ACE_Equal_To<ACE_CString>::operator () (const ACE_CString &lhs,
                                        const ACE_CString &rhs) const
{
  return lhs == rhs;
}

ACE_INLINE int
ACE_Less_Than<ACE_CString>::operator () (const ACE_CString &lhs,
                                         const ACE_CString &rhs) const
{
  return (lhs < rhs);
}


ACE_INLINE unsigned long
ACE_Hash<ACE_WString>::operator () (const ACE_WString &t) const
{
  return t.hash ();
}


ACE_INLINE int
ACE_Equal_To<ACE_WString>::operator () (const ACE_WString &lhs,
                                        const ACE_WString &rhs) const
{
  return lhs == rhs;
}

ACE_INLINE int
ACE_Less_Than<ACE_WString>::operator () (const ACE_WString &lhs,
                                         const ACE_WString &rhs) const
{
  return (lhs < rhs);
}

