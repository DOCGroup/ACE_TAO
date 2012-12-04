// -*- C++ -*-

//=============================================================================
/**
 *  @file   Naming_Context_Interface.h
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_NAMING_CONTEXT_INTERFACE_H
#define TAO_NAMING_CONTEXT_INTERFACE_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosNamingS.h"

#include "orbsvcs/Naming/naming_serv_export.h"
#include "ace/Null_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Naming_Context_Impl;

// This is to remove "inherits via dominance" warnings from MSVC.
#if defined (_MSC_VER)
# pragma warning (disable : 4250)
#endif /* _MSC_VER */

/**
 * @class TAO_Naming_Context
 *
 * @brief This class plays a role of the 'Abstraction' (aka 'Interface')
 * in the Bridge pattern architecture of the CosNaming::NamingContext
 * implementation.
 *
 * This class simply forwards all client requests to a concrete
 * NamingContext implementation through its <impl_> pointer.  See
 * README file for more info.  Comments for the idl methods
 * describe methods semantics - actual actions are carried out by
 * concrete implementors.
 */

class TAO_Naming_Serv_Export TAO_Naming_Context :
  public virtual POA_CosNaming::NamingContextExt
{
public:

  // = Initialization and termination methods.
  /// Constructor.  Initializes <impl_> with a concrete implementation.
  TAO_Naming_Context (TAO_Naming_Context_Impl *impl);

  /// Destructor.
  ~TAO_Naming_Context (void);

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
   * This operation returns a new naming context implemented by the
   * same naming server in which the operation was invoked.  The
   * context is not bound.
   */
  virtual CosNaming::NamingContext_ptr new_context ();

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

  /**
   * Returns at most the requested number of bindings <how_many> in
   * <bl>.  If the naming context contains additional bindings, they
   * are returned with a BindingIterator.  In the naming context does
   * not contain any additional bindings <bi> returned as null.
   */
  virtual void list (CORBA::ULong how_many,
                     CosNaming::BindingList_out bl,
                     CosNaming::BindingIterator_out bi);

  /**
   * Stringify the name using '\' as the escape character. The
   * characters '.' , '/' and '\' are to be escaped. If the input name
   * is invalid i.e. if the number of characters in the name is zero,
   * an InvalidName exception is to be raised.
   */
  virtual char * to_string (const CosNaming::Name &n);

  /**
   * The in parameter is an stringified name. This function removes the
   * escape character '\' and destringifies the stringified name and returns
   * it.
   */
  virtual CosNaming::Name * to_name (const char *sn);

  /**
   * The in parameter addr refers to the address of the naming context
   * and sn refers to the strigified name of the object in that
   * context. This function returns a fully formed URL string like
   * iiopname://1.1@myhost.555xyz.com:9999/a/b/c
   */
  virtual char * to_url ( const char * addr,
                          const char * sn);

  /**
   * Similar to <resolve> as in the CosNaming::NamingContext interface.
   * It accepts a strigified name as an argument instead of a Name.
   */
  virtual CORBA::Object_ptr resolve_str (const char * n);

  /**
   * Mark the implementation stale state for replicated
   * persistence support.
   */
  void stale (bool value);

  /// Returns the Default POA of this Servant object
  virtual PortableServer::POA_ptr _default_POA (void);

private:

  enum Hint
    {
      HINT_ID,
      HINT_KIND
    };

  /**
   * This private function is used as a helper to <to_name>. It reads
   * character by character from 'src' and depending on the character,
   * either assigns it to 'dest' or returns back to the calling
   * function. If the character is a seperator between the 'id' and
   * 'kind' fields or a seperator between two name components, the
   * control is returned back to the calling function <to_name>.
   */
  void to_name_helper (char *dest, const char*& src, Hint hint);

  /**
   * This method functions similar to <to_name_helper>. If the
   * character read is '.' or '/' or '\', an escape character '\' is
   * prepended before the character.
   */
  void to_string_helper_assign (char * &k, const char * &src);

  /**
   * This method helps count the number of characters in 'src' so
   * that memory can be allocated for the return parameter. For
   * all '.' , '/' and '\', the count is incremented by 'one' to
   * account for the escape character that needs to be
   * added. Seperators between 'id' and 'kind' as well as seperators
   * between the name components are also counted.
   */
  void to_string_helper_length (CORBA::ULong &len, const char * &src);

  /// Return 1 if the character is alphanumeric or a non-scaped
  /// punctuation.
  static int to_url_is_alnum_or_punctuation (char c);

  /// Validate the to_url() method input, and compute the size of the
  /// returned URL address.
  static size_t to_url_validate_and_compute_size (const char *add,
                                                  const char *sn);

protected:

  /// A concrete implementor of the NamingContext functions.
  TAO_Naming_Context_Impl *impl_;
};

