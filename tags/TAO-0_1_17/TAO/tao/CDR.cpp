// $Id$

//
// Portions of this file are:
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

#if !defined (__ACE_INLINE__)
# include "tao/CDR.i"
#endif /* ! __ACE_INLINE__ */

// This functions are private, so it is safe to declare them inline in
// the .cpp file, we still use the ACE_INLINE macro to support
// compilations without any inline code.

ACE_INLINE
void CDR::swap_2 (const char *orig, char* target)
{
  target[1] = *orig++;
  target[0] = *orig++;
}

ACE_INLINE
void CDR::swap_4 (const char *orig, char* target)
{
  target [3] = *orig++;
  target [2] = *orig++;
  target [1] = *orig++;
  target [0] = *orig++;
}

ACE_INLINE
void CDR::swap_8 (const char *orig, char* target)
{
  target [7] = *orig++;
  target [6] = *orig++;
  target [5] = *orig++;
  target [4] = *orig++;
  target [3] = *orig++;
  target [2] = *orig++;
  target [1] = *orig++;
  target [0] = *orig++;
}

ACE_INLINE
void CDR::swap_16 (const char *orig, char* target)
{
  target [15] = *orig++;
  target [14] = *orig++;
  target [13] = *orig++;
  target [12] = *orig++;
  target [11] = *orig++;
  target [10] = *orig++;
  target [9] = *orig++;
  target [8] = *orig++;
  target [7] = *orig++;
  target [6] = *orig++;
  target [5] = *orig++;
  target [4] = *orig++;
  target [3] = *orig++;
  target [2] = *orig++;
  target [1] = *orig++;
  target [0] = *orig++;
}

int
CDR::grow (ACE_Message_Block*& mb, size_t minsize)
{
  // Calculate the new buffer's length; if growing for encode, we
  // don't grow in "small" chunks because of the cost.

  size_t size = mb->size();
  size_t newsize = size;

  if (minsize == 0)
    {
      // TODO We should the growth strategy should be controlled using
      // the ORB parameters....
      if (size < CDR::EXP_GROWTH_MAX)
        newsize *= 2;
      else
        newsize += CDR::LINEAR_GROWTH_CHUNK;
    }
  else if (minsize <= size)
    return 0;
  else
    {
      while (newsize < minsize)
        {
          if (newsize < CDR::EXP_GROWTH_MAX)
            newsize *= 2;
          else
            newsize += CDR::LINEAR_GROWTH_CHUNK;
        }
    }

  ACE_Message_Block* tmp;
  ACE_NEW_RETURN (tmp, ACE_Message_Block (newsize), -1);

  CDR::mb_align (tmp);

  tmp->copy (mb->rd_ptr (), mb->length());
  ACE_Message_Block::release (mb);
  mb = tmp;

  return 0;
}

// ****************************************************************
// ****************************************************************
// ****************************************************************

TAO_OutputCDR::TAO_OutputCDR (size_t size,
                              int byte_order,
                              TAO_Marshal_Factory *factory)
  :  factory_ (factory),
     do_byte_swap_ (byte_order != TAO_ENCAP_BYTE_ORDER),
     good_bit_ (1)
{
  if (size == 0)
    {
      size = CDR::DEFAULT_BUFSIZE + CDR::MAX_ALIGNMENT;
    }
  ACE_NEW (this->start_, ACE_Message_Block (size));
  CDR::mb_align (this->start_);
}

TAO_OutputCDR::TAO_OutputCDR (char *data, size_t size,
                              int byte_order,
                              TAO_Marshal_Factory *factory)
  :  factory_ (factory),
     do_byte_swap_ (byte_order != TAO_ENCAP_BYTE_ORDER),
     good_bit_ (1)
{
  ACE_NEW (this->start_, ACE_Message_Block (data, size));
  // We cannot trust the buffer to be properly aligned
  CDR::mb_align (this->start_);
}

