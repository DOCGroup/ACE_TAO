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

struct TAO_Param_Data_Skel
  // = TITLE
  //   Description of a single parameter.
  //
  // = DESCRIPTION
  //
  //   If value_size is nonzero for OUT, INOUT, or RETURN parameters,
  //   it's (a) an indicator that the ORB returns a pointer-to-value
  //   for this parameter, and also (b) is the size of the top-level
  //   of the value that's returned (e.g. ignoring nested sequence
  //   buffers).  That is, it moves CPU cycles from runtime -- some
  //   calls to tc->size() -- to compile time where they're
  //   cheap/free.
  //
  //   It _must_ only be set for cases where the ORB allocates the
  //   return value, which must then be ORB::free()d ... e.g. where
  //   the value is a pointer to data such as a struct, sequence, or
  //   union.  (The CORBA C++ mapping doesn't require that for all
  //   "out" structs; only those of "variable size".)  If this value
  //   is nonzero, the value passed to do_call() must be the address
  //   of a pointer.

{
  CORBA::TypeCode_ptr tc;
  // Type of param.

  CORBA::ULong mode;
  // Its mode.

  CORBA::Boolean own;
  // whether we own it or not
};

struct TAO_Call_Data_Skel
  // = TITLE
  //   Descriptions of operations, as used by the stub interpreter.
  //   Only interpretive marshaling/unmarshaling is used, and the
  //   stubs don't know what particular on-the-wire protocol is being
  //   used.
  //
  // = DESCRIPTION
  //   When using C++ exceptions, many C++ compilers will require the
  //   use of compiled code throw the exception.  As binary standards
  //   for exception throwing evolve, it may become practical to
  //   interpretively throw exceptions.
{
  const char *opname;
  // Operation name.

  CORBA::Boolean is_roundtrip;
  // !oneway

  // When constructing tables of parameters, put them in the same
  // order they appear in the IDL spec: return value, then parameters
  // left to right.  Other orders may produce illegal IIOP protocol
  // messages.

  CORBA::ULong param_count;
  // # parameters.

  const TAO_Param_Data_Skel *params;
  // Their descriptions.

};

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

  // Extensions
  virtual void demarshal (CORBA::Environment &env,
                          const TAO_Call_Data_Skel *info,
                          ...) = 0;
  virtual void marshal (CORBA::Environment &env,
                        const TAO_Call_Data_Skel *info,
                        ...) = 0;
  virtual void init_reply (CORBA::Environment &env) = 0;
  // start a Reply message

  virtual CDR &incoming (void) = 0;
  // retrieve the incoming stream

  virtual CDR &outgoing (void) = 0;
  // retrieve the outgoing stream
};

class TAO_Export IIOP_ServerRequest : public CORBA_ServerRequest
  // = TITLE
  //    Class representing an IIOP ServerRequest object.
{
public:
  // Constructor, destructor

  IIOP_ServerRequest (CDR *req,
                      CDR *resp,
                      CORBA::ULong reqid,
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

  virtual void demarshal (CORBA::Environment &env,
                          const TAO_Call_Data_Skel *info,
                          ...);
  virtual void marshal (CORBA::Environment &env,
                        const TAO_Call_Data_Skel *info,
                        ...);
  virtual void init_reply (CORBA::Environment &env);
  // start a Reply message

  virtual CDR &incoming (void);
  // retrieve the incoming stream

  virtual CDR &outgoing (void);
  // retrieve the outgoing stream

  // private:
  CORBA::String opname_;        // Operation name.
  CDR *incoming_;               // Incoming stream.
  CDR *outgoing_;               // Outgoing stream.
  CORBA::ULong reqid_;          // request ID
  CORBA::NVList_ptr params_;    // Incoming parameters.
  CORBA::Any_ptr retval_;       // Return value.
  CORBA::Any_ptr exception_;    // Any exception which might be raised.
  CORBA::ExceptionType ex_type_; // The type of <exception_>.

  void release (void) { refcount_--; }
  // Just drop the refcount, don't destroy the object; most of these
  // are stack-allocated.

private:
  u_int refcount_;
  // Number of things hold references to here.

  CORBA::ORB_ptr orb_;
  // The ORB with which this server request is associated.

  TAO_POA *poa_;
  // The object adapter with whicih this server request is associated.
};

#endif /* TAO_SVRRQST_H */
