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

#include	<assert.h>
#include	<stdio.h>
#include	<string.h>

#include	<orb.h>
#include	<boa.h>

#include	"thread.h"
#include	"debug.h"

// XXX this should not know implementation or other details of any
// protocol modules!  This is an implementation shortcut only.

#include	"iioporb.h"
#include	"roa.h"

#include	<initguid.h>


// {A201E4C8-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_BOA,
0xa201e4c8, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98) ;

// destructor
TAO_Object_Table::~TAO_Object_Table (void)
{
}

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

CORBA_BOA_ptr
CORBA_BOA::get_named_boa (CORBA_ORB_ptr orb,
                          CORBA_String name,
                          CORBA_Environment &env) 
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

        ACE_INET_Addr boa_name (name, INADDR_ANY);

        tcp_oa = ROA::init (orb, boa_name, env);

        if (env.exception () != 0) 
          return 0;
        else
          return tcp_oa;		// derives from BOA
      }
  }

  // We don't know how to deal with this kind of ORB.  Report error.

  env.exception (new CORBA_BAD_PARAM (COMPLETED_NO) );
  return 0;
}

// An "Anonymous" BOA is used more routinely.  The name used doesn't
// matter to anyone; it is only used to create object references with
// a short lifespan, namely that of the process acquiring this BOA.

CORBA_BOA_ptr
CORBA_BOA::get_boa (CORBA_ORB_ptr orb,
		    CORBA_Environment &env) 
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

        ACE_INET_Addr anonymous ((unsigned short) 0, INADDR_ANY);

        tcp_oa = ROA::init (orb, anonymous, env);

        if (env.exception () != 0) 
          return 0;
        else
          return tcp_oa;		// derives from BOA
      }
  }

  // We don't know how to deal with this kind of ORB.  Report error.

  env.exception (new CORBA_BAD_PARAM (COMPLETED_NO) );
  return 0;
}

void CORBA_BOA::dispatch (CORBA_OctetSeq &key,
			  CORBA_ServerRequest &req,
			  void *context,
			  CORBA_Environment &env) 
{
  TAO_Skeleton skel;  // pointer to function pointer for the operation
  CORBA_Object_ptr obj;  // object that will be looked up based on the key
  CORBA_OctetSeq *obj_key;
  CORBA_String  opname;

#if 0
  // this whole thing doesn't seem to be right since the context will
  // be the key of just one object
  obj_key = (CORBA_OctetSeq *) context;

  if (obj_key->length != key.length
      || ACE_OS::memcmp (obj_key->buffer, key.buffer,
			 obj_key->length) != 0) {
    env.exception (new CORBA_OBJECT_NOT_EXIST (COMPLETED_NO) );
#ifdef	DEBUG
    if (debug_level) 
      dmsg_opaque ("request to nonexistent object, key = ",
		   key.buffer, key.length);
#endif
    return;
  }
#endif /* 0 */

  // Get the skeleton

  // @@ Please add more comments here.  This is a very important part
  // of the code.

  if (this->find (key, obj) != -1) 
    {
      opname = req.op_name ();

      if (obj->find (opname, skel) != -1) 
	skel (req, obj, env); // really should be scheduled .
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

#if !defined (__ACE_INLINE__) 
#  include "boa.i"
#endif
