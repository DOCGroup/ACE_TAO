// -*- C++ -*-

//=============================================================================
/**
 *  @file    Attributes_Def_Builder.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
#ifndef _ACEXML_ATTRIBUTES_DEF_BUILDER_H_
#define _ACEXML_ATTRIBUTES_DEF_BUILDER_H_

#include "XML_Types.h"

/**
 * @ class ACEXML_Attributes_Def_Builder Attributes_Def_Builder.h "common/Attributes_Def_Builder.h"
 *
 * @ brief An abstract virtual class defining an interface
 *         for building attribute definitions from DTD.
 *
 * This class should be invisible to application programmers and
 * is only used for validator implementors.
 */
class ACEXML_Export ACEXML_Attributes_Def_Builder
{
public:
  typedef enum {
    CDATA,
    ID,
    IDREF,
    IDREFS,
    ENTITY,
    ENTITIES,
    NMTOKEN,
    NMTOKENS,
    NOTATION,
    ENUMERATION
  } ATT_TYPE;

  virtual ~ACEXML_Attributes_Def_Builder () = 0;

  /**
   * Set the element name that the attribute builder applies.
   *
   * @retval 0 if valid, -1 otherwise.
   */
  virtual int setElement (const ACEXML_Char *namespaceURI,
                          const ACEXML_Char *localName,
                          const ACEXML_Char *qName,
                          ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;

};

#endif /* _ACEXML_ATTRIBUTES_DEF_BUILDER_H_ */
