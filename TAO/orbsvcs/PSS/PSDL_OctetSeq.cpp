// $Id$

#include "PSDL_OctetSeq.h"

#include "ace/Malloc_Base.h"



// Constructor
TAO_PSDL_OctetSeq::TAO_PSDL_OctetSeq ()
  : allocator_ (0),
    buffer_ (0),
    length_ (0)
{
}

// Constructor
TAO_PSDL_OctetSeq::TAO_PSDL_OctetSeq (ACE_Allocator * persistent_allocator)
  : allocator_ (persistent_allocator),
    buffer_ (0),
    length_ (0)
{
}

// Destructor.
TAO_PSDL_OctetSeq::~TAO_PSDL_OctetSeq (void)
{
  if (this->buffer_ != 0)
    this->allocator_->free (this->buffer_);
}

// Copy constructor.
TAO_PSDL_OctetSeq::TAO_PSDL_OctetSeq (const TAO_PSDL_OctetSeq & rhs)
{
  *this = rhs;
}

// Assignment operator.
void
TAO_PSDL_OctetSeq::operator= (const TAO_PSDL_OctetSeq & rhs)
{
  if (this == &rhs)  // Prevent self assignment
    return;

  // Free the existing buffer before assigning a new
  // allocator and buffer.
  if (this->buffer_ != 0)
    this->allocator_->free (this->buffer_);

  this->allocator_ = rhs.allocator_;
  void * buf = this->allocator_->malloc (rhs.length_);
  this->buffer_ = static_cast<CORBA::Octet *> (buf);
  if (this->buffer_)
    {
      // Deep copy the buffer.
      ACE_OS::memcpy (this->buffer_, rhs.buffer_, rhs.length_);

      this->length_ = rhs.length_;
    }
  else
    this->length_ = 0;
}

void
TAO_PSDL_OctetSeq::operator= (const CORBA::OctetSeq & rhs)
{
  // Free the existing buffer before assigning octet
  // sequence.
  if (this->buffer_ != 0)
    this->allocator_->free (this->buffer_);

  const CORBA::ULong len = rhs.length ();

  void * buf = this->allocator_->malloc (len);
  this->buffer_ = static_cast<CORBA::Octet *> (buf);
  if (this->buffer_)
    {
      CORBA::Octet * dest = this->buffer_;
      const CORBA::Octet * src = rhs.get_buffer ();

      // Deep copy the buffer.
      ACE_OS::memcpy (dest, src, len);

      this->length_ = len;
    }
  else
    this->length_ = 0;
}

// Conversion operators (cast)
// (caller owns storage of return values)
TAO_PSDL_OctetSeq::operator CORBA::OctetSeq *() const
{
  CORBA::OctetSeq * tmp;
  ACE_NEW_RETURN (tmp,
                  CORBA::OctetSeq (this->length_),
                  0);
  CORBA::OctetSeq_var seq = tmp;

  seq->length (this->length_);

  CORBA::Octet * dest = seq->get_buffer ();
  const CORBA::Octet * src = this->buffer_;
  ACE_OS::memcpy (dest, src, this->length_);

  return seq._retn ();
}

TAO_PSDL_OctetSeq::operator CORBA::OctetSeq *()
{
  CORBA::OctetSeq * tmp;
  ACE_NEW_RETURN (tmp,
                  CORBA::OctetSeq (this->length_),
                  0);
  CORBA::OctetSeq_var seq = tmp;

  seq->length (this->length_);

  CORBA::Octet * dest = seq->get_buffer ();
  const CORBA::Octet * src = this->buffer_;
  ACE_OS::memcpy (dest, src, this->length_);

  return seq._retn ();
}
