// URL_Locator.cpp
// $Id$

#if !defined (ACE_URL_LOCATOR_C)
#define ACE_URL_LOCATOR_C

#define ACE_BUILD_DLL
#include "URL_Locator.h"

#if !defined (__ACE_INLINE__)
#include "URL_Locator.i"
#endif /* __ACE_INLINE__ */

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
}

ACE_URL_Property *
ACE_URL_Property::decode (void *buf)
{
  ACE_USHORT16 *n = (ACE_USHORT16 *) buf;
  ACE_USHORT16 *val = n;
  ACE_URL_Property *retv;

  for (int i = 0; n[i] != 0; i++) ;
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

#endif /* ACE_URL_LOCATOR_C */
