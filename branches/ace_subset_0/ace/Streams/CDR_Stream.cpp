// $Id$

#include "ace/Streams/CDR_Stream.h"
#include "ace/Utils/SString.h"

#if !defined (__ACE_INLINE__)
# include "ace/Streams/CDR_Stream.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, CDR_Stream, "$Id$")



// ****************************************************************

ACE_OutputCDR::ACE_OutputCDR (size_t size,
                              int byte_order,
                              ACE_Allocator *buffer_allocator,
                              ACE_Allocator *data_block_allocator,
                              ACE_Allocator *message_block_allocator,
                              size_t memcpy_tradeoff,
                              ACE_CDR::Octet major_version,
                              ACE_CDR::Octet minor_version)
  :  start_ (size ? size : ACE_CDR::DEFAULT_BUFSIZE + ACE_CDR::MAX_ALIGNMENT,
             ACE_Message_Block::MB_DATA,
             0,
             0,
             buffer_allocator,
             0,
             0,
             ACE_Time_Value::zero,
             ACE_Time_Value::max_time,
             data_block_allocator,
             message_block_allocator),
     current_is_writable_ (1),
     current_alignment_ (0),
     do_byte_swap_ (byte_order != ACE_CDR_BYTE_ORDER),
     good_bit_ (1),
     memcpy_tradeoff_ (memcpy_tradeoff),
     major_version_ (major_version),
     minor_version_ (minor_version),
     char_translator_ (0),
     wchar_translator_ (0)
{
  ACE_CDR::mb_align (&this->start_);
  this->current_ = &this->start_;
}

ACE_OutputCDR::ACE_OutputCDR (char *data, size_t size,
                              int byte_order,
                              ACE_Allocator *buffer_allocator,
                              ACE_Allocator *data_block_allocator,
                              ACE_Allocator *message_block_allocator,
                              size_t memcpy_tradeoff,
                              ACE_CDR::Octet major_version,
                              ACE_CDR::Octet minor_version)
  :  start_ (size,
             ACE_Message_Block::MB_DATA,
             0,
             data,
             buffer_allocator,
             0,
             0,
             ACE_Time_Value::zero,
             ACE_Time_Value::max_time,
             data_block_allocator,
             message_block_allocator),
     current_is_writable_ (1),
     current_alignment_ (0),
     do_byte_swap_ (byte_order != ACE_CDR_BYTE_ORDER),
     good_bit_ (1),
     memcpy_tradeoff_ (memcpy_tradeoff),
     major_version_ (major_version),
     minor_version_ (minor_version),
     char_translator_ (0),
     wchar_translator_ (0)
{
  // We cannot trust the buffer to be properly aligned
  ACE_CDR::mb_align (&this->start_);
  this->current_ = &this->start_;
}

ACE_OutputCDR::ACE_OutputCDR (ACE_Message_Block *data,
                              int byte_order,
                              size_t memcpy_tradeoff,
                              ACE_CDR::Octet major_version,
                              ACE_CDR::Octet minor_version)
  :  start_ (data->data_block ()->duplicate ()),
     current_is_writable_ (1),
     current_alignment_ (0),
     do_byte_swap_ (byte_order != ACE_CDR_BYTE_ORDER),
     good_bit_ (1),
     memcpy_tradeoff_ (memcpy_tradeoff),
     major_version_ (major_version),
     minor_version_ (minor_version),
     char_translator_ (0),
     wchar_translator_ (0)
{
  // We cannot trust the buffer to be properly aligned
  ACE_CDR::mb_align (&this->start_);
  this->current_ = &this->start_;
}

