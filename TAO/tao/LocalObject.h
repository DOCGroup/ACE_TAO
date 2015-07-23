// -*- C++ -*-

//=============================================================================
/**
 * @file LocalObject.h
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

#include "ace/Thread_Mutex.h"
#include "ace/Atomic_Op.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class LocalObject;
  typedef LocalObject *LocalObject_ptr;
  typedef TAO_Pseudo_Var_T<LocalObject> LocalObject_var;
  typedef TAO_Pseudo_Out_T<LocalObject> LocalObject_out;

  /**
   * @class LocalObject
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
    static LocalObject_ptr _narrow (CORBA::Object_ptr obj);

#if (TAO_HAS_MINIMUM_CORBA == 0)

    /// Always returns false.
    virtual CORBA::Boolean _non_existent (void);

    /// Get the repository id.
    virtual char * _repository_id (void);

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
    /// Gets info about object from the Interface Repository.
    virtual CORBA::InterfaceDef_ptr _get_interface (void);

    /// Throws NO_IMPLEMENT.
    virtual CORBA::Object_ptr _get_component (void);

    virtual void _create_request (CORBA::Context_ptr ctx,
                                  const char * operation,
                                  CORBA::NVList_ptr arg_list,
                                  CORBA::NamedValue_ptr result,
                                  CORBA::Request_ptr & request,
                                  CORBA::Flags req_flags);

    virtual void _create_request (CORBA::Context_ptr ctx,
                                  const char * operation,
                                  CORBA::NVList_ptr arg_list,
                                  CORBA::NamedValue_ptr result,
                                  CORBA::ExceptionList_ptr exclist,
                                  CORBA::ContextList_ptr ctxtlist,
                                  CORBA::Request_ptr & request,
                                  CORBA::Flags req_flags);

    /// Throws NO_IMPLEMENT.
    virtual CORBA::Request_ptr _request (const char * operation);

#endif
#endif /* TAO_HAS_MINIMUM_CORBA */

#if (TAO_HAS_CORBA_MESSAGING == 1)

    /// Throws CORBA::NO_IMPLEMENT.
    CORBA::Policy_ptr _get_policy (CORBA::PolicyType type);

    /// Throws CORBA::NO_IMPLEMENT.
    CORBA::Policy_ptr _get_cached_policy (TAO_Cached_Policy_Type type);

    /// Throws CORBA::NO_IMPLEMENT.
    CORBA::Object_ptr _set_policy_overrides (
        const CORBA::PolicyList & policies,
        CORBA::SetOverrideType set_add);

    /// Throws CORBA::NO_IMPLEMENT.
    CORBA::PolicyList * _get_policy_overrides (
        const CORBA::PolicyTypeSeq & types);

    /// Throws CORBA::NO_IMPLEMENT.
    CORBA::Boolean _validate_connection (
        CORBA::PolicyList_out inconsistent_policies);

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

    /**
     * Return a hash value for this object.  The hash is based on the
     * address of the object.  On non-32 bit platforms, the hash may
     * be non-unique.
     */
    virtual CORBA::ULong _hash (CORBA::ULong maximum);

    /**
     * Try to determine if this object is the same as @c other_obj.
     * This method relies on the representation of the object
     * reference's private state.  Since that changes easily (when
     * different ORB protocols are in use) there is no default
     * implementation.
     */
    virtual CORBA::Boolean _is_equivalent (CORBA::Object_ptr other_obj);

    virtual CORBA::ORB_ptr _get_orb (void);

    // = TAO extensions

    /// Throws CORBA::NO_IMPLEMENT.
    virtual TAO::ObjectKey * _key (void);

    /// Useful for template programming.
    typedef LocalObject_ptr _ptr_type;
    typedef LocalObject_var _var_type;
    typedef LocalObject_out _out_type;

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
    ACE_UNIMPLEMENTED_FUNC (LocalObject (const LocalObject &))
    ACE_UNIMPLEMENTED_FUNC (LocalObject & operator = (const LocalObject &))
    //@}

  };
}   // End CORBA namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/LocalObject.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CORBA_LOCALOBJECT_H */
