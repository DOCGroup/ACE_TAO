// $Id$

#include "PSDL_String.h"

#include "ace/Malloc_Base.h"
#include "ace/SString.h"



// Constructor
TAO_PSDL_String::TAO_PSDL_String ()
  : allocator_ (0),
    buffer_ (0),
    length_ (0)
{
}

// Constructor
TAO_PSDL_String::TAO_PSDL_String (ACE_Allocator * persistent_allocator)
  : allocator_ (persistent_allocator),
    buffer_ (0),
    length_ (0)
{
}

// Destructor.
TAO_PSDL_String::~TAO_PSDL_String (void)
{
  if (this->buffer_ != 0)
    this->allocator_->free ((void *) this->buffer_);
}

// Copy constructor.
TAO_PSDL_String::TAO_PSDL_String (const TAO_PSDL_String & rhs)
{
  *this = rhs;
}

// Assignment operator.
void
TAO_PSDL_String::operator= (const TAO_PSDL_String & rhs)
{
  if (this == &rhs)  // Prevent self assignment
    return;

  // Free the existing buffer before assigning a new
  // allocator and buffer.
  if (this->buffer_ != 0)
    this->allocator_->free ((void *) this->buffer_);

  this->allocator_ = rhs.allocator_;
  void * buf = this->allocator_->malloc (rhs.length_);
  this->buffer_ = (const char *) buf;
  if (this->buffer_)
    {
      // Deep copy the buffer.
      ACE_OS::memcpy ( (void *) this->buffer_,
                       rhs.buffer_,
                       rhs.length_);

      this->length_ = rhs.length_;
    }
  else
    this->length_ = 0;
}

void
TAO_PSDL_String::operator= (const ACE_CString & rhs)
{
  // Free the existing buffer before assigning octet
  // sequence.
  if (this->buffer_ != 0)
    this->allocator_->free ((void *) this->buffer_);

  const CORBA::ULong len = rhs.length ();

  void * buf = this->allocator_->malloc (len);

  this->buffer_ = (const char *) buf;

  if (this->buffer_)
    {
      const char * dest = this->buffer_;
      const char * src = rhs.rep ();

      // Deep copy the buffer.
      ACE_OS::memcpy ((void *)dest,
                      src,
                      len);

      this->length_ = len;
    }
  else
    this->length_ = 0;
}

bool
TAO_PSDL_String::operator== (const TAO_PSDL_String &rhs) const
{
  return (ACE_OS::strcmp (buffer_, rhs.buffer_) == 0);
}

bool
TAO_PSDL_String::operator!= (const TAO_PSDL_String &rhs) const
{
  return (ACE_OS::strcmp (buffer_, rhs.buffer_) != 0);
}

u_long
TAO_PSDL_String::hash (void) const
{
  return ACE::hash_pjw (this->buffer_);
}

// Conversion operators (cast)
// (caller owns storage of return values)
TAO_PSDL_String::operator ACE_CString *() const
{
  ACE_CString *str = 0;

  char * dest = str->rep ();
  const char * src = this->buffer_;

  ACE_OS::memcpy (dest, src, this->length_);

  return str;
}

TAO_PSDL_String::operator ACE_CString *()
{
  ACE_CString *str = 0;

  char * dest = str->rep ();
  const char * src = this->buffer_;
  ACE_OS::memcpy (dest, src, this->length_);

  return str;
}
