// $Id$

#include "tao/Sequence.h"

#if !defined (__ACE_INLINE__)
#include "tao/Sequence.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Sequence, "$Id$")

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
TAO_Base_Sequence::_downcast (void*,
                              CORBA_Object*,
                              CORBA_Environment &)
{
  // default is no op.
  // @@ TODO Maybe throw an exception?
}

CORBA_Object*
TAO_Base_Sequence::_upcast (void*) const
{
  return 0;
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

// constructor for unbounded seq
TAO_Unbounded_String_Sequence::
TAO_Unbounded_String_Sequence (CORBA::ULong maximum)
  : TAO_Unbounded_Base_Sequence (maximum,
                                 TAO_Unbounded_String_Sequence::allocbuf (maximum))
{
}

TAO_Unbounded_String_Sequence::
TAO_Unbounded_String_Sequence (const TAO_Unbounded_String_Sequence &rhs)
  : TAO_Unbounded_Base_Sequence (rhs)
{
  char* *tmp1 = TAO_Unbounded_String_Sequence::allocbuf (this->maximum_);
  char ** const tmp2 =
    ACE_reinterpret_cast (char ** ACE_CAST_CONST, rhs.buffer_);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    tmp1[i] = CORBA::string_dup (tmp2[i]);

  this->buffer_ = tmp1;
}

TAO_Unbounded_String_Sequence::~TAO_Unbounded_String_Sequence (void)
{
  this->_deallocate_buffer ();
}

TAO_Unbounded_String_Sequence&
TAO_Unbounded_String_Sequence::
operator= (const TAO_Unbounded_String_Sequence &rhs)
{
  if (this == &rhs)
    return *this;

  if (this->release_)
    {
      char **tmp = ACE_reinterpret_cast (char **, this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          CORBA::string_free (tmp[i]);
          tmp[i] = 0;
        }

      if (this->maximum_ < rhs.maximum_)
        {
          // free the older buffer
          TAO_Unbounded_String_Sequence::freebuf (tmp);
          this->buffer_ =
            TAO_Unbounded_String_Sequence::allocbuf (rhs.maximum_);
        }
    }
  else
    this->buffer_ =
      TAO_Unbounded_String_Sequence::allocbuf (rhs.maximum_);

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  char **tmp1 = ACE_reinterpret_cast (char **, this->buffer_);
  char ** const tmp2 = ACE_reinterpret_cast (char ** ACE_CAST_CONST,
                                             rhs.buffer_);
  for (CORBA::ULong i=0; i < rhs.length_; ++i)
    tmp1[i] = CORBA::string_dup (tmp2[i]);

  return *this;
}

TAO_SeqElem_String_Manager
TAO_Unbounded_String_Sequence::operator[] (CORBA::ULong index) const
{
  ACE_ASSERT (index < this->maximum_);
  char **const tmp =
    ACE_reinterpret_cast (char **ACE_CAST_CONST, this->buffer_);
  return TAO_SeqElem_String_Manager (tmp + index, this->release_);
}

char* *
TAO_Unbounded_String_Sequence::allocbuf (CORBA::ULong nelems)
{
  char* *buf = new char*[nelems];
  for (CORBA::ULong i=0; i < nelems; i++)
    buf[i] = 0;
  return buf;
}

void
TAO_Unbounded_String_Sequence::freebuf (char* *buffer)
{
  if (buffer == 0)
    return;

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

  delete[] buffer;
}

char**
TAO_Unbounded_String_Sequence::get_buffer (CORBA::Boolean orphan)
{
  char* *result = 0;
  if (orphan == 0)
    {
      // We retain ownership.
      if (this->buffer_ == 0)
        {
          result = allocbuf (this->length_);
          this->buffer_ = result;
        }
      else
        {
          result = ACE_reinterpret_cast (char**, this->buffer_);
        }
    }
  else // if (orphan == 1)
    {
      if (this->release_ != 0)
        {
          // We set the state back to default and relinquish
          // ownership.
          result = ACE_reinterpret_cast (char**, this->buffer_);
          this->maximum_ = 0;
          this->length_ = 0;
          this->buffer_ = 0;
          this->release_ = 0;
        }
    }
  return result;
}

const char**
TAO_Unbounded_String_Sequence::get_buffer (void) const
{
  return ACE_reinterpret_cast (const char** ACE_CAST_CONST, this->buffer_);
}

void
TAO_Unbounded_String_Sequence::_allocate_buffer (CORBA::ULong length)
{
  char* *tmp = TAO_Unbounded_String_Sequence::allocbuf (length);

  if (this->buffer_ != 0)
    {
      char* *old = ACE_reinterpret_cast (char **, this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        // Only call duplicate when we did not own the previous
        // buffer, since after this method we own it we must also
        // own the objects.  If we already own the objects there is
        // no need to copy them, if we did we would also have to
        // remove the old instances.
        if (!this->release_)
          tmp [i] = CORBA::string_dup (old[i]);
        else
          tmp [i] = old[i];

      if (this->release_)
        delete[] old;
    }
  this->buffer_ = tmp;
}

void
TAO_Unbounded_String_Sequence::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    return;

  char **tmp = ACE_reinterpret_cast (char **, this->buffer_);

  for (CORBA::ULong i = 0;
       i < this->length_;
       ++i)
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
  char **tmp = ACE_reinterpret_cast (char **, this->buffer_);
  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      CORBA::string_free (tmp[i]);
      tmp[i] = 0;
    }
}

