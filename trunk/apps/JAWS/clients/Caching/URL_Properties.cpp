// URL_Locator.cpp
// $Id$

#if !defined (ACE_URL_PROPERTIES_C)
#define ACE_URL_PROPERTIES_C

#define ACE_BUILD_DLL
#include "URL_Properties.h"

#if !defined (__ACE_INLINE__)
#include "URL_Properties.i"
#endif /* __ACE_INLINE__ */

#include "ace/OS.h"

template <class T>
size_t ace_array_bsize (T &x)
{
  size_t sum = sizeof (ACE_UINT32);
  for (int i = 0; i < x.size (); i++)
    sum += x[i].bsize ();
  return sum;
}

template <class T>
void ace_array_encode (void *buf, T &x)
{
  size_t len = 0;
  for (size_t i = 0; i < x.size (); i++)
    {
      x[i].encode ((void *) ((char *) buf + len));
      len += x[i].bsize ();
    }
}

template <class T>
void ace_array_decode (void *buf, T &x)
{
  size_t len = 0;
  for (size_t i = 0; i < x.size (); i++)
    {
      x[i].decode ((void *) ((char *) buf + len));
      len += x[i].bsize ();
    }
}

void
ACE_URL_Property::encode (void *buf) const
{
  size_t len = (this->name_->length () + 1) * sizeof (ACE_USHORT16);
  ACE_OS::memcpy (buf, (void *) this->name_->fast_rep (), len);
  ACE_OS::memcpy ((void *) ((char *) buf + len),
		  (void *) this->value_->fast_rep(),
		  (this->value_->length () + 1) * sizeof (ACE_USHORT16));
  ACE_USHORT16 *m_buf = (ACE_USHORT16 *) buf;
  for (size_t i = 0; i < this->name_->length () + this->value_->length () + 2 ;
       i++)
    m_buf[i] = htons (m_buf[i]);
}

ACE_URL_Property *
ACE_URL_Property::decode (void *buf)
{
  ACE_USHORT16 *n = (ACE_USHORT16 *) buf;
  int i;

  for (i = 0; n[i] != 0; i++)
    n[i] = ntohs (n[i]);
  ACE_USHORT16 *val = &n[i+1];
  for (i = 0; val[i] != 0; i++)
    val[i] = ntohs (val[i]);
  this->name (n);
  this->value (val);
  return this ;
}

size_t
ACE_URL_Offer::bsize (void) const
{
  size_t sum = ACE_OS::strlen (this->url_) + 1;
  sum += ::ace_array_bsize (this->prop_);
  return sum;
}

void
ACE_URL_Offer::encode (void *buf) const
{
  ACE_UINT32 *s_buf = (ACE_UINT32 *) buf;
  *s_buf = htonl (this->prop_.size ());

  size_t len = ACE_OS::strlen (this->url_) + 1;
  ACE_OS::memcpy ((void *) ((char *) buf + sizeof (ACE_UINT32)),
		  this->url_, len);

  len += sizeof (ACE_UINT32);
  ::ace_array_encode ((void *) ((char *) buf + len), this->prop_);
}

ACE_URL_Offer *
ACE_URL_Offer::decode (void *buf)
{
  size_t a_size = (size_t) ntohl ((ACE_UINT32 *) buf);
  this->url ((const char *) buf + sizeof (ACE_UINT32));

  ACE_URL_Property_Seq prop_seq (a_size);
  this->url_properties (prop_seq);
  // Set this property array to correct size.
  
  ::ace_array_decode ((void *)((char *) buf + ACE_OS::strlen (this->url ())
			       + 1 + sizeof (ACE_UINT32)),
		      this->prop_);
  return this;
}

#endif /* ACE_URL_PROPERTIES_C */
