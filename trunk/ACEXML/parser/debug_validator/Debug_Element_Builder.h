// -*- C++ -*-

//=============================================================================
/**
 *  @file    Debug_Element_Builder.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
#ifndef _ACEXML_DEBUG_ELEMENT_BUILDER_H_
#define _ACEXML_DEBUG_ELEMENT_BUILDER_H_

#include "ace/pre.h"
#include "ACEXML/parser/debug_validator/Debug_DTD_Manager_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/Element_Def_Builder.h"
#include "ACEXML/parser/debug_validator/Element_Tree.h"

/**
 * @class ACEXML_Debug_Element_Builder Debug_Element_Builder.h "parser/debug_validator/Debug_Element_Builder.h"
 *
 * This class prints out the element definition for debugging purpose.
 *
 * @todo This class is not namespace-aware.
 */
class ACEXML_DEBUG_DTD_MANAGER_Export ACEXML_Debug_Element_Builder
  : public ACEXML_Element_Def_Builder
{
public:
  ACEXML_Debug_Element_Builder ();

  virtual ~ACEXML_Debug_Element_Builder ();

  /**
   * Define the name of the element.
   *
   * @retval 0 if valid, -1 otherwise.
   */
  virtual int setElementName (const ACEXML_Char *namespaceURI,
                              const ACEXML_Char *localName,
                              const ACEXML_Char *qName ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /**
   * Define the content type of the element.
   *
   * @retval 0 if valid, -1 otherwise.
   */
  virtual int setContentType (CONTENT_TYPE type ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /**
   * Insert one more element into Mixed definition.
   */
  virtual int insertMixedElement (const ACEXML_Char *namespaceURI,
                                  const ACEXML_Char *localName,
                                  const ACEXML_Char *qName ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /**
   * Start a new group of children.
   */
  virtual int startChildGroup ();

  /**
   * End a new group of children.
   *
   * @retval 0 on success.
   */
  virtual int endChildGroup (CARDINALITY card ACEXML_ENV_ARG_DECL);

  /**
   * Set the type of current child group to Choice.
   *
   * @retval 0 on success, -1 if the type of the child group has
   * already been set and this action conflicts with the previous
   * setting.
   */
  virtual int setChoice ();

  /**
   * Set the type of current child group to Sequence.
   *
   * @retval 0 on success, -1 if the type of the child group has
   * already been set and this action conflicts with the previous
   * setting.
   */
  virtual int setSequence ();

  /**
   * Insert an new element into the current child group.
   *
   * @retval 0 on success, -1 otherwise.
   */
  virtual int insertElement  (const ACEXML_Char *namespaceURI,
                              const ACEXML_Char *localName,
                              const ACEXML_Char *qName ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /**
   * Dump the content of the attribute definition.
   */
  virtual void dump (void);
private:
  CONTENT_TYPE type_;

  ACEXML_String element_;

  ACEXML_Element_Tree_List_Node *root_;

  ACEXML_Element_Tree_List_Stack active_list_;
};


#include "ace/post.h"

#endif /* _ACEXML_DEBUG_ELEMENT_BUILDER_H_ */
