// $Id$

#include "tao/Sequence.h"
#include "ace/Message_Block.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#if !defined (__ACE_INLINE__)
#include "tao/Sequence.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao, 
           Sequence, 
           "$Id$")

// *************************************************************
// Operations for class TAO_Base_Sequence
// *************************************************************

TAO_Base_Sequence::~TAO_Base_Sequence (void)
{
}

void TAO_Base_Sequence::_shrink_buffer (CORBA::ULong, CORBA::ULong)
{
  // default is no op.
}

void
TAO_Base_Sequence::_downcast (void *,
                              CORBA::Object *
                              ACE_ENV_ARG_DECL_NOT_USED)
{
  // default is no op.
  // @@ TODO Maybe throw an exception?
}

CORBA::Object *
TAO_Base_Sequence::_upcast (void *) const
{
  return 0;
}

void TAO_Base_Sequence::
check_bounds(
    char const * filename, unsigned long lineno,
    CORBA::ULong tao_idx, CORBA::ULong tao_max) const
{
  // TODO use hook
  if(tao_idx >= tao_max)
  {
    ACE_ERROR((LM_ERROR,
                  "Access error in TAO_Sequence file=%s, line=%u, "
                  "idx=%u, max=%u\n",
                  filename, lineno, tao_idx, tao_max));
  }
}

// *************************************************************
// Operations for class TAO_Unbounded_Base_Sequence
// *************************************************************

TAO_Unbounded_Base_Sequence::~TAO_Unbounded_Base_Sequence (void)
{
}

// *************************************************************
// Operations for class TAO_Bounded_Base_Sequence
// *************************************************************

TAO_Bounded_Base_Sequence::~TAO_Bounded_Base_Sequence (void)
{
}

// *************************************************************

// constructor for unbounded string seq
TAO_Unbounded_String_Sequence::TAO_Unbounded_String_Sequence (
    CORBA::ULong maximum
  )
  : TAO_Unbounded_Base_Sequence (
        maximum,
        TAO_Unbounded_String_Sequence::allocbuf (maximum)
      )
{
}

TAO_Unbounded_String_Sequence::TAO_Unbounded_String_Sequence (
    const TAO_Unbounded_String_Sequence & rhs
  )
  : TAO_Unbounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      char* *tmp1 =
        TAO_Unbounded_String_Sequence::allocbuf (this->maximum_);

      char ** const tmp2 =
        ACE_reinterpret_cast (char ** ACE_CAST_CONST, rhs.buffer_);

      for (CORBA::ULong i = 0; i < rhs.length_; ++i)
        {
          tmp1[i] = CORBA::string_dup (tmp2[i]);
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

TAO_Unbounded_String_Sequence::~TAO_Unbounded_String_Sequence (void)
{
  this->_deallocate_buffer ();
}

TAO_Unbounded_String_Sequence &
TAO_Unbounded_String_Sequence::operator= (
    const TAO_Unbounded_String_Sequence & rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      char ** tmp = ACE_reinterpret_cast (char **, 
                                          this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          CORBA::string_free (tmp[i]);
          tmp[i] = 0;
        }

      if (this->maximum_ < rhs.maximum_)
        {
          // Free the older buffer.
          TAO_Unbounded_String_Sequence::freebuf (tmp);
          this->buffer_ =
            TAO_Unbounded_String_Sequence::allocbuf (rhs.maximum_);
        }
    }
  else
    {
      if (rhs.maximum_ == 0)
        {
          this->buffer_ = 0;
        }
      else
        {
          this->buffer_ =
            TAO_Unbounded_String_Sequence::allocbuf (rhs.maximum_);
        }
    }

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  char ** tmp1 = ACE_reinterpret_cast (char **, this->buffer_);
  char ** const tmp2 = ACE_reinterpret_cast (char ** ACE_CAST_CONST,
                                             rhs.buffer_);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
      tmp1[i] = CORBA::string_dup (tmp2[i]);
    }

  return *this;
}

