// -*- C++ -*-
// $Id$

#include "TAO_TIO.h"

// Constructor.
TAO_TIO::TAO_TIO (TimeBase::TimeT lower,
                  TimeBase::TimeT upper)
{
  this->attr_time_interval.lower_bound = lower;
  this->attr_time_interval.upper_bound = upper;
}

// Destructor.
TAO_TIO::~TAO_TIO (void)
{
}

// This is the get method for the attribute time interval.

TimeBase::IntervalT
TAO_TIO::time_interval (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return attr_time_interval;
}

// This operation returns a value of type OverlapType depending on how
// the interval in the object and the time range represented by the
// parameter UTO overlap.  If OverlapType is not OTNoOverlap, then the
// out parameter overlap contains the overlap interval, otherwise the
// out parameter contains the gap between the two intervals.

CosTime::OverlapType
TAO_TIO::spans (CosTime::UTO_ptr uto,
                CosTime::TIO_out overlap
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TIO *tio = 0;

  ACE_TRY
    {
      TimeBase::TimeT lb1 =
        this->time_interval ().lower_bound;

      TimeBase::TimeT up1 =
        this->time_interval ().upper_bound;

      TimeBase::TimeT tmp1 = uto->time (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TimeBase::TimeT tmp2 = uto->inaccuracy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TimeBase::TimeT lb2 = tmp1 - tmp2;


      tmp1 = uto->time (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      tmp2 = uto->inaccuracy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TimeBase::TimeT up2 = tmp1 + tmp2;

      if (lb1 == lb2 && up1 == up2)
        {
          ACE_NEW_RETURN (tio,
                          TAO_TIO (lb1, up1),
                          CosTime::OTNoOverlap);
          overlap = tio->_this ();

          return CosTime::OTOverlap;
        }
      else if (lb1 > lb2 && up1 < up2)
        {
          ACE_NEW_RETURN (tio,
                          TAO_TIO (lb1, up1),
                          CosTime::OTNoOverlap);

          overlap = tio->_this ();

          return CosTime::OTContained;
        }
      else if (lb1 < lb2 && up1 > up2)
        {
          ACE_NEW_RETURN (tio,
                          TAO_TIO (lb2, up2),
                          CosTime::OTNoOverlap);

          overlap = tio->_this ();

          return CosTime::OTContained;
        }
      else if (lb1 < lb2)
        {
          if (up1 < lb2)
            {
              ACE_NEW_RETURN (tio,
                              TAO_TIO (0, 0),
                              CosTime::OTNoOverlap);

              overlap = tio->_this ();

              return CosTime::OTNoOverlap;
            }
          else
            {
              ACE_NEW_RETURN (tio,
                              TAO_TIO (lb2, up1),
                              CosTime::OTNoOverlap);
              overlap = tio->_this ();

              return CosTime::OTOverlap;
            }
        }
      else if (up2 < lb1)
        {

          ACE_NEW_RETURN (tio,
                          TAO_TIO (0, 0),
                          CosTime::OTNoOverlap);

          overlap = tio->_this ();

          return CosTime::OTNoOverlap;
        }
      else
        {
          ACE_NEW_RETURN (tio,
                          TAO_TIO (lb1, up2),
                          CosTime::OTNoOverlap);

          overlap = tio->_this ();

        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception:");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CosTime::OTNoOverlap);

  return CosTime::OTNoOverlap;
}

// This operation returns a value of type OverlapType depending on how
// the interval in the object and interval in the parameter TIO
// overlap. If OverlapType is not OTNoOverlap, then the out parameter
// overlap contains the overlap interval, otherwise the out parameter
// contains the gap between the two intervals.

CosTime::OverlapType
TAO_TIO::overlaps (CosTime::TIO_ptr tio,
                   CosTime::TIO_out overlap
                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TIO *tio_i = 0;

  TimeBase::TimeT lb1 =
    this->time_interval ().lower_bound;

  TimeBase::TimeT up1 =
    this->time_interval ().upper_bound;

  TimeBase::TimeT lb2 =
    tio->time_interval ().lower_bound;

  TimeBase::TimeT up2 =
    tio->time_interval ().upper_bound;

  if (lb1 == lb2 && up1 == up2)
    {
      ACE_NEW_THROW_EX (tio_i,
                        TAO_TIO (lb1, up1),
                        CORBA::NO_MEMORY ());

      ACE_CHECK_RETURN (CosTime::OTNoOverlap);

      overlap = tio_i->_this ();

      return CosTime::OTOverlap;
    }
  else if (lb1 > lb2 && up1 < up2)
    {
      ACE_NEW_THROW_EX (tio_i,
                        TAO_TIO (lb1, up1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CosTime::OTNoOverlap);

      overlap = tio_i->_this ();

      return CosTime::OTContained;
    }
  else if (lb1 < lb2 && up1 > up2)
    {
      ACE_NEW_THROW_EX (tio_i,
                        TAO_TIO (lb2, up2),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CosTime::OTNoOverlap);

      overlap = tio_i->_this ();

      return CosTime::OTContained;
    }
  else if (lb1 < lb2)
    {
      if (up1 < lb2)
        {
          ACE_NEW_THROW_EX (tio_i,
                            TAO_TIO (0, 0),
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (CosTime::OTNoOverlap);
          overlap = tio_i->_this ();

          return CosTime::OTNoOverlap;
        }
      else
        {
          ACE_NEW_THROW_EX (tio_i,
                            TAO_TIO (lb2, up1),
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (CosTime::OTNoOverlap);

          overlap = tio_i->_this ();

          return CosTime::OTOverlap;
        }
    }
  else if (up2 < lb1)
    {
      ACE_NEW_THROW_EX (tio_i,
                        TAO_TIO (0, 0),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CosTime::OTNoOverlap);

      overlap = tio_i->_this ();

      return CosTime::OTNoOverlap;
    }
  else
    {
      ACE_NEW_THROW_EX (tio_i,
                        TAO_TIO (lb1, up2),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CosTime::OTNoOverlap);
      overlap = tio_i->_this ();
    }

  return CosTime::OTNoOverlap;
}

CosTime::UTO_ptr
TAO_TIO::time (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_UTO *uto = 0;

  ACE_NEW_THROW_EX (uto,
                    TAO_UTO ((this->time_interval ().upper_bound -
                              this->time_interval ().lower_bound) / 2,
                             this->time_interval ().upper_bound -
                             this->time_interval ().lower_bound,
                             0),
                    CORBA::NO_MEMORY ());

  ACE_CHECK_RETURN (CosTime::UTO::_nil ());

  return uto->_this ();
}
