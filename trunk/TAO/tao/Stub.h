// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Stub.h
//
// = DESCRIPTION
//
//    Data structures used by static and dynamic stubs, and to a
//    lesser degree by skeletons
//
//    Header file for Win32 C/C++/COM interface to one kind of CORBA
//    remote invocation framework.  This is for use by
//    compiler-generated code, not by portable applications!
//
//    These constitute the stub API to this "ORB Core".  Such
//    interfaces are not specified by OMG, and may differ between
//    different ORBs.  This one has the particular advantage that
//    stubs can be quite small.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
//
// ============================================================================

#if !defined (TAO_STUB_H)
#  define TAO_STUB_H

// Descriptions of parameters.

enum TAO_Param_Type
  // = TITLE
  // Parameter mode of a given parameter.
{
  PARAM_IN,
  PARAM_OUT,
  PARAM_INOUT,
  PARAM_RETURN  // = PARAM_OUT
};

struct TAO_Param_Data
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
  //   is nonzero, the value passed to do_static_call() must be the address
  //   of a pointer.

{
  CORBA::TypeCode_ptr tc;
  // Type of param.

  TAO_Param_Type mode;
  // Its mode.

  size_t value_size;
  // zero or tc->size ().
};

struct TAO_Call_Data
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

  u_int param_count;
  // # parameters.

  const TAO_Param_Data *params;
  // Their descriptions.

  // The exceptions allowed by this operation can be listed in any
  // order, and need not include the system exceptions which may be
  // returned by OMG-IDL operations.  If an operation tries to return
  // any exception not allowed by its type signature, this causes a a
  // system exception to be reported.

  u_int except_count;
  // # exceptions.

  const CORBA::TypeCode_ptr *excepts;
  // Their descriptions.
};

struct TAO_Skel_Entry
  // = TITLE
  //   Skeletons map "ServerRequest" generic signatures to the static
  //   call signature required by the implementation's methods.  table
  //   of these per implementation
  //
  // = DESCRIPTION
  //   There are several optimizations that'd be desirable for use by
  //   "static skeletons", notably (a) passing of per-object data held
  //   by the OA so that the method doesn't need to look it up itself,
  //   (b) removing all mandatory heap allocation of data, and of
  //   course (c) handling all the built-in ORB calls like "is_a" and
  //   "get_implementation".  This code is currently set up only for
  //   Dynamic Skeletons and bridging, for which none of those are
  //   real issues.
{
  const TAO_Call_Data *op_descriptor;
  TAO_Skeleton impl_skeleton;
};

class TAO_Export STUB_Object : public TAO_IUnknown
  // = TITLE
  //   Per-objref data includes the (protocol-specific) Profile, which
  //   is handled by placing it into a subclass of this type along
  //   with data that may be used in protocol-specific caching
  //   schemes.
  //
  //   The type ID (the data specified by CORBA 2.0 that gets exposed
  //   "on the wire", and in stringified objrefs) is held by this
  //   module.
  //
  //   The stub and DII interpreter APIs are member functions of this
  //   type.
{
public:
  virtual void do_static_call (CORBA::Environment &env,
                        const TAO_Call_Data *info,
                        ...) = 0;
  // The "stub interpreter" method parameters are:
  //
  //    - env ... used for exception reporting
  //    - info ... describes the call
  //    - varargs parameters follow
  //
  // The varargs parameters are pointers to data instances as
  // described by info->params.  (If the value_size is nonzero, the
  // data instance is itself a pointer.)  The order on the call stack
  // must be exactly the order they're listed in info->params;
  // normally this is the order the values are listed in the OMG-IDL
  // operation specification.
  //
  // NOTE: This can be sped up by eliminating some parameter
  // shuffling.  The stub and "do_static_call" parameters are all but the
  // same, except that their order is different.

  virtual void do_dynamic_call (const char *opname,
                                CORBA::Boolean is_roundtrip,
                                CORBA::NVList_ptr args,
                                CORBA::NamedValue_ptr result,
                                CORBA::Flags flags,
                                CORBA::ExceptionList &exceptions,
                                CORBA::Environment &env) = 0;
  // Dynamic invocations use a more costly "varargs" calling
  // convention; it's got the same input data as the (static)
  // stub-oriented one, but the data is represented somewhat
  // differently.
  //
  // Operation-specific data isn't collected in a stack frame and into
  // a static/readonly "calldata" structure, but instead is collected
  // into these parameters, the majority of which are heap-allocated:
  //
  //    - opname ... the name of the operation
  //    - is_roundtrip ... true except for oneway operations, or ones where
  //      the client doesn't care about responses
  //    - args ... call parameters and their descriptions
  //    - result ... result and its description
  //    - flags ... only one DII flag is legal
  //    - exceptions ... list of legal user-defined exceptions
  //    - env ... used for exception reporting.

  CORBA::String_var type_id;
  // All objref representations carry around a type ID.

  virtual CORBA::ULong hash (CORBA::ULong maximum,
                             CORBA::Environment &env) = 0;
  // All objref representations know how to hash themselves and
  // compare themselves for equivalence to others.  It's easily
  // possible to have two objrefs that are distinct copies of data
  // that refers/points to the same remote object (i.e. are
  // equivalent).

  virtual CORBA::Boolean is_equivalent (CORBA::Object_ptr other_obj,
                                        CORBA::Environment &env) = 0;
  // @@ (ANDY) Please comment me.

  STUB_Object (CORBA::String p = 0);
  // XXX All objref representations should know how to marshal
  // themselves.  That will involve ensuring that the IOR that gets
  // marshaled talks a specific protocol, otherwise the target of a
  // message would not be invoke using the objref it receives
  // (compromising functionality in a very basic and mysterious
  // manner).  So for example an objref might need to create a proxy
  // for itself rather than marshaling its own representation.  [ The
  // IIOP engine does not need to worry about such issues since it
  // only supports one protocol -- the problem won't show up.
  // "Multiprotocol ORBs" will need to solve that problem though.  ]

  virtual TAO_ObjectKey *key (CORBA::Environment &env) = 0;
  // Return the object key as an out parameter.  Caller should release
  // return value when finished with it.

protected:
  virtual ~STUB_Object (void);
  // XXX virtual inlines are evil.

  // COM operations ... provided by an implementation class which
  // inherits from this one.

private:
  // = Disallow copy constructor and assignment operator
  ACE_UNIMPLEMENTED_FUNC (STUB_Object (const STUB_Object &))
  ACE_UNIMPLEMENTED_FUNC (STUB_Object &operator = (const STUB_Object &))
};

#endif /* TAO_STUB_H */
