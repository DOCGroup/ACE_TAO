// $Id$

// @ (#)iiopobj.cpp     1.9 95/11/04
// Copyright 1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// XXXX Bridge:         CORBA::Object operations
//
// Some CORBA::Object and other operations are specific to this STUB
// based implementation, and can neither be used by other kinds of
// objref nor have a default implementation.

#include "tao/Stub.h"
#include "tao/Sequence.h"
#include "tao/Sequence_T.h"
#include "tao/Object.h"
#include "tao/GIOP.h"
#include "tao/NVList.h"
#include "tao/Invocation.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"

#if !defined (__ACE_INLINE__)
# include "tao/Stub.i"
#endif /* ! __ACE_INLINE__ */

#include "tao/Timeprobe.h"

ACE_RCSID(tao, STUB_Object, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_STUB_Object_Timeprobe_Description[] =
  {
    "STUB_Object::do_static_call - start",
    "STUB_Object::do_static_call - end",
    "STUB_Object::do_static_call - set_cancel",
    "STUB_Object::do_static_call - grab_orb_core",
    "STUB_Object::do_static_call - invocation_ctor",
    "STUB_Object::do_static_call - invocation_start",
    "STUB_Object::do_static_call - put_params"
  };

enum
  {
    // Timeprobe description table start key
    TAO_STUB_OBJECT_DO_STATIC_CALL_START = 500,
    TAO_STUB_OBJECT_DO_STATIC_CALL_END,
    TAO_STUB_OBJECT_DO_STATIC_CALL_SET_CANCEL,
    TAO_STUB_OBJECT_DO_STATIC_CALL_GRAB_ORB_CORE,
    TAO_STUB_OBJECT_DO_STATIC_CALL_INVOCATION_CTOR,
    TAO_STUB_OBJECT_DO_STATIC_CALL_INVOCATION_START,
    TAO_STUB_OBJECT_DO_STATIC_CALL_PUT_PARAMS
  };

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_STUB_Object_Timeprobe_Description,
                                  TAO_STUB_OBJECT_DO_STATIC_CALL_START);

#endif /* ACE_ENABLE_TIMEPROBES */

STUB_Object::STUB_Object (char *repository_id,
                          TAO_MProfile &profiles)
  : type_id (repository_id),
    base_profiles_ ((CORBA::ULong) 0),
    forward_profiles_ (0),
    profile_in_use_ (0),
    profile_lock_ptr_ (0),
    profile_success_ (0),
    // what about ACE_SYNCH_MUTEX refcount_lock_
    refcount_ (1),
    use_locate_request_ (0),
    first_locate_request_ (0)
{

  this->profile_lock_ptr_ =
    TAO_ORB_Core_instance ()->client_factory ()->create_iiop_profile_lock ();

  set_base_profiles (&profiles);
}

STUB_Object::STUB_Object (char *repository_id,
                          TAO_Profile *profile)
  : type_id (repository_id),
    base_profiles_ ((CORBA::ULong) 0),
    forward_profiles_ (0),
    profile_in_use_ (0),
    profile_lock_ptr_ (0),
    profile_success_ (0),
    // what about ACE_SYNCH_MUTEX refcount_lock_
    refcount_ (1),
    use_locate_request_ (0),
    first_locate_request_ (0)
{
  // @@ XXX need to verify type and deal with wrong types

  this->profile_lock_ptr_ =
    TAO_ORB_Core_instance ()->client_factory ()->create_iiop_profile_lock ();

  base_profiles_.set (1);

  base_profiles_.give_profile (profile);

  reset_base ();

}

STUB_Object::STUB_Object (char *repository_id,
                          TAO_MProfile *profiles)
  : type_id (repository_id),
    base_profiles_ ((CORBA::ULong) 0),
    forward_profiles_ (0),
    profile_in_use_ (0),
    profile_lock_ptr_ (0),
    profile_success_ (0),
    // what about ACE_SYNCH_MUTEX refcount_lock_
    refcount_ (1),
    use_locate_request_ (0),
    first_locate_request_ (0)
{
  // @@ XXX need to verify type and deal with wrong types

  // @@ does this need to be freed?
  this->profile_lock_ptr_ =
    TAO_ORB_Core_instance ()->client_factory ()->create_iiop_profile_lock ();

  set_base_profiles (profiles);

}

