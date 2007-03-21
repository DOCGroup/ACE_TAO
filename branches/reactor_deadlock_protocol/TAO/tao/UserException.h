// -*- C++ -*-

//=============================================================================
/**
 *  @file    UserException.h
 *
 *  $Id$
 *
 *  CORBA::UserException class header.
 *
 *  @author DOC Group at Vanderbilt U, Wash U, and UCI
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_USER_EXCEPTION_H
#define TAO_USER_EXCEPTION_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Exception.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  /**
   * @class UserException
   *
   * @brief Application- or OMG- defined CORBA exception sub-class.
   *
   * User exceptions can be defined in application OMG IDL or by the
   * OMG itself for IDL interfaces and services it defines.
   */
  class TAO_Export UserException : public Exception
  {
  public:

    /// Copy constructor.
    UserException (UserException const &rhs);

    /// Destructor.
    virtual ~UserException (void);

    /// Assignment operator.
    UserException &operator= (UserException const &rhs);

    /// The narrow operation.
    static UserException *_downcast (CORBA::Exception *exception);

    /// The const version of narrow operation
    static const UserException *_downcast (CORBA::Exception const *exception);

    virtual void _raise (void) const = 0;

    virtual CORBA::TypeCode_ptr _tao_type (void) const;

    // = TAO specific extension.

    /// Constructor from a repository id.
    UserException (char const *repository_id, char const *local_name);

    /// Returns a string containing information about the exception. This
    /// function is not CORBA compliant.
    virtual ACE_CString _info (void) const;

  protected:
    /// Default constructor.
    UserException (void);
  };

}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/UserException.inl"
#endif /* __ACE_INLINE__ */

#include /**/"ace/post.h"

#endif /* TAO_USER_EXCEPTION_H */
