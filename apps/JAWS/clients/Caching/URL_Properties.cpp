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

char ACE_URL_Offer::NULL_STRING = '\0';

template <class T>
size_t ace_array_bsize (const T &x)
{
  size_t sum = sizeof (ACE_UINT32);
  for (size_t i = 0; i < x.size (); i++)
    sum += x[i].bsize ();
  return sum;
}

template <class T>
void ace_array_encode (void *buf, const T &x)
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

ACE_URL_Property::~ACE_URL_Property (void)
{
  delete this->name_;
  delete this->value_;
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

void
ACE_URL_Property::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  if (this->name_->length () > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "\n    name_:  \"%s\"\n", 
		  ACE_Auto_Basic_Array_Ptr<char> (this->name_->char_rep ()).get ()));
    }
  else
    ACE_DEBUG ((LM_DEBUG, "\n   name_:  \"\"\n"));
  if (this->value_->length () > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "   value_:  \"%s\"\n",
		  ACE_Auto_Basic_Array_Ptr<char> (this->value_->char_rep ()).get ()));
    }
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

void
ACE_URL_Offer::encode (void *buf) const
{
  ACE_UINT32 *s_buf = (ACE_UINT32 *) buf;
  *s_buf = htonl (this->prop_.size ());

  size_t len = (this->url_->length () + 1) * sizeof (ACE_USHORT16);
  ACE_USHORT16 *w_buf = (ACE_USHORT16 *) ((char *) buf + sizeof (ACE_UINT32));

  ACE_OS::memcpy ((void *) w_buf,
		  this->url_->fast_rep (), len);
  for (int i = 0; w_buf[i] != 0; i++)
    w_buf[i] = htons (w_buf[i]);

  len += sizeof (ACE_UINT32);
  ::ace_array_encode ((void *) ((char *) buf + len), this->prop_);
}

ACE_URL_Offer *
ACE_URL_Offer::decode (void *buf)
{
  size_t a_size = (size_t) ntohl (* (ACE_UINT32 *) buf);
  ACE_USHORT16 *url = (ACE_USHORT16 *) ((char *) buf + sizeof (ACE_UINT32));
  for (int i = 0; url[i] != 0; i++)
    url[i] = ntohs (url[i]);
  this->url (url);

  ACE_URL_Property_Seq prop_seq (a_size);
  this->url_properties (prop_seq);
  
  ::ace_array_decode ((void *)((char *) buf + (this->url_->length () + 1) *
			       sizeof (ACE_USHORT16) + sizeof (ACE_UINT32)),
 		      this->prop_);
  return this;
}

void
ACE_URL_Offer::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  if (this->url_->length () > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "\n url_:  \"%s\"\n", 
		  ACE_Auto_Basic_Array_Ptr<char> (this->url_->char_rep ()).get ()));
    }
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
template void  ace_array_encode (void *, ACE_Array<ACE_URL_Property> &);
template void  ace_array_decode (void *, ACE_Array<ACE_URL_Property> &);
template size_t ace_array_bsize (ACE_Array<ACE_URL_Offer> &);
template void  ace_array_encode (void *, ACE_Array<ACE_URL_Offer> &);
template void  ace_array_decode (void *, ACE_Array<ACE_URL_Offer> &);
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_Basic_Array_Ptr<char>
#pragma instantiate ACE_Array<ACE_URL_Property>
#pragma instantiate ACE_Array<ACE_URL_Offer>
#pragma instantiate size_t ace_array_bsize (ACE_Array<ACE_URL_Property> &)
#pragma instantiate void  ace_array_encode (void *, ACE_Array<ACE_URL_Property> &)
#pragma instantiate void  ace_array_decode (void *, ACE_Array<ACE_URL_Property> &)
#pragma instantiate size_t ace_array_bsize (ACE_Array<ACE_URL_Offer> &)
#pragma instantiate void  ace_array_encode (void *, ACE_Array<ACE_URL_Offer> &)
#pragma instantiate void  ace_array_decode (void *, ACE_Array<ACE_URL_Offer> &)
#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /* ACE_URL_PROPERTIES_C */
