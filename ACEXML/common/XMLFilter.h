// -*- C++ -*- $Id$

#ifndef _ACEXML_XMLFILTER_H_
#define _ACEXML_XMLFILTER_H_

#include "Common/XMLReader.h"

class ACEXML_Export ACEXML_XMLFilter : public ACEXML_XMLReader
{
public:
  /*
   * Get the parent reader.
   */
  virtual ACEXML_XMLReader *getParent (void) const = 0;

  /*
   * Set the parent reader.
   */
  virtual void setParent (ACEXML_XMLReader *parent) = 0;
};

#endif /* _ACEXML_XMLFILTER_H_ */
