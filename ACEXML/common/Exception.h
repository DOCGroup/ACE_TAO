// -*- C++ -*- $Id$

#ifndef _ACEXML_EXCEPTION_H_
#define _ACEXML_EXCEPTION_H_

#include "Common/XML_Types.h"

class ACEXML_Export ACEXML_Exception
{
public:
  ACEXML_Exception (void);
  ACEXML_Exception (const ACEXML_Exception &ex);

  virtual ~ACEXML_Exception (void);

  static const ACEXML_Char *name (void);
  // Accessor for the exception name.

  virtual const ACEXML_Char *id (void);
  // Return the exception type.  (for safe downcast.)

  virtual ACEXML_Exception *duplicate (void) = 0;
  // Dynamically create a copy of this exception.

  virtual int is_a (const ACEXML_Char *name) = 0;
  // Check whether this is an exception of type specify by
  // <name>.

  virtual void print (void) = 0;
  // Print out exception using ACE_DEBUG.

protected:
  static const ACEXML_Char *exception_name_;

  static const ACEXML_Char *null_;
};

#if defined (__ACEXML_INLINE__)
# include "Common/Exception.i"
#endif /* __ACEXML_INLINE__ */
#endif /* _ACEXML_EXCEPTION_H_ */
