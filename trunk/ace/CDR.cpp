// CDR.cpp
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
// the fact that (for example) CORBA's Long is always four bytes long
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

#define ACE_BUILD_DLL
#include "ace/CDR.h"

#if !defined (__ACE_INLINE__)
# include "ace/CDR.i"
#endif /* ! __ACE_INLINE__ */

int
ACE_CDR::grow (ACE_Message_Block *mb, size_t minsize)
{
  // Calculate the new buffer's length; if growing for encode, we
  // don't grow in "small" chunks because of the cost.

  size_t size = mb->size();
  size_t newsize = size;

  if (minsize == 0)
    {
      // TODO The growth strategy should be controlled using
      // the ORB parameters....
      if (newsize == 0)
        newsize = ACE_CDR::DEFAULT_BUFSIZE;
      else if (size < ACE_CDR::EXP_GROWTH_MAX)
        newsize *= 2;
      else
        newsize += ACE_CDR::LINEAR_GROWTH_CHUNK;
    }
  else if (minsize + ACE_CDR::MAX_ALIGNMENT <= size)
    return 0;
  else
    {
      if (newsize == 0)
	newsize = ACE_CDR::DEFAULT_BUFSIZE;

      while (newsize < minsize + ACE_CDR::MAX_ALIGNMENT)
        {
          if (newsize < ACE_CDR::EXP_GROWTH_MAX)
            newsize *= 2;
          else
            newsize += ACE_CDR::LINEAR_GROWTH_CHUNK;
        }
    }

  ACE_Message_Block tmp (newsize);

  ACE_CDR::mb_align (&tmp);

  tmp.copy (mb->rd_ptr (), mb->length());
  mb->data_block (tmp.data_block ()->duplicate ());
  mb->rd_ptr (tmp.rd_ptr ());
  mb->wr_ptr (tmp.wr_ptr ());

  return 0;
}

// ****************************************************************
// ****************************************************************
// ****************************************************************

ACE_OutputCDR::ACE_OutputCDR (size_t size,
                              int byte_order,
			      ACE_Allocator *buffer_allocator,
			      ACE_Allocator *data_block_allocator,
                              size_t memcpy_tradeoff)
  :  start_ (size ? size : ACE_CDR::DEFAULT_BUFSIZE + ACE_CDR::MAX_ALIGNMENT,
             ACE_Message_Block::MB_DATA,
	     0, 
             0,
	     buffer_allocator,
	     0,
	     0,
	     ACE_Time_Value::zero,
	     ACE_Time_Value::max_time,
	     data_block_allocator),
     do_byte_swap_ (byte_order != STREAM_BYTE_ORDER),
     good_bit_ (1),
     memcpy_tradeoff_ (memcpy_tradeoff)
{
  ACE_CDR::mb_align (&this->start_);
  this->current_ = &this->start_;
}

ACE_OutputCDR::ACE_OutputCDR (char *data, size_t size,
                              int byte_order,
			      ACE_Allocator *buffer_allocator,
			      ACE_Allocator *data_block_allocator,
                              size_t memcpy_tradeoff)
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
     do_byte_swap_ (byte_order != STREAM_BYTE_ORDER),
     good_bit_ (1),
     memcpy_tradeoff_ (memcpy_tradeoff)
{
  // We cannot trust the buffer to be properly aligned
  ACE_CDR::mb_align (&this->start_);
  this->current_ = &this->start_;
}

ACE_OutputCDR::ACE_OutputCDR (ACE_Message_Block *data,
                              int byte_order,
                              size_t memcpy_tradeoff)
  :  start_ (data->data_block ()->duplicate ()),
     do_byte_swap_ (byte_order != STREAM_BYTE_ORDER),
     good_bit_ (1),
     memcpy_tradeoff_ (memcpy_tradeoff)
{ 
  // We cannot trust the buffer to be properly aligned
  ACE_CDR::mb_align (&this->start_);
  this->current_ = &this->start_;
}

