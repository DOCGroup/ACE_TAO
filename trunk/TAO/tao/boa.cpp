// @(#) $Id$
//
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// POA initialisation -- both anonymous and (for system bootstrapping) 
// named BOAs.
//
// XXX at this time, there's a strong linkage between this code and
// the modules knowing about IIOP.  In the future, a looser coupling
// between OA initialiszation and protocol components is desired.

#if 0
#include "ace/OS.h"    // WARNING! This MUST come before objbase.h on WIN32!
#include <objbase.h>
#include <initguid.h>

#include "tao/orb.h"
#include "tao/boa.h"

// XXX this should not know implementation or other details of any
// protocol modules!  This is an implementation shortcut only.

#include "tao/iioporb.h"
#endif  /* 0 */

#include "tao/corba.h"

// {A201E4C8-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_BOA,
0xa201e4c8, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98) ;

#if !defined (__ACE_INLINE__) 
#  include "boa.i"
#endif

// CORBA_POA::init() is used in get_boa() and get_named_boa() in order
// to initialize the OA.  It was originally part of ROA, and may no
// longer be useful.
CORBA::POA_ptr
CORBA_POA::init (CORBA::ORB_ptr parent,
                 ACE_INET_Addr &,
                 CORBA::Environment &env)
{
  env.clear ();
  TAO_ORB_Core *p = TAO_ORB_Core_instance ();

  if (p->root_poa ())
    {
      env.exception (new CORBA_INITIALIZE (CORBA::COMPLETED_NO));
      return 0;
    }

  CORBA::POA_ptr rp;
  ACE_NEW_RETURN (rp, CORBA_POA (parent, env), 0);
  p->root_poa (rp);

  return rp;
}

CORBA_POA::CORBA_POA (CORBA::ORB_ptr owning_orb,
                      CORBA::Environment &)
  : do_exit_ (CORBA::B_FALSE), 
    orb_ (owning_orb),
    call_count_ (0),
    skeleton_ (0)
{
  TAO_Server_Strategy_Factory *f = orb_->server_factory ();
  TAO_ORB_Core* p = TAO_ORB_Core_instance ();
  
  this->objtable_ = f->create_object_table ();

  // @@ What is this doing here?  Why is it setting the root poa based
  // on whether objtable_ is non-zero?  (cjc)
  if (this->objtable_ != 0)
    p->root_poa (this);
}

CORBA_POA::~CORBA_POA (void)
{
}

// Create an objref

CORBA::Object_ptr
CORBA_POA::create (CORBA::OctetSeq &key,
             CORBA::String type_id,
             CORBA::Environment &env)
{
  CORBA::String id;
  IIOP_Object *data;

  if (type_id)
    id = CORBA::string_copy (type_id);
  else
    id = 0;

  IIOP::Version ver (IIOP::MY_MAJOR, IIOP::MY_MINOR);
  // Cast below de-warns on Sun's C++
  const ACE_INET_Addr& addr = orb_->params ()->addr ();
  CORBA::String h = (char*)addr.get_host_name ();

  data = new IIOP_Object (id, IIOP::ProfileBody (ver,
						 h,
						 addr.get_port_number (),
						 key));
  if (data != 0)
    env.clear ();
  else
    {
      env.exception (new CORBA_NO_MEMORY (CORBA::COMPLETED_NO));
      return 0;
    }

  // Return the CORBA::Object_ptr interface to this objref.
  CORBA::Object_ptr new_obj;

  if (data->QueryInterface (IID_CORBA_Object,
			    (void**)&new_obj) != NOERROR)
    env.exception (new CORBA::INTERNAL (CORBA::COMPLETED_NO));

  data->Release ();
  return new_obj;
}

// Return the key fed into an object at creation time.

CORBA::OctetSeq *
CORBA_POA::get_key (CORBA::Object_ptr,
                    CORBA::Environment &env)
{
  // XXX implement me ! ... must have been created by this OA.
  env.exception (new CORBA_IMP_LIMIT (CORBA::COMPLETED_NO));
  return 0;
}

// Used by method code to ask the OA to shut down.
void
CORBA_POA::please_shutdown (CORBA::Environment &env)
{
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, boa_mon, lock_));

  env.clear ();
  do_exit_ = CORBA::B_TRUE;
}

// Used by non-method code to tell the OA to shut down.
void
CORBA_POA::clean_shutdown (CORBA::Environment &env)
{
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, boa_mon, lock_));

  env.clear ();

  if (call_count_ != 0)
    {
      dmsg ("called clean_shutdown with requests outstanding");
      env.exception (new CORBA::BAD_INV_ORDER (CORBA::COMPLETED_NO));
      return;
    }

  // Here we need to tell all the endpoints to shut down...
}

