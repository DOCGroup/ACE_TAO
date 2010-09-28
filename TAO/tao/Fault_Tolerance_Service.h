//-*- C++ -*-

//=============================================================================
/**
 *  @file    Fault_Tolerance_Service.h
 *
 *  $Id$
 *
 *  A collection of the ORB and ORB core related properties that are
 *  specific to FT service. The TAO_ORB_Core holds an instance of this
 *  class.
 *
 *
 *  @author Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================
#ifndef TAO_FAULT_TOLERANCE_SERVICE_H
#define TAO_FAULT_TOLERANCE_SERVICE_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Service_Callbacks;
class TAO_ORB_Core;

/**
 * @class TAO_Fault_Tolerance_Service
 *
 * @brief TAO_Fault_Tolerant_Service
 *
 * A class that holds an FT service call back instance.
 */
class TAO_Export TAO_Fault_Tolerance_Service
{

public:
  /// Ctor
  TAO_Fault_Tolerance_Service (void);

  /// Dtor
  ~TAO_Fault_Tolerance_Service (void);

  /// Initialize the internal data structures
  void init (TAO_ORB_Core *orb_core);

  /// Return the underlying callback object
  TAO_Service_Callbacks *service_callback (void);

private:
  void operator= (const TAO_Fault_Tolerance_Service &);
  TAO_Fault_Tolerance_Service (const TAO_Fault_Tolerance_Service &);

private:

  /// hook to callback on to the service
  TAO_Service_Callbacks *ft_service_callback_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Fault_Tolerance_Service.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_FAULT_TOLERANCE_SERVICE_H*/
