// -*- C++ -*-
//=============================================================================
/**
 *  @file  FTEC_Gateway.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef FTEC_GATEWAY_H
#define FTEC_GATEWAY_H

#include "ace/pre.h"

#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventChannelAdminS.h"

namespace TAO_FTRTEC
{
  struct FTEC_Gateway_Impl;
  class TAO_FtRtEvent_Export FTEC_Gateway :
    public POA_RtecEventChannelAdmin::EventChannel
  {
  public:
      FTEC_Gateway(CORBA::ORB_ptr orb,
                   FtRtecEventChannelAdmin::EventChannel_ptr ftec);
      ~FTEC_Gateway();

      RtecEventChannelAdmin::EventChannel_ptr activate
          (PortableServer::POA_ptr poa
            ACE_ENV_ARG_DECL);

      RtecEventChannelAdmin::EventChannel_ptr 
        _this(ACE_ENV_SINGLE_ARG_DECL);

      //= The RtecEventChannelAdmin::EventChannel methods
      virtual RtecEventChannelAdmin::ConsumerAdmin_ptr
        for_consumers (ACE_ENV_SINGLE_ARG_DECL) 
          ACE_THROW_SPEC((CORBA::SystemException));

      virtual RtecEventChannelAdmin::SupplierAdmin_ptr
        for_suppliers (ACE_ENV_SINGLE_ARG_DECL) 
          ACE_THROW_SPEC((CORBA::SystemException));

      virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED) 
         ACE_THROW_SPEC((CORBA::SystemException));

      virtual RtecEventChannelAdmin::Observer_Handle
        append_observer
          (RtecEventChannelAdmin::Observer_ptr observer
           ACE_ENV_ARG_DECL) ACE_THROW_SPEC((
                             CORBA::SystemException,
                             RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
                             RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER));

      virtual void remove_observer
        (RtecEventChannelAdmin::Observer_Handle
         ACE_ENV_ARG_DECL)  ACE_THROW_SPEC((
                            CORBA::SystemException,
                            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
                            RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER));
  private:
      struct FTEC_Gateway_Impl  *impl_;
  };
}

#if defined(__ACE_INLINE__)
#include "FTEC_Gateway.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif
