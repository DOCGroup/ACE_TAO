// -*- C++ -*-
// $Id$

#include "test_i.h"
#include "ace/ACE.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

Database_i::~Database_i (void)
{
  delete[] this->elements_;
}

void
Database_i::set (Test::Index i,
                 const Test::OctetSeq& seq,
                 CORBA::Double verification_token,
                 CORBA::Double_out returned_token)
{
  if (i >= this->max_range_)
    throw Test::OutOfRange (0, this->max_range_);

  returned_token = verification_token;

#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  CORBA::ULong len = seq.length ();
  // CORBA::ULong max = seq.maximum ();
  ACE_Message_Block *mb = seq.mb ();
  if (mb == 0)
    {
      this->elements_[i] = seq;
    }
  else
    {
      this->elements_[i].replace (len, seq.mb ());
    }
#else
  this->elements_[i] = seq;
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
}

Test::OctetSeq*
Database_i::get (Test::Index i)
{
  if (i >= this->max_range_)
    throw Test::OutOfRange (0, this->max_range_);

  Test::OctetSeq* copy = 0;
  ACE_NEW_THROW_EX (copy,
                    Test::OctetSeq (this->elements_[i]),
                    CORBA::NO_MEMORY ());
  return copy;
}

CORBA::ULong
Database_i::get_crc (Test::Index i)
{
  if (i >= this->max_range_)
    throw Test::OutOfRange (0, this->max_range_);

  return ACE::crc32 (this->elements_[i].get_buffer (),
                     this->elements_[i].length ());
}

void
Database_i::shutdown (void)
{
  this->orb_->shutdown ();
}