STUB_Object::STUB_Object (char *repository_id)
  : type_id (repository_id),
    base_profiles_ ((CORBA::ULong) 0),
    forward_profiles_ (0),
    profile_in_use_ (0),
    profile_lock_ptr_ (0),
    profile_success_ (0),
    // what about ACE_SYNCH_MUTEX refcount_lock_
    refcount_ (1),
    use_locate_request_ (0),
    first_locate_request_ (0)
{
  this->profile_lock_ptr_ =
    TAO_ORB_Core_instance ()->client_factory ()->create_iiop_profile_lock ();
}

// Quick'n'dirty hash of objref data, for partitioning objrefs into
// sets.
//
// NOTE that this must NOT go across the network!

// @@ Use all profiles for hash function!!!!!  FRED
//    can get different values, depending on the profile_in_use!!
CORBA::ULong
STUB_Object::hash (CORBA::ULong max,
                   CORBA::Environment &env)
{
  // we rely on the profile object to has it's address info
  if (profile_in_use_)
    return profile_in_use_->hash (max, env);
  ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) hash called on a null profile!\n"), 0);
}

int operator==(const TAO_opaque& rhs,
               const TAO_opaque& lhs)
{
  if (rhs.length () != lhs.length ())
    return 0;

  for (CORBA::ULong i = 0;
       i < rhs.length ();
       ++i)
    if (rhs[i] != lhs[i])
      return 0;

  return 1;
}

int operator!=(const TAO_opaque& rhs,
               const TAO_opaque& lhs)
{
  return !(rhs == lhs);
}

// Expensive comparison of objref data, to see if two objrefs
// certainly point at the same object. (It's quite OK for this to
// return FALSE, and yet have the two objrefs really point to the same
// object.)
//
// NOTE that this must NOT go across the network!
// @@ Two object references are the same if any two profiles are the same!
CORBA::Boolean
STUB_Object::is_equivalent (CORBA::Object_ptr other_obj,
                            CORBA::Environment &env)
{
  if (CORBA::is_nil (other_obj) == 1)
    return 0;

  TAO_Profile *other_profile = other_obj->_stubobj ()->profile_in_use_;
  TAO_Profile *this_profile = this->profile_in_use_;

  if (other_profile == 0 || this_profile == 0)
    return 0;

  // Compare the profiles
  return this_profile->is_equivalent (other_profile, env);
}

// Memory managment

CORBA::ULong
STUB_Object::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->refcount_lock_, 0);

  return this->refcount_++;
}

CORBA::ULong
STUB_Object::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->refcount_lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  delete this;
  return 0;
}

// Note that if the repository ID (typeID) is NULL, it will make
// narrowing rather expensive, though it does ensure that type-safe
// narrowing code gets thoroughly exercised/debugged!  Without a
// typeID, the _narrow will be required to make an expensive remote
// "is_a" call.

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

class TAO_Synchronous_Cancellation_Required
  // = TITLE
  //     Stick one of these at the beginning of a block that can't
  //     support asynchronous cancellation, and which must be
  //     cancel-safe.
  //
  // = EXAMPLE
  //     somefunc()
  //     {
  //       TAO_Synchronous_Cancellation_Required NOT_USED;
  //       ...
  //     }
{
public:
  // These should probably be in a separate inline file, but they're
  // only used within this one file right now, and we always want them
  // inlined, so here they sit.
  TAO_Synchronous_Cancellation_Required (void)
    : old_type_ (0)
    {
#if !defined (VXWORKS)
      ACE_OS::thr_setcanceltype (THR_CANCEL_DEFERRED, &old_type_);
#endif /* ! VXWORKS */
    }

  ~TAO_Synchronous_Cancellation_Required (void)
    {
#if !defined (VXWORKS)
      int dont_care;
      ACE_OS::thr_setcanceltype(old_type_, &dont_care);
#endif /* ! VXWORKS */
    }
private:
  int old_type_;
};

