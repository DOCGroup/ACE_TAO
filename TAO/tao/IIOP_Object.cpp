// $Id$

// @ (#)iiopobj.cpp     1.9 95/11/04
// Copyright 1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// IIOP Bridge:         CORBA::Object operations
//
// Some CORBA::Object and other operations are specific to this IIOP
// based implementation, and can neither be used by other kinds of
// objref nor have a default implementation.

#include "tao/corba.h"

#if !defined (__ACE_INLINE__)
# include "tao/IIOP_Object.i"
#endif /* ! __ACE_INLINE__ */

#include "tao/Timeprobe.h"

ACE_RCSID(tao, IIOP_Object, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_IIOP_Object_Timeprobe_Description[] =
{
  "IIOP_Object::do_static_call - start",
  "IIOP_Object::do_static_call - end",
  "IIOP_Object::do_static_call - invocation_ctor",
  "IIOP_Object::do_static_call - invocation_start",
  "IIOP_Object::do_static_call - put_params"
};

enum
{
  // Timeprobe description table start key
  TAO_IIOP_OBJECT_DO_STATIC_CALL_START = 500,
  TAO_IIOP_OBJECT_DO_STATIC_CALL_END,
  TAO_IIOP_OBJECT_DO_STATIC_CALL_INVOCATION_CTOR,
  TAO_IIOP_OBJECT_DO_STATIC_CALL_INVOCATION_START,
  TAO_IIOP_OBJECT_DO_STATIC_CALL_PUT_PARAMS
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_IIOP_Object_Timeprobe_Description,
                                  TAO_IIOP_OBJECT_DO_STATIC_CALL_START);

#endif /* ACE_ENABLE_TIMEPROBES */

int
IIOP::Profile::set (const char *h,
                    const CORBA::UShort p,
                    const ACE_INET_Addr *addr)
{
  this->iiop_version.major = IIOP::MY_MAJOR;
  this->iiop_version.minor = IIOP::MY_MINOR;

  if (this->host)
    {
      delete [] this->host;
      this->host = 0;
    }

  this->port = p;

  if (h)
    {
      ACE_NEW_RETURN (this->host,
                      char[ACE_OS::strlen(h) + 1],
                      -1);
      ACE_OS::strcpy (this->host, h);
    }

  this->object_addr (addr);
  return 0;
}

int
IIOP::Profile::set (const char *h,
                    const CORBA::UShort p,
                    const char *key,
                    const ACE_INET_Addr *addr)
{
  if (key == 0)
    return -1;

  if (this->set (h, p, addr) == -1)
    return -1;

  int l = ACE_OS::strlen (key);
  this->object_key.length (l);

  for (int i = 0; i < l; ++i)
    this->object_key[i] = key[i];

  return 0;
}

int
IIOP::Profile::set (const char *h,
                    const CORBA::UShort p,
                    const TAO_opaque &key,
                    const ACE_INET_Addr *addr)
{
  if (this->set (h, p, addr) == -1)
    return -1;

  this->object_key = key;

  return 0;
}

IIOP::Profile::Profile (const IIOP::Profile &src)
  : host (0)
{
  (void) this->set (src.host,
                    src.port,
                    src.object_key,
                    &src.object_addr_);
}

int
IIOP::Profile::set (const ACE_INET_Addr &addr,
                    const char *key)
{
  // Set up an IIOP Profile to hold the host name.

  if (addr.get_ip_address () == INADDR_ANY)
    {
      // Special case.
      char temphost[MAXHOSTNAMELEN + 1];
      if (addr.get_host_name (temphost, sizeof(temphost)) != 0)
        return -1;
      else
        return this->set (temphost,
                          addr.get_port_number (),
                          key,
                          &addr);
    }
  else
    {
      const char *tempaddr = addr.get_host_name ();
      if (tempaddr == 0)
        return -1;
      else
        return this->set (tempaddr,
                          addr.get_port_number (),
                          key,
                          &addr);
    }
}

int
IIOP::Profile::set (const ACE_INET_Addr &addr,
                    const TAO_opaque &key)
{
  // Set up an IIOP Profile to hold the host name.

  if (addr.get_ip_address () == INADDR_ANY)
    {
      // Special case.
      char temphost[MAXHOSTNAMELEN + 1];
      if (addr.get_host_name (temphost, sizeof(temphost)) != 0)
        return -1;
      else
        return this->set (temphost,
                          addr.get_port_number (),
                          key,
                          &addr);
    }
  else
    {
      const char *tempaddr = addr.get_host_name ();
      if (tempaddr == 0)
        return -1;
      else
        return this->set (tempaddr,
                          addr.get_port_number (),
                          key,
                          &addr);
    }
}

IIOP::Profile::Profile (const char *h,
                        const CORBA::UShort p,
                        const char *key)
  : host (0)
{
  (void) this->set (h, p, key);
}

IIOP::Profile::Profile (const char *h,
                        const CORBA::UShort p,
                        const char *key,
                        const ACE_INET_Addr &addr)
  : host (0)
{
  (void) this->set (h, p, key, &addr);
}

IIOP::Profile::Profile (const ACE_INET_Addr &addr,
                        const char *key)
  : host (0)
{
  (void) this->set (addr, key);
}

IIOP::Profile::Profile (const ACE_INET_Addr &addr,
                        const TAO_opaque &key)
  : host (0)
{
  (void) this->set (addr, key);
}


IIOP::Profile
&IIOP::Profile::operator = (const IIOP::Profile &src)
{
  this->set (src.host,
             src.port,
             src.object_key,
             &src.object_addr_);
  return *this;
}



// Quick'n'dirty hash of objref data, for partitioning objrefs into
// sets.
//
// NOTE that this must NOT go across the network!

CORBA::ULong
IIOP_Object::hash (CORBA::ULong max,
                   CORBA::Environment &env)
{
  CORBA::ULong hashval;

  env.clear ();

  // Just grab a bunch of convenient bytes and hash them; could do
  // more (hostname, full key, exponential hashing) but no real need
  // to do so except if performance requires a more costly hash.

  hashval = profile.object_key.length () * profile.port;
  hashval += profile.iiop_version.minor;

  if (profile.object_key.length () >= 4)
    {
      hashval += profile.object_key [1];
      hashval += profile.object_key [3];
    }

  return hashval % max;
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

// Expensive comparison of objref data, to see if two objrefs
// certainly point at the same object. (It's quite OK for this to
// return FALSE, and yet have the two objrefs really point to the same
// object.)
//
// NOTE that this must NOT go across the network!

CORBA::Boolean
IIOP_Object::is_equivalent (CORBA::Object_ptr other_obj,
                            CORBA::Environment &env)
{
  env.clear ();

  if (CORBA::is_nil (other_obj) == CORBA::B_TRUE)
    return CORBA::B_FALSE;

  IIOP_Object *other_iiop_obj =
    ACE_dynamic_cast (IIOP_Object*, other_obj->_stubobj ());
  if (other_iiop_obj == 0)
    return CORBA::B_FALSE;

  // Compare all the bytes of the object address -- must be the same.

  IIOP::Profile *body = &profile;
  IIOP::Profile *body2 = &other_iiop_obj->profile;

  ACE_ASSERT (body->object_key.length () < UINT_MAX);

  return body->object_key == body2->object_key
    && body->port == body2->port
    && ACE_OS::strcmp (body->host, body2->host) == 0
    && body->iiop_version.minor == body2->iiop_version.minor
    && body->iiop_version.major == body2->iiop_version.major;
}

// Memory managment

CORBA::ULong
IIOP_Object::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->refcount_lock_, 0);

  return this->refcount_++;
}

