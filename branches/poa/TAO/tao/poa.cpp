// @(#) $Id$
//
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// POA initialisation -- both anonymous and (for system bootstrapping)
// named POAs.
//
// XXX at this time, there's a strong linkage between this code and
// the modules knowing about IIOP.  In the future, a looser coupling
// between OA initialiszation and protocol components is desired.

#include "tao/corba.h"

// {A201E4C8-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_POA,
0xa201e4c8, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98) ;

// CORBA_POA::init() is used in get_poa() and get_named_poa() in order
// to initialize the OA.  It was originally part of POA, and may no
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
  TAO_ORB_Core* p = TAO_ORB_Core_instance ();
  TAO_Server_Strategy_Factory *f = p->server_factory ();

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



  data = new IIOP_Object (id,
                          IIOP::Profile (TAO_ORB_Core_instance ()->orb_params ()->addr (),
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
			    (void **) &new_obj) != TAO_NOERROR)
    env.exception (new CORBA::INTERNAL (CORBA::COMPLETED_NO));

  data->Release ();
  return new_obj;
}

#if 0
// Used by method code to ask the OA to shut down.
void
CORBA_POA::please_shutdown (CORBA::Environment &env)
{
  ACE_MT (ACE_GUARD (ACE_SYNCH_MUTEX, poa_mon, lock_));

  env.clear ();
  do_exit_ = CORBA::B_TRUE;
}

// Used by non-method code to tell the OA to shut down.
void
CORBA_POA::clean_shutdown (CORBA::Environment &env)
{
  ACE_MT (ACE_GUARD (ACE_SYNCH_MUTEX, poa_mon, lock_));

  env.clear ();

  if (call_count_ != 0)
    {
      dmsg ("called clean_shutdown with requests outstanding");
      env.exception (new CORBA::BAD_INV_ORDER (CORBA::COMPLETED_NO));
      return;
    }

  // Here we need to tell all the endpoints to shut down...
}
#endif /* 0 */

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
// distinct "king" POAs.
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
CORBA_POA::get_named_poa (CORBA::ORB_ptr orb,
                          CORBA::String name,
                          CORBA::Environment &env)
{
  env.clear ();

  // If the ORB is an Internet ORB, we know this must be a TCP OA.
  {
    IIOP_ORB *internet;

    if (orb->QueryInterface (IID_IIOP_ORB, (void **) &internet) == TAO_NOERROR)
      {
        CORBA::POA_ptr tcp_oa;

        internet->Release ();

        // POA initialization with name specified; it'll come from
        // /etc/services if it's not a port number.

        ACE_INET_Addr poa_name (name, (ACE_UINT32) INADDR_ANY);

        tcp_oa = CORBA::POA::init (orb, poa_name, env);

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
CORBA_POA::get_poa (CORBA::ORB_ptr orb,
		    CORBA::Environment &env)
{
  env.clear ();

  // If the ORB is an Internet ORB, we know this must be a TCP OA.
  {
    IIOP_ORB	*internet;

    if (orb->QueryInterface (IID_IIOP_ORB, (void **) &internet) == TAO_NOERROR)
      {
        CORBA::POA_ptr tcp_oa;

        internet->Release ();

        // POA initialization with null name means anonymous OA

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

void
CORBA_POA::dispatch (CORBA::OctetSeq &key,
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
#if 0 // XXXASG - testing a new way of handling requests to overcome the
      // casting problem arising out of virtual inheritance
      opname = req.op_name ();

      // Find the skeleton "glue" function based on the operation name
      if (obj->find (opname, skel) != -1)
        // Schedule the upcall.  This is the degenerate case of scheduling...
        // using a "do it now!" scheduler
        skel (req, obj, env);
      else
        {
          // Something really bad happened: the operation was not
          // found in the object, fortunately there is a standard
          // exception for that purpose.
          env.exception (new CORBA_BAD_OPERATION (CORBA::COMPLETED_NO));
          ACE_ERROR ((LM_ERROR,
                      "Cannot find operation <%s> in object\n",
                      opname));
        }
#else
      ACE_UNUSED_ARG (opname);
      ACE_UNUSED_ARG (skel);
#endif
      obj->dispatch (req, context, env);
    }
  else
    {
      env.exception (new CORBA::OBJECT_NOT_EXIST (CORBA::COMPLETED_NO));
      ACE_ERROR ((LM_ERROR, "Cannot find object\n"));

    }

  // @@ XXXASG -
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


// IUnknown calls
ULONG
CORBA_POA::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, poa_mon, com_lock_, 0));
  return ++refcount_;
}

ULONG
CORBA_POA::Release (void)
{
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, poa_mon, com_lock_, 0));

    if (--refcount_ != 0)
      return refcount_;
  }

  delete this;
  return 0;
}

TAO_HRESULT
CORBA_POA::QueryInterface (REFIID riid,
                           void **ppv)
{
  *ppv = 0;

  if (IID_POA == riid
      || IID_TAO_IUnknown == riid)
    *ppv = this;

  if (*ppv == 0)
    return ResultFromScode (TAO_E_NOINTERFACE);

 (void) AddRef ();
  return TAO_NOERROR;
}
