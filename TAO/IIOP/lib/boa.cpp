// @(#)boa.cpp	1.3 95/09/29
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// BOA initialisation -- both anonymous and (for system bootstrapping)
// named BOAs.
//
// XXX at this time, there's a strong linkage between this code and
// the modules knowing about IIOP.  In the future, a looser coupling
// between OA initialiszation and protocol components is desired.
//

#include	<assert.h>
#include	<stdio.h>
#include	<string.h>

#include	<orb.hh>
#include	<boa.hh>

#include	"thread.hh"
#include	"debug.hh"

// XXX this should not know implementation or other details of any
// protocol modules!  This is an implementation shortcut only.

#include	"iioporb.hh"
#include	"roa.hh"

#include	<initguid.h>


// {A201E4C8-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_BOA,
0xa201e4c8, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);


//
// A "Named BOA" is used in bootstrapping some part of the ORB since
// it's name-to-address binding is managed by the OS.  Examples of such
// bindings are /etc/services (for TCP) and /etc/rpc (for ONC RPC).  The
// nam" of a BOA is only guaranteed to be unique within the domain of a
// single system, as a rule; two hosts would have distinct "king" BOAs.
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
//
BOA_ptr
BOA::get_named_boa (
    CORBA_ORB_ptr	orb,
    CORBA_String	name,
    CORBA_Environment	&env
)
{
    env.clear ();

    //
    // If the ORB is an Internet ORB, we know this must be a TCP OA.
    //
    {
	IIOP_ORB	*internet;

	if (orb->QueryInterface (IID_IIOP_ORB, (void **)&internet)
		== NOERROR) {
	    ROA* tcp_oa;

	    internet->Release ();

	    //
	    // ROA initialization with name specified; it'll
	    // come from /etc/services if it's not a port number.
	    //
	    ACE_INET_Addr boa_name(name, INADDR_ANY);
	    tcp_oa = ROA::init (orb, boa_name, env);
	    if (env.exception () != 0)
		return 0;
	    else
		return tcp_oa;		// derives from BOA
	}
    }

    //
    // We don't know how to deal with this kind of ORB.  Report error.
    //
    env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
    return 0;
}


//
// An "Anonymous" BOA is used more routinely.  The name used doesn't
// matter to anyone; it is only used to create object references with
// a short lifespan, namely that of the process acquiring this BOA.
//
BOA_ptr
BOA::get_boa (
    CORBA_ORB_ptr	orb,
    CORBA_Environment	&env
)
{
    env.clear ();

    //
    // If the ORB is an Internet ORB, we know this must be a TCP OA.
    //
    {
	IIOP_ORB	*internet;

	if (orb->QueryInterface (IID_IIOP_ORB, (void **)&internet)
		== NOERROR) {
	    ROA* tcp_oa;

	    internet->Release ();

	    //
	    // ROA initialization with null name means anonymous OA
	    //
	    ACE_INET_Addr anonymous((unsigned short)0, INADDR_ANY);
	    tcp_oa = ROA::init (orb, anonymous, env);
	    if (env.exception () != 0)
		return 0;
	    else
		return tcp_oa;		// derives from BOA
	}
    }

    //
    // We don't know how to deal with this kind of ORB.  Report error.
    //
    env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
    return 0;
}
