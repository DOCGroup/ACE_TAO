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
STUB_Object::do_static_call (CORBA::Environment &TAO_IN_ENV,
                             const TAO_Call_Data *info,
                             void** args)

{
  ACE_FUNCTION_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_START);

  TAO_Synchronous_Cancellation_Required NOT_USED;

  ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_SET_CANCEL);

  TAO_ORB_Core* orb_core = TAO_ORB_Core_instance ();

  ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_GRAB_ORB_CORE);

  TAO_GIOP_ReplyStatusType status = TAO_GIOP_NO_EXCEPTION;

  // Do a locate_request if necessary/wanted.
  // Suspect that you will be forwarded, so be proactive!
  // strategy for reducing overhead when you think a request will
  // be forwarded.  No standard way now to know.
  if (this->use_locate_request_ && this->first_locate_request_)
    {
      TAO_GIOP_Locate_Request_Invocation call (this, orb_core);

      // Simply let these exceptions propagate up
      // (if any of them occurs.)
      call.start (TAO_IN_ENV);

      status = call.invoke (TAO_IN_ENV);

      this->first_locate_request_ = 0;

      if (status == TAO_GIOP_SYSTEM_EXCEPTION)
        return;
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
          // Start the call by constructing the request message header.
	  // and connecting to the server.
          TAO_TRY_VAR_EX (TAO_IN_ENV, SYSEX1)
            {
              call.start (TAO_IN_ENV);
              TAO_CHECK_ENV_EX (SYSEX1);

              ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_INVOCATION_START);
            }
          TAO_CATCH (CORBA_SystemException, ex)
            {
              ACE_MT (ACE_GUARD (ACE_Lock,
                                guard,
                                *this->profile_lock_ptr_));

	      // get the next profile and try again
	      // If a forward profile once succeeded but now fails then
	      // start all over again.  Otherwise get the next profile and 
	      // try again.  If this was the last profile in the list then
	      // stop.
	      if (profile_success_ && forward_profiles_)
		{
		  // reset profiles list and start all over again
		  reset_profiles ();
                  TAO_IN_ENV.clear ();
                  TAO_GOTO (roundtrip_continue_label);
                }
              else if (next_profile () != 0)
		{
                  TAO_IN_ENV.clear ();
  		  TAO_GOTO (roundtrip_continue_label);
		}
	
              // @@ Should re reset the profile list here?
              reset_profiles ();
              TAO_RETHROW_SAME_ENV_RETURN_VOID;
            }
          TAO_ENDTRY;

          this->put_params (TAO_IN_ENV, info, call, args);
          TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

          ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_PUT_PARAMS);

          TAO_TRY_VAR_EX (TAO_IN_ENV, SYSEX2)
            {
              status = call.invoke (info->excepts, info->except_count, TAO_IN_ENV);
              TAO_CHECK_ENV_EX (SYSEX2);
            }
          TAO_CATCH (CORBA_SystemException, ex)
            {
              ACE_MT (ACE_GUARD (ACE_Lock,
                                guard,
                                *this->profile_lock_ptr_));

	      if (profile_success_ && forward_profiles_)
		{
		  // reset profiles list and start all over again
		  reset_profiles ();
                  TAO_IN_ENV.clear ();
                  TAO_GOTO (roundtrip_continue_label);
                }
              else if (next_profile () != 0)
		{
                  TAO_IN_ENV.clear ();
  		  TAO_GOTO (roundtrip_continue_label);
		}
	
              // @@ Should re reset the profile list here?
              reset_profiles ();
              TAO_RETHROW_SAME_ENV_RETURN_VOID;
            }
          TAO_ENDTRY;

          if (status == TAO_GIOP_USER_EXCEPTION)
            return;
          else if (status == TAO_GIOP_NO_EXCEPTION)
            {
	      profile_success_ = 1;

              // Now, get all the "return", "out", and "inout"
              // parameters from the response message body.

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
                        call.get_value (pdp->tc, ptr, TAO_IN_ENV);
                      else
                        {
                          // @@ (ASG) -  I think we must completely
                          // get rid of this case because IDL compiler
                          // generated stubs will use this function
                          // and they better allocate all the memory.

                          // assert (value_size == tc->size());
                          *(void **)ptr = new CORBA::Octet [pdp->value_size];
                          call.get_value (pdp->tc, *(void **)ptr, TAO_IN_ENV);
                        }

                      if (TAO_IN_ENV.exception ())
                        {
                          TAO_IN_ENV.print_exception ("do_static_call, get reply parameter");
                          return;
                        }
                    }
                }
              return;
            }
          // ... or maybe this request got forwarded to someplace
          // else; send the request there instead.
          if (status != TAO_GIOP_LOCATION_FORWARD)
            {
              // @@ What is the right exception to throw in this case?
              // TRANSIENT - FRED
              TAO_IN_ENV.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_MAYBE));
              return;
            }
          TAO_LABEL (roundtrip_continue_label);
        } // for loop
    } // if (two way)
  else
    {
      for (;;)
        {
          TAO_GIOP_Oneway_Invocation call (this, info->opname, orb_core);
          ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_INVOCATION_CTOR);

          // Start the call by constructing the request message header.
          TAO_TRY_VAR_EX (TAO_IN_ENV, SYSEX3)
            {
              call.start (TAO_IN_ENV);
              TAO_CHECK_ENV_EX (SYSEX3);
              ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_INVOCATION_START);
            }
          TAO_CATCH (CORBA_SystemException, ex)
            {
              ACE_MT (ACE_GUARD (ACE_Lock,
                                guard,
	                                *this->profile_lock_ptr_));

              // If this is the forward_profile, then check to see if we
              // need to go back to the original profile and try that.
	      if (profile_success_ && forward_profiles_)
		{
		  // reset profiles list and start all over again
		  reset_profiles ();
                  TAO_IN_ENV.clear ();
                  TAO_GOTO (oneway_continue_label);
                }
              else if (next_profile () != 0)
		{
                  TAO_IN_ENV.clear ();
  		  TAO_GOTO (oneway_continue_label);
		}
	
              // @@ Should re reset the profile list here?
              reset_profiles ();
              TAO_RETHROW_SAME_ENV_RETURN_VOID;
            }
          TAO_ENDTRY;

          this->put_params (TAO_IN_ENV, info, call, args);
          TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

          ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_PUT_PARAMS);
          /* TAO_GIOP_ReplyStatusType status = */ call.invoke (TAO_IN_ENV);

          profile_success_ = 1;

          // @@ TODO We do not get any LOCATION_FORWARD in this case,
          // IMHO this is a good case for use of a LocateRequest,
          // under some strategy control, of course. In that case we need
          // a loop, as above.
          return;
        }
      TAO_LABEL (oneway_continue_label);
    }
}

