// @(#)typecode.cpp	1.4 95/09/19
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// TYPECODE:	basic implementation of TypeCodes
//
// Typecodes essentially consist of just the CDR octets that get marshaled
// and unmarshaled, and this code knows how to parse those octets and answer
// questions CORBA's TypeCode APIs require.
//
// NOTE: This isn't well tuned performance-wise.  Given how much is variable
// (byte order, alignment) it's clear tuning has its limits with respect to
// CDR bytecode interpretation.
//
// THREADING NOTE:  Typecodes are readonly data structures, and the only
// mutual exclusion relates to reference counting and construction.
//

#include	<assert.h>
#include	<limits.h>
#include	<string.h>
#include	<orb.hh>

#include	"debug.hh"
#include	"cdr.hh"
#include	"thread.hh"

#include	<memory.h>
#include	<sys/types.h>

#include	<initguid.h>


#ifdef	ACE_HAS_THREADS
//
// If POSIX threads are available, set up lock covering refcounts.
//
static pthread_mutex_t		refcnt_lock = PTHREAD_MUTEX_INITIALIZER;
#endif	// ACE_HAS_THREADS


void
CORBA_release (CORBA_TypeCode_ptr tc)
{
    if (tc)
	tc->Release ();
}

CORBA_Boolean
CORBA_is_nil (CORBA_TypeCode_ptr tc)
{
    return (CORBA_Boolean) (tc == 0);
}

//
// Constructor for CONSTANT typecodes with empty parameter lists.
// These are only created once, and those constants are shared. 
//
CORBA_TypeCode::CORBA_TypeCode (
    CORBA_TCKind	kind
) :
    _length	(0),
    _buffer	(0),
    _kind	(kind),
    _parent	(0),
    _refcount	(1),
    _orb_owns	(CORBA_B_FALSE)
{
}


//
// Constructor for all other typecodes, including constants with non-empty
// parameter lists.  See "corba.hh" for details.
//
CORBA_TypeCode::CORBA_TypeCode (
    CORBA_TCKind	kind,
    CORBA_ULong		length,
    CORBA_Octet		*buffer,
    CORBA_Boolean	orb_owns_tc
) :
    _length	(length),
    _buffer	(buffer),
    _kind	(kind),
    _parent	(0),
    _refcount	(1),
    _orb_owns	(orb_owns_tc)
{
    //
    // The CDR code used to interpret TypeCodes requires in-memory
    // alignments to match the "on-the-wire" alignments, simplifying
    // algorithms used to marshal/unmarshal.
    //
    // However, it's often hard to get compilers (in particular) to
    // generate data that's so aligned, since C++ doesn't provide
    // primitives giving control at that low a level.  Although there
    // are ways to get that alignment which work in almost all cases,
    // we need to ensure adequate alignment in _all_ cases.
    //
    // This code exists to ensure such alignment; since the constructor
    // is intended only for use by an IDL compiler or ORB code, it's
    // not currently a priority to ensure the allocated code is freed.
    //
    if ((((ptr_arith_t)buffer) & 0x03) != 0) {
	ptr_arith_t	temp;

	temp = (ptr_arith_t) ACE_OS::malloc (length + 4);
	temp += 3;
	temp &= ~0x03;
	_buffer = (CORBA_Octet *) temp;

	(void) ACE_OS::memcpy (_buffer, buffer, (size_t) length);
	_orb_owns = CORBA_B_FALSE;	// XXX may leak
    }
}


//
// Destructor.  For "indirected" typecodes, the typecode reuses the buffer
// owned by its parent, and so rather than deleting the buffer it just drops
// the parent's refcount.
//
CORBA_TypeCode::~CORBA_TypeCode ()
{
    if (_parent)
	_parent->Release ();
    else if (_orb_owns)
	delete _buffer;
}


//
// COM's IUnknown support
//
//

// {A201E4C1-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_TypeCode,
0xa201e4c1, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);


ULONG
__stdcall
CORBA_TypeCode::AddRef ()
{
#ifdef	ACE_HAS_THREADS
    Critical		region (&refcnt_lock);
#endif

    return _refcount++;
}

ULONG
__stdcall
CORBA_TypeCode::Release ()
{
#ifdef	ACE_HAS_THREADS
    Critical		region (&refcnt_lock);
#endif

    assert (this != 0);

    if (--_refcount != 0)
	return _refcount;
    if (_orb_owns)
	delete this;
    return 0;
}

