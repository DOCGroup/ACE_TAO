// @(#)cdr.cpp	1.2 95/11/04
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// CDR:		Encode/Decode basic machine data types
//
// Implementation of OMG "Common Data Representation" (CDR) ... there are
// one routine each for byte/halfword/word/doubleword put/get, which adjust
// to establish "natural" alignment (the bulk of the code) and then put or
// get with byteswapping as needed.
//
// The implementation knows that native data formats are conformant with
// OMG-IDL's (and hence CDR's) size requirements, and relies on the fact
// that (for example) CORBA_Long is always four bytes long even if the
// environment's "int" is a different size.
//
//	char, octet			  8 bits (1 byte)
//	short, unsigned short		 16 bits (2 bytes)
//	long, unsigned long, float	 32 bits (4 bytes)
//	double, (unsigned) long long	 64 bits (8 bytes)
//	long double			128 bits (16 bytes)
//
// Moreover, this "knows" that the native 'char' represents ISO Latin/1
// characters (an ASCII superset addressing Western European characters)
// and that "double" and "float" comply with the IEEE standards.  (The
// "long double" may not be a native data type, though.)
//
// THREADING NOTE:  "CDR" is a data structure which must be protected by
// external critical sections.   Like simpler numeric types, "CDR" instances
// are accessed and modified atomically.  This implementation is reentrant,
// so that independent "CDR" values may be manipulated concurrently when
// the underlying programming environment is itself reentrant.
//

#include	<assert.h>
#include	<limits.h>
#include	<string.h>

#include	<corba/orb.hh>

#include	"runtime/debug.hh"
#include	"runtime/cdr.hh"


//
// ENCODING routines ... pad, store.  Never swap.  Padding uses
// whatever value is already in the buffer.
//
CORBA_Boolean
CDR::put_byte (
    char	c
)
{
    if (remaining < sizeof (char) && grow (0) == CORBA_B_FALSE)
	return CORBA_B_FALSE;

    *next++ = (unsigned char) c;
    remaining--;
    return CORBA_B_TRUE;
}


CORBA_Boolean
CDR::put_short (
    CORBA_Short		s
)
{
    register unsigned char	*tmp_next;
    register unsigned		temp;

    //
    // Adjust pointer and count of remaining bytes; maybe
    // grow the buffer if there's not enough left
    //
    tmp_next = ptr_align_binary (next, SHORT_SIZE);
    temp = SHORT_SIZE + (tmp_next - next);
    if (temp > remaining) {
	if (grow (0) == CORBA_B_FALSE)
	    return CORBA_B_FALSE;
	tmp_next = next + temp - SHORT_SIZE;
    }
    remaining -= temp;

    //
    // copy the half word, native byte order
    //
    *(CORBA_Short *)tmp_next = s;
    next = tmp_next + SHORT_SIZE;
    return CORBA_B_TRUE;
}

CORBA_Boolean
CDR::put_long (
    CORBA_Long		l
)
{
    register unsigned char	*tmp_next;
    register unsigned		temp;

    //
    // Adjust pointer and count of remaining bytes; maybe
    // grow the buffer if there's not enough left
    //
    tmp_next = ptr_align_binary (next, LONG_SIZE);
    temp = LONG_SIZE + (tmp_next - next);
    if (temp > remaining) {
	if (grow (0) == CORBA_B_FALSE)
	    return CORBA_B_FALSE;
	tmp_next = next + temp - LONG_SIZE;
    }
    remaining -= temp;

    //
    // copy the word, native byte order
    //
    *(CORBA_Long *)tmp_next =  l;

    next = tmp_next + LONG_SIZE;
    return CORBA_B_TRUE;
}

CORBA_Boolean
CDR::put_longlong (
    const CORBA_LongLong	&ll
)
{
    register unsigned char	*tmp_next;
    register unsigned		temp;

    //
    // Adjust pointer and count of remaining bytes; maybe
    // grow the buffer if there's not enough left
    //
    tmp_next = ptr_align_binary (next, LONGLONG_SIZE);
    temp = LONGLONG_SIZE + (tmp_next - next);
    if (temp > remaining) {
	if (grow (0) == CORBA_B_FALSE)
	    return CORBA_B_FALSE;
	tmp_next = next + temp - LONGLONG_SIZE;
    }
    remaining -= temp;

    //
    // copy the double word in "native" byte order.
    //
    *(CORBA_LongLong *)tmp_next = ll;
    next = tmp_next + LONGLONG_SIZE;
    return CORBA_B_TRUE;
}

