// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Any_SystemException.h
 *
 *  $Id$
 *
 *  @authors  Carlos O'Ryan and Jeff Parsons
 */
//=============================================================================
#ifndef TAO_ANY_SYSTEMEXCEPTION_H
#define TAO_ANY_SYSTEMEXCEPTION_H

#include /**/ "ace/pre.h"

#include "tao/Any.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  typedef CORBA::SystemException* (*excp_factory)(void);

  /**
   * @class Any_Exception
   *
   * @brief Template Any class for IDL types with 2 modes of insertion
   *
   * Used for the IDL types that have copying and non-copying insertion,
   * but which are not (de)marshaled as pointers - struct, union, sequence,
   * and exception.
   */
  class Any_SystemException : public Any_Impl
  {
  public:
    Any_SystemException (_tao_destructor destructor,
                         CORBA::TypeCode_ptr,
                         CORBA::SystemException* const);

    Any_SystemException (_tao_destructor destructor,
                         CORBA::TypeCode_ptr,
                         const CORBA::SystemException &);

    Any_SystemException (CORBA::TypeCode_ptr);

    virtual ~Any_SystemException (void);

    static void insert (CORBA::Any &,
                        _tao_destructor,
                        CORBA::TypeCode_ptr,
                        CORBA::SystemException * const);

    static void insert_copy (CORBA::Any &,
                             _tao_destructor destructor,
                             CORBA::TypeCode_ptr,
                             const CORBA::SystemException &);

    static CORBA::Boolean extract (const CORBA::Any &,
                                   _tao_destructor,
                                   CORBA::TypeCode_ptr,
                                   const CORBA::SystemException *&,
                                   excp_factory f);

    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &);
    CORBA::Boolean demarshal_value (TAO_InputCDR &);

    virtual const void *value (void) const;
    virtual void free_value (void);

  protected:
    CORBA::SystemException *value_;
  };
};

#include /**/ "ace/post.h"
#endif /*TAO_ANY_SYSTEMEXCEPTION_H*/
