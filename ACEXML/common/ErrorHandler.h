// -*- C++ -*- $Id$

#ifndef _ACEXML_ERRORHANDLER_H_
#define _ACEXML_ERRORHANDLER_H_

#include "common/Env.h"
#include "common/SAXExceptions.h"

class ACEXML_Export ACEXML_ErrorHandler
{
public:
  /*
   * Receive notification of a recoverable error.
   */
  virtual void error (ACEXML_SAXParseException &exception,
                      ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;

  /*
   * Receive notification of a non-recoverable error.
   */
  virtual void fatalError (ACEXML_SAXParseException &exception,
                           ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;

  /*
   * Receive notification of a warning.
   */
  virtual void warning (ACEXML_SAXParseException &exception,
                        ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;
};

#endif /* _ACEXML_ERRORHANDLER_H_ */
