/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   AVStreams_i.h
//
// = AUTHOR
//    Sumedh Mungee <sumedh@cs.wustl.edu>
//    Nagarajan Surendran <naga@cs.wustl.edu>
//
//
// ============================================================================

#ifndef AVSTREAMS_I_H
#define AVSTREAMS_I_H

#include "orbsvcs/orbsvcs_export.h"
#include "orbsvcs/CosPropertyServiceS.h"
#include "orbsvcs/AVStreamsS.h"
#include "orbsvcs/Property/CosPropertyService_i.h"
#include "ace/Process.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/Null_MediaCtrlS.h"

#include "orbsvcs/Trader/Trader.h"
// for the Hash_Map helper classes.
class TAO_ORBSVCS_Export AV_Null_MediaCtrl : public virtual POA_Null_MediaCtrl
{
  public:
  AV_Null_MediaCtrl (void);
};


class TAO_ORBSVCS_Export TAO_Basic_StreamCtrl 
  : public virtual POA_AVStreams::Basic_StreamCtrl,
    public virtual TAO_PropertySet<POA_AVStreams::Basic_StreamCtrl>
  // = DESCRIPTION
  //    Base class for StreamCtrl, implements basic stream start
  //    and stop functionality
{
 public:
  TAO_Basic_StreamCtrl (void);
  // Default Constructor

  virtual void stop (const AVStreams::flowSpec &the_spec,
                     CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Stop the transfer of data of the stream
  // Empty the_spec means apply operation to all flows

  virtual void start (const AVStreams::flowSpec &the_spec,
                      CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Start the transfer of data in the stream.
  // Empty the_spec means apply operation to all flows

  virtual void destroy (const AVStreams::flowSpec &the_spec,
                        CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Tears down the stream. This will close the connection, and delete
  // the streamendpoint and vdev associated with this stream
  // Empty the_spec means apply operation to all flows

  virtual CORBA::Boolean modify_QoS (AVStreams::streamQoS &new_qos,
                                     const AVStreams::flowSpec &the_spec,
                                     CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Changes the QoS associated with the stream
  // Empty the_spec means apply operation to all flows

  virtual void push_event (const struct CosPropertyService::Property & the_event,  
                           CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Used by StreamEndPoint and VDev to inform StreamCtrl of events.
  // E.g., loss of flow, reestablishment of flow, etc..

  virtual void set_FPStatus (const AVStreams::flowSpec &the_spec,
                             const char *fp_name,
                             const CORBA::Any &fp_settings,
                             CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Used to control the flow protocol parameters.

  virtual CORBA::Object_ptr get_flow_connection (const char *flow_name,
                                                 CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Not implemented in the light profile, will raise the notsupported
  // exception

  virtual void set_flow_connection (const char *flow_name,
                                    CORBA::Object_ptr flow_connection,
                                    CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Not implemented in the light profile, will raise the notsupported
  // exception

  virtual ~TAO_Basic_StreamCtrl (void);
  // Destructor

 protected:

  AVStreams::VDev_var vdev_a_;
  AVStreams::VDev_var vdev_b_;
  // The Virtual Devices for this stream

  AVStreams::StreamEndPoint_A_var sep_a_;
  AVStreams::StreamEndPoint_B_var sep_b_;
  // The Endpoints for this stream

  typedef ACE_Hash_Map_Manager <TAO_String_Hash_Key,CORBA::Object_ptr,ACE_Null_Mutex> FlowConnection_Map;
  FlowConnection_Map flow_map_;
  // Hash table for the flow names and its corresponding flowconnection object reference.
  AVStreams::FlowConnection_seq flowConnections_;

  u_int flow_count_;
  AVStreams::flowSpec flows_;
  //sequence of flow names.
};

class TAO_ORBSVCS_Export TAO_Negotiator
  : public POA_AVStreams::Negotiator
{
public:
  virtual CORBA::Boolean negotiate (AVStreams::Negotiator_ptr remote_negotiator,
                                    const AVStreams::streamQoS &qos_spec,
                                    CORBA::Environment &ACE_TRY_ENV = CORBA::Environment::default_environment ());
};

class TAO_MCastConfigIf;

class TAO_ORBSVCS_Export TAO_StreamCtrl
  : public virtual POA_AVStreams::StreamCtrl,
    public virtual TAO_Basic_StreamCtrl
// = DESCRIPTION
//    Implementation the A/V StreamCtrl class. this class
//    is used to control the stream. It should be subclassed
//    by applications that want to provide more control features.
{
public:
  TAO_StreamCtrl (void);
  // Default Constructor

  virtual CORBA::Boolean bind_devs (AVStreams::MMDevice_ptr a_party,
                                    AVStreams::MMDevice_ptr b_party,
                                    AVStreams::streamQoS &the_qos,
                                    const AVStreams::flowSpec &the_flows,
                                    CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Establish a stream between a_party and b_party,
  // with qos the_qos, and for the flows in the_flows
  // if the_flows is empty, bind all the flows
  // Causes a connection to be established between the StreamEndpoints.
  // Returns success/failure

  virtual CORBA::Boolean bind (AVStreams::StreamEndPoint_A_ptr a_party,
                               AVStreams::StreamEndPoint_B_ptr b_party,
                               AVStreams::streamQoS &the_qos,
                               const AVStreams::flowSpec &the_flows,
                               CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Establish a connection between two streamendpoints. This can
  // be used if the streamendpoints have been created independent of
  // a MMDevice

  virtual void unbind_party (AVStreams::StreamEndPoint_ptr the_ep,
                             const AVStreams::flowSpec &the_spec,
                             CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Unbind the_ep from the stream. Empty the_spec means apply to all flows.

  virtual void unbind (CORBA::Environment &env = CORBA::Environment::default_environment ());
  // unbind the stream. Same effect as Basic_StreamCtrl::destroy ()

  virtual ~TAO_StreamCtrl (void);
  // Destructor.

private:
  TAO_MCastConfigIf *mcastconfigif_;
  AVStreams::MCastConfigIf_ptr mcastconfigif_ptr_;
};

class TAO_ORBSVCS_Export TAO_MCastConfigIf
  : public virtual POA_AVStreams::MCastConfigIf,
    public virtual TAO_PropertySet<POA_AVStreams::MCastConfigIf>
{
public:
  virtual CORBA::Boolean set_peer (CORBA::Object_ptr peer,
                                   AVStreams::streamQoS & the_qos,
                                   const AVStreams::flowSpec & the_spec,
                                   CORBA::Environment &ACE_TRY_ENV = CORBA::Environment::default_environment ()) ;

  virtual void configure (const CosPropertyService::Property & a_configuration,
                          CORBA::Environment &ACE_TRY_ENV = CORBA::Environment::default_environment ()) ;

  virtual void set_initial_configuration (const CosPropertyService::Properties & initial,
                                          CORBA::Environment &ACE_TRY_ENV = CORBA::Environment::default_environment ()) ;

  virtual void set_format (const char * flowName,
                           const char * format_name,
                           CORBA::Environment &ACE_TRY_ENV = 
                           CORBA::Environment::default_environment ()) ;

  virtual void set_dev_params (const char * flowName,
                               const CosPropertyService::Properties & new_params,
                               CORBA::Environment &ACE_TRY_ENV = CORBA::Environment::default_environment ()) ;

};

class TAO_ORBSVCS_Export TAO_Base_StreamEndPoint
// = DESCRIPTION
//    Base class for the A/V StreamEndPoint class. this class
//    is used to control the stream. It should be subclassed
//    by applications that want to provide more control features.
{
public:
  virtual ~TAO_Base_StreamEndPoint (void);

  virtual int handle_open (void) = 0;
  // called when streamendpoint is instantiated

  virtual int handle_close (void) = 0;
  // called when streamendpoint is being destructed

  virtual int handle_stop (const AVStreams::flowSpec &the_spec,
                            CORBA::Environment &env = CORBA::Environment::default_environment ()) = 0;
  // Application needs to define this

  virtual int handle_start (const AVStreams::flowSpec &the_spec,
                             CORBA::Environment &env = CORBA::Environment::default_environment ()) = 0;
  // Application needs to define this


  virtual int handle_destroy (const AVStreams::flowSpec &the_spec,
                               CORBA::Environment &env = CORBA::Environment::default_environment ()) = 0;
  // Application needs to define this
};

class TAO_ORBSVCS_Export TAO_Client_Base_StreamEndPoint
  : public virtual TAO_Base_StreamEndPoint
{
public:
  virtual CORBA::Boolean handle_preconnect (AVStreams::flowSpec &the_spec) = 0;
  // Application needs to define this

  virtual CORBA::Boolean handle_postconnect (AVStreams::flowSpec &the_spec) = 0;
  // Application needs to define this
};

class TAO_ORBSVCS_Export TAO_Server_Base_StreamEndPoint
  : public virtual TAO_Base_StreamEndPoint
{
public:
  virtual CORBA::Boolean handle_connection_requested (AVStreams::flowSpec &the_spec,
                                                      CORBA::Environment &env = CORBA::Environment::default_environment ()) = 0;
  // Application needs to define this
};


// Include the templates here.
#include "AVStreams_i_T.h"

class TAO_ORBSVCS_Export TAO_Client_StreamEndPoint :
  public virtual TAO_StreamEndPoint<POA_AVStreams::StreamEndPoint_A>,
  public virtual TAO_Client_Base_StreamEndPoint
{
  // = DESCRIPTION
  //     The "A" side of a streamendpoint
public:
  TAO_Client_StreamEndPoint (void);
  // Constructor

  virtual CORBA::Boolean connect (AVStreams::StreamEndPoint_ptr responder,
                                  AVStreams::streamQoS &qos_spec,
                                  const AVStreams::flowSpec &the_spec,
                                  CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Called by StreamCtrl. responder is the peer to connect to

  virtual CORBA::Boolean multiconnect (AVStreams::streamQoS &the_qos,
                                       AVStreams::flowSpec &the_spec,
                                       CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Used for ATM-style multicast

  virtual CORBA::Boolean connect_leaf (AVStreams::StreamEndPoint_B_ptr the_ep,
                                       AVStreams::streamQoS &the_qos,
                                       const AVStreams::flowSpec &the_flows,
                                       CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Used for ATM-style multicast

  virtual void disconnect_leaf (AVStreams::StreamEndPoint_B_ptr the_ep,
                                const AVStreams::flowSpec &theSpec,
                                CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Used to remove a multicast leaf

  virtual ~TAO_Client_StreamEndPoint (void);
  // Destructor

};

class TAO_ORBSVCS_Export TAO_Server_StreamEndPoint :
  public virtual POA_AVStreams::StreamEndPoint_B,
  public virtual TAO_StreamEndPoint<POA_AVStreams::StreamEndPoint_B>,
  public virtual TAO_Server_Base_StreamEndPoint // Abstract interface
{
  // = DESCRIPTION
  //     The "B" side of a streamendpoint
public:
  TAO_Server_StreamEndPoint (void);
  // Constructor

  virtual CORBA::Boolean connect (AVStreams::StreamEndPoint_ptr responder,
                                  AVStreams::streamQoS &qos_spec,
                                  const AVStreams::flowSpec &the_spec,
                                  CORBA::Environment &env = CORBA::Environment::default_environment ());

  virtual CORBA::Boolean request_connection (AVStreams::StreamEndPoint_ptr initiator,
                                             CORBA::Boolean is_mcast,
                                             AVStreams::streamQoS &qos,
                                             AVStreams::flowSpec &the_spec,
                                             CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Called by the peer StreamEndPoint. The flow_spec indicates the
  // flows (which contain transport addresses etc.)

  virtual CORBA::Boolean multiconnect (AVStreams::streamQoS &the_qos,
                                       AVStreams::flowSpec &the_spec,
                                       CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Used for internet-style multicast

  virtual ~TAO_Server_StreamEndPoint (void);
  // Destructor
};

class TAO_ORBSVCS_Export TAO_VDev
  :public virtual TAO_PropertySet<POA_AVStreams::VDev>,
   public virtual POA_AVStreams::VDev
// = DESCRIPTION
//    Implements the VDev interface. One of these is created per connection,
//    and represents device-specific parameters
{
 public:
  TAO_VDev (void);
  // Default Constructor

  virtual CORBA::Boolean set_peer (AVStreams::StreamCtrl_ptr the_ctrl,
                                   AVStreams::VDev_ptr the_peer_dev,
                                   AVStreams::streamQoS &the_qos,
                                   const AVStreams::flowSpec &the_spec,
                                   CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Called to tell the vdev who the streamctrl, peer vdev is

  virtual CORBA::Boolean set_Mcast_peer (AVStreams::StreamCtrl_ptr the_ctrl,
                                         AVStreams::MCastConfigIf_ptr a_mcastconfigif,
                                         AVStreams::streamQoS &the_qos,
                                         const AVStreams::flowSpec &the_spec,
                                         CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Used to set the streamctrl and multicast device

  virtual void configure (const CosPropertyService::Property &the_config_mesg,
                          CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Called by the peer VDev to configure the device (catch all)

  virtual void set_format (const char *flowName,
                           const char *format_name,
                           CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Used to set a format on a flowname

  virtual void set_dev_params (const char *flowName,
                               const CosPropertyService::Properties &new_params,
                               CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Used to set device parameters

  virtual CORBA::Boolean modify_QoS (AVStreams::streamQoS &the_qos,
                                     const AVStreams::flowSpec &the_spec,
                                     CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Called to change QoS of the device

  virtual ~TAO_VDev (void);
  // Destructor

 protected:
  virtual CORBA::Boolean set_media_ctrl (CORBA::Object_ptr media_ctrl,
                                         CORBA::Environment &env = CORBA::Environment::default_environment ());
  // hook called after set_peer is done to set the media ctrl of the peer vdev.
 private:
  AVStreams::StreamCtrl_var streamctrl_;
  // My stream controller

  AVStreams::VDev_var peer_;
  // My peer
};

class TAO_AV_Endpoint_Strategy;

class TAO_ORBSVCS_Export TAO_MMDevice
  :public virtual POA_AVStreams::MMDevice,
   public TAO_PropertySet<POA_AVStreams::MMDevice>

// = DESCRIPTION
//     Implements a factory to create Endpoints and VDevs
{
 protected:

  TAO_AV_Endpoint_Strategy *endpoint_strategy_;

 public:

  TAO_MMDevice (TAO_AV_Endpoint_Strategy *endpoint_strategy_);
  // Constructor

  virtual AVStreams::StreamCtrl_ptr  bind (AVStreams::MMDevice_ptr peer_device,
                                           AVStreams::streamQoS &the_qos,
                                           CORBA::Boolean_out is_met,
                                           const AVStreams::flowSpec &the_spec,
                                           CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Can be used to request the MMDevice to create a new StreamCtrl,
  // and call bind_devs on it

  virtual AVStreams::StreamCtrl_ptr  bind_mcast (AVStreams::MMDevice_ptr first_peer,
                                                 AVStreams::streamQoS &the_qos,
                                                 CORBA::Boolean_out is_met,
                                                 const AVStreams::flowSpec &the_spec,
                                                 CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Multicast bind

  virtual AVStreams::StreamEndPoint_A_ptr  create_A (AVStreams::StreamCtrl_ptr the_requester,
                                                     AVStreams::VDev_out the_vdev,
                                                     AVStreams::streamQoS &the_qos,
                                                     CORBA::Boolean_out met_qos,
                                                     char *&named_vdev,
                                                     const AVStreams::flowSpec &the_spec,
                                                     CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Called by StreamCtrl to create a "A" type streamandpoint and vdev

  virtual AVStreams::StreamEndPoint_B_ptr  create_B (AVStreams::StreamCtrl_ptr the_requester,
                                                     AVStreams::VDev_out the_vdev,
                                                     AVStreams::streamQoS &the_qos,
                                                     CORBA::Boolean_out met_qos,
                                                     char *&named_vdev,
                                                     const AVStreams::flowSpec &the_spec,
                                                     CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Called by StreamCtrl to create a "B" type streamandpoint and vdev

  virtual void destroy (AVStreams::StreamEndPoint_ptr the_ep,
                        const char *vdev_name,
                        CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Remove the StreamEndPoint and the related vdev

  virtual char * add_fdev (CORBA::Object_ptr the_fdev,
                           CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Not supported in the light profile, raises notsupported

  virtual CORBA::Object_ptr get_fdev (const char *flow_name,
                                      CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Not supported in the light profile, raises notsupported

  virtual void remove_fdev (const char *flow_name,
                            CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Not supported in the light profile, raises notsupported

  virtual ~TAO_MMDevice (void);
  // Destructor

private:
  u_int flow_count_;
  // Count of the number of flows in this MMDevice , used to
  // generate unique names for the flows.
  u_int flow_num_;
  // current flow number used for system generation of flow names.
  typedef ACE_Hash_Map_Manager <TAO_String_Hash_Key,CORBA::Object_ptr,ACE_Null_Mutex>
    FDev_Map;
  FDev_Map fdev_map_;
  // hash table for the flownames and its corresponding flowEndpoint
  // reference.
  AVStreams::flowSpec flows_;
  // sequence of supported flow names.
};

class TAO_FlowConsumer;
class TAO_FlowProducer;

class TAO_ORBSVCS_Export TAO_FlowConnection
 : public virtual POA_AVStreams::FlowConnection,
   public TAO_PropertySet<POA_AVStreams::FlowConnection>
					   
{
  //  =TITLE
  //     Class to manage a flow connection.
  //
  //  =Description
  //     This class currently supports only one producer and one
  //     consumer  per flow.
public:
  TAO_FlowConnection (void);
  // default constructor.

  virtual void stop (CORBA::Environment &env = CORBA::Environment::default_environment ());
  // stop this flow.

  virtual void start (CORBA::Environment &env = CORBA::Environment::default_environment ());
  // start this flow.

  virtual void destroy (CORBA::Environment &env = CORBA::Environment::default_environment ());
  // destroy this flow.

  virtual CORBA::Boolean modify_QoS (AVStreams::QoS & new_qos,
                                     CORBA::Environment &env = CORBA::Environment::default_environment ());
  // modify the QoS for this flow.

  virtual CORBA::Boolean use_flow_protocol (const char * fp_name,
                                            const CORBA::Any & fp_settings,
                                            CORBA::Environment &env = CORBA::Environment::default_environment ());
  // use the specified flow protocol for this flow.

  virtual void push_event (const AVStreams::streamEvent & the_event,
                           CORBA::Environment &env = CORBA::Environment::default_environment ());

  // pushes an event , to be handled by the application.

  virtual CORBA::Boolean connect_devs (AVStreams::FDev_ptr a_party,
                                       AVStreams::FDev_ptr b_party,
                                       AVStreams::QoS & the_qos,
                                       CORBA::Environment &env = CORBA::Environment::default_environment ());
  // connect 2 Flow Devices.
 
  virtual CORBA::Boolean connect (AVStreams::FlowProducer_ptr flow_producer,
                                  AVStreams::FlowConsumer_ptr flow_consumer,
                                  AVStreams::QoS & the_qos,
                                  CORBA::Environment &env = CORBA::Environment::default_environment ());
  // Connect a flow producer and consumer under this flow connection.

  virtual CORBA::Boolean disconnect (CORBA::Environment &env = CORBA::Environment::default_environment ());
  // disconnect this flow connection.

  virtual CORBA::Boolean add_producer (AVStreams::FlowProducer_ptr flow_producer,
                                       AVStreams::QoS & the_qos,
                                       CORBA::Environment &env = CORBA::Environment::default_environment ());
  // adds the producer to this flow connection.
 
  virtual CORBA::Boolean add_consumer (AVStreams::FlowConsumer_ptr flow_consumer,
                                       AVStreams::QoS & the_qos,
                                       CORBA::Environment &env = CORBA::Environment::default_environment ());
  // adds a consumer to this flow connection.

  virtual CORBA::Boolean drop (AVStreams::FlowEndPoint_ptr target,
                               CORBA::Environment &env = CORBA::Environment::default_environment ());
  // drops a flow endpoint from the flow.

private:
  AVStreams::FlowProducer *producer_;
  // The producer of this flow.
  AVStreams::FlowConsumer *consumer_;
  // The consumer of this flow
  char * fp_name_;
  // name of the flow protocol to be used.
};

class TAO_ORBSVCS_Export TAO_FlowEndPoint :
  public virtual POA_AVStreams::FlowEndPoint,
  public virtual TAO_PropertySet<POA_AVStreams::FlowEndPoint>
{
  // = DESCRIPTION
  //     This class is used per flow e.g video flow and an audio flow
  //     to encapuslate the transport details. 

  public:

  TAO_FlowEndPoint (void);
  //default constructor.

  virtual CORBA::Boolean lock (CORBA::Environment &env =
                               CORBA::Environment::default_environment ());
  // lock the flow endpoint for a particular flow.

  virtual void unlock (CORBA::Environment &env =
                       CORBA::Environment::default_environment ());
  // unlock the flow endpoint for subsequent use.

  virtual void stop (CORBA::Environment &env =
                     CORBA::Environment::default_environment ());
  // stop this flow, to be overridden by the application.  

  virtual void start (CORBA::Environment &env =
                      CORBA::Environment::default_environment ());
  // start this flow, to be overridden by the application.

  virtual void destroy (CORBA::Environment &env =
                        CORBA::Environment::default_environment ());
  // destroy this flow.

  virtual AVStreams::StreamEndPoint_ptr related_sep
    (CORBA::Environment &env = 
     CORBA::Environment::default_environment ());
  // get method for the related streamendpoint under which this
  // flowendpoint is.

  virtual void related_sep (AVStreams::StreamEndPoint_ptr related_sep,
                            CORBA::Environment &env = 
                            CORBA::Environment::default_environment ());
  // set method for the related streamendpoint under which this
  // flowendpoint is.

  virtual AVStreams::FlowConnection_ptr related_flow_connection(CORBA::Environment &env = 
                                                                CORBA::Environment::default_environment ());

  // accessor for the related flow connection attribute.

  virtual void related_flow_connection (AVStreams::FlowConnection_ptr related_flow_connection,
                                        CORBA::Environment &env = CORBA::Environment::default_environment ());
  // set method for the related flow connection attribute.

  virtual AVStreams::FlowEndPoint_ptr get_connected_fep
    (CORBA::Environment &env = 
     CORBA::Environment::default_environment ());
  // returns the other flowendpoint to which this is connected.

  virtual CORBA::Boolean use_flow_protocol (const char * fp_name,
                                            const CORBA::Any & fp_settings,
                                            CORBA::Environment &env = CORBA::Environment::default_environment ());
  /// use the specified flow protocol.

  virtual void set_format (const char * format,
                           CORBA::Environment &env = CORBA::Environment::default_environment ());
  // sets the data format.

  virtual void set_dev_params (const CosPropertyService::Properties & new_settings,
                               CORBA::Environment &env = CORBA::Environment::default_environment ());
  // sets the device parameters.

  virtual void set_protocol_restriction (const AVStreams::protocolSpec & the_spec,
                                         CORBA::Environment &env = CORBA::Environment::default_environment ());
  // sets the list of protocols to be used.

  virtual CORBA::Boolean is_fep_compatible (AVStreams::FlowEndPoint_ptr fep,
                                            CORBA::Environment &env = CORBA::Environment::default_environment ());
  // checks whether the passed flowendpoint is compatible with this.

  virtual CORBA::Boolean set_peer (AVStreams::FlowConnection_ptr the_fc,
                                   AVStreams::FlowEndPoint_ptr the_peer_fep,
                                   AVStreams::QoS & the_qos,
                                   CORBA::Environment &env = CORBA::Environment::default_environment ());
  //sets the peer flowendpoint.

  virtual CORBA::Boolean set_Mcast_peer (AVStreams::FlowConnection_ptr the_fc,
                                         AVStreams::MCastConfigIf_ptr a_mcastconfigif,
                                         AVStreams::QoS & the_qos,
                                         CORBA::Environment &env =
                                         CORBA::Environment::default_environment ());
  ///sets the multicast peer flowendpoint, not implemented.
private:
  AVStreams::StreamEndPoint_ptr related_sep_;
  // The related streamendpoint.
  AVStreams::FlowConnection_ptr related_flow_connection_;
  // The related flow connection reference
  AVStreams::FlowEndPoint_ptr peer_fep_;
  // The peer flowendpoint reference.
};

class TAO_ORBSVCS_Export TAO_FlowProducer:
  public virtual POA_AVStreams::FlowProducer,
         public virtual TAO_FlowEndPoint
{
  public:
  TAO_FlowProducer (void);
  // default constructor

  virtual CORBA::Boolean connect_to_peer (AVStreams::QoS & the_qos,
                                          const char * address,
                                          const char * use_flow_protocol,
                                          CORBA::Environment &env =
                                          CORBA::Environment::default_environment ());
  // connect to the peer endpoint.

  virtual CORBA::Boolean handle_connect_to_peer (AVStreams::QoS & the_qos,
                                                 const char * address,
                                                 const char * use_flow_protocol,
                                                 CORBA::Environment &env =
                                                 CORBA::Environment::default_environment ());

  // hook method to be overridden by the application to handle the connection request.

  virtual char * connect_mcast (AVStreams::QoS & the_qos,
                                CORBA::Boolean_out is_met,
                                const char * address,
                                const char * use_flow_protocol,
                                CORBA::Environment &env = CORBA::Environment::default_environment ());

  // connect to the multicast address, not implemented.

  virtual char * get_rev_channel (const char * pcol_name,
                                  CORBA::Environment &env = CORBA::Environment::default_environment ());
  // get the reverse channel, to be used for feedback for protocols like UDP.

  virtual void set_key (const AVStreams::key & the_key,
                        CORBA::Environment &env = CORBA::Environment::default_environment ());
  // sets the public key  to be used for encryption of the data.

  virtual void set_source_id (CORBA::Long source_id,
                              CORBA::Environment &env =
                              CORBA::Environment::default_environment
                              ());
  // sets the source id of this flow producer so that it can be used
  // to distinguish this producer from others in the multicast case.

  private:
  CORBA::Long source_id_;
  // source id of this producer.
};

class TAO_ORBSVCS_Export TAO_FlowConsumer :
  public virtual POA_AVStreams::FlowConsumer,
         public virtual TAO_FlowEndPoint
{
  public:
  TAO_FlowConsumer (void);
  // default constructor.

  virtual char * go_to_listen (AVStreams::QoS & the_qos,
                               CORBA::Boolean is_mcast,
                               AVStreams::FlowProducer_ptr peer,
                               char *& flowProtocol,
                               CORBA::Environment &env =
                               CORBA::Environment::default_environment
                               ());

  // listen request from the peer.

  virtual char * handle_go_to_listen (AVStreams::QoS & the_qos,
                               CORBA::Boolean is_mcast,
                               AVStreams::FlowProducer_ptr peer,
                               char *& flowProtocol,
                               CORBA::Environment &env =
                               CORBA::Environment::default_environment
                               ());
  // applications should override this method.

};

class TAO_ORBSVCS_Export TAO_FDev :
  public virtual POA_AVStreams::FDev,
  public virtual TAO_PropertySet<POA_AVStreams::FDev>
{
  public:
  TAO_FDev (void);
  // default constructor

  AVStreams::FlowProducer_ptr create_producer (AVStreams::FlowConnection_ptr the_requester,
                                                       AVStreams::QoS & the_qos,
                                                       CORBA::Boolean_out met_qos,
                                                       char *& named_fdev,
                                                       CORBA::Environment &env = CORBA::Environment::default_environment ());
  // create a flow producer object.

  virtual AVStreams::FlowProducer_ptr make_producer (AVStreams::FlowConnection_ptr the_requester,
                                                       AVStreams::QoS & the_qos,
                                                       CORBA::Boolean_out met_qos,
                                                       char *& named_fdev,
                                                       CORBA::Environment &env = CORBA::Environment::default_environment ());
  // bridge method for the application to override the producer object
  // creation. Default implementation creates a TAO_FlowProducer.

  virtual AVStreams::FlowConsumer_ptr create_consumer (AVStreams::FlowConnection_ptr the_requester,
                                                       AVStreams::QoS & the_qos,
                                                       CORBA::Boolean_out met_qos,
                                                       char *& named_fdev,
                                                       CORBA::Environment &env = CORBA::Environment::default_environment ());
  // create a flow consumer object.

  virtual AVStreams::FlowConsumer_ptr make_consumer (AVStreams::FlowConnection_ptr the_requester,
                                                       AVStreams::QoS & the_qos,
                                                       CORBA::Boolean_out met_qos,
                                                       char *& named_fdev,
                                                       CORBA::Environment &env = CORBA::Environment::default_environment ());
  // bridge method for the application to override the consumer object
  // creation. Default implementation creates a TAO_FlowConsumer.

  virtual AVStreams::FlowConnection_ptr bind (AVStreams::FDev_ptr peer_device,
                                              AVStreams::QoS & the_qos,
                                              CORBA::Boolean_out is_met,
                                              CORBA::Environment &env = CORBA::Environment::default_environment ());
  // bind this FDev with another FDev.

  virtual AVStreams::FlowConnection_ptr bind_mcast (AVStreams::FDev_ptr first_peer,
                                                    AVStreams::QoS & the_qos,
                                                    CORBA::Boolean_out is_met,
                                                    CORBA::Environment &env = CORBA::Environment::default_environment ());
  // multicast bind is not implemented yet.

  virtual void destroy (AVStreams::FlowEndPoint_ptr the_ep,
                        const char * fdev_name,
                        CORBA::Environment &env =
                        CORBA::Environment::default_environment ());
  // destroys this FDev.

  private:
  AVStreams::FlowProducer_ptr producer_;
  AVStreams::FlowConsumer_ptr consumer_;
  // references to the created producers and consumers.
};

class TAO_ORBSVCS_Export TAO_MediaControl
  :public virtual POA_AVStreams::MediaControl
{
  // = TITLE
  //     Abstract Mediacontrol class.
  //
  // = DESCRIPTION
  //     The following are to be handled by the specialized media control for the specific
  //     media like camera,speaker.
  public:
  TAO_MediaControl (void);
  // default constructor

  virtual AVStreams::Position get_media_position (AVStreams::PositionOrigin an_origin,
                                                  AVStreams::PositionKey a_key,
                                                  CORBA::Environment &env = CORBA::Environment::default_environment ()) =0;

  virtual void set_media_position (const AVStreams::Position & a_position,
                                   CORBA::Environment &env = CORBA::Environment::default_environment ()) =0;

  virtual void start (const AVStreams::Position & a_position,
                      CORBA::Environment &env = CORBA::Environment::default_environment ()) =0;

  virtual void pause (const AVStreams::Position & a_position,
                      CORBA::Environment &env = CORBA::Environment::default_environment ()) =0;

  virtual void resume (const AVStreams::Position & a_position,
                       CORBA::Environment &env = CORBA::Environment::default_environment ()) =0;

  virtual void stop (const AVStreams::Position & a_position,
                     CORBA::Environment &env = CORBA::Environment::default_environment ()) = 0;

};

#endif /* AVSTREAMS_I_H */