TAO_OutputCDR::TAO_OutputCDR (ACE_Message_Block *data,
                              int byte_order,
                              TAO_Marshal_Factory *factory)
  :  factory_ (factory),
     do_byte_swap_ (byte_order != TAO_ENCAP_BYTE_ORDER),
     good_bit_ (1)
{
  this->start_ = ACE_Message_Block::duplicate (data);
  // We cannot trust the buffer to be properly aligned
  CDR::mb_align (this->start_);
}

TAO_OutputCDR::~TAO_OutputCDR (void)
{
  ACE_Message_Block::release (this->start_);
  this->start_ = 0;
}

void
TAO_OutputCDR::reset (void)
{
  CDR::mb_align (this->start_);
}

ACE_INLINE char*
TAO_OutputCDR::wr_ptr (void) const
{
  return this->start_->wr_ptr ();
}

ACE_INLINE char*
TAO_OutputCDR::end (void) const
{
  return this->start_->end ();
}

ACE_INLINE int
TAO_OutputCDR::adjust (size_t size, size_t align, char*& buf)
{
  buf = ptr_align_binary (this->wr_ptr(), align);
  char *end = buf + size;

  if (end <= this->end ())
    {
      this->start_->wr_ptr (end);
      return 0;
    }
  else if (CDR::grow (this->start_,
                      this->start_->size() + (end - this->end () )) == 0)
    {
      // grow(0) may change the value of wr_ptr() so we have to
      // recompute the position....
      buf = ptr_align_binary (this->wr_ptr(), align);
      this->start_->wr_ptr (buf + size);
      return 0;
    }
  this->good_bit_ = 0;
  return -1;
}

ACE_INLINE int
TAO_OutputCDR::adjust (size_t size, char*& buf)
{
  return this->adjust (size, size, buf);
}

CORBA_Boolean
TAO_OutputCDR::write_1 (const CORBA::Octet* x)
{
  if (this->wr_ptr () < this->end() || CDR::grow(this->start_, 0) == 0)
    {
      *ACE_reinterpret_cast(CORBA::Octet*,this->wr_ptr()) = *x;
      this->start_->wr_ptr (1);
      return CORBA::B_TRUE;
    }

  return CORBA::B_FALSE;
}

CORBA_Boolean
TAO_OutputCDR::write_2 (const CORBA::UShort* x)
{
  char* buf;
  if (this->adjust (CDR::SHORT_SIZE, buf) == 0)
    {
#if !defined (TAO_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast(CORBA::UShort*,buf) = *x;
      return CORBA::B_TRUE;
#else
      if (!this->do_byte_swap_)
	{
	  *ACE_reinterpret_cast(CORBA::UShort*,buf) = *x;
	  return CORBA::B_TRUE;
	}
      else
	{
	  CDR::swap_2 (ACE_reinterpret_cast(char*,x), buf);
	  return CORBA::B_TRUE;
	}
#endif /* TAO_ENABLE_SWAP_ON_WRITE */
    }
  return CORBA::B_FALSE;
}

CORBA_Boolean
TAO_OutputCDR::write_4 (const CORBA::ULong* x)
{
  char* buf;
  if (this->adjust (CDR::LONG_SIZE, buf) == 0)
    {
#if !defined (TAO_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast(CORBA::ULong*,buf) = *x;
      return CORBA::B_TRUE;
#else
      if (!this->do_byte_swap_)
	{
	  *ACE_reinterpret_cast(CORBA::ULong*,buf) = *x;
	  return CORBA::B_TRUE;
	}
      else
	{
	  CDR::swap_4 (ACE_reinterpret_cast(char*,x), buf);
	  return CORBA::B_TRUE;
	}
#endif /* TAO_ENABLE_SWAP_ON_WRITE */
    }
  return CORBA::B_FALSE;
}