CORBA::ULong
IIOP_Object::_decr_refcnt (void)
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

// TAO extensions
TAO_ObjectKey*
IIOP_Object::key (CORBA::Environment &)
{
  return new TAO_ObjectKey (this->profile.object_key);
}

// Note that if the repository ID (typeID) is NULL, it will make
// narrowing rather expensive, though it does ensure that type-safe
// narrowing code gets thoroughly exercised/debugged!  Without a
// typeID, the _narrow will be required to make an expensive remote
// "is_a" call.

IIOP_Object::IIOP_Object (const char *host,
                          const CORBA::UShort port,
                          const char *objkey,
                          char *repository_id)
  : STUB_Object (repository_id),
    profile (host, port, objkey),
    refcount_ (1)
{
}

// Constructor.  It will usually be used by the server side.
IIOP_Object::IIOP_Object (char *repository_id,
                          const ACE_INET_Addr &addr,
                          const char *objkey)
  : STUB_Object (repository_id),
    profile (addr, objkey),
    fwd_profile_ (0),
    refcount_ (1)
{
  this->fwd_profile_lock_ptr_ =  TAO_ORB_Core_instance ()
                                ->client_factory ()
                                  ->create_iiop_profile_lock ();
}

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
IIOP_Object::do_static_call (CORBA::Environment &env,   // exception reporting
                             const TAO_Call_Data *info, // call description
                             ...)                       // ... any parameters