int
ACE_OutputCDR::grow_and_adjust (size_t size,
                                size_t align,
                                char*& buf)
{
  if (!this->current_is_writable_
      || this->current_->cont () == 0
      || this->current_->cont ()->size () < size + ACE_CDR::MAX_ALIGNMENT)
    {
      // Calculate the new buffer's length; if growing for encode, we
      // don't grow in "small" chunks because of the cost.
      size_t cursize = this->current_->size ();
      if (this->current_->cont () != 0)
        cursize = this->current_->cont ()->size ();

      size_t minsize = size + ACE_CDR::MAX_ALIGNMENT;
      // Make sure that there is enough room for <minsize> bytes, but
      // also make it bigger than whatever our current size is.
      if (minsize < cursize)
        {
          minsize = cursize;
        }

      size_t newsize =
        ACE_CDR::next_size (minsize);

      this->good_bit_ = 0;
      ACE_Message_Block* tmp;
      ACE_NEW_RETURN (tmp,
                      ACE_Message_Block (newsize,
                                         ACE_Message_Block::MB_DATA,
                                         0,
                                         0,
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
        ptr_arith_t(tmp->rd_ptr ()) % ACE_CDR::MAX_ALIGNMENT;
      ptr_arith_t curalign =
        ptr_arith_t(this->current_alignment_) % ACE_CDR::MAX_ALIGNMENT;
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
  this->current_is_writable_ = 1;

  return this->adjust (size, align, buf);
}

ACE_CDR::Boolean
ACE_OutputCDR::write_wchar (ACE_CDR::WChar x)
{
  if (ACE_static_cast (ACE_CDR::Short, major_version_) == 1
          && ACE_static_cast (ACE_CDR::Short, minor_version_) == 2)
    {
      ACE_CDR::Octet len = ACE_static_cast (ACE_CDR::Octet, sizeof(x));
      if (this->write_1 (&len))
        return this->write_octet_array (ACE_reinterpret_cast
                                         (const ACE_CDR::Octet*, &x),
                                         ACE_static_cast (ACE_CDR::ULong, len));
    }
  else
    if (this->wchar_translator_ == 0)
      return this->write_2 (ACE_reinterpret_cast (const ACE_CDR::UShort*, &x));
  return this->wchar_translator_->write_wchar (*this, x);
}

ACE_CDR::Boolean
ACE_OutputCDR::write_string (ACE_CDR::ULong len,
                             const char *x)
{
  // @@ This is a slight violation of "Optimize for the common case",
  // i.e. normally the translator will be 0, but OTOH the code is
  // smaller and should be better for the cache ;-) ;-)
  if (this->char_translator_ != 0)
    return this->char_translator_->write_string (*this, len, x);

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

ACE_CDR::Boolean
ACE_OutputCDR::write_string (const ACE_CString &x)
{
  // @@ Leave this method in here, not the `.i' file so that we don't
  //    have to unnecessarily pull in the `ace/SString.h' header.
  return this->write_string (x.length(), x.c_str());
}

ACE_CDR::Boolean
ACE_OutputCDR::write_wstring (ACE_CDR::ULong len,
                              const ACE_CDR::WChar *x)
{
  // @@ This is a slight violation of "Optimize for the common case",
  // i.e. normally the translator will be 0, but OTOH the code is
  // smaller and should be better for the cache ;-) ;-)
  // What do we do for GIOP 1.2???
  if (this->wchar_translator_ != 0)
    return this->wchar_translator_->write_wstring (*this, len, x);

  if (ACE_static_cast (ACE_CDR::Short, this->major_version_) == 1
          && ACE_static_cast (ACE_CDR::Short, this->minor_version_) == 2)
    {
      if (x != 0)
        {
          //In GIOP 1.2 the length field contains the number of bytes
          //the wstring occupies rather than number of wchars
          //Taking sizeof might not be a good way! This is a temporary fix.
          if (this->write_ulong (sizeof(ACE_CDR::WChar)*len))
            {
              return this->write_wchar_array (x, len);
            }
        }
      else
        {
          //In GIOP 1.2 zero length wstrings are legal
          return this->write_ulong (0);
        }
    }

  else
    if (x != 0)
      {
        if (this->write_ulong (len + 1))
          return this->write_wchar_array (x, len + 1);
      }
    else
      {
        if (this->write_ulong (1))
          return this->write_wchar (0);
      }
  return 0;
}

ACE_CDR::Boolean
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
                                   length))
            {
              return 0;
            }
          continue;
        }

      ACE_Message_Block* cont;
      this->good_bit_ = 0;
      ACE_NEW_RETURN (cont,
                      ACE_Message_Block (i->data_block ()->duplicate ()),
                      0);
      this->good_bit_ = 1;

      if (cont != 0)
        {
          if (this->current_->cont () != 0)
            ACE_Message_Block::release (this->current_->cont ());
          cont->rd_ptr (i->rd_ptr ());
          cont->wr_ptr (i->wr_ptr ());

          this->current_->cont (cont);
          this->current_ = cont;
          this->current_is_writable_ = 0;
          this->current_alignment_ =
            (this->current_alignment_ + cont->length ()) % ACE_CDR::MAX_ALIGNMENT;
        }
      else
        {
          this->good_bit_ = 0;
          return 0;
        }
    }
  return 1;
}

ACE_CDR::Boolean
ACE_OutputCDR::write_1 (const ACE_CDR::Octet *x)
{
  char *buf;
  if (this->adjust (1, buf) == 0)
    {
      *ACE_reinterpret_cast(ACE_CDR::Octet*, buf) = *x;
      return 1;
    }

  return 0;
}

ACE_CDR::Boolean
ACE_OutputCDR::write_2 (const ACE_CDR::UShort *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::SHORT_SIZE, buf) == 0)
    {
#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast(ACE_CDR::UShort*,buf) = *x;
      return 1;
#else
      if (!this->do_byte_swap_)
        {
          *ACE_reinterpret_cast (ACE_CDR::UShort *, buf) = *x;
          return 1;
        }
      else
        {
          ACE_CDR::swap_2 (ACE_reinterpret_cast (const char*, x), buf);
          return 1;
        }
#endif /* ACE_ENABLE_SWAP_ON_WRITE */
    }

  return 0;
}

ACE_CDR::Boolean
ACE_OutputCDR::write_4 (const ACE_CDR::ULong *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::LONG_SIZE, buf) == 0)
    {
#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast(ACE_CDR::ULong*,buf) = *x;
      return 1;
#else
      if (!this->do_byte_swap_)
        {
          *ACE_reinterpret_cast (ACE_CDR::ULong *, buf) = *x;
          return 1;
        }
      else
        {
          ACE_CDR::swap_4 (ACE_reinterpret_cast (const char*, x), buf);
          return 1;
        }
#endif /* ACE_ENABLE_SWAP_ON_WRITE */
    }

  return 0;
}

