// -*- C++ -*-  $Id$

#ifndef ACEXML_LOCALTORIMPL_H
#define ACEXML_LOCALTORIMPL_H

#include "Common/Locator.h"

class ACEXML_Export ACEXML_LocatorImpl : public ACEXML_Locator
{
public:
  /*
   * Default constructor.
   */
  ACEXML_LocatorImpl (void);

  /*
   * Copy constructor.  Create a persistent copy of the current state
   * of a locator. When the original locator changes, this copy will
   * still keep the original values (and it can be used outside the
   * scope of DocumentHandler methods).
   */
  ACEXML_LocatorImpl (const ACEXML_Locator *locator);

  /*
   * Destructor.
   */
  virtual ~ACEXML_LocatorImpl (void);

  /*
   * Return the column number where the current document event ends.
   */
  virtual int getColumnNumber (void) const;

  /*
   * Return the line number where the current document event ends.
   */
  virtual int getLineNumber (void) const;

  /*
   * Return the public identifier for the current document event.
   */
  virtual const ACEXML_Char *getPublicId (void) const;

  /*
   * Return the system identifier for the current document event.
   */
  virtual const ACEXML_Char *getSystemId (void) const;

  /*
   * Set the column number of this locator.
   */
  void setColumnNumber (int cn);

  /*
   * Set the line number of this locator.
   */
  void setLineNumber (int ln);

  /*
   * Set the public identifier of this locator.
   */
  void setPublicId (const ACEXML_Char *id);

  /*
   * Set the system identifier of this locator.
   */
  void setSystemId (const ACEXML_Char *id);

private:
  ACEXML_Char *publicId_;
  ACEXML_Char *systemId_;
  int lineNumber_;
  int columnNumber_;
};

#if defined (__ACEXML_INLINE__)
# include "Common/LocatorImpl.i"
#endif /* __ACEXML_INLINE__ */
#endif /* ACEXML_LOCALTORIMPL_H */
