//$Id$

#ifndef TAO_AV_FLOWS_T_CPP
#define TAO_AV_FLOWS_T_CPP

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/AV/Flows_T.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// ------------------------------------------------------------
// TAO_FDev
// ------------------------------------------------------------

// default constructor
template <class T_Producer, class T_Consumer>
TAO_FDev<T_Producer, T_Consumer>::TAO_FDev (void)
{
}

template <class T_Producer, class T_Consumer>
TAO_FDev<T_Producer, T_Consumer>::TAO_FDev (const char *flowname)
  :flowname_ (flowname)
{
  try
    {
      CORBA::Any flowname_any;
      flowname_any <<= flowname;
      this->define_property ("Flow",
                             flowname_any);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO_FDev::TAO_FDev");
    }
}

template <class T_Producer, class T_Consumer>
TAO_FDev<T_Producer, T_Consumer>::~TAO_FDev (void)
{
  //no-op
}

template <class T_Producer, class T_Consumer>
const char *
TAO_FDev<T_Producer, T_Consumer>::flowname (void)
{
  return this->flowname_.in ();
}

template <class T_Producer, class T_Consumer>
void
TAO_FDev<T_Producer, T_Consumer>::flowname (const char *flow_name)
{
  try
    {
      CORBA::Any flowname_any;
      flowname_any <<= flow_name;
      this->define_property ("Flow",
                             flowname_any);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO_FDev::flowname");
    }
  this->flowname_ = flow_name;
}

template <class T_Producer, class T_Consumer>
AVStreams::FlowProducer_ptr
TAO_FDev<T_Producer, T_Consumer>::create_producer (AVStreams::FlowConnection_ptr the_requester,
                                                   AVStreams::QoS & the_qos,
                                                   CORBA::Boolean_out met_qos,
                                                   char *& named_fdev)
{
  // call the bridge method.
  return this->make_producer (the_requester,
                              the_qos,
                              met_qos,
                              named_fdev);
}

template <class T_Producer, class T_Consumer>
// hook for the applications to override the creation process.
AVStreams::FlowProducer_ptr
TAO_FDev<T_Producer, T_Consumer>::make_producer (AVStreams::FlowConnection_ptr /* the_requester */,
                                                 AVStreams::QoS & /* the_qos */,
                                                 CORBA::Boolean_out /* met_qos */,
                                                 char *& /* named_fdev */)
{
  AVStreams::FlowProducer_ptr producer = AVStreams::FlowProducer::_nil ();
  try
    {
      // Activate the producer implementation under the Root POA.
      T_Producer *producer_i = 0;
      ACE_NEW_RETURN (producer_i, T_Producer, 0);
      this->producer_list_.insert_tail (producer_i);
      producer = producer_i->_this ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO_FDev::make_producer");
      return producer;
    }
  return producer;
}

template <class T_Producer, class T_Consumer>
// hook for the applications to override the consumer creation.
AVStreams::FlowConsumer_ptr
TAO_FDev<T_Producer, T_Consumer>::make_consumer (AVStreams::FlowConnection_ptr /* the_requester */,
                                                 AVStreams::QoS & /* the_qos */,
                                                 CORBA::Boolean_out /* met_qos */,
                                                 char *& /* named_fdev */)
{
  AVStreams::FlowConsumer_ptr consumer = AVStreams::FlowConsumer::_nil ();
  try
    {
      // Activate the consumer implementation under the Root POA.
      T_Consumer *consumer_i;
      ACE_NEW_RETURN (consumer_i, T_Consumer, 0 );

      this->consumer_list_.insert_tail (consumer_i);
      consumer = consumer_i->_this ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO_FDev::make_consumer");
      return consumer;
    }
  return consumer;
}

template <class T_Producer, class T_Consumer>
AVStreams::FlowConsumer_ptr
TAO_FDev<T_Producer, T_Consumer>::create_consumer (AVStreams::FlowConnection_ptr the_requester,
                                                   AVStreams::QoS & the_qos,
                                                   CORBA::Boolean_out met_qos,
                                                   char *& named_fdev)
{
  return this->make_consumer (the_requester,
                              the_qos,
                              met_qos,
                              named_fdev);
}

template <class T_Producer, class T_Consumer>
// not implemented yet.
AVStreams::FlowConnection_ptr
TAO_FDev<T_Producer, T_Consumer>::bind (AVStreams::FDev_ptr peer_device,
                                        AVStreams::QoS & the_qos,
                                        CORBA::Boolean_out is_met)
{
  ACE_UNUSED_ARG (peer_device);
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (is_met);
  return 0;
}

template <class T_Producer, class T_Consumer>
// multicast is not supported yet.
AVStreams::FlowConnection_ptr
TAO_FDev<T_Producer, T_Consumer>::bind_mcast (AVStreams::FDev_ptr first_peer,
                                              AVStreams::QoS & the_qos,
                                              CORBA::Boolean_out is_met)
{
  ACE_UNUSED_ARG (first_peer);
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (is_met);
  return 0;
}

template <class T_Producer, class T_Consumer>
void
TAO_FDev<T_Producer, T_Consumer>::destroy (AVStreams::FlowEndPoint_ptr /* the_ep */,
                                           const char * /* fdev_name */)
{
  // @@ Shouldn't the parameters be made use of!
  // Destroy/delete all the producers and consumers.

  TAO_FlowProducer *producer_i;

  for (PRODUCER_LIST_ITERATOR producer_list_iterator (this->producer_list_);
       (producer_i = producer_list_iterator.next ()) != 0;
       producer_list_iterator.advance ())
    {
      TAO_AV_Core::deactivate_servant (producer_i);
      delete producer_i;
    }

  TAO_FlowConsumer *consumer_i;

  for (CONSUMER_LIST_ITERATOR consumer_list_iterator (this->consumer_list_);
       (consumer_i = consumer_list_iterator.next ()) != 0;
       consumer_list_iterator.advance ())
    {
      TAO_AV_Core::deactivate_servant (consumer_i);
      delete consumer_i;
    }
  int result = TAO_AV_Core::deactivate_servant (this);
  if (result < 0)
    if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"TAO_StreamEndPoint::destroy failed\n"));
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_AV_FLOWS_T_CPP */
