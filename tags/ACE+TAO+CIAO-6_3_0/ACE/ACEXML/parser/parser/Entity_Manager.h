// -*- C++ -*-

//=============================================================================
/**
 *  @file    Entity_Manager.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author Krishnakumar B <kitty@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACEXML_ENTITY_MANAGER_H
#define ACEXML_ENTITY_MANAGER_H

#include /**/ "ace/pre.h"
#include "ACEXML/parser/parser/Parser_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"

typedef ACE_Hash_Map_Entry<ACEXML_String,
                           ACEXML_String> ACEXML_ENTITY_ENTRY;

typedef ACE_Hash_Map_Manager_Ex<ACEXML_String,
                                ACEXML_String,
                                ACE_Hash<ACEXML_String>,
                                ACE_Equal_To<ACEXML_String>,
                                ACE_Null_Mutex> ACEXML_ENTITIES_MANAGER;

typedef ACE_Hash_Map_Iterator_Ex<ACEXML_String,
                                 ACEXML_String,
                                 ACE_Hash<ACEXML_String>,
                                 ACE_Equal_To<ACEXML_String>,
                                 ACE_Null_Mutex> ACEXML_ENTITIES_MANAGER_ITER;

typedef ACE_Hash_Map_Reverse_Iterator_Ex<ACEXML_String,
                                         ACEXML_String,
                                         ACE_Hash<ACEXML_String>,
                                         ACE_Equal_To<ACEXML_String>,
                                         ACE_Null_Mutex> ACEXML_ENTITIES_MANAGER_REVERSE_ITER;

typedef ACE_Hash_Map_Bucket_Iterator<ACEXML_String,
                                     ACEXML_String,
                                     ACE_Hash<ACEXML_String>,
                                     ACE_Equal_To<ACEXML_String>,
                                     ACE_Null_Mutex> ACEXML_ENTITY_ENTRY_ITERATOR;

/**
 * @class ACEXML_Entity_Manager Entity_Manager.h "ACEXML/parser/parser/Entity_Manager.h"
 *
 * @brief Class to manage and resolve entity references.
 *
 * @todo Fill in details for this class.
 */
class ACEXML_PARSER_Export ACEXML_Entity_Manager
{
public:
  /// Default constructor.
  ACEXML_Entity_Manager (void);

  /// Destructor.
  ~ACEXML_Entity_Manager (void);

  /// Add a new entity declaration.
  int add_entity (const ACEXML_Char *ref, const ACEXML_Char *value);

  /// Resolve an entity reference.
  const ACEXML_Char* resolve_entity (const ACEXML_Char *ref);

  /// Resolve an entity reference and return the tuple of @c systemId and
  /// @c publicId
  int resolve_entity (const ACEXML_Char* ref, ACEXML_Char*& systemId,
                      ACEXML_Char*& publicId);

  /// Number of items in the Entity Manager
  size_t size(void) const;

  /// Reset the state
  int reset (void);

private:
  ACEXML_ENTITIES_MANAGER* entities_;
  // bool init_;

};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/parser/parser/Entity_Manager.inl"
#endif /* __ACEXML_INLINE__ */

#include /**/ "ace/post.h"

#endif /* ACEXML_ENTITY_MANAGER_H */
