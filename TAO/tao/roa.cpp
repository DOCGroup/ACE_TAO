//     $Id$

#if 0
#include "ace/OS.h"    // WARNING! This MUST come before objbase.h on WIN32!
#include <objbase.h>
#include <initguid.h>

#include "tao/orb.h"
#include "tao/roa.h"
#include "tao/boa.h"
#include "tao/cdr.h"
#include "tao/connect.h"
#include "tao/giop.h"
#include "tao/params.h"
#include "tao/orbobj.h"
#include "tao/nvlist.h"
#include "tao/debug.h"
#include "tao/Orb_Core.h"
#endif

#include "tao/corba.h"

ROA_ptr
ROA::init (CORBA::ORB_ptr parent,
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

  ROA_ptr rp;
  ACE_NEW_RETURN (rp, ROA (parent, env), 0);
  p->root_poa (rp);

  return rp;
}

ROA::ROA (CORBA::ORB_ptr owning_orb,
	  CORBA::Environment &)
  : do_exit_ (CORBA::B_FALSE), 
    orb_ (owning_orb),
    call_count_ (0),
    skeleton_ (0)
{
  TAO_Server_Strategy_Factory *f = orb_->server_factory ();
  TAO_ORB_Core* p = TAO_ORB_Core_instance ();
  
  this->objtable_ = f->create_object_table ();

  if (this->objtable_ != 0)
    p->root_poa (this);
}

ROA::~ROA (void)
{
}

// Create an objref

CORBA::Object_ptr
ROA::create (CORBA::OctetSeq &key,
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
ROA::get_key (CORBA::Object_ptr,
	      CORBA::Environment &env)
{
  // XXX implement me ! ... must have been created by this OA.
  env.exception (new CORBA_IMP_LIMIT (CORBA::COMPLETED_NO));
  return 0;
}

// Used by method code to ask the OA to shut down.
void
ROA::please_shutdown (CORBA::Environment &env)
{
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, roa_mon, lock_));

  env.clear ();
  do_exit_ = CORBA::B_TRUE;
}

// Used by non-method code to tell the OA to shut down.
void
ROA::clean_shutdown (CORBA::Environment &env)
{
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, roa_mon, lock_));

  env.clear ();

  if (call_count_ != 0)
    {
      dmsg ("called clean_shutdown with requests outstanding");
      env.exception (new CORBA::BAD_INV_ORDER (CORBA::COMPLETED_NO));
      return;
    }

  // Here we need to tell all the endpoints to shut down...
}

// For BOA -- BOA operations for which we provide the vtable entry

void
ROA::register_dir (CORBA::BOA::dsi_handler handler,
		   void *ctx, CORBA::Environment &env)
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


// For COM -- IUnknown operations, we provide the vtable entry

// {A201E4C4-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_ROA,
0xa201e4c4, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

ULONG __stdcall
ROA::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, roa_mon, com_lock_, 0));
  return ++refcount_;
}

ULONG __stdcall
ROA::Release (void)
{
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, roa_mon, com_lock_, 0));

    if (--refcount_ != 0)
      return refcount_;
  }

  delete this;
  return 0;
}

HRESULT __stdcall
ROA::QueryInterface (REFIID riid,
		     void **ppv)
{
  *ppv = 0;

  if (IID_ROA == riid
      || IID_BOA == riid
      || IID_IUnknown == riid)
    *ppv = this;

  if (*ppv == 0)
    return ResultFromScode (E_NOINTERFACE);

 (void) AddRef ();
  return NOERROR;
}

#if !defined (__ACE_INLINE__)
#  include "roa.i"
#endif /* __ACE_INLINE__ */
