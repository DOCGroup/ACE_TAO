/* -*- C++ -*- */

// $Id$

ACE_INLINE
ACE_URL_Locator_Request::ACE_URL_Locator_Request (void)
  : how_(INVALID_REQUEST),
    seq1_ (0),
    seq2_ (0),
    offer_ (0),
    buffer_ (0)
{
}

ACE_INLINE
ACE_URL_Locator_Request::~ACE_URL_Locator_Request (void)
{
  delete this->seq1_;
  delete this->seq2_;
  delete this->offer_;
  delete [] this->buffer_;
}

ACE_INLINE const  int
ACE_URL_Locator_Request::how (void) const
{
  return this-> how_;
}

ACE_INLINE const  int
ACE_URL_Locator_Request::how_many (void) const
{
  return this->how_many_;
}

ACE_INLINE const  ACE_URL_Property_Seq *
ACE_URL_Locator_Request::seq (void) const
{
  return this->seq1_;
}

ACE_INLINE const  ACE_URL_Property_Seq *
ACE_URL_Locator_Request::del (void) const
{
  return this->seq1_;
}

ACE_INLINE const  ACE_URL_Property_Seq *
ACE_URL_Locator_Request::modify (void) const
{
  return this->seq2_;
}

ACE_INLINE const ACE_URL_Offer *
ACE_URL_Locator_Request::offer (void) const
{
  return this->offer_;
}

ACE_INLINE const  ACE_WString &
ACE_URL_Locator_Request::id (void) const
{
  return this->id_;
}

ACE_INLINE const  ACE_WString &
ACE_URL_Locator_Request::url (void) const
{
  return this->url_;
}

ACE_INLINE const  char *
ACE_URL_Locator_Request::buffer (void) const
{
  return this->buffer_;
}
