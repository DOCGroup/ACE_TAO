// -*- C++ -*-

//=============================================================================
/**
 *  @file     ORB_Table.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#ifndef TAO_ORB_TABLE_H
#define TAO_ORB_TABLE_H

#include "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/TAO_Singleton.h"
#include "ace/Synch.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Functor.h"

// Forward declarations.
class TAO_ORB_Core;

/**
 * @class TAO_ORB_Table
 *
 * @brief Keep a table with all the ORBs in the system.
 *
 * CORBA::ORB_init() is supposed to return the same ORB if the
 * user specifies the same ORBid, either in the ORB_init()
 * parameter or in the -ORBid option.
 * This class is used to implement that feature.
 * It is also useful when trying to determine if an object
 * reference is collocated or not.
 */
class TAO_Export TAO_ORB_Table
{
  friend class TAO_Export TAO_Singleton<TAO_ORB_Table, TAO_SYNCH_MUTEX>;

public:
  /// destructor
  ~TAO_ORB_Table (void);

  typedef ACE_Hash_Map_Manager_Ex<const char *, TAO_ORB_Core *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex> Table;
  typedef Table::iterator Iterator;

  /// The canonical ACE_Map methods.
  //@{
  Iterator begin (void);
  Iterator end (void);
  int bind (const char *orb_id, TAO_ORB_Core *orb_core);
  TAO_ORB_Core* find (const char *orb_id);
  int unbind (const char *orb_id);
  //@}

  /// Obtain the first ORB for the ORB_Core_instance() implementation
  TAO_ORB_Core *first_orb (void);

  /// Return a unique instance
  static TAO_ORB_Table *instance (void);

protected:

  /// Constructor
  TAO_ORB_Table (void);

private:

  /// Prevent copying
  ACE_UNIMPLEMENTED_FUNC (TAO_ORB_Table (const TAO_ORB_Table &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_ORB_Table &))

private:

  /// The implementation.
  Table table_;

  /// The first ORB created by the user
  TAO_ORB_Core *first_orb_;

};

#if defined (__ACE_INLINE__)
# include "tao/ORB_Table.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif  /* TAO_ORB_TABLE_H */