// For POA -- POA operations for which we provide the vtable entry
void
CORBA_POA::register_dir (dsi_handler handler,
                         void *ctx,
                         CORBA::Environment &env)
{
  if (handler == 0)
    {
      env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
      return;
    }

  skeleton_ = handler;
  context_ = ctx;

  env.clear ();
}

// A "Named POA" is used in bootstrapping some part of the ORB since
// it's name-to-address binding is managed by the OS.  Examples of
// such bindings are /etc/services (for TCP) and /etc/rpc (for ONC
// RPC) .  The name of a POA is only guaranteed to be unique within
// the domain of a single system, as a rule; two hosts would have
// distinct "king" BOAs.
//
// For network endpoints, most such names are manually administered.
// Some other namespaces (AF_UNIX filesystem names for example) have a
// more formal underlying name service that can be dynamically updated
// while not compromising system security.
//
// The address family used by the POA is found from the ORB passed in.
//
// XXX the coupling could stand to be looser here, so this module did
// not know specifically about the Internet ORB !!

CORBA::POA_ptr
CORBA_POA::get_named_boa (CORBA::ORB_ptr orb,
                          CORBA::String name,
                          CORBA::Environment &env) 
{
  env.clear ();

  // If the ORB is an Internet ORB, we know this must be a TCP OA.
  {
    IIOP_ORB *internet;

    if (orb->QueryInterface (IID_IIOP_ORB, (void **) &internet) == NOERROR) 
      {
        CORBA::POA_ptr tcp_oa;

        internet->Release ();

        // POA initialization with name specified; it'll come from
        // /etc/services if it's not a port number.

        ACE_INET_Addr boa_name (name, (ACE_UINT32) INADDR_ANY);

        tcp_oa = CORBA::POA::init (orb, boa_name, env);

        if (env.exception () != 0) 
          return 0;
        else
          return tcp_oa;		// derives from POA
      }
  }

  // We don't know how to deal with this kind of ORB.  Report error.

  env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO) );
  return 0;
}

// An "Anonymous" POA is used more routinely.  The name used doesn't
// matter to anyone; it is only used to create object references with
// a short lifespan, namely that of the process acquiring this POA.

CORBA::POA_ptr
CORBA_POA::get_boa (CORBA::ORB_ptr orb,
		    CORBA::Environment &env) 
{
  env.clear ();

  // If the ORB is an Internet ORB, we know this must be a TCP OA.
  {
    IIOP_ORB	*internet;

    if (orb->QueryInterface (IID_IIOP_ORB, (void **) &internet) == NOERROR) 
      {
        CORBA::POA_ptr tcp_oa;

        internet->Release ();

        // ROA initialization with null name means anonymous OA

        ACE_INET_Addr anonymous ((u_short) 0, (ACE_UINT32) INADDR_ANY);

        tcp_oa = CORBA::POA::init (orb, anonymous, env);

        if (env.exception () != 0) 
          return 0;
        else
          return tcp_oa;		// derives from POA
      }
  }

  // We don't know how to deal with this kind of ORB.  Report error.

  env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO) );
  return 0;
}

void CORBA_POA::dispatch (CORBA::OctetSeq &key,
			  CORBA::ServerRequest &req,
			  void *context,
			  CORBA::Environment &env) 
{
  ACE_UNUSED_ARG(context);
  
  TAO_Skeleton skel;  // pointer to function pointer for the operation
  CORBA::Object_ptr obj;  // object that will be looked up based on the key
  CORBA::String  opname;

  // Get the skeleton

  // Find the object based on the key
  if (this->find (key, obj) != -1) 
    {
      opname = req.op_name ();

      // Find the skeleton "glue" function based on the operation name
      if (obj->find (opname, skel) != -1)
        // Schedule the upcall.  This is the degenerate case of scheduling...
        // using a "do it now!" scheduler
	skel (req, obj, env);
      else
	{
	  // this may fail in which case, we must try out the default
	  // operations such as "_is_a", "non_existent", "hash",
	  // ... At this time, we try the "is_a" method
	}
    }

  // We need to pass this skel and associated information to the
  // scheduler. How do we do it??
}

int
CORBA_POA::find (const CORBA::OctetSeq &key,
		 CORBA::Object_ptr &obj)
{
  return objtable_->find (key, obj);
}

int
CORBA_POA::bind (const CORBA::OctetSeq &key, 
		 CORBA::Object_ptr obj)
{
  return objtable_->bind (key, obj);
}

