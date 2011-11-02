/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Storable_Naming_Context.h
 *
 *  $Id$
 *
 *  @author Bruce Trask <trask_b@ociweb.com>
 */
//=============================================================================


#ifndef TAO_STORABLE_NAMING_CONTEXT_H
#define TAO_STORABLE_NAMING_CONTEXT_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Naming/Hash_Naming_Context.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Auto_Ptr.h"

#include "orbsvcs/Naming/Storable.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Naming_Serv_Export TAO_Storable_IntId
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_Storable_IntId (void);

  /// Constructor.
  TAO_Storable_IntId (/* in */ const char * ior,
                        CosNaming::BindingType type /* = CosNaming::nobject */);

  /// Copy constructor.
  TAO_Storable_IntId (const TAO_Storable_IntId & rhs);

  /// Destructor.
  ~TAO_Storable_IntId (void);

  /// Assignment operator.
  void operator= (const TAO_Storable_IntId & rhs);

  // = Data members.

  /// Stringified IOR to be stored in a Persistent Naming Context.
  CORBA::String_var  ref_;

  /// Binding type for <ref_>.
  CosNaming::BindingType type_;
};

class TAO_Naming_Serv_Export TAO_Storable_ExtId
{
public:
  // = Initialization and termination methods.

  /// Constructor.
  TAO_Storable_ExtId (void);

  /// Constructor.
  TAO_Storable_ExtId (/* in */ const char *id,
                        /* in */ const char *kind);

  /// Copy constructor.
  TAO_Storable_ExtId (const TAO_Storable_ExtId & rhs);

  /// Destructor.
  ~TAO_Storable_ExtId (void);

  // = Assignment and comparison methods.

  /// Assignment operator (does copy memory).
  void operator= (const TAO_Storable_ExtId & rhs);

  /// Equality comparison operator (must match both id_ and kind_).
  bool operator== (const TAO_Storable_ExtId &rhs) const;

  /// Inequality comparison operator.
  bool operator!= (const TAO_Storable_ExtId &rhs) const;

  /// <hash> function is required in order for this class to be usable by
  /// ACE_Hash_Map_Manager.
  u_long hash (void) const;

  // = Data members.

  /// <id> portion of the name to be associated with some object
  /// reference in a Storable Naming Context.
  CORBA::String_var id_;

  /// <kind> portion of the name to be associated with some object
  /// reference in a Storable Naming Context.
  CORBA::String_var kind_;

  // Accessors.
  // follow the mapping rules!

  const char * id (void);
  const char * kind (void);

};


/**
 * @class TAO_Storable_Bindings_Map
 *
 * @brief Provides hash-table-based transient storage for name to object
 * bindings in a Naming Context.
 *
 * A thin wrapper on top of ACE_Hash_Map_Manager.  Supports
 * TAO_Bindings_Map interface.  Used by TAO_Transient_Naming_Context.
 */
class TAO_Naming_Serv_Export TAO_Storable_Bindings_Map : public TAO_Bindings_Map
{
public:

  /// Underlying data structure - typedef for ease of use.
  typedef ACE_Hash_Map_Manager<TAO_Storable_ExtId,
                               TAO_Storable_IntId,
                               ACE_Null_Mutex> HASH_MAP;

  // = Initialization and termination methods.

  /// Constructor.
  TAO_Storable_Bindings_Map (size_t hash_table_size, CORBA::ORB_ptr orb);

  /// Destructor.
  virtual ~TAO_Storable_Bindings_Map (void);

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
  virtual int unbind (const char * id, const char * kind);

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

  CORBA::ORB_var orb_;

};

/**
 * @class TAO_Storable_Naming_Context
 *
 * @brief This class plays a role of a 'ConcreteImplementor' in the
 * Bridge pattern architecture of the CosNaming::NamingContext
 * implementation.
 *
 * This class provides a  implementation of the
 * NamingContext functionality, i.e., the state can be preserved
 * across process boundaries.  Derives from
 * TAO_Hash_Naming_Context and uses TAO_Storable_Bindings_Map to
 * store name to object bindings.
 */
