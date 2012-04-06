// -*- C++ -*-

//=============================================================================
/**
 *  @file    Exception_Data.h
 *
 *  $Id$
 *
 *  @author  balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_EXCEPTION_DATA_H
#define TAO_EXCEPTION_DATA_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class TypeCode;
  typedef TypeCode *TypeCode_ptr;

  class Exception;
}

namespace TAO
{
  // Function pointer returning a pointer to CORBA::Exception. This is used to
  // describe the allocator for user-defined exceptions that are used internally
  // by the interpreter.
  typedef CORBA::Exception* (*TAO_Exception_Alloc) (void);

  /**
   * @struct Exception_Data
   *
   * @brief Description of a single exception.
   *
   * The interpreter needs a way to allocate memory to hold the exception
   * that was raised by the stub. This data structure provides the typecode
   * for the exception as well as a static function pointer that
   * does the job of memory allocation.
   */
  struct Exception_Data
  {
    /// Repository id of the exception.
    const char *id;

    /// The allocator for this exception.
    TAO_Exception_Alloc alloc;

#if TAO_HAS_INTERCEPTORS == 1
    /// The typecode pointer for this exception.
    CORBA::TypeCode_ptr tc_ptr;
#endif /* TAO_HAS_INTERCEPTORS */
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /*TAO_EXCEPTION_DATA_H*/