/**
 * @class TAO_Naming_Context_Impl
 *
 * @brief This abstract base class plays a role of the 'Implementor' in the Bridge
 * pattern architecture of the NamingContext implementation.
 *
 * Subclasses of TAO_Naming_Context_Impl provide concrete
 * implementations of the NamingContext functionality.
 */
class TAO_Naming_Serv_Export TAO_Naming_Context_Impl
{

public:

  /// Destructor.
  virtual ~TAO_Naming_Context_Impl (void);

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
                     CORBA::Object_ptr obj) = 0;

  /**
   * This is similar to <bind> operation above, except for when the
   * binding for the specified name already exists in the specified
   * context.  In that case, the existing binding is replaced with the
   * new one.
   */
  virtual void rebind (const CosNaming::Name &n,
                       CORBA::Object_ptr obj) = 0;

  /**
   * This is the version of <bind> specifically for binding naming
   * contexts, so that they will participate in name resolution when
   * compound names are passed to be resolved.
   */
  virtual void bind_context (const CosNaming::Name &n,
                             CosNaming::NamingContext_ptr nc) = 0;

  /**
   * This is a version of <rebind> specifically for naming contexts,
   * so that they can participate in name resolution when compound
   * names are passed.
   */
  virtual void rebind_context (const CosNaming::Name &n,
                               CosNaming::NamingContext_ptr nc) = 0;

  /**
   * Return object reference that is bound to the name.  Compound name
   * resolve is defined as follows: ctx->resolve (<c1; c2; cn>) =
   * ctx->resolve (<c1; c2 cn-1>)->resolve (<cn>) The naming service
   * does not return the type of the object.  Clients are responsible
   * for "narrowing" the object to the appropriate type.
   */
  virtual CORBA::Object_ptr resolve (const CosNaming::Name &n) = 0;

  /**
   * Remove the name binding from the context.  When compound names
   * are used, unbind is defined as follows: ctx->unbind (<c1; c2;
   * cn>) = (ctx->resolve (<c1; c2; cn-1>))->unbind (<cn>)
   */
  virtual void unbind (const CosNaming::Name &n) = 0;

  /**
   * This operation returns a new naming context implemented by the
   * same naming server in which the operation was invoked.  The
   * context is not bound.
   */
  virtual CosNaming::NamingContext_ptr new_context (void) = 0;

  /**
   * This operation creates a new context and binds it to the name
   * supplied as an argument.  The newly-created context is
   * implemented by the same server as the context in which it was
   * bound (the name argument excluding the last component).
   */
  virtual CosNaming::NamingContext_ptr bind_new_context (const CosNaming::Name &n) = 0;

  /**
   * Delete the naming context.  The user should take care to <unbind> any
   * bindings in which the given context is bound to some names, to
   * avoid dangling references when invoking <destroy> operation.
   * NOTE: <destory> is a no-op on the root context.
   * NOTE: after <destroy> is invoked on a Naming Context, all
   * BindingIterators associated with that Naming Context are also destroyed.
   */
  virtual void destroy (void) = 0;

  /**
   * Returns at most the requested number of bindings <how_many> in
   * <bl>.  If the naming context contains additional bindings, they
   * are returned with a BindingIterator.  In the naming context does
   * not contain any additional bindings <bi> returned as null.
   */
  virtual void list (CORBA::ULong how_many,
                     CosNaming::BindingList_out &bl,
                     CosNaming::BindingIterator_out &bi) = 0;

  /// Returns the Default POA of this Servant object
  virtual PortableServer::POA_ptr _default_POA (void) = 0;

  /**
   * Set the stale flag for replicated persistence support.
   */
  virtual void stale (bool value);

  /**
   * Query if the the implementation is stale for replicated
   * persistence support.
   */
  virtual bool stale (void);

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_NAMING_CONTEXT_INTERFACE_H */
