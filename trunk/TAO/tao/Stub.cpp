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
#include "tao/Object.h"
#include "tao/GIOP.h"
#include "tao/NVList.h"
#include "tao/Invocation.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/debug.h"
#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
# include "tao/Stub.i"
#endif /* ! __ACE_INLINE__ */

#include "tao/Timeprobe.h"

ACE_RCSID(tao, TAO_Stub, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_TAO_Stub_Timeprobe_Description[] =
  {
    "TAO_Stub::do_static_call - start",
    "TAO_Stub::do_static_call - end",
    "TAO_Stub::do_static_call - set_cancel",
    "TAO_Stub::do_static_call - grab_orb_core",
    "TAO_Stub::do_static_call - invocation_ctor",
    "TAO_Stub::do_static_call - invocation_start",
    "TAO_Stub::do_static_call - put_params"
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
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_TAO_Stub_Timeprobe_Description,
                                  TAO_STUB_OBJECT_DO_STATIC_CALL_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_Stub::TAO_Stub (char *repository_id,
                    const TAO_MProfile &profiles,
                    TAO_ORB_Core* orb_core)
  : type_id (repository_id),
    base_profiles_ ((CORBA::ULong) 0),
    forward_profiles_ (0),
    profile_in_use_ (0),
    profile_lock_ptr_ (0),
    profile_success_ (0),
    refcount_lock_ (),
    refcount_ (1),
    use_locate_request_ (0),
    first_locate_request_ (0),
    orb_core_ (orb_core),
    orb_ (),
    servant_orb_ ()
#if defined (TAO_HAS_CORBA_MESSAGING)
    , policies_ (0)
#endif /* TAO_HAS_CORBA_MESSAGING */
{
  if (this->orb_core_ == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO: (%P|%t) TAO_Stub created with default "
                      "ORB core\n"));
        }
      this->orb_core_ = TAO_ORB_Core_instance ();
    }

  // Duplicate the ORB. This will help us keep the ORB around until
  // the CORBA::Object we represent dies.
  this->orb_ = CORBA::ORB::_duplicate (this->orb_core_->orb ());

  this->profile_lock_ptr_ =
    this->orb_core_->client_factory ()->create_profile_lock ();

  this->base_profiles (profiles);
}

TAO_Stub::~TAO_Stub (void)
{
  assert (this->refcount_ == 0);

  if (forward_profiles_)
    reset_profiles ();

  if (this->profile_in_use_ != 0)
    {
      this->profile_in_use_->reset_hint ();
      // decrease reference count on profile
      this->profile_in_use_->_decr_refcnt ();
      this->profile_in_use_ = 0;
    }

  if (this->profile_lock_ptr_)
    delete this->profile_lock_ptr_;

#if defined (TAO_HAS_CORBA_MESSAGING)
  delete this->policies_;
#endif /* TAO_HAS_CORBA_MESSAGING */
}

void
TAO_Stub::add_forward_profiles (const TAO_MProfile &mprofiles)
{
  // we assume that the profile_in_use_ is being
  // forwarded!  Grab the lock so things don't change.
  ACE_MT (ACE_GUARD (ACE_Lock,
                     guard,
                     *this->profile_lock_ptr_));

  TAO_MProfile *now_pfiles = this->forward_profiles_;
  if (now_pfiles == 0)
    now_pfiles = &this->base_profiles_;

  ACE_NEW (this->forward_profiles_,
           TAO_MProfile (mprofiles));

  // forwarded profile points to the new IOR (profiles)
  this->profile_in_use_->forward_to (this->forward_profiles_);

  // new profile list points back to the list which was forwarded.
  this->forward_profiles_->forward_from (now_pfiles);

  // make sure we start at the beginning of mprofiles
  this->forward_profiles_->rewind ();
}

// Quick'n'dirty hash of objref data, for partitioning objrefs into
// sets.
//
// NOTE that this must NOT go across the network!

// @@ Use all profiles for hash function!!!!!  FRED
//    can get different values, depending on the profile_in_use!!
CORBA::ULong
TAO_Stub::hash (CORBA::ULong max,
                CORBA::Environment &ACE_TRY_ENV)
{
  // we rely on the profile object to has it's address info
  if (profile_in_use_)
    return profile_in_use_->hash (max, ACE_TRY_ENV);
  ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) hash called on a null profile!\n"), 0);
}

