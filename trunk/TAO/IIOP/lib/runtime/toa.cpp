// @(#)toa.cpp	1.3 95/09/29
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// TOA initialisation -- both anonymous and (for system bootstrapping)
// named TOAs.
//
// XXX at this time, there's a strong linkage between this code and
// the modules knowing about IIOP.  In the future, a looser coupling
// between OA initialiszation and protocol components is desired.
//

#include	<assert.h>
#include	<stdio.h>
#include	<string.h>

#if	unix
#include	<unistd.h>
#include	<netdb.h>

#else
#include	<winsock.h>

#endif

#include	<corba/orb.hh>
#include	<corba/toa.hh>

#include	"runtime/thread.hh"
#include	"runtime/debug.hh"

// XXX this should not know implementation or other details of any
// protocol modules!  This is an implementation shortcut only.

#include	"bridge/iioporb.hh"
#include	"bridge/connmgr.hh"
#include	"bridge/tcpoa.hh"

#include	<initguid.h>


// {A201E4C8-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_TOA,
0xa201e4c8, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);


//
// A "Named TOA" is used in bootstrapping some part of the ORB since
// it's name-to-address binding is managed by the OS.  Examples of such
// bindings are /etc/services (for TCP) and /etc/rpc (for ONC RPC).  The
// nam" of a TOA is only guaranteed to be unique within the domain of a
// single system, as a rule; two hosts would have distinct "king" TOAs.
//
// For network endpoints, most such names are manually administered.
// Some other namespaces (AF_UNIX filesystem names for example) have a
// more formal underlying name service that can be dynamically updated
// while not compromising sysem security.
//
// The address family used by the TOA is found from the ORB passed in.
//
// XXX the coupling could stand to be looser here, so this module did
// not know specifically about the Internet ORB !!
//
TOA_ptr
TOA::get_named_toa (
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
	    TCP_OA	*tcp_oa;

	    internet->Release ();

	    //
	    // TCP_OA initialization with name specified; it'll
	    // come from /etc/services if it's not a port number.
	    //
	    tcp_oa = TCP_OA::init (orb, name, env);
	    if (env.exception () != 0)
		return 0;
	    else
		return tcp_oa;		// derives from TOA
	}
    }

    //
    // We don't know how to deal with this kind of ORB.  Report error.
    //
    env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
    return 0;
}


//
// An "Anonymous" TOA is used more routinely.  The name used doesn't
// matter to anyone; it is only used to create object references with
// a short lifespan, namely that of the process acquiring this TOA.
//
TOA_ptr
TOA::get_toa (
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
	    TCP_OA	*tcp_oa;

	    internet->Release ();

	    //
	    // TCP_OA initialization with null name means anonymous OA
	    //
	    tcp_oa = TCP_OA::init (orb, 0, env);
	    if (env.exception () != 0)
		return 0;
	    else
		return tcp_oa;		// derives from TOA
	}
    }

    //
    // We don't know how to deal with this kind of ORB.  Report error.
    //
    env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
    return 0;
}