{
  ACE_FUNCTION_TIMEPROBE (TAO_IIOP_OBJECT_DO_STATIC_CALL_START);

  TAO_Synchronous_Cancellation_Required NOT_USED;

  // Do a locate_request if necessary/wanted.
  if (this->use_locate_request_ && this->first_locate_request_)
  {
    TAO_GIOP_Locate_Request_Invocation call (this);

    call.start (env);
         
    TAO_GIOP_ReplyStatusType  status = call.invoke (env);

    this->first_locate_request_ = CORBA::B_FALSE;

    if (status == TAO_GIOP_SYSTEM_EXCEPTION)
      return;
  }


  if (info->is_roundtrip)
    {
      TAO_GIOP_Twoway_Invocation call (this, info->opname);
      ACE_TIMEPROBE (TAO_IIOP_OBJECT_DO_STATIC_CALL_INVOCATION_CTOR);

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
          call.start (env);

          ACE_TIMEPROBE (TAO_IIOP_OBJECT_DO_STATIC_CALL_INVOCATION_START);
          if (env.exception () != 0) return;

          va_list argp;
          va_start (argp, info);
          this->put_params (env, info, call, argp);
          va_end (argp);
          if (env.exception () != 0) return;

          ACE_TIMEPROBE (TAO_IIOP_OBJECT_DO_STATIC_CALL_PUT_PARAMS);
          TAO_GIOP_ReplyStatusType status =
            call.invoke (info->excepts, info->except_count,env);

          if (status == TAO_GIOP_SYSTEM_EXCEPTION
              || status == TAO_GIOP_USER_EXCEPTION)
            return;
          else if (status == TAO_GIOP_NO_EXCEPTION)
            {
              // Now, get all the "return", "out", and "inout"
              // parameters from the response message body.

              va_start (argp, info);

#if defined (sparc)  &&  \
    defined (__GNUG__)  &&  (__GNUC__ == 2 && __GNUC_MINOR__ < 8)
              // g++ 2.7.x, but not egcs, on sparc.

              // Skip the first arg, due to an apparent bug in g++
              // 2.7.2.3 on sparc.
              va_arg (argp, void *);
#endif /* g++ < 2.8 on sparc */

              const TAO_Param_Data *pdp = info->params;
              for (u_int i = 0;
                   i < info->param_count;
                   i++, pdp++)
                {
                  void *ptr = va_arg (argp, void *);

                  // if it is an inout parameter, it would become
                  // necessary to first release the "in" memory
                  if (pdp->mode == PARAM_INOUT)
                    {
                      // @@ TODO - add others as we test each case
                      // (ASG) will do 03/22/98.
                      // @@ IMHO this should be handled in the stub
                      // (coryan)
                      switch (pdp->tc->kind (env))
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
                        call.get_value (pdp->tc, ptr, env);
                      else
                        {
                          // @@ (ASG) -  I think we must completely
                          // get rid of this case because IDL compiler
                          // generated stubs will use this function
                          // and they better allocate all the memory.

                          // assert (value_size == tc->size());
                          *(void **)ptr = new CORBA::Octet [pdp->value_size];
                          call.get_value (pdp->tc, *(void **)ptr, env);
                        }

                      if (env.exception ())
                        {
                          dexc (env, "do_static_call, get reply parameter");
                          return;
                        }
                    }
                }
              va_end (argp);
              return;
            }
          // ... or maybe this request got forwarded to someplace
          // else; send the request there instead.
          if (status != TAO_GIOP_LOCATION_FORWARD)
            {
              // @@ What is the right exception to throw in this case?
              env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_MAYBE));
              return;
            }
        }
    }
  else
    {
      TAO_GIOP_Oneway_Invocation call (this, info->opname);
      ACE_TIMEPROBE (TAO_IIOP_OBJECT_DO_STATIC_CALL_INVOCATION_CTOR);

      // Start the call by constructing the request message header.
      call.start (env);
      ACE_TIMEPROBE (TAO_IIOP_OBJECT_DO_STATIC_CALL_INVOCATION_START);
      if (env.exception () != 0) return;

      va_list argp;
      va_start (argp, info);
      this->put_params (env, info, call, argp);
      va_end (argp);
      if (env.exception () != 0) return;

      ACE_TIMEPROBE (TAO_IIOP_OBJECT_DO_STATIC_CALL_PUT_PARAMS);
      /* TAO_GIOP_ReplyStatusType status = */ call.invoke (env);

      // @@ TODO We do not get any LOCATION_FORWARD in this case,
      // IMHO this is a good case for use of a LocateRequest,
      // under some strategy control, of course. In that case we need
      // a loop, as above.
    }
}

