// -*- C++ -*- $Id$

#ifndef _ACEXML_LOCATOR_H_
#define _ACEXML_LOCATOR_H_

#include "common/XML_Types.h"

class ACEXML_Export ACEXML_Locator
{
public:
  /*
   * Return the column number where the current document event ends.
   */
  virtual int getColumnNumber (void) const = 0;

  /*
   * Return the line number where the current document event ends.
   */
  virtual int getLineNumber (void) const = 0;

  /*
   * Return the public identifier for the current document event.
   */
  virtual const ACEXML_Char *getPublicId (void) const = 0;

  /*
   * Return the system identifier for the current document event.
   */
  virtual const ACEXML_Char *getSystemId (void) const = 0;
};

#endif /* _ACEXML_LOCATOR_H_ */
