// -*- C++ -*-
// $Id$

#include "TAO_UTO.h"
#include "TAO_TIO.h"
#include "ace/OS.h"

// Constructor.

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
  this->attr_utc_time_.inacchi = ACE_static_cast (CORBA::UShort, (inaccuracy >> 32) & 0xFFFF);

#endif /* ACE_LACKS_U_LONGLONG_T */

  this->attr_utc_time_.tdf = tdf;

}

// Destructor.

TAO_UTO::~TAO_UTO (void)
{
}

// Get Method for the readonly attribute time.

TimeBase::TimeT
TAO_UTO::time (CORBA::Environment &)
{
  return attr_utc_time_.time;
}

// Get method for the readonly attribute inaccuracy.

TimeBase::InaccuracyT
TAO_UTO::inaccuracy (CORBA::Environment &)
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
TAO_UTO::tdf (CORBA::Environment &)
{
  return attr_utc_time_.tdf;
}

// Get method for the readonly attribute utc_time.

TimeBase::UtcT
TAO_UTO::utc_time (CORBA::Environment &)
{
  return attr_utc_time_;
}

// Absolute time = Relative time + Base time.  ?? Find out more about
// the Base Time, UTC and Distributed Time Sync. Algos. [3].

CosTime::UTO_ptr
TAO_UTO::absolute_time (CORBA::Environment &)
{
  return 0;
}

// Compares the time contained in the object with the time in the
// supplied uto according to the supplied comparison type.

CosTime::TimeComparison
TAO_UTO::compare_time (CosTime::ComparisonType comparison_type,
                       CosTime::UTO_ptr uto,
                       CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      if (comparison_type == CosTime::MidC)
        {
          if (this->time (ACE_TRY_ENV) == uto->time (ACE_TRY_ENV))
            {
              ACE_TRY_CHECK;
              return CosTime::TCEqualTo;
            }
          else if (this->time (ACE_TRY_ENV) > uto->time (ACE_TRY_ENV))
            {
              ACE_TRY_CHECK;
              return CosTime::TCGreaterThan;
            }
          else
            return CosTime::TCLessThan;
        }
      else if (this->time (ACE_TRY_ENV) == uto->time (ACE_TRY_ENV))
        {
          ACE_TRY_CHECK;
          if (this->inaccuracy (ACE_TRY_ENV) == 0 && uto->inaccuracy (ACE_TRY_ENV) == 0)
            {
              ACE_TRY_CHECK;
              return CosTime::TCEqualTo;
            }
        }
      else
        {
          if (this->time (ACE_TRY_ENV) > uto->time (ACE_TRY_ENV))
            {
              ACE_TRY_CHECK;
              if (this->time (ACE_TRY_ENV) - this->inaccuracy (ACE_TRY_ENV)
                  > uto->time (ACE_TRY_ENV) - uto->inaccuracy (ACE_TRY_ENV))
                {
                  ACE_TRY_CHECK;
                  return CosTime::TCGreaterThan;
                }
            }
          else if (this->time (ACE_TRY_ENV) + this->inaccuracy (ACE_TRY_ENV)
                   < uto->time (ACE_TRY_ENV) - uto->inaccuracy (ACE_TRY_ENV))

            {
              ACE_TRY_CHECK;
              return CosTime::TCLessThan;
            }
        }

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception:");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CosTime::TCIndeterminate);
  
  return CosTime::TCIndeterminate;
}

// Returns a TIO representing the time interval between the time in
// the object and the time in the UTO passed as a parameter. The
// interval returned is the interval between the mid-points of the two
// UTOs. Inaccuracies are ignored.  Note the result of this operation
// is meaningless if the base times of UTOs are different.

CosTime::TIO_ptr
TAO_UTO::time_to_interval (CosTime::UTO_ptr uto,
                           CORBA::Environment &ACE_TRY_ENV)
{
  TAO_TIO *tio = 0;

  ACE_TRY
    {
      if (this->time (ACE_TRY_ENV) > uto->time (ACE_TRY_ENV))
        {
	  ACE_NEW_THROW_EX (tio,
			    TAO_TIO (uto->time (ACE_TRY_ENV),
				     this->time (ACE_TRY_ENV)),
			    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));
	  
	  ACE_TRY_CHECK;
	}
      else
        {
	  ACE_NEW_THROW_EX (tio,
			    TAO_TIO (this->time (ACE_TRY_ENV),
				     uto->time (ACE_TRY_ENV)),
			    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));
	  
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
  
  return tio->_this ();
}

// Returns a TIO object representing the error interval around the
// time value in the UTO.

CosTime::TIO_ptr
TAO_UTO::interval (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_TIO *tio = 0;
  
  ACE_TRY
    {
      TimeBase::TimeT lower =
        this->time (ACE_TRY_ENV) - this->inaccuracy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      TimeBase::TimeT upper =
        this->time (ACE_TRY_ENV) + this->inaccuracy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_NEW_THROW_EX (tio,
			TAO_TIO (lower,
				 upper),
			CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));
      ACE_TRY_CHECK;      
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception:");
      return CosTime::TIO::_nil ();
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CosTime::TIO::_nil ());
  
  return tio->_this ();
}