void
IIOP_Object::put_params (CORBA::Environment &env,
                         const TAO_Call_Data *info,
                         TAO_GIOP_Invocation &call,
                         va_list argp)
{
  if (env.exception ())
    {
      dexc (env, "do_static_call, start request message");
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

#if defined (sparc)  &&  \
    defined (__GNUG__)  &&  (__GNUC__ == 2 && __GNUC_MINOR__ < 8)
  // g++ 2.7.x, but not egcs, on sparc.

  // Skip the first arg, due to an apparent bug in g++ 2.7.2.3 on
  // sparc.
  va_arg (argp, void *);
#endif /* g++ < 2.8 on sparc */

  const TAO_Param_Data *pdp = info->params;
  for (u_int i = 0;
       i < info->param_count;
       i++, pdp++)
    {
      void *ptr = va_arg (argp, void *);

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
          dexc (env, "do_static_call, put request parameter");
          return;
        }
    }
}

// DII analogue of the above.  Differs in how the vararg calling
// convention is implemented -- DII doesn't use the normal call stack
// with its implicit typing, but iinstead uses heap-based arguments
// with explicit typing.

void
IIOP_Object::do_dynamic_call (const char *opname,
                              CORBA::Boolean is_roundtrip,
                              CORBA::NVList_ptr args,
                              CORBA::NamedValue_ptr result,
                              CORBA::Flags flags,
                              CORBA::ExceptionList &exceptions,
                              CORBA::Environment &env)
{
  TAO_Synchronous_Cancellation_Required NOT_USED;

  if (is_roundtrip)
    {
      TAO_GIOP_Twoway_Invocation call (this, opname);

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
              dexc (env, "do_dynamic_call, invoke");
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
                      dexc (env, "do_dynamic_call, get result size");

                      if (size != 0)
                        {
                          void *ptr = new CORBA::Octet [size];

                          result->value ()->replace (tcp.in (), ptr,
                                                     CORBA::B_TRUE, env);
                          dexc (env, "do_dynamic_call, set result mem");
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
                          dexc (env, "do_dynamic_call, get param size");

                          if (size != 0)
                            {
                              CORBA::Octet *ptr = new CORBA::Octet [size];

                              value->value ()->replace (tcp.in (), ptr,
                                                        CORBA::B_TRUE, env);
                              dexc (env, "do_dynamic_call, set result mem");
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
                          dexc (env, "do_dynamic_call, get response parameter");
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
      TAO_GIOP_Oneway_Invocation call (this, opname);
      call.start (env);
      if (env.exception () != 0) return;

      this->put_params (call, args, env);
      if (env.exception () != 0) return;

      (void) call.invoke (env);
    }

}

void
IIOP_Object::put_params (TAO_GIOP_Invocation &call,
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
          // if the Any owns the data, then we already have a CDR encoded
          // data
          if (value->value ()->any_owns_data_)
            {
              TAO_OutputCDR &cdr = call.out_stream ();
              TAO_InputCDR in (value->value ()->cdr_);
              cdr.append (value->value ()->type_, &in, env);
            }
          else
            {
              call.put_param (value->value ()->type_,
                              value->value ()->value_, env);
            }
          if (env.exception ())
            {
              dexc (env, "do_dynamic_call, put request parameter");
              return;
            }
        }
    }
}
