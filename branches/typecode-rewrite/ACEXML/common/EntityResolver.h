// -*- C++ -*-

//=============================================================================
/**
 *  @file    EntityResolver.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
#ifndef _ACEXML_ENTITYHANDLER_H_
#define _ACEXML_ENTITYHANDLER_H_

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/Env.h"
#include "ACEXML/common/InputSource.h"

/**
 * @class ACEXML_EntityResolver EntityResolver.h "ACEXML/common/EntityResolver.h"
 *
 * @brief ACEXML_EntityResolver
 *
 * If a SAX application needs to implement customized handling for
 * external entities, it must implement this interface and register an
 * instance with the SAX driver using the setEntityResolver method.
 *
 * The XML reader will then allow the application to intercept any
 * external entities (including the external DTD subset and external
 * parameter entities, if any) before including them.
 *
 * Many SAX applications will not need to implement this interface,
 * but it will be especially useful for applications that build XML
 * documents from databases or other specialised input sources, or for
 * applications that use URI types other than URLs.
 */
class ACEXML_Export ACEXML_EntityResolver
{
public:
  /**
   * Allow the application to resolve external entities.
   */
  virtual ACEXML_InputSource *resolveEntity (const ACEXML_Char *publicId,
                                             const ACEXML_Char *systemId ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
};


#include /**/ "ace/post.h"

#endif /* _ACEXML_ENTITYHANDLER_H_ */
