// $Id$

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
// by external critical sections.

#include "tao/corba.h"
#include "ace/Timeprobe.h"

#if !defined (__ACE_INLINE__)
# include "tao/CDR.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, CDR, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_CDR_Timeprobe_Description[] =
{
  "OutputCDR::ctor[1] - enter",
  "OutputCDR::ctor[1] - leave",
  "OutputCDR::ctor[2] - enter",
  "OutputCDR::ctor[2] - leave",
  "OutputCDR::ctor[3] - enter",
  "OutputCDR::ctor[3] - leave"
};

enum
{
  TAO_OUTPUT_CDR_CTOR1_ENTER = 2000,
  TAO_OUTPUT_CDR_CTOR1_LEAVE,
  TAO_OUTPUT_CDR_CTOR2_ENTER,
  TAO_OUTPUT_CDR_CTOR2_LEAVE,
  TAO_OUTPUT_CDR_CTOR3_ENTER,
  TAO_OUTPUT_CDR_CTOR3_LEAVE
};


// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_CDR_Timeprobe_Description,
                                  TAO_OUTPUT_CDR_CTOR1_ENTER);

#endif /* ACE_ENABLE_TIMEPROBES */

int
CDR::grow (ACE_Message_Block *mb, size_t minsize)
{
  // Calculate the new buffer's length; if growing for encode, we
  // don't grow in "small" chunks because of the cost.

  size_t size = mb->size();
  size_t newsize = size;

  if (minsize == 0)
    {
      // TODO We should the growth strategy should be controlled using
      // the ORB parameters....
      if (newsize == 0)
        newsize = CDR::DEFAULT_BUFSIZE;
      else if (size < CDR::EXP_GROWTH_MAX)
        newsize *= 2;
      else
        newsize += CDR::LINEAR_GROWTH_CHUNK;
    }
  else if (minsize + CDR::MAX_ALIGNMENT <= size)
    return 0;
  else
    {
      if (newsize == 0)
	newsize = CDR::DEFAULT_BUFSIZE;

      while (newsize < minsize + CDR::MAX_ALIGNMENT)
        {
          if (newsize < CDR::EXP_GROWTH_MAX)
            newsize *= 2;
          else
            newsize += CDR::LINEAR_GROWTH_CHUNK;
        }
    }

#if 0
  TAO_ORB_Core* orb_core = TAO_ORB_Core_instance ();
  ACE_Message_Block tmp (newsize,
			 ACE_Message_Block::MB_DATA,
			 0, 0,
			 orb_core->cdr_buffer_allocator (),
			 0,
			 0,
			 ACE_Time_Value::zero,
			 ACE_Time_Value::max_time,
			 orb_core->data_block_allocator ());
#else
  ACE_Message_Block tmp (newsize);
#endif
  CDR::mb_align (&tmp);

  tmp.copy (mb->rd_ptr (), mb->length());
  mb->data_block (tmp.data_block ()->duplicate ());
  mb->rd_ptr (tmp.rd_ptr ());
  mb->wr_ptr (tmp.wr_ptr ());

  return 0;
}

// ****************************************************************
// ****************************************************************
// ****************************************************************

TAO_OutputCDR::TAO_OutputCDR (size_t size,
                              int byte_order,
                              TAO_Marshal_Factory *factory,
			      ACE_Allocator *buffer_allocator,
			      ACE_Allocator *data_block_allocator)
  :  start_ (size?size:CDR::DEFAULT_BUFSIZE + CDR::MAX_ALIGNMENT,
	     ACE_Message_Block::MB_DATA,
	     0, 0,
	     buffer_allocator,
	     0,
	     0,
	     ACE_Time_Value::zero,
	     ACE_Time_Value::max_time,
	     data_block_allocator),
     factory_ (factory),
     do_byte_swap_ (byte_order != TAO_ENCAP_BYTE_ORDER),
     good_bit_ (1)
{
  ACE_FUNCTION_TIMEPROBE (TAO_OUTPUT_CDR_CTOR1_ENTER);
  CDR::mb_align (&this->start_);
  this->current_ = &this->start_;
}

