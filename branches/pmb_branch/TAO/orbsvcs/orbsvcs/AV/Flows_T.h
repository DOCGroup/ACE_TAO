/* -*- C++ -*- */


//=============================================================================
/**
 *  @file   Flows_T.h
 *
 *  $Id$
 *
 *  @author Nagarajan Surendran <naga@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_AV_FLOWS_T_H
#define TAO_AV_FLOWS_T_H
#include "ace/pre.h"

#include "AVStreams_i.h"

/**
 * @class TAO_FDev
 * @brief Implementation of the AV/Streams Flow Device.
 *        A FlowConnection is used to bind FDevs for flows,
 *        much like how a StreamCtrl is used to bind MMDevices for streams.
 */
template <class T_Producer, class T_Consumer>
class TAO_AV_Export TAO_FDev :
  public virtual POA_AVStreams::FDev,
  public virtual TAO_PropertySet,
  public virtual PortableServer::RefCountServantBase
{
public:
  /// default constructor
  TAO_FDev (void);

  /// constructor taking a flowname.
  TAO_FDev (const char *flowname);

  /// Destructor..
  ~TAO_FDev (void);

  /// set/get the flowname.
  /// create a flow producer object.
  const char *flowname (void);
  void flowname (const char *flowname);
  AVStreams::FlowProducer_ptr create_producer (AVStreams::FlowConnection_ptr the_requester,
                                                       AVStreams::QoS & the_qos,
                                                       CORBA::Boolean_out met_qos,
                                                       char *& named_fdev
                                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::streamOpFailed,
                     AVStreams::streamOpDenied,
                     AVStreams::notSupported,
                     AVStreams::QoSRequestFailed));

  /// bridge method for the application to override the producer object
  /// creation. Default implementation creates a TAO_FlowProducer.
  virtual AVStreams::FlowProducer_ptr make_producer (AVStreams::FlowConnection_ptr the_requester,
                                                       AVStreams::QoS & the_qos,
                                                       CORBA::Boolean_out met_qos,
                                                       char *& named_fdev
                                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// create a flow consumer object.
  virtual AVStreams::FlowConsumer_ptr create_consumer (AVStreams::FlowConnection_ptr the_requester,
                                                       AVStreams::QoS & the_qos,
                                                       CORBA::Boolean_out met_qos,
                                                       char *& named_fdev
                                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::streamOpFailed,
                     AVStreams::streamOpDenied,
                     AVStreams::notSupported,
                     AVStreams::QoSRequestFailed));

  /// bridge method for the application to override the consumer object
  /// creation. Default implementation creates a TAO_FlowConsumer.
  virtual AVStreams::FlowConsumer_ptr make_consumer (AVStreams::FlowConnection_ptr the_requester,
                                                       AVStreams::QoS & the_qos,
                                                       CORBA::Boolean_out met_qos,
                                                       char *& named_fdev
                                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// bind this FDev with another FDev.
  virtual AVStreams::FlowConnection_ptr bind (AVStreams::FDev_ptr peer_device,
                                              AVStreams::QoS & the_qos,
                                              CORBA::Boolean_out is_met
                                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::streamOpFailed,
                     AVStreams::QoSRequestFailed));

  /// multicast bind is not implemented yet.
  virtual AVStreams::FlowConnection_ptr bind_mcast (AVStreams::FDev_ptr first_peer,
                                                    AVStreams::QoS & the_qos,
                                                    CORBA::Boolean_out is_met
                                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::streamOpFailed,
                     AVStreams::QoSRequestFailed));

  /// destroys this FDev.
  virtual void destroy (AVStreams::FlowEndPoint_ptr the_ep,
                        const char * fdev_name
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::notSupported));

protected:
  ACE_DLList <TAO_FlowProducer> producer_list_;
  typedef ACE_DLList_Iterator <TAO_FlowProducer> PRODUCER_LIST_ITERATOR;
  ACE_DLList <TAO_FlowConsumer> consumer_list_;
  typedef ACE_DLList_Iterator <TAO_FlowConsumer> CONSUMER_LIST_ITERATOR;
  CORBA::String_var flowname_;
};


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Flows_T.cpp"
#endif /*ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Flows_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_AV_FLOWS_T_H */