CORBA_Boolean
CDR::put_longdouble (
    CORBA_LongDouble	&ld
)
{
    register unsigned char	*tmp_next;
    register unsigned		temp;

    //
    // Adjust pointer and count of remaining bytes; maybe
    // grow the buffer if there's not enough left
    //
    tmp_next = ptr_align_binary (next, LONGDOUBLE_SIZE);
    temp = LONGDOUBLE_SIZE + (tmp_next - next);
    if (temp > remaining) {
	if (grow (0) == CORBA_B_FALSE)
	    return CORBA_B_FALSE;
	tmp_next = next + temp - LONGDOUBLE_SIZE;
    }
    remaining -= temp;

    //
    // copy the long double in "native" byte order.
    //
    *(CORBA_LongDouble *)tmp_next = ld;
    next = tmp_next + LONGDOUBLE_SIZE;
    return CORBA_B_TRUE;
}


//
// DECODING routines ... adjust pointer, then byteswap as needed.
//

CORBA_Boolean
CDR::get_byte (
    char	&c
)
{
    if (remaining < sizeof (char))
	return CORBA_B_FALSE;

    c = (char) *next++;
    remaining--;
    return CORBA_B_TRUE;
}


CORBA_Boolean
CDR::get_short (
    CORBA_Short		&s
)
{
    register unsigned char	*tmp_next;
    register unsigned		temp;

    //
    // Adjust pointer and count of remaining bytes
    //
    tmp_next = ptr_align_binary (next, SHORT_SIZE);
    temp = SHORT_SIZE + (tmp_next - next);
    if (temp > remaining)
	return CORBA_B_FALSE;
    remaining -= temp;

    //
    // decode halfword, swapping as needed
    //
    if (!do_byteswap) {
	s = *(CORBA_Short *)tmp_next;
	next = tmp_next + SHORT_SIZE;
    } else {
	register unsigned char	*sp = (unsigned char *) &s;

	sp [1] = *tmp_next++;
	sp [0] = *tmp_next++;
	next = tmp_next;
    }
    return CORBA_B_TRUE;
}

CORBA_Boolean
CDR::get_long (
    CORBA_Long		&l
)
{
    register unsigned char	*tmp_next;
    register unsigned		temp;

    //
    // Adjust pointer and count of remaining bytes
    //
    tmp_next = ptr_align_binary (next, LONG_SIZE);
    temp = LONG_SIZE + (tmp_next - next);
    if (temp > remaining)
	return CORBA_B_FALSE;
    remaining -= temp;

    //
    // decode word, swapping as needed
    //
    if (!do_byteswap) {
        l =  *(CORBA_Long *)tmp_next;
	next = tmp_next + LONG_SIZE;
    } else {
	register unsigned char	*lp = (unsigned char *) &l;

	//
	// NOTE:  environment-specific speedups abound for this kind
	// of stuff.  This generic code takes advanage of none of them.
	//
	lp [3] = *tmp_next++;
	lp [2] = *tmp_next++;
	lp [1] = *tmp_next++;
	lp [0] = *tmp_next++;
	next = tmp_next;
    }
    return CORBA_B_TRUE;
}

CORBA_Boolean
CDR::get_longlong (
    CORBA_LongLong		&ll
)
{
    register unsigned char	*tmp_next;
    register unsigned		temp;

    //
    // Adjust pointer and count of remaining bytes
    //
    tmp_next = ptr_align_binary (next, LONGLONG_SIZE);
    temp = LONGLONG_SIZE + (tmp_next - next);
    if (temp > remaining)
	return CORBA_B_FALSE;
    remaining -= temp;

    //
    // decode doubleword, swapping as needed
    //
    if (!do_byteswap) {
	ll = *(CORBA_LongLong *)tmp_next;
	next = tmp_next + LONGLONG_SIZE;
    } else {
	register unsigned char	*llp = (unsigned char *) &ll;

	//
	// NOTE:  environment-specific speedups abound for this kind
	// of stuff.  This generic code takes advanage of none of them.
	//
	llp [7] = *tmp_next++;
	llp [6] = *tmp_next++;
	llp [5] = *tmp_next++;
	llp [4] = *tmp_next++;
	llp [3] = *tmp_next++;
	llp [2] = *tmp_next++;
	llp [1] = *tmp_next++;
	llp [0] = *tmp_next++;
	next = tmp_next;
    }
    return CORBA_B_TRUE;
}


