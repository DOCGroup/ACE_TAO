// $Id$

// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved

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
// standards. (The "long double" may not be a native data type,
// though.)
//
// THREADING NOTE: "CDR" is a data structure which must be protected
// by external critical sections.  Like simpler numeric types, "CDR"
// instances are accessed and modified atomically.  This
// implementation is reentrant, so that independent "CDR" values may
// be manipulated concurrently when the underlying programming
// environment is itself reentrant.

#include "tao/corba.h"

// This functions are private, so it is safe to declare them inline in
// the .cpp file, we still use the ACE_INLINE macro to support
// compilations without any inline code.

ACE_INLINE
void CDR::swap_long(char *orig, CORBA::Long &target)
{
  register char *lp = ACE_reinterpret_cast(char *,&target);

  lp [3] = *orig++;
  lp [2] = *orig++;
  lp [1] = *orig++;
  lp [0] = *orig++;
}

ACE_INLINE
void CDR::swap_ulonglong(char *orig, CORBA::ULongLong &target)
{
  register char *llp = ACE_reinterpret_cast(char *, &target);

  llp [7] = *orig++;
  llp [6] = *orig++;
  llp [5] = *orig++;
  llp [4] = *orig++;
  llp [3] = *orig++;
  llp [2] = *orig++;
  llp [1] = *orig++;
  llp [0] = *orig++;
}

ACE_INLINE
void CDR::swap_longdouble(char *orig, CORBA::LongDouble &target)
{
  register char *ldp = ACE_reinterpret_cast(char *, &target);

  ldp [15] = *orig++;
  ldp [14] = *orig++;
  ldp [13] = *orig++;
  ldp [12] = *orig++;
  ldp [11] = *orig++;
  ldp [10] = *orig++;
  ldp [9] = *orig++;
  ldp [8] = *orig++;
  ldp [7] = *orig++;
  ldp [6] = *orig++;
  ldp [5] = *orig++;
  ldp [4] = *orig++;
  ldp [3] = *orig++;
  ldp [2] = *orig++;
  ldp [1] = *orig++;
  ldp [0] = *orig++;
}

ACE_INLINE void
CDR::mb_align (ACE_Message_Block* mb)
{
  ptr_arith_t temp = (ptr_arith_t) mb->base ();

  temp += MAX_ALIGNMENT - 1;
  temp &= ~ ((ptr_arith_t) MAX_ALIGNMENT - 1);
  char* start = ACE_reinterpret_cast(char*,temp);
  mb->rd_ptr (start);
  mb->wr_ptr (start);
}

ACE_INLINE CORBA::Boolean
CDR::adjust_to_put (size_t size, char*& buf)
{
  buf = ACE_reinterpret_cast (char *,
                              ptr_align_binary (this->mb_->wr_ptr (),
                                                size));
  char *end = buf + size;

  if (end <= this->mb_->end ())
    {
      this->mb_->wr_ptr (end);
      return CORBA::B_TRUE;
    }
  else if (this->grow (0))
    {
      // grow(0) may change the value of wr_ptr() so we have to
      // recompute the position....
      buf = ACE_reinterpret_cast (char *,
                                  ptr_align_binary (this->mb_->wr_ptr (), 
                                                    size));
      this->mb_->wr_ptr (buf + size);
      return CORBA::B_TRUE;
    }

  return CORBA::B_FALSE;
}

ACE_INLINE CORBA::Boolean
CDR::adjust_to_get (size_t size,
                    char*& buf)
{
  buf = ACE_reinterpret_cast (char *,
                              ptr_align_binary (this->mb_->rd_ptr(),
                                                size));
  char *end = buf + size;
  if (end <= this->mb_->end ())
    {
      this->mb_->rd_ptr (end);
      return CORBA::B_TRUE;
    }

  return CORBA::B_FALSE;
}

CDR::CDR (char *buf,
          size_t len,
          int byte_order,
          int consume_buf,
          TAO_Marshal_Factory *f)
  // Constructor ... buffer must be aligned for the strictest CDR
  // alignment requirement, since the algorithms used here only
  // maintain alignment with respect to &buffer [0].  Yes, that
  // complicates the grow () primitive.
  : do_byteswap (byte_order != TAO_ENCAP_BYTE_ORDER),
    factory_ (f),
    mobj_ (0),
    good_bit_ (1)
{
  if (buf != 0)
    {
      ACE_Message_Block::Message_Flags flags =
        consume_buf ? 0 : ACE_Message_Block::DONT_DELETE;
      ACE_NEW (this->mb_,
               ACE_Message_Block
                 (new ACE_Data_Block (len,
                                      ACE_Message_Block::MB_DATA,
                                      (char *) buf, 
                                      0,
                                      0,
                                      flags)));
      // We cannot trust the buffer to be properly aligned.
      CDR::mb_align (this->mb_);
    }
  else
    {
      if (len == 0)
        len = CDR::DEFAULT_BUFSIZE + CDR::MAX_ALIGNMENT;
      ACE_NEW (this->mb_, ACE_Message_Block (len));
      CDR::mb_align (this->mb_);
    }

#if defined(ACE_PURIFY)
  if (this->mb_->base () != 0)
    (void) ACE_OS::memset (this->mb_->base (), 0, len);
#endif
}