TAO_OutputCDR::TAO_OutputCDR (char *data, size_t size,
                              int byte_order,
                              TAO_Marshal_Factory *factory,
			      ACE_Allocator *buffer_allocator,
			      ACE_Allocator *data_block_allocator)
  :  start_ (size,
	     ACE_Message_Block::MB_DATA,
	     0,
	     data,
	     buffer_allocator,
	     0,
	     0,
	     ACE_Time_Value::zero,
	     ACE_Time_Value::max_time,
	     data_block_allocator),
     factory_ (factory),
     do_byte_swap_ (byte_order != TAO_ENCAP_BYTE_ORDER),
     good_bit_ (1)
{
  ACE_FUNCTION_TIMEPROBE (TAO_OUTPUT_CDR_CTOR2_ENTER);
  // We cannot trust the buffer to be properly aligned
  CDR::mb_align (&this->start_);
  this->current_ = &this->start_;
}

TAO_OutputCDR::TAO_OutputCDR (ACE_Message_Block *data,
                              int byte_order,
                              TAO_Marshal_Factory *factory)
  :  start_ (data->data_block ()->duplicate ()),
     factory_ (factory),
     do_byte_swap_ (byte_order != TAO_ENCAP_BYTE_ORDER),
     good_bit_ (1)
{ 
  ACE_FUNCTION_TIMEPROBE (TAO_OUTPUT_CDR_CTOR3_ENTER);
  // We cannot trust the buffer to be properly aligned
  CDR::mb_align (&this->start_);
  this->current_ = &this->start_;
}

TAO_OutputCDR::~TAO_OutputCDR (void)
{
  if (this->start_.cont () != 0)
    {
      ACE_Message_Block::release (this->start_.cont ());
      this->start_.cont (0);
    }
  this->current_ = 0;
}

void
TAO_OutputCDR::reset (void)
{
  this->current_ = &this->start_;
  CDR::mb_align (&this->start_);
}

size_t
TAO_OutputCDR::total_length (void) const
{
  size_t l = 0;
  // Compute the total size.
  for (const ACE_Message_Block *i = this->begin ();
       i != this->end ();
       i = i->cont ())
    l += i->length ();
  return l;
}

int
TAO_OutputCDR::grow_and_adjust (size_t size, size_t align, char*& buf)
{
  if (this->current_->cont () == 0
      || this->current_->cont ()->size () < size + CDR::MAX_ALIGNMENT)
    {
      // Allocate the next block, it must be large enough.
      size_t block_size = CDR::DEFAULT_BUFSIZE;
      while (block_size < size + CDR::MAX_ALIGNMENT)
	{
	  if (block_size < CDR::EXP_GROWTH_MAX)
	    block_size *= 2;
	  else
	    block_size += CDR::LINEAR_GROWTH_CHUNK;
	}
      this->good_bit_ = 0;
      ACE_Message_Block* tmp;
      TAO_ORB_Core* orb_core = TAO_ORB_Core_instance ();
      ACE_NEW_RETURN (tmp,
		      ACE_Message_Block (block_size,
					 ACE_Message_Block::MB_DATA,
					 0, 0,
					 orb_core->output_cdr_buffer_allocator (),
					 0,
					 0,
					 ACE_Time_Value::zero,
					 ACE_Time_Value::max_time,
					 orb_core->output_cdr_dblock_allocator ()),
		      -1);
      this->good_bit_ = 1;

      // The new block must start with the same alignment as the
      // previous block finished.
      ptr_arith_t tmpalign =
	ptr_arith_t(tmp->wr_ptr ()) % CDR::MAX_ALIGNMENT;
      ptr_arith_t curalign =
	ptr_arith_t(this->current_->wr_ptr ()) % CDR::MAX_ALIGNMENT;
      int offset = curalign - tmpalign;
      if (offset < 0)
	offset += CDR::MAX_ALIGNMENT;
      tmp->rd_ptr (offset);
      tmp->wr_ptr (tmp->rd_ptr ());

      // grow the chain and set the current block.
      tmp->cont (this->current_->cont ());
      this->current_->cont (tmp);
    }
  this->current_ = this->current_->cont ();

  // Now we are ready to set buf..
  // recompute the position....
  buf = ptr_align_binary (this->current_->wr_ptr(), align);
  this->current_->wr_ptr (buf + size);
  return 0;
}