// Expensive comparison of objref data, to see if two objrefs
// certainly point at the same object. (It's quite OK for this to
// return FALSE, and yet have the two objrefs really point to the same
// object.)
//
// NOTE that this must NOT go across the network!
// @@ Two object references are the same if any two profiles are the same!
CORBA::Boolean
TAO_Stub::is_equivalent (CORBA::Object_ptr other_obj)
{
  if (CORBA::is_nil (other_obj) == 1)
    return 0;

  TAO_Profile *other_profile = other_obj->_stubobj ()->profile_in_use_;
  TAO_Profile *this_profile = this->profile_in_use_;

  if (other_profile == 0 || this_profile == 0)
    return 0;

  // Compare the profiles
  return this_profile->is_equivalent (other_profile);
}

// Memory managment

CORBA::ULong
TAO_Stub::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->refcount_lock_, 0);

  return this->refcount_++;
}

CORBA::ULong
TAO_Stub::_decr_refcnt (void)
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
TAO_Stub::do_static_call (CORBA::Environment &ACE_TRY_ENV,
                             const TAO_Call_Data *info,
                             void** args)

{
  ACE_FUNCTION_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_START);

  TAO_Synchronous_Cancellation_Required NOT_USED;

  ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_SET_CANCEL);

  ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_GRAB_ORB_CORE);

  // Do a locate_request if necessary/wanted.
  // Suspect that you will be forwarded, so be proactive!
  // strategy for reducing overhead when you think a request will
  // be forwarded.  No standard way now to know.
  if (this->use_locate_request_ && this->first_locate_request_)
    {
      TAO_GIOP_Locate_Request_Invocation call (this,
                                               this->orb_core_);

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
      TAO_GIOP_Twoway_Invocation call (this, info->opname,
                                       this->orb_core_);
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
            ACE_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));

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
              CORBA::TCKind kind = pdp->tc->kind (ACE_TRY_ENV);
              ACE_CHECK;

              switch (kind)
                {
                  case CORBA::tk_string:
                    CORBA::string_free (*(char **) ptr);
                    *(char **)ptr = 0;
                    break;
                  case CORBA::tk_objref:
                    CORBA::release (*(CORBA::Object_ptr *) ptr);
                    break;
                  case CORBA::tk_any:
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
                  ACE_NEW (*(void **)ptr,
                           CORBA::Octet [pdp->value_size]);
                  call.get_value (pdp->tc, *(void **)ptr, ACE_TRY_ENV);
                  ACE_CHECK;
                }
            }
        }
    } // if (two way)
  else
    {
      TAO_GIOP_Oneway_Invocation call (this, info->opname,
                                       this->orb_core_);
      ACE_TIMEPROBE (TAO_STUB_OBJECT_DO_STATIC_CALL_INVOCATION_CTOR);

      for (;;)
        {
          call.start (ACE_TRY_ENV);
          ACE_CHECK;

          call.prepare_header (0, ACE_TRY_ENV);
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
            ACE_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));

          break;
        }
    }
}

