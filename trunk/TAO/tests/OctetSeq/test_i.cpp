// $Id$

#include "test_i.h"
#include "ace/ACE.h"

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(OctetSeq, test_i, "$Id$")

Database_i::~Database_i (void)
{
  delete[] this->elements_;
}

void
Database_i::set (Test::Index i,
                 const Test::OctetSeq& seq,
                 CORBA::Double verification_token,
                 CORBA::Double_out returned_token
                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((Test::OutOfRange))
{
  if (i >= this->max_range_)
    ACE_THROW (Test::OutOfRange (0, this->max_range_));

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
Database_i::get (Test::Index i
                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,Test::OutOfRange))
{
  if (i >= this->max_range_)
    ACE_THROW_RETURN (Test::OutOfRange (0, this->max_range_), 0);

  Test::OctetSeq* copy;
  ACE_NEW_THROW_EX (copy,
                    Test::OctetSeq (this->elements_[i]),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);
  return copy;
}

CORBA::ULong
Database_i::get_crc (Test::Index i
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,Test::OutOfRange))
{
  if (i >= this->max_range_)
    ACE_THROW_RETURN (Test::OutOfRange (0, this->max_range_), 0);

  return ACE::crc32 (this->elements_[i].get_buffer (),
                     this->elements_[i].length ());
}

void
Database_i::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown ();
}