CORBA_Boolean
TAO_OutputCDR::write_1 (const CORBA::Octet* x)
{
  char* buf;
  if (this->adjust (1, buf) == 0)
    {
      *ACE_reinterpret_cast(CORBA::Octet*, buf) = *x;
      return 1;
    }

  return 0;
}

CORBA_Boolean
TAO_OutputCDR::write_2 (const CORBA::UShort* x)
{
  char* buf;
  if (this->adjust (CDR::SHORT_SIZE, buf) == 0)
    {
#if !defined (TAO_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast(CORBA::UShort*,buf) = *x;
      return 1;
#else
      if (!this->do_byte_swap_)
        {
          *ACE_reinterpret_cast(CORBA::UShort*,buf) = *x;
          return 1;
        }
      else
        {
          CDR::swap_2 (ACE_reinterpret_cast(char*,x), buf);
          return 1;
        }
#endif /* TAO_ENABLE_SWAP_ON_WRITE */
    }
  return 0;
}

CORBA_Boolean
TAO_OutputCDR::write_4 (const CORBA::ULong* x)
{
  char* buf;
  if (this->adjust (CDR::LONG_SIZE, buf) == 0)
    {
#if !defined (TAO_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast(CORBA::ULong*,buf) = *x;
      return 1;
#else
      if (!this->do_byte_swap_)
        {
          *ACE_reinterpret_cast(CORBA::ULong*,buf) = *x;
          return 1;
        }
      else
        {
          CDR::swap_4 (ACE_reinterpret_cast(char*,x), buf);
          return 1;
        }
#endif /* TAO_ENABLE_SWAP_ON_WRITE */
    }
  return 0;
}

CORBA_Boolean
TAO_OutputCDR::write_8 (const CORBA::ULongLong* x)
{
  char* buf;
  if (this->adjust (CDR::LONGLONG_SIZE, buf) == 0)
    {
#if !defined (TAO_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast(CORBA::ULongLong*,buf) = *x;
      return 1;
#else
      if (!this->do_byte_swap_)
        {
          *ACE_reinterpret_cast(CORBA::ULongLong*,buf) = *x;
          return 1;
        }
      else
        {
          CDR::swap_8 (ACE_reinterpret_cast(char*,x), buf);
          return 1;
        }
#endif /* TAO_ENABLE_SWAP_ON_WRITE */
    }
  return 0;
}

CORBA_Boolean
TAO_OutputCDR::write_16 (const CORBA::LongDouble* x)
{
  char* buf;
  if (this->adjust (CDR::LONGDOUBLE_SIZE, CDR::LONGDOUBLE_ALIGN, buf) == 0)
    {
#if !defined (TAO_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast(CORBA::LongDouble*,buf) = *x;
      return 1;
#else
      if (!this->do_byte_swap_)
        {
          *ACE_reinterpret_cast(CORBA::LongDouble*,buf) = *x;
          return 1;
        }
      else
        {
          CDR::swap_16 (ACE_reinterpret_cast(char*,x), buf);
          return 1;
        }
#endif /* TAO_ENABLE_SWAP_ON_WRITE */
    }
  return 0;
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
      return 1;
#else
      if (!this->do_byte_swap_)
        {
          ACE_OS::memcpy (buf, x, size*length);
          return 1;
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
              return 0;
            }
          char *source = ACE_reinterpret_cast(char*,x);
          char *end = source + size*length;
          for (; source != end; source += size, buf += size)
            {
              (*swapper)(source, buf);
            }
        }
#endif /* TAO_ENABLE_SWAP_ON_WRITE */
    }
  this->good_bit_ = 0;
  return 0;
}

