// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     ORB_Table.h
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//     Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef TAO_ORB_TABLE_H
#define TAO_ORB_TABLE_H

#include "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Singleton.h"
#include "ace/Synch.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Functor.h"


// Forward declarations.
class TAO_ORB_Core;

class TAO_Export TAO_ORB_Table
{
  // = TITLE
  //   Keep a table with all the ORBs in the system.
  //
  // = DESCRIPTION
  //   CORBA::ORB_init() is supposed to return the same ORB if the
  //   user specifies the same ORBid, either in the ORB_init()
  //   parameter or in the -ORBid option.
  //   This class is used to implement that feature.
  //   It is also useful when trying to determine if an object
  //   reference is collocated or not.
  //
public:
  ~TAO_ORB_Table (void);
  // destructor

  typedef ACE_Hash_Map_Manager_Ex<const char *, TAO_ORB_Core *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex> Table;
  typedef Table::iterator Iterator;

  Iterator begin (void);
  Iterator end (void);
  int bind (const char *orb_id, TAO_ORB_Core *orb_core);
  TAO_ORB_Core* find (const char *orb_id);
  int unbind (const char *orb_id);
  // The canonical ACE_Map methods.

  TAO_ORB_Core *first_orb (void);
  // Obtain the first ORB for the ORB_Core_instance() implementation

  static TAO_ORB_Table *instance (void);
  // Return a unique instance

protected:
  friend class TAO_Singleton<TAO_ORB_Table, ACE_SYNCH_MUTEX>;
  TAO_ORB_Table (void);
  // Constructor

private:
  // Prevent copying
  ACE_UNIMPLEMENTED_FUNC (TAO_ORB_Table (const TAO_ORB_Table &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_ORB_Table &))

private:
  Table table_;
  // The implementation.

  TAO_ORB_Core *first_orb_;
  // The first ORB created by the user
};


#if defined (__ACE_INLINE__)
# include "tao/ORB_Table.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif  /* TAO_ORB_TABLE_H */
