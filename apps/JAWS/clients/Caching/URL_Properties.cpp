// URL_Properties.cpp
// $Id$

#if !defined (ACE_URL_PROPERTIES_C)
#define ACE_URL_PROPERTIES_C

#define ACE_BUILD_DLL
#include "URL_Properties.h"

#if !defined (__ACE_INLINE__)
#include "URL_Properties.i"
#endif /* __ACE_INLINE__ */

#include "ace/OS.h"
#include "ace/Auto_Ptr.h"
#include "URL_Array_Helper.h"

size_t
ACE_WString_Helper::encode (void *buf, ACE_WString *wstr)
{
  ACE_USHORT16 *wptr = (ACE_USHORT16 *) buf;
  size_t i;

  for (i= 0; i <= wstr->length (); i++)
    wptr[i] = htons ((*wstr)[i]);

  return i * sizeof (ACE_USHORT16);
}

size_t
ACE_WString_Helper::decode (void *buf)
{
  ACE_USHORT16 *wptr = (ACE_USHORT16 *) buf;
  size_t i;
  
  for (i = 0; wptr[i] != 0; i++)
    wptr[i] = ntohs (wptr[i]);

  return (i + 1) * sizeof (ACE_USHORT16);
}

ACE_URL_Property::~ACE_URL_Property (void)
{
  delete this->name_;
  delete this->value_;
}

size_t
ACE_URL_Property::encode (void *buf) const
{
  size_t blen = ACE_WString_Helper::encode (buf, this->name_);
  blen += ACE_WString_Helper::encode ((void *) ((char *) buf + blen),
				      this->value_);
  return blen;
}

size_t
ACE_URL_Property::decode (void *buf)
{
  char *cbuf = (char *) buf;
  size_t len = ACE_WString_Helper::decode(buf);
  this->name ((ACE_USHORT16 *) cbuf);

  cbuf += len;
  len += ACE_WString_Helper::decode ((void *) cbuf);
  return len;
}

void
ACE_URL_Property::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  if (this->name_->length () > 0)
    ACE_DEBUG ((LM_DEBUG, "\n    name_:  \"%s\"\n", 
		ACE_Auto_Basic_Array_Ptr<char> (this->name_->char_rep ()).get ()));
  else
    ACE_DEBUG ((LM_DEBUG, "\n   name_:  \"\"\n"));

  if (this->value_->length () > 0)
    ACE_DEBUG ((LM_DEBUG, "   value_:  \"%s\"\n",
		ACE_Auto_Basic_Array_Ptr<char> (this->value_->char_rep ()).get ()));
  else
    ACE_DEBUG ((LM_DEBUG, "   value_:  \"\"\n"));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_URL_Offer::~ACE_URL_Offer (void)
{
  delete this->url_;
}

size_t
ACE_URL_Offer::bsize (void) const
{
  size_t sum = (this->url_->length () + 1) * sizeof (ACE_USHORT16);
  sum += ::ace_array_bsize (this->prop_);
  return sum;
}

size_t
ACE_URL_Offer::encode (void *buf) const
{
  ACE_UINT32 *s_buf = (ACE_UINT32 *) buf;
  *s_buf = htonl (this->prop_.size ());

  size_t len = sizeof (ACE_UINT32);
  len += ACE_WString_Helper::encode ((void *) ((char *) buf + len),
				     this->url_);
  
  len += ::ace_array_encode ((void *) ((char *) buf + len),
			     this->prop_);
  return len;
}

size_t
ACE_URL_Offer::decode (void *buf)
{
  size_t len = sizeof (ACE_UINT32);
  size_t a_size = (size_t) ntohl (*(ACE_UINT32 *) buf);
  len += ACE_WString_Helper::decode ((void *) ((char *) buf + len));
  this->url ((ACE_USHORT16 *) ((char *) buf + len));
  
  ACE_URL_Property_Seq prop_seq (a_size);
  this->url_properties (prop_seq);
  
  len += ::ace_array_decode ((void *)((char *) buf + len),
			     this->prop_);
  return len;
}

void
ACE_URL_Offer::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  if (this->url_->length () > 0)
    ACE_DEBUG ((LM_DEBUG, "\n url_:  \"%s\"\n", 
		ACE_Auto_Basic_Array_Ptr<char> (this->url_->char_rep ()).get ()));
  else
    ACE_DEBUG ((LM_DEBUG, "\n url_:  \"\"\n"));

  for (size_t i = 0; i < this->prop_.size (); i++)
    this->prop_[i].dump ();

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Auto_Basic_Array_Ptr<char>;
template class ACE_Array<ACE_URL_Property>;
template class ACE_Array<ACE_URL_Offer>;
template size_t ace_array_bsize (ACE_Array<ACE_URL_Property> &);
template size_t ace_array_encode (void *, ACE_Array<ACE_URL_Property> &);
template size_t ace_array_decode (void *, ACE_Array<ACE_URL_Property> &);
template size_t ace_array_bsize (ACE_Array<ACE_URL_Offer> &);
template size_t ace_array_encode (void *, ACE_Array<ACE_URL_Offer> &);
template size_t ace_array_decode (void *, ACE_Array<ACE_URL_Offer> &);
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_Basic_Array_Ptr<char>
#pragma instantiate ACE_Array<ACE_URL_Property>
#pragma instantiate ACE_Array<ACE_URL_Offer>
#pragma instantiate size_t ace_array_bsize (ACE_Array<ACE_URL_Property> &)
#pragma instantiate size_t ace_array_encode (void *, ACE_Array<ACE_URL_Property> &)
#pragma instantiate size_t ace_array_decode (void *, ACE_Array<ACE_URL_Property> &)
#pragma instantiate size_t ace_array_bsize (ACE_Array<ACE_URL_Offer> &)
#pragma instantiate size_t ace_array_encode (void *, ACE_Array<ACE_URL_Offer> &)
#pragma instantiate size_t ace_array_decode (void *, ACE_Array<ACE_URL_Offer> &)
#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /* ACE_URL_PROPERTIES_C */
