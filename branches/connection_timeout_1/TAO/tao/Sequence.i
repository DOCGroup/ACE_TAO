/* -*- C++ -*- */
// $Id$

// Operations on the unbounded sequence class.

ACE_INLINE
TAO_Base_Sequence::TAO_Base_Sequence (void)
  : maximum_ (0),
    length_ (0),
    buffer_ (0),
    release_ (0)
{
}

ACE_INLINE
TAO_Base_Sequence::TAO_Base_Sequence (CORBA::ULong maximum,
                                      CORBA::ULong length,
                                      void *buffer,
                                      CORBA::Boolean release)
  : maximum_ (maximum),
    length_ (length),
    buffer_ (buffer),
    release_ (release)
{
}

ACE_INLINE
TAO_Base_Sequence::TAO_Base_Sequence (CORBA::ULong maximum,
                                      void *data)
  : maximum_ (maximum),
    length_ (0),
    buffer_ (data),
    release_ (1)
{
}

ACE_INLINE
TAO_Base_Sequence::TAO_Base_Sequence (const TAO_Base_Sequence &rhs)
  : maximum_ (rhs.maximum_),
    length_ (rhs.length_),
    buffer_ (0),
    release_ (1)
{
}

ACE_INLINE CORBA::Boolean
TAO_Base_Sequence::release (void) const
{
  return this->release_;
}

ACE_INLINE TAO_Base_Sequence &
TAO_Base_Sequence::operator= (const TAO_Base_Sequence &rhs)
{
  this->maximum_ = rhs.maximum_;
  this->length_ = rhs.length_;
  this->release_ = 1;
  return *this;
}

ACE_INLINE CORBA::ULong
TAO_Base_Sequence::maximum (void) const
{
  return this->maximum_;
}

// ****************************************************************

ACE_INLINE
TAO_Unbounded_Base_Sequence::TAO_Unbounded_Base_Sequence (void)
{
}

ACE_INLINE
TAO_Unbounded_Base_Sequence::
TAO_Unbounded_Base_Sequence (CORBA::ULong maximum,
                             CORBA::ULong length,
                             void* buffer,
                             CORBA::Boolean release)
  :  TAO_Base_Sequence (maximum, length, buffer, release)
{
}

ACE_INLINE
TAO_Unbounded_Base_Sequence::
TAO_Unbounded_Base_Sequence (CORBA::ULong maximum,
                             void* buffer)
  :  TAO_Base_Sequence (maximum, buffer)
{
}

ACE_INLINE CORBA::ULong
TAO_Unbounded_Base_Sequence::length (void) const
{
  return this->length_;
}

ACE_INLINE void
TAO_Unbounded_Base_Sequence::length (CORBA::ULong length)
{
  if (length > this->maximum_)
    {
      this->_allocate_buffer (length);
      this->maximum_ = length;
      this->release_ = 1;
    }
  else if (length < this->length_)
    {
      this->_shrink_buffer (length, this->length_);
    }
  this->length_ = length;
}

// ****************************************************************

ACE_INLINE
TAO_Bounded_Base_Sequence::TAO_Bounded_Base_Sequence (void)
{
}

ACE_INLINE
TAO_Bounded_Base_Sequence::
TAO_Bounded_Base_Sequence (CORBA::ULong maximum,
                           CORBA::ULong length,
                           void* buffer,
                           CORBA::Boolean release)
  :  TAO_Base_Sequence (maximum, length, buffer, release)
{
}

ACE_INLINE
TAO_Bounded_Base_Sequence::
TAO_Bounded_Base_Sequence (CORBA::ULong maximum,
                           void* buffer)
  :  TAO_Base_Sequence (maximum, buffer)
{
}

ACE_INLINE CORBA::ULong
TAO_Bounded_Base_Sequence::length (void) const
{
  return this->length_;
}

ACE_INLINE void
TAO_Bounded_Base_Sequence::length (CORBA::ULong length)
{
  if (this->buffer_ == 0)
    {
      this->_allocate_buffer (this->maximum_);
      this->release_ = 1;
    }

  if (length > this->maximum_)
    {
      return;
    }
  else if (length < this->length_)
    {
      this->_shrink_buffer (length, this->length_);
    }

  this->length_ = length;
}