HRESULT
__stdcall
CORBA_TypeCode::QueryInterface (
    REFIID	riid,
    void	**ppv
)
{
    *ppv = 0;

    if (IID_CORBA_TypeCode == riid || IID_IUnknown == riid)
	*ppv = this;

    if (*ppv == 0)
	return ResultFromScode (E_NOINTERFACE);

    (void) AddRef ();
    return NOERROR;
}


//
// just fetch the 'kind' field out of the typecode
//
CORBA_TCKind
CORBA_TypeCode::kind (
    CORBA_Environment	&env
) const
{
    env.clear ();
    return _kind;
}


//
// skip a typecode encoding in a given CDR stream
//
static CORBA_Boolean
skip_typecode(
    CDR	    	    	    &stream
)
{
    CORBA_ULong   	    kind;
    CORBA_ULong   	    temp;

    if (!stream.get_ulong (kind)
    	    || kind >= TC_KIND_COUNT)
    	return CORBA_B_FALSE;

    switch (kind) {
      //
      // Most TypeCodes have empty parameter lists, nothing to skip
      //
      default:
	break;

      //
      // Some have single integer parameters, easy to skip.
      // 
      // have preallocated constants that could be used.
      //
      case tk_string:
      case tk_wstring:
      case ~0:
	return stream.get_ulong (temp);

      //
      // The rest have "complex" parameter lists that are
      // encoded as bulk octets ... just skip them
      //
      case tk_objref:
      case tk_struct:
      case tk_union:
      case tk_enum:
      case tk_sequence:
      case tk_array:
      case tk_alias:
      case tk_except:
	return stream.get_ulong (temp) != CORBA_B_FALSE
		&& stream.skip_bytes (temp) != CORBA_B_FALSE;
    }

    return CORBA_B_TRUE;
}


//
// Return member labels for tk_union typecodes.
//
CORBA_Any_ptr
CORBA_TypeCode::member_label (
    CORBA_ULong   	    n,
    CORBA_Environment	    &env
) const
{
    env.clear ();

    CDR	    	    stream;

    stream.setup_encapsulation(_buffer, (size_t)_length);

    //
    // this function is only applicable to the tk_union TC
    //
    if (_kind != tk_union) {
    	env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
    	return 0;
    }

    //
    // skip ID and name, and then get the discriminant TC
    //
    CORBA_TypeCode_ptr    tc = 0;

    if (!stream.skip_string ()		// type ID, hidden
    	    || !stream.skip_string ()	// typedef name
	    || CDR::decoder (_tc_CORBA_TypeCode, &tc, this, &stream, env)
    	    	    != CORBA_TypeCode::TRAVERSE_CONTINUE) {
    	env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
    	return 0;
    }

    //
    // skip default used, and get member count
    //
    CORBA_ULong   	member_count;

    if (!stream.get_ulong (member_count)	// default used
    	    || !stream.get_ulong (member_count)
    	    ) {
    	env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
    	return 0;
    }

    //
    // If caller asked for the label for a nonexistent member, they
    // get an error report!
    //
    if (n >= member_count) {
    	env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
    	return 0;
    }

    //
    // Get the n-th member label; they're all the same size and have
    // no nested pointers, so we just overwrite each one with the
    // enxt parameter.
    //
    void    	*buf = new CORBA_Octet [tc->size (env)];

    if (env.exception () != 0)
	return 0;

    for (CORBA_ULong i = 0; i <= n; i++) {
    	if (CDR::decoder (tc, buf, this, &stream, env)
    	    	    	!= CORBA_TypeCode::TRAVERSE_CONTINUE
    	    	|| !stream.skip_string ()   	    // member name
    	    	|| !skip_typecode (stream)) {	    // member TC
    	    delete buf;
    	    env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
    	    return 0;
    	}
    }

    //
    // return the member label as an any
    //
    CORBA_Any		*retval;

    retval = new CORBA_Any(tc, buf, CORBA_B_TRUE);
    tc->Release ();
    return retval;
}


