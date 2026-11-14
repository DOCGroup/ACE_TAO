#if !defined (ACE_URL_PROPERTIES_C)
#define ACE_URL_PROPERTIES_C

#include "URL_Properties.h"

#if !defined (__ACE_INLINE__)
#include "URL_Properties.inl"
#endif /* __ACE_INLINE__ */

#include "URL_Array_Helper.h"
#include <cstring>
#include <memory>

static ACE_NS_WString
ace_url_wstring (ACE_UINT16 const *value)
{
  ACE_NS_WString result;
  if (value != nullptr)
    while (*value != 0)
      {
        result += static_cast<ACE_WSTRING_TYPE> (*value);
        ++value;
      }
  return result;
}

static ACE_NS_WString
ace_url_wstring_from_buffer (char const *buffer)
{
  ACE_NS_WString result;
  ACE_UINT16 value {};
  size_t offset {};
  do
    {
      std::memcpy (&value, buffer + offset, sizeof (value));
      if (value != 0)
        result += static_cast<ACE_WSTRING_TYPE> (value);
      offset += sizeof (value);
    }
  while (value != 0);
  return result;
}

ACE_URL_Property::ACE_URL_Property (ACE_UINT16 const *name,
                                    ACE_UINT16 const *value)
  : name_ (ace_url_wstring (name)),
    value_ (ace_url_wstring (value))
{
}

void
ACE_URL_Property::name (ACE_UINT16 const *name)
{
  this->name_ = ace_url_wstring (name);
}

void
ACE_URL_Property::value (ACE_UINT16 const *value)
{
  this->value_ = ace_url_wstring (value);
}

void
ACE_URL_Offer::url (ACE_UINT16 const *url)
{
  this->url_ = ace_url_wstring (url);
}

size_t
ACE_WString_Helper::encode (void *buf, const ACE_WString &wstr)
{
  char *bytes = static_cast<char *> (buf);
  size_t const count = wstr.length () + 1;
  for (size_t i = 0; i < count; ++i)
    {
      ACE_UINT16 const encoded_value = htons (wstr[i]);
      std::memcpy (bytes + i * sizeof (encoded_value), &encoded_value, sizeof (encoded_value));
    }
  return count * sizeof (ACE_UINT16);
}

size_t
ACE_WString_Helper::decode (void *buf)
{
  char *bytes = static_cast<char *> (buf);
  size_t count {};
  ACE_UINT16 encoded_value {};
  do
    {
      std::memcpy (&encoded_value, bytes + count * sizeof (encoded_value), sizeof (encoded_value));
      if (encoded_value != 0)
        {
          ACE_UINT16 const decoded_value = ntohs (encoded_value);
          std::memcpy (bytes + count * sizeof (decoded_value),
                       &decoded_value,
                       sizeof (decoded_value));
        }
      ++count;
    }
  while (encoded_value != 0);
  return count * sizeof (ACE_UINT16);
}

size_t
ACE_URL_Property::encode (void *buf) const
{
  size_t blen = ACE_WString_Helper::encode (buf, this->name_);
  blen += ACE_WString_Helper::encode (static_cast<char *> (buf) + blen,
                                      this->value_);
  return blen;
}

size_t
ACE_URL_Property::decode (void *buf)
{
  char *cbuf = static_cast<char *> (buf);
  size_t len = ACE_WString_Helper::decode(buf);
  this->name_ = ace_url_wstring_from_buffer (cbuf);

  cbuf += len;
  len += ACE_WString_Helper::decode (cbuf);
  this->value_ = ace_url_wstring_from_buffer (cbuf);
  return len;
}

void
ACE_URL_Property::dump () const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  if (this->name_.length () > 0)
    {
      std::unique_ptr<char[]> name (this->name_.char_rep ());
      ACE_DEBUG ((LM_DEBUG, "\n    name_:  \"%s\"\n",
                  name.get ()));
    }
  else
    ACE_DEBUG ((LM_DEBUG, "\n   name_:  \"\"\n"));

  if (this->value_.length () > 0)
    {
      std::unique_ptr<char[]> value (this->value_.char_rep ());
      ACE_DEBUG ((LM_DEBUG, "   value_:  \"%s\"\n",
                  value.get ()));
    }
  else
    ACE_DEBUG ((LM_DEBUG, "   value_:  \"\"\n"));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

size_t
ACE_URL_Offer::size () const
{
  size_t sum = (this->url_.length () + 1) * sizeof (ACE_UINT16);
  sum += ::ace_array_size (this->prop_);
  return sum;
}

size_t
ACE_URL_Offer::encode (void *buf) const
{
  ACE_UINT32 const encoded_size = htonl (this->prop_.size ());
  std::memcpy (buf, &encoded_size, sizeof (encoded_size));

  size_t len = sizeof (ACE_UINT32);
  len += ACE_WString_Helper::encode (static_cast<char *> (buf) + len,
                                     this->url_);

  len += ::ace_array_encode (static_cast<char *> (buf) + len,
                             this->prop_);
  return len;
}

size_t
ACE_URL_Offer::decode (void *buf)
{
  size_t len = sizeof (ACE_UINT32);
  ACE_UINT32 encoded_size;
  std::memcpy (&encoded_size, buf, sizeof (encoded_size));
  size_t a_size = static_cast<size_t> (ntohl (encoded_size));
  len += ACE_WString_Helper::decode (static_cast<char *> (buf) + len);
  this->url_ = ace_url_wstring_from_buffer (static_cast<char *> (buf) + len);

  ACE_URL_Property_Seq prop_seq (a_size);
  this->url_properties (prop_seq);

  len += ::ace_array_decode (static_cast<char *> (buf) + len,
                             this->prop_);
  return len;
}

void
ACE_URL_Offer::dump () const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  if (this->url_.length () > 0)
    {
      std::unique_ptr<char[]> url (this->url_.char_rep ());
      ACE_DEBUG ((LM_DEBUG, "\n url_:  \"%s\"\n",
                  url.get ()));
    }
  else
    ACE_DEBUG ((LM_DEBUG, "\n url_:  \"\"\n"));

  for (size_t i = 0; i < this->prop_.size (); ++i)
    this->prop_[i].dump ();

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#endif /* ACE_URL_PROPERTIES_C */
