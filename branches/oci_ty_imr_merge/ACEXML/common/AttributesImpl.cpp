// -*- C++ -*-  $Id$

#include "ACEXML/common/AttributesImpl.h"

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/common/AttributesImpl.i"
#endif /* __ACEXML_INLINE__ */

ACEXML_AttributesImpl::ACEXML_AttributesImpl (int size)
  : attrs_ (size)
{
  this->attrs_.size (0);        // attrs array contains nothing
}

ACEXML_AttributesImpl::ACEXML_AttributesImpl (const
                                              ACEXML_AttributesImpl &attrs)
  : ACEXML_Attributes (attrs),
    attrs_ (attrs.attrs_.size ())
{
  for (size_t i = 0; i < attrs.attrs_.size (); i++)
    this->attrs_[i] = attrs.attrs_[i];
}

ACEXML_AttributesImpl::~ACEXML_AttributesImpl (void)
{
}

int
ACEXML_AttributesImpl::addAttribute (const ACEXML_Char *uri,
                                     const ACEXML_Char *localName,
                                     const ACEXML_Char *qName,
                                     const ACEXML_Char *type,
                                     const ACEXML_Char *value)
{
  if (this->isDuplicate (uri, localName, qName))
    return -1;
  size_t length = this->attrs_.size ();
  this->attrs_.size (length+1);
  this->setAttribute (length,
                      uri,
                      localName,
                      qName,
                      type,
                      value);
  return ACE_static_cast (int, length);
}

int
ACEXML_AttributesImpl::addAttribute (const ACEXML_Attribute &att)
{
  if (this->isDuplicate (att.uri(), att.localName(), att.qName()))
    return -1;
  size_t length = this->attrs_.size ();
  this->attrs_.size (length+1);
  this->attrs_[length] = att;
  return ACE_static_cast (int, length);
}

int
ACEXML_AttributesImpl::isDuplicate (const ACEXML_Char *uri,
                                    const ACEXML_Char *localName,
                                    const ACEXML_Char *qName)
{
  for (size_t i = 0; i < this->attrs_.size(); ++i)
    {
      if (ACE_OS::strcmp (this->attrs_[i].localName(), localName) == 0)
        {
          if (qName != 0 && this->attrs_[i].qName() != 0
              && ACE_OS::strcmp (this->attrs_[i].qName(), qName) == 0)
            {
              if (uri != 0 && this->attrs_[i].uri() != 0
                  && ACE_OS::strcmp (this->attrs_[i].uri(), uri) == 0)
                return 1;
            }
        }
    }
  return 0;
}

int
ACEXML_AttributesImpl::removeAttribute (size_t index)
{
  size_t length = this->attrs_.size ();

  if (index >= length)
    return -1;

  this->attrs_[index] = this->attrs_[length - 1];
  this->attrs_.size (length - 1);

  return 0;
}


int
ACEXML_AttributesImpl::getIndex (const ACEXML_Char *qName)
{
  for (size_t i = 0; i < this->attrs_.size (); i++)
    if (ACE_OS::strcmp (qName, this->attrs_[i].qName ()) == 0)
      return ACE_static_cast (int, i);

  return -1;
}

int
ACEXML_AttributesImpl::getIndex (const ACEXML_Char *uri,
                                 const ACEXML_Char *localPart)
{
  for (size_t i = 0; i < this->attrs_.size (); i++)
    if (ACE_OS::strcmp (uri, this->attrs_[i].uri ()) == 0 &&
        ACE_OS::strcmp (localPart, this->attrs_[i].localName ()) == 0)
      return ACE_static_cast (int, i);

  return -1;
}

size_t
ACEXML_AttributesImpl::getLength (void)
{
  return this->attrs_.size ();
}

const ACEXML_Char *
ACEXML_AttributesImpl::getLocalName (size_t index)
{
  if (index < this->attrs_.size ())
    return this->attrs_[index].localName ();
  return 0;
}


const ACEXML_Char *
ACEXML_AttributesImpl::getQName (size_t index)
{
  if (index < this->attrs_.size ())
    return this->attrs_[index].qName ();
  return 0;
}

