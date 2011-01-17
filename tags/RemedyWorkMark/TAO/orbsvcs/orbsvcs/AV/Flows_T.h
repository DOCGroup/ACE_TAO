// -*- C++ -*-

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
#include /**/ "ace/pre.h"

#include "orbsvcs/AV/AVStreams_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_FDev
 * @brief Implementation of the AV/Streams Flow Device.
 *        A FlowConnection is used to bind FDevs for flows,
 *        much like how a StreamCtrl is used to bind MMDevices for streams.
 */
template <class T_Producer, class T_Consumer>
class TAO_FDev :
  public virtual POA_AVStreams::FDev,
  public virtual TAO_PropertySet
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
                                                       char *& named_fdev);

  /// bridge method for the application to override the producer object
  /// creation. Default implementation creates a TAO_FlowProducer.
  virtual AVStreams::FlowProducer_ptr make_producer (AVStreams::FlowConnection_ptr the_requester,
                                                       AVStreams::QoS & the_qos,
                                                       CORBA::Boolean_out met_qos,
                                                       char *& named_fdev);

  /// create a flow consumer object.
  virtual AVStreams::FlowConsumer_ptr create_consumer (AVStreams::FlowConnection_ptr the_requester,
                                                       AVStreams::QoS & the_qos,
                                                       CORBA::Boolean_out met_qos,
                                                       char *& named_fdev);

  /// bridge method for the application to override the consumer object
  /// creation. Default implementation creates a TAO_FlowConsumer.
  virtual AVStreams::FlowConsumer_ptr make_consumer (AVStreams::FlowConnection_ptr the_requester,
                                                       AVStreams::QoS & the_qos,
                                                       CORBA::Boolean_out met_qos,
                                                       char *& named_fdev);

  /// bind this FDev with another FDev.
  virtual AVStreams::FlowConnection_ptr bind (AVStreams::FDev_ptr peer_device,
                                              AVStreams::QoS & the_qos,
                                              CORBA::Boolean_out is_met);

  /// multicast bind is not implemented yet.
  virtual AVStreams::FlowConnection_ptr bind_mcast (AVStreams::FDev_ptr first_peer,
                                                    AVStreams::QoS & the_qos,
                                                    CORBA::Boolean_out is_met);

  /// destroys this FDev.
  virtual void destroy (AVStreams::FlowEndPoint_ptr the_ep,
                        const char * fdev_name);

protected:
  ACE_DLList <TAO_FlowProducer> producer_list_;
  typedef ACE_DLList_Iterator <TAO_FlowProducer> PRODUCER_LIST_ITERATOR;
  ACE_DLList <TAO_FlowConsumer> consumer_list_;
  typedef ACE_DLList_Iterator <TAO_FlowConsumer> CONSUMER_LIST_ITERATOR;
  CORBA::String_var flowname_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/AV/Flows_T.cpp"
#endif /*ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Flows_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_AV_FLOWS_T_H */
