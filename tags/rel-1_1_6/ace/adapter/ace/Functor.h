// $Id$

#ifndef ACE_ADAPTER_FUNCTOR_H
#define ACE_ADAPTER_FUNCTOR_H
#include "ace/pre.h"

#include "../../Functor.h"

ACE_TEMPLATE_SPECIALIZATION
class ACE_Hash<const wchar_t *>
{
  // = TITLE
  //     Function object for hashing a const string
public:
  u_long operator () (const wchar_t *t) const
  { return ACE::hash_pjw  (t); }
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Hash<wchar_t *>
{
  // = TITLE
  //     Function object for hashing a string
public:
  u_long operator () (const wchar_t *t) const
  { return ACE::hash_pjw  (t); }
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Equal_To<const wchar_t *>
{
  // = TITLE
  //     Function object for determining whether two const strings are equal.
public:
  int operator () (const wchar_t *lhs,
                   const wchar_t *rhs) const
  { return ACE_OS::strcmp (lhs, rhs) == 0; }
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Export ACE_Equal_To<wchar_t *>
{
  // = TITLE
  //     Function object for determining whether two non-const
  //     strings are equal.
public:
  int operator () (const wchar_t *lhs,
                   const wchar_t *rhs) const
  { return ACE_OS::strcmp (lhs, rhs) == 0; }
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Export ACE_Less_Than<const wchar_t *>
{
  // = TITLE
  //     Function object for determining whether the first const string
  //     is less than the second const string.
public:
  int operator () (const wchar_t *lhs,
                   const wchar_t *rhs) const
  { return ACE_OS::strcmp (lhs, rhs) == 0; }
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Export ACE_Less_Than<wchar_t *>
{
  // = TITLE
  //     Function object for determining whether the first string
  //     is less than the second string.
public:
  int operator () (const wchar_t *lhs,
                   const wchar_t *rhs) const
  { return ACE_OS::strcmp (lhs, rhs) == 0; }
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_FUNCTOR_H */
