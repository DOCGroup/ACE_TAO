// @(#) $Id$
//
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// BOA initialisation -- both anonymous and (for system bootstrapping) 
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
#include "tao/roa.h"
#endif  /* 0 */

#include "tao/corba.h"

// {A201E4C8-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_BOA,
0xa201e4c8, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98) ;

#if !defined (__ACE_INLINE__) 
#  include "boa.i"
#endif

// A "Named BOA" is used in bootstrapping some part of the ORB since
// it's name-to-address binding is managed by the OS.  Examples of
// such bindings are /etc/services (for TCP) and /etc/rpc (for ONC
// RPC) .  The nam" of a BOA is only guaranteed to be unique within
// the domain of a single system, as a rule; two hosts would have
// distinct "king" BOAs.
//
// For network endpoints, most such names are manually administered.
// Some other namespaces (AF_UNIX filesystem names for example) have a
// more formal underlying name service that can be dynamically updated
// while not compromising sysem security.
//
// The address family used by the BOA is found from the ORB passed in.
//
// XXX the coupling could stand to be looser here, so this module did
// not know specifically about the Internet ORB !!

CORBA::BOA_ptr
CORBA_BOA::get_named_boa (CORBA::ORB_ptr orb,
                          CORBA::String name,
                          CORBA::Environment &env) 
{
  env.clear ();

  // If the ORB is an Internet ORB, we know this must be a TCP OA.
  {
    IIOP_ORB *internet;

    if (orb->QueryInterface (IID_IIOP_ORB, (void **) &internet) == NOERROR) 
      {
        ROA* tcp_oa;

        internet->Release ();

        // ROA initialization with name specified; it'll come from
        // /etc/services if it's not a port number.

        ACE_INET_Addr boa_name (name, (ACE_UINT32) INADDR_ANY);

        tcp_oa = ROA::init (orb, boa_name, env);

        if (env.exception () != 0) 
          return 0;
        else
          return tcp_oa;		// derives from BOA
      }
  }

  // We don't know how to deal with this kind of ORB.  Report error.

  env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO) );
  return 0;
}

// An "Anonymous" BOA is used more routinely.  The name used doesn't
// matter to anyone; it is only used to create object references with
// a short lifespan, namely that of the process acquiring this BOA.

CORBA::BOA_ptr
CORBA_BOA::get_boa (CORBA::ORB_ptr orb,
		    CORBA::Environment &env) 
{
  env.clear ();

  // If the ORB is an Internet ORB, we know this must be a TCP OA.
  {
    IIOP_ORB	*internet;

    if (orb->QueryInterface (IID_IIOP_ORB, (void **) &internet) == NOERROR) 
      {
        ROA* tcp_oa;

        internet->Release ();

        // ROA initialization with null name means anonymous OA

        ACE_INET_Addr anonymous ((u_short) 0, (ACE_UINT32) INADDR_ANY);

        tcp_oa = ROA::init (orb, anonymous, env);

        if (env.exception () != 0) 
          return 0;
        else
          return tcp_oa;		// derives from BOA
      }
  }

  // We don't know how to deal with this kind of ORB.  Report error.

  env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO) );
  return 0;
}

void CORBA_BOA::dispatch (CORBA::OctetSeq &key,
			  CORBA::ServerRequest &req,
			  void *context,
			  CORBA::Environment &env) 
{
  TAO_Skeleton skel;  // pointer to function pointer for the operation
  CORBA::Object_ptr obj;  // object that will be looked up based on the key
  CORBA::String  opname;

  // Get the skeleton

  // @@ Please add more comments here.  This is a very important part
  // of the code.

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
CORBA_BOA::find (const CORBA::OctetSeq &key,
		 CORBA::Object_ptr &obj)
{
  return objtable_->find (key, obj);
}

int
CORBA_BOA::bind (const CORBA::OctetSeq &key, 
		 CORBA::Object_ptr obj)
{
  return objtable_->bind (key, obj);
}

