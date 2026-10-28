// -*- C++ -*-
#include "tao/orbconf.h"        /* For POA_NO_TIMESTAMP definition. */
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ACE_INLINE CORBA::ULong
    Creation_Time::creation_time_length ()
    {
      return 2 * sizeof (CORBA::ULong);
    }

    ACE_INLINE
    Creation_Time::Creation_Time (ACE_Time_Value const &creation_time)
    {
      this->time_stamp_[Creation_Time::SEC_FIELD] = static_cast<CORBA::ULong> (creation_time.sec ());
      this->time_stamp_[Creation_Time::USEC_FIELD] = static_cast<CORBA::ULong> (creation_time.usec ());
    }

    ACE_INLINE
    Creation_Time::Creation_Time () = default;

    ACE_INLINE void
    Creation_Time::creation_time (void const *creation_time)
    {
      ACE_OS::memcpy (&this->time_stamp_,
                      creation_time,
                      Creation_Time::creation_time_length ());
    }

    ACE_INLINE void const *
    Creation_Time::creation_time () const
    {
      return &this->time_stamp_;
    }

    ACE_INLINE bool
    Creation_Time::operator== (Creation_Time const &rhs) const
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
    Creation_Time::operator!= (Creation_Time const &rhs) const
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
    Temporary_Creation_Time::operator== (Creation_Time const &rhs) const
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
    Temporary_Creation_Time::operator!= (Creation_Time const &rhs) const
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
    Creation_Time::operator== (Temporary_Creation_Time const &rhs) const
    {
      return rhs == *this;
    }

    ACE_INLINE bool
    Creation_Time::operator!= (Temporary_Creation_Time const &rhs) const
    {
      return rhs != *this;
    }

    ACE_INLINE
    Temporary_Creation_Time::Temporary_Creation_Time () = default;

    ACE_INLINE void
    Temporary_Creation_Time::creation_time (void const *creation_time)
    {
      this->time_stamp_ = creation_time;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