void
STUB_Object::put_params (CORBA::Environment &env,
                         const TAO_Call_Data *info,
                         TAO_GIOP_Invocation &call,
                         void** args)
{
  if (env.exception ())
    {
      env.print_exception ("do_static_call, start request message");
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

  const TAO_Param_Data *pdp = info->params;
  for (void** i = args;
       i != args + info->param_count;
       i++, pdp++)
    {
      void *ptr = *i;

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
          env.print_exception ("do_static_call, put request parameter");
          return;
        }
    }
}

// DII analogue of the above.

void
STUB_Object::do_dynamic_call (const char *opname,
                              CORBA::Boolean is_roundtrip,
                              CORBA::NVList_ptr args,
                              CORBA::NamedValue_ptr result,
                              CORBA::Flags,
                              CORBA::ExceptionList &exceptions,
                              CORBA::Environment &env)
{
  TAO_Synchronous_Cancellation_Required NOT_USED;

  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();
  if (is_roundtrip)
    {
      TAO_GIOP_Twoway_Invocation call (this, opname, orb_core);

      // Loop as needed for forwarding; see above.

      for (;;)
        {
          call.start (env);
          if (env.exception () != 0) return;

          this->put_params (call, args, env);
          if (env.exception () != 0) return;

          // Make the call ... blocking for the response.
          TAO_GIOP_ReplyStatusType status =
            call.invoke (exceptions, env);
          if (env.exception ())
            {
              env.print_exception ("do_dynamic_call, invoke");
              return;
            }
          if (status == TAO_GIOP_SYSTEM_EXCEPTION
              || status == TAO_GIOP_USER_EXCEPTION)
            return;

          // Now, get all the "return", "out", and "inout" parameters
          // from the response message body ... return parameter is
          // first, the rest are in the order defined in the IDL spec
          // (which is also the order that DII users are required to
          // use).

          if (status == TAO_GIOP_NO_EXCEPTION)
            {
              if (result != 0)
                {
#if 0
                  // @@ (ASG) I need to look into this OUT_LIST_MEMORY stuff
                  // (4/21/98).

                  // If caller didn't set OUT_LIST_MEMORY flag, allocate
                  // memory for return value ...

                  if (!(flags & CORBA::OUT_LIST_MEMORY))
                    {
                      CORBA::TypeCode_var tcp = result->value ()->type ();
                      size_t size = tcp->size (env);
                      env.print_exception ("do_dynamic_call, get result size");

                      if (size != 0)
                        {
                          void *ptr = new CORBA::Octet [size];

                          result->value ()->replace (tcp.in (), ptr,
                                                     1, env);
                          env.print_exception ("do_dynamic_call, set result mem");
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
                      retval = temp.skip (any->type_, env);
                      if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                        {
                          end = temp.rd_ptr ();
                          any->cdr_ = new ACE_Message_Block (end - begin);
                          TAO_OutputCDR out (any->cdr_);

                          retval = out.append (any->type_,
                                               &call.inp_stream (), env);
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
                                      result->value ()->value_, env);
                    }
                }

              for (u_int i = 0; i < args->count (); i++)
                {
                  CORBA::NamedValue_ptr value = args->item (i, env);
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
                          size_t size = tcp->size (env);
                          env.print_exception ("do_dynamic_call, get param size");

                          if (size != 0)
                            {
                              CORBA::Octet *ptr = new CORBA::Octet [size];

                              value->value ()->replace (tcp.in (), ptr,
                                                        1, env);
                              env.print_exception ("do_dynamic_call, set result mem");
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
                          retval = temp.skip (any->type_, env);
                          if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                            {
                              end = temp.rd_ptr ();
                              any->cdr_ = new ACE_Message_Block (end - begin);
                              TAO_OutputCDR out (any->cdr_);

                              retval = out.append (any->type_,
                                                   &call.inp_stream (), env);
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
                                          (void *) any->value_, env);
                        }
                      if (env.exception ())
                        {
                          env.print_exception ("do_dynamic_call, get response parameter");
                          return;
                        }
                    }
                }
              return;
            }
          // ... or maybe this request got forwarded to someplace else.
          assert (status == TAO_GIOP_LOCATION_FORWARD);
          // @@ TODO We should not use assert because it crashes the
          // application, raise an exception
        }
    }
  else
    {
      TAO_GIOP_Oneway_Invocation call (this, opname, orb_core);
      call.start (env);
      if (env.exception () != 0) return;

      this->put_params (call, args, env);
      if (env.exception () != 0) return;

      (void) call.invoke (env);
    }

}

void
STUB_Object::put_params (TAO_GIOP_Invocation &call,
                         CORBA::NVList_ptr args,
                         CORBA::Environment &env)
{
  // Now, put all "in" and "inout" parameters into the request
  // message body.

  for (u_int i = 0; i < args->count (); i++)
    {
      CORBA::NamedValue_ptr value = args->item (i, env);

      if (value->flags () == CORBA::ARG_IN
          || value->flags () == CORBA::ARG_INOUT)
        {
          // If the Any owns the data, then we have allocated space.
          if (value->value ()->any_owns_data_)
            {
              call.put_param (value->value ()->type_,
                              value->value ()->value_, env);
            }
          else
            {
              TAO_OutputCDR &cdr = call.out_stream ();
              TAO_InputCDR in (value->value ()->cdr_);
              cdr.append (value->value ()->type_, &in, env);
            }
          if (env.exception ())
            {
              env.print_exception ("do_dynamic_call, put request parameter");
              return;
            }
        }
    }
}
