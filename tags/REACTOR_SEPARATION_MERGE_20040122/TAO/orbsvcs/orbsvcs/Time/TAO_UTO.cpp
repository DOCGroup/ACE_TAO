// -*- C++ -*-
// $Id$

#include "TAO_UTO.h"
#include "TAO_TIO.h"
#include "ace/OS.h"


ACE_RCSID (Time,
           TAO_UTO,
           "$Id$")


TAO_UTO::TAO_UTO (TimeBase::TimeT time,
                  TimeBase::InaccuracyT inaccuracy,
                  TimeBase::TdfT tdf)
{

  this->attr_utc_time_.time = time;

  // Extract the lower 32 bits in the inacclo.
  this->attr_utc_time_.inacclo = (CORBA::ULong) ACE_U64_TO_U32 (inaccuracy);

  // Extract the lower 16 bits of the remaining bits. 'And'ing with 0xFFFF
  // is only a sanity check.

#if defined (ACE_LACKS_U_LONGLONG_T)
  this->attr_utc_time_.inacchi = 0;
#else
  this->attr_utc_time_.inacchi =
    ACE_static_cast (CORBA::UShort,
                     (inaccuracy >> 32U) & 0xFFFF);

#endif /* ACE_LACKS_U_LONGLONG_T */

  this->attr_utc_time_.tdf = tdf;

}

// Destructor.

TAO_UTO::~TAO_UTO (void)
{
}

// Get Method for the readonly attribute time.

TimeBase::TimeT
TAO_UTO::time (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return attr_utc_time_.time;
}

// Get method for the readonly attribute inaccuracy.

TimeBase::InaccuracyT
TAO_UTO::inaccuracy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Construct the Inaccuracy from the
  // inacchi and inacclo.

  TimeBase::InaccuracyT inaccuracy = attr_utc_time_.inacchi;
  inaccuracy <<= 32;
  inaccuracy |= attr_utc_time_.inacclo;
  return inaccuracy;
}

// Get method for the readonly attribute tdf.

TimeBase::TdfT
TAO_UTO::tdf (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return attr_utc_time_.tdf;
}

// Get method for the readonly attribute utc_time.

TimeBase::UtcT
TAO_UTO::utc_time (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return attr_utc_time_;
}

// Absolute time = Relative time + Base time.  ?? Find out more about
// the Base Time, UTC and Distributed Time Sync. Algos. [3].

CosTime::UTO_ptr
TAO_UTO::absolute_time (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CosTime::UTO::_nil ();
}

// Compares the time contained in the object with the time in the
// supplied uto according to the supplied comparison type.

CosTime::TimeComparison
TAO_UTO::compare_time (CosTime::ComparisonType comparison_type,
                       CosTime::UTO_ptr uto
                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TimeBase::TimeT uto_time = uto->time (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosTime::TCIndeterminate);

  TimeBase::InaccuracyT this_inaccuracy =
    this->inaccuracy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosTime::TCIndeterminate);

  TimeBase::InaccuracyT uto_inaccuracy =
    uto->inaccuracy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosTime::TCIndeterminate);

  if (comparison_type == CosTime::MidC)
    {
      if (this->time () == uto_time)
        {
          return CosTime::TCEqualTo;
        }
      else if (this->time () > uto_time)
        {
          return CosTime::TCGreaterThan;
        }
      else
        return CosTime::TCLessThan;
    }
  else if (this->time () == uto_time)
    {
      if (this_inaccuracy == 0U
          && uto_inaccuracy  == 0U)
        {
          return CosTime::TCEqualTo;
        }
    }
  else
    {
      if (this->time () > uto_time)
        {
          if (this->time () - this_inaccuracy
              > uto_time  - uto_inaccuracy)
            {
              return CosTime::TCGreaterThan;
            }
        }
      else if (this->time () + this_inaccuracy
               < uto_time - uto_inaccuracy)
        {
          return CosTime::TCLessThan;
        }
    }

  return CosTime::TCIndeterminate;
}

// Returns a TIO representing the time interval between the time in
// the object and the time in the UTO passed as a parameter. The
// interval returned is the interval between the mid-points of the two
// UTOs. Inaccuracies are ignored.  Note the result of this operation
// is meaningless if the base times of UTOs are different.

CosTime::TIO_ptr
TAO_UTO::time_to_interval (CosTime::UTO_ptr uto
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TIO *tio = 0;

  ACE_TRY
    {
      TimeBase::TimeT uto_time = uto->time (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (this->time () > uto_time)
        {
          ACE_NEW_THROW_EX (tio,
                            TAO_TIO (uto_time,
                                     this->time ()),
                            CORBA::NO_MEMORY ());

          ACE_TRY_CHECK;
        }
      else
        {
          ACE_NEW_THROW_EX (tio,
                            TAO_TIO (this->time (),
                                     uto_time),
                            CORBA::NO_MEMORY ());

          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception:");
      return CosTime::TIO::_nil ();
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CosTime::TIO::_nil ());

  return tio->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// Returns a TIO object representing the error interval around the
// time value in the UTO.

CosTime::TIO_ptr
TAO_UTO::interval (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TIO *tio = 0;

  ACE_TRY
    {
      TimeBase::TimeT this_inaccuracy =
          this->inaccuracy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TimeBase::TimeT lower =
        this->time () - this_inaccuracy;

      TimeBase::TimeT upper =
        this->time () + this_inaccuracy;

      ACE_NEW_THROW_EX (tio,
                        TAO_TIO (lower,
                                 upper),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception:");
      return CosTime::TIO::_nil ();
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CosTime::TIO::_nil ());

  return tio->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}