ACE_OutputCDR::~ACE_OutputCDR (void)
{
  if (this->start_.cont () != 0)
    {
      ACE_Message_Block::release (this->start_.cont ());
      this->start_.cont (0);
    }
  this->current_ = 0;
}

void
ACE_OutputCDR::reset (void)
{
  this->current_ = &this->start_;
  ACE_CDR::mb_align (&this->start_);
}

size_t
ACE_OutputCDR::total_length (void) const
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
ACE_OutputCDR::grow_and_adjust (size_t size, size_t align, char*& buf)
{
  if (this->current_->cont () == 0
      || this->current_->cont ()->size () < size + ACE_CDR::MAX_ALIGNMENT)
    {
      // Allocate the next block, it must be large enough.
      size_t block_size = ACE_CDR::DEFAULT_BUFSIZE;
      while (block_size < size + ACE_CDR::MAX_ALIGNMENT)
	{
	  if (block_size < ACE_CDR::EXP_GROWTH_MAX)
	    block_size *= 2;
	  else
	    block_size += ACE_CDR::LINEAR_GROWTH_CHUNK;
	}
      this->good_bit_ = 0;
      ACE_Message_Block* tmp;
      ACE_NEW_RETURN (tmp,
		      ACE_Message_Block (block_size,
					 ACE_Message_Block::MB_DATA,
					 0, 0,
					 this->current_->data_block ()->allocator_strategy (),
					 0,
					 0,
					 ACE_Time_Value::zero,
					 ACE_Time_Value::max_time,
					 this->current_->data_block ()->data_block_allocator ()),
		      -1);
      this->good_bit_ = 1;

      // The new block must start with the same alignment as the
      // previous block finished.
      ptr_arith_t tmpalign =
	ptr_arith_t(tmp->wr_ptr ()) % ACE_CDR::MAX_ALIGNMENT;
      ptr_arith_t curalign =
	ptr_arith_t(this->current_->wr_ptr ()) % ACE_CDR::MAX_ALIGNMENT;
      int offset = curalign - tmpalign;
      if (offset < 0)
	offset += ACE_CDR::MAX_ALIGNMENT;
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

CDR_Boolean
ACE_OutputCDR::write_string (int len, 
                             const char *x)
{
  if (len != 0)
    {
      if (this->write_ulong (len + 1) != -1)
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

CDR_Boolean
ACE_OutputCDR::write_wstring (unsigned long len,
			      const ACE_OS::WChar *x)
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

CDR_Boolean
ACE_OutputCDR::write_octet_array_mb (const ACE_Message_Block* mb)
{
  // If the buffer is small and it fits in the current message
  // block it is be cheaper just to copy the buffer.
  for (const ACE_Message_Block* i = mb;
       i != 0;
       i = i->cont ())
    {
      size_t length = i->length ();

      // If the mb does not own its data we are forced to make a copy.
      if (ACE_BIT_ENABLED (i->flags (),
			   ACE_Message_Block::DONT_DELETE))
	{
	  if (! this->write_array (i->rd_ptr (),
				   ACE_CDR::OCTET_SIZE,
				   ACE_CDR::OCTET_ALIGN,
				   length))
	    {
	      return 0;
	    }
	  continue;
	}

      if (length < this->memcpy_tradeoff_
	  && this->current_->wr_ptr () + length < this->current_->end ())
	{
	  if (! this->write_array (i->rd_ptr (),
				   ACE_CDR::OCTET_SIZE,
				   ACE_CDR::OCTET_ALIGN,
				   length) )
	    {
	      return 0;
	    }
	  continue;
	}

      ACE_Message_Block* cont = ACE_Message_Block::duplicate (i);
      if (cont != 0)
	{
	  cont->cont (this->current_->cont ());
	  this->current_->cont (cont);
	  this->current_ = cont;
	}
      else
	{
	  this->good_bit_ = 0;
	  return 0;
	}
    }
  return 1;
}

CDR_Boolean
ACE_OutputCDR::write_1 (const char *x)
{
  char *buf;
  if (this->adjust (1, buf) == 0)
    {
      *ACE_reinterpret_cast(CDR_Octet*, buf) = *x;
      return 1;
    }

  return 0;
}

CDR_Boolean
ACE_OutputCDR::write_2 (const ACE_UINT16 *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::SHORT_SIZE, buf) == 0)
    {
      if (!do_byte_swap_)
	{
	  *ACE_reinterpret_cast (ACE_UINT16 *, buf) = *x;
	  return 1;
	}
      else
	{
	  ACE_CDR::swap_2 (ACE_reinterpret_cast (char*, x), buf);
	  return 1;
	}
    }

  return 0;
}

CDR_Boolean
ACE_OutputCDR::write_4 (const ACE_UINT32 *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::LONG_SIZE, buf) == 0)
    {
      if (!this->do_byte_swap_)
	{
	  *ACE_reinterpret_cast (ACE_UINT32 *, buf) = *x;
	  return 1;
	}
      else
	{
	  ACE_CDR::swap_4 (ACE_reinterpret_cast (char*, x), buf);
	  return 1;
	}
    }

  return 0;
}

