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
  ACE_URL_Property *retv;
  int i;

  for (i = 0; n[i] != 0; i++)
    n[i] = ntohs (n[i]);
  ACE_USHORT16 *val = &n[i+1];
  for (i = 0; val[i] != 0; i++)
    val[i] = ntohs (val[i]);
  ACE_NEW_RETURN (retv, ACE_URL_Property (n, val), 0);
  return retv;
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
  size_t len = ACE_OS::strlen (this->url_) + 1;
  ACE_OS::memcpy (buf, this->url_, len);

  ACE_UINT32 *s_buf = (ACE_UINT32 *) ((char *) buf + len);
  *s_buf = htonl (this->prop_.size ());
  len += sizeof (ACE_UINT32);

  for (int i = 0; i < this->prop_.size (); i++)
    {
      this->prop_[i].encode ((void *) ((char *) buf + len));
      len += this->prop_[i].bsize ();
    }
}

ACE_URL_Offer *
ACE_URL_Offer::decode (void *buf)
{
  ACE_URL_Offer *retv;

  ACE_NEW_RETURN (retv, ACE_URL_Offer);

  //
  // Unfinished.
  //

  return 0;
}

#endif /* ACE_URL_PROPERTIES_C */
