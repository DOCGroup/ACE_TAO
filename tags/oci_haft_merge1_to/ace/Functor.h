// -*- C++ -*-

//==========================================================================
/**
 *  @file    Functor.h
 *
 *  $Id$
 *
 *   Non-templatized classes and class template specializations for
 *   implementing function objects that are used in  various places
 *   in ACE.  There are currently two major categories of function
 *   objects in ACE: GoF Command Pattern objects, and STL-style
 *   functors for comparison of container elements.  The command objects
 *   are invoked via an execute () method, while the STL-style functors are
 *   invoked via an operator() () method.
 *  Non-templatized classes for implementing the GoF Command Pattern,
 *  also known as functors or function objects.
 *
 *
 *  @author Chris Gill <cdgill@cs.wustl.edu>
 *  @author Based on Command Pattern implementations originally done by
 *  @author Carlos O'Ryan <coryan@cs.wustl.edu>
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 *  @author and on STL-style functor implementations originally done by
 *  @author Irfan Pyarali  <irfan@cs.wustl.edu>
 */
//==========================================================================


#ifndef ACE_FUNCTOR_H
#define ACE_FUNCTOR_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ACE_export.h"
#include "ace/Basic_Types.h"

//////////////////////////////////////////////////////////////
// GOF Command Pattern Classes and Template Specializations //
//////////////////////////////////////////////////////////////

/**
 * @class ACE_Command_Base
 *
 * @brief Defines an abstract class that allows us to invoke commands
 * without knowing anything about the implementation.
 *
 * This class declares an interface to execute a command
 * independent of the effect of the command, or the objects used
 * to implement it.
 */
class ACE_Export ACE_Command_Base
{
public:
  // = Initialization and termination methods.
  /// Default constructor.
  ACE_Command_Base (void);

  /// Virtual destructor.
  virtual ~ACE_Command_Base (void);

  /**
   * Invokes the method encapsulated by the command, passing along the
   * passed argument (if any).  Users of classes derived from this
   * class must ensure that the resulting invocation can tolerate a
   * null void pointer being passed, or otherwise ensure that this
   * will never occur.
   */
  virtual int execute (void *arg = 0) = 0;
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
/**
 * @class ACE_Hash<char>
 *
 * @brief Function object for hashing a char
 */
class ACE_Export ACE_Hash<char>
{
public:
  /// Simply returns t
  unsigned long operator () (char t) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Hash<signed
 *
 * @brief Function object for hashing a signed char
 */
class ACE_Export ACE_Hash<signed char>
{
public:
  /// Simply returns t
  unsigned long operator () (signed char t) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Hash<unsigned
 *
 * @brief Function object for hashing an unsigned char
 */
class ACE_Export ACE_Hash<unsigned char>
{
public:
  /// Simply returns t
  unsigned long operator () (unsigned char t) const;
};

// @@ ADD HASHES FOR ACE TYPES

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Hash<ACE_INT16>
 *
 * @brief Function object for hashing a 16-bit signed number
 */
class ACE_Export ACE_Hash<ACE_INT16>
{
public:
  /// Simply returns t
  unsigned long operator () (ACE_INT16 t) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Hash<ACE_UINT16>
 *
 * @brief Function object for hashing a 16-bit unsigned number
 */
class ACE_Export ACE_Hash<ACE_UINT16>
{
public:
  /// Simply returns t
  unsigned long operator () (ACE_UINT16 t) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Hash<ACE_INT32>
 *
 * @brief Function object for hashing a 32-bit signed number
 */
class ACE_Export ACE_Hash<ACE_INT32>
{
public:
  /// Simply returns t
  unsigned long operator () (ACE_INT32 t) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Hash<ACE_UINT32>
 *
 * @brief Function object for hashing a 32-bit unsigned number
 */
class ACE_Export ACE_Hash<ACE_UINT32>
{
public:
  /// Simply returns t
  unsigned long operator () (ACE_UINT32 t) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Hash<ACE_UINT64>
 *
 * @brief Function object for hashing a 64-bit unsigned number
 */
class ACE_Export ACE_Hash<ACE_UINT64>
{
public:
  /// Simply returns t
  unsigned long operator () (ACE_UINT64 t) const;
};

// @@ DONE ADDING HASHES FOR ACE TYPES

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Hash<const
 *
 * @brief Function object for hashing a const string
 */
class ACE_Export ACE_Hash<const ACE_TCHAR *>
{
public:
  /// Calls ACE::hash_pjw
  unsigned long operator () (const ACE_TCHAR *t) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Hash<ACE_TCHAR
 *
 * @brief Function object for hashing a string
 */
class ACE_Export ACE_Hash<ACE_TCHAR *>
{
public:
  /// Calls ACE::hash_pjw
  unsigned long operator () (const ACE_TCHAR *t) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Hash<void *>
 *
 * @brief Function object for hashing a void *
 */
class ACE_Export ACE_Hash<void *>
{
public:
  unsigned long operator () (const void *) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Equal_To<const
 *
 * @brief Function object for determining whether two const strings are equal.
 */
class ACE_Export ACE_Equal_To<const ACE_TCHAR *>
{
public:
  /// Simply calls ACE_OS::strcmp
  int operator () (const ACE_TCHAR *lhs,
                   const ACE_TCHAR *rhs) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Equal_To<ACE_TCHAR
 *
 * @brief Function object for determining whether two non-const
 * strings are equal.
 */
class ACE_Export ACE_Equal_To<ACE_TCHAR *>
{
public:
  /// Simply calls ACE_OS::strcmp
  int operator () (const ACE_TCHAR *lhs,
                   const ACE_TCHAR *rhs) const;
};

