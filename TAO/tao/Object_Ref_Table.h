// -*- C++ -*-

//=============================================================================
/**
 *  @file     Object_Ref_Table.h
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_OBJECT_REF_TABLE_H
#define TAO_OBJECT_REF_TABLE_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CORBA_String.h"
#include "tao/Object.h"
#include /**/ "tao/TAO_Export.h"

#include "ace/Array_Map.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  typedef ACE_Array_Map<CORBA::String_var,
                        CORBA::Object_var,
                        TAO::String_Var_Equal_To> Table;

  typedef Table::iterator iterator;

  /// Constructor
  TAO_Object_Ref_Table ();

  /**
   * Register an object reference with the table, and map the given
   * ID to it.
   * @retval 0 Success
   * @retval -1 Duplicate id if @c rebind is false
   */
  int register_initial_reference (const char * id,
                                  CORBA::Object_ptr obj,
                                  bool rebind = false);

  /**
   * Unregister an object reference with the table
   */
  CORBA::Object_ptr unregister_initial_reference (const char * id);

  /// Return the object reference associated with the given ID.
  /// A duplicate is returned.
  CORBA::Object_ptr resolve_initial_reference (const char * id);

  /// Explicitly destroy the contents of the object reference table.
  void destroy ();

  /**
   * @name Forward Iterators
   */
  //@{
  iterator begin ();
  iterator end ();
  //@}

  /// Return the current size of the underlying table.
  size_t current_size () const;

private:
  /**
   * @name The canonical ACE_Map methods
   */
  //@{
  int bind_i (const char *orb_id, CORBA::Object_ptr obj);
  CORBA::Object_ptr find_i (const char *orb_id); // Returns a duplicate.
  int unbind_i (const char *orb_id);
  //@}

private:
  TAO_Object_Ref_Table (const TAO_Object_Ref_Table &) = delete;
  void operator= (const TAO_Object_Ref_Table &) = delete;
  TAO_Object_Ref_Table (TAO_Object_Ref_Table &&) = delete;
  void operator= (TAO_Object_Ref_Table &&) = delete;

private:
  /// The implementation.
  Table table_;

  /// Table synchronization lock.
  TAO_SYNCH_MUTEX lock_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#ifdef __ACE_INLINE__
# include "tao/Object_Ref_Table.inl"
#endif  /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_OBJECT_REF_TABLE_H */
