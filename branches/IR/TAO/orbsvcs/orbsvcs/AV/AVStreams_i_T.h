// $Id$

#ifndef TAO_AV_AVSTREAMS_I_T_H
#define TAO_AV_AVSTREAMS_I_T_H

template <class IF>
class TAO_ORBSVCS_Export TAO_StreamEndPoint
  : public virtual TAO_Base_StreamEndPoint,
    public virtual TAO_PropertySet<IF>
{
  // = DESCRIPTION
  //    The Stream EndPoint. Used to implement one endpoint of a stream
  //    that implements the transport layer.
public:
  TAO_StreamEndPoint (void);
  // Constructor

  virtual void stop (const AVStreams::flowSpec &the_spec,
                     CORBA::Environment &env = CORBA::Environment::default_environment ());
   // Stop the stream. Empty the_spec means, for all the flows

  virtual void start (const AVStreams::flowSpec &the_spec,
                      CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Start the stream, Empty the_spec means, for all the flows

  virtual void destroy (const AVStreams::flowSpec &the_spec,
                        CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Destroy the stream, Empty the_spec means, for all the flows


  virtual CORBA::Boolean connect (AVStreams::StreamEndPoint_ptr responder,
                                  AVStreams::streamQoS &qos_spec,
                                  const AVStreams::flowSpec &the_spec,
                                  CORBA::Environment &env = CORBA::Environment::default_environment ()) = 0;
  // Called by StreamCtrl. responder is the peer to connect to

  virtual CORBA::Boolean request_connection (AVStreams::StreamEndPoint_ptr initiator,
                                             CORBA::Boolean is_mcast,
                                             AVStreams::streamQoS &qos,
                                             AVStreams::flowSpec &the_spec,
                                             CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Called by the peer StreamEndPoint. The flow_spec indicates the
  // flows (which contain transport addresses etc.)

  virtual CORBA::Boolean modify_QoS (AVStreams::streamQoS &new_qos,
                                     const AVStreams::flowSpec &the_flows,
                                     CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Change the transport qos on a stream

  virtual CORBA::Boolean set_protocol_restriction (const AVStreams::protocolSpec &the_pspec,
                                                   CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Used to restrict the set of protocols

  virtual void disconnect (const AVStreams::flowSpec &the_spec,
                           CORBA::Environment &env = CORBA::Environment::default_environment ());
  // disconnect the flows

  virtual void set_FPStatus (const AVStreams::flowSpec &the_spec,
                             const char *fp_name,
                             const CORBA::Any &fp_settings,
                             CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Used to control the flow

  virtual CORBA::Object_ptr get_fep (const char *flow_name,
                                     CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Not implemented in the light profile, throws notsupported

  virtual char * add_fep (CORBA::Object_ptr the_fep,
                          CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Not implemented in the light profile, throws notsupported

  virtual void remove_fep (const char *fep_name,
                           CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Not implemented in the light profile, throws notsupported

  virtual void set_negotiator (AVStreams::Negotiator_ptr new_negotiator,
                               CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Used to "attach" a negotiator to the endpoint

  virtual void set_key (const char *flow_name,
                        const AVStreams::key & the_key,
                        CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Used for public key encryption.

  virtual void set_source_id (CORBA::Long source_id,
                              CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Used to set a unique id for packets sent by this streamendpoint

  virtual ~TAO_StreamEndPoint (void);
  // Destructor

private:
  u_int flow_count_;
  // Count of the number of flows in this streamendpoint, used to
  // generate unique names for the flows.
  u_int flow_num_;
  // current flow number used for system generation of flow names.
  typedef ACE_Hash_Map_Manager <TAO_String_Hash_Key,CORBA::Object_ptr,ACE_Null_Mutex>
  FlowEndPoint_Map;
  FlowEndPoint_Map fep_map_;
  // hash table for the flownames and its corresponding flowEndpoint
  // reference.
  AVStreams::flowSpec flows_;
  // sequence of supported flow names.
  CORBA::Long source_id_;
  // source id used for multicast.
};


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "AVStreams_i_T.cpp"
#endif /*ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("AVStreams_i_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_AV_AVSTREAMS_I_T_H */