void
CORBA_POA::handle_request (TAO_GIOP_RequestHeader hdr,
                           CDR &request_body,
                           CDR &response,
                           TAO_Dispatch_Context *some_info,
                           CORBA::Environment &env)
{
  ACE_UNUSED_ARG (some_info);
  
  IIOP_ServerRequest svr_req (&request_body, this->orb (), this);

  svr_req.opname_ = hdr.operation; // why are we copying this when we can just pass in
                                   // a handle to the hdr?

  this->dispatch (hdr.object_key, svr_req, 0 /* this is IIOP residue */, env);

  // FIXME! I don't think this should happen yet!
  svr_req.release ();

  // If no reply is necessary (i.e., oneway), then return!
  if (! hdr.response_expected)
    return;
  
  // Otherwise check for correct parameter handling, and reply as
  // appropriate.
  //
  // NOTE: if "env" is set, it takes precedence over exceptions
  // reported using the mechanism of the ServerRequest.  Only system
  // exceptions are reported that way ...
  //
  // XXX Exception reporting is ambiguous; it can be cleaner than
  // this.  With both language-mapped and dynamic/explicit reporting
  // mechanisms, one of must be tested "first" ... so an exception
  // reported using the other mechanism could be "lost".  Perhaps only
  // the language mapped one should be used for system exceptions.

  TAO_GIOP::start_message (TAO_GIOP_Reply, response);
  TAO_GIOP_ServiceContextList resp_ctx;
  resp_ctx.length = 0;
  response.encode (&TC_ServiceContextList, &resp_ctx, 0, env);
  response.put_ulong (hdr.request_id);
  
  CORBA::TypeCode_ptr tc;
  const void *value;

  if (!svr_req.params_ && env.exception () == 0) 
    {
      dmsg ("DSI user error, didn't supply params");
      env.exception (new CORBA::BAD_INV_ORDER (CORBA::COMPLETED_NO));
    }

  if (env.exception () != 0) 
    {	// standard exceptions only
      CORBA::Environment env2;
      CORBA::Exception *x = env.exception ();
      CORBA::TypeCode_ptr except_tc = x->type ();

      response.put_ulong (TAO_GIOP_SYSTEM_EXCEPTION);
      (void) response.encode (except_tc, x, 0, env2);
    }
  else if (svr_req.exception_)
    {	// any exception at all
      CORBA::Exception *x;
      CORBA::TypeCode_ptr except_tc;

      x = (CORBA::Exception *) svr_req.exception_->value ();
      except_tc = svr_req.exception_->type ();

      // Finish the GIOP Reply header, then marshal the exception.
      //
      // XXX x->type () someday ...
      if (svr_req.ex_type_ == CORBA::SYSTEM_EXCEPTION)
	response.put_ulong (TAO_GIOP_SYSTEM_EXCEPTION);
      else
	response.put_ulong (TAO_GIOP_USER_EXCEPTION);

      (void) response.encode (except_tc, x, 0, env);
    }
  else
    {				// normal reply
      // First finish the GIOP header ...
      response.put_ulong (TAO_GIOP_NO_EXCEPTION);

      // ... then send any return value ...
      if (svr_req.retval_)
	{
	  tc = svr_req.retval_->type ();
	  value = svr_req.retval_->value ();
	  (void) response.encode (tc, value, 0, env);
	}

      // ... followed by "inout" and "out" parameters, left to right
      for (u_int i = 0; i < svr_req.params_->count (); i++)
	{
	  CORBA::NamedValue_ptr	nv = svr_req.params_->item (i);
	  CORBA::Any_ptr any;

	  if (!(nv->flags () & (CORBA::ARG_INOUT|CORBA::ARG_OUT)))
	    continue;

	  any = nv->value ();
	  tc = any->type ();
	  value = any->value ();
	  (void) response.encode (tc, value, 0, env);
	}
    }
}

// IUnknown calls
ULONG __stdcall
CORBA_POA::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, boa_mon, com_lock_, 0));
  return ++refcount_;
}

ULONG __stdcall
CORBA_POA::Release (void)
{
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, boa_mon, com_lock_, 0));

    if (--refcount_ != 0)
      return refcount_;
  }

  delete this;
  return 0;
}

HRESULT __stdcall
CORBA_POA::QueryInterface (REFIID riid,
                           void **ppv)
{
  *ppv = 0;

  if (IID_BOA == riid
      || IID_IUnknown == riid)
    *ppv = this;

  if (*ppv == 0)
    return ResultFromScode (E_NOINTERFACE);

 (void) AddRef ();
  return NOERROR;
}