TAO_SeqElem_String_Manager
TAO_Unbounded_String_Sequence::operator[] (CORBA::ULong slot) const
{
  ACE_ASSERT (slot < this->maximum_);
  char ** const tmp =
    ACE_reinterpret_cast (char ** ACE_CAST_CONST,
                          this->buffer_);
  return TAO_SeqElem_String_Manager (tmp + slot,
                                     this->release_);
}

char **
TAO_Unbounded_String_Sequence::allocbuf (CORBA::ULong nelems)
{
  char ** buf = 0;
  ACE_NEW_RETURN (buf,
                  char * [nelems],
                  0);

  for (CORBA::ULong i = 0; i < nelems; ++i)
    {
      buf[i] = 0;
    }

  return buf;
}

void
TAO_Unbounded_String_Sequence::freebuf (char ** buffer)
{
  if (buffer == 0)
    {
      return;
    }

  // {orbos/97-05-15:16.11}
  // The freebuf function ensures that the destructor for each element
  // is called before the buffer is destroyed, except for string
  // elements, which are freed using string_free(), and object
  // reference elements, which are freed using release(). The freebuf
  // function will ignore null pointers passed to it.

  // @@ How are we supposed to implement that! We don't know the
  // length of the buffer here.
  //  Mark the length in the first four bytes? For the moment we let
  //  that be.

  delete [] buffer;
}

void
TAO_Unbounded_String_Sequence::_tao_any_destructor (
    void * _tao_void_pointer
  )
{
  TAO_Unbounded_String_Sequence * tmp = 
    ACE_static_cast (TAO_Unbounded_String_Sequence *,
                     _tao_void_pointer);
  delete tmp;
}

char **
TAO_Unbounded_String_Sequence::get_buffer (CORBA::Boolean orphan)
{
  char ** result = 0;

  if (orphan == 0)
    {
      // We retain ownership.
      if (this->buffer_ == 0)
        {
          result = allocbuf (this->length_);
          this->buffer_ = result;
          this->release_ = 1;
        }
      else
        {
          result = ACE_reinterpret_cast (char **, this->buffer_);
        }
    }
  else // if (orphan == 1)
    {
      if (this->release_ != 0)
        {
          // We set the state back to default and relinquish
          // ownership.
          result = ACE_reinterpret_cast (char **, this->buffer_);
          this->maximum_ = 0;
          this->length_ = 0;
          this->buffer_ = 0;
          this->release_ = 0;
        }
    }

  return result;
}

const char **
TAO_Unbounded_String_Sequence::get_buffer (void) const
{
  return ACE_reinterpret_cast (const char ** ACE_CAST_CONST, 
                               this->buffer_);
}

void
TAO_Unbounded_String_Sequence::_allocate_buffer (CORBA::ULong length)
{
  char ** tmp = TAO_Unbounded_String_Sequence::allocbuf (length);

  if (this->buffer_ != 0)
    {
      char ** old = ACE_reinterpret_cast (char **, this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          // Only call duplicate when we did not own the previous
          // buffer, since after this method we own it we must also
          // own the objects.  If we already own the objects there is
          // no need to copy them, if we did we would also have to
          // remove the old instances.
          if (!this->release_)
            {
              tmp [i] = CORBA::string_dup (old[i]);
            }
          else
            {
              tmp [i] = old[i];
            }
        }

      if (this->release_)
        {
          delete [] old;
        }
    }

  this->buffer_ = tmp;
}

void
TAO_Unbounded_String_Sequence::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    {
      return;
    }

  char ** tmp = ACE_reinterpret_cast (char **, this->buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    {
      CORBA::string_free (tmp[i]);
      tmp[i] = 0;
    }

  TAO_Unbounded_String_Sequence::freebuf (tmp);
  this->buffer_ = 0;
}

void
TAO_Unbounded_String_Sequence::_shrink_buffer (CORBA::ULong nl,
                                               CORBA::ULong ol)
{
  char ** tmp = ACE_reinterpret_cast (char **, 
                                      this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      CORBA::string_free (tmp[i]);
      tmp[i] = 0;
    }
}

