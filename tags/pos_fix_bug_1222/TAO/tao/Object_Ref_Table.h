// -*- C++ -*-

//=============================================================================
/**
 *  @file     Object_Ref_Table.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_OBJECT_REF_TABLE_H
#define TAO_OBJECT_REF_TABLE_H

#include "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Functor.h"


/**
 * @class TAO_Object_Ref_Table
 *
 * @brief Keep a table de-stringified object references registered
 * with the ORB.
 *
 * The class is necessary to allow local objects to be accessible via
 * the resolve_initial_references() mechanism.  Since local object
 * references cannot be stringified, they cannot be placed into the
 * initial reference map that maps object key/name to stringified
 * object reference.  Hence, another table is needed.
 *
 * @note
 * The stringified reference table is still needed since it is
 * sometimes necessary to delay de-stringification of an IOR until it
 * is needed.  For example, "corbaname" may return different results
 * on each use.
 */
class TAO_Export TAO_Object_Ref_Table
{
public:

  typedef ACE_Hash_Map_Manager_Ex<const char *, CORBA::Object_ptr, ACE_Hash<const char *>, ACE_Equal_To<const char *>, TAO_SYNCH_MUTEX> Table;
  typedef Table::iterator Iterator;

  /// Constructor
  TAO_Object_Ref_Table (void);

  /// Destructor
  ~TAO_Object_Ref_Table (void);

  /// Register an object reference with the table, and map the given
  /// ID to it.
  void register_initial_reference (const char * id,
                                   CORBA::Object_ptr obj
                                   ACE_ENV_ARG_DECL);

  /// Return the object reference associated with the given ID.
  /// A duplicate is returned.
  CORBA::Object_ptr resolve_initial_references (
    const char * id
    ACE_ENV_ARG_DECL);

  /// Explicitly destroy the contents of the object reference table.
  void destroy (void);

  /**
   * @name Forward Iterators
   */
  //@{
  Iterator begin (void);
  Iterator end (void);
  //@}

  /// Return the current size of the underlying table.
  size_t current_size (void) const;

private:

  /**
   * @name The canonical ACE_Map methods
   */
  //@{
  int bind (const char *orb_id, CORBA::Object_ptr obj);
  CORBA::Object_ptr find (const char *orb_id); // Returns a duplicate.
  int unbind (const char *orb_id);
  //@}

private:

  /// Prevent copying
  ACE_UNIMPLEMENTED_FUNC (TAO_Object_Ref_Table (const TAO_Object_Ref_Table &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Object_Ref_Table &))

private:

  /// The implementation.
  Table table_;

};

#include "ace/post.h"

#endif  /* TAO_OBJECT_REF_TABLE_H */
