// -*- C++ -*- $Id$

#ifndef _ACEXML_ENTITYHANDLER_H_
#define _ACEXML_ENTITYHANDLER_H_

#include "common/Env.h"
#include "common/InputSource.h"

class ACEXML_Export ACEXML_EntityResolver
{
public:
  /*
   * Allow the application to resolve external entities.
   */
  virtual ACEXML_InputSource *resolveEntity (const ACEXML_Char *publicId,
                                             const ACEXML_Char *systemId,
                                             ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;
};

#endif /* _ACEXML_ENTITYHANDLER_H_ */
