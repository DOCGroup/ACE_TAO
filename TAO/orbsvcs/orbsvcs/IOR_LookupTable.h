/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs
//
// = FILENAME
//    IOR_LookupTable.h
//
// = DESCRIPTION
//    Represents a table for the IOR lookups, given the object keys. A Hash_Map_Manager
//    is being used currently for the table. This could be changed/added-on-to by other
//    data structures like a file etc. This class is going to be used as a backing store
//    by the resolve initial references. IOR_Multicast will be responsible for initialising
//    this class.
//
// = AUTHOR
//   Vishal Kachroo.
//
// ============================================================================

#ifndef TAO_IOR_LOOKUPTABLE_H
#define TAO_IOR_LOOKUPTABLE_H

#include "ace/Synch_T.h"
#include "ace/Hash_Map_Manager.h"
#include "orbsvcs/orbsvcs_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORBSVCS_Export TAO_IOR_LookupTable
{

public:

  // = Initialization and termination methods.
  TAO_IOR_LookupTable (void);
  // Constructor.

  ~TAO_IOR_LookupTable (void);
  // Destructor.

  int init (void);
  // This creates and initialises the table.
  // Currently, the hash map manager.

  int add_ior (const ACE_CString &object_name,
               const ACE_CString &ior);
  // Associate <object_name> with <ior>.  If <object_name> is already in the
  // map then the <ACE_Hash_Map_Entry> is not changed.  Returns 0 if a
  // new IOR is bound successfully, returns 1 if an attempt is made
  // to bind an existing object_name, and returns -1 if failures occur.

  int find_ior (const ACE_CString &object_name,
                ACE_CString &ior);
  // Locate <object_name> and pass out parameter via <ior>.  If the IOR is
  // found, return 0. Return -1 if not found.

private:

  ACE_CString object_name_;
  // object string to do the lookup with.

  ACE_CString ior_;
  // IOR returned as a result of the lookup.

  ACE_Hash_Map_Manager<ACE_CString,
                       ACE_CString,
                       ACE_Null_Mutex> hash_map_;
  // Maintains the table of object_names -> IORs.

};

#endif /* TAO_IOR_LOOKUPTABLE_H */