CORBA_Boolean
TAO_OutputCDR::write_8 (const CORBA::ULongLong* x)
{
  char* buf;
  if (this->adjust (CDR::LONGLONG_SIZE, buf) == 0)
    {
#if !defined (TAO_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast(CORBA::ULongLong*,buf) = *x;
      return CORBA::B_TRUE;
#else
      if (!this->do_byte_swap_)
	{
	  *ACE_reinterpret_cast(CORBA::ULongLong*,buf) = *x;
	  return CORBA::B_TRUE;
	}
      else
	{
	  CDR::swap_8 (ACE_reinterpret_cast(char*,x), buf);
	  return CORBA::B_TRUE;
	}
#endif /* TAO_ENABLE_SWAP_ON_WRITE */
    }
  return CORBA::B_FALSE;
}

CORBA_Boolean
TAO_OutputCDR::write_16 (const CORBA::LongDouble* x)
{
  char* buf;
  if (this->adjust (CDR::LONGDOUBLE_SIZE, CDR::LONGDOUBLE_ALIGN, buf) == 0)
    {
#if !defined (TAO_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast(CORBA::LongDouble*,buf) = *x;
      return CORBA::B_TRUE;
#else
      if (!this->do_byte_swap_)
	{
	  *ACE_reinterpret_cast(CORBA::LongDouble*,buf) = *x;
	  return CORBA::B_TRUE;
	}
      else
	{
	  CDR::swap_16 (ACE_reinterpret_cast(char*,x), buf);
	  return CORBA::B_TRUE;
	}
#endif /* TAO_ENABLE_SWAP_ON_WRITE */
    }
  return CORBA::B_FALSE;
}

CORBA_Boolean
TAO_OutputCDR::write_array (const void* x,
                            size_t size,
                            size_t align,
                            CORBA::ULong length)
{
  char* buf;
  if (this->adjust (size * length, align, buf) == 0)
    {
#if !defined (TAO_ENABLE_SWAP_ON_WRITE)
      ACE_OS::memcpy (buf, x, size*length);
      return CORBA::B_TRUE;
#else
      if (!this->do_byte_swap_)
	{
	  ACE_OS::memcpy (buf, x, size*length);
	  return CORBA::B_TRUE;
	}
      else
	{
          // I cannot see any fast way out of this....
          typedef void (*SWAPPER)(const char*, char*);
          SWAPPER swapper;
          switch (size)
            {
            case 2:
              swapper = CDR::swap_2;
              break;
            case 4:
              swapper = CDR::swap_4;
              break;
            case 8:
              swapper = CDR::swap_8;
              break;
            case 16:
              swapper = CDR::swap_16;
              break;
            default:
              // TODO: print something?
              this->good_bit_ = 0;
              return CORBA::B_FALSE;
            }
          char *source = ACE_reinterpret_cast(char*,x);
          char *end = target + size*length;
          for (; source != end; source += size, buf += size)
            {
              (*swapper)(source, buf);
            }
	}
#endif /* TAO_ENABLE_SWAP_ON_WRITE */
    }
  this->good_bit_ = 0;
  return CORBA::B_FALSE;
}

CORBA_Boolean
TAO_OutputCDR::write_string (const CORBA::Char *x)
{
  if (x != 0)
    {
      CORBA::ULong len = ACE_OS::strlen (x) + 1;
      if (this->write_ulong (len))
	{
	  return this->write_char_array (x, len);
	}
    }
  else
    {
      // Be nice to programmers: treat nulls as empty strings not
      // errors. (OMG-IDL supports languages that don't use the C/C++
      // notion of null v. empty strings; nulls aren't part of the OMG-IDL
      // string model.)
      if (this->write_ulong (1))
	{
	  return this->write_char (0);
	}
    }
  return CORBA::B_FALSE;
}

CORBA_Boolean
TAO_OutputCDR::write_wstring (const CORBA::WChar *x)
{
  if (x != 0)
    {
      CORBA::ULong len = ACE_OS::wslen (x) + 1;
      if (this->write_ulong (len))
	{
	  return this->write_wchar_array (x, len);
	}
    }
  else
    {
      if (this->write_ulong (1))
	{
	  return this->write_wchar (0);
	}
    }
  return CORBA::B_FALSE;
}

