// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    svrrqst.h
//
// = DESCRIPTION
//     
//    Header file for Win32 C/C++/COM interface to CORBA's Dynamic
//    Server Skeleton Interface's "Server Request" type.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
//
// = VERSION
//     $Id$
// 
// ============================================================================

#if !defined (TAO_SVRRQST_H)
#define TAO_SVRRQST_H

class TAO_POA;

class TAO_Export CORBA_ServerRequest : public TAO_IUnknown
  // = TITLE
  //   XXX this is currently in an intermediate state; this is not
  //   supposed to be IIOP-specific, or to expose quite so many
  //   implementation details, but right now it is.
{
public:
  virtual void params (CORBA::NVList_ptr list,
				 CORBA::Environment &env) = 0;
  // Implementation uses this to provide the ORB with the operation's
  // parameter list ... on return, their values are available; the
  // list fed in has typecodes and (perhap) memory assigned.

  virtual void result (CORBA::Any_ptr value,
				 CORBA::Environment &env) = 0;
  // Implementation uses this to provide the operation result
  // ... illegal if exception() was called or params() was not called.
  //
  // XXX Implementation should be able to assume response has been
  // sent when this returns, and reclaim memory it allocated.

  virtual void exception (CORBA::ExceptionType	type,
				    CORBA::Any_ptr value,
				    CORBA::Environment &env) = 0;
  // Implementation uses this to provide the exception value which is
  // the only result of this particular invocation.
  //
  // XXX Implementation should be able to assume response has been
  // sent when this returns, and reclaim memory it allocated.

  // = Get various universal call attributes.

  // e.g., who made the call, the target of the call, what ORB and OA
  // that target object uses.
  //
  // NOTE: none of these report exceptions; unavailability of any of
  // this stuff is a catastrophic error since this is all part of the
  // basic CORBA Object Model.
  //
  // XXX should not be not assuming all OAs implement the POA API !!
  virtual CORBA::Principal_ptr 	caller (void) = 0;
  virtual CORBA::Object_ptr  target (void) = 0;
  virtual CORBA::String  op_name (void) = 0;
  virtual TAO_POA *oa (void) = 0;
  virtual CORBA::ORB_ptr  orb (void) = 0;
};

class TAO_Export IIOP_ServerRequest : public CORBA_ServerRequest
  // = TITLE
  //    Class representing an IIOP ServerRequest object.
{
public:
  // Constructor, destructor

  IIOP_ServerRequest (CDR *msg,
		      CORBA::ORB_ptr the_orb,
		      TAO_POA *the_poa);

  virtual ~IIOP_ServerRequest (void);

  // = General ServerRequest operations
  void  params (CORBA::NVList_ptr list,
			 CORBA::Environment &env);

  void  result (CORBA::Any_ptr value,
			 CORBA::Environment &env);

  void  exception (CORBA::ExceptionType type,
			    CORBA::Any_ptr value,
			    CORBA::Environment &env);

  // = Request attributes

  CORBA::String  op_name (void);
  CORBA::Principal_ptr 	caller (void);
  CORBA::Object_ptr  target (void);
  CORBA::ORB_ptr  orb (void);
  TAO_POA *oa (void);

  // Stuff required for COM IUnknown support

  ULONG  AddRef (void);
  ULONG  Release (void);
  TAO_HRESULT  QueryInterface (REFIID riid,
                               void **ppv);

  // private:
  CORBA::String opname_;        // Operation name.
  CDR *incoming_;               // Incoming stream.
  CORBA::NVList_ptr params_;    // Incoming parameters.
  CORBA::Any_ptr retval_;       // Return value.
  CORBA::Any_ptr exception_;    // Any exception which might be raised.
  CORBA::ExceptionType ex_type_; // The type of <exception_>.

  void release (void) { refcount_--; }
  // Just drop the refcount, don't destroy the object; most of these
  // are stack-allocated.

private:
  ACE_SYNCH_MUTEX lock_;
  // Lock protecting each instance.

  u_int refcount_;
  // Number of things hold references to here.

  CORBA::ORB_ptr orb_;
  // The ORB with which this server request is associated.

  TAO_POA *poa_;
  // The object adapter with whicih this server request is associated.
};

#endif /* TAO_SVRRQST_H */