void
TAO_Stub::put_params (CORBA::Environment &ACE_TRY_ENV,
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
TAO_Stub::do_dynamic_call (const char *opname,
                              CORBA::Boolean is_roundtrip,
                              CORBA::NVList_ptr args,
                              CORBA::NamedValue_ptr result,
                              CORBA::Flags,
                              CORBA::ExceptionList &exceptions,
                              CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Synchronous_Cancellation_Required NOT_USED;

  // Do a locate_request if necessary/wanted.
  // Suspect that you will be forwarded, so be proactive!
  // strategy for reducing overhead when you think a request will
  // be forwarded.  No standard way now to know.
  if (this->use_locate_request_ && this->first_locate_request_)
    {
      TAO_GIOP_Locate_Request_Invocation call (this, this->orb_core_);

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
      TAO_GIOP_Twoway_Invocation call (this, opname, this->orb_core_);

      // Loop as needed for forwarding; see above.

      for (;;)
        {
          call.start (ACE_TRY_ENV);
          ACE_CHECK;

          call.prepare_header (1, ACE_TRY_ENV);
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
            ACE_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));

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
                  ACE_NEW (any->cdr_,
                           ACE_Message_Block (end - begin));
                  any->cdr_->wr_ptr (end - begin);
                  TAO_OutputCDR out (any->cdr_);
                  retval = out.append (any->type_,
                                       &call.inp_stream (), ACE_TRY_ENV);
                  ACE_CHECK;

                  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                    {
                      any->value_ = 0;
                      any->any_owns_data_ = 0;
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
                  retval = temp.skip (any->type_,
                                      ACE_TRY_ENV);
                  ACE_CHECK;

                  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                    {
                      end = temp.rd_ptr ();
                      ACE_NEW (any->cdr_,
                               ACE_Message_Block (end - begin));
                      any->cdr_->wr_ptr (end - begin);
                      TAO_OutputCDR out (any->cdr_);

                      retval = out.append (any->type_,
                                           &call.inp_stream (),
                                           ACE_TRY_ENV);
                      ACE_CHECK;

                      if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                        {
                          any->value_ = 0;
                          any->any_owns_data_ = 0;
                        }
                    }
                }
              else
                {
                  // the application had allocated the top level
                  // storage. We simply retrieve the data.
                  // But first we must gracefully release the 'in'
                  // part if our parameter is INOUT. As with the
                  // SII counterpart above, this test is incomplete.
                  if (value->flags () == CORBA::ARG_INOUT)
                    {
                      switch (any->type_->kind_)
                        {
                          case CORBA::tk_string:
                            CORBA::string_free (*(char **) any->value_);
                            break;
                          case CORBA::tk_objref:
                            break;
                          case CORBA::tk_any:
                            {
                              CORBA_Any_ptr inside_any = (CORBA_Any *) any->value_;
                              inside_any->free_value (ACE_TRY_ENV);
                              ACE_CHECK;
                            }
                            break;
                          default:
                            break;
                        }
                    }

                  call.get_value (any->type_,
                                  (void *) any->value_,
                                  ACE_TRY_ENV);
                  ACE_CHECK;
                }
            }
        }
    }
  else
    {
      TAO_GIOP_Oneway_Invocation call (this, opname, this->orb_core_);

      for (;;)
        {
          call.start (ACE_TRY_ENV);
          ACE_CHECK;

          call.prepare_header (0, ACE_TRY_ENV);
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
            ACE_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));

          break;
        }
    }
}

void
TAO_Stub::put_params (TAO_GIOP_Invocation &call,
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
              TAO_InputCDR in (value->value ()->cdr_,
                               TAO_ENCAP_BYTE_ORDER,
                               this->orb_core_);
              cdr.append (value->value ()->type_, &in, ACE_TRY_ENV);
              ACE_CHECK;
            }
        }
    }
}

#endif /* TAO_HAS_MINIMUM_CORBA */

// ****************************************************************

#if defined (TAO_HAS_CORBA_MESSAGING)

CORBA::Policy_ptr
TAO_Stub::get_policy (
    CORBA::PolicyType type,
    CORBA::Environment &ACE_TRY_ENV)
{
  // No need to lock, the stub only changes its policies at
  // construction time...

  if (this->policies_ == 0)
    return CORBA::Policy::_nil ();

  return this->policies_->get_policy (type, ACE_TRY_ENV);
}