void
TAO_Unbounded_String_Sequence::replace (CORBA::ULong maximum,
                                        CORBA::ULong length,
                                        char ** data,
                                        CORBA::Boolean release)
{
  if (this->release_ == 1)
    {
      char ** tmp = ACE_reinterpret_cast (char **, 
                                          this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          CORBA::string_free (tmp[i]);
          tmp[i] = 0;
        }
    }

  this->maximum_ = maximum;
  this->length_ = length;

  // If 'release' is 1, it is the caller's responsibility to allocate
  // 'data' with CORBA::string_alloc. 
  this->buffer_ = data;
  this->release_ = release;
}
// *************************************************************

// constructor for unbounded wide string seq
TAO_Unbounded_WString_Sequence::TAO_Unbounded_WString_Sequence (
    CORBA::ULong maximum
  )
  : TAO_Unbounded_Base_Sequence (
        maximum,
        TAO_Unbounded_WString_Sequence::allocbuf (maximum)
      )
{
}

TAO_Unbounded_WString_Sequence::TAO_Unbounded_WString_Sequence (
    const TAO_Unbounded_WString_Sequence & rhs
  )
  : TAO_Unbounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      CORBA::WChar ** tmp1 =
        TAO_Unbounded_WString_Sequence::allocbuf (this->maximum_);

      CORBA::WChar ** const tmp2 =
        ACE_reinterpret_cast (CORBA::WChar ** ACE_CAST_CONST,
                              rhs.buffer_);

      for (CORBA::ULong i = 0; i < rhs.length_; ++i)
        {
          tmp1[i] = CORBA::wstring_dup (tmp2[i]);
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

TAO_Unbounded_WString_Sequence::~TAO_Unbounded_WString_Sequence (void)
{
  this->_deallocate_buffer ();
}

TAO_Unbounded_WString_Sequence &
TAO_Unbounded_WString_Sequence::operator= (
    const TAO_Unbounded_WString_Sequence & rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      CORBA::WChar ** tmp = ACE_reinterpret_cast (CORBA::WChar **,
                                                  this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          CORBA::wstring_free (tmp[i]);
          tmp[i] = 0;
        }

      if (this->maximum_ < rhs.maximum_)
        {
          // free the older buffer
          TAO_Unbounded_WString_Sequence::freebuf (tmp);
          this->buffer_ =
            TAO_Unbounded_WString_Sequence::allocbuf (rhs.maximum_);
        }
    }
  else
    {
      if (rhs.maximum_ == 0)
        {
          this->buffer_ = 0;
        }
      else
        {
          this->buffer_ =
            TAO_Unbounded_WString_Sequence::allocbuf (rhs.maximum_);
        }
    }

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  CORBA::WChar ** tmp1 = ACE_reinterpret_cast (CORBA::WChar **,
                                               this->buffer_);
  CORBA::WChar ** const tmp2 = 
    ACE_reinterpret_cast (CORBA::WChar ** ACE_CAST_CONST,
                          rhs.buffer_);

  for (CORBA::ULong i=0; i < rhs.length_; ++i)
    {
      tmp1[i] = CORBA::wstring_dup (tmp2[i]);
    }

  return *this;
}

TAO_SeqElem_WString_Manager
TAO_Unbounded_WString_Sequence::operator[] (CORBA::ULong slot) const
{
  ACE_ASSERT (slot < this->maximum_);
  CORBA::WChar ** const tmp =
    ACE_reinterpret_cast (CORBA::WChar ** ACE_CAST_CONST,
                          this->buffer_);
  return TAO_SeqElem_WString_Manager (tmp + slot,
                                      this->release_);
}

CORBA::WChar **
TAO_Unbounded_WString_Sequence::allocbuf (CORBA::ULong nelems)
{
  CORBA::WChar ** buf = 0;
  ACE_NEW_RETURN (buf,
                  CORBA::WChar * [nelems],
                  0);

  for (CORBA::ULong i=0; i < nelems; ++i)
    {
      buf[i] = 0;
    }

  return buf;
}