CORBA_Boolean
TAO_OutputCDR::write_string (CORBA::ULong len,
			     const CORBA::Char *x)
{
  if (len != 0)
    {
      if (this->write_ulong (len + 1))
        {
          return this->write_char_array (x, len + 1);
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
  return 0;
}

CORBA_Boolean
TAO_OutputCDR::write_wstring (CORBA::ULong len,
			      const CORBA::WChar *x)
{
  if (x != 0)
    {
      if (this->write_ulong (len + 1))
        {
          return this->write_wchar_array (x, len + 1);
        }
    }
  else
    {
      if (this->write_ulong (1))
        {
          return this->write_wchar (0);
        }
    }
  return 0;
}

CORBA_Boolean
TAO_OutputCDR::write_octet_array_mb (const ACE_Message_Block* mb)
{
  // If the buffer is small and it fits in the current message
  // block it is be cheaper just to copy the buffer.
  const size_t memcpy_tradeoff =
    TAO_ORB_Core_instance ()->orb_params ()->cdr_memcpy_tradeoff ();

  for (const ACE_Message_Block* i = mb;
       i != 0;
       i = i->cont ())
    {
      size_t length = i->length ();

#if !defined (TAO_NO_COPY_OCTET_SEQUENCES)
      if (!this->write_array (i->rd_ptr (),
			      CDR::OCTET_SIZE,
			      CDR::OCTET_ALIGN,
			      length) )
	{
	  return 0;
	}
#else
      // If the mb does not own its data we are forced to make a copy.
      if (ACE_BIT_ENABLED (i->flags (),
			   ACE_Message_Block::DONT_DELETE))
	{
	  if (! this->write_array (i->rd_ptr (),
				   CDR::OCTET_SIZE,
				   CDR::OCTET_ALIGN,
				   length))
	    {
	      return 0;
	    }
	  continue;
	}

      if (length < memcpy_tradeoff
	  && this->current_->wr_ptr () + length < this->current_->end ())
	{
	  if (! this->write_array (i->rd_ptr (),
				   CDR::OCTET_SIZE,
				   CDR::OCTET_ALIGN,
				   length) )
	    {
	      return 0;
	    }
	  continue;
	}

      ACE_Message_Block* cont =
        new ACE_Message_Block (i->data_block ()->duplicate ());
      if (cont != 0)
	{
	  cont->cont (this->current_->cont ());
	  this->current_->cont (cont);
	  this->current_ = cont;
          cont->rd_ptr (i->rd_ptr ());
          cont->wr_ptr (i->wr_ptr ());
	}
      else
	{
	  this->good_bit_ = 0;
	  return 0;
	}
#endif /* TAO_NO_COPY_OCTET_SEQUENCES */
    }
  return 1;
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
  : start_ (buf, bufsiz),
    factory_ (factory),
    do_byte_swap_ (byte_order != TAO_ENCAP_BYTE_ORDER),
    good_bit_ (1)
{
  this->start_.wr_ptr (bufsiz);
}

TAO_InputCDR::TAO_InputCDR (size_t bufsiz,
                            int byte_order,
                            TAO_Marshal_Factory *factory)
  : start_ (bufsiz),
    factory_ (factory),
    do_byte_swap_ (byte_order != TAO_ENCAP_BYTE_ORDER),
    good_bit_ (1)
{
}

TAO_InputCDR::TAO_InputCDR (ACE_Message_Block *data,
                            int byte_order,
                            TAO_Marshal_Factory *factory)
  :  start_ (data->data_block ()->duplicate ()),
     factory_ (factory),
     do_byte_swap_ (byte_order != TAO_ENCAP_BYTE_ORDER),
     good_bit_ (1)
{
  this->start_.rd_ptr (data->rd_ptr ());
  this->start_.wr_ptr (data->wr_ptr ());
}

TAO_InputCDR::TAO_InputCDR (ACE_Data_Block *data,
                            int byte_order,
                            TAO_Marshal_Factory *factory)
  :  start_ (data),
     factory_ (factory),
     do_byte_swap_ (byte_order != TAO_ENCAP_BYTE_ORDER),
     good_bit_ (1)
{
}

TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs,
                            size_t size,
                            CORBA::Long offset)
  : start_ (rhs.start_.data_block ()->duplicate ()),
    factory_ (rhs.factory_),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1)
{
  char* newpos = rhs.start_.rd_ptr() + offset;
  if (this->start_.base () <= newpos
      && newpos <= this->start_.end ()
      && newpos + size <= this->start_.end ())
    {
      this->start_.rd_ptr (newpos);
      this->start_.wr_ptr (newpos + size);
    }
  else
    {
      this->good_bit_ = 0;
    }
}

TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs,
                            size_t size)
  : start_ (rhs.start_.data_block ()->duplicate ()),
    factory_ (rhs.factory_),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1)
{
  char* newpos = rhs.start_.rd_ptr();
  if (this->start_.base () <= newpos
      && newpos <= this->start_.end ()
      && newpos + size <= this->start_.end ())
    {
      // Notice that ACE_Message_Block::duplicate may leave the
      // wr_ptr() with a higher value that what we actually want.
      this->start_.rd_ptr (newpos);
      this->start_.wr_ptr (newpos + size);

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
  : start_ (rhs.start_.data_block ()->duplicate ()),
    factory_ (rhs.factory_),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1)
{
  this->start_.rd_ptr (rhs.start_.rd_ptr ());
  this->start_.wr_ptr (rhs.start_.wr_ptr ());
}

TAO_InputCDR&
TAO_InputCDR::operator= (const TAO_InputCDR& rhs)
{
  if (this != &rhs)
    {
      this->start_.data_block (rhs.start_.data_block ()->duplicate ());
      this->start_.rd_ptr (rhs.start_.rd_ptr ());
      this->start_.wr_ptr (rhs.start_.wr_ptr ());
      this->factory_ = rhs.factory_;
      this->do_byte_swap_ = rhs.do_byte_swap_;
      this->good_bit_ = 1;
    }
  return *this;
}

TAO_InputCDR::TAO_InputCDR (const TAO_OutputCDR& rhs,
                            ACE_Allocator* buffer_allocator,
                            ACE_Allocator* data_block_allocator)
  : start_ (rhs.total_length () + CDR::MAX_ALIGNMENT,
            ACE_Message_Block::MB_DATA,
            0,
            0,
            buffer_allocator,
            0,
            0,
            ACE_Time_Value::zero,
            ACE_Time_Value::max_time,
            data_block_allocator),
    factory_ (rhs.factory_),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1)
{
  CDR::mb_align (&this->start_);
  for (const ACE_Message_Block *i = rhs.begin ();
       i != rhs.end ();
       i = i->cont ())
    this->start_.copy (i->rd_ptr (), i->length ());
}