// "Stub interpreter" for static stubs.  IDL compiler (or human
// equivalent thereof :-) should just dump a read-only description of
// the call into "calldata" and do varargs calls to this routine,
// which does all the work.

void
STUB_Object::do_static_call (CORBA::Environment &ACE_TRY_ENV,
                             const TAO_Call_Data *info,
                             void** args)

{
  ACE_FUNCTION_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_START);

  TAO_Synchronous_Cancellation_Required NOT_USED;

  ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_SET_CANCEL);

  TAO_ORB_Core* orb_core = TAO_ORB_Core_instance ();

  ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_GRAB_ORB_CORE);

  // Do a locate_request if necessary/wanted.
  // Suspect that you will be forwarded, so be proactive!
  // strategy for reducing overhead when you think a request will
  // be forwarded.  No standard way now to know.
  if (this->use_locate_request_ && this->first_locate_request_)
    {
      TAO_GIOP_Locate_Request_Invocation call (this, orb_core);

      // Simply let these exceptions propagate up
      // (if any of them occurs.)
      call.start (ACE_TRY_ENV);
      ACE_CHECK;

      call.invoke (ACE_TRY_ENV);
      ACE_CHECK;

      this->first_locate_request_ = 0;
    }

  if (info->is_roundtrip)
    {
      TAO_GIOP_Twoway_Invocation call (this, info->opname, orb_core);
      ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_INVOCATION_CTOR);

      // We may need to loop through here more than once if we're
      // forwarded to some other object reference.
      //
      // NOTE: A quality-of-service policy may be useful to establish
      // here, specifically one controlling how many times the call is
      // reissued before failing the call on the assumption that
      // something is broken.
      //
      // NOTE: something missing is a dynamic way to change the policy
      // of whether to issue LocateRequest messages or not.  This code
      // uses a simple, fixed policy: never use LocateRequest
      // messages.
      //
      for (;;)
        {
          call.start (ACE_TRY_ENV);
          ACE_CHECK;

          ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_INVOCATION_START);

          // Make the call ... blocking for the response.
          this->put_params (ACE_TRY_ENV, info, call, args);
          ACE_CHECK;

          ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_PUT_PARAMS);

          int status =
            call.invoke (info->excepts,
                         info->except_count,
                         ACE_TRY_ENV);
          ACE_CHECK;

          if (status == TAO_INVOKE_RESTART)
            continue;

          if (status == TAO_INVOKE_EXCEPTION)
            return; // Shouldn't happen

          if (status != TAO_INVOKE_OK)
            ACE_THROW (CORBA::UNKNOWN (CORBA::COMPLETED_MAYBE));

          // The only case left is status == TAO_INVOKE_OK, exit the
          // loop.  We cannot retry because at this point we either
          // got a reply or something with an status of
          // COMPLETED_MAYBE, thus we cannot reissue the request if we
          // are to satisfy the "at most once" semantics.
          break;
        }

      // Now, get all the "return", "out", and "inout" parameters
      // from the response message body ... return parameter is
      // first, the rest are in the order defined in the IDL spec
      // (which is also the order that DII users are required to
      // use).

      const TAO_Param_Data *pdp = info->params;
      for (void** i = args;
           i !=  args + info->param_count;
           i++, pdp++)
        {
          void *ptr = *i;

          // if it is an inout parameter, it would become
          // necessary to first release the "in" memory
          if (pdp->mode == PARAM_INOUT)
            {
              // @@ TODO - add others as we test each case
              // (ASG) will do 03/22/98.
              // @@ IMHO this should be handled in the stub
              // (coryan)
              switch (pdp->tc->kind (TAO_IN_ENV))
                {
                case CORBA::tk_string:
                  {
                    CORBA::string_free (*(char **)ptr);
                    *(char **)ptr = 0;
                  }
                  break;
                default:
                  break;
                }
            }

          if (pdp->mode == PARAM_RETURN
              || pdp->mode == PARAM_OUT
              || pdp->mode == PARAM_INOUT)
            {
              // The language mapping's memory allocation
              // policy says that some data is heap-allocated.
              // This interpreter is told about the relevant
              // policy by whoever built the operation
              // description (e.g. the IDL compiler) so it
              // doesn't have to know the policy associated
              // with a particular language binding
              // (e.g. C/C++ differ, and C++ even has
              // different policies for different kinds of
              // structures).
              if (pdp->value_size == 0)
                {
                  call.get_value (pdp->tc, ptr, ACE_TRY_ENV);
                  ACE_CHECK;
                }
              else
                {
                  // @@ (ASG) -  I think we must completely
                  // get rid of this case because IDL compiler
                  // generated stubs will use this function
                  // and they better allocate all the memory.

                  // assert (value_size == tc->size());
                  *(void **)ptr = new CORBA::Octet [pdp->value_size];
                  call.get_value (pdp->tc, *(void **)ptr, ACE_TRY_ENV);
                  ACE_CHECK;
                }
            }
        }
    } // if (two way)
  else
    {
      TAO_GIOP_Oneway_Invocation call (this, info->opname, orb_core);
      ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_INVOCATION_CTOR);

      for (;;)
        {
          call.start (ACE_TRY_ENV);
          ACE_CHECK;

          this->put_params (ACE_TRY_ENV, info, call, args);
          ACE_CHECK;

          ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_PUT_PARAMS);
          int status = call.invoke (ACE_TRY_ENV);
          ACE_CHECK;

          if (status == TAO_INVOKE_RESTART)
            continue;

          if (status == TAO_INVOKE_EXCEPTION)
            return; // Shouldn't happen

          if (status != TAO_INVOKE_OK)
            ACE_THROW (CORBA::UNKNOWN (CORBA::COMPLETED_MAYBE));

          break;
        }
    }
}

