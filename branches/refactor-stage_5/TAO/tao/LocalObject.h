// -*- C++ -*-

//=============================================================================
/**
 * @file LocalObject.h
 *
 * $Id$
 *
 * Header file for CORBA's base "LocalObject" type.
 *
 * A "LocalObject" is an entity that can be the target of a local
 * invocation.  See the CORBA Component Model specification for
 * details.
 *
 * @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_CORBA_LOCALOBJECT_H
#define TAO_CORBA_LOCALOBJECT_H

#include /**/ "ace/pre.h"

#include "Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Object_KeyC.h"

namespace CORBA
{
  class LocalObject;
  typedef TAO_Pseudo_Var_T<LocalObject> LocalObject_var;
  typedef TAO_Pseudo_Out_T<LocalObject, LocalObject_var> LocalObject_out;

  /**
   * @class LocalObject
   *
   * @note NW: It is not clear whether minimum CORBA should support
   * LocalObject or not.  I think it should.
   */
  class TAO_Export LocalObject : public virtual CORBA::Object
  {
  public:

    /// Destructor
    virtual ~LocalObject (void);

    /// Increment the ref count
    static LocalObject_ptr _duplicate (LocalObject_ptr obj);

    /// Return a NIL object
    static LocalObject_ptr _nil (void);

    /**
     * @todo Narrowing a LocalObject to a CORBA::Object is broken
     * right now.  The solution seems to be making CORBA::Object an
     * abstract base class and create a CORBA::RemoteObject for
     * regular object.  Or, even easier, add a @c is_local member into
     * CORBA::Object.  I'll take the easier route for now.
     */
    static LocalObject_ptr _narrow (CORBA::Object_ptr obj
                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// No-op it is just here to simplify some templates.
    static LocalObject_ptr _unchecked_narrow (
        CORBA::Object_ptr obj
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );

#if (TAO_HAS_MINIMUM_CORBA == 0)

    /// Always returns false.
    virtual CORBA::Boolean _non_existent (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      );

    /// Throws CORBA::NO_IMPLEMENT.
    virtual CORBA::ImplementationDef_ptr _get_implementation (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      );

    /// Gets info about object from the Interface Repository.
    virtual CORBA::InterfaceDef_ptr _get_interface (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      );

    /// Throws NO_IMPLEMENT.
    virtual CORBA::Object_ptr _get_component (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      );

    virtual void _create_request (CORBA::Context_ptr ctx,
                                  const char * operation,
                                  CORBA::NVList_ptr arg_list,
                                  CORBA::NamedValue_ptr result,
                                  CORBA::Request_ptr & request,
                                  CORBA::Flags req_flags
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    virtual void _create_request (CORBA::Context_ptr ctx,
                                  const char * operation,
                                  CORBA::NVList_ptr arg_list,
                                  CORBA::NamedValue_ptr result,
                                  CORBA::ExceptionList_ptr exclist,
                                  CORBA::ContextList_ptr ctxtlist,
                                  CORBA::Request_ptr & request,
                                  CORBA::Flags req_flags
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// Throws NO_IMPLEMENT.
    virtual CORBA::Request_ptr _request (const char * operation
                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS);

#endif /* TAO_HAS_MINIMUM_CORBA */

#if (TAO_HAS_CORBA_MESSAGING == 1)

    /// Throws CORBA::NO_IMPLEMENT.
    CORBA::Policy_ptr _get_policy (CORBA::PolicyType type
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// Throws CORBA::NO_IMPLEMENT.
    CORBA::Policy_ptr _get_client_policy (CORBA::PolicyType type
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// Throws CORBA::NO_IMPLEMENT.
    CORBA::Object_ptr _set_policy_overrides (
        const CORBA::PolicyList & policies,
        CORBA::SetOverrideType set_add
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );

    /// Throws CORBA::NO_IMPLEMENT.
    CORBA::PolicyList * _get_policy_overrides (
        const CORBA::PolicyTypeSeq & types
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );

    /// Throws CORBA::NO_IMPLEMENT.
    CORBA::Boolean _validate_connection (
        CORBA::PolicyList_out inconsistent_policies
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

    /**
     * Return a hash value for this object.  The hash is based on the
     * address of the object.  On non-32 bit platforms, the hash may
     * be non-unique.
     */
    virtual CORBA::ULong _hash (CORBA::ULong maximum
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /**
     * Try to determine if this object is the same as @c other_obj.
     * This method relies on the representation of the object
     * reference's private state.  Since that changes easily (when
     * different ORB protocols are in use) there is no default
     * implementation.
     */
    virtual CORBA::Boolean _is_equivalent (CORBA::Object_ptr other_obj
                                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC (());

    // = Reference count managment.
    /// Increment the reference count.
    virtual void _add_ref (void);

    /// Decrement the reference count.
    virtual void _remove_ref (void);

    // = TAO extensions

    /// Throws CORBA::NO_IMPLEMENT.
    virtual TAO::ObjectKey * _key (ACE_ENV_SINGLE_ARG_DECL);

    /// Useful for template programming.
    typedef LocalObject_ptr _ptr_type;
    typedef LocalObject_var _var_type;

  protected:

    /// Default constructor.
    /**
     * Make it protected to prevent instantiation of this class.
     */
    LocalObject (void);

  private:

    /**
     * @name Unimplemented methods
     */
    //@{
    LocalObject (const LocalObject &);
    LocalObject & operator = (const LocalObject &);
    //@}

  };
}   // End CORBA namespace

/**
 * @class TAO_Local_RefCounted_Object
 *
 * @brief This class adds default reference counting to local
 *        objects.
 *
 * This is proprietary convenience class that implements reference
 * counting in locality constrained objects.
 */
class TAO_Export TAO_Local_RefCounted_Object
  : public virtual CORBA::LocalObject
{
public:

  /// Increment reference count.
  virtual void _add_ref (void);

  /// Decrement reference count.
  virtual void _remove_ref (void);

protected:

  // Default constructor.
  /**
   * Make it protected to prevent instantiation of this class.
   */
  TAO_Local_RefCounted_Object (void);

private:

  /**
   * @name Unimplemented methods
   */
  //@{
  TAO_Local_RefCounted_Object (const TAO_Local_RefCounted_Object &);
  TAO_Local_RefCounted_Object & operator = (
      const TAO_Local_RefCounted_Object &
    );
  //@}

protected:

  /// Mutex to protect reference count.
  TAO_SYNCH_MUTEX refcount_lock_;

  /// Number of outstanding references to this object.
  CORBA::ULong refcount_;
};


#if defined (__ACE_INLINE__)
# include "LocalObject.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_CORBA_LOCALOBJECT_H */
