// @(#)orbobj.cpp	1.8 95/09/24
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:		CORBA::ORB operations
//
// XXX as with TOA, this has a strong coupling to the Internet ORB (IIOP)
// code.  We should make it know less about that protocol component and
// have a loose table-driven coupling to ORB/protocol library components.
//

#include	<assert.h>
#include	<limits.h>
#include	<signal.h>
#include	<string.h>

#include	<corba/orb.hh>
#include	<corba/stub.hh>

#include	"runtime/debug.hh"
#include	"runtime/thread.hh"

#include	"bridge/iioporb.hh"		// XXX

#include	<initguid.h>


extern void __TC_init_table ();
extern void __TC_init_standard_exceptions (CORBA_Environment &env);

#ifdef	_POSIX_THREADS
static pthread_mutex_t		refcnt_lock = PTHREAD_MUTEX_INITIALIZER;
#endif

#if defined (SIG_IGN_BROKEN)
#	undef	SIG_IGN 
#	define	SIG_IGN		((RETSIGTYPE (*)(int))1)
#endif	// NeXT


//
// Constructor and destructor are accessible to subclasses
//
CORBA_ORB::CORBA_ORB ()
{
    _refcount = 1;
}

CORBA_ORB::~CORBA_ORB ()
{
    assert (_refcount == 0);
}

//
// CORBA dup/release build on top of COM's (why not).
//
void
CORBA_release (
    CORBA_ORB_ptr	obj
)
{
    if (obj)
	obj->Release ();
}

CORBA_ORB_ptr
CORBA_ORB::_duplicate (CORBA_ORB_ptr obj)
{
    if (obj)
	obj->AddRef ();
    return obj;
}

//
// Null pointers represent nil objects.
//
CORBA_ORB_ptr
CORBA_ORB::_nil ()
{
    return 0;
}

CORBA_Boolean
CORBA_is_nil (CORBA_ORB_ptr	obj)
{
    return (CORBA_Boolean) (obj == 0);
}


//
// COM's IUnknown support
//

// {A201E4C6-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_ORB,
0xa201e4c6, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

// {A201E4C7-F258-11ce-9598-0000C07CA898}
// Modified by BRM: 2/21/97
// Code move to IIOP_Object CPP file

//DEFINE_GUID (IID_STUB_Object,
//0xa201e4c7, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);


ULONG
__stdcall
CORBA_ORB::AddRef ()
{
#ifdef	_POSIX_THREADS
    Critical		region (&refcnt_lock);
#endif

    return _refcount++;
}

ULONG
__stdcall
CORBA_ORB::Release ()
{
#ifdef	_POSIX_THREADS
    Critical		region (&refcnt_lock);
#endif

    assert (this != 0);

    if (--_refcount != 0)
	return _refcount;

#ifdef	_POSIX_THREADS
    region.leave ();
#endif

    delete this;
    return 0;
}


//
// ORB initialisation, per OMG document 94-9-46.
//
// XXX in addition to the "built in" Internet ORB, there will be ORBs which
// are added separately, e.g. through a DLL listed in the registry.  Registry
// will be used to assign orb names and to establish which is the default.
//
static CORBA_ORB_ptr	the_orb;

