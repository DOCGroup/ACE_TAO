// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    object.h
//
// = DESCRIPTION
//     Header file for Win32 interface to CORBA's base "Object" type.
//
//     A "Object" is an entity that can be the target of an invocation
//     using an ORB.  All CORBA objects provide this functionality.
//     See the CORBA 2.0 specification for details.
//
// = AUTHOR
//     Portions Copyright 1994-1995 by Sun Microsystems Inc.
//     Portions Copyright 1997 by Washington University
//
// ============================================================================

#ifndef TAO_CORBA_OBJECT_H
#define TAO_CORBA_OBJECT_H

#include "tao/corbafwd.h"

class TAO_ServantBase;
class TAO_Stub;

class TAO_Export CORBA_Object
{
public:
  static CORBA_Object_ptr _duplicate (CORBA_Object_ptr obj);
  // increment the ref count

  static CORBA_Object_ptr _nil (void);
  // return a NUL object

  static CORBA_Object_ptr _narrow (CORBA_Object_ptr obj,
                                   CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // no-op it is just here to simplify some templates.

  // These calls correspond to over-the-wire operations, or at least
  // do so in many common cases.  The normal implementation assumes a
  // particular simple, efficient, protocol-neutral interface for
  // making such calls, but may be overridden when it appears
  // appropriate.

  virtual CORBA::Boolean _is_a (const CORBA::Char *logical_type_id,
                                CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // determine if we are of the type specified by the "logical_type_id"

  virtual const char* _interface_repository_id (void) const;
  // The repository ID for the most derived class, this is an
  // implementation method and does no remote invocations!

  virtual TAO_ServantBase *_servant (void) const;
  // return the associated servant (if one exists)

  virtual CORBA::Boolean _is_collocated (void) const;
  // are we collocated with the servant?

  virtual CORBA::Boolean _non_existent (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

#if !defined (TAO_HAS_MINIMUM_CORBA)

  virtual CORBA::ImplementationDef_ptr
      _get_implementation (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // This method is deprecated in the CORBA 2.2 spec, we just return 0
  // every time.

  virtual CORBA::InterfaceDef_ptr _get_interface (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Interface repository related operation

  virtual void _create_request (CORBA::Context_ptr ctx,
                                const CORBA::Char *operation,
                                CORBA::NVList_ptr arg_list,
                                CORBA::NamedValue_ptr result,
                                CORBA::Request_ptr &request,
                                CORBA::Flags req_flags,
                                CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // DII operation to create a request.
  //
  // The mapping for create_request is split into two forms,
  // corresponding to the two usage styles described in CORBA section
  // 6.2.1.
  //
  // The default implementation of this method uses the same simple,
  // multi-protocol remote invocation interface as is assumed by the
  // calls above ... that's how it can have a default implementation.

  virtual CORBA::Request_ptr _request (const CORBA::Char *operation,
                                       CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // DII operation to create a request.

#endif /* TAO_HAS_MINIMUM_CORBA */

  virtual CORBA::ULong _hash (CORBA::ULong maximum,
                              CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Return a (potentially non-unique) hash value for this object.
  // This method relies on the representation of the object
  // reference's private state.  Since that changes easily (when
  // different ORB protocols are in use) there is no default
  // implementation.

  virtual CORBA::Boolean _is_equivalent (CORBA::Object_ptr other_obj,
                                         CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Try to determine if this object is the same as <other_obj>.  This
  // method relies on the representation of the object reference's
  // private state.  Since that changes easily (when different ORB
  // protocols are in use) there is no default implementation.

  // = Reference count managment.
  CORBA::ULong _incr_refcnt (void);
  // Increment the reference count.

  CORBA::ULong _decr_refcnt (void);
  // Decrement the reference count.

  CORBA_Object (TAO_Stub *p = 0,
                TAO_ServantBase *servant = 0,
                CORBA::Boolean collocated = 0);
  // constructor

  virtual ~CORBA_Object (void);
  // destructor

  virtual TAO_ObjectKey *_key (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Return the object key as an out parameter.  Caller should release
  // return value when finished with it.

  virtual TAO_Stub *_stubobj (void) const;
  // get the underlying stub object

  virtual void _use_locate_requests (CORBA::Boolean use_it);
  // the the object to use a locate request for the first call to
  // the object

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_Object_ptr _ptr_type;
  typedef CORBA_Object_var _var_type;
#endif /* __GNUC__ */
  // Useful for template programming.

protected:
  TAO_ServantBase *servant_;
  // Servant pointer.  It is 0 except for collocated objects.

  CORBA::Boolean is_collocated_;
  // Flag to indicate collocation.  It is 0 except for collocated
  // objects.

private:
  TAO_Stub *protocol_proxy_;
  // Pointer to the protocol-specific "object" containing important
  // profiling information regarding this proxy.
  // The protocol proxy is (potentially) shared among several
  // CORBA_Objects

  CORBA::ULong refcount_;
  // Number of outstanding references to this object.

  ACE_SYNCH_MUTEX refcount_lock_;
  // Protect the reference count, this is OK because we do no
  // duplicates or releases on the critical path.

  // = Unimplemented methods
  CORBA_Object (const CORBA_Object &);
  CORBA_Object &operator = (const CORBA_Object &);
};

class TAO_Export CORBA_Object_var
{
public:
  CORBA_Object_var (void); // default constructor
  CORBA_Object_var (CORBA::Object_ptr);
  CORBA_Object_var (const CORBA_Object_var &); // copy constructor
  ~CORBA_Object_var (void); // destructor

  CORBA_Object_var &operator= (CORBA::Object_ptr);
  CORBA_Object_var &operator= (const CORBA_Object_var &);
  CORBA::Object_ptr operator-> (void) const;

  operator const CORBA::Object_ptr &() const;
  operator CORBA::Object_ptr &();
  // in, inout, out, _retn
  CORBA::Object_ptr in (void) const;
  CORBA::Object_ptr &inout (void);
  CORBA::Object_ptr &out (void);
  CORBA::Object_ptr _retn (void);
  CORBA::Object_ptr ptr (void) const;

private:
  CORBA::Object_ptr ptr_;
};

class TAO_Export CORBA_Object_out
{
public:
  CORBA_Object_out (CORBA::Object_ptr &);
  CORBA_Object_out (CORBA_Object_var &);
  CORBA_Object_out (const CORBA_Object_out &);
  CORBA_Object_out &operator= (const CORBA_Object_out &);
  CORBA_Object_out &operator= (const CORBA_Object_var &);
  CORBA_Object_out &operator= (CORBA::Object_ptr);
  operator CORBA::Object_ptr &();
  CORBA::Object_ptr &ptr (void);
  CORBA::Object_ptr operator-> (void);

private:
  CORBA::Object_ptr &ptr_;
};

class TAO_Export TAO_Object_Field
{
  // = TITLE
  //   Base class to represent fields (in structures) corresponding to
  //   object references.
  //
  // = DESCRIPTION
  //   When an object reference appears in a structure the marshaling
  //   and demarhsaling of the structure gets complicated:
  //   the interpreter can only marshal CORBA_Object_ptr when it
  //   dermarshal it creates on of those objects.
  //   The downcasting to the right type must be executed by classes
  //   with compile-time knowledge of the object type.
  //   The solution addopted in TAO is to create a special manager
  //   class for that field called TAO_Object_Field_T<T> (see
  //   varout.h), this class serves as an abstract interface to
  //   manipulate instances of those classes (by the interpreter, of
  //   course).
  //
public:
  virtual ~TAO_Object_Field (void);
  // destructor

  virtual void _downcast (CORBA_Object* base_ptr,
                          CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) = 0;
  virtual CORBA_Object* _upcast (void) = 0;
  virtual void _release (void) = 0;
};

extern TAO_Export CORBA::Boolean
operator<< (TAO_OutputCDR&, const CORBA_Object*);

extern TAO_Export CORBA::Boolean
operator>> (TAO_InputCDR&, CORBA_Object*&);

#if defined (__ACE_INLINE__)
# include "tao/Object.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_CORBA_OBJECT_H */