CDR::CDR (const CDR& rhs)
  :  do_byteswap (rhs.do_byteswap),
     factory_ (rhs.factory_),
     mobj_ (rhs.mobj_),
     good_bit_ (rhs.good_bit_)
{
  this->mb_ = ACE_Message_Block::duplicate (rhs.mb_);
  this->mb_->wr_ptr (rhs.mb_->wr_ptr ());
  this->mb_->rd_ptr (rhs.mb_->rd_ptr ());
}

CDR::~CDR (void)
{
  ACE_Message_Block::release (this->mb_);
  this->mb_ = 0;
}

CORBA::Boolean
CDR::get_string (char*& buf)
{
  CORBA::ULong len;
  return this->get_encapsulation (buf, len);
}

CORBA::Boolean
CDR::get_encapsulation (char *&buf, CORBA::ULong& size)
{
  // TODO in some platforms this may not be safe.....
  // @@ Carlos, can you please address this?
  if (this->get_ulong (size))
    {
      buf = this->mb_->rd_ptr ();
      this->mb_->rd_ptr (size);
      return CORBA::B_TRUE;
    }

  return CORBA::B_FALSE;
}

void
CDR::setup_encapsulation (char *buf, u_int len)
{
  // Also used when interpreting typecodes, but more generally when
  // getting ready to read from encapsulations.  In such cases the
  // buffer alignment guarantees must be provided by the caller, this
  // code doesn't verify them.  These streams are "read only".
  ACE_Message_Block::release (this->mb_);
  this->mb_ = 0;
  ACE_NEW (this->mb_,
           ACE_Message_Block (buf + 1, len - 1));
  do_byteswap = (buf[0] != TAO_ENCAP_BYTE_ORDER);

  this->mb_->wr_ptr (buf + len);

#if 0
  // @@ TODO This test should be activated in debug version, but it
  // seems a bit too conservative, checking for 4 byte boudaries
  // should be OK for most cases.

  ptr_arith_t temp = (ptr_arith_t) buf + 1;

  temp += MAX_ALIGNMENT - 1;
  temp &= ~ ((ptr_arith_t) MAX_ALIGNMENT - 1);
  char* start = ACE_reinterpret_cast(char*,temp);

  if (start != buf + 1)
    {
      ACE_ERROR ((LM_WARNING,
                  "unproperly aligned buffer in "
                  "CDR::setup_encapsulation\n"));
    }
#endif /* 0 */
}

void
CDR::setup_indirection (CDR& cdr, CORBA::Long offset)
{
  ACE_Message_Block::release (this->mb_);
  this->do_byteswap  = cdr.do_byteswap;
  this->factory_ = cdr.factory_;
  this->mobj_ = cdr.mobj_;
  this->good_bit_ = 1;

  this->mb_ = ACE_Message_Block::duplicate (cdr.mb_);
  this->mb_->wr_ptr (cdr.mb_->wr_ptr ());
  this->mb_->rd_ptr (cdr.mb_->rd_ptr () + offset);

  if (this->mb_->rd_ptr () < this->mb_->base ()
      || this->mb_->rd_ptr () > this->mb_->wr_ptr () )
    this->good_bit_ = 0;
}

void
CDR::reset (void)
{
  CDR::mb_align (this->mb_);
}

// Grow the CDR buffer, either to a known size (incoming message) or
// by a standard increment (creating outgoing message).
//
// We can't use realloc () because of a constraint that the part of the
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

CORBA::Boolean
CDR::grow (size_t newsize)
{
  // Calculate the new buffer's length; if growing for encode, we
  // don't grow in "small" chunks because of the cost.

  size_t new_len;

  size_t size = this->mb_->size();
  if (newsize == 0)
    {
      // TODO We should the growth strategy should be controlled using
      // the ORB parameters....
      if (size < CDR::EXP_GROWTH_MAX)
        new_len = size * 2;
      else
        new_len = size + CDR::LINEAR_GROWTH_CHUNK;
    }
  else if (newsize <= size)
    return CORBA::B_TRUE;
  else
    new_len = newsize;

  new_len += MAX_ALIGNMENT - 1;

  ACE_Message_Block *mb;
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (new_len),
                  CORBA::B_FALSE);

  CDR::mb_align (mb);

  mb->copy (this->mb_->rd_ptr (),
            this->mb_->length());
  ACE_Message_Block::release (this->mb_);
  this->mb_ = mb;

  return CORBA::B_TRUE;
}

