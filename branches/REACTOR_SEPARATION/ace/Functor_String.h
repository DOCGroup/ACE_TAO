// -*- C++ -*-

//==========================================================================
/**
 *  @file    Functor_String.h
 *
 *  $Id$
 *
 *   Class template specializations for ACE_*String types implementing
 *   function objects that are used in  various places in ATC. They
 *   could be placed in Functor.h. But we don't want to couple string
 *   types to the rest of ACE+TAO. Hence they are placed in a seperate
 *   file.
 *
 */
//==========================================================================
#ifndef ACE_FUNCTOR_STRING_H
#define ACE_FUNCTOR_STRING_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ACE_export.h"
#include "ace/SStringfwd.h"

////////////////////////////////////////////////////////////
// STL-style Functor Classes and Template Specializations //
////////////////////////////////////////////////////////////

// Forward declaration since we are going to specialize that template
// here. The template itself requires this file so every user of the
// template should also see the specialization.
template <class TYPE> class ACE_Hash;
template <class TYPE> class ACE_Equal_To;
template <class TYPE> class ACE_Less_Than;

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Equal_To<ACE_CString>
 *
 * @brief Function object for determining whether two ACE_CStrings are
 * equal.
 */
class ACE_Export ACE_Equal_To<ACE_CString>
{
public:
  int operator () (const ACE_CString &lhs,
                   const ACE_CString &rhs) const;
};


ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Hash<ACE_CString>
 *
 * @brief Function object for hashing a ACE_CString
 */
class ACE_Export ACE_Hash<ACE_CString>
{
public:
  /// Calls ACE::hash_pjw
  unsigned long operator () (const ACE_CString &lhs) const;
};


ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Less_Than<ACE_CString>
 *
 * @brief Function object for determining whether the first const string
 * is less than the second const string.
 */
class ACE_Export ACE_Less_Than<ACE_CString>
{
public:
  /// Simply calls ACE_OS::strcmp
  int operator () (const ACE_CString &lhs,
                   const ACE_CString &rhs) const;
};


#if defined (ACE_USES_WCHAR)

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Equal_To<ACE_WString>
 *
 * @brief Function object for determining whether two ACE_CStrings are
 * equal.
 */
class ACE_Export ACE_Equal_To<ACE_WString>
{
public:
  int operator () (const ACE_WString &lhs,
                   const ACE_WString &rhs) const;
};


ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Hash<ACE_WString>
 *
 * @brief Function object for hashing a ACE_WString
 */
class ACE_Export ACE_Hash<ACE_WString>
{
public:
  /// Calls ACE::hash_pjw
  unsigned long operator () (const ACE_WString &lhs) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Less_Than<ACE_WString>
 *
 * @brief Function object for determining whether the first const string
 * is less than the second const string.
 */
class ACE_Export ACE_Less_Than<ACE_WString>
{
public:
  /// Simply calls ACE_OS::strcmp
  int operator () (const ACE_WString &lhs,
                   const ACE_WString &rhs) const;
};

#endif /*ACE_USES_WCHAR*/
#if defined (__ACE_INLINE__)
#include "ace/Functor_String.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*ACE_FUNCTOR_STRING_H*/