void
TAO_Unbounded_WString_Sequence::freebuf (CORBA::WChar ** buffer)
{
  if (buffer == 0)
    {
      return;
    }

  // {orbos/97-05-15:16.11}
  // The freebuf function ensures that the destructor for each element
  // is called before the buffer is destroyed, except for string
  // elements, which are freed using wstring_free(), and object
  // reference elements, which are freed using release(). The freebuf
  // function will ignore null pointers passed to it.

  // @@ How are we supposed to implement that! We don't know the
  // length of the buffer here.
  //  Mark the length in the first four bytes? For the moment we let
  //  that be.

  delete [] buffer;
}

void
TAO_Unbounded_WString_Sequence::_tao_any_destructor (
    void * _tao_void_pointer
  )
{
  TAO_Unbounded_WString_Sequence * tmp = 
    ACE_static_cast (TAO_Unbounded_WString_Sequence *,
                     _tao_void_pointer);
  delete tmp;
}

CORBA::WChar **
TAO_Unbounded_WString_Sequence::get_buffer (CORBA::Boolean orphan)
{
  CORBA::WChar ** result = 0;

  if (orphan == 0)
    {
      // We retain ownership.
      if (this->buffer_ == 0)
        {
          result = allocbuf (this->length_);
          this->buffer_ = result;
          this->release_ = 1;
        }
      else
        {
          result = ACE_reinterpret_cast (CORBA::WChar **,
                                         this->buffer_);
        }
    }
  else // if (orphan == 1)
    {
      if (this->release_ != 0)
        {
          // We set the state back to default and relinquish
          // ownership.
          result = ACE_reinterpret_cast (CORBA::WChar **,
                                         this->buffer_);
          this->maximum_ = 0;
          this->length_ = 0;
          this->buffer_ = 0;
          this->release_ = 0;
        }
    }

  return result;
}

const CORBA::WChar **
TAO_Unbounded_WString_Sequence::get_buffer (void) const
{
  return ACE_reinterpret_cast (const CORBA::WChar ** ACE_CAST_CONST,
                               this->buffer_);
}

void
TAO_Unbounded_WString_Sequence::_allocate_buffer (CORBA::ULong length)
{
  CORBA::WChar ** tmp = TAO_Unbounded_WString_Sequence::allocbuf (length);

  if (this->buffer_ != 0)
    {
      CORBA::WChar ** old = ACE_reinterpret_cast (CORBA::WChar **,
                                                  this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          // Only call duplicate when we did not own the previous
          // buffer, since after this method we own it we must also
          // own the objects.  If we already own the objects there is
          // no need to copy them, if we did we would also have to
          // remove the old instances.
          if (!this->release_)
            {
              tmp [i] = CORBA::wstring_dup (old[i]);
            }
          else
            {
              tmp [i] = old[i];
            }
        }

      if (this->release_)
        {
          delete [] old;
        }
    }

  this->buffer_ = tmp;
}

void
TAO_Unbounded_WString_Sequence::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    {
      return;
    }

  CORBA::WChar **tmp = ACE_reinterpret_cast (CORBA::WChar **,
                                             this->buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    {
      CORBA::wstring_free (tmp[i]);
      tmp[i] = 0;
    }

  TAO_Unbounded_WString_Sequence::freebuf (tmp);
  this->buffer_ = 0;
}

void
TAO_Unbounded_WString_Sequence::_shrink_buffer (CORBA::ULong nl,
                                                CORBA::ULong ol)
{
  CORBA::WChar ** tmp = ACE_reinterpret_cast (CORBA::WChar **,
                                              this->buffer_);
  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      CORBA::wstring_free (tmp[i]);
      tmp[i] = 0;
    }
}

