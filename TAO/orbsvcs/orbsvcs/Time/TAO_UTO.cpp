// -*- C++ -*-
#include "orbsvcs/Time/TAO_UTO.h"
#include "orbsvcs/Time/TAO_TIO.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Split inaccuracy into the lower 32 bits and the upper 16 bits stored by UtcT.
TAO_UTO::TAO_UTO (TimeBase::TimeT time,
                  TimeBase::InaccuracyT inaccuracy,
                  TimeBase::TdfT tdf)
  : attr_utc_time_ {time,
                    static_cast<CORBA::ULong> (ACE_U64_TO_U32 (inaccuracy)),
                    static_cast<CORBA::UShort> ((inaccuracy >> 32U) & 0xFFFF),
                    tdf}
{
}

/// Returns the readonly time attribute.
TimeBase::TimeT
TAO_UTO::time ()
{
  return attr_utc_time_.time;
}

/// Returns the readonly inaccuracy attribute.
TimeBase::InaccuracyT
TAO_UTO::inaccuracy ()
{
  // Construct the Inaccuracy from the
  // inacchi and inacclo.

  TimeBase::InaccuracyT inaccuracy = attr_utc_time_.inacchi;
  inaccuracy <<= 32;
  inaccuracy |= attr_utc_time_.inacclo;
  return inaccuracy;
}

/// Returns the readonly time displacement factor attribute.
TimeBase::TdfT
TAO_UTO::tdf ()
{
  return attr_utc_time_.tdf;
}

/// Returns the readonly UTC time attribute.
TimeBase::UtcT
TAO_UTO::utc_time ()
{
  return attr_utc_time_;
}

/// Returns the absolute time. The current implementation returns nil.
CosTime::UTO_ptr
TAO_UTO::absolute_time ()
{
  return CosTime::UTO::_nil ();
}

/// Compares this object's time with `uto` according to `comparison_type`.
CosTime::TimeComparison
TAO_UTO::compare_time (CosTime::ComparisonType comparison_type,
                       CosTime::UTO_ptr uto)
{
  TimeBase::TimeT const uto_time = uto->time ();

  TimeBase::InaccuracyT const this_inaccuracy =
    this->inaccuracy ();

  TimeBase::InaccuracyT const uto_inaccuracy =
    uto->inaccuracy ();

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

/// Returns a TIO for the interval between this time and `uto`'s time.
/// The interval spans the midpoints of the two UTOs and ignores their
/// inaccuracies. The result is meaningful only when both UTOs use the
/// same base time.
CosTime::TIO_ptr
TAO_UTO::time_to_interval (CosTime::UTO_ptr uto)
{
  TAO_TIO * tio {};
  PortableServer::ServantBase_var xfer;

  try
    {
      TimeBase::TimeT const uto_time = uto->time ();

      if (this->time () > uto_time)
        {
          ACE_NEW_THROW_EX (tio,
                            TAO_TIO (uto_time,
                                     this->time ()),
                            CORBA::NO_MEMORY ());
        }
      else
        {
          ACE_NEW_THROW_EX (tio,
                            TAO_TIO (this->time (),
                                     uto_time),
                            CORBA::NO_MEMORY ());
        }
      xfer = tio;
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Exception:");
      return CosTime::TIO::_nil ();
    }

  return tio->_this ();
}

/// Returns a TIO representing the error interval around this UTO's time.
CosTime::TIO_ptr
TAO_UTO::interval ()
{
  TAO_TIO * tio {};
  PortableServer::ServantBase_var xfer;

  try
    {
      TimeBase::TimeT const this_inaccuracy =
          this->inaccuracy ();

      TimeBase::TimeT const lower =
        this->time () - this_inaccuracy;

      TimeBase::TimeT const upper =
        this->time () + this_inaccuracy;

      ACE_NEW_THROW_EX (tio,
                        TAO_TIO (lower,
                                 upper),
                        CORBA::NO_MEMORY ());
      xfer = tio;
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Exception:");
      return CosTime::TIO::_nil ();
    }

  return tio->_this ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