CDR_Boolean
ACE_OutputCDR::write_8 (const ACE_UINT64 *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::LONGLONG_SIZE, buf) == 0)
    {
      if (!this->do_byte_swap_)
	{
	  *ACE_reinterpret_cast (ACE_UINT64 *, buf) = *x;
	  return 1;
	}
      else
	{
	  ACE_CDR::swap_8 (ACE_reinterpret_cast (char*, x), buf);
	  return 1;
	}
    }

  return 0;
}
      
#  if   ACE_SIZEOF_LONG_DOUBLE == 16
CDR_Boolean
ACE_OutputCDR::write_16 (const long double* x)
{
  char* buf;
  if (this->adjust (CDR::LONGDOUBLE_SIZE, CDR::LONGDOUBLE_ALIGN, buf) == 0)
    {
      if (!this->do_byte_swap_)
        {
          *ACE_reinterpret_cast (long double*, buf) = *x;
          return 1;
        }
      else
        {
          CDR::swap_16 (ACE_reinterpret_cast (char*, x), buf);
          return 1;
        }
    }
  return 0;
}
#  endif /* ACE_SIZEOF_LONG_DOUBLE == 16 */

CDR_Boolean
ACE_OutputCDR::write_array (const void *x,
			    size_t size,
			    size_t align,
			    unsigned long length)
{
  char *buf;
  if (this->adjust (size * length, align, buf) == 0)
    {
      if (!this->do_byte_swap_)
        {
          ACE_OS::memcpy (buf, x, size*length);
          return 1;
        }
      else
        {
          // I cannot see any fast way out of this....
          typedef void (*SWAPPER)(const char *, char *);
          SWAPPER swapper;
          switch (size)
            {
            case 2:
              swapper = ACE_CDR::swap_2;
              break;
            case 4:
              swapper = ACE_CDR::swap_4;
              break;
            case 8:
              swapper = ACE_CDR::swap_8;
              break;
            case 16:
              swapper = ACE_CDR::swap_16;
              break;
            default:
              // TODO: print something?
              this->good_bit_ = 0;
              return 0;
            }
	  
          char *source = ACE_reinterpret_cast (char *, x);
          char *end = source + size*length;

          for (; source != end; source += size, buf += size)
            {
              (*swapper)(source, buf);
            }

          return 0;
        }
    }
  this->good_bit_ = 0;
  return 0;
}

CDR_Boolean
ACE_OutputCDR::write_boolean_array (CDR_Boolean* x,
                                    unsigned long length)
{
  // It is hard to optimize this, the spec requires that on the wire
  // booleans be represented as a byte with value 0 or 1, but in
  // memoery it is possible (though very unlikely) that a boolean has
  // a non-zero value (different from 1).
  // We resort to a simple loop.
  const CDR_Boolean* end = x + length;
  for (const CDR_Boolean* i = x; i != end && this->good_bit (); ++i)
    {
      this->write_boolean (*i);
    }
  return this->good_bit ();
}

// ****************************************************************