CORBA_Boolean
TAO_OutputCDR::write_boolean_array (const CORBA::Boolean* x,
                                    CORBA::ULong length)
{
  // It is hard to optimize this, the spec requires that on the wire
  // booleans be represented as a byte with value 0 or 1, but in
  // memoery it is possible (though very unlikely) that a boolean has
  // a non-zero value (different from 1).
  // We resort to a simple loop.
  const CORBA::Boolean* end = x + length;
  for (const CORBA::Boolean* i = x; i != end && this->good_bit(); ++i)
    {
      this->write_boolean (*i);
    }
  return this->good_bit ();
}

// ****************************************************************

TAO_InputCDR::TAO_InputCDR (const char *buf, size_t bufsiz,
                            int byte_order,
                            TAO_Marshal_Factory *factory)
  : factory_ (factory),
    do_byte_swap_ (byte_order != TAO_ENCAP_BYTE_ORDER),
    good_bit_ (1)
{
  ACE_NEW (this->start_, ACE_Message_Block (buf, bufsiz));
  this->start_->wr_ptr (bufsiz);
}

TAO_InputCDR::TAO_InputCDR (ACE_Message_Block *data,
                            int byte_order,
                            TAO_Marshal_Factory *factory)
  :  factory_ (factory),
     do_byte_swap_ (byte_order != TAO_ENCAP_BYTE_ORDER),
     good_bit_ (1)
{
  this->start_ = ACE_Message_Block::duplicate (data);
}

TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs,
                            size_t size,
                            CORBA::Long offset)
  : start_ (ACE_Message_Block::duplicate (rhs.start_)),
    factory_ (rhs.factory_),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1)
{
  char* newpos = this->start_->rd_ptr() + offset;
  if (this->start_->base () <= newpos
      && newpos <= this->start_->end ()
      && newpos + size <= this->start_->end ())
    {
      this->start_->rd_ptr (newpos);
      this->start_->wr_ptr (newpos + size);
    }
  else
    {
      this->good_bit_ = 0;
    }
}

TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs,
                            size_t size)
  : start_ (ACE_Message_Block::duplicate (rhs.start_)),
    factory_ (rhs.factory_),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1)
{
  char* newpos = this->start_->rd_ptr();
  if (this->start_->base () <= newpos
      && newpos <= this->start_->end ()
      && newpos + size <= this->start_->end ())
    {
      // Notice that ACE_Message_Block::duplicate may leave the
      // wr_ptr() with a higher value that what we actually want.
      this->start_->wr_ptr (newpos + size);

      CORBA::Octet byte_order;
      this->read_octet (byte_order);
      this->do_byte_swap_ = (byte_order != TAO_ENCAP_BYTE_ORDER);
    }
  else
    {
      this->good_bit_ = 0;
    }
}

TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs)
  : start_ (ACE_Message_Block::duplicate (rhs.start_)),
    factory_ (rhs.factory_),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1)
{
}

TAO_InputCDR&
TAO_InputCDR::operator= (const TAO_InputCDR& rhs)
{
  if (this != &rhs)
    {
      ACE_Message_Block::release (this->start_);
      this->start_ = ACE_Message_Block::duplicate (rhs.start_);
      this->factory_ = rhs.factory_;
      this->do_byte_swap_ = rhs.do_byte_swap_;
      this->good_bit_ = 1;
    }
  return *this;
}

TAO_InputCDR::TAO_InputCDR (const TAO_OutputCDR& rhs)
  : start_ (ACE_Message_Block::duplicate (rhs.start_)),
    factory_ (rhs.factory_),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1)
{
}

TAO_InputCDR::~TAO_InputCDR (void)
{
  ACE_Message_Block::release (this->start_);
  this->start_ = 0;
}

