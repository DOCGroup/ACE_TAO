// $Id$

#include "test_i.h"
#include "tao/debug.h"
#include "ace/ACE.h"

ACE_RCSID(Reliable, test_i, "$Id$")

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

void
Test_i::oneway_op (CORBA::ULong work,
                   CORBA::Environment &) 
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (CORBA::ULong i = 0; i < work; i++)
    {
      for (int j = 0; j < 1000; j++);
    }
}

void
Test_i::twoway_op (CORBA::ULong work,
                   CORBA::Environment &) 
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (CORBA::ULong i = 0; i < work; i++)
    {
      for (int j = 0; j < 1000; j++);
    }
}

void
Test_i::shutdown (CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0, 
                        ACE_TRY_ENV);
}
