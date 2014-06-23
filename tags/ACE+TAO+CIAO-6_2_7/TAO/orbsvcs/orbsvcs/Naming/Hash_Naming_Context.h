// -*- C++ -*-

//=============================================================================
/**
 *  @file   Hash_Naming_Context.h
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_HASH_NAMING_CONTEXT_H
#define TAO_HASH_NAMING_CONTEXT_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Naming/Naming_Context_Interface.h"
#include "orbsvcs/Naming/naming_serv_export.h"

#include "ace/Recursive_Thread_Mutex.h"
#include "ace/SString.h"

// This is to remove "inherits via dominance" warnings from MSVC.
#if defined (_MSC_VER)
# pragma warning (disable : 4250)
#endif /* _MSC_VER */

// Note: 'interface' has been defined as struct on WinCE platform and
//       gives a compiler error.  This undef has been found harmless on
//       Windows and solaris platforms; however, if this generates
//       error, then proper ifdef must be added around following block.
#if defined (interface)
#undef interface
#endif  // interface

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Bindings_Map
 *
 * @brief This abstract base class defines an interface for hash-based
 * data structures used in implementations of NamingContext
 * (i.e., TAO_Transient_Naming_Context and TAO_Persistent_Naming_Context)
 *
 * Define an interface for several hash-based data structures, so
 * that we can write some code that would work with any of them,
 * i.e., TAO_Hash_Naming_Context.
 */
class TAO_Naming_Serv_Export TAO_Bindings_Map
{

public:

  /// Destructor.
  virtual ~TAO_Bindings_Map (void);

  /// Return current number of entries (name bindings) in the
  /// underlying hash map.
  virtual size_t current_size (void) = 0;

  /**
   * Add a binding with the specified parameters to the table.
   * Return 0 on success and -1 on failure, 1 if there already is a
   * binding with <id> and <kind>.
   */
  virtual int bind (const char *id,
                    const char *kind,
                    CORBA::Object_ptr obj,
                    CosNaming::BindingType type) = 0;

  /**
   * Overwrite a binding containing <id> and <kind> (or create a new
   * one if one doesn't exist) with the specified parameters.  Returns
   * -1 on failure.
   */
  virtual int rebind (const char *id,
                      const char *kind,
                      CORBA::Object_ptr obj,
                      CosNaming::BindingType type) = 0;

  /// Remove a binding containing <id> and <kind> from the table.
  /// Return 0 on success and -1 on failure.
  virtual int unbind (const char * id,
                      const char * kind) = 0;

  /**
   * Find the binding containing <id> and <kind> in the table, and
   * pass binding's type and object back to the caller by reference.
   * Return 0 on success and -1 on failure.   Note: a 'duplicated' object
   * reference is assigned to <obj>, so the caller is responsible for
   * its deallocation.
   */
  virtual int find (const char * id,
                    const char * kind,
                    CORBA::Object_ptr & obj,
                    CosNaming::BindingType &type) = 0;

};

/**
 * @class TAO_Hash_Naming_Context
 *
 * @brief This class factors out common code for two 'ConcreteImplementors'
 * in the Bridge pattern architecture of the CosNaming::NamingContext
 * implementation.
 *
 * This class contains 'algorithm' code that is common to two
 * hash-table-based implementations of the NamingContext:
 * TAO_Transient_Naming_Context and TAO_Persistent_Naming_Context.
 * To help achieve this 'templatization', we use the abstract base
 * class TAO_Bindings_Map, which provides a common interface to the data structures
 * used in TAO_Persistent_Namng_Context and TAO_Transient_Naming_Context.
 */
class TAO_Naming_Serv_Export TAO_Hash_Naming_Context :public TAO_Naming_Context_Impl
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_Hash_Naming_Context (PortableServer::POA_ptr poa,
                           const char *poa_id);

  /// Set our <interface_> pointer.
  void interface (TAO_Naming_Context *i);

  /// Destructor.
  virtual ~TAO_Hash_Naming_Context (void);

  // = Accessors.

  /// Get the pointer to our <interface>.
  TAO_Naming_Context *interface (void);

  /// Returns true if this Naming Context is a root Naming Context for
  /// the server, and false otherwise.
  int root (void);

  /// Returns true if this context had <destroy> operation invoked on
  /// it, and false otherwise.
  int destroyed (void);

  // = CosNaming::NamingContext idl interface methods.

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
   * This is similar to <bind> operation above, except for when the
   * binding for the specified name already exists in the specified
   * context.  In that case, the existing binding is replaced with the
   * new one.
   */
  virtual void rebind (const CosNaming::Name &n,
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
  virtual CosNaming::NamingContext_ptr bind_new_context (const CosNaming::Name &n);

  /**
   * Delete the naming context.  The user should take care to <unbind> any
   * bindings in which the given context is bound to some names, to
   * avoid dangling references when invoking <destroy> operation.
   * NOTE: <destory> is a no-op on the root context.
   * NOTE: after <destroy> is invoked on a Naming Context, all
   * BindingIterators associated with that Naming Context are also destroyed.
   */
  virtual void destroy (void);

  /// Returns the Default POA of this Servant object
  virtual PortableServer::POA_ptr _default_POA (void);

  TAO_SYNCH_RW_MUTEX &lock (void);

protected:
  // = Helper method used by other methods.

  /**
   * <get_context> is used by methods that need to resolve a compound
   * name before performing the actual operation (e.g., bind, unbind,
   * etc.)  <get_context> takes a full name (including the last
   * component that doesn't need to be resolved), and returns a
   * pointer to the target context.
   */
  CosNaming::NamingContext_ptr get_context (const CosNaming::Name &name);

  /**
   * Pointer to the data structure used to store this Naming Context's
   * bindings.  <context_> is initialized with a concrete data
   * structure by subclasses, which know which data structure to use.
   */
  TAO_Bindings_Map *context_;

  /**
   * Pointer to the <interface> object for which we serve as a
   * <concrete implementation>, i.e., the object that delegates to us
   * all client CosNaming::NamingContext CORBA calls.
   * We need this pointer for reference counting.
   */
  TAO_Naming_Context *interface_;

  /// Lock used to serialize access to the underlying data structure.
  TAO_SYNCH_RW_MUTEX lock_;

  /**
   * Flag indicating whether this Naming Context is no longer valid.
   * This flag is necessary because immediate destruction
   * might not be possible if there are pending requests on this servant
   * in the POA.
   */
  int destroyed_;

  /// POA we are registered with.
  PortableServer::POA_var poa_;

  /**
   * ID with which we are registered with <poa_>.
   * Note, if <poa_id_> is equivalent to TAO_ROOT_NAMING_CONTEXT, then this Naming Context
   * is the root Naming Context for the server, i.e., it is un<destroy>able.
   */
  ACE_CString poa_id_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_HASH_NAMING_CONTEXT_H */
