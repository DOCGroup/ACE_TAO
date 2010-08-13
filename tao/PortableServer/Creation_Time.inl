// -*- C++ -*-
//
// $Id$

#include "tao/orbconf.h"        /* For POA_NO_TIMESTAMP definition. */
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ACE_INLINE CORBA::ULong
    Creation_Time::creation_time_length (void)
    {
      return 2 * sizeof (CORBA::ULong);
    }

    ACE_INLINE
    Creation_Time::Creation_Time (const ACE_Time_Value &creation_time)
    {
      this->time_stamp_[Creation_Time::SEC_FIELD]  = (CORBA::ULong) creation_time.sec ();
      this->time_stamp_[Creation_Time::USEC_FIELD] = (CORBA::ULong) creation_time.usec ();
    }

    ACE_INLINE
    Creation_Time::Creation_Time (void)
    {
      this->time_stamp_[Creation_Time::SEC_FIELD]  = 0;
      this->time_stamp_[Creation_Time::USEC_FIELD] = 0;
    }

    ACE_INLINE void
    Creation_Time::creation_time (const void *creation_time)
    {
      ACE_OS::memcpy (&this->time_stamp_,
                      creation_time,
                      Creation_Time::creation_time_length ());
    }

    ACE_INLINE const void *
    Creation_Time::creation_time (void) const
    {
      return &this->time_stamp_;
    }

    ACE_INLINE bool
    Creation_Time::operator== (const Creation_Time &rhs) const
    {
    #if (POA_NO_TIMESTAMP == 1)
      ACE_UNUSED_ARG (rhs);
      return true;
    #else
      return ACE_OS::memcmp (&this->time_stamp_,
                             &rhs.time_stamp_,
                             Creation_Time::creation_time_length ()) == 0;
    #endif /* POA_NO_TIMESTAMP */
    }

    ACE_INLINE bool
    Creation_Time::operator!= (const Creation_Time &rhs) const
    {
    #if (POA_NO_TIMESTAMP == 1)
      ACE_UNUSED_ARG (rhs);
      return false;
    #else
      return ACE_OS::memcmp (&this->time_stamp_,
                             &rhs.time_stamp_,
                             Creation_Time::creation_time_length ()) != 0;
    #endif /* POA_NO_TIMESTAMP */
    }

    ACE_INLINE bool
    Temporary_Creation_Time::operator== (const Creation_Time &rhs) const
    {
    #if (POA_NO_TIMESTAMP == 1)
      ACE_UNUSED_ARG (rhs);
      return true;
    #else
      return ACE_OS::memcmp (this->time_stamp_,
                             rhs.creation_time (),
                             Creation_Time::creation_time_length ()) == 0;
    #endif /* POA_NO_TIMESTAMP */
    }

    ACE_INLINE bool
    Temporary_Creation_Time::operator!= (const Creation_Time &rhs) const
    {
    #if (POA_NO_TIMESTAMP == 1)
      ACE_UNUSED_ARG (rhs);
      return false;
    #else
      return ACE_OS::memcmp (this->time_stamp_,
                             rhs.creation_time (),
                             Creation_Time::creation_time_length ()) != 0;
    #endif /* POA_NO_TIMESTAMP */
    }

    ACE_INLINE bool
    Creation_Time::operator== (const Temporary_Creation_Time &rhs) const
    {
      return rhs == *this;
    }

    ACE_INLINE bool
    Creation_Time::operator!= (const Temporary_Creation_Time &rhs) const
    {
      return rhs != *this;
    }

    ACE_INLINE
    Temporary_Creation_Time::Temporary_Creation_Time (void)
      : time_stamp_ (0)
    {
    }

    ACE_INLINE void
    Temporary_Creation_Time::creation_time (const void *creation_time)
    {
      this->time_stamp_ = (void *) creation_time;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
