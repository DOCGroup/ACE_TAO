// -*- C++ -*-
// $Id$

#include "TAO_UTO.h"
#include "TAO_TIO.h"

// Constructor.

TAO_UTO::TAO_UTO (TimeBase::TimeT time,
		  TimeBase::InaccuracyT inaccuracy,
		  TimeBase::TdfT tdf)
  // @@ Vishal, please consider putting these assignments into the
  // constructor of attr_utc_time_.  BTW, I think some of your other
  // implementation classes could benefit from this same technique.

  // ?? Tried to initialise the structure here. Failed.

  // @@ Vishal, you'll need to add a CONSTRUCTOR for the structure and
  // then initialize this.  Please let me know if you have any
  // questions about how to do that.
{
  this->attr_utc_time_.time = time;
  this->attr_utc_time_.inacchi = inaccuracy / 2;
  this->attr_utc_time_.inacclo = inaccuracy - inaccuracy / 2;
  this->attr_utc_time_.tdf = tdf;
}

// Destructor.

TAO_UTO::~TAO_UTO (void)
{
}

// Get Method for the readonly attribute time.

TimeBase::TimeT
TAO_UTO::time (CORBA::Environment &env)
{
  return attr_utc_time_.time;
}

// Get method for the readonly attribute inaccuracy.

TimeBase::InaccuracyT
TAO_UTO::inaccuracy (CORBA::Environment &env)
{
  return attr_utc_time_.inacclo + attr_utc_time_.inacchi;
}

// Get method for the readonly attribute tdf.

TimeBase::TdfT
TAO_UTO::tdf (CORBA::Environment &env)
{
  return attr_utc_time_.tdf;
}

// Get method for the readonly attribute utc_time.

TimeBase::UtcT
TAO_UTO::utc_time (CORBA::Environment &env)
{
  return attr_utc_time_;
}

// Absolute time = Relative time + Base time.  ?? Find out more about
// the Base Time, UTC and Distributed Time Sync. Algos. [3].

CosTime::UTO_ptr
TAO_UTO::absolute_time (CORBA::Environment &env)
{
  return 0;
}

// Compares the time contained in the object with the time in the
// supplied uto according to the supplied comparison type.

CosTime::TimeComparison
TAO_UTO::compare_time (CosTime::ComparisonType comparison_type,
		       CosTime::UTO_ptr uto,
		       CORBA::Environment &env)
{
  if (comparison_type == CosTime::MidC)
    {
      if (this->time (env) == uto->time (env))
	return CosTime::TCEqualTo;
      else if (this->time (env) > uto->time (env))
	return CosTime::TCGreaterThan;
      else
	return CosTime::TCLessThan;
    }
  else if (this->time (env) == uto->time (env))
    {
      if (this->inaccuracy (env) == 0 && uto->inaccuracy (env) == 0)
        return CosTime::TCEqualTo;
    }
  else
    {
      if (this->time (env) > uto->time (env))
        {
          if (this->time (env) - this->inaccuracy (env)
              > uto->time (env) - uto->inaccuracy (env))
            return CosTime::TCGreaterThan;
        }
      else if (this->time (env) + this->inaccuracy (env)
               < uto->time (env) - uto->inaccuracy (env))
        return CosTime::TCLessThan;
    }

  return CosTime::TCIndeterminate;
}

// Returns a TIO representing the time interval between the time in
// the object and the time in the UTO passed as a parameter. The
// interval returned is the interval between the mid-points of the two
// UTOs. Inaccuracies are ignored.  Note the result of this operation
// is meaningless if the base times of UTOs are different.

CosTime::TIO_ptr
TAO_UTO::time_to_interval (CosTime::UTO_ptr uto,
// @@ Vishal, please don't use _env, use TAO_TRY_ENV instead.  Please
// apply this fix to ALL of your code.
			   CORBA::Environment &_env)
{
  TAO_TIO *tio = 0;

  if (this->time (_env) > uto->time (_env))
    ACE_NEW_THROW_RETURN (tio,
			  TAO_TIO (uto->time (_env),
				   this->time (_env)),
			  CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
			  CosTime::TIO::_nil ());
  else
    ACE_NEW_THROW_RETURN (tio,
			  TAO_TIO (this->time (_env),
				   uto->time (_env)),
			  CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
			  CosTime::TIO::_nil ());
  return tio->_this ();
}

// Returns a TIO object representing the error interval around the
// time value in the UTO.

CosTime::TIO_ptr
TAO_UTO::interval (CORBA::Environment &_env)
{
  TAO_TIO *tio = 0;

  TimeBase::TimeT lower =
    this->time (_env) - this->inaccuracy (_env);
  TimeBase::TimeT upper =
    this->time (_env) + this->inaccuracy (_env);

  ACE_NEW_THROW_RETURN (tio,
			TAO_TIO (lower,
				 upper),
			CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
			CosTime::TIO::_nil ());
  return tio->_this ();
};