// ****************************************************************

//default constructors
ACE_INLINE
TAO_Unbounded_String_Sequence::TAO_Unbounded_String_Sequence (void)
{
}

ACE_INLINE
TAO_Unbounded_String_Sequence::
TAO_Unbounded_String_Sequence (CORBA::ULong maximum,
                               CORBA::ULong length,
                               char* *value,
                               CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence (maximum, length, value, release)
{
}

ACE_INLINE
TAO_Unbounded_WString_Sequence::TAO_Unbounded_WString_Sequence (void)
{
}

ACE_INLINE
TAO_Unbounded_WString_Sequence::
TAO_Unbounded_WString_Sequence (CORBA::ULong maximum,
                               CORBA::ULong length,
                               CORBA::WChar* *value,
                               CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence (maximum, length, value, release)
{
}

// ****************************************************************

ACE_TEMPLATE_METHOD_SPECIALIZATION
ACE_INLINE CORBA::Octet *
TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (CORBA::ULong size)
{
  return new CORBA::Octet[size];
}

ACE_TEMPLATE_METHOD_SPECIALIZATION
ACE_INLINE void
TAO_Unbounded_Sequence<CORBA::Octet>::freebuf (CORBA::Octet *buffer)
{
  delete [] buffer;
}

ACE_TEMPLATE_METHOD_SPECIALIZATION
ACE_INLINE
TAO_Unbounded_Sequence<CORBA::Octet>::TAO_Unbounded_Sequence (void)
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  :  mb_ (0)
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
{
}

ACE_TEMPLATE_METHOD_SPECIALIZATION
ACE_INLINE
TAO_Unbounded_Sequence<CORBA::Octet>::TAO_Unbounded_Sequence (CORBA::ULong maximum)
  : TAO_Unbounded_Base_Sequence (maximum,
                                 TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (maximum))
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  ,  mb_ (0)
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
{
}

ACE_TEMPLATE_METHOD_SPECIALIZATION
ACE_INLINE
TAO_Unbounded_Sequence<CORBA::Octet>::TAO_Unbounded_Sequence (CORBA::ULong maximum,
                                                   CORBA::ULong length,
                                                   CORBA::Octet *data,
                                                   CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence (maximum, length, data, release)
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  , mb_ (0)
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
{
}

ACE_TEMPLATE_METHOD_SPECIALIZATION
ACE_INLINE const CORBA::Octet *
TAO_Unbounded_Sequence<CORBA::Octet>::get_buffer (void) const
{
  return ACE_reinterpret_cast(const CORBA::Octet * ACE_CAST_CONST, this->buffer_);
}

// This function is a little too big to be inlined, but some compilers
// (Sun/CC 4.1?) die if it isn't :-(
ACE_TEMPLATE_METHOD_SPECIALIZATION
ACE_INLINE CORBA::Octet *
TAO_Unbounded_Sequence<CORBA::Octet>::get_buffer (CORBA::Boolean orphan)
{
  CORBA::Octet *result = 0;
  if (orphan == 0)
    {
      // We retain ownership.

      if (this->buffer_ == 0)
        {
          // The buffer was not allocated, we must allocate it now.
          result = TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (this->length_);
          this->buffer_ = result;
	  this->release_ = 1;
        }
      else
        {
          result =
            ACE_reinterpret_cast (CORBA::Octet*,this->buffer_);
        }
    }
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  else if (this->mb_ != 0) // (orphan == 1)
    {
      // We must create a copy anyway:
      //   the user is supposed to call freebuf() to release the
      //   buffer, but the buffer is inside a Message_Block...
      //   We thought about storing the pointer to the Message_Block
      //   somewhere at the beginning of the buffer (before the actual
      //   data), but that will not work in 64 bit machines when the
      //   buffer comes from a CDR stream.
      //
      result = TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (this->length_);
      ACE_OS::memcpy (result, this->buffer_, this->length_);
    }
  else if (this->release_ != 0)
    {
      // We set the state back to default and relinquish
      // ownership.
      result = ACE_reinterpret_cast(CORBA::Octet*,this->buffer_);
      this->maximum_ = 0;
      this->length_ = 0;
      this->buffer_ = 0;
      this->release_ = 0;
    }
#else /* TAO_NO_COPY_OCTET_SEQUENCES == 0 */
  else
    {
      result = ACE_reinterpret_cast (CORBA::Octet*,this->buffer_);
      if (this->release_ != 0)
        {
          // We set the state back to default and relinquish
          // ownership.
          this->maximum_ = 0;
          this->length_ = 0;
          this->buffer_ = 0;
          this->release_ = 0;
        }
    }
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 0 */
  /* else
     // Oops, it's not our buffer to relinquish...
     return 0;
  */
  return result;
}

ACE_TEMPLATE_METHOD_SPECIALIZATION
ACE_INLINE CORBA::Octet &
TAO_Unbounded_Sequence<CORBA::Octet>::operator[] (CORBA::ULong i)
{
  ACE_ASSERT (i < this->maximum_);
  CORBA::Octet* tmp = ACE_reinterpret_cast(CORBA::Octet*,this->buffer_);
  return tmp[i];
}

ACE_TEMPLATE_METHOD_SPECIALIZATION
ACE_INLINE const CORBA::Octet &
TAO_Unbounded_Sequence<CORBA::Octet>::operator[] (CORBA::ULong i) const
{
  ACE_ASSERT (i < this->maximum_);
  CORBA::Octet * const tmp = ACE_reinterpret_cast (CORBA::Octet* ACE_CAST_CONST, this->buffer_);
  return tmp[i];
}

#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
ACE_TEMPLATE_METHOD_SPECIALIZATION
ACE_INLINE ACE_Message_Block*
TAO_Unbounded_Sequence<CORBA::Octet>::mb (void) const
{
  return this->mb_;
}

ACE_TEMPLATE_METHOD_SPECIALIZATION
ACE_INLINE void
TAO_Unbounded_Sequence<CORBA::Octet>::replace (CORBA::ULong length,
                                               const ACE_Message_Block* mb)
{
  this->_deallocate_buffer ();

  // Get the message block flags.
  ACE_Message_Block::Message_Flags flg = mb->self_flags ();

  // If the DONT_DELETE flag is disabled just a duplicate would
  // help. If the DONT_DELETE flag is enabled a deep copy is needed as
  // the contents would be on stack. Just incrementing the ref count
  // on the stack based data block would only crash the program when
  // the stack unwinds
  if (ACE_BIT_DISABLED (flg,
                        ACE_Message_Block::DONT_DELETE))
    {
      this->mb_ = ACE_Message_Block::duplicate (mb);
    }
  else
    {
      // As we are in CORBA mode, all the data blocks would be aligned
      // on an 8 byte boundary
      ACE_Message_Block msgb (*mb,
                              ACE_CDR::MAX_ALIGNMENT);

      // Get the base pointer of the incoming message block
      char *start = ACE_ptr_align_binary (mb->base (),
                                          ACE_CDR::MAX_ALIGNMENT);

      // Get the read and write displacements in the incoming stream
      size_t rd_pos = mb->rd_ptr () - start;
      size_t wr_pos = mb->wr_ptr () - start;

      this->mb_ = ACE_Message_Block::duplicate (&msgb);

      this->mb_->rd_ptr (rd_pos);
      this->mb_->wr_ptr (wr_pos);
    }

  this->buffer_ = this->mb_->rd_ptr ();
  this->maximum_ = length;
  this->length_ = length;
  this->release_ = 0;
}

#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

ACE_TEMPLATE_METHOD_SPECIALIZATION
ACE_INLINE void
TAO_Unbounded_Sequence<CORBA::Octet>::replace (CORBA::ULong max,
                                               CORBA::ULong length,
                                               CORBA::Octet *data,
                                               CORBA::Boolean release)
{
  this->maximum_ = max;
  this->length_ = length;

#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  if (this->mb_ != 0)
    {
      ACE_Message_Block::release (this->mb_);
      this->mb_ = 0;
    }
  else
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
    if (this->buffer_ && this->release_ == 1)
    {
      CORBA::Octet* tmp = ACE_reinterpret_cast(CORBA::Octet*,this->buffer_);
      TAO_Unbounded_Sequence<CORBA::Octet>::freebuf (tmp);
    }
  this->buffer_ = data;
  this->release_ = release;
}
