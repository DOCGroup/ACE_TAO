// -*- C++ -*-

//=============================================================================
/**
 *  @file    Exception.h
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_EXCEPTION_H_
#define _ACEXML_EXCEPTION_H_

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"

/**
 * @class ACEXML_Exception
 *
 * @brief ACEXML_Exception
 *
 * ACEXML_Exception is the base class for all ACEXML related exceptions.
 * Since ACEXML currently does not support native exceptions, all
 * exceptions should be thrown thru ACEXML_Env.
 *
 * @sa ACEXML_Env
 */
class ACEXML_Export ACEXML_Exception
{
public:

  /// Destructor.
  virtual ~ACEXML_Exception ();

  /// Throw the exception.
  virtual void _raise () = 0;

  /// Static narrow operation.
  static ACEXML_Exception* _downcast (ACEXML_Exception* ex);

  /// Return the exception type.  (for safe downcast.)
  virtual const ACEXML_Char *id () const ;

  /// Dynamically create a copy of this exception.
  virtual ACEXML_Exception *duplicate () const = 0;

  /// Check whether this is an exception of type specified by <name>.
  virtual int is_a (const ACEXML_Char *name);

  /// Print out exception using ACE_DEBUG.
  virtual void print () const = 0;

protected:
  /// Default constructor.
  ACEXML_Exception ();

  /// All exceptions have names.  This name is used to identify the
  /// type of an exception.
  static const ACEXML_Char *exception_name_;

  /// A null string that we return when there is no exception.
  static const ACEXML_Char *null_;
};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/common/Exception.inl"
#endif /* __ACEXML_INLINE__ */

#include /**/ "ace/post.h"

#endif /* _ACEXML_EXCEPTION_H_ */