ACE_InputCDR::ACE_InputCDR (const char *buf, 
                            size_t bufsiz,
                            int byte_order)
  : start_ (buf, bufsiz),
    do_byte_swap_ (byte_order != STREAM_BYTE_ORDER),
    good_bit_ (1)
{
  this->start_.wr_ptr (bufsiz);
}

ACE_InputCDR::ACE_InputCDR (size_t bufsiz,
                            int byte_order)
  : start_ (bufsiz),
    do_byte_swap_ (byte_order != STREAM_BYTE_ORDER),
    good_bit_ (1)
{
}

ACE_InputCDR::ACE_InputCDR (ACE_Message_Block *data,
                            int byte_order)
  :  start_ (data->data_block ()->duplicate ()),
     do_byte_swap_ (byte_order != STREAM_BYTE_ORDER),
     good_bit_ (1)
{
  this->start_.rd_ptr (data->rd_ptr ());
  this->start_.wr_ptr (data->wr_ptr ());
}

ACE_InputCDR::ACE_InputCDR (ACE_Data_Block *data,
                            int byte_order)
  :  start_ (data),
     do_byte_swap_ (byte_order != STREAM_BYTE_ORDER),
     good_bit_ (1)
{
}

ACE_InputCDR::ACE_InputCDR (const ACE_InputCDR& rhs,
                            size_t size,
                            long offset)
  : start_ (rhs.start_.data_block ()->duplicate ()),
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

ACE_InputCDR::ACE_InputCDR (const ACE_InputCDR& rhs,
                            size_t size)
  : start_ (rhs.start_.data_block ()->duplicate ()),
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

      CDR_Octet byte_order;
      this->read_octet (byte_order);
      this->do_byte_swap_ = (byte_order != STREAM_BYTE_ORDER);
    }
  else
    {
      this->good_bit_ = 0;
    }
}

ACE_InputCDR::ACE_InputCDR (const ACE_InputCDR& rhs)
  : start_ (rhs.start_.data_block ()->duplicate ()),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1)
{
  this->start_.rd_ptr (rhs.start_.rd_ptr ());
  this->start_.wr_ptr (rhs.start_.wr_ptr ());
}

ACE_InputCDR&
ACE_InputCDR::operator= (const ACE_InputCDR& rhs)
{
  if (this != &rhs)
    {
      this->start_.data_block (rhs.start_.data_block ()->duplicate ());
      this->start_.rd_ptr (rhs.start_.rd_ptr ());
      this->start_.wr_ptr (rhs.start_.wr_ptr ());
      this->do_byte_swap_ = rhs.do_byte_swap_;
      this->good_bit_ = 1;
    }
  return *this;
}

ACE_InputCDR::ACE_InputCDR (const ACE_OutputCDR& rhs,
                            ACE_Allocator* buffer_allocator,
                            ACE_Allocator* data_block_allocator)
  : start_ (rhs.total_length () + ACE_CDR::MAX_ALIGNMENT,
            ACE_Message_Block::MB_DATA,
            0,
            0,
            buffer_allocator,
            0,
            0,
            ACE_Time_Value::zero,
            ACE_Time_Value::max_time,
            data_block_allocator),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1)
{
  ACE_CDR::mb_align (&this->start_);
  for (const ACE_Message_Block *i = rhs.begin ();
       i != rhs.end ();
       i = i->cont ())
    this->start_.copy (i->rd_ptr (), i->length ());
}

ACE_InputCDR::~ACE_InputCDR (void)
{
}

CDR_Boolean
ACE_InputCDR::read_string (char *&x)
{
  u_long len;

  this->read_ulong (len);
  if (len > 0)
    {
      ACE_NEW_RETURN (x, char[len], 0);
      if (this->read_char_array (x, len))
        return 1;
      delete x;
    }

  x = 0;
  return 0;
}

CDR_Boolean
ACE_InputCDR::read_string (ACE_CString &x)
{
  char *data;
  if (this->read_string (data))
    {
      x = data;
      delete [] data;
      return 1;
    }

  x = "";
  return 0;
}