CORBA::Policy_ptr
TAO_Stub::get_client_policy (
    CORBA::PolicyType type,
    CORBA::Environment &ACE_TRY_ENV)
{
  // No need to lock, the stub only changes its policies at
  // construction time...

  CORBA::Policy_var result;
  if (this->policies_ != 0)
    {
      result = this->policies_->get_policy (type, ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  if (CORBA::is_nil (result.in ()))
    {
      TAO_Policy_Current &policy_current = this->orb_core_->policy_current ();
      result = policy_current.get_policy (type, ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  if (CORBA::is_nil (result.in ()))
    {
      // @@ Must lock, but is is harder to implement than just modifying
      //    this call: the ORB does take a lock to modify the policy
      //    manager
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        {
          result = policy_manager->get_policy (type, ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::Policy::_nil ());
        }
    }

  if (CORBA::is_nil (result.in ()))
    {
      result = this->orb_core_->get_default_policy (type, ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  return result._retn ();
}

POA_TAO::ClientPriorityPolicy*
TAO_Stub::client_priority (void)
{
  // No need to lock, the stub only changes its policies at
  // construction time...

  POA_TAO::ClientPriorityPolicy* result = 0;
  if (this->policies_ != 0)
    result = this->policies_->client_priority ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.client_priority ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->client_priority ();
    }

  if (result == 0)
    result = this->orb_core_->default_client_priority ();

  return result;
}

POA_Messaging::RelativeRoundtripTimeoutPolicy*
TAO_Stub::relative_roundtrip_timeout (void)
{
  // No need to lock, the stub only changes its policies at
  // construction time...

  POA_Messaging::RelativeRoundtripTimeoutPolicy* result = 0;
  if (this->policies_ != 0)
    result = this->policies_->relative_roundtrip_timeout ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.relative_roundtrip_timeout ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->relative_roundtrip_timeout ();
    }

  if (result == 0)
    result = this->orb_core_->default_relative_roundtrip_timeout ();

  return result;
}

TAO_Stub*
TAO_Stub::set_policy_overrides (
    const CORBA::PolicyList & policies,
    CORBA::SetOverrideType set_add,
    CORBA::Environment &ACE_TRY_ENV)
{
  // Notice the use of an explicit constructor....
  auto_ptr<TAO_Policy_Manager_Impl> policy_manager (new TAO_Policy_Manager_Impl);

  if (set_add == CORBA::SET_OVERRIDE)
    {
      policy_manager->set_policy_overrides (policies,
                                            set_add,
                                            ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }
  else if (this->policies_ == 0)
    {
      policy_manager->set_policy_overrides (policies,
                                            CORBA::SET_OVERRIDE,
                                            ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
     }
  else
    {
      policy_manager->copy_from (this->policies_,
                                 ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
      policy_manager->set_policy_overrides (policies,
                                            set_add,
                                            ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  TAO_Stub* stub;
  ACE_NEW_RETURN (stub, TAO_Stub (CORBA::string_dup (this->type_id.in ()),
                                  this->base_profiles_,
                                  this->orb_core_),
                  0);
  stub->policies_ = policy_manager.release ();

  // Copy the servant ORB if it is present.
  stub->servant_orb (this->servant_orb_var ().in ());

  return stub;
}

CORBA::PolicyList *
TAO_Stub::get_policy_overrides (
    const CORBA::PolicyTypeSeq & types,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (this->policies_ == 0)
    return 0;

  return this->policies_->get_policy_overrides (types, ACE_TRY_ENV);
}

CORBA::Boolean
TAO_Stub::validate_connection (CORBA::PolicyList_out inconsistent_policies,
                               CORBA::Environment &ACE_TRY_ENV)
{
  inconsistent_policies = 0;

  // Check if we care about Client Priority policy, and store the
  // result in the variable called <set>.
  int set = 1;
  POA_TAO::ClientPriorityPolicy *policy =
    this->client_priority ();
  if (policy == 0)
    set = 0;
  else
    // Policy is set.
    {
      TAO::PrioritySpecification priority_spec =
        policy->priority_specification (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
      TAO::PrioritySelectionMode mode = priority_spec.mode;

      // Don't care about priority.
      if (mode == TAO::USE_NO_PRIORITY)
        set = 0;
    }

  // Use Locate Request to establish connection/make sure the object
  // is there ...
  TAO_GIOP_Locate_Request_Invocation locate_request (this,
                                                     this->orb_core_);

  //@@ Currently, if we select profiles based on priorities (i.e.,
  //  ClientPriorityPolicy is set), and we get a FORWARD reply to our
  // location request, we don't go to the new location - just return 0.
  // This is because we don't yet have full support in MProfiles&friends
  // for profiles used based on priorities.  Once the support is
  // there, we should follow a forwarded object to track it down,
  // just like in the case where ClientPriorityPolicy is not set.
  // At that point, we can remove a lot of 'special case' code from
  // this function, along with this comment ;-) (marina).
  for (;;)
    {
      locate_request.start (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      int status = locate_request.invoke (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      // We'll get this only if the object was, in fact, forwarded.
      if (status == TAO_INVOKE_RESTART)
        if (set)
          return 0;
        else
          continue;

      if (status != TAO_INVOKE_OK)
        {
          ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                            CORBA::COMPLETED_YES),
                            0);

        }
      break;
    }
  return 1;
}

#endif /* TAO_HAS_CORBA_MESSAGING */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class auto_ptr<TAO_Policy_Manager_Impl>;
template class ACE_Auto_Basic_Ptr<TAO_Policy_Manager_Impl>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate auto_ptr<TAO_Policy_Manager_Impl>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Policy_Manager_Impl>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
