// -*- C++ -*-

//=============================================================================
/**
 *  @file   Unknown_User_Exception.h
 *
 *  $Id$
 *
 *  Defines the way an exception is reported by a DII request.
 *
 *
 *  @author  Portions Copyright 1994-1995 by Sun Microsystems Inc.
 *  @author  Portions Copyright 1997-2003 by Washington University
 */
//=============================================================================


#ifndef TAO_UNKNOWN_USER_EXCEPTION_H
#define TAO_UNKNOWN_USER_EXCEPTION_H

#include "ace/pre.h"

#include "dynamicinterface_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Exception.h"

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_DynamicInterface_Export

#if defined (TAO_EXPORT_NESTED_CLASSES)
#  if defined (TAO_EXPORT_NESTED_MACRO)
#    undef TAO_EXPORT_NESTED_MACRO
#  endif /* defined (TAO_EXPORT_NESTED_MACRO) */
#  define TAO_EXPORT_NESTED_MACRO TAO_DynamicInterface_Export
#endif /* TAO_EXPORT_NESTED_CLASSES */

namespace CORBA
{
  /**
   * @class UnknownUserException
   *
   * @brief UnknownUserException
   *
   * When user exceptions are received by a DII invocation the ORB
   * is unable to create the exception with the right dynamic type;
   * to workaround this problem it throws a
   * @c CORBA::UnknownUserException that contains the exception inside
   * an Any.
   */
  class TAO_DynamicInterface_Export UnknownUserException
    : public CORBA::UserException
  {
  public:

    /// Constructor.
    UnknownUserException (void);

    /// Constructor.
    UnknownUserException (CORBA::Any& exception);

    /// Copy constructor.
    UnknownUserException (const UnknownUserException& e);

    /// Destructor.
    virtual ~UnknownUserException (void);

    /// Return the any containing the user exception.
    CORBA::Any& exception (void);

    /// To throw an UnknownUserException of this type.
    virtual void _raise (void);

    virtual CORBA::Exception *_tao_duplicate (void) const;
    virtual void _tao_encode (TAO_OutputCDR &cdr
                              ACE_ENV_ARG_DECL) const;
    virtual void _tao_decode (TAO_InputCDR &cdr
                              ACE_ENV_ARG_DECL);

    /// Narrow to an UnknowUserException.
    static UnknownUserException *_downcast (CORBA::Exception *ex);

    // = TAO specific extension.

    /// Helper method to implement _downcast.
    virtual int _is_a (const char *type_id) const;

    /// This class has a specific typecode.
    virtual CORBA::TypeCode_ptr _type (void) const;

  private:
    /// Holder for the actual exception.
    CORBA::Any *exception_;
  };
} // End CORBA namespace.


#include "ace/post.h"

#endif /* TAO_UNKNOWN_USER_EXCEPTION_H */