TAO_InputCDR::~TAO_InputCDR (void)
{
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
        return 1;
      CORBA::string_free (x);
    }
  x = 0;
  return 0;
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
        return 1;

      CORBA::wstring_free (x);
    }
  x = 0;
  return 0;
}

ACE_INLINE char*
TAO_InputCDR::end (void)
{
  return this->start_.end ();
}

ACE_INLINE void
TAO_InputCDR::rd_ptr (size_t offset)
{
  this->start_.rd_ptr (offset);
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
      this->start_.rd_ptr (end);
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
      this->start_.rd_ptr (1);
      return 1;
    }

  this->good_bit_ = 0;
  return 0;
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
      return 1;
    }
  return 0;
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
      return 1;
    }
  return 0;
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
      return 1;
    }
  return 0;
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
      return 1;
    }
  return 0;
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
          char *target = ACE_reinterpret_cast(char*,x);
          char *end = target + size*length;
          switch (size)
            {
            case 2:
	      for (; target != end; target += size, buf += size)
		{
		  CDR::swap_2(buf, target);
		}
              break;
            case 4:
	      for (; target != end; target += size, buf += size)
		{
		  CDR::swap_4(buf, target);
		}
              break;
            case 8:
	      for (; target != end; target += size, buf += size)
		{
		  CDR::swap_8(buf, target);
		}
              break;
            case 16:
	      for (; target != end; target += size, buf += size)
		{
		  CDR::swap_16(buf, target);
		}
              break;
            default:
              // TODO: print something?
              this->good_bit_ = 0;
              return 0;
            }
        }
#else
      ACE_OS::memcpy (x, buf, size*length);
#endif /* TAO_DISABLE_SWAP_ON_READ */
      return this->good_bit_;
    }
  return 0;
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
          return 1;
        }
      this->good_bit_ = 0;
    }

  return 0;
}

CORBA_Boolean
TAO_InputCDR::skip_bytes (size_t len)
{
  if (this->rd_ptr () + len <= this->end ())
    {
      this->rd_ptr (len);
      return 1;
    }
  this->good_bit_ = 0;
  return 0;
}