// ****************************************************************
// put_ methods

CORBA::Boolean
CDR::put_short (CORBA::Short s)
{
  char *buf;

  if (this->adjust_to_put (SHORT_SIZE, buf))
    {
      // copy the half word, native byte order
      *(CORBA::Short *)buf = s;
      return CORBA::B_TRUE;
    }

  this->good_bit_ = 0;
  return CORBA::B_FALSE;
}

CORBA::Boolean
CDR::put_long (CORBA::Long l)
{
  char *buf;

  if (this->adjust_to_put (LONG_SIZE, buf))
    {
      // copy the word, native byte order
      *(CORBA::Long *)buf =  l;
      return CORBA::B_TRUE;
    }

  this->good_bit_ = 0;
  return CORBA::B_FALSE;
}

CORBA::Boolean
CDR::put_ulonglong (const CORBA::ULongLong &ull)
{
  char *buf;

  if (this->adjust_to_put (LONGLONG_SIZE, buf))
    {
      *(CORBA::ULongLong *)buf = ull;
      return CORBA::B_TRUE;
    }

  this->good_bit_ = 0;
  return CORBA::B_FALSE;
}

CORBA::Boolean
CDR::put_longdouble (CORBA::LongDouble &ld)
{
  char *buf;

  if (this->adjust_to_put (LONGDOUBLE_SIZE, buf))
    {
      // copy the longdouble in native byte order
      *(CORBA::LongDouble *)buf = ld;
      return CORBA::B_TRUE;
    }

  this->good_bit_ = 0;
  return CORBA::B_FALSE;
}

CORBA::Boolean
CDR::put_string (const char *str, CORBA::ULong len)
{
  // Assuming that length is the length of the string. We insert len+1
  // for the additional NUL character.
  if (this->put_long (len + 1))
    {
      char *buf = this->mb_->wr_ptr ();
      char *end = buf + len + 1;

      if (end <= this->mb_->end ()
          || this->grow (this->size () + len + 1))
        this->mb_->copy (str, len + 1);

      return CORBA::B_TRUE;
    }
  return CORBA::B_FALSE;
}

// ****************************************************************
// get_

CORBA::Boolean
CDR::get_short (CORBA::Short &s)
{
  char *buf;
  if (this->adjust_to_get (SHORT_SIZE, buf))
    {
      // decode halfword, swapping as needed
      if (!do_byteswap)
        {
          s = *(CORBA::Short *)buf;
          return CORBA::B_TRUE;  // put a return here to avoid a jump
        }
      else
        {
          // do swapping
          register char   *sp = ACE_reinterpret_cast(char *, &s);

          sp [1] = buf[0];
          sp [0] = buf[1];
          return CORBA::B_TRUE;
        }
    }
  this->good_bit_ = 0;
  return CORBA::B_FALSE;
}

CORBA::Boolean
CDR::get_long (CORBA::Long &l)
{
  char *buf;
  if (this->adjust_to_get (LONG_SIZE, buf))
    {
      if (!do_byteswap)
        {
          l =  *(CORBA::Long *)buf;
          return CORBA::B_TRUE;
        }
      else
        {
          // NOTE: environment-specific speedups abound for this kind
          // of stuff.  This generic code takes advantage of none of
          // them.
          swap_long (buf, l);
          return CORBA::B_TRUE;
        }
    }
  this->good_bit_ = 0;
  return CORBA::B_FALSE;
}

CORBA::Boolean
CDR::get_ulonglong (CORBA::ULongLong &ull)
{
  char *buf;

  if (this->adjust_to_get (LONGLONG_SIZE, buf))
    {
      if (!do_byteswap)
        {
          ull = *(CORBA::ULongLong *)buf;
          return CORBA::B_TRUE;
        }
      else
        {
          swap_ulonglong (buf, ull);
          return CORBA::B_TRUE;
        }
    }

  this->good_bit_ = 0;
  return CORBA::B_FALSE;
}

CORBA::Boolean
CDR::get_longdouble (CORBA::LongDouble &ld)
{
  char *buf;

  if (this->adjust_to_get (LONGDOUBLE_SIZE, buf))
    {
      if (!do_byteswap)
        {
          ld = *(CORBA::LongDouble *)buf;
          return CORBA::B_TRUE;
        }
      else
        {
          swap_longdouble (buf, ld);
          return CORBA::B_TRUE;
        }
    }

  this->good_bit_ = 0;
  return CORBA::B_FALSE;
}

CORBA::Boolean
CDR::get_string (char *&str, CORBA::ULong len)
{
  // len includes the terminating 0
  char *end = this->mb_->rd_ptr () + len;

  if (end <= this->mb_->end ())
    {
      ACE_OS::memcpy (str, this->mb_->rd_ptr (), len);
      this->mb_->rd_ptr (end);
      return CORBA::B_TRUE;
    }

  return CORBA::B_FALSE;
}
