/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Transient_Naming_Context.h
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_TRANSIENT_NAMING_CONTEXT_H
#define TAO_TRANSIENT_NAMING_CONTEXT_H
#include /**/ "ace/pre.h"

#include "Hash_Naming_Context.h"
#include "Entries.h"
#include "ace/Hash_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Transient_Bindings_Map
 *
 * @brief Provides hash-table-based transient storage for name to object
 * bindings in a Naming Context.
 *
 * A thin wrapper on top of ACE_Hash_Map_Manager.  Supports
 * TAO_Bindings_Map interface.  Used by TAO_Transient_Naming_Context.
 */
class TAO_Naming_Export TAO_Transient_Bindings_Map : public TAO_Bindings_Map
{
public:

  /// Underlying data structure - typedef for ease of use.
  typedef ACE_Hash_Map_Manager<TAO_ExtId, TAO_IntId, ACE_Null_Mutex> HASH_MAP;

  // = Initialization and termination methods.

  /// Constructor.
  TAO_Transient_Bindings_Map (size_t hash_table_size);

  /// Destructor.
  virtual ~TAO_Transient_Bindings_Map (void);

  // = Accessors.

  /// Get a reference to the underlying hash map.
  HASH_MAP &map (void);

  /// Return the size of the underlying hash table.
  size_t total_size (void);

  /// Return current number of entries (name bindings) in the
  /// underlying hash map.
  virtual size_t current_size (void);

  // = Name bindings manipulation methods.

  /**
   * Add a binding with the specified parameters to the table.
   * Return 0 on success and -1 on failure, 1 if there already is a
   * binding with <id> and <kind>.
   */
  virtual int bind (const char *id,
                    const char *kind,
                    CORBA::Object_ptr obj,
                    CosNaming::BindingType type);

  /**
   * Overwrite a binding containing <id> and <kind> (or create a new
   * one if one doesn't exist) with the specified parameters.  Return
   * 0 or 1 on success.  Return -1 or -2 on failure. (-2 is returned
   * if the new and old bindings differ in type).
   */
  virtual int rebind (const char *id,
                      const char *kind,
                      CORBA::Object_ptr obj,
                      CosNaming::BindingType type);

  /**
   * Remove a binding containing <id> and <kind> from the table.
   * Return 0 on success and -1 on failure.
   */
  virtual int unbind (const char * id,
                      const char * kind);

  /**
   * Find the binding containing <id> and <kind> in the table, and
   * pass binding's type and object back to the caller by reference.
   * Return 0 on success and -1 on failure.  Note: a 'duplicated' object
   * reference is assigned to <obj>, so the caller is responsible for
   * its deallocation.
   */
  virtual int find (const char * id,
                    const char * kind,
                    CORBA::Object_ptr & obj,
                    CosNaming::BindingType &type);

private:

  /// Helper: factors common code from <bind> and <rebind>.
  int shared_bind (const char *id,
                   const char *kind,
                   CORBA::Object_ptr obj,
                   CosNaming::BindingType type,
                   int rebind);

  /// Hash map used for storage.
  HASH_MAP map_;
};

/**
 * @class TAO_Transient_Naming_Context
 *
 * @brief This class plays a role of a 'ConcreteImplementor' in the
 * Bridge pattern architecture of the CosNaming::NamingContext
 * implementation.
 *
 * This class provides a transient implementation of the
 * NamingContext functionality, i.e., the state is not preserved
 * across process boundaries.  Derives from
 * TAO_Hash_Naming_Context and uses TAO_Transient_Bindings_Map to
 * store name to object bindings.
 */
class TAO_Naming_Export TAO_Transient_Naming_Context : public TAO_Hash_Naming_Context
{

public:

  /// Underlying data structure - typedef for ease of use.
  typedef TAO_Transient_Bindings_Map::HASH_MAP HASH_MAP;

  // = Initialization and termination methods.

  /// Constructor.
  TAO_Transient_Naming_Context (PortableServer::POA_ptr poa,
                                const char *poa_id,
                                size_t hash_table_size
                                = ACE_DEFAULT_MAP_SIZE);

  /// Destructor.
  virtual ~TAO_Transient_Naming_Context (void);

  // = Utility methods.
  /**
   * This utility method factors out the code needed to create a new
   * Transient Naming Context servant and activate it under the
   * specified POA with the specified id.  This function is static so
   * that the code can be used, both from inside the class (e.g., <new_context>),
   * and from outside (e.g., Naming_Utils.cpp).
   */
  static CosNaming::NamingContext_ptr make_new_context (PortableServer::POA_ptr poa,
                                                        const char *poa_id,
                                                        size_t context_size
                                                        ACE_ENV_ARG_DECL);

  // = Methods not implemented in TAO_Hash_Naming_Context.

  /**
   * This operation returns a new naming context implemented by the
   * same naming server in which the operation was invoked.  The
   * context is not bound.
   */
  virtual CosNaming::NamingContext_ptr new_context (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * Returns at most the requested number of bindings <how_many> in
   * <bl>.  If the naming context contains additional bindings, they
   * are returned with a BindingIterator.  In the naming context does
   * not contain any additional bindings <bi> returned as null.
   */
  virtual void list (CORBA::ULong how_many,
                     CosNaming::BindingList_out &bl,
                     CosNaming::BindingIterator_out &bi
                     ACE_ENV_ARG_DECL);

protected:

  /// Counter used for generation of POA ids for children Naming
  /// Contexts.
  ACE_UINT32 counter_;

  /**
   * A pointer to the underlying data structure used to store name
   * bindings. While our superclass (TAO_Hash_Naming_Context) also
   * maintains a pointer to the data structure, keeping this pointer
   * around saves us from the need to downcast when invoking
   * non-virtual methods.
   */
  TAO_Transient_Bindings_Map *transient_context_;
};

#include /**/ "ace/post.h"
#endif /* TAO_TRANSIENT_NAMING_CONTEXT_H */
