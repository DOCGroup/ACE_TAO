// -*- C++ -*-

//=============================================================================
/**
 *  @file    Validator.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
#ifndef _ACEXML_VALIDATOR_H_
#define _ACEXML_VALIDATOR_H_

#include "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/Attributes.h"
#include "ACEXML/common/Env.h"
#include "ACEXML/common/SAXExceptions.h"

/**
 * @class ACEXML_Validator Validator.h "common/Validator.h"
 *
 * @brief An abstract virtual class defining validator interface.
 *
 * An validator provides validation service for one XML element.
 * ACEXML_Validators are stateful object.  Implementations should
 * remember the current element encountered and determine if
 * it's a valid sequence of child elements.  A parser fishes
 * out a validator of certain
 */
class ACEXML_Export ACEXML_Validator
{
public:
  virtual ~ACEXML_Validator () = 0;

  /**
   * Validate attributes of an element.
   *
   * @retval 0 if valid, -1 otherwise.
   */
  virtual int startElement (ACEXML_Attributes *atts ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;

  /**
   * Validate the next child element.
   *
   * @retval 0 if valid, -1 otherwise.
   */
  virtual int nextElement (const ACEXML_Char *namespaceURI,
                            const ACEXML_Char *localName,
                            const ACEXML_Char *qName ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
};


#include "ace/post.h"

#endif /* _ACEXML_VALIDATOR_H_ */
