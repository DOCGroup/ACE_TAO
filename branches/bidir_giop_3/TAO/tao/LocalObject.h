// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    LocalObject.h
//
// = DESCRIPTION
//     Header file for CORBA's base "LocalObject" type.
//
//     A "LocalObject" is an entity that can be the target of an local
//     invocation.  See the CORBA Component Model specification for details.
//
// = AUTHOR
//     Portions Copyright 1997 by Washington University
//
// ============================================================================

#ifndef TAO_CORBA_LOCALOBJECT_H
#define TAO_CORBA_LOCALOBJECT_H
#include "ace/pre.h"

#include "tao/corbafwd.h"
#include "tao/Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Export CORBA_LocalObject : public virtual CORBA_Object
{
  // @@ NW: It is not clear whether minimum CORBA should support
  //    LocalObject or not.  I think it should.
public:
  virtual ~CORBA_LocalObject (void);
  // destructor

  static CORBA_LocalObject_ptr _duplicate (CORBA_LocalObject_ptr obj);
  // increment the ref count

  static CORBA_LocalObject_ptr _nil (void);
  // return a NUL object

  static CORBA_LocalObject_ptr _narrow (CORBA_Object_ptr obj,
                                        CORBA_Environment &ACE_TRY_ENV =
                                          TAO_default_environment ());
  // @@ Narrowing a CORBA_LocalObject to a CORBA_Object is broken
  // right now.  The solution seems to be making CORBA_Object an
  // abstract base class and create a CORBA_RemoteObject for regular
  // object.  Or, even easier, add a <is_local> member into
  // CORBA_Object.  I'll take the easier route for now.

  static CORBA_LocalObject_ptr _unchecked_narrow (CORBA_Object_ptr obj,
                                                  CORBA_Environment &ACE_TRY_ENV =
                                                    TAO_default_environment ());
  // no-op it is just here to simplify some templates.

#if (TAO_HAS_MINIMUM_CORBA == 0)

  virtual CORBA::Boolean _non_existent (CORBA_Environment &ACE_TRY_ENV =
                                          TAO_default_environment ());
  // Always returns false.

  virtual CORBA::ImplementationDef_ptr
      _get_implementation (CORBA_Environment &ACE_TRY_ENV =
                             TAO_default_environment ());
  // throws NO_IMPLEMENT.


#if (TAO_HAS_INTERFACE_REPOSITORY == 1)
  virtual IR_InterfaceDef *_get_interface (CORBA_Environment &ACE_TRY_ENV =
                                             TAO_default_environment ());
  // throws NO_IMPLEMENT.
#endif /* TAO_HAS_INTERFACE_REPOSITORY == 1 */

  virtual void _create_request (CORBA::Context_ptr ctx,
                                const CORBA::Char *operation,
                                CORBA::NVList_ptr arg_list,
                                CORBA::NamedValue_ptr result,
                                CORBA::Request_ptr &request,
                                CORBA::Flags req_flags,
                                CORBA_Environment &ACE_TRY_ENV =
                                  TAO_default_environment ());
  // throws NO_IMPLEMENT.

  virtual void _create_request (CORBA::Context_ptr ctx,
                                const CORBA::Char *operation,
                                CORBA::NVList_ptr arg_list,
                                CORBA::NamedValue_ptr result,
                                CORBA::ExceptionList_ptr exclist,
                                CORBA::ContextList_ptr ctxtlist,
                                CORBA::Request_ptr &request,
                                CORBA::Flags req_flags,
                                CORBA_Environment &ACE_TRY_ENV =
                                  TAO_default_environment ());

  // throws NO_IMPLEMENT.

  virtual CORBA::Request_ptr _request (const CORBA::Char *operation,
                                       CORBA_Environment &ACE_TRY_ENV =
                                        TAO_default_environment ());
  // throws NO_IMPLEMENT.

#endif /* TAO_HAS_MINIMUM_CORBA */

#if (TAO_HAS_CORBA_MESSAGING == 1)

  CORBA::Policy_ptr _get_policy (
      CORBA::PolicyType type,
      CORBA_Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  // throws NO_IMPLEMENT.

  CORBA::Policy_ptr _get_client_policy (
      CORBA::PolicyType type,
      CORBA_Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  // throws NO_IMPLEMENT.

  CORBA::Object_ptr _set_policy_overrides (
      const CORBA::PolicyList & policies,
      CORBA::SetOverrideType set_add,
      CORBA_Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  // throws NO_IMPLEMENT.

  CORBA::PolicyList * _get_policy_overrides (
      const CORBA::PolicyTypeSeq & types,
      CORBA_Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  // throws NO_IMPLEMENT.

  CORBA::Boolean _validate_connection (
      CORBA::PolicyList_out inconsistent_policies,
      CORBA_Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  // throws NO_IMPLEMENT.

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  virtual CORBA::ULong _hash (CORBA::ULong maximum,
                              CORBA_Environment &ACE_TRY_ENV =
                                TAO_default_environment ());
  // Return a (potentially non-unique) hash value for this object.
  // This method relies on the representation of the object
  // reference's private state.  Since that changes easily (when
  // different ORB protocols are in use) there is no default
  // implementation.

  virtual CORBA::Boolean _is_equivalent (CORBA_Object_ptr other_obj,
                                         CORBA_Environment &ACE_TRY_ENV =
                                             TAO_default_environment ())
    ACE_THROW_SPEC (());
  // Try to determine if this object is the same as <other_obj>.  This
  // method relies on the representation of the object reference's
  // private state.  Since that changes easily (when different ORB
  // protocols are in use) there is no default implementation.

  // = Reference count managment.
  virtual void _add_ref (void);
  // Increment the reference count.

  virtual void _remove_ref (void);
  // Decrement the reference count.

  // = TAO extensions

  virtual TAO_ObjectKey *_key (CORBA_Environment &ACE_TRY_ENV =
                                 TAO_default_environment ());
  // throws NO_IMPLEMENT.

#if 0
  virtual const TAO_ObjectKey &_object_key (void);
  // Return a reference to the object key of profile in-use.
  // If there's no in-use profile, then the program will
  // probably crash.  This method does not create a new copy.

  virtual void * _tao_QueryInterface (ptr_arith_t type);
  // Downcasting this object pointer to some other derived class.
  // This QueryInterface stuff only work for local object.
#endif /* 0 */

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_LocalObject_ptr _ptr_type;
  typedef CORBA_LocalObject_var _var_type;
#endif /* __GNUC__ */
  // Useful for template programming.

protected:
  CORBA_LocalObject (void);
  // Default constructor.  Make it protected to prevent instantiation
  // of this class.

private:

  // = Unimplemented methods
  // constructor

  CORBA_LocalObject (const CORBA_LocalObject &);
  CORBA_LocalObject &operator = (const CORBA_LocalObject &);
};

class TAO_Export CORBA_LocalObject_var
{
public:
  CORBA_LocalObject_var (void); // default constructor
  CORBA_LocalObject_var (CORBA_LocalObject_ptr);
  CORBA_LocalObject_var (const CORBA_LocalObject_var &); // copy constructor
  ~CORBA_LocalObject_var (void); // destructor

  CORBA_LocalObject_var &operator= (CORBA_LocalObject_ptr);
  CORBA_LocalObject_var &operator= (const CORBA_LocalObject_var &);
  CORBA_LocalObject_ptr operator-> (void) const;

  operator const CORBA_LocalObject_ptr &() const;
  operator CORBA_LocalObject_ptr &();
  // in, inout, out, _retn
  CORBA_LocalObject_ptr in (void) const;
  CORBA_LocalObject_ptr &inout (void);
  CORBA_LocalObject_ptr &out (void);
  CORBA_LocalObject_ptr _retn (void);
  CORBA_LocalObject_ptr ptr (void) const;

private:
  CORBA_LocalObject_ptr ptr_;
};

class TAO_Export CORBA_LocalObject_out
{
public:
  CORBA_LocalObject_out (CORBA_LocalObject_ptr &);
  CORBA_LocalObject_out (CORBA_LocalObject_var &);
  CORBA_LocalObject_out (const CORBA_LocalObject_out &);
  CORBA_LocalObject_out &operator= (const CORBA_LocalObject_out &);
  CORBA_LocalObject_out &operator= (const CORBA_LocalObject_var &);
  CORBA_LocalObject_out &operator= (CORBA_LocalObject_ptr);
  operator CORBA_LocalObject_ptr &();
  CORBA_LocalObject_ptr &ptr (void);
  CORBA_LocalObject_ptr operator-> (void);

private:
  CORBA_LocalObject_ptr &ptr_;
};

class TAO_Export TAO_Local_RefCounted_Object : public virtual CORBA_LocalObject
{
  //
  // This class adds default reference counting to local objects.
  //
public:

  virtual void _add_ref (void);
  // Increment reference count.

  virtual void _remove_ref (void);
  // Decrement reference count.

protected:
  TAO_Local_RefCounted_Object (void);
  // Default constructor.  Make it protected to prevent instantiation
  // of this class.

private:

  // = Unimplemented methods
  // constructor

  TAO_Local_RefCounted_Object (const TAO_Local_RefCounted_Object &);
  TAO_Local_RefCounted_Object &operator = (const TAO_Local_RefCounted_Object &);
};

#if defined (__ACE_INLINE__)
# include "tao/LocalObject.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CORBA_LOCALOBJECT_H */
