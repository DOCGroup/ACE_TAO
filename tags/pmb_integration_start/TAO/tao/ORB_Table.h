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

#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager_T.h"
#include "ace/Null_Mutex.h"

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
 *
 * @note This class should be instantiated via its instance() method.
 *       Normally this would be enforced by making the constructor
 *       protected but that forces a friend declaration containing a
 *       template type (TAO_Singleton) with a static member to be
 *       introduced.  In turn, this potentially introduces problems in
 *       MS Windows DLL environments due to the occurance of multiple
 *       singleton instances.  There should only be one!
 */
class TAO_Export TAO_ORB_Table
{
public:

  /// Constructor
  /**
   * @note See the note in the class description for an explanation of
   *       why this constructor is not protected.
   */
  TAO_ORB_Table (void);

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
  TAO_ORB_Core* const * get_orbs( size_t& num_orbs );
  //@}

  /// Obtain the first ORB for the ORB_Core_instance() implementation
  TAO_ORB_Core *first_orb (void);

  /// Return a unique instance
  static TAO_ORB_Table *instance (void);

  /// Set the ORB related to the orb_id as the default ORB and not the
  /// ORB that is first binded.
  void set_default (const char *orb_id);

  /// Method the ORB invokes to specify that it doesnt want to be the
  /// default ORB if there are more than one ORB registered.
  void not_default (const char *orb_id);

  /// Accessor to the underlying table_
  Table * table (void);

private:

  /// Prevent copying
  ACE_UNIMPLEMENTED_FUNC (TAO_ORB_Table (const TAO_ORB_Table &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_ORB_Table &))

private:

  /// Update our list of orbs
  void update_orbs();

  /// Variable to check if the first ORB decides not to be the default
  bool first_orb_not_default_;

  /// The implementation.
  Table table_;

  /// The first ORB created by the user
  TAO_ORB_Core *first_orb_;

  /// List of orbs for get_orbs call
  TAO_ORB_Core **orbs_;
  size_t       num_orbs_;

};

#if defined (__ACE_INLINE__)
# include "tao/ORB_Table.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_ORB_TABLE_H */
