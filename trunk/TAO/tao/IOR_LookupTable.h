/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     IOR_LookupTable.h
//
// = DESCRIPTION
//     Contains the class that defines the IOR lookup table.
//
// = AUTHOR
//     Vishal Kachroo
//     Ossama Othman
//
// ============================================================================

#ifndef TAO_IOR_LOOKUPTABLE_H
#include "ace/pre.h"
#define TAO_IOR_LOOKUPTABLE_H

#include "ace/Synch_T.h"
#include "ace/Hash_Map_Manager.h"
#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Export TAO_IOR_LookupTable_Callback
{
public:
  virtual int find_ior (const ACE_CString &object_name,
                        ACE_CString &ior);
};

class TAO_Export TAO_IOR_LookupTable
{
  //   Represents a table for the IOR lookups, given the object keys. A
  //   Hash_Map_Manager is being used currently for the table. This
  //   could be changed/added-on-to by other data structures like a file
  //   etc. This class is going to be used as a backing store by the
  //   resolve initial references. IOR_Multicast will be responsible for
  //   initialising this class.

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

  void register_callback (TAO_IOR_LookupTable_Callback *callback, 
                          int delete_callback = 0);
  // Registers a callback class with the lookup table.

  CORBA_ORB_ObjectIdList_ptr list_initial_services (
              CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Returns a sequence of ObjectIds that lists which objects have
  // references available via the initial references mechanism.

  typedef ACE_Hash_Map_Manager<ACE_CString,
                               ACE_CString,
                               ACE_Null_Mutex> TAO_IOR_Map;

private:
  TAO_IOR_Map table_;
  // Maintains the table of object_names -> IORs.

  TAO_IOR_LookupTable_Callback *callback_;
  // The callback class that table can also use to find IORs.
  
  int delete_callback_;
  // If true, means delete the callback_.
};

#include "ace/post.h"
#endif /* TAO_IOR_LOOKUPTABLE_H */