class TAO_Naming_Serv_Export TAO_Storable_Naming_Context : public TAO_Hash_Naming_Context
{

public:

  /// Underlying data structure - typedef for ease of use.
  typedef TAO_Storable_Bindings_Map::HASH_MAP HASH_MAP;

  // = Initialization and termination methods.

  /// Constructor.
  TAO_Storable_Naming_Context (CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               const char *poa_id,
                               TAO_Naming_Service_Persistence_Factory *factory,
                               const ACE_TCHAR *persistence_directory,
                               size_t hash_table_size = ACE_DEFAULT_MAP_SIZE);

  /// Destructor.
  virtual ~TAO_Storable_Naming_Context (void);

  // = Utility methods.
  /**
   * This utility method factors out the code needed to create a new
   * Storable Naming Context servant and activate it under the
   * specified POA with the specified id.  This function is static so
   * that the code can be used, both from inside the class (e.g.,
   * <new_context>), and from outside (e.g., Naming_Utils.cpp).
   */
  static CosNaming::NamingContext_ptr make_new_context (
                               CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               const char *poa_id,
                               size_t context_size,
                               TAO_Naming_Service_Persistence_Factory *factory,
                               const ACE_TCHAR *persistence_directory,
                               TAO_Storable_Naming_Context **new_context);

  // = Methods not implemented in TAO_Hash_Naming_Context.

  static CosNaming::NamingContext_ptr recreate_all(
                              CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr poa,
                              const char *poa_id,
                              size_t context_size,
                              int reentering,
                              TAO_Naming_Service_Persistence_Factory *factory,
                              const ACE_TCHAR *persistence_directory,
                              int use_redundancy);


  /**
   * This operation returns a new naming context implemented by the
   * same naming server in which the operation was invoked.  The
   * context is not bound.
   */
  virtual CosNaming::NamingContext_ptr new_context (void);

  /**
   * Returns at most the requested number of bindings <how_many> in
   * <bl>.  If the naming context contains additional bindings, they
   * are returned with a BindingIterator.  In the naming context does
   * not contain any additional bindings <bi> returned as null.
   */
  virtual void list (CORBA::ULong how_many,
                     CosNaming::BindingList_out &bl,
                     CosNaming::BindingIterator_out &bi);


  virtual void rebind (const CosNaming::Name& n,
                       CORBA::Object_ptr obj);

  /**
   * Create a binding for name <n> and object <obj> in the naming
   * context.  Compound names are treated as follows: ctx->bind (<c1;
   * c2; c3; cn>, obj) = (ctx->resolve (<c1; c2; cn-1>))->bind (<cn>,
   * obj) if the there already exists a binding for the specified
   * name, <AlreadyBound> exception is thrown.  Naming contexts should
   * be bound using <bind_context> and <rebind_context> in order to
   * participate in name resolution later.
   */
  virtual void bind (const CosNaming::Name &n,
                     CORBA::Object_ptr obj);


  /**
   * This is the version of <bind> specifically for binding naming
   * contexts, so that they will participate in name resolution when
   * compound names are passed to be resolved.
   */
  virtual void bind_context (const CosNaming::Name &n,
                             CosNaming::NamingContext_ptr nc);

  /**
   * This is a version of <rebind> specifically for naming contexts,
   * so that they can participate in name resolution when compound
   * names are passed.
   */
   virtual void rebind_context (const CosNaming::Name &n,
                                CosNaming::NamingContext_ptr nc);

  /**
   * Return object reference that is bound to the name.  Compound name
   * resolve is defined as follows: ctx->resolve (<c1; c2; cn>) =
   * ctx->resolve (<c1; c2 cn-1>)->resolve (<cn>) The naming service
   * does not return the type of the object.  Clients are responsible
   * for "narrowing" the object to the appropriate type.
   */
  virtual CORBA::Object_ptr resolve (const CosNaming::Name &n);

