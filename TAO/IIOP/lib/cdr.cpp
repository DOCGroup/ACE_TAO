// @(#)cdr.cpp  1.2 95/11/04
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// CDR:         Encode/Decode basic machine data types
//
// Implementation of OMG "Common Data Representation" (CDR) ... there
// are one routine each for byte/halfword/word/doubleword put/get,
// which adjust to establish "natural" alignment (the bulk of the
// code) and then put or get with byteswapping as needed.
//
// The implementation knows that native data formats are conformant
// with OMG-IDL's (and hence CDR's) size requirements, and relies on
// the fact that (for example) CORBA_Long is always four bytes long
// even if the environment's "int" is a different size.
//
//      char, octet                       8 bits (1 byte)
//      short, unsigned short            16 bits (2 bytes)
//      long, unsigned long, float       32 bits (4 bytes)
//      double, (unsigned) long long     64 bits (8 bytes)
//      long double                     128 bits (16 bytes)
//
// Moreover, this "knows" that the native 'char' represents ISO
// Latin/1 characters (an ASCII superset addressing Western European
// characters) and that "double" and "float" comply with the IEEE
// standards.  (The "long double" may not be a native data type,
// though.)
//
// THREADING NOTE: "CDR" is a data structure which must be protected
// by external critical sections.  Like simpler numeric types, "CDR"
// instances are accessed and modified atomically.  This
// implementation is reentrant, so that independent "CDR" values may
// be manipulated concurrently when the underlying programming
// environment is itself reentrant.

#include        <assert.h>
#include        <limits.h>
#include        <string.h>

#include        "orb.h"
#include        "cdr.h"

#if !defined(ACE_INLINE)
inline
#endif
void CDR::swap_long(unsigned char *orig, CORBA_Long &target)
{
  register unsigned char	*lp = (unsigned char *) &target;

  lp [ 3] = *orig++;
  lp [ 2] = *orig++;
  lp [ 1] = *orig++;
  lp [ 0] = *orig++;
}

#if !defined(ACE_INLINE)
inline
#endif
void CDR::swap_longlong(unsigned char *orig, CORBA_LongLong &target)
{
	register unsigned char	*llp = (unsigned char *) &target;

	llp [ 7] = *orig++;
	llp [ 6] = *orig++;
	llp [ 5] = *orig++;
	llp [ 4] = *orig++;
	llp [ 3] = *orig++;
	llp [ 2] = *orig++;
	llp [ 1] = *orig++;
	llp [ 0] = *orig++;
}

#if !defined(ACE_INLINE)
inline
#endif
void CDR::swap_longdouble(unsigned char *orig, CORBA_LongDouble &target)
{
	register unsigned char	*ldp = (unsigned char *) &target;

	ldp [15] = *orig++;
	ldp [14] = *orig++;
	ldp [13] = *orig++;
	ldp [12] = *orig++;
	ldp [11] = *orig++;
	ldp [10] = *orig++;
	ldp [ 9] = *orig++;
	ldp [ 8] = *orig++;
	ldp [ 7] = *orig++;
	ldp [ 6] = *orig++;
	ldp [ 5] = *orig++;
	ldp [ 4] = *orig++;
	ldp [ 3] = *orig++;
	ldp [ 2] = *orig++;
	ldp [ 1] = *orig++;
	ldp [ 0] = *orig++;
}

// Grow the CDR buffer, either to a known size (incoming message) or
// by a standard increment (creating outgoing message).
//
// We can't use realloc() because of a constraint that the part of the
// buffer into which we marshal be aligned according to MAX_ALIGNMENT,
// which can be a stronger requirement than malloc/realloc places on
// buffer.  This makes growing a buffer on the encode side costly,
// since it can need to be done repetitively and copies more data each
// time.
//
// NOTE: this code knows about what's involved in the constructor and
// destructor, as it needs to invoke the constructor and do what the
// destructor would do (and not in the normal order).  It also knows
// all other state that's significant.  Change with care!
//
// NOTE: arguably this is a good place to ensure that the memory's
// zeroed out to comply with Orange Book C2 "object reuse" (meaning
// data, like I/O buffers) policy.  IIOP doesn't mandate such policies
// though.

CORBA_Boolean
CDR::grow (size_t newsize)
{
  u_char *old_realbuf;
  u_char *oldbuf;
  size_t offset;
  int old_do_swap = do_byteswap;

  // Iff old buffer was heap allocated, it gets freed soon.  In any
  // case, we need to know which bytes that have been marshaled or
  // read thus far, so they'll also be in the newly grown buffer.

  if (do_free)
    old_realbuf = real_buffer;
  else
    old_realbuf = 0;
  oldbuf = buffer;
  assert ((next - buffer) < UINT_MAX);
  offset = (u_int) (next - buffer);

  // Calculate the new buffer's length; if growing for encode, we
  // don't grow in "small" chunks because of the cost.

  size_t new_len;

  if (newsize == 0) 
    {
      // @@ Can we replace the magic number 4096 with a const or enum?
      // I defer to Andy on this since he has much more experience... --cjc
      if (length < 4096)
	new_len = length + 4096;
      else
	new_len = length * 2;
    } 
  else if (newsize <= length) 
    return CORBA_B_TRUE;
  else
    new_len = newsize;

  // Get a new buffer that's adequately aligned, and use it to
  // reinitialize ourselves with the "free this buffer later" flag.

  u_char *new_buffer;

  new_len += MAX_ALIGNMENT - 1;

  // Is there a reason we use malloc here and not new?
  new_buffer = (u_char *) ACE_OS::malloc (new_len);

  if (new_buffer == 0)
    return CORBA_B_FALSE;

  (void) new (this) CDR (new_buffer, new_len, MY_BYTE_SEX, 1);

  // Now restore all the relevant old state that we saved earlier, and
  // free the original buffer if needed.  (The first buffer is
  // normally stack-allocated and so mustn't be freed this way.)

  do_byteswap = old_do_swap;
  ACE_OS::memcpy (buffer, oldbuf, offset);
  skip_bytes (offset);

  if (old_realbuf)
    ACE_OS::free ((char *) old_realbuf);
    
  return CORBA_B_TRUE;
}