const ACEXML_Char *
ACEXML_AttributesImpl::getType (size_t index)
{
  if (index < this->attrs_.size ())
    return this->attrs_[index].type ();
  return 0;
}


const ACEXML_Char *
ACEXML_AttributesImpl::getType (const ACEXML_Char *qName)
{
  for (size_t i = 0; i < this->attrs_.size (); i++)
    if (ACE_OS::strcmp (qName, this->attrs_[i].qName ()) == 0)
      return this->attrs_[i].type ();

  return 0;
}

const ACEXML_Char *
ACEXML_AttributesImpl::getType (const ACEXML_Char *uri,
                                const ACEXML_Char *localPart)
{
  for (size_t i = 0; i < this->attrs_.size (); i++)
    if (ACE_OS::strcmp (uri, this->attrs_[i].uri ()) == 0 &&
        ACE_OS::strcmp (localPart, this->attrs_[i].localName ()) == 0)
      return this->attrs_[i].type ();

  return 0;
}


const ACEXML_Char *
ACEXML_AttributesImpl::getURI (size_t index)
{
  if (index < this->attrs_.size ())
    return this->attrs_[index].uri ();
  return 0;
}


const ACEXML_Char *
ACEXML_AttributesImpl::getValue (size_t index)
{
  if (index < this->attrs_.size ())
    return this->attrs_[index].value ();
  return 0;
}

const ACEXML_Char *
ACEXML_AttributesImpl::getValue (const ACEXML_Char *qName)
{
  for (size_t i = 0; i < this->attrs_.size (); i++)
    if (ACE_OS::strcmp (qName, this->attrs_[i].qName ()) == 0)
      return this->attrs_[i].value ();

  return 0;
}

const ACEXML_Char *
ACEXML_AttributesImpl::getValue (const ACEXML_Char *uri,
                                 const ACEXML_Char *localPart)
{
  for (size_t i = 0; i < this->attrs_.size (); i++)
    if (ACE_OS::strcmp (uri, this->attrs_[i].uri ()) == 0 &&
        ACE_OS::strcmp (localPart, this->attrs_[i].localName ()) == 0)
      return this->attrs_[i].value ();

  return 0;
}

int
ACEXML_AttributesImpl::setAttribute (size_t index,
                                     const ACEXML_Char *uri,
                                     const ACEXML_Char *localName,
                                     const ACEXML_Char *qName,
                                     const ACEXML_Char *type,
                                     const ACEXML_Char *value)
{
  if (index < this->attrs_.size ())
    {
      this->attrs_[index].setAttribute (uri,
                                        localName,
                                        qName,
                                        type,
                                        value);
      return 0;
    }

  return -1;
}


int
ACEXML_AttributesImpl::setLocalName (size_t index,
                                     const ACEXML_Char *localName)
{
  if (index < this->attrs_.size ())
    {
      this->attrs_[index].localName (localName);
      return 0;
    }
  return -1;
}

int
ACEXML_AttributesImpl::setQName (size_t index,
                                 const ACEXML_Char *qName)
{
  if (index < this->attrs_.size ())
    {
      this->attrs_[index].qName (qName);
      return 0;
    }
  return -1;
}


int
ACEXML_AttributesImpl::setURI (size_t index,
                               const ACEXML_Char *uri)
{
  if (index < this->attrs_.size ())
    {
      this->attrs_[index].uri (uri);
      return 0;
    }
  return -1;
}

int
ACEXML_AttributesImpl::setType (size_t index,
                                const ACEXML_Char *type)
{
  if (index < this->attrs_.size ())
    {
      this->attrs_[index].type (type);
      return 0;
    }
  return -1;
}

int
ACEXML_AttributesImpl::setValue (size_t index,
                                 const ACEXML_Char *value)
{
  if (index < this->attrs_.size ())
    {
      this->attrs_[index].value (value);
      return 0;
    }
  return -1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Array_Base<ACEXML_Attribute>;
template class ACE_Array<ACEXML_Attribute>;
template class ACE_Array_Iterator<ACEXML_Attribute>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Array_Base<ACEXML_Attribute>
#pragma instantiate ACE_Array<ACEXML_Attribute>
#pragma instantiate ACE_Array_Iterator<ACEXML_Attribute>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