CORBA_ORB_ptr
CORBA_ORB_init (
    int			&,		// argc
    char		*const *,	// argv
    char		*orb_name,
    CORBA_Environment	&env
)
{
#ifdef	_POSIX_THREADS
    static pthread_mutex_t	lock = PTHREAD_MUTEX_INITIALIZER;

    Critical			region (&lock);
#endif	// _POSIX_THREADS

    env.clear ();

    //
    // Initialising is done only once.
    //
    // XXX Applications that can't tell if they've initialized (e.g.
    // some library modules) must check for a particular error and ignore
    // it if they get it.  We need a minor code to indicate this case.
    // In general, one-time initialization is suboptimal.
    //
    // XXX We also need to enable initialising more than one ORB!!
    //
    if (the_orb) {
	env.exception (new CORBA_INITIALIZE (COMPLETED_NO));
	return 0;
    }

    //
    // Verify some of the basic implementation requirements.  This test
    // gets optimized away by a decent compiler (or else the rest of the
    // routine does).
    //
    // NOTE:  we still "just" assume that native floating point is IEEE.
    //
    if (sizeof (CORBA_Short) != 2
	    || sizeof (CORBA_Long) != 4
	    || sizeof (CORBA_LongLong) != 8
	    || sizeof (CORBA_Float) != 4
	    || sizeof (CORBA_Double) != 8
	    || sizeof (CORBA_LongDouble) != 16
	    || sizeof (CORBA_WChar) < 2
	    || sizeof (void *) != SIZEOF_VOID_P
	    ) {
	env.exception (new CORBA_INITIALIZE (COMPLETED_NO));
	return 0;
    }

    //
    // ignoring argc, argv for now -- no arguments we care about
    //
    // XXX should remove any of the "-ORB*" arguments that we know
    // about ... and report errors for the rest.
    //

#ifdef	DEBUG
    //
    // Make it a little easier to debug programs using this code.
    //
    {
	char		*value = getenv ("ORB_DEBUG");

	if (value != 0) {
	    debug_level = atoi (value);
	    if (debug_level <= 0)
		debug_level = 1;
	    dmsg1 ("debug_level == %d", debug_level);
	}
    }
#endif	// DEBUG

    //
    // On Win32, we should be collecting information from the Registry
    // such as what ORBs are configured, specific configuration details
    // like whether they generate IOR or URL style stringified objrefs
    // and which addresses they listen to (e.g. allowing multihomed
    // hosts to implement firewalls), user-meaningful orb names (they
    // will normally indicate domains), and more.
    //
    // On UNIX, we should collect that from some private config file.
    //
    // Instead, this just treats the "internet" ORB name specially and
    // makes it always use URL-style stringified objrefs, where the
    // hostname and TCP port number are explicit (and the whole objref
    // is readable by mortals).
    //
    CORBA_Boolean	use_ior;

    if (orb_name != 0 && strcmp (orb_name, "internet") == 0)
	use_ior = CORBA_B_FALSE;
    else
	use_ior = CORBA_B_TRUE;

#ifdef	SIGPIPE
    //
    // Impractical to have each call to the ORB protect against the
    // implementation artifact of potential writes to dead connections,
    // as it'd be way expensive.  Do it here; who cares about SIGPIPE
    // in these kinds of applications, anyway?
    //
    (void) signal (SIGPIPE, SIG_IGN);
#endif	// SIGPIPE

#if	defined (_WINSOCKAPI_)
    //
    // winsock needs explicit initialization, and applications must manage
    // versioning problems directly.
    //
    WSADATA	wsadata;

    if (WSAStartup (MAKEWORD (1, 1), &wsadata) != 0) {
        dsockerr ("WSAStartup");
	env.exception (new CORBA_INITIALIZE (COMPLETED_NO));
	return 0;
    }
    if (LOBYTE (wsadata.wVersion) != 1 || HIBYTE (wsadata.wVersion != 1)) {
	dmsg2 ("bad winsock version %d.%d",
		HIBYTE (wsadata.wVersion), LOBYTE (wsadata.wVersion));
	env.exception (new CORBA_INITIALIZE (COMPLETED_NO));
	return 0;
    }
#endif	// _WINSOCKAPI_

    //
    // Call various internal initialization routines.
    //
    __TC_init_table ();
    __TC_init_standard_exceptions (env);
    if (env.exception () != 0)
	return 0;

    //
    // Inititalize the "ORB" pseudo-object now.
    //
    the_orb = new IIOP_ORB (use_ior);

    return the_orb;
}

void
CORBA_ORB::create_list (
    CORBA_Long		count,
    CORBA_NVList_ptr	&retval
)
{
    assert (count <= UINT_MAX);

    retval = new CORBA_NVList;

    if (count != 0) {
	retval->_len = 0;
	retval->_max = (unsigned) count;
	retval->_values = (CORBA_NamedValue_ptr) calloc ((unsigned) count,
		    sizeof (CORBA_NamedValue));
    }
}


//
// This is a server-side internal routine; it's not available to any
// portable code except method code, which moreover may not access the
// state variable directly since its implemention may differ between ORBs.
//
// XXX it's server-side so should be OA-specific and not in this module
//
CORBA_ORB_ptr
_orb ()
{
    return the_orb;
}
