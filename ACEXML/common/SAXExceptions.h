// -*- C++ -*- $Id$

#ifndef _ACEXML_SAXEXCEPTIONS_H_
#define _ACEXML_SAXEXCEPTIONS_H_

#include "common/Exception.h"

class ACEXML_Export ACEXML_SAXException : public ACEXML_Exception
{
public:
  ACEXML_SAXException (void);
  ACEXML_SAXException (const ACEXML_Char *msg);
  ACEXML_SAXException (const ACEXML_SAXException &ex);

  virtual ~ACEXML_SAXException (void);

  static const ACEXML_Char *name (void);
  // Accessor for the exception name.

  virtual const ACEXML_Char *id (void);
  // Return the exception type.  (for safe downcast.)

  const ACEXML_Char *message (void);
  // Return the extra message accompanying the exception.

  virtual ACEXML_Exception *duplicate (void);
  // Dynamically create a copy of this exception.

  virtual int is_a (const ACEXML_Char *name);
  // Check whether this is an exception of type specify by
  // <name>.

  virtual void print (void);
  // Print out exception using ACE_DEBUG.

protected:
  static const ACEXML_Char *exception_name_;

  ACEXML_Char *message_;
};

class ACEXML_Export ACEXML_SAXNotSupportedException
  : public ACEXML_SAXException
{
public:
  ACEXML_SAXNotSupportedException (void);
  ACEXML_SAXNotSupportedException (const ACEXML_SAXNotSupportedException &ex);

  virtual ~ACEXML_SAXNotSupportedException (void);

  static const ACEXML_Char *name (void);
  // Accessor for the exception name.

  virtual const ACEXML_Char *id (void);
  // Return the exception type.  (for safe downcast.)

  virtual ACEXML_Exception *duplicate (void);
  // Dynamically create a copy of this exception.

  virtual int is_a (const ACEXML_Char *name);
  // Check whether this is an exception of type specify by
  // <name>.

  virtual void print (void);
  // Print out exception using ACE_DEBUG.

protected:
  static const ACEXML_Char *exception_name_;
};

class ACEXML_Export ACEXML_SAXNotRecognizedException
  : public ACEXML_SAXException
{
public:
  ACEXML_SAXNotRecognizedException (void);
  ACEXML_SAXNotRecognizedException (const ACEXML_Char *msg);
  ACEXML_SAXNotRecognizedException (const ACEXML_SAXNotRecognizedException &ex);

  virtual ~ACEXML_SAXNotRecognizedException (void);

  static const ACEXML_Char *name (void);
  // Accessor for the exception name.

  virtual const ACEXML_Char *id (void);
  // Return the exception type.  (for safe downcast.)

  virtual ACEXML_Exception *duplicate (void);
  // Dynamically create a copy of this exception.

  virtual int is_a (const ACEXML_Char *name);
  // Check whether this is an exception of type specify by
  // <name>.

  virtual void print (void);
  // Print out exception using ACE_DEBUG.

protected:
  static const ACEXML_Char *exception_name_;
};

class ACEXML_Export ACEXML_SAXParseException
  : public ACEXML_SAXException
{
public:
  ACEXML_SAXParseException (void);
  ACEXML_SAXParseException (const ACEXML_Char *msg);
  ACEXML_SAXParseException (const ACEXML_SAXParseException &ex);

  virtual ~ACEXML_SAXParseException (void);

  static const ACEXML_Char *name (void);
  // Accessor for the exception name.

  virtual const ACEXML_Char *id (void);
  // Return the exception type.  (for safe downcast.)

  virtual ACEXML_Exception *duplicate (void);
  // Dynamically create a copy of this exception.

  virtual int is_a (const ACEXML_Char *name);
  // Check whether this is an exception of type specify by
  // <name>.

  virtual void print (void);
  // Print out exception using ACE_DEBUG.

protected:
  static const ACEXML_Char *exception_name_;
};

#if defined (__ACEXML_INLINE__)
# include "common/SAXExceptions.i"
#endif /* __ACEXML_INLINE__ */
#endif /* _ACEXML_SAXEXCEPTIONS_H_ */