// ****************************************************************

#if defined (TAO_NO_COPY_OCTET_SEQUENCES)
TAO_Unbounded_Sequence<CORBA::Octet>::
TAO_Unbounded_Sequence (const TAO_Unbounded_Sequence<CORBA::Octet> &rhs)
  : TAO_Unbounded_Base_Sequence (rhs),
    mb_ (0)
{
  CORBA::Octet *tmp1 = TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (this->maximum_);
  CORBA::Octet * const tmp2 = ACE_reinterpret_cast (CORBA::Octet * ACE_CAST_CONST, rhs.buffer_);

  // for (CORBA::ULong i = 0; i < this->length_; ++i)
  // tmp1[i] = tmp2[i];
  if (rhs.mb_ == 0)
    ACE_OS::memcpy (tmp1, tmp2, this->length_);
  else
    {
      size_t offset = 0;
      for (const ACE_Message_Block *i = rhs.mb_;
           i != 0;
           i = i->cont ())
        {
          ACE_OS::memcpy (tmp1 + offset, i->rd_ptr (), i->length ());
          offset += i->length ();
        }
    }

  this->buffer_ = tmp1;
}

TAO_Unbounded_Sequence<CORBA::Octet> &
TAO_Unbounded_Sequence<CORBA::Octet>::operator= (const TAO_Unbounded_Sequence<CORBA::Octet> &rhs)
{
  if (this == &rhs)
    return *this;

  if (this->mb_ != 0)
    {
      ACE_Message_Block::release (this->mb_);
      this->mb_ = 0;
      this->buffer_ =
        TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (rhs.length_);
    }
  else if (this->release_)
    {
      if (this->maximum_ < rhs.length_)
        {
          // free the old buffer
          CORBA::Octet *tmp = ACE_reinterpret_cast (CORBA::Octet *,
                                                    this->buffer_);
          TAO_Unbounded_Sequence<CORBA::Octet>::freebuf (tmp);
          this->buffer_ =
            TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (rhs.length_);
        }
    }
  else
    this->buffer_ =
      TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (rhs.length_);

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  CORBA::Octet *tmp1 = ACE_reinterpret_cast (CORBA::Octet *, this->buffer_);
  CORBA::Octet * const tmp2 = ACE_reinterpret_cast (CORBA::Octet * ACE_CAST_CONST, rhs.buffer_);

  // for (CORBA::ULong i = 0; i < this->length_; ++i)
  // tmp1[i] = tmp2[i];
  if (rhs.mb_ == 0)
    ACE_OS::memcpy (tmp1, tmp2, this->length_);
  else
    {
      size_t offset = 0;
      for (const ACE_Message_Block *i = rhs.mb_;
           i != 0;
           i = i->cont ())
        {
          ACE_OS::memcpy (tmp1 + offset, i->rd_ptr (), i->length ());
          offset += i->length ();
        }
    }

  return *this;
}

TAO_Unbounded_Sequence<CORBA::Octet>::
TAO_Unbounded_Sequence (CORBA::ULong length,
                        const ACE_Message_Block *mb)
  :  TAO_Unbounded_Base_Sequence (length,
                                  length,
                                  mb->rd_ptr (),
                                  0),
     mb_ (ACE_Message_Block::duplicate (mb))
{
}

TAO_Unbounded_Sequence<CORBA::Octet>::~TAO_Unbounded_Sequence (void)
{
  this->_deallocate_buffer ();
}

void
TAO_Unbounded_Sequence<CORBA::Octet>::_allocate_buffer (CORBA::ULong length)
{
  CORBA::Octet *tmp = TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (length);

  if (this->buffer_ != 0)
    {
      CORBA::Octet *old = ACE_reinterpret_cast (CORBA::Octet *, this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        tmp[i] = old[i];

      if (this->mb_ != 0)
        {
          ACE_Message_Block::release (this->mb_);
          this->mb_ = 0;
        }
      else if (this->release_)
        {
          TAO_Unbounded_Sequence<CORBA::Octet>::freebuf (old);
        }
    }

  this->buffer_ = tmp;
}

void TAO_Unbounded_Sequence<CORBA::Octet>::_deallocate_buffer (void)
{
  if (this->mb_ == 0
      && this->buffer_ != 0
      && this->release_ != 0)
    {
      CORBA::Octet *tmp = ACE_reinterpret_cast (CORBA::Octet *, this->buffer_);
      TAO_Unbounded_Sequence<CORBA::Octet>::freebuf (tmp);
    }
  else
    {
      ACE_Message_Block::release (this->mb_);
      this->mb_ = 0;
    }
  this->buffer_ = 0;
}

#endif /* defined (TAO_NO_COPY_OCTET_SEQUENCES) */
