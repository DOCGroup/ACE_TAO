/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Functor.h
//
// = DESCRIPTION
//     Non-templatized classes and class template specializations for
//     implementing function objects that are used in  various places
//     in ACE.  There are currently two major categories of function
//     objects in ACE: GoF Command Pattern objects, and STL-style
//     functors for comparison of container elements.  The command objects
//     are invoked via an execute () method, while the STL-style functors are
//     invoked via an operator() () method.
//    Non-templatized classes for implementing the GoF Command Pattern,
//    also known as functors or function objects.
//
// = AUTHOR
//    Chris Gill           <cdgill@cs.wustl.edu>
//
//    Based on Command Pattern implementations originally done by
//
//    Carlos O'Ryan        <coryan@cs.wustl.edu>  and
//    Douglas C. Schmidt   <schmidt@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
//    and on STL-style functor implementations originally done by
//
//    Irfan Pyarali  <irfan@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_FUNCTOR_H
#define ACE_FUNCTOR_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//////////////////////////////////////////////////////////////
// GOF Command Pattern Classes and Template Specializations //
//////////////////////////////////////////////////////////////

class ACE_Export ACE_Command_Base
{
  // = TITLE
  //    Defines an abstract class that allows us to invoke commands
  //    without knowing anything about the implementation.
  //
  // = DESCRIPTION
  //    This class declares an interface to execute a command
  //    independent of the effect of the command, or the objects used
  //    to implement it.
public:
  // = Initialization and termination methods.
  ACE_Command_Base (void);
  // Default constructor.

  virtual ~ACE_Command_Base (void);
  // Virtaul destructor.

  virtual int execute (void *arg = 0) = 0;
  // Invokes the method encapsulated by the command, passing along the
  // passed argument (if any).  Users of classes derived from this
  // class must ensure that the resulting invocation can tolerate a
  // null void pointer being passed, or otherwise ensure that this
  // will never occur.
};

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
class ACE_Export ACE_Hash<char>
{
  // = TITLE
  //     Function object for hashing a char
public:
  u_long operator () (char t) const;
  // Simply returns t
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Export ACE_Hash<signed char>
{
  // = TITLE
  //     Function object for hashing a signed char
public:
  u_long operator () (signed char t) const;
  // Simply returns t
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Export ACE_Hash<unsigned char>
{
  // = TITLE
  //     Function object for hashing an unsigned char
public:
  u_long operator () (unsigned char t) const;
  // Simply returns t
};

// @@ ADD HASHES FOR ACE TYPES

ACE_TEMPLATE_SPECIALIZATION
class ACE_Export ACE_Hash<ACE_INT16>
{
  // = TITLE
  //     Function object for hashing a 16-bit signed number
public:
  u_long operator () (ACE_INT16 t) const;
  // Simply returns t
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Export ACE_Hash<ACE_UINT16>
{
  // = TITLE
  //     Function object for hashing a 16-bit unsigned number
public:
  u_long operator () (ACE_UINT16 t) const;
  // Simply returns t
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Export ACE_Hash<ACE_INT32>
{
  // = TITLE
  //     Function object for hashing a 32-bit signed number
public:
  u_long operator () (ACE_INT32 t) const;
  // Simply returns t
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Export ACE_Hash<ACE_UINT32>
{
  // = TITLE
  //     Function object for hashing a 32-bit unsigned number
public:
  u_long operator () (ACE_UINT32 t) const;
  // Simply returns t
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Export ACE_Hash<ACE_UINT64>
{
  // = TITLE
  //     Function object for hashing a 64-bit unsigned number
public:
  u_long operator () (ACE_UINT64 t) const;
  // Simply returns t
};

// @@ DONE ADDING HASHES FOR ACE TYPES

ACE_TEMPLATE_SPECIALIZATION
class ACE_Export ACE_Hash<const ACE_TCHAR *>
{
  // = TITLE
  //     Function object for hashing a const string
public:
  u_long operator () (const ACE_TCHAR *t) const;
  // Calls ACE::hash_pjw
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Export ACE_Hash<ACE_TCHAR *>
{
  // = TITLE
  //     Function object for hashing a string
public:
  u_long operator () (const ACE_TCHAR *t) const;
  // Calls ACE::hash_pjw
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Export ACE_Equal_To<const ACE_TCHAR *>
{
  // = TITLE
  //     Function object for determining whether two const strings are equal.
public:
  int operator () (const ACE_TCHAR *lhs,
                   const ACE_TCHAR *rhs) const;
  // Simply calls ACE_OS::strcmp
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Export ACE_Equal_To<ACE_TCHAR *>
{
  // = TITLE
  //     Function object for determining whether two non-const
  //     strings are equal.
public:
  int operator () (const ACE_TCHAR *lhs,
                   const ACE_TCHAR *rhs) const;
  // Simply calls ACE_OS::strcmp
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Export ACE_Less_Than<const ACE_TCHAR *>
{
  // = TITLE
  //     Function object for determining whether the first const string
  //     is less than the second const string.
public:
  int operator () (const ACE_TCHAR *lhs,
                   const ACE_TCHAR *rhs) const;
  // Simply calls ACE_OS::strcmp
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Export ACE_Less_Than<ACE_TCHAR *>
{
  // = TITLE
  //     Function object for determining whether the first string
  //     is less than the second string.
public:
  int operator () (const ACE_TCHAR *lhs,
                   const ACE_TCHAR *rhs) const;
  // Simply calls ACE_OS::strcmp
};

#if defined (__ACE_INLINE__)
#include "ace/Functor.i"
#endif /* __ACE_INLINE__ */

// Include the templates here.
#include "ace/Functor_T.h"

#include "ace/post.h"
#endif /* ACE_FUNCTOR_H */