ACE_CDR::Boolean
ACE_OutputCDR::write_8 (const ACE_CDR::ULongLong *x)
{
  char *buf;

  if (this->adjust (ACE_CDR::LONGLONG_SIZE, buf) == 0)
    {
#if defined (__arm__)
      // Convert to Intel format (12345678 => 56781234)
      const char *orig = ACE_reinterpret_cast (const char *, x);
      char *target = buf;
      register ACE_UINT32 x =
        *ACE_reinterpret_cast (const ACE_UINT32 *, orig);
      register ACE_UINT32 y =
        *ACE_reinterpret_cast (const ACE_UINT32 *, orig + 4);
      *ACE_reinterpret_cast (ACE_UINT32 *, target) = y;
      *ACE_reinterpret_cast (ACE_UINT32 *, target + 4) = x;
      return 1;
#else
#  if !defined (ACE_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast (ACE_CDR::ULongLong *,buf) = *x;
      return 1;
#  else
      if (!this->do_byte_swap_)
        {
          *ACE_reinterpret_cast (ACE_CDR::ULongLong *, buf) = *x;
          return 1;
        }
      else
        {
          ACE_CDR::swap_8 (ACE_reinterpret_cast (const char*, x), buf);
          return 1;
        }
#  endif /* ACE_ENABLE_SWAP_ON_WRITE */
#endif /* !__arm__ */
     }

  return 0;
}

ACE_CDR::Boolean
ACE_OutputCDR::write_16 (const ACE_CDR::LongDouble *x)
{
  char* buf;
  if (this->adjust (ACE_CDR::LONGDOUBLE_SIZE,
                    ACE_CDR::LONGDOUBLE_ALIGN,
                    buf) == 0)
    {
#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast(ACE_CDR::LongDouble*,buf) = *x;
      return 1;
#else
      if (!this->do_byte_swap_)
        {
          *ACE_reinterpret_cast (ACE_CDR::LongDouble *, buf) = *x;
          return 1;
        }
      else
        {
          ACE_CDR::swap_16 (ACE_reinterpret_cast (const char*, x), buf);
          return 1;
        }
#endif /* ACE_ENABLE_SWAP_ON_WRITE */
    }

  return 0;
}

ACE_CDR::Boolean
ACE_OutputCDR::write_array (const void *x,
                            size_t size,
                            size_t align,
                            ACE_CDR::ULong length)
{
  if (length == 0)
    return 1;
  char *buf;
  if (this->adjust (size * length, align, buf) == 0)
    {
#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
      ACE_OS::memcpy (buf, x, size*length);
      return 1;
#else
      if (!this->do_byte_swap_ || size == 1)
        {
          ACE_OS::memcpy (buf, x, size*length);
          return 1;
        }
      else
        {
          const char *source = ACE_reinterpret_cast (const char *, x);
          switch (size)
            {
            case 2:
              ACE_CDR::swap_2_array (source, buf, length);
              return 1;
            case 4:
              ACE_CDR::swap_4_array (source, buf, length);
              return 1;
            case 8:
              ACE_CDR::swap_8_array (source, buf, length);
              return 1;
            case 16:
              ACE_CDR::swap_16_array (source, buf, length);
              return 1;
            default:
              // TODO: print something?
              this->good_bit_ = 0;
              return 0;
            }
        }
#endif /* ACE_ENABLE_SWAP_ON_WRITE */
    }
  this->good_bit_ = 0;
  return 0;
}

ACE_CDR::Boolean
ACE_OutputCDR::write_boolean_array (const ACE_CDR::Boolean* x,
                                    ACE_CDR::ULong length)
{
  // It is hard to optimize this, the spec requires that on the wire
  // booleans be represented as a byte with value 0 or 1, but in
  // memoery it is possible (though very unlikely) that a boolean has
  // a non-zero value (different from 1).
  // We resort to a simple loop.
  const ACE_CDR::Boolean* end = x + length;
  for (const ACE_CDR::Boolean* i = x; i != end && this->good_bit (); ++i)
    {
      this->write_boolean (*i);
    }
  return this->good_bit ();
}

// ****************************************************************

ACE_InputCDR::ACE_InputCDR (const char *buf,
                            size_t bufsiz,
                            int byte_order,
                            ACE_CDR::Octet major_version,
                            ACE_CDR::Octet minor_version)
  : start_ (buf, bufsiz),
    do_byte_swap_ (byte_order != ACE_CDR_BYTE_ORDER),
    good_bit_ (1),
    char_translator_ (0),
    wchar_translator_ (0),
    major_version_ (major_version),
    minor_version_ (minor_version)
{
  this->start_.wr_ptr (bufsiz);
}

ACE_InputCDR::ACE_InputCDR (size_t bufsiz,
                            int byte_order,
                            ACE_CDR::Octet major_version,
                            ACE_CDR::Octet minor_version)
  : start_ (bufsiz),
    do_byte_swap_ (byte_order != ACE_CDR_BYTE_ORDER),
    good_bit_ (1),
    char_translator_ (0),
    wchar_translator_ (0),
    major_version_ (major_version),
    minor_version_ (minor_version)
{
}

