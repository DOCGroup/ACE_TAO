// @(#)invoke.cpp	1.3 95/09/24
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// IIOP		stub support for static and dynamic invocation
//
// This file holds DII support, and an analagous interpreter that let
// static stubs be very small.  It's specific to objrefs with
// IIOP::ProfileBody.
//
// NOTE: this may someday be moved within an IIOP class so that the
// public stub interface is completely independent of ORB/protocol
// internals.
//
// THREADING NOTE: Code below this point is of course thread-safe (at
// least on supported threaded platforms), so the caller of these
// routines need only ensure that the data being passed in is not
// being modified by any other thread.
//
// As an _experiment_ (to estimate the performance cost) remote calls
// are currently deemed "cancel-safe".  That means that they can be
// called by threads when they're in asynchronous cancellation mode.
// The only effective way to do this is to disable async cancellation
// for the duration of the call.  There are numerous rude interactions
// with code generators for C++ ... cancellation handlers just do
// normal stack unwinding like exceptions, but exceptions are purely
// synchronous and sophisticated code generators rely on that to
// generate better code, which in some cases may be very hard to
// unwind.

#include "tao/orb.h"
#include "tao/cdr.h"
#include "tao/giop.h"
#include "tao/nvlist.h"
#include "tao/debug.h"

class ACE_Synchronous_Cancellation_Required
  // = TITLE
  // 
  //     ACE_Synchronous_Cancellation_Required
  //     
  // = DESCRIPTION
  // 
  //     Stick one of these at the beginning of a block that can't
  //     support asynchronous cancellation, and which must be
  //     cancel-safe.
  // 
  // = EXAMPLE
  //     somefunc()
  //     {
  //       ACE_Synchronous_Cancellation_Required NOT_USED;
  //       ...
  //     }
{
public:
  // These should probably be in a separate inline file, but they're
  // only used within this one file right now, and we always want them
  // inlined, so here they sit.
  ACE_Synchronous_Cancellation_Required (void)
    {
      ACE_OS::thr_setcanceltype (THR_CANCEL_DEFERRED, &old_type_);
    }

  ~ACE_Synchronous_Cancellation_Required (void)
    {
      int dont_care;
      ACE_OS::thr_setcanceltype(old_type_, &dont_care);
    }
private:
  int old_type_;
};

// "stub interpreter" for static stubs.  IDL compiler (or human
// equivalent thereof :-) should just dump a read-only description of
// the call into "calldata" and do varargs calls to this routine,
// which does all the work.
//
// NOTE: This routine includes stub interpreter code, upon which a
// patent application is pending.

void 
IIOP_Object::do_call (CORBA_Environment &env,	// exception reporting
                      const TAO_Call_Data *info,	// call description
                      ...)                       // ... any parameters

{
  ACE_Synchronous_Cancellation_Required NOT_USED;

  GIOP::Invocation call (this,
			 info->opname,
			 info->is_roundtrip);

  // We may need to loop through here more than once if we're
  // forwarded to some other object reference.
  //
  // NOTE: A quality-of-service policy may be useful to establish
  // here, specifically one controlling how many times the call is
  // reissued before failing the call on the assumption that something
  // is broken.
  //
  // NOTE: something missing is a dynamic way to change the policy of
  // whether to issue LocateRequest messages or not.  This code uses a
  // simple, fixed policy: never use LocateRequest messages.
  //
  for (;;)
    {
      // Start the call by constructing the request message header.

      env.clear ();
      call.start (env);

      if (env.exception ())
        {
          dexc (env, "do_call, start request message");
          return;
        }

      // Now, put all "in" and "inout" parameters into the request
      // message body.
      //
      // Some "inout" data have an extra level of indirection,
      // specified by the language mapping's memory allocation
      // policies ... the indirection only shows up here when it's
      // needed later for allocating "out" memory, otherwise there's
      // just one indirection.

      u_int i;
      const TAO_Param_Data *pdp;
      va_list param_vector;

      va_start (param_vector, info);

      for (i = 0, pdp = info->params;
           i < info->param_count;
           i++, pdp++)
        {
          void *ptr = va_arg (param_vector, void *);

          if (pdp->mode == PARAM_IN)
            call.put_param (pdp->tc, ptr, env);
          else if (pdp->mode == PARAM_INOUT)
            {
              if (pdp->value_size == 0)
                call.put_param (pdp->tc, ptr, env);
              else
                call.put_param (pdp->tc, *(void **)ptr, env);
            }

          if (env.exception ())
            {
              dexc (env, "do_call, put request parameter");
              return;
            }
        }
      va_end (param_vector);

      // Make the call ... blocking for response if needed.  Note that
      // "oneway" calls can't return any exceptions except system
      // ones.

      GIOP::ReplyStatusType status;
      CORBA_ExceptionList exceptions;

      exceptions.length = exceptions.maximum = info->except_count;
      exceptions.buffer = (CORBA_TypeCode_ptr *) info->excepts;

      status = call.invoke (exceptions, env);

      exceptions.buffer = 0;		// don't free it

      if (env.exception ())
        {
          dexc (env, "do_call, invoke");
          return;
        }
      if (!info->is_roundtrip
          || status == GIOP::SYSTEM_EXCEPTION
          || status == GIOP::USER_EXCEPTION)
        return;

      // Now, get all the "return", "out", and "inout" parameters from
      // the response message body.

      if (status == GIOP::NO_EXCEPTION)
        {
          va_start (param_vector, info);
          for (i = 0, pdp = info->params;
               i < info->param_count;
               i++, pdp++)
            {
              void *ptr = va_arg (param_vector, void *);

              if (pdp->mode == PARAM_RETURN
                  || pdp->mode == PARAM_OUT
                  || pdp->mode == PARAM_INOUT)
                {
                  // The language mapping's memory allocation policy
                  // says that some data is heap-allocated.  This
                  // interpreter is told about the relevant policy by
                  // whoever built the operation description (e.g. the
                  // IDL compiler) so it doesn't have to know the
                  // policy associated with a particular language
                  // binding (e.g. C/C++ differ, and C++ even has
                  // different policies for different kinds of
                  // structures).
                  if (pdp->value_size == 0)
		    call.get_value (pdp->tc, ptr, env);
                  else
                    {
                      // assert (value_size == tc->size());
                      *(void **)ptr = new CORBA_Octet [pdp->value_size];
                      call.get_value (pdp->tc, *(void **)ptr, env);
                    }

                  if (env.exception ())
                    {
                      dexc (env, "do_call, get reply parameter");
                      return;
                    }
                }
            }
          va_end (param_vector);
          return;
        }

      // ... or maybe this request got forwarded to someplace else; send
      // the request there instead.
      assert (status == GIOP::LOCATION_FORWARD);
    }
}