CORBA_Boolean
TAO_InputCDR::read_string (CORBA::Char*& x)
{
  CORBA::ULong len;
  this->read_ulong (len);
  if (this->good_bit() && len > 0)
    {
      x = CORBA::string_alloc (len);
      if (this->read_char_array (x, len))
        return CORBA::B_TRUE;
      CORBA::string_free (x);
    }
  x = 0;
  return CORBA::B_FALSE;
}

CORBA_Boolean
TAO_InputCDR::read_wstring (CORBA::WChar*& x)
{
  CORBA::ULong len;
  this->read_ulong (len);
  if (this->good_bit())
    {
      x = CORBA::wstring_alloc (len);
      if (this->read_wchar_array (x, len))
        return CORBA::B_TRUE;

      CORBA::wstring_free (x);
    }
  x = 0;
  return CORBA::B_FALSE;
}

ACE_INLINE char*
TAO_InputCDR::end (void)
{
  return this->start_->end ();
}

ACE_INLINE void
TAO_InputCDR::rd_ptr (size_t offset)
{
  this->start_->rd_ptr (offset);
}

ACE_INLINE int
TAO_InputCDR::adjust (size_t size,
                      size_t align,
                      char*& buf)
{
  buf = ptr_align_binary (this->rd_ptr(), align);
  char *end = buf + size;
  if (end <= this->end ())
    {
      this->start_->rd_ptr (end);
      return 0;
    }

  this->good_bit_ = 0;
  return -1;
}

ACE_INLINE int
TAO_InputCDR::adjust (size_t size,
                      char*& buf)
{
  return this->adjust (size, size, buf);
}

CORBA_Boolean
TAO_InputCDR::read_1 (CORBA::Octet* x)
{
  if (this->rd_ptr () < this->end())
    {
      *x = *ACE_reinterpret_cast(CORBA::Octet*,this->rd_ptr());
      this->start_->rd_ptr (1);
      return CORBA::B_TRUE;
    }

  this->good_bit_ = 0;
  return CORBA::B_FALSE;
}

CORBA_Boolean
TAO_InputCDR::read_2 (CORBA::UShort* x)
{
  char* buf;
  if (this->adjust (CDR::SHORT_SIZE, buf) == 0)
    {
#if !defined (TAO_DISABLE_SWAP_ON_READ)
      if (!this->do_byte_swap_)
        {
          *x = *ACE_reinterpret_cast(CORBA::UShort*,buf);
        }
      else
        {
          CDR::swap_2 (buf, ACE_reinterpret_cast(char*,x));
        }
#else
      *x = *ACE_reinterpret_cast(CORBA::UShort*,buf);
#endif /* TAO_DISABLE_SWAP_ON_READ */
      return CORBA::B_TRUE;
    }
  return CORBA::B_FALSE;
}

CORBA_Boolean
TAO_InputCDR::read_4 (CORBA::ULong* x)
{
  char* buf;
  if (this->adjust (CDR::LONG_SIZE, buf) == 0)
    {
#if !defined (TAO_DISABLE_SWAP_ON_READ)
      if (!this->do_byte_swap_)
        {
          *x = *ACE_reinterpret_cast(CORBA::ULong*,buf);
        }
      else
        {
          CDR::swap_4 (buf, ACE_reinterpret_cast(char*,x));
        }
#else
      *x = *ACE_reinterpret_cast(CORBA::ULong*,buf);
#endif /* TAO_DISABLE_SWAP_ON_READ */
      return CORBA::B_TRUE;
    }
  return CORBA::B_FALSE;
}