ACE_InputCDR::ACE_InputCDR (const ACE_Message_Block *data,
                            int byte_order,
                            ACE_CDR::Octet major_version,
                            ACE_CDR::Octet minor_version)
  : start_ (),
    good_bit_ (1),
    char_translator_ (0),
    wchar_translator_ (0),
    major_version_ (major_version),
    minor_version_ (minor_version)
{
  this->reset (data, byte_order);
}

ACE_InputCDR::ACE_InputCDR (ACE_Data_Block *data,
                            ACE_Message_Block::Message_Flags flag,
                            int byte_order,
                            ACE_CDR::Octet major_version,
                            ACE_CDR::Octet minor_version)
  : start_ (data, flag),
    do_byte_swap_ (byte_order != ACE_CDR_BYTE_ORDER),
    good_bit_ (1),
    char_translator_ (0),
    wchar_translator_ (0),
    major_version_ (major_version),
    minor_version_ (minor_version)
{
}

ACE_InputCDR::ACE_InputCDR (ACE_Data_Block *data,
                            ACE_Message_Block::Message_Flags flag,
                            size_t rd_pos,
                            size_t wr_pos,
                            int byte_order,
                            ACE_CDR::Octet major_version,
                            ACE_CDR::Octet minor_version)
  : start_ (data, flag),
    do_byte_swap_ (byte_order != ACE_CDR_BYTE_ORDER),
    good_bit_ (1),
    char_translator_ (0),
    wchar_translator_ (0),
    major_version_ (major_version),
    minor_version_ (minor_version)
{
  // Set the read pointer
  this->start_.rd_ptr (rd_pos);

  // Set the write pointer after doing a sanity check.
  char* wrpos = this->start_.base () + wr_pos;

  if (this->start_.end () >= wrpos)
    {
      this->start_.wr_ptr (wr_pos);
    }
}


ACE_InputCDR::ACE_InputCDR (const ACE_InputCDR& rhs,
                            size_t size,
                            ACE_CDR::Long offset)
  : start_ (rhs.start_,
            ACE_CDR::MAX_ALIGNMENT),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1),
    char_translator_ (0),
    wchar_translator_ (0),
    major_version_ (rhs.major_version_),
    minor_version_ (rhs.minor_version_)
{
  // Align the base pointer assuming that the incoming stream is also
  // aligned the way we are aligned
  char *incoming_start = ACE_ptr_align_binary (rhs.start_.base (),
                                               ACE_CDR::MAX_ALIGNMENT);

  size_t newpos =
    (rhs.start_.rd_ptr() - incoming_start)  + offset;

  if (newpos <= this->start_.space ()
      && newpos + size <= this->start_.space ())
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
  : start_ (rhs.start_,
            ACE_CDR::MAX_ALIGNMENT),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1),
    char_translator_ (0),
    wchar_translator_ (0),
    major_version_ (rhs.major_version_),
    minor_version_ (rhs.minor_version_)
{
  // Align the base pointer assuming that the incoming stream is also
  // aligned the way we are aligned
  char *incoming_start = ACE_ptr_align_binary (rhs.start_.base (),
                                               ACE_CDR::MAX_ALIGNMENT);

  size_t newpos =
    rhs.start_.rd_ptr() - incoming_start;

  if (newpos <= this->start_.space ()
      && newpos + size <= this->start_.space ())
    {
      // Notice that ACE_Message_Block::duplicate may leave the
      // wr_ptr() with a higher value that what we actually want.
      this->start_.rd_ptr (newpos);
      this->start_.wr_ptr (newpos + size);

      ACE_CDR::Octet byte_order;
      this->read_octet (byte_order);
      this->do_byte_swap_ = (byte_order != ACE_CDR_BYTE_ORDER);
    }
  else
    {
      this->good_bit_ = 0;
    }
}

ACE_InputCDR::ACE_InputCDR (const ACE_InputCDR& rhs)
  : start_ (rhs.start_,
            ACE_CDR::MAX_ALIGNMENT),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1),
    char_translator_ (rhs.char_translator_),
    wchar_translator_ (rhs.wchar_translator_),
    major_version_ (rhs.major_version_),
    minor_version_ (rhs.minor_version_)
{
  char *buf = ACE_ptr_align_binary (rhs.start_.base (),
                                    ACE_CDR::MAX_ALIGNMENT);

  size_t rd_offset = rhs.start_.rd_ptr () - buf;
  size_t wr_offset = rhs.start_.wr_ptr () - buf;
  this->start_.rd_ptr (rd_offset);
  this->start_.wr_ptr (wr_offset);
}

ACE_InputCDR::ACE_InputCDR (ACE_InputCDR::Transfer_Contents x)
  : start_ (x.rhs_.start_.data_block ()),
    do_byte_swap_ (x.rhs_.do_byte_swap_),
    good_bit_ (1),
    char_translator_ (x.rhs_.char_translator_),
    wchar_translator_ (x.rhs_.wchar_translator_),
    major_version_ (x.rhs_.major_version_),
    minor_version_ (x.rhs_.minor_version_)
{
  this->start_.rd_ptr (x.rhs_.start_.rd_ptr ());
  this->start_.wr_ptr (x.rhs_.start_.wr_ptr ());

  ACE_Data_Block* db = this->start_.data_block ()->clone_nocopy ();
  (void) x.rhs_.start_.replace_data_block (db);
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
      this->major_version_ = rhs.major_version_;
      this->minor_version_ = rhs.minor_version_;
    }
  return *this;
}

