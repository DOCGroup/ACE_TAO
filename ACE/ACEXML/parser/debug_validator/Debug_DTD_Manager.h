// -*- C++ -*-

//=============================================================================
/**
 *  @file    Debug_DTD_Manager.h
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
#ifndef _ACEXML_DEBUG_DTD_Manager_H_
#define _ACEXML_DEBUG_DTD_Manager_H_

#include /**/ "ace/pre.h"
#include "ACEXML/parser/debug_validator/Debug_DTD_Manager_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/DTD_Manager.h"

class ACEXML_DEBUG_DTD_MANAGER_Export ACEXML_Debug_DTD_Manager : public ACEXML_DTD_Manager
{
public:
  ACEXML_Debug_DTD_Manager ();

  ~ACEXML_Debug_DTD_Manager () override;
  ACEXML_Debug_DTD_Manager (ACEXML_Debug_DTD_Manager const &) = delete;
  ACEXML_Debug_DTD_Manager (ACEXML_Debug_DTD_Manager &&) = delete;
  ACEXML_Debug_DTD_Manager &operator= (ACEXML_Debug_DTD_Manager const &) = delete;
  ACEXML_Debug_DTD_Manager &operator= (ACEXML_Debug_DTD_Manager &&) = delete;

  /**
   * Acquire a pointer to an element definition builder. The XML parser use
   * this interface to acquire the definition builder and use the builder
   * to create the DTD element definition. The resulting builder is then
   * registered with the DTD Manager or destroyed if error occurred when the
   * builder encountered errors.
   *
   * @retval 0 if error occurs creating the builder.
   */
  ACEXML_Element_Def_Builder *getElement_Def_Builder () override;

  /**
   * Insert a new element definition into the DTD Manager.
   *
   * @retval 0 if success, -1 if error.
   */
  int insertElement_Definition (ACEXML_Element_Def_Builder *def) override;

  /**
   * Acquire a pointer to an attributes definition builder.
   *
   */
  ACEXML_Attributes_Def_Builder *getAttribute_Def_Builder () override;

  /**
   * Insert a new attributes definition into the DTD Manager.
   *
   * @retval 0 if success, -1 otherwise.
   */
  int insertAttributes_Definition (ACEXML_Attributes_Def_Builder *def) override;

  /**
   * Acquire an element validator to validate an XML element.
   *
   * @todo I haven't figured out what memory management scheme
   *       we should use for the acquired validator.
   */
  ACEXML_Validator *getValidator (ACEXML_Char const *namespaceURI,
                                  ACEXML_Char const *localName,
                                  ACEXML_Char const *qName) override;
};

#include /**/ "ace/post.h"


#endif /* _ACEXML_DTD_Manager_H_ */
