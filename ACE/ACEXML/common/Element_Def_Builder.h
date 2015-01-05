// -*- C++ -*-

//=============================================================================
/**
 *  @file    Element_Def_Builder.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
#ifndef _ACEXML_ELEMENT_DEF_BUILDER_H_
#define _ACEXML_ELEMENT_DEF_BUILDER_H_

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Auto_Ptr.h"
#include "ACEXML/common/XML_Types.h"
#include "ACEXML/common/SAXExceptions.h"

/**
 * @class ACEXML_Element_Def_Builder
 *
 * @brief An abstract virtual class that defines the interface to define an
 * element definition.
 *
 * This class defines how to define an element definition after parsing a
 * DTD.
 */
class ACEXML_Export ACEXML_Element_Def_Builder
{
public:

  typedef auto_ptr<ACEXML_Element_Def_Builder> VAR;

  typedef enum {
    EMPTY,
    ANY,
    MIXED,
    CHILDREN,
    UNDEFINED
  } CONTENT_TYPE;

  typedef enum {
    ONE,
    ZERO_OR_MORE,
    ONE_OR_MORE,
    ONE_OR_ZERO
  } CARDINALITY;

  virtual ~ACEXML_Element_Def_Builder () = 0;

  /**
   * Define the name of the element.
   *
   * @retval 0 if valid, -1 otherwise.
   */
  virtual int setElementName (const ACEXML_Char *namespaceURI,
                              const ACEXML_Char *localName,
                              const ACEXML_Char *qName)
        = 0;

  /**
   * Define the content type of the element.
   *
   * @retval 0 if valid, -1 otherwise.
   */
  virtual int setContentType (CONTENT_TYPE type)
        = 0;

  /**
   * Insert one more element into Mixed definition.
   */
  virtual int insertMixedElement (const ACEXML_Char *namespaceURI,
                                  const ACEXML_Char *localName,
                                  const ACEXML_Char *qName)
        = 0;

  /**
   * Start a new group of children.
   */
  virtual int startChildGroup () = 0;

  /**
   * End a new group of children.
   *
   * @retval 0 on success.
   */
  virtual int endChildGroup (CARDINALITY card) = 0;

  /**
   * Set the type of current child group to Choice.
   *
   * @retval 0 on success, -1 if the type of the child group has
   * already been set and this action conflicts with the previous
   * setting.
   */
  virtual int setChoice () = 0;

  /**
   * Set the type of current child group to Sequence.
   *
   * @retval 0 on success, -1 if the type of the child group has
   * already been set and this action conflicts with the previous
   * setting.
   */
  virtual int setSequence () = 0;

  /**
   * Insert an new element into the current child group.
   *
   * @retval 0 on success, -1 otherwise.
   */
  virtual int insertElement  (const ACEXML_Char *namespaceURI,
                              const ACEXML_Char *localName,
                              const ACEXML_Char *qName)
        = 0;

  /**
   * Dump the content of the attribute definition.
   */
  virtual void dump (void) = 0;
};


#include /**/ "ace/post.h"

#endif /* _ACEXML_ELEMENT_DEF_BUILDER_H_ */
