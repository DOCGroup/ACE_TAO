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

#ifndef TAO_CORBA_OBJECT_H
#define TAO_CORBA_OBJECT_H
#include "ace/pre.h"

#include "tao/corbafwd.h"
#include "tao/Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Export CORBA::LocalObject : public virtual CORBA::Object
{
  // @@ NW: It is not clear whether minimum CORBA should support
  //    LocalObject or not.  I think it should.
public:
  virtual ~LocalObject (void);
  // destructor

  static CORBA::LocalObject_ptr _duplicate (CORBA::LocalObject_ptr obj);
  // increment the ref count

  static CORBA::LocalObject_ptr _nil (void);
  // return a NUL object

  static CORBA::LocalObject_ptr _narrow (CORBA::Object_ptr obj,
                                         CORBA_Environment &ACE_TRY_ENV =
                                           TAO_default_environment ());
  static CORBA::LocalObject_ptr _unchecked_narrow (CORBA::Object_ptr obj,
                                                   CORBA_Environment &ACE_TRY_ENV =
                                                     TAO_default_environment ());
  // no-op it is just here to simplify some templates.

  static void _tao_any_destructor (void*);
  // @@ Does this stuff make sense at all in LocalObject?
  // Used in the implementation of CORBA::Any

  // These calls correspond to over-the-wire operations, or at least
  // do so in many common cases.  The normal implementation assumes a
  // particular simple, efficient, protocol-neutral interface for
  // making such calls, but may be overridden when it appears
  // appropriate.

  virtual CORBA::Boolean _is_a (const CORBA::Char *logical_type_id,
                                CORBA_Environment &ACE_TRY_ENV =
                                  TAO_default_environment ());
  // throws NO_IMPLEMENT.

  virtual const char* _interface_repository_id (void) const;
  // The repository ID for the most derived class, this is an
  // implementation method and does no remote invocations!

  virtual TAO_ServantBase *_servant (void) const;
  // return 0.

  virtual CORBA::Boolean _is_collocated (void) const;
  // return 0 (even if local object is always collocated, there's no
  // servant associate with the object (the object itself implement
  // all operations.

#if (TAO_HAS_MINIMUM_CORBA == 0)

  virtual CORBA::Boolean _non_existent (CORBA_Environment &ACE_TRY_ENV =
                                          TAO_default_environment ());
  // Always returns false.

  virtual CORBA::ImplementationDef_ptr
      _get_implementation (CORBA_Environment &ACE_TRY_ENV =
                             TAO_default_environment ());
  // throws NO_IMPLEMENT.


  virtual CORBA::InterfaceDef_ptr _get_interface (CORBA_Environment &ACE_TRY_ENV =
                                                    TAO_default_environment ());
  // throws NO_IMPLEMENT.

#endif /* TAO_HAS_MINIMUM_CORBA */

#if (TAO_HAS_CORBA_MESSAGING == 1)

  CORBA::Policy_ptr _get_policy (
      CORBA::PolicyType type,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  // throws NO_IMPLEMENT.

  CORBA::Policy_ptr _get_client_policy (
      CORBA::PolicyType type,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  // throws NO_IMPLEMENT.

  CORBA::Object_ptr _set_policy_overrides (
      const CORBA::PolicyList & policies,
      CORBA::SetOverrideType set_add,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  // throws NO_IMPLEMENT.

  CORBA::PolicyList * _get_policy_overrides (
      const CORBA::PolicyTypeSeq & types,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  // throws NO_IMPLEMENT.

  CORBA::Boolean _validate_connection (
      CORBA::PolicyList_out inconsistent_policies,
      CORBA::Environment &ACE_TRY_ENV =
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

  virtual CORBA::Boolean _is_equivalent (CORBA::Object_ptr other_obj,
                                         CORBA_Environment &ACE_TRY_ENV =
                                             TAO_default_environment ())
    ACE_THROW_SPEC (());
  // Try to determine if this object is the same as <other_obj>.  This
  // method relies on the representation of the object reference's
  // private state.  Since that changes easily (when different ORB
  // protocols are in use) there is no default implementation.

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_Object_ptr _ptr_type;
  typedef CORBA_Object_var _var_type;
#endif /* __GNUC__ */
  // Useful for template programming.

  // = TAO extensions

  // = Reference count managment.
  CORBA::ULong _incr_refcnt (void);
  // Increment the reference count.

  CORBA::ULong _decr_refcnt (void);
  // Decrement the reference count.

protected:
private:
#if (TAO_HAS_MINIMUM_CORBA==0)
  virtual void _create_request (CORBA::Context_ptr ctx,
                                const CORBA::Char *operation,
                                CORBA::NVList_ptr arg_list,
                                CORBA::NamedValue_ptr result,
                                CORBA::Request_ptr &request,
                                CORBA::Flags req_flags,
                                CORBA_Environment &ACE_TRY_ENV =
                                  TAO_default_environment ());

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

  // The default implementation of this method uses the same simple,
  // multi-protocol remote invocation interface as is assumed by the
  // calls above ... that's how it can have a default implementation.

  virtual CORBA::Request_ptr _request (const CORBA::Char *operation,
                                       CORBA_Environment &ACE_TRY_ENV =
                                        TAO_default_environment ());
  // DII operation to create a request.

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */

  virtual TAO_ObjectKey *_key (CORBA_Environment &ACE_TRY_ENV =
                                 TAO_default_environment ());
  // Return the object key as an out parameter.  Caller should release
  // return value when finished with it.

  virtual const TAO_ObjectKey &_object_key (void);
  // Return a reference to the object key of profile in-use.
  // If there's no in-use profile, then the program will
  // probably crash.  This method does not create a new copy.

  virtual TAO_Stub *_stubobj (void) const;
  // get the underlying stub object

  virtual void _use_locate_requests (CORBA::Boolean use_it);
  // the the object to use a locate request for the first call to
  // the object

  // = Unimplemented methods
  CORBA_Object (const CORBA_Object &);
  CORBA_Object &operator = (const CORBA_Object &);
};

class TAO_Export CORBA::LocalObject_var
{
public:
  LocalObject_var (void); // default constructor
  LocalObject_var (CORBA::Object_ptr);
  LocalObject_var (const CORBA_Object_var &); // copy constructor
  ~LocalObject_var (void); // destructor

  LocalObject_var &operator= (CORBA::Object_ptr);
  LocalObject_var &operator= (const CORBA_Object_var &);
  CORBA::LocalObject_ptr operator-> (void) const;

  operator const CORBA::LocalObject_ptr &() const;
  operator CORBA::LocalObject_ptr &();
  // in, inout, out, _retn
  CORBA::LocalObject_ptr in (void) const;
  CORBA::LocalObject_ptr &inout (void);
  CORBA::LocalObject_ptr &out (void);
  CORBA::LocalObject_ptr _retn (void);
  CORBA::LocalObject_ptr ptr (void) const;

private:
  CORBA::LocalObject_ptr ptr_;
};

class TAO_Export CORBA::LocalObject_out
{
public:
  LocalObject_out (CORBA::Object_ptr &);
  LocalObject_out (CORBA_Object_var &);
  LocalObject_out (const CORBA_Object_out &);
  LocalObject_out &operator= (const CORBA_Object_out &);
  LocalObject_out &operator= (const CORBA_Object_var &);
  LocalObject_out &operator= (CORBA::Object_ptr);
  operator CORBA::LocalObject_ptr &();
  CORBA::LocalObject_ptr &ptr (void);
  CORBA::LocalObject_ptr operator-> (void);

private:
  CORBA::LocalObject_ptr &ptr_;
};

#if defined (__ACE_INLINE__)
# include "tao/Object.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CORBA_OBJECT_H */
