// -*- C++ -*-

//=============================================================================
/**
 *  @file    Exception.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_EXCEPTION_H_
#define _ACEXML_EXCEPTION_H_

#include "ACEXML/common/XML_Types.h"

/**
 * @class ACEXML_Exception Exception.h "ACEXML/common/Exception.h"
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
  /// Default contructor.
  ACEXML_Exception (void);

  /// Copu constructor.
  ACEXML_Exception (const ACEXML_Exception &ex);

  /// Destructor.
  virtual ~ACEXML_Exception (void);

  /// Accessor for the exception name.
  static const ACEXML_Char *name (void);

  /// Return the exception type.  (for safe downcast.)
  virtual const ACEXML_Char *id (void);

  /// Dynamically create a copy of this exception.
  virtual ACEXML_Exception *duplicate (void) = 0;

  /// Check whether this is an exception of type specify by
  /// <name>.
  virtual int is_a (const ACEXML_Char *name) = 0;

  /// Print out exception using ACE_DEBUG.
  virtual void print (void) = 0;

protected:
  /// All exceptions have names.  This name is used to identify the
  /// type of an exception.
  static const ACEXML_Char *exception_name_;

  /// A null string that we return when there's no exception occurred.
  static const ACEXML_Char *null_;
};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/common/Exception.i"
#endif /* __ACEXML_INLINE__ */
#endif /* _ACEXML_EXCEPTION_H_ */
