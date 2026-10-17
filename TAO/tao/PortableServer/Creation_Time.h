// -*- C++ -*-

//=============================================================================
/**
 *  @file    Creation_Time.h
 *
 *  Header file for Creation_Time and Temporary_Creation_Time
 *
 *  @author  Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_CREATION_TIME_H
#define TAO_CREATION_TIME_H

#include /**/ "ace/pre.h"

#include "ace/Time_Value.h"
#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class Temporary_Creation_Time;

    class Creation_Time
    {
    public:
      explicit Creation_Time (ACE_Time_Value const &creation_time);

      Creation_Time ();

      void creation_time (void const *creation_time);

      void const *creation_time () const;

      static CORBA::ULong creation_time_length ();

      bool operator== (Creation_Time const &rhs) const;

      bool operator!= (Creation_Time const &rhs) const;

      bool operator== (Temporary_Creation_Time const &rhs) const;

      bool operator!= (Temporary_Creation_Time const &rhs) const;

    protected:
      enum
      {
        SEC_FIELD = 0,
        USEC_FIELD = 1
      };

      /// Timestamp
      CORBA::ULong time_stamp_[2] {};
    };

    /**
     * @class Temporary_Creation_Time
     *
     * @brief Special creation time only useful in the lifetime of the
     *        upcall.
     *
     * Special creation time only useful in the lifetime of the upcall.
     */
    class Temporary_Creation_Time
    {
    public:
      Temporary_Creation_Time ();

      void creation_time (void const *creation_time);

      bool operator== (Creation_Time const &rhs) const;

      bool operator!= (Creation_Time const &rhs) const;

    protected:
      void const *time_stamp_ {};
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/PortableServer/Creation_Time.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CREATION_TIME_H */