  /**
   * Remove the name binding from the context.  When compound names
   * are used, unbind is defined as follows: ctx->unbind (<c1; c2;
   * cn>) = (ctx->resolve (<c1; c2; cn-1>))->unbind (<cn>)
   */
  virtual void unbind (const CosNaming::Name &n);

  /**
   * This operation creates a new context and binds it to the name
   * supplied as an argument.  The newly-created context is
   * implemented by the same server as the context in which it was
   * bound (the name argument excluding the last component).
   */
  virtual CosNaming::NamingContext_ptr bind_new_context (
                                const CosNaming::Name &n);

  /**
   * Delete the naming context.  The user should take care to <unbind> any
   * bindings in which the given context is bound to some names, to
   * avoid dangling references when invoking <destroy> operation.
   * NOTE: <destory> is a no-op on the root context.
   * NOTE: after <destroy> is invoked on a Naming Context, all
   * BindingIterators associated with that Naming Context are also destroyed.
   */
  virtual void destroy (void);

protected:

  /// Global counter used for generation of POA ids for children Naming
  /// Contexts.
  static ACE_UINT32 gcounter_;

  /// Counter used for generation of transients
  ACE_UINT32 counter_;

  /**
   * A pointer to the underlying data structure used to store name
   * bindings. While our superclass (TAO_Hash_Naming_Context) also
   * maintains a pointer to the data structure, keeping this pointer
   * around saves us from the need to downcast when invoking
   * non-virtual methods.
   */
  TAO_Storable_Bindings_Map *storable_context_;

  CORBA::ORB_var orb_;

  ACE_CString name_;

  PortableServer::POA_var poa_;

  TAO_Naming_Service_Persistence_Factory *factory_;

  /// The directory in which to store the files
  ACE_CString persistence_directory_;

  /// Save the hash table initial size
  size_t hash_table_size_;

  /// Disk time that match current memory state
  time_t last_changed_;

  /// Flag to tell use whether we are redundant or not
  static int redundant_;

  static const char * root_name_;

  /// The pointer to the global file used to allocate new contexts
  static ACE_Auto_Ptr<TAO_Storable_Base> gfl_;

/**
 * @class File_Open_Lock_and_Check
 *
 * @brief Helper class for the TAO_Storable_Naming_Context.
 *
 * Guard class for the TAO_Storable_Naming_Context.  It opens
 * a file for read/write and sets a lock on it.  It then checks
 * if the file has changed and re-reads it if it has.
 *
 * The destructor insures that the lock gets released.
 *
 * <pre>
 * How to use this class:
 *   File_Open_Lock_and_Check flck(this, name_len > 1 ? "r" : "rw");
 * </pre>
 */
class File_Open_Lock_and_Check
{
public:

  /// Constructor - we always need the object which we guard.
  File_Open_Lock_and_Check(TAO_Storable_Naming_Context * context,
                                const char * mode);

  /// Destructor
  ~File_Open_Lock_and_Check(void);

  /// Releases the lock, closes the file, and deletes the I/O stream.
  void release(void);

  /// Returns the stream to read/write on
  TAO_Storable_Base & peer(void);

private:
  /// Default constructor
  File_Open_Lock_and_Check(void);

  /// A flag to keep us from trying to close things more than once.
  int closed_;

  /// We need to save the pointer to our parent for cleaning up
  TAO_Storable_Naming_Context * context_;

  /// The pointer to the actual file I/O (bridge pattern)
  TAO_Storable_Base *fl_;

  /// The flags that we were opened with
  int rwflags_;

  /// Symbolic values for the flags in the above
  enum{ mode_write = 1, mode_read = 2, mode_create = 4 };
}; // end of embedded class File_Open_Lock_and_Check

  friend class File_Open_Lock_and_Check;

  int load_map(File_Open_Lock_and_Check *flck);

  void Write(TAO_Storable_Base& wrtr);

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_STORABLE_NAMING_CONTEXT_H */