CDR_Boolean
ACE_InputCDR::read_wstring (ACE_OS::WChar*& x)
{
  u_long len;
  this->read_ulong (len);
  if (this->good_bit())
    {
      ACE_NEW_RETURN (x, ACE_OS::WChar[len], 0);
      if (this->read_wchar_array (x, len))
        return 1;

      delete x;
    }
  x = 0;
  return 0;
}

CDR_Boolean
ACE_InputCDR::read_array (void* x,
			  size_t size,
			  size_t align,
			  unsigned long length)
{
  char* buf;
  if (this->adjust (size * length, align, buf) == 0)
    {
      if (!this->do_byte_swap_ || size == 1)
        {
          ACE_OS::memcpy (x, buf, size*length);
        }
      else
        {
          // I cannot see any fast way out of this....
          char *target = ACE_reinterpret_cast (char*, x);
          char *end = target + size*length;
          switch (size)
            {
            case 2:
	      for (; target != end; target += size, buf += size)
		{
		  ACE_CDR::swap_2 (buf, target);
		}
              break;
            case 4:
	      for (; target != end; target += size, buf += size)
		{
		  ACE_CDR::swap_4 (buf, target);
		}
              break;
            case 8:
	      for (; target != end; target += size, buf += size)
		{
		  ACE_CDR::swap_8 (buf, target);
		}
              break;	      
            case 16:
	      for (; target != end; target += size, buf += size)
		{
		  ACE_CDR::swap_16 (buf, target);
		}
              break;
            default:
              // TODO: print something?
              this->good_bit_ = 0;
              return 0;
            }
        }
      return this->good_bit_;
    }
  return 0;
}

CDR_Boolean
ACE_InputCDR::read_1 (CDR_Octet *x)
{
  if (this->rd_ptr () < this->end ())
    {
      *x = *ACE_reinterpret_cast (CDR_Octet*,this->rd_ptr());
      this->start_.rd_ptr (1);
      return 1;
    }

  this->good_bit_ = 0;
  return 0;
}

CDR_Boolean
ACE_InputCDR::read_2 (ACE_UINT16 *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::SHORT_SIZE, buf) == 0)
    {
      if (!this->do_byte_swap_)
	{
	  *x = *ACE_reinterpret_cast (ACE_UINT16*, buf);
	}
      else
	{
	  ACE_CDR::swap_2 (buf, ACE_reinterpret_cast (char*, x));
	}
      return 1;
    }
  return 0;
}

CDR_Boolean
ACE_InputCDR::read_4 (ACE_UINT32 *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::LONG_SIZE, buf) == 0)
    {
      if (!this->do_byte_swap_)
	{
	  *x = *ACE_reinterpret_cast (ACE_UINT32*, buf);
	}
      else
	{
	  ACE_CDR::swap_4 (buf, ACE_reinterpret_cast (char*, x));
	}
      return 1;
    }
  return 0;
}

CDR_Boolean
ACE_InputCDR::read_8 (ACE_UINT64 *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::LONGLONG_SIZE, buf) == 0)
    {
      if (!this->do_byte_swap_)
	{
	  *x = *ACE_reinterpret_cast (ACE_UINT64 *, buf);
	}
      else
	{
	  ACE_CDR::swap_8 (buf, ACE_reinterpret_cast (char*, x));
	}
      return 1;
    }
  return 0;
}

#  if   ACE_SIZEOF_LONG_DOUBLE == 16
CDR_Boolean
ACE_InputCDR::read_16 (long double *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::LONGLONG_SIZE, 
                    ACE_CDR::LONGLONG_ALIGN, 
                    buf) == 0)
    {
      if (!this->do_byte_swap_)
	{
	  *x = *ACE_reinterpret_cast (long double *, buf);
	}
      else
	{
	  ACE_CDR::swap_16 (buf, ACE_reinterpret_cast (char*, x));
	}
      return 1;
    }
  return 0;
}
#  endif /* ACE_SIZEOF_LONG_DOUBLE == 16 */




