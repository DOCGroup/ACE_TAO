/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Persistent_Entries.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_PERSISTENT_ENTRIES_H
#define TAO_PERSISTENT_ENTRIES_H

#include "tao/corba.h"
#include "orbsvcs/CosNamingC.h"
#include "Shared_Hash_Map.h"

class TAO_ORBSVCS_Export TAO_Persistent_IntId
{
  // = TITLE
  //     Stores information a context keeps for each bound name.
  //
  // = DESCRIPTION
  //     Each bound name is associated with an object reference and
  //     the type of binding.
public:
  // = Initialization and termination methods.
  TAO_Persistent_IntId (void);
  // Constructor.

  TAO_Persistent_IntId (char * obj_ref,
                        CosNaming::BindingType type /* = CosNaming::nobject */);
  // Constructor.

  TAO_Persistent_IntId (const TAO_Persistent_IntId & rhs);
  // Copy constructor.

  ~TAO_Persistent_IntId (void);
  // Destructor.

  void operator= (const TAO_Persistent_IntId & rhs);
  // Assignment operator (does copy memory).

  CosNaming::BindingType type (void);
  //

  const char * ref_;
  // This should be allocated from shared/mmap memory.

  CosNaming::BindingType type_;
  // Indicator of whether the object is a NamingContext that should
  // participate in name resolution when compound names are used.
};

class TAO_ORBSVCS_Export TAO_Persistent_ExtId
{
  // = TITLE
  //    Stores the name to which an object is bound.
  //
  // = DESCRIPTION
  //    This class is used as the External ID for the
  //    <ACE_Hash_Map_Manager>.  We do not allocate memory for <id>
  //    and <kind>, we do not deallocate it.
public:
  // = Initialization and termination methods.

  TAO_Persistent_ExtId (void);
  // Constructor.

  TAO_Persistent_ExtId (const char *id,
                        const char *kind);
  // Constructor.

  TAO_Persistent_ExtId (const TAO_Persistent_ExtId & rhs);
  // Copy constructor.

  ~TAO_Persistent_ExtId (void);
  // Destructor.

  void operator= (const TAO_Persistent_ExtId & rhs);
  // Assignment operator (does copy memory).

  int operator== (const TAO_Persistent_ExtId &rhs) const;
  // Equality comparison operator (must match both id_ and kind_).

  int operator!= (const TAO_Persistent_ExtId &rhs) const;
  // Inequality comparison operator.

  u_long hash (void) const;
  // This class has to have a hash for use with ACE_Hash_Map_Manager.

  const char * id_;
  const char * kind_;
  // These point into shared/mmaped memory.

};

class TAO_ORBSVCS_Export TAO_Persistent_Index_IntId
{
  // = TITLE
  //     Stores information about a context.
  //
  // = DESCRIPTION
  //     Each context
  //     the type of binding.
public:
  // = Initialization and termination methods.
  TAO_Persistent_Index_IntId (void);
  // Constructor.

  TAO_Persistent_Index_IntId (ACE_UINT32 *counter,
                              ACE_Shared_Hash_Map<TAO_Persistent_ExtId,
                              TAO_Persistent_IntId> * hash_map);
  // Constructor.

  TAO_Persistent_Index_IntId (const TAO_Persistent_Index_IntId & rhs);
  // Copy constructor.

  ~TAO_Persistent_Index_IntId (void);
  // Destructor.

  void operator= (const TAO_Persistent_Index_IntId & rhs);
  // Assignment operator (does copy memory).

  ACE_UINT32 *counter_;
  //

  ACE_Shared_Hash_Map<TAO_Persistent_ExtId,
    TAO_Persistent_IntId> * hash_map_;
  // The pointer to the context's bindings hash map.
};

class TAO_ORBSVCS_Export TAO_Persistent_Index_ExtId
{
  // = TITLE
  //    Stores the poa_id of a context.
  //
  // = DESCRIPTION
  //    This class is used as the External ID for the
  //    <ACE_Hash_Map_Manager>.  We do not allocate memory for
  //    <poa_id>, we do not deallocate it.
public:
  // = Initialization and termination methods.

  TAO_Persistent_Index_ExtId (void);
  // Constructor.

  TAO_Persistent_Index_ExtId (const char *poa_id);
  // Constructor.

  TAO_Persistent_Index_ExtId (const TAO_Persistent_Index_ExtId & rhs);
  // Copy constructor.

  ~TAO_Persistent_Index_ExtId (void);
  // Destructor.

  void operator= (const TAO_Persistent_Index_ExtId & rhs);
  // Assignment operator (does copy memory).

  int operator== (const TAO_Persistent_Index_ExtId &rhs) const;
  // Equality comparison operator (must match both id_ and kind_).

  int operator!= (const TAO_Persistent_Index_ExtId &rhs) const;
  // Inequality comparison operator.

  u_long hash (void) const;
  // This class has to have a hash for use with ACE_Hash_Map_Manager.

  const char * poa_id_;
  // These point into shared/mmaped memory.

};

#endif /* TAO_PERSISTENT_ENTRIES_H */