CORBA_Boolean
TAO_InputCDR::read_8 (CORBA::ULongLong* x)
{
  char* buf;
  if (this->adjust (CDR::LONGLONG_SIZE, buf) == 0)
    {
#if !defined (TAO_DISABLE_SWAP_ON_READ)
      if (!this->do_byte_swap_)
        {
          *x = *ACE_reinterpret_cast(CORBA::ULongLong*,buf);
        }
      else
        {
          CDR::swap_8 (buf, ACE_reinterpret_cast(char*,x));
        }
#else
      *x = *ACE_reinterpret_cast(CORBA::ULongLong*,buf);
#endif /* TAO_DISABLE_SWAP_ON_READ */
      return CORBA::B_TRUE;
    }
  return CORBA::B_FALSE;
}

CORBA_Boolean
TAO_InputCDR::read_16 (CORBA::LongDouble* x)
{
  char* buf;
  if (this->adjust (CDR::LONGDOUBLE_SIZE,
                    CDR::LONGDOUBLE_ALIGN,
                    buf) == 0)
    {
#if !defined (TAO_DISABLE_SWAP_ON_READ)
      if (!this->do_byte_swap_)
        {
          *x = *ACE_reinterpret_cast(CORBA::LongDouble*,buf);
        }
      else
        {
          CDR::swap_16 (buf, ACE_reinterpret_cast(char*,x));
        }
#else
      *x = *ACE_reinterpret_cast(CORBA::LongDouble*,buf);
#endif /* TAO_DISABLE_SWAP_ON_READ */
      return CORBA::B_TRUE;
    }
  return CORBA::B_FALSE;
}

CORBA_Boolean
TAO_InputCDR::read_array (void* x,
                          size_t size,
                          size_t align,
                          CORBA::ULong length)
{
  char* buf;
  if (this->adjust (size * length, align, buf) == 0)
    {
#if !defined (TAO_DISABLE_SWAP_ON_READ)
      if (!this->do_byte_swap_ || size == 1)
        {
          ACE_OS::memcpy (x, buf, size*length);
        }
      else
        {
          // I cannot see any fast way out of this....
          typedef void (*SWAPPER)(const char*, char*);
          SWAPPER swapper;
          switch (size)
            {
            case 2:
              swapper = CDR::swap_2;
              break;
            case 4:
              swapper = CDR::swap_4;
              break;
            case 8:
              swapper = CDR::swap_8;
              break;
            case 16:
              swapper = CDR::swap_16;
              break;
            default:
              // TODO: print something?
              this->good_bit_ = 0;
              return CORBA::B_FALSE;
            }
          char *target = ACE_reinterpret_cast(char*,x);
          char *end = target + size*length;
          for (; target != end; target += size, buf += size)
            {
              (*swapper)(buf, target);
            }
        }
#else
      ACE_OS::memcpy (x, buf, size*length);
#endif /* TAO_DISABLE_SWAP_ON_READ */
      return this->good_bit_;
    }
  return CORBA::B_FALSE;
}

CORBA_Boolean
TAO_InputCDR::read_boolean_array (CORBA::Boolean* x,
                                  CORBA::ULong length)
{
  // It is hard to optimize this, the spec requires that on the wire
  // booleans be represented as a byte with value 0 or 1, but in
  // memoery it is possible (though very unlikely) that a boolean has
  // a non-zero value (different from 1).
  // We resort to a simple loop.
  for (CORBA::ULong i = 0; i != length && this->good_bit_; ++i)
    {
      this->read_boolean (x[i]);
    }
  return this->good_bit_;
}

CORBA_Boolean
TAO_InputCDR::skip_string (void)
{
  CORBA::ULong len;
  if (this->read_ulong (len))
    {
      if (this->rd_ptr () + len <= this->end ())
        {
          this->rd_ptr (len);
          return CORBA::B_TRUE;
        }
      this->good_bit_ = 0;
    }

  return CORBA::B_FALSE;
}

CORBA_Boolean
TAO_InputCDR::skip_bytes (size_t len)
{
  if (this->rd_ptr () + len <= this->end ())
    {
      this->rd_ptr (len);
      return CORBA::B_TRUE;
    }
  this->good_bit_ = 0;
  return CORBA::B_FALSE;
}
