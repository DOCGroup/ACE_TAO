// -*- C++ -*- $Id$

#ifndef _ACEXML_DTDHANDLER_H_
#define _ACEXML_DTDHANDLER_H_

#include "Common/Env.h"
#include "Common/SAXExceptions.h"

class ACEXML_Export ACEXML_DTDHandler
{
public:
  /*
   * Receive notification of a notation declaration event.
   */
  virtual void notationDecl (const ACEXML_Char *name,
                             const ACEXML_Char *publicId,
                             const ACEXML_Char *systemId,
                             ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;

  /*
   * Receive notification of an unparsed entity declaration event.
   */
  virtual void unparsedEntityDecl (const ACEXML_Char *name,
                                   const ACEXML_Char *publicId,
                                   const ACEXML_Char *systemId,
                                   const ACEXML_Char *notationName,
                                   ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;
};

#endif /* _ACEXML_DTDHANDLER_H_ */
