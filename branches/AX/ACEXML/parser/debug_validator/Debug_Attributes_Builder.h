// -*- C++ -*-

//=============================================================================
/**
 *  @file   Debug_Attributes_Builder.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
#ifndef _ACEXML_DEBUG_ATTRIBUTES_BUILDER_H_
#define _ACEXML_DEBUG_ATTRIBUTES_BUILDER_H_

#include "common/Attributes_Def_Builder.h"
#include "parser/debug_validator/Debug_DTD_Manager_Export.h"

/**
 * @ class ACEXML_Debug_Attribute_Builder Debug_Attributes_Builder.h "parser/debug_validator/Debug_Attributes_Builder.h"
 *
 * This class prints out the Attribute definition for debugging purpose.
 */
class ACEXML_DEBUG_DTD_MANAGER_Export ACEXML_Debug_Attribute_Builder
  : public ACEXML_Attribute_Def_Builder
{
public:
  ACEXML_Debug_Attribute_Builder ();

  virtual ~ACEXML_Debug_Attribute_Builder ();

  /**
   * Set the attribute type.
   */
  virtual int setAttType (const ATT_TYPE type,
                          ACEXML_Env &xmlenv)
    // ACE_THORW_SPEC ((ACEXML_SAXException))
    ;

  /**
   * Insert an element for NOTATION or ENUMERATION type attribute.
   */
  virtual int insertList (const ACEXML_Char Name,
                          ACEXML_Env &xmlenv)
    // ACE_THORW_SPEC ((ACEXML_SAXException))
    ;

  /**
   * Set default attribute declaration.
   */
  virtual int setDefault (const DEFAULT_DECL def,
                          const ACEXML_Char *value,
                          ACEXML_Env &xmlenv)
    // ACE_THORW_SPEC ((ACEXML_SAXException))
    ;

  /**
   * Check validity of the current attribute definition being built.
   *
   * @retval 0 if the attribute is not a valid combo.
   */
  virtual int validAttr (void);

private:
  /// Type of attribute.
  ATT_TYPE type_;

  /// Default value type.
  DEFAULT_DECL default_decl_;
};

/**
 * @ class ACEXML_Debug_Attributes_Builder Debug_Attributes_Builder.h "parser/debug_validator/Debug_Attributes_Builder.h"
 *
 * This class prints out Attribute definitions for debugging purpose.
 */
class ACEXML_DEBUG_DTD_MANAGER_Export ACEXML_Debug_Attributes_Builder
  : public ACEXML_Attributes_Def_Builder
{
public:
  ACEXML_Debug_Attributes_Builder ();

  virtual ~ACEXML_Debug_Attributes_Builder ();

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
    ;

  /**
   * Acquire an Attribute_Builder.
   */
  virtual ACEXML_Attribute_Def_Builder *getAttribute_Def_Builder ();

  /**
   * Add a definition for one attribute.
   */
  virtual int insertAttribute (ACEXML_Attribute_Def_Builder *def,
                               ACEXML_Env &xmlenv);

};

#endif /* _ACEXML_DEBUG_ATTRIBUTES_BUILDER_H_ */