// DII analogue of the above.  Differs in how the vararg calling
// convention is implemented -- DII doesn't use the normal call stack
// with its implicit typing, but iinstead uses heap-based arguments
// with explicit typing.

void
IIOP_Object::do_dynamic_call (const char *opname,       	// operation name
                              CORBA_Boolean is_roundtrip,	// results required?
                              CORBA_NVList_ptr args,		// parameters
                              CORBA_NamedValue_ptr result,	// result
                              CORBA_Flags flags,		// per-call flag (one!)
                              CORBA_ExceptionList &exceptions,	// possible user exceptions
                              CORBA_Environment &env)		// exception reporting
{
  ACE_Synchronous_Cancellation_Required NOT_USED;

  GIOP::Invocation call (this, opname, is_roundtrip);

  // Loop as needed for forwarding; see above.

  for (;;)
    {
      // Start the call by constructing the request message header.
      env.clear ();
      call.start (env);

      if (env.exception ())
        {
          dexc (env, "do_call, start request message");
          return;
        }

      // Now, put all "in" and "inout" parameters into the request
      // message body.

      u_int i;

      for (i = 0; i < args->count (); i++)
        {
          CORBA_NamedValue_ptr value = args->item (i);

          if (value->flags () == CORBA_ARG_IN
              || value->flags () == CORBA_ARG_INOUT)
            {
              call.put_param (value->value ()->type (),
                              value->value ()->value (), env);
              if (env.exception ())
                {
                  dexc (env, "do_dynamic_call, put request parameter");
                  return;
                }
            }
        }

      // Make the call ... blocking for response if needed.  Note that
      // "oneway" calls can't return any exceptions except system ones.

      GIOP::ReplyStatusType status;

      status = call.invoke (exceptions, env);
      if (env.exception ())
        {
          dexc (env, "do_dynamic_call, invoke");
          return;
        }
      if (!is_roundtrip
          || status == GIOP::SYSTEM_EXCEPTION
          || status == GIOP::USER_EXCEPTION)
        return;

      // Now, get all the "return", "out", and "inout" parameters from the
      // response message body ... return parameter is first, the rest are
      // in the order defined in the IDL spec (which is also the order that
      // DII users are required to use).

      if (status == GIOP::NO_EXCEPTION)
        {
          if (result != 0)
            {
              // If caller didn't set OUT_LIST_MEMORY flag, allocate
              // memory for return value ...

              if (!(flags & CORBA_OUT_LIST_MEMORY))
                {
                  CORBA_TypeCode_ptr tcp;
                  size_t size;

                  tcp = result->value ()->type ();
                  size = tcp->size (env);
                  dexc (env, "do_dynamic_call, get result size");

                  if (size != 0)
                    {
                      void *ptr = new CORBA_Octet [size];

                      tcp->AddRef ();
                      result->value ()->replace (tcp, ptr,
                                                 CORBA_B_TRUE, env);
                      dexc (env, "do_dynamic_call, set result mem");
                    }
                }

              call.get_value (result->value ()->type (),
                              result->value ()->value (), env);
            }

          for (i = 0; i < args->count (); i++)
            {
              CORBA_NamedValue_ptr	value = args->item (i);

              if (value->flags () == CORBA_ARG_OUT
                  || value->flags () == CORBA_ARG_INOUT)
                {
                  // If caller didn't set OUT_LIST_MEMORY flag, allocate
                  // memory for this parameter ...
                  if (!(flags & CORBA_OUT_LIST_MEMORY))
                    {
                      CORBA_TypeCode_ptr tcp;
                      size_t size;

                      tcp = value->value ()->type ();
                      size = tcp->size (env);
                      dexc (env, "do_dynamic_call, get param size");

                      if (size != 0)
                        {
                          void *ptr = new CORBA_Octet [size];

                          tcp->AddRef ();
                          value->value ()->replace (tcp, ptr,
                                                    CORBA_B_TRUE, env);
                          dexc (env, "do_dynamic_call, set result mem");
                        }
                    }

                  call.get_value (value->value ()->type (),
                                  value->value ()->value (), env);
                  if (env.exception ())
                    {
                      dexc (env, "do_dynamic_call, get response parameter");
                      return;
                    }
                }
            }
          return;
        }

      // ... or maybe this request got forwarded to someplace else.
      assert (status == GIOP::LOCATION_FORWARD);
    }
}