ACE_InputCDR::ACE_InputCDR (const ACE_OutputCDR& rhs,
                            ACE_Allocator* buffer_allocator,
                            ACE_Allocator* data_block_allocator,
                            ACE_Allocator* message_block_allocator)
  : start_ (rhs.total_length () + ACE_CDR::MAX_ALIGNMENT,
            ACE_Message_Block::MB_DATA,
            0,
            0,
            buffer_allocator,
            0,
            0,
            ACE_Time_Value::zero,
            ACE_Time_Value::max_time,
            data_block_allocator,
            message_block_allocator),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1),
    char_translator_ (0),
    wchar_translator_ (0),
    major_version_ (rhs.major_version_),
    minor_version_ (rhs.minor_version_)
{
  ACE_CDR::mb_align (&this->start_);
  for (const ACE_Message_Block *i = rhs.begin ();
       i != rhs.end ();
       i = i->cont ())
    this->start_.copy (i->rd_ptr (), i->length ());
}

ACE_CDR::Boolean
ACE_InputCDR::skip_wchar (void)
{
  if (ACE_static_cast (ACE_CDR::Short, major_version_) == 1
          && ACE_static_cast (ACE_CDR::Short, minor_version_) == 2)
  {
    ACE_CDR::Octet len;
    if (this->read_1 (&len))
      return this->skip_bytes (ACE_static_cast (size_t, len));
  }
  else
  {
    ACE_CDR::WChar x;
      return this->read_2 (ACE_reinterpret_cast (ACE_CDR::UShort*,&x));
  }

  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_wchar (ACE_CDR::WChar& x)
{
  if (ACE_static_cast (ACE_CDR::Short, major_version_) == 1
          && ACE_static_cast (ACE_CDR::Short, minor_version_) == 2)
    {
      ACE_CDR::Octet len;

      if (this->read_1 (&len))
        {
          return this->read_octet_array (
                           ACE_reinterpret_cast (ACE_CDR::Octet*,
                                                 &x),
                           ACE_static_cast (ACE_CDR::ULong,
                                            len)
                         );
        }
    }
  else if (this->wchar_translator_ == 0)
    {
      return this->read_2 (ACE_reinterpret_cast (ACE_CDR::UShort*,
                                                 &x));
    }

  return this->wchar_translator_->read_wchar (*this,
                                              x);
}

ACE_CDR::Boolean
ACE_InputCDR::read_string (char *&x)
{
  // @@ This is a slight violation of "Optimize for the common case",
  // i.e. normally the translator will be 0, but OTOH the code is
  // smaller and should be better for the cache ;-) ;-)
  if (this->char_translator_ != 0)
    {
      return this->char_translator_->read_string (*this,
                                                  x);
    }

  ACE_CDR::ULong len;

  this->read_ulong (len);
  // A check for the length being too great is done later in the
  // call to read_char_array but we want to have it done before
  // the memory is allocated.
  if (len > 0 && len <= this->length())
    {
      ACE_NEW_RETURN (x,
                      ACE_CDR::Char[len],
                      0);
      if (this->read_char_array (x, len))
        {
          return 1;
        }

      delete [] x;
    }
  else if (len == 0)
    {
      // Convert any null strings to empty strings since empty
      // strings can cause crashes. (See bug 58.)
      ACE_NEW_RETURN (x,
                      ACE_CDR::Char[1],
                      0);
      ACE_OS::strcpy (ACE_const_cast (char *&, x), "");
      return 1;
    }

  x = 0;
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_string (ACE_CString &x)
{
  ACE_CDR::Char *data;
  if (this->read_string (data))
    {
      x = data;
      delete [] data;
      return 1;
    }

  x = "";
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_wstring (ACE_CDR::WChar*& x)
{
  // @@ This is a slight violation of "Optimize for the common case",
  // i.e. normally the translator will be 0, but OTOH the code is
  // smaller and should be better for the cache ;-) ;-)
  if (this->wchar_translator_ != 0)
    return this->wchar_translator_->read_wstring (*this, x);

  ACE_CDR::ULong len;
  this->read_ulong (len);

  // A check for the length being too great is done later in the
  // call to read_char_array but we want to have it done before
  // the memory is allocated.
  if (len > 0 && len <= this->length())
    {

      if (ACE_static_cast (ACE_CDR::Short, this->major_version_) == 1
          && ACE_static_cast (ACE_CDR::Short, this->minor_version_) == 2)
        {
          len = len / sizeof (ACE_CDR::WChar);

          //allocating one extra for the null character needed by applications
          ACE_NEW_RETURN (x,
                          ACE_CDR::WChar [len + 1],
                          0);

          if (this->read_wchar_array (x, len))
            {

              //Null character used by applications to find the end of
              //the wstring
              //Is this okay with the GIOP 1.2 spec??
              x[len] = '\x00';

              return 1;
            }
        }
      else
        {
          ACE_NEW_RETURN (x,
                          ACE_CDR::WChar [len],
                          0);

          if (this->read_wchar_array (x, len))
            return 1;
        }

      delete [] x;
    }
  else if (len == 0)
    {
      // Convert any null strings to empty strings since empty
      // strings can cause crashes. (See bug 58.)
      ACE_NEW_RETURN (x,
                      ACE_CDR::WChar[1],
                      0);
      ACE_OS::memcpy(x, "", 2);
      return 1;
    }

  this->good_bit_ = 0;
  x = 0;
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_array (void* x,
                          size_t size,
                          size_t align,
                          ACE_CDR::ULong length)
{
  if (length == 0)
    return 1;
  char* buf;

  if (this->adjust (size * length, align, buf) == 0)
    {
#if defined (ACE_DISABLE_SWAP_ON_READ)
      ACE_OS::memcpy (x, buf, size*length);
#else
      if (!this->do_byte_swap_ || size == 1)
        {
          ACE_OS::memcpy (x, buf, size*length);
        }
      else
        {
          char *target = ACE_reinterpret_cast (char*, x);
          switch (size)
            {
            case 2:
              ACE_CDR::swap_2_array (buf, target, length);
              break;
            case 4:
              ACE_CDR::swap_4_array (buf, target, length);
              break;
            case 8:
              ACE_CDR::swap_8_array (buf, target, length);
              break;
            case 16:
              ACE_CDR::swap_16_array (buf, target, length);
              break;
            default:
              // TODO: print something?
              this->good_bit_ = 0;
              return 0;
            }
        }
#endif /* ACE_DISABLE_SWAP_ON_READ */
      return this->good_bit_;
    }
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_boolean_array (ACE_CDR::Boolean *x,
                                  ACE_CDR::ULong length)
{
  // Make sure the length of the array isn't greater than the length of
  // the stream.
  if (length > this->length())
    return 0;

  // It is hard to optimize this, the spec requires that on the wire
  // booleans be represented as a byte with value 0 or 1, but in
  // memoery it is possible (though very unlikely) that a boolean has
  // a non-zero value (different from 1).
  // We resort to a simple loop.
  for (ACE_CDR::ULong i = 0; i != length && this->good_bit_; ++i)
    {
      this->read_boolean (x[i]);
    }
  return this->good_bit_;
}

ACE_CDR::Boolean
ACE_InputCDR::read_1 (ACE_CDR::Octet *x)
{
  if (this->rd_ptr () < this->wr_ptr ())
    {
      *x = *ACE_reinterpret_cast (ACE_CDR::Octet*,this->rd_ptr ());
      this->start_.rd_ptr (1);
      return 1;
    }

  this->good_bit_ = 0;
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_2 (ACE_CDR::UShort *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::SHORT_SIZE, buf) == 0)
    {
#if !defined (ACE_DISABLE_SWAP_ON_READ)
      if (!this->do_byte_swap_)
        {
          *x = *ACE_reinterpret_cast (ACE_CDR::UShort*, buf);
        }
      else
        {
          ACE_CDR::swap_2 (buf, ACE_reinterpret_cast (char*, x));
        }
#else
      *x = *ACE_reinterpret_cast(ACE_CDR::UShort*,buf);
#endif /* ACE_DISABLE_SWAP_ON_READ */
      return 1;
    }
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_4 (ACE_CDR::ULong *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::LONG_SIZE, buf) == 0)
    {
#if !defined (ACE_DISABLE_SWAP_ON_READ)
      if (!this->do_byte_swap_)
        {
          *x = *ACE_reinterpret_cast (ACE_CDR::ULong*, buf);
        }
      else
        {
          ACE_CDR::swap_4 (buf, ACE_reinterpret_cast (char*, x));
        }
#else
      *x = *ACE_reinterpret_cast(ACE_CDR::ULong*,buf);
#endif /* ACE_DISABLE_SWAP_ON_READ */
      return 1;
    }
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_8 (ACE_CDR::ULongLong *x)
{
  char *buf;

  if (this->adjust (ACE_CDR::LONGLONG_SIZE, buf) == 0)
    {
#if !defined (ACE_DISABLE_SWAP_ON_READ)
#  if defined (__arm__)
      if (!this->do_byte_swap_)
        {
          // Convert from Intel format (12345678 => 56781234)
          const char *orig = buf;
          char *target = ACE_reinterpret_cast (char *, x);
          register ACE_UINT32 x =
            *ACE_reinterpret_cast (const ACE_UINT32 *, orig);
          register ACE_UINT32 y =
            *ACE_reinterpret_cast (const ACE_UINT32 *, orig + 4);
          *ACE_reinterpret_cast (ACE_UINT32 *, target) = y;
          *ACE_reinterpret_cast (ACE_UINT32 *, target + 4) = x;
        }
      else
        {
          // Convert from Sparc format (12345678 => 43218765)
          const char *orig = buf;
          char *target = ACE_reinterpret_cast (char *, x);
          register ACE_UINT32 x =
            *ACE_reinterpret_cast (const ACE_UINT32 *, orig);
          register ACE_UINT32 y =
            *ACE_reinterpret_cast (const ACE_UINT32 *, orig + 4);
          x = (x << 24) | ((x & 0xff00) << 8) | ((x & 0xff0000) >> 8) | (x >> 24);
          y = (y << 24) | ((y & 0xff00) << 8) | ((y & 0xff0000) >> 8) | (y >> 24);
          *ACE_reinterpret_cast (ACE_UINT32 *, target) = x;
          *ACE_reinterpret_cast (ACE_UINT32 *, target + 4) = y;
        }
#  else
      if (!this->do_byte_swap_)
        {
          *x = *ACE_reinterpret_cast (ACE_CDR::ULongLong *, buf);
        }
      else
        {
          ACE_CDR::swap_8 (buf, ACE_reinterpret_cast (char *, x));
        }
#  endif /* !__arm__ */
#else
      *x = *ACE_reinterpret_cast (ACE_CDR::ULongLong *, buf);
#endif /* ACE_DISABLE_SWAP_ON_READ */
      return 1;
    }

  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_16 (ACE_CDR::LongDouble *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::LONGLONG_SIZE,
                    ACE_CDR::LONGLONG_ALIGN,
                    buf) == 0)
    {
#if !defined (ACE_DISABLE_SWAP_ON_READ)
      if (!this->do_byte_swap_)
        {
          *x = *ACE_reinterpret_cast (ACE_CDR::LongDouble *, buf);
        }
      else
        {
          ACE_CDR::swap_16 (buf, ACE_reinterpret_cast (char*, x));
        }
#else
      *x = *ACE_reinterpret_cast(ACE_CDR::LongDouble*,buf);
#endif /* ACE_DISABLE_SWAP_ON_READ */
      return 1;
    }
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::skip_string (void)
{
  ACE_CDR::ULong len;
  if (this->read_ulong (len))
    {
      if (this->rd_ptr () + len <= this->wr_ptr ())
        {
          this->rd_ptr (len);
          return 1;
        }
      this->good_bit_ = 0;
    }

  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::skip_wstring (void)
{
  ACE_CDR::Boolean continue_skipping = 1;
  ACE_CDR::ULong len;

  continue_skipping = read_ulong (len);

  if (continue_skipping != 0 && len != 0)
    {
      if (ACE_static_cast (ACE_CDR::Short, this->major_version_) == 1
            && ACE_static_cast (ACE_CDR::Short, this->minor_version_) == 2)
          continue_skipping = this->skip_bytes ((size_t)len);
      else
        while (continue_skipping != 0 && len--)
          continue_skipping = this->skip_wchar ();
    }
  return continue_skipping;
}

ACE_CDR::Boolean
ACE_InputCDR::skip_bytes (size_t len)
{
  if (this->rd_ptr () + len <= this->wr_ptr ())
    {
      this->rd_ptr (len);
      return 1;
    }
  this->good_bit_ = 0;
  return 0;
}

int
ACE_InputCDR::grow (size_t newsize)
{
  if (ACE_CDR::grow (&this->start_, newsize) == -1)
    return -1;

  ACE_CDR::mb_align (&this->start_);
  this->start_.wr_ptr (newsize);
  return 0;
}

void
ACE_InputCDR::reset (const ACE_Message_Block* data,
                     int byte_order)
{
  this->reset_byte_order (byte_order);
  ACE_CDR::consolidate (&this->start_, data);
}

void
ACE_InputCDR::steal_from (ACE_InputCDR &cdr)
{
  this->do_byte_swap_ = cdr.do_byte_swap_;
  this->start_.data_block (cdr.start_.data_block ()->duplicate ());

  // If the message block had a DONT_DELETE flags, just clear it off..
  this->start_.clr_self_flags (ACE_Message_Block::DONT_DELETE);
  this->start_.rd_ptr (cdr.start_.rd_ptr ());

  this->start_.wr_ptr (cdr.start_.wr_ptr ());
  this->major_version_ = cdr.major_version_;
  this->minor_version_ = cdr.minor_version_;
  cdr.reset_contents ();
}

void
ACE_InputCDR::exchange_data_blocks (ACE_InputCDR &cdr)
{
  // Exchange byte orders
  int byte_order = cdr.do_byte_swap_;
  cdr.do_byte_swap_ = this->do_byte_swap_;
  this->do_byte_swap_ = byte_order;

  // Get the destination read and write pointers
  size_t drd_pos =
    cdr.start_.rd_ptr () - cdr.start_.base ();
  size_t dwr_pos =
    cdr.start_.wr_ptr () - cdr.start_.base ();

  // Get the source read & write pointers
  size_t srd_pos =
    this->start_.rd_ptr () - this->start_.base ();
  size_t swr_pos =
    this->start_.wr_ptr () - this->start_.base ();

  // Exchange data_blocks. Dont release any of the data blocks.
  ACE_Data_Block *dnb =
    this->start_.replace_data_block (cdr.start_.data_block ());
  cdr.start_.replace_data_block (dnb);

  // Exchange the flags information..
  ACE_Message_Block::Message_Flags df = cdr.start_.self_flags ();
  ACE_Message_Block::Message_Flags sf = this->start_.self_flags ();

  cdr.start_.clr_self_flags (df);
  this->start_.clr_self_flags (sf);

  cdr.start_.set_self_flags (sf);
  this->start_.set_self_flags (df);

  // Reset the <cdr> pointers to zero before it is set again.
  cdr.start_.reset ();
  this->start_.reset ();

  // Set the read and write pointers.
  if (cdr.start_.size () >= srd_pos)
    cdr.start_.rd_ptr (srd_pos);

  if (cdr.start_.size () >= swr_pos)
    cdr.start_.wr_ptr (swr_pos);

  if (this->start_.size () >= drd_pos)
    this->start_.rd_ptr (drd_pos);

  if (this->start_.size () >= dwr_pos)
    this->start_.wr_ptr (dwr_pos);

  ACE_CDR::Octet dmajor = cdr.major_version_;
  ACE_CDR::Octet dminor = cdr.minor_version_;

  // Exchange the GIOP version info
  cdr.major_version_ = this->major_version_;
  cdr.minor_version_ = this->minor_version_;

  this->major_version_ = dmajor;
  this->minor_version_ = dminor;
}


ACE_Data_Block *
ACE_InputCDR::clone_from (ACE_InputCDR &cdr)
{
  this->do_byte_swap_ = cdr.do_byte_swap_;

  // Replace our data block by using the incoming CDR stream.
  ACE_Data_Block *db =
    this->start_.replace_data_block (cdr.start_.data_block ()->clone_nocopy ());

  // Align the start_ message block.
  ACE_CDR::mb_align (&this->start_);

  // Clear the DONT_DELETE flag if it has been set
  this->start_.clr_self_flags (ACE_Message_Block::DONT_DELETE);

  // Get the read & write pointer positions in the incoming CDR
  // streams
  char *rd_ptr = cdr.start_.rd_ptr ();
  char *wr_ptr = cdr.start_.wr_ptr ();

  // Now reset the incoming CDR stream
  cdr.start_.reset ();

  // As we have reset the stream, try to align the underlying message
  // block in the incoming stream
  ACE_CDR::mb_align (&cdr.start_);

  // Get the read & write pointer positions again
  char *nrd_ptr = cdr.start_.rd_ptr ();
  char *nwr_ptr = cdr.start_.wr_ptr ();

  // Actual length of the stream is..
  // @todo: This will look idiotic, but we dont seem to have much of a
  // choice. How do we calculate the length of the incoming stream?
  // Calling the method before calling reset () would give us the
  // wrong length of the stream that needs copying.  So we do the
  // calulation like this
  // (1) We get the <rd_ptr> and <wr_ptr> positions of the incoming
  // stream.
  // (2) Then we reset the <incoming> stream and then align it.
  // (3) We get the <rd_ptr> and <wr_ptr> positions again. (Points #1
  // thru #3 has been done already)
  // (4) The difference in the <rd_ptr> and <wr_ptr> positions gives
  // us the following, the actual bytes traversed by the <rd_ptr> and
  // <wr_ptr>.
  // (5) The bytes traversed by the <wr_ptr> is the actual length of
  // the stream.

  // Actual bytes traversed
  size_t rd_bytes = rd_ptr - nrd_ptr;
  size_t wr_bytes = wr_ptr - nwr_ptr;

  // Now do the copy
  (void) ACE_OS::memcpy (this->start_.wr_ptr (),
                         cdr.start_.rd_ptr (),
                         wr_bytes);

  // Set the read pointer position to the same point as that was in
  // <incoming> cdr.
  this->start_.rd_ptr (rd_bytes);
  this->start_.wr_ptr (wr_bytes);

  // We have changed the read & write pointers for the incoming
  // stream. Set them back to the positions that they were before..
  cdr.start_.rd_ptr (rd_bytes);
  cdr.start_.wr_ptr (wr_bytes);

  this->major_version_ = cdr.major_version_;
  this->minor_version_ = cdr.minor_version_;

  return db;
}

ACE_Message_Block*
ACE_InputCDR::steal_contents (void)
{
  ACE_Message_Block* block =
    this->start_.clone ();
  this->start_.data_block (block->data_block ()->clone ());

  // If at all our message had a DONT_DELETE flag set, just clear it
  // off.
  this->start_.clr_self_flags (ACE_Message_Block::DONT_DELETE);

  ACE_CDR::mb_align (&this->start_);

  return block;
}

void
ACE_InputCDR::reset_contents (void)
{
  this->start_.data_block (this->start_.data_block ()->clone_nocopy
                           ());

  // Reset the flags...
  this->start_.clr_self_flags (ACE_Message_Block::DONT_DELETE);
}

// --------------------------------------------------------------

#if defined (VXWORKS) && defined (ghs)
ACE_CDR::Boolean
ACE_OutputCDR::write_float (ACE_CDR::Float x)
{
  return this->write_4 (ACE_reinterpret_cast (const ACE_CDR::ULong*, &x));
}

ACE_CDR::Boolean
ACE_InputCDR::read_float (ACE_CDR::Float &x)
{
  return this->read_4 (ACE_reinterpret_cast (ACE_CDR::ULong*, &x));
}
#endif /* VXWORKS && ghs */

ACE_CDR::Boolean
operator<< (ACE_OutputCDR &os, const ACE_CString &x)
{
  os.write_string (x);
  return os.good_bit ();
}


ACE_CDR::Boolean
operator>> (ACE_InputCDR &is, ACE_CString &x)
{
  is.read_string (x);
  return is.good_bit ();
}
