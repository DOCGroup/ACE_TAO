/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//    Persistent_Context_Index.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_PERSISTENT_CONTEXT_INDEX_H
#define TAO_PERSISTENT_CONTEXT_INDEX_H


#define TAO_NAME_CONTEXTS_INDEX "Naming_Service_Index"

#include "Persistent_Entries.h"

class TAO_ORBSVCS_Export TAO_Index
{
  // = TITLE
  //   Base class.  For now it is here to avoid templatizing
  //   TAO_Persistent_Naming_Context.

public:

typedef ACE_Shared_Hash_Map<TAO_Persistent_Index_ExtId,
  TAO_Persistent_Index_IntId> INDEX;
typedef ACE_Shared_Hash_Map<TAO_Persistent_ExtId,
  TAO_Persistent_IntId> CONTEXT;

  virtual ~TAO_Index (void);
  // Destructor.

  virtual int bind (const char *poa_id,
                    ACE_UINT32* & counter,
                    CONTEXT *hash_map) = 0;
  // Create an entry - new context is created.

  virtual int unbind (const char * poa_id) = 0;
  // Unbind an entry - a context is destroyed.

  virtual ACE_Allocator *allocator (void) = 0;
  // accessor.

  virtual CORBA::ORB_ptr orb (void) = 0;
  //
};

#endif /* TAO_PERSISTENT_CONTEXT_INDEX_H */