void
STUB_Object::put_params (CORBA::Environment &ACE_TRY_ENV,
                         const TAO_Call_Data *info,
                         TAO_GIOP_Invocation &call,
                         void** args)
{
  // Now, put all "in" and "inout" parameters into the request
  // message body.
  //
  // Some "inout" data have an extra level of indirection,
  // specified by the language mapping's memory allocation
  // policies ... the indirection only shows up here when it's
  // needed later for allocating "out" memory, otherwise there's
  // just one indirection.

  const TAO_Param_Data *pdp = info->params;
  for (void** i = args;
       i != args + info->param_count;
       i++, pdp++)
    {
      void *ptr = *i;

      if (pdp->mode == PARAM_IN)
        {
          call.put_param (pdp->tc, ptr, ACE_TRY_ENV);
        }
      else if (pdp->mode == PARAM_INOUT)
        {
          if (pdp->value_size == 0)
            call.put_param (pdp->tc, ptr, ACE_TRY_ENV);
          else
            call.put_param (pdp->tc, *(void **)ptr, ACE_TRY_ENV);
        }
      ACE_CHECK;
    }
}

#if !defined (TAO_HAS_MINIMUM_CORBA)

// DII analogue of the above.

void
STUB_Object::do_dynamic_call (const char *opname,
                              CORBA::Boolean is_roundtrip,
                              CORBA::NVList_ptr args,
                              CORBA::NamedValue_ptr result,
                              CORBA::Flags,
                              CORBA::ExceptionList &exceptions,
                              CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Synchronous_Cancellation_Required NOT_USED;

  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();

  // Do a locate_request if necessary/wanted.
  // Suspect that you will be forwarded, so be proactive!
  // strategy for reducing overhead when you think a request will
  // be forwarded.  No standard way now to know.
  if (this->use_locate_request_ && this->first_locate_request_)
    {
      TAO_GIOP_Locate_Request_Invocation call (this, orb_core);

      // Simply let these exceptions propagate up
      // (if any of them occurs.)
      call.start (ACE_TRY_ENV);
      ACE_CHECK;

      call.invoke (ACE_TRY_ENV);
      ACE_CHECK;

      this->first_locate_request_ = 0;
    }

  if (is_roundtrip)
    {
      TAO_GIOP_Twoway_Invocation call (this, opname, orb_core);

      // Loop as needed for forwarding; see above.

      for (;;)
        {
          call.start (ACE_TRY_ENV);
          ACE_CHECK;

          this->put_params (call, args, ACE_TRY_ENV);
          ACE_CHECK;

          // Make the call ... blocking for the response.
          int status =
            call.invoke (exceptions, ACE_TRY_ENV);
          ACE_CHECK;

          if (status == TAO_INVOKE_RESTART)
            continue;

          if (status == TAO_INVOKE_EXCEPTION)
            return; // Shouldn't happen

          if (status != TAO_INVOKE_OK)
            ACE_THROW (CORBA::UNKNOWN (CORBA::COMPLETED_MAYBE));

          // The only case left is status == TAO_INVOKE_OK, exit the
          // loop.  We cannot retry because at this point we either
          // got a reply or something with an status of
          // COMPLETED_MAYBE, thus we cannot reissue the request if we
          // are to satisfy the "at most once" semantics.
          break;
        }

      // Now, get all the "return", "out", and "inout" parameters
      // from the response message body ... return parameter is
      // first, the rest are in the order defined in the IDL spec
      // (which is also the order that DII users are required to
      // use).

      if (result != 0)
        {
#if 0
          // @@ (ASG) I need to look into this OUT_LIST_MEMORY stuff
          // (4/21/98).
          // @@ (Carlos) All this code seems bogus, we know that
          //    allocating memory of behalf of the user is and endless
          //    source of trouble (due to vtbls and the such).

          // If caller didn't set OUT_LIST_MEMORY flag, allocate
          // memory for return value ...

          if (!(flags & CORBA::OUT_LIST_MEMORY))
            {
              CORBA::TypeCode_var tcp = result->value ()->type ();
              size_t size = tcp->size (ACE_TRY_ENV);
              ACE_CHECK;

              if (size != 0)
                {
                  void *ptr = new CORBA::Octet [size];

                  result->value ()->replace (tcp.in (), ptr, 1, ACE_TRY_ENV);
                  ACE_CHECK;
                }
            }
#endif
          if (!result->value ()->value_)
            {
              // storage was not allocated. In this case, we
              // simply grab the portion of the CDR stream
              // that contained this parameter, The
              // application should use the appropriate >>=
              // operator to retrieve the value
              char *begin, *end;
              TAO_InputCDR temp (call.inp_stream ());
              CORBA::TypeCode::traverse_status retval;
              CORBA::Any *any = result->value ();

              begin = call.inp_stream ().rd_ptr ();
              // skip the parameter to get the ending position
              retval = temp.skip (any->type_, ACE_TRY_ENV);
              ACE_CHECK;

              if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                {
                  end = temp.rd_ptr ();
                  any->cdr_ = new ACE_Message_Block (end - begin);
                  TAO_OutputCDR out (any->cdr_);

                  retval = out.append (any->type_,
                                       &call.inp_stream (), ACE_TRY_ENV);
                  ACE_CHECK;

                  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                    {
                      any->any_owns_data_ = 1;
                      any->value_ = 0;
                    }
                }
            }
          else
            {
              // the application had allocated the top level
              // storage. We simply retrieve the data
              call.get_value (result->value ()->type_,
                              result->value ()->value_, ACE_TRY_ENV);
              ACE_CHECK;
            }
        }

      for (u_int i = 0; i < args->count (); i++)
        {
          CORBA::NamedValue_ptr value = args->item (i, ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::Any *any = value->value ();

          if (value->flags () == CORBA::ARG_OUT
              || value->flags () == CORBA::ARG_INOUT)
            {
#if 0
              // @@  (ASG) need to deal with this

              // If caller didn't set OUT_LIST_MEMORY flag, allocate
              // memory for this parameter ...
              if (!(flags & CORBA::OUT_LIST_MEMORY))
                {
                  CORBA::TypeCode_var tcp = value->value ()->type ();
                  size_t size = tcp->size (ACE_TRY_ENV);
                  ACE_CHECK;

                  if (size != 0)
                    {
                      CORBA::Octet *ptr = new CORBA::Octet [size];

                      value->value ()->replace (tcp.in (), ptr,
                                                1, ACE_TRY_ENV);
                      ACE_CHECK;
                    }
                }
#endif
              if (!any->value_)
                {
                  // storage was not allocated. In this case,
                  // we simply grab the portion of the CDR
                  // stream that contained this parameter, The
                  // application should use the appropriate
                  // >>= operator to retrieve the value
                  char *begin, *end;
                  TAO_InputCDR temp (call.inp_stream ());
                  CORBA::TypeCode::traverse_status retval;

                  begin = call.inp_stream ().rd_ptr ();
                  // skip the parameter to get the ending position
                  retval = temp.skip (any->type_, ACE_TRY_ENV);
                  ACE_CHECK;

                  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                    {
                      end = temp.rd_ptr ();
                      any->cdr_ = new ACE_Message_Block (end - begin);
                      TAO_OutputCDR out (any->cdr_);

                      retval = out.append (any->type_,
                                           &call.inp_stream (), ACE_TRY_ENV);
                      ACE_CHECK;

                      if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                        {
                          any->any_owns_data_ = 1;
                          any->value_ = 0;
                        }
                    }
                }
              else
                {
                  // the application had allocated the top level
                  // storage. We simply retrieve the data
                  call.get_value (any->type_,
                                  (void *) any->value_, ACE_TRY_ENV);
                  ACE_CHECK;
                }
            }
        }
    }
  else
    {
      TAO_GIOP_Oneway_Invocation call (this, opname, orb_core);

      for (;;)
        {
          call.start (ACE_TRY_ENV);
          ACE_CHECK;

          this->put_params (call, args, ACE_TRY_ENV);
          ACE_CHECK;

          int status = call.invoke (ACE_TRY_ENV);
          ACE_CHECK;

          if (status == TAO_INVOKE_RESTART)
            continue;

          if (status == TAO_INVOKE_EXCEPTION)
            return; // Shouldn't happen

          if (status != TAO_INVOKE_OK)
            ACE_THROW (CORBA::UNKNOWN (CORBA::COMPLETED_MAYBE));

          break;
        }
    }
}

void
STUB_Object::put_params (TAO_GIOP_Invocation &call,
                         CORBA::NVList_ptr args,
                         CORBA::Environment &ACE_TRY_ENV)
{
  // Now, put all "in" and "inout" parameters into the request
  // message body.

  for (u_int i = 0; i < args->count (); i++)
    {
      CORBA::NamedValue_ptr value = args->item (i, ACE_TRY_ENV);
      ACE_CHECK;

      if (value->flags () == CORBA::ARG_IN
          || value->flags () == CORBA::ARG_INOUT)
        {
          // If the Any owns the data, then we have allocated space.
          if (value->value ()->any_owns_data_)
            {
              call.put_param (value->value ()->type_,
                              value->value ()->value_, ACE_TRY_ENV);
              ACE_CHECK;
            }
          else
            {
              TAO_OutputCDR &cdr = call.out_stream ();
              TAO_InputCDR in (value->value ()->cdr_);
              cdr.append (value->value ()->type_, &in, ACE_TRY_ENV);
              ACE_CHECK;
            }
        }
    }
}

#endif /* TAO_HAS_MINIMUM_CORBA */
