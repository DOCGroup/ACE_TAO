// -*- C++ -*-

//=============================================================================
/**
 *  @file    DTDHandler.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
#ifndef _ACEXML_DTDHANDLER_H_
#define _ACEXML_DTDHANDLER_H_

#include "ACEXML/common/Env.h"
#include "ACEXML/common/SAXExceptions.h"

/**
 * @class ACEXML_DTDHandler DTDHandler.h "ACEXML/common/DTDHandler.h"
 *
 * @brief ACEXML_DTDHandler
 *
 * If a SAX application needs information about notations and unparsed
 * entities, then the application implements this interface and
 * registers an instance with the SAX parser using the parser's
 * setDTDHandler method. The parser uses the instance to report
 * notation and unparsed entity declarations to the application.
 *
 * Note that this interface includes only those DTD events that the
 * XML recommendation requires processors to report: notation and
 * unparsed entity declarations.
 *
 * The SAX parser may report these events in any order, regardless of
 * the order in which the notations and unparsed entities were
 * declared; however, all DTD events must be reported after the
 * document handler's startDocument event, and before the first
 * startElement event.
 *
 * It is up to the application to store the information for future use
 * (perhaps in a hash table or object tree). If the application
 * encounters attributes of type "NOTATION", "ENTITY", or "ENTITIES",
 * it can use the information that it obtained through this interface
 * to find the entity and/or notation corresponding with the attribute
 * value.
 */
class ACEXML_Export ACEXML_DTDHandler
{
public:
  /**
   * Receive notification of a notation declaration event.
   */
  virtual void notationDecl (const ACEXML_Char *name,
                             const ACEXML_Char *publicId,
                             const ACEXML_Char *systemId,
                             ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;

  /**
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