//
// say how many parameters this typecode has; normally a fixed number, some
// are variable length.
//
// NOTE:  This refers to "real" parameters, not what shows up in the IFR
// spec !!   That is, "hidden" parameters are counted here, this doesn't
// strictly comply with what CORBA says "param_count" provides.
//
CORBA_ULong
CORBA_TypeCode::param_count (
    CORBA_Environment		&env
) const
{
    env.clear ();

    switch (_kind) {
      default:
	return 0;

      case tk_string:
      case tk_wstring:
	return 1;

      case tk_objref:
      case tk_sequence:
      case tk_array:
	return 2;

      case tk_alias:
	return 3;

      case tk_except:
      case tk_struct:
    	{
	    CORBA_ULong		members;
	    CDR			stream;

	    stream.setup_encapsulation(_buffer, (size_t)_length);

    	    // skip rest of header (type ID and name) and collect the
    	    // number of struct members
    	    if (!stream.skip_string ()	    	    // ID
            	    || !stream.skip_string ()  	    // struct name
		    || !stream.get_ulong (members)) {
		env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
		return 0;
    	    }

    	    return 3 + 2 * members;
    	}
      case tk_enum:
    	{
	    CORBA_ULong		members;
	    CDR			stream;

	    stream.setup_encapsulation(_buffer, (size_t)_length);

	    // skip rest of header (type ID and name) and collect the
	    // number of struct members
	    if (!stream.skip_string ()			// ID
		    || !stream.skip_string ()		// typedef name
		    || !stream.get_ulong (members)) {
		env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
            	return 0;
    	    }

    	    return 3 + members;
    	}
      case tk_union:
    	{
	    CORBA_ULong		members;
	    CDR			stream;

	    stream.setup_encapsulation(_buffer, (size_t)_length);

    	    // skip rest of header (type ID, name, etc...) and collect the
    	    // number of struct members
    	    if (!stream.skip_string ()			// ID
            	    || !stream.skip_string ()		// struct name
    	    	    || !skip_typecode (stream)		// discriminant TC
		    || !stream.get_ulong (members)	// default used
            	    || !stream.get_ulong (members)	// real member count
		    ) {
		env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
            	return 0;
    	    }

    	    return 5 + 3 * members;
    	}
    }
}


//
// Internal hack, used until member_count() and length() are implemented.
// Doesn't support all the types that those routines support.
//
CORBA_ULong
CORBA_TypeCode::ulong_param (
    CORBA_ULong		n,
    CORBA_Environment	&env
) const
{
    CORBA_ULong		temp;

    temp = param_count (env);		// clears env
    if (env.exception ())
	return 0;

    if (temp < n) {
	env.exception (new CORBA_Bounds);
	return 0;
    }

    //
    // Get parameters for non-empty typecodes; their parameter lists are
    // encapsulated CDR (for complex ones) or inlined (for simple ones).
    //
    switch (_kind) {
      default:				// most have no long params
	break;

      //
      // Array, sequence ... complex parameter lists
      //
      case tk_array:		// param 1 is an integer
      case tk_sequence:		// ... identical content
	{ 
	    if (n == 0)
		break;

	    //
	    // Build CDR stream for encapsulated params, and skip the
	    // typecode up front.
	    //
	    CDR		stream;

	    stream.setup_encapsulation (_buffer, (size_t)_length);
    	    if (!skip_typecode (stream)) {
		env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
    	    	return 0;
    	    }

	    //
	    // Then comes the "bounds" parameter.
	    //
	    if (!stream.get_ulong (temp))
		env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	    return temp;
	}

      //
      // string, wstring ... simple parameter lists, containing just
      // the string bounds (zero indicates unbounded).  Stored specially
      //
      case tk_string:
      case tk_wstring:
	if (n != 0)
	    break;
	return _length;
    }
    env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
    return 0;
}