void
TAO_Unbounded_WString_Sequence::replace (CORBA::ULong maximum,
                                         CORBA::ULong length,
                                         CORBA::WChar* *data,
                                         CORBA::Boolean release)
{
  if (this->release_ == 1)
    {
      CORBA::WChar **tmp = ACE_reinterpret_cast (CORBA::WChar **, 
                                                 this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          CORBA::wstring_free (tmp[i]);
          tmp[i] = 0;
        }
    }

  this->maximum_ = maximum;
  this->length_ = length;

  // If 'release' is 1, it is the caller's responsibility to allocate
  // 'data' with CORBA::wstring_alloc. 
  this->buffer_ = data;
  this->release_ = release;
}
// ****************************************************************

TAO_Unbounded_Sequence<CORBA::Octet>::TAO_Unbounded_Sequence (
    const TAO_Unbounded_Sequence<CORBA::Octet> &rhs
  )
  : TAO_Unbounded_Base_Sequence (rhs)
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  , mb_ (0)
#endif /* (TAO_NO_COPY_OCTET_SEQUENCES == 0) */
{
  if (rhs.buffer_ != 0)
    {
      CORBA::Octet * tmp1 =
       TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (this->maximum_);


      CORBA::Octet * const tmp2 =
        ACE_reinterpret_cast (CORBA::Octet * ACE_CAST_CONST,
                              rhs.buffer_);

#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
      if (rhs.mb_ == 0)
        {
          ACE_OS::memcpy (tmp1,
                          tmp2,
                          this->length_);
        }
      else
        {
          size_t offset = 0;

          for (const ACE_Message_Block *i = rhs.mb_; i != 0; i = i->cont ())
            {
              ACE_OS::memcpy (tmp1 + offset,
                              i->rd_ptr (),
                              i->length ());

              offset += i->length ();
            }
        }
#else /* (TAO_NO_COPY_OCTET_SEQUENCES == 0) */
      ACE_OS::memcpy (tmp1, tmp2, this->length_);
#endif /* (TAO_NO_COPY_OCTET_SEQUENCES == 0) */

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

TAO_Unbounded_Sequence<CORBA::Octet> &
TAO_Unbounded_Sequence<CORBA::Octet>::operator= (
    const TAO_Unbounded_Sequence<CORBA::Octet> & rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  if (this->mb_ != 0)
    {
      ACE_Message_Block::release (this->mb_);
      this->mb_ = 0;
      this->buffer_ =
        TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (rhs.length_);
    }
  else
#endif /* (TAO_NO_COPY_OCTET_SEQUENCES == 0) */
    if (this->release_)
    {
      if (this->maximum_ < rhs.length_)
        {
          // free the old buffer
          CORBA::Octet * tmp = ACE_reinterpret_cast (CORBA::Octet *,
                                                     this->buffer_);
          TAO_Unbounded_Sequence<CORBA::Octet>::freebuf (tmp);
          this->buffer_ =
            TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (rhs.length_);
        }
    }
  else
    {
      if (rhs.maximum_ == 0)
        {
          this->buffer_ = 0;
        }
      else
        {
          this->buffer_ =
            TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (rhs.maximum_);
        }
    }

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  CORBA::Octet * tmp1 = ACE_reinterpret_cast (CORBA::Octet *, this->buffer_);
  CORBA::Octet * const tmp2 = 
    ACE_reinterpret_cast (CORBA::Octet * ACE_CAST_CONST, 
                          rhs.buffer_);

#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  // for (CORBA::ULong i = 0; i < this->length_; ++i)
  // tmp1[i] = tmp2[i];
  if (rhs.mb_ == 0)
    {
      ACE_OS::memcpy (tmp1, 
                      tmp2, 
                      this->length_);
    }
  else
    {
      size_t offset = 0;

      for (const ACE_Message_Block *i = rhs.mb_; i != 0; i = i->cont ())
        {
          ACE_OS::memcpy (tmp1 + offset, 
                          i->rd_ptr (), 
                          i->length ());
          offset += i->length ();
        }
    }
#else /* (TAO_NO_COPY_OCTET_SEQUENCES == 0) */
  ACE_OS::memcpy (tmp1, tmp2, this->length_);
#endif /* (TAO_NO_COPY_OCTET_SEQUENCES == 0) */

  return *this;
}

#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
TAO_Unbounded_Sequence<CORBA::Octet>::TAO_Unbounded_Sequence (
    CORBA::ULong length,
    const ACE_Message_Block * mb
  )
  :  TAO_Unbounded_Base_Sequence (length,
                                  length,
                                  mb->rd_ptr (),
                                  0),
     mb_ (0)
{
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
}
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

TAO_Unbounded_Sequence<CORBA::Octet>::~TAO_Unbounded_Sequence (void)
{
  this->_deallocate_buffer ();
}

// This function is a little too big to be inlined, but some compilers
// (Sun/CC 4.1?) die if it isn't :-(
CORBA::Octet *
TAO_Unbounded_Sequence<CORBA::Octet>::get_buffer (CORBA::Boolean orphan)
{
  CORBA::Octet * result = 0;

  if (orphan == 0)
    {
      // We retain ownership.

      if (this->buffer_ == 0)
        {
          // The buffer was not allocated, we must allocate it now.
          result = 
            TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (this->length_);
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
      result = ACE_reinterpret_cast(CORBA::Octet *, this->buffer_);
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

#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
void
TAO_Unbounded_Sequence<CORBA::Octet>::replace (CORBA::ULong length,
                                               const ACE_Message_Block * mb)
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
      char * start = ACE_ptr_align_binary (mb->base (),
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

void
TAO_Unbounded_Sequence<CORBA::Octet>::_tao_any_destructor (void * x)
{
  TAO_Unbounded_Sequence<CORBA::Octet> * tmp = 
    ACE_static_cast (TAO_Unbounded_Sequence<CORBA::Octet> *,
                     x);
  delete tmp;
}

void
TAO_Unbounded_Sequence<CORBA::Octet>::_allocate_buffer (CORBA::ULong length)
{
  CORBA::Octet *tmp = TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (length);

  if (this->buffer_ != 0)
    {
      CORBA::Octet * old = ACE_reinterpret_cast (CORBA::Octet *, 
                                                 this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          tmp[i] = old[i];
        }

#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
      if (this->mb_ != 0)
        {
          ACE_Message_Block::release (this->mb_);
          this->mb_ = 0;
        }
      else
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
        if (this->release_)
        {
          TAO_Unbounded_Sequence<CORBA::Octet>::freebuf (old);
        }
    }

  this->buffer_ = tmp;
}

void TAO_Unbounded_Sequence<CORBA::Octet>::_deallocate_buffer (void)
{
  if (this->buffer_ != 0
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
      && this->mb_ == 0
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
      && this->release_ != 0)
    {
      CORBA::Octet * tmp = ACE_reinterpret_cast (CORBA::Octet *, this->buffer_);
      TAO_Unbounded_Sequence<CORBA::Octet>::freebuf (tmp);
    }
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  else
    {
      ACE_Message_Block::release (this->mb_);
      this->mb_ = 0;
    }
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

  this->buffer_ = 0;
}


void
TAO_Unbounded_Sequence<CORBA::Octet>::replace (CORBA::ULong max,
                                               CORBA::ULong length,
                                               CORBA::Octet * data,
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
      CORBA::Octet * tmp = 
        ACE_reinterpret_cast(CORBA::Octet *, this->buffer_);
      TAO_Unbounded_Sequence<CORBA::Octet>::freebuf (tmp);
    }

  this->buffer_ = data;
  this->release_ = release;
}

// ****************************************************************

int
operator== (const TAO_Unbounded_Sequence<CORBA::Octet> & lhs,
            const TAO_Unbounded_Sequence<CORBA::Octet> & rhs)
{
  if (rhs.length () != lhs.length ())
    {
      return 0;
    }

  for (CORBA::ULong i = 0; i < rhs.length (); ++i)
    {
      if (rhs[i] != lhs[i])
        {
          return 0;
        }
    }

  return 1;
}

int
operator!= (const TAO_Unbounded_Sequence<CORBA::Octet> & lhs,
            const TAO_Unbounded_Sequence<CORBA::Octet> & rhs)
{
  return !(lhs == rhs);
}