 ACE_TEMPLATE_SPECIALIZATION
 /**
 * @class ACE_Equal_To<ACE_UINT16>
 *
 * @brief Function object for determining whether two unsigned
 * 16 bit ints are equal.
 */
class ACE_Export ACE_Equal_To<ACE_UINT16>
{
public:
  /// Simply calls built-in operators
  int operator () (const ACE_UINT16 lhs,
                   const ACE_UINT16 rhs) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Equal_To<ACE_INT16>
 *
 * @brief Function object for determining whether two
 * 16 bit ints are equal.
 */
class ACE_Export ACE_Equal_To<ACE_INT16>
{
public:
  /// Simply calls built-in operators
  int operator () (const ACE_INT16 lhs,
                   const ACE_INT16 rhs) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Equal_To<ACE_UINT32>
 *
 * @brief Function object for determining whether two unsigned
 * 32 bit ints are equal.
 */
class ACE_Export ACE_Equal_To<ACE_UINT32>
{
public:
  /// Simply calls built-in operators
  int operator () (const ACE_UINT32 lhs,
                   const ACE_UINT32 rhs) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Equal_To<ACE_INT32>
 *
 * @brief Function object for determining whether two
 * 32 bit ints are equal.
 */
class ACE_Export ACE_Equal_To<ACE_INT32>
{
public:
  /// Simply calls built-in operators
  int operator () (const ACE_INT32 lhs,
                   const ACE_INT32 rhs) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Equal_To<ACE_UINT64>
 *
 * @brief Function object for determining whether two unsigned
 * 64 bit ints are equal.
 */
class ACE_Export ACE_Equal_To<ACE_UINT64>
{
public:
  /// Simply calls built-in operators
  int operator () (const ACE_UINT64 lhs,
                   const ACE_UINT64 rhs) const;
};


ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Less_Than<const
 *
 * @brief Function object for determining whether the first const string
 * is less than the second const string.
 */
class ACE_Export ACE_Less_Than<const ACE_TCHAR *>
{
public:
  /// Simply calls ACE_OS::strcmp
  int operator () (const ACE_TCHAR *lhs,
                   const ACE_TCHAR *rhs) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Less_Than<ACE_TCHAR
 *
 * @brief Function object for determining whether the first string
 * is less than the second string.
 */
class ACE_Export ACE_Less_Than<ACE_TCHAR *>
{
public:
  /// Simply calls ACE_OS::strcmp
  int operator () (const ACE_TCHAR *lhs,
                   const ACE_TCHAR *rhs) const;
};

#if defined (ACE_USES_WCHAR)

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Less_Than<const
 *
 * @brief Function object for determining whether the first const string
 * is less than the second const string.
 */
class ACE_Export ACE_Less_Than<const ACE_ANTI_TCHAR *>
{
public:
  /// Simply calls ACE_OS::strcmp
  int operator () (const ACE_ANTI_TCHAR *lhs,
                   const ACE_ANTI_TCHAR *rhs) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Less_Than<ACE_ANTI_TCHAR
 *
 * @brief Function object for determining whether the first string
 * is less than the second string.
 */
class ACE_Export ACE_Less_Than<ACE_ANTI_TCHAR *>
{
public:
  /// Simply calls ACE_OS::strcmp
  int operator () (const ACE_ANTI_TCHAR *lhs,
                   const ACE_ANTI_TCHAR *rhs) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Hash<const
 *
 * @brief Function object for hashing a const string
 */
class ACE_Export ACE_Hash<const ACE_ANTI_TCHAR *>
{
public:
  /// Calls ACE::hash_pjw
  unsigned long operator () (const ACE_ANTI_TCHAR *t) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Hash<ACE_ANTI_TCHAR
 *
 * @brief Function object for hashing a string
 */
class ACE_Export ACE_Hash<ACE_ANTI_TCHAR *>
{
public:
  /// Calls ACE::hash_pjw
  unsigned long operator () (const ACE_ANTI_TCHAR *t) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Equal_To<const
 *
 * @brief Function object for determining whether two const strings are equal.
 */
class ACE_Export ACE_Equal_To<const ACE_ANTI_TCHAR *>
{
public:
  /// Simply calls ACE_OS::strcmp
  int operator () (const ACE_ANTI_TCHAR *lhs,
                   const ACE_ANTI_TCHAR *rhs) const;
};

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Equal_To<ACE_ANTI_TCHAR
 *
 * @brief Function object for determining whether two non-const
 * strings are equal.
 */
class ACE_Export ACE_Equal_To<ACE_ANTI_TCHAR *>
{
public:
  /// Simply calls ACE_OS::strcmp
  int operator () (const ACE_ANTI_TCHAR *lhs,
                   const ACE_ANTI_TCHAR *rhs) const;
};

#endif  // ACE_USES_WCHAR

#if defined (__ACE_INLINE__)
#include "ace/Functor.i"
#endif /* __ACE_INLINE__ */

// Include the templates here.
#include "ace/Functor_T.h"

#include "ace/post.h"
#endif /* ACE_FUNCTOR_H */