//
// Internal hack, used until member_type(), discriminator_type(),
// and content_type() are implemented.
//
// NOTE special calling convention for CDR::decoder() when we're
// potentially deencapsulating an indirected typecode:  the "data2"
// value indicates that this typecode is the parent.  See comments
// at CDR::decoder() for further details.
//
CORBA_TypeCode_ptr
CORBA_TypeCode::typecode_param (
    CORBA_ULong		n,
    CORBA_Environment	&env
) const
{
    CORBA_ULong		temp;

    temp = param_count (env);		// clears env
    if (env.exception ())
	return 0;

    if (temp < n) {
	env.exception (new CORBA_Bounds);
	return 0;
    }

    //
    // Build the de-encapsulating CDR stream, bypassing the stringent
    // alignment tests (we're a bit looser in what we need here, and
    // we _know_ we're OK).  Then skip the byte order code.
    //
    CDR				stream;
    CORBA_TypeCode_ptr	tc = 0;

    stream.setup_encapsulation (_buffer, (size_t)_length);

    switch (_kind) {
      default:				// most have no tc params
	break;

      case tk_sequence:		// param 0 is a tc
      case tk_array:
	if (n != 0)
	    break;
	if (CDR::decoder (_tc_CORBA_TypeCode, &tc, this, &stream, env)
		!= CORBA_TypeCode::TRAVERSE_CONTINUE) {
	    env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	    return 0;
	}
	return tc;

      case tk_alias:		// #1 is a tc
	if (n != 2)
	    break;
	if (!stream.skip_string ()		// type ID, hidden
		|| !stream.skip_string ()	// typedef name
		|| CDR::decoder (_tc_CORBA_TypeCode, &tc, this, &stream, env)
		    != CORBA_TypeCode::TRAVERSE_CONTINUE) {
	    env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	    return 0;
	}
	return tc;

      case tk_except:
      case tk_struct:		// #5 and up are tc, index from 0
	if ((n < 4) || (n & 0x1)) {	// tc is at odd number of param list
	    env.exception (new CORBA_Bounds);
	    return 0;
    	}

	if (!stream.skip_string ()		// type ID, hidden
		|| !stream.skip_string ()	// typedef name
            	|| !stream.get_ulong (temp)) { 	// member count
	    env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	    return 0;
    	} else {
	    CORBA_ULong   i;
    	    temp = (n - 3) / 2;

    	    // skip member pairs to the one we want
	    for (i = 0; i < temp; i++) {
    	    	// skip to the member being asked
		if (!stream.skip_string ()	// member name
    	    	    	|| !skip_typecode (stream)) {
		    env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	    	    return 0;
    	    	}
    	    }

    	    if (!stream.skip_string ()
		    || CDR::decoder (_tc_CORBA_TypeCode, &tc, this, &stream,
    	    	    	    env)!= CORBA_TypeCode::TRAVERSE_CONTINUE) {
		env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
    	    	return 0;
    	    }
    	    return tc;
	}

      case tk_union:            // #6 and up are tc, index from 0
	if (n != 2 && (n < 7 || (n - 7) % 3)) {
	    env.exception (new CORBA_Bounds);
	    return 0;
    	}

        if (!stream.skip_string ()		// type ID, hidden
		|| !stream.skip_string ()	// typedef name
		|| CDR::decoder (_tc_CORBA_TypeCode, &tc, this, &stream, env)
                        != CORBA_TypeCode::TRAVERSE_CONTINUE  // TC
		) {
	    env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
            return 0;
        } else if (!stream.get_ulong (temp)	// default used
                || !stream.get_ulong (temp)	// member count
		) {
	    tc->Release ();
	    env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
            return 0;
        }

        if (n == 2)
            return tc;

    	temp = (n - 7) / 3;

	//
	// skip to the member requested
	//
    	CORBA_ULong		i;
	CORBA_Long		scratch;		// always big enough

    	for (i = 0; i < temp; i++) {

    	    if (CDR::decoder (tc, &scratch, this, &stream, env) // member label
                    	    != CORBA_TypeCode::TRAVERSE_CONTINUE
    	    	    || !stream.skip_string ()		// member name
    	    	    || !skip_typecode (stream)) {	// member typecode
		tc->Release ();
		env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	    	return 0;
    	    }
    	}

    	if (CDR::decoder (tc, &scratch, this, &stream, env)	    // member label
    	    	    	!= CORBA_TypeCode::TRAVERSE_CONTINUE
    	    	|| !stream.skip_string ()		// member name
		) {
	    tc->Release ();
	    env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	    return 0;
    	}
	tc->Release ();

	if (CDR::decoder (_tc_CORBA_TypeCode, &tc, this, &stream, env)
                        != CORBA_TypeCode::TRAVERSE_CONTINUE) {
	    env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	    return 0;
	}
    	return tc;
    }

    env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
    return 0;
}


//
// Return the type ID (RepositoryId) for the TypeCode; it may be empty.
//
// NOTE the string returned here is owned by the typecode!!
//
CORBA_String
CORBA_TypeCode::id (
    CORBA_Environment		&env
) const
{
    env.clear ();

    switch (_kind) {
      //
      // These are all complex typecodes, which have as their first parameter
      // (number zero) a repository/type ID string encoded per CDR rules.
      // That means we can just return a pointer to that string directly!
      //
      case tk_objref:
      case tk_struct:
      case tk_union:
      case tk_enum:
      case tk_alias:
      case tk_except:
	return (CORBA_String) (_buffer
		    + 4		// skip byte order flag and padding
		    + 4		// skip (strlen + 1)
		    );

      //
      // No other typecodes ever have type IDs
      //
      default:
	env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
	return 0;
    }
}

