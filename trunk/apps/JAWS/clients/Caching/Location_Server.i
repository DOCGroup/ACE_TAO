/* -*- C++ -*- */

// $Id$

ACE_INLINE ACE_URL_OfferID
ACE_URL_Record::id (void) const
{
  return this->id_;
}

ACE_INLINE void
ACE_URL_Record::id (ACE_URL_OfferID id)
{
  delete[] this->id_;
  if (id)
    {
      ACE_NEW (this->id_, TCHAR[ACE_OS::strlen (id) + 1]);
      ACE_OS::strcpy (this->id_, id);
    }
  else
    this->id_ = 0;
}
