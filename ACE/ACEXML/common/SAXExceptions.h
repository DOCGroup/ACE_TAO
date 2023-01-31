// -*- C++ -*-

//=============================================================================
/**
 *  @file    SAXExceptions.h
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_SAXEXCEPTIONS_H_
#define _ACEXML_SAXEXCEPTIONS_H_

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/Exception.h"

/**
 * @class ACEXML_SAXException
 *
 * @brief ACEXML_SAXException
 *
 * ACEXML_SAXException is the mother of all SAX related exceptions.
 */
class ACEXML_Export ACEXML_SAXException : public ACEXML_Exception
{
public:
  /// Default constructor.
  ACEXML_SAXException ();

  /// Constructor initializing the exception message.
  ACEXML_SAXException (const ACEXML_Char *msg);

  /// Assignment operator
  ACEXML_SAXException& operator= (const ACEXML_SAXException& src);

  /// Copy constructor.
  ACEXML_SAXException (const ACEXML_SAXException &ex);

  /// Destructor.
  virtual ~ACEXML_SAXException ();

  /// Throw the exception.
  virtual void _raise ();

  /// Static narrow operation.
  static ACEXML_SAXException* _downcast (ACEXML_Exception* ex);

  /// Return the name of the exception.
  virtual const ACEXML_Char *id () const;

  /// Return the extra message accompanying the exception.
  const ACEXML_Char *message () const;

  virtual ACEXML_Exception *duplicate () const;

  virtual int is_a (const ACEXML_Char *name);

  virtual void print () const;

protected:
  static const ACEXML_Char * exception_name_;

  /// A message providing more information about the exception being thrown.
  ACEXML_Char *message_;
};

/**
 * @class ACEXML_SAXNotSupportedException SAXExceptions.h "ACEXML/common/SAXExceptions.h"
 *
 * @brief ACEXML_SAXNotSupportedException
 */
class ACEXML_Export ACEXML_SAXNotSupportedException
  : public ACEXML_SAXException
{
public:
  /// Default constructor.
  ACEXML_SAXNotSupportedException ();

  /// Copy constructor.
  ACEXML_SAXNotSupportedException (const ACEXML_SAXNotSupportedException &ex);

  /// Assignment operator
  ACEXML_SAXNotSupportedException& operator= (const ACEXML_SAXNotSupportedException &ex);

  /// Constructor which accepts an informational message
  ACEXML_SAXNotSupportedException (const ACEXML_Char* msg);

  /// Destructor.
  virtual ~ACEXML_SAXNotSupportedException ();

  /// Throw the exception.
  virtual void _raise ();

  /// Static narrow operation.
  static ACEXML_SAXNotSupportedException* _downcast (ACEXML_Exception* ex);

  virtual const ACEXML_Char *id () const;

  virtual ACEXML_Exception *duplicate () const;

  virtual int is_a (const ACEXML_Char *name);

  virtual void print () const;

protected:
  static const ACEXML_Char exception_name_[];
};

/**
 * @class ACEXML_SAXNotRecognizedException SAXExceptions.h "ACEXML/common/SAXExceptions.h"
 *
 * @brief ACEXML_SAXNotRecognizedException
 */
class ACEXML_Export ACEXML_SAXNotRecognizedException
  : public ACEXML_SAXException
{
public:
  /// Default constructor.
  ACEXML_SAXNotRecognizedException ();

  /// Constructor with an initializing exception message.
  ACEXML_SAXNotRecognizedException (const ACEXML_Char *msg);

  /// Copy constructor.
  ACEXML_SAXNotRecognizedException (const ACEXML_SAXNotRecognizedException &ex);

  /// Assignment operator.
  ACEXML_SAXNotRecognizedException& operator= (const ACEXML_SAXNotRecognizedException &ex);

  /// Destructor.
  virtual ~ACEXML_SAXNotRecognizedException ();

  /// Throw the exception.
  virtual void _raise ();

  /// Static narrow operation.
  static ACEXML_SAXNotRecognizedException* _downcast (ACEXML_Exception* ex);

  virtual const ACEXML_Char *id () const;

  virtual ACEXML_Exception *duplicate () const;

  virtual int is_a (const ACEXML_Char *name);

  virtual void print () const;

protected:
  static const ACEXML_Char exception_name_[];
};

/**
 * @class ACEXML_SAXParseException SAXExceptions.h "ACEXML/common/SAXExceptions.h"
 *
 * @brief ACEXML_SAXParseException
 */
class ACEXML_Export ACEXML_SAXParseException
  : public ACEXML_SAXException
{
public:
  /// Default constructor.
  ACEXML_SAXParseException ();

  /// Constructor with an initializing exception message.
  ACEXML_SAXParseException (const ACEXML_Char *msg);

  /// Copy constructor.
  ACEXML_SAXParseException (const ACEXML_SAXParseException &ex);

  /// Assignment operator.
  ACEXML_SAXParseException& operator= (const ACEXML_SAXParseException &ex);

  /// Destructor.
  virtual ~ACEXML_SAXParseException ();

  /// Throw the exception.
  virtual void _raise ();

  /// Static narrow operation.
  static ACEXML_SAXParseException* _downcast (ACEXML_Exception* ex);

  virtual const ACEXML_Char *id () const;

  virtual ACEXML_Exception *duplicate () const;

  virtual int is_a (const ACEXML_Char *name);

  virtual void print () const;

protected:
  static const ACEXML_Char exception_name_[];
};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/common/SAXExceptions.inl"
#endif /* __ACEXML_INLINE__ */

#include /**/ "ace/post.h"

#endif /* _ACEXML_SAXEXCEPTIONS_H_ */
