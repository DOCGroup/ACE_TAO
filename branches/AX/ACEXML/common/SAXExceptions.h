// -*- C++ -*-

//=============================================================================
/**
 *  @file    SAXExceptions.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_SAXEXCEPTIONS_H_
#define _ACEXML_SAXEXCEPTIONS_H_

#include "common/Exception.h"

/**
 * @class ACEXML_SAXException SAXExceptions.h "common/SAXExceptions.h"
 *
 * @brief ACEXML_SAXException
 *
 * ACEXML_SAXException is the mother of all SAX related exceptions.
 */
class ACEXML_Export ACEXML_SAXException : public ACEXML_Exception
{
public:
  /// Default constructor.
  ACEXML_SAXException (void);

  /// Constructor initializing the exception message.
  ACEXML_SAXException (const ACEXML_Char *msg);

  /// Copy constructor.
  ACEXML_SAXException (const ACEXML_SAXException &ex);

  /// Destructor.
  virtual ~ACEXML_SAXException (void);

  static const ACEXML_Char *name (void);

  virtual const ACEXML_Char *id (void);

  /// Return the extra message accompanying the exception.
  const ACEXML_Char *message (void);

  virtual ACEXML_Exception *duplicate (void);

  virtual int is_a (const ACEXML_Char *name);

  virtual void print (void);

protected:
  static const ACEXML_Char *exception_name_;

  /// A message providing more information about the exception being thrown.
  ACEXML_Char *message_;
};

/**
 * @class ACEXML_SAXNotSupportedException SAXExceptions.h "common/SAXExceptions.h"
 *
 * @brief ACEXML_SAXNotSupportedException
 */
class ACEXML_Export ACEXML_SAXNotSupportedException
  : public ACEXML_SAXException
{
public:
  /// Default constructor.
  ACEXML_SAXNotSupportedException (void);

  /// Copy constructor.
  ACEXML_SAXNotSupportedException (const ACEXML_SAXNotSupportedException &ex);

  /// Destructor.
  virtual ~ACEXML_SAXNotSupportedException (void);

  static const ACEXML_Char *name (void);

  virtual const ACEXML_Char *id (void);

  virtual ACEXML_Exception *duplicate (void);

  virtual int is_a (const ACEXML_Char *name);

  virtual void print (void);

protected:
  static const ACEXML_Char *exception_name_;
};

/**
 * @class ACEXML_SAXNotRecognizedException SAXExceptions.h "common/SAXExceptions.h"
 *
 * @brief ACEXML_SAXNotRecognizedException
 */
class ACEXML_Export ACEXML_SAXNotRecognizedException
  : public ACEXML_SAXException
{
public:
  /// Default constructor.
  ACEXML_SAXNotRecognizedException (void);

  /// Constructor with an initializing exception message.
  ACEXML_SAXNotRecognizedException (const ACEXML_Char *msg);

  /// Copy constructor.
  ACEXML_SAXNotRecognizedException (const ACEXML_SAXNotRecognizedException &ex);

  /// Destructor.
  virtual ~ACEXML_SAXNotRecognizedException (void);

  static const ACEXML_Char *name (void);

  virtual const ACEXML_Char *id (void);

  virtual ACEXML_Exception *duplicate (void);

  virtual int is_a (const ACEXML_Char *name);

  virtual void print (void);

protected:
  static const ACEXML_Char *exception_name_;
};

/**
 * @class ACEXML_SAXParseException SAXExceptions.h "common/SAXExceptions.h"
 *
 * @brief ACEXML_SAXParseException
 *
 * @todo This exception needs to provide error location information to behave like
 * the real SAXParseException defined in SAX2 spec.
 */
class ACEXML_Export ACEXML_SAXParseException
  : public ACEXML_SAXException
{
public:
  /// Default constructor.
  ACEXML_SAXParseException (void);

  /// Constructor with an initializing exception message.
  ACEXML_SAXParseException (const ACEXML_Char *msg);

  /// Copy constructor.
  ACEXML_SAXParseException (const ACEXML_SAXParseException &ex);

  /// Destructor.
  virtual ~ACEXML_SAXParseException (void);

  static const ACEXML_Char *name (void);

  virtual const ACEXML_Char *id (void);

  virtual ACEXML_Exception *duplicate (void);

  virtual int is_a (const ACEXML_Char *name);

  virtual void print (void);

protected:
  static const ACEXML_Char *exception_name_;
};

#if defined (__ACEXML_INLINE__)
# include "common/SAXExceptions.i"
#endif /* __ACEXML_INLINE__ */
#endif /* _ACEXML_SAXEXCEPTIONS_H_ */