CORBA_Boolean
CDR::get_longdouble (
    CORBA_LongDouble		&ld
)
{
    register unsigned char	*tmp_next;
    register unsigned		temp;

    //
    // Adjust pointer and count of remaining bytes
    //
    tmp_next = ptr_align_binary (next, LONGDOUBLE_SIZE);
    temp = LONGDOUBLE_SIZE + (tmp_next - next);
    if (temp > remaining)
	return CORBA_B_FALSE;
    remaining -= temp;

    //
    // copy the long double, swapping bytes as needed
    //
    if (!do_byteswap) {
	ld = *(CORBA_LongDouble *)tmp_next;
	next = tmp_next + LONGDOUBLE_SIZE;
    } else {
	register unsigned char	*ldp = (unsigned char *) &ld;

	//
	// NOTE:  this is a single SPARC V9 instruction
	//
	ldp [15] = *tmp_next++;
	ldp [14] = *tmp_next++;
	ldp [13] = *tmp_next++;
	ldp [12] = *tmp_next++;
	ldp [11] = *tmp_next++;
	ldp [10] = *tmp_next++;
	ldp [ 9] = *tmp_next++;
	ldp [ 8] = *tmp_next++;
	ldp [ 7] = *tmp_next++;
	ldp [ 6] = *tmp_next++;
	ldp [ 5] = *tmp_next++;
	ldp [ 4] = *tmp_next++;
	ldp [ 3] = *tmp_next++;
	ldp [ 2] = *tmp_next++;
	ldp [ 1] = *tmp_next++;
	ldp [ 0] = *tmp_next++;
	next = tmp_next;
    }
    return CORBA_B_TRUE;
}


CORBA_Boolean
CDR::skip_string ()			// ISO/1 or octet string
{
    CORBA_ULong	len;

    if (get_ulong (len) == CORBA_B_FALSE || len > remaining)
	return CORBA_B_FALSE;		// buffer's changed

    next += (unsigned) len;
    remaining -= (unsigned) len;
    return CORBA_B_TRUE;
}


//
// Grow the CDR buffer, either to a known size (incoming message) or
// by a standard increment (creating outgoing message).
//
// We can't use realloc() because of a constraint that the part of the
// buffer into which we marshal be aligned according to MAX_ALIGNMENT,
// which can be a stronger requirement than malloc/realloc places on
// buffer.  This makes growing a buffer on the encode side costly, since
// it can need to be done repetitively and copies more data each time.
//
// NOTE:  this code knows about what's involved in the constructor and
// destructor, as it needs to invoke the constructor and do what the
// destructor would do (and not in the normal order).  It also knows
// all other state that's significant.  Change with care!
//
// NOTE: arguably this is a good place to ensure that the memory's zeroed
// out to comply with Orange Book C2 "object reuse" (meaning data, like
// I/O buffers) policy.  IIOP doesn't mandate such policies though.
//
CORBA_Boolean
CDR::grow (size_t newsize)
{
    unsigned char	*old_realbuf, *oldbuf;
    size_t		offset;
    int			old_do_swap = do_byteswap;

    //
    // Iff old buffer was heap allocated, it gets freed soon.  In any case,
    // we need to know which bytes that have been marshaled or read thus
    // far, so they'll also be in the newly grown buffer.
    //
    if (do_free)
	old_realbuf = real_buffer;
    else
	old_realbuf = 0;
    oldbuf = buffer;
    assert ((next - buffer) < UINT_MAX);
    offset = (unsigned) (next - buffer);

    //
    // Calculate the new buffer's length; if growing for encode, we
    // don't grow in "small" chunks because of the cost.
    //
    size_t		new_len;

    if (newsize == 0) {
	if (length < 4096)
	    new_len = length + 4096;
	else
	    new_len = length * 2;
    } else if (newsize <= length) {
	return CORBA_B_TRUE;
    } else
	new_len = newsize;

    //
    // Get a new buffer that's adequately aligned, and use it to
    // reinitialize ourselves with the "free this buffer later" flag.
    //
    unsigned char	*new_buffer;

    new_len += MAX_ALIGNMENT - 1;
    if ((new_buffer = (unsigned char *) malloc (new_len)) == 0)
	return CORBA_B_FALSE;

    (void) new (this) CDR (new_buffer, new_len, MY_BYTE_SEX, 1);

    //
    // Now restore all the relevant old state that we saved earlier,
    // and free the original buffer if needed.  (The first buffer is
    // normally stack-allocated and so mustn't be freed this way.)
    //
    do_byteswap = old_do_swap;
    memcpy (buffer, oldbuf, offset);
    skip_bytes (offset);

    if (old_realbuf)
	free ((char *) old_realbuf);
    
    return CORBA_B_TRUE;
}
