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
//    
// 
// ============================================================================

#if !defined (AVSTREAMS_I_H)
#define	AVSTREAMS_I_H

#include "orbsvcs/orbsvcs_export.h"
#include "orbsvcs/CosPropertyServiceS.h"
#include "orbsvcs/AVStreamsS.h"
#include "orbsvcs/Property/CosPropertyService_i.h"
#include "ace/Process.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/Null_MediaCtrlS.h"

class TAO_ORBSVCS_Export AV_Null_MediaCtrl
  : public virtual POA_Null_MediaCtrl
{
public:
  AV_Null_MediaCtrl (void);
};

class TAO_ORBSVCS_Export TAO_Basic_StreamCtrl 
  : public virtual POA_AVStreams::Basic_StreamCtrl
// = DESCRIPTION
//    Base class for StreamCtrl, implements basic stream start
//    and stop functionality 
{
 public:
  TAO_Basic_StreamCtrl (void);
  // Default Constructor

  virtual void stop (const AVStreams::flowSpec &the_spec,  
                     CORBA::Environment &env);
  // Stop the transfer of data of the stream
  // Empty the_spec means apply operation to all flows

  virtual void start (const AVStreams::flowSpec &the_spec,  
                      CORBA::Environment &env);
  // Start the transfer of data in the stream. 
  // Empty the_spec means apply operation to all flows   

  virtual void destroy (const AVStreams::flowSpec &the_spec,  
                        CORBA::Environment &env);
  // Tears down the stream. This will close the connection, and delete 
  // the streamendpoint and vdev associated with this stream
  // Empty the_spec means apply operation to all flows

  virtual CORBA::Boolean modify_QoS (AVStreams::streamQoS &new_qos, 
                                     const AVStreams::flowSpec &the_spec,  
                                     CORBA::Environment &env);
  // Changes the QoS associated with the stream
  // Empty the_spec means apply operation to all flows

  virtual void push_event (const char *the_event,  
                           CORBA::Environment &env);
  // Used by StreamEndPoint and VDev to inform StreamCtrl of events.
  // E.g., loss of flow, reestablishment of flow, etc..
   
  virtual void set_FPStatus (const AVStreams::flowSpec &the_spec, 
                             const char *fp_name, 
                             const CORBA::Any &fp_settings,  
                             CORBA::Environment &env);
  // Used to control the flow protocol parameters.

  virtual CORBA::Object_ptr get_flow_connection (const char *flow_name,  
                                                 CORBA::Environment &env);
  // Not implemented in the light profile, will raise the notsupported 
  // exception 
   
  virtual void set_flow_connection (const char *flow_name, 
                                    CORBA::Object_ptr flow_connection,  
                                    CORBA::Environment &env);
  // Not implemented in the light profile, will raise the notsupported
  // exception
   
  virtual ~TAO_Basic_StreamCtrl (void);
  // Destructor

 protected:
  
  AVStreams::VDev_var vdev_a_;
  AVStreams::VDev_var vdev_b_;
  // The Virtual Devices for this stream
   
  AVStreams::StreamEndPoint_A_var stream_endpoint_a_;
  AVStreams::StreamEndPoint_B_var stream_endpoint_b_;
  // The Endpoints for this stream
};

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
                                    CORBA::Environment &env);
  // Establish a stream between a_party and b_party,
  // with qos the_qos, and for the flows in the_flows
  // if the_flows is empty, bind all the flows
  // Causes a connection to be established between the StreamEndpoints.
  // Returns success/failure

  virtual CORBA::Boolean bind (AVStreams::StreamEndPoint_A_ptr a_party, 
                               AVStreams::StreamEndPoint_B_ptr b_party, 
                               AVStreams::streamQoS &the_qos, 
                               const AVStreams::flowSpec &the_flows,  
                               CORBA::Environment &env);
  // Establish a connection between two streamendpoints. This can
  // be used if the streamendpoints have been created independent of
  // a MMDevice
   
  virtual void unbind_party (AVStreams::StreamEndPoint_ptr the_ep, 
                             const AVStreams::flowSpec &the_spec,  
                             CORBA::Environment &env);
  // Unbind the_ep from the stream. Empty the_spec means apply to all flows. 
   
  virtual void unbind (CORBA::Environment &env);
  // unbind the stream. Same effect as Basic_StreamCtrl::destroy ()
   
  virtual ~TAO_StreamCtrl (void);
  // Destructor

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
                            CORBA::Environment &env) = 0;
  // Application needs to define this
  
  virtual int handle_start (const AVStreams::flowSpec &the_spec,  
                             CORBA::Environment &env) = 0;
  // Application needs to define this

  
  virtual int handle_destroy (const AVStreams::flowSpec &the_spec,  
                               CORBA::Environment &env) = 0;
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
                                                      CORBA::Environment &env) = 0;
  // Application needs to define this
};

class TAO_ORBSVCS_Export TAO_StreamEndPoint 
  : public virtual POA_AVStreams::StreamEndPoint, // The POA class
    public virtual TAO_Base_StreamEndPoint
{
  // = DESCRIPTION
  //    The Stream EndPoint. Used to implement one endpoint of a stream
  //    that implements the transport layer.
public:
  TAO_StreamEndPoint (void);
  // Constructor

  virtual void stop (const AVStreams::flowSpec &the_spec,  
                     CORBA::Environment &env);
   // Stop the stream. Empty the_spec means, for all the flows
  
  virtual void start (const AVStreams::flowSpec &the_spec,  
                      CORBA::Environment &env);
  // Start the stream, Empty the_spec means, for all the flows

  virtual void destroy (const AVStreams::flowSpec &the_spec,  
                        CORBA::Environment &env);
  // Destroy the stream, Empty the_spec means, for all the flows


  virtual CORBA::Boolean connect (AVStreams::StreamEndPoint_ptr responder, 
                                  AVStreams::streamQoS &qos_spec, 
                                  const AVStreams::flowSpec &the_spec,  
                                  CORBA::Environment &env) = 0;
  // Called by StreamCtrl. responder is the peer to connect to

  virtual CORBA::Boolean request_connection (AVStreams::StreamEndPoint_ptr initiator, 
                                             CORBA::Boolean is_mcast, 
                                             AVStreams::streamQoS &qos, 
                                             AVStreams::flowSpec &the_spec,  
                                             CORBA::Environment &env);
  // Called by the peer StreamEndPoint. The flow_spec indicates the
  // flows (which contain transport addresses etc.)

  virtual CORBA::Boolean modify_QoS (AVStreams::streamQoS &new_qos, 
                                     const AVStreams::flowSpec &the_flows,  
                                     CORBA::Environment &env);
  // Change the transport qos on a stream
   
  virtual CORBA::Boolean set_protocol_restriction (const AVStreams::protocolSpec &the_pspec,  
                                                   CORBA::Environment &env);
  // Used to restrict the set of protocols
   
  virtual void disconnect (const AVStreams::flowSpec &the_spec,  
                           CORBA::Environment &env);
  // disconnect the flows
   
  virtual void set_FPStatus (const AVStreams::flowSpec &the_spec, 
                             const char *fp_name, 
                             const CORBA::Any &fp_settings,  
                             CORBA::Environment &env);
  // Used to control the flow
   
  virtual CORBA::Object_ptr get_fep (const char *flow_name,  
                                     CORBA::Environment &env);
  // Not implemented in the light profile, throws notsupported

  virtual char * add_fep (CORBA::Object_ptr the_fep,  
                          CORBA::Environment &env);
  // Not implemented in the light profile, throws notsupported
   
  virtual void remove_fep (const char *fep_name,  
                           CORBA::Environment &env);
  // Not implemented in the light profile, throws notsupported
   
  virtual void set_negotiator (AVStreams::Negotiator_ptr new_negotiator,  
                               CORBA::Environment &env);
  // Used to "attach" a negotiator to the endpoint
   
  virtual void set_key (const char *flow_name, 
                        const AVStreams::encryption_key & the_key,
                        CORBA::Environment &env);
  // Used for public key encryption. 
    
  virtual void set_source_id (CORBA::Long source_id,  
                              CORBA::Environment &env);
  // Used to set a unique id for packets sent by this streamendpoint

  virtual ~TAO_StreamEndPoint (void);
  // Destructor
   
};

class TAO_ORBSVCS_Export TAO_Client_StreamEndPoint : 
  public virtual POA_AVStreams::StreamEndPoint_A, 
  public virtual TAO_StreamEndPoint,
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
                                  CORBA::Environment &env);
  // Called by StreamCtrl. responder is the peer to connect to

  virtual CORBA::Boolean multiconnect (AVStreams::streamQoS &the_qos, 
                                       AVStreams::flowSpec &the_spec,  
                                       CORBA::Environment &env);
  // Used for ATM-style multicast
  
  virtual CORBA::Boolean connect_leaf (AVStreams::StreamEndPoint_B_ptr the_ep, 
                                       AVStreams::streamQoS &the_qos, 
                                       const AVStreams::flowSpec &the_flows,  
                                       CORBA::Environment &env);
  // Used for ATM-style multicast

  virtual void disconnect_leaf (AVStreams::StreamEndPoint_B_ptr the_ep, 
                                const AVStreams::flowSpec &theSpec,  
                                CORBA::Environment &env);
  // Used to remove a multicast leaf

  virtual ~TAO_Client_StreamEndPoint (void);
  // Destructor

};

class TAO_ORBSVCS_Export TAO_Server_StreamEndPoint : 
  public virtual POA_AVStreams::StreamEndPoint_B, 
  public virtual TAO_StreamEndPoint,
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
                                  CORBA::Environment &env);

  virtual CORBA::Boolean request_connection (AVStreams::StreamEndPoint_ptr initiator, 
                                             CORBA::Boolean is_mcast, 
                                             AVStreams::streamQoS &qos, 
                                             AVStreams::flowSpec &the_spec,  
                                             CORBA::Environment &env);
  // Called by the peer StreamEndPoint. The flow_spec indicates the
  // flows (which contain transport addresses etc.)

  virtual CORBA::Boolean multiconnect (AVStreams::streamQoS &the_qos, 
                                       AVStreams::flowSpec &the_spec,  
                                       CORBA::Environment &env);
  // Used for internet-style multicast

  virtual ~TAO_Server_StreamEndPoint (void);
  // Destructor
};

class TAO_ORBSVCS_Export TAO_VDev 
  :public virtual TAO_PropertySet, 
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
                                   CORBA::Environment &env);
  // Called to tell the vdev who the streamctrl, peer vdev is

  virtual CORBA::Boolean set_Mcast_peer (AVStreams::StreamCtrl_ptr the_ctrl, 
                                         AVStreams::MCastConfigIf_ptr a_mcastconfigif, 
                                         AVStreams::streamQoS &the_qos, 
                                         const AVStreams::flowSpec &the_spec,  
                                         CORBA::Environment &env);
  // Used to set the streamctrl and multicast device
   
  virtual void configure (const CosPropertyService::Property &the_config_mesg,  
                          CORBA::Environment &env);
  // Called by the peer VDev to configure the device (catch all)
  
  virtual void set_format (const char *flowName, 
                           const char *format_name,  
                           CORBA::Environment &env);
  // Used to set a format on a flowname
   
  virtual void set_dev_params (const char *flowName, 
                               const CosPropertyService::Properties &new_params,  
                               CORBA::Environment &env);
  // Used to set device parameters
  
  virtual CORBA::Boolean modify_QoS (AVStreams::streamQoS &the_qos, 
                                     const AVStreams::flowSpec &the_spec,  
                                     CORBA::Environment &env);
  // Called to change QoS of the device

  virtual ~TAO_VDev (void);
  // Destructor

 protected:
  virtual CORBA::Boolean set_media_ctrl (CORBA::Object_ptr media_ctrl,
                                         CORBA::Environment &env);
  // hook called after set_peer is done to set the media ctrl of the peer vdev.
 private:
  AVStreams::StreamCtrl_var streamctrl_;
  // My stream controller
   
  AVStreams::VDev_var peer_;
  // My peer
};

class TAO_AV_Endpoint_Strategy;

class TAO_ORBSVCS_Export TAO_MMDevice 
  : public virtual TAO_PropertySet, 
    public virtual POA_AVStreams::MMDevice
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
                                           CORBA::Environment &env);
  // Can be used to request the MMDevice to create a new StreamCtrl,
  // and call bind_devs on it

  virtual AVStreams::StreamCtrl_ptr  bind_mcast (AVStreams::MMDevice_ptr first_peer,
                                                 AVStreams::streamQoS &the_qos,
                                                 CORBA::Boolean_out is_met,
                                                 const AVStreams::flowSpec &the_spec,
                                                 CORBA::Environment &env);
  // Multicast bind

  virtual AVStreams::StreamEndPoint_A_ptr  create_A (AVStreams::StreamCtrl_ptr the_requester, 
                                                     AVStreams::VDev_out the_vdev, 
                                                     AVStreams::streamQoS &the_qos, 
                                                     CORBA::Boolean_out met_qos, 
                                                     char *&named_vdev, 
                                                     const AVStreams::flowSpec &the_spec,  
                                                     CORBA::Environment &env);
  // Called by StreamCtrl to create a "A" type streamandpoint and vdev
  
  virtual AVStreams::StreamEndPoint_B_ptr  create_B (AVStreams::StreamCtrl_ptr the_requester, 
                                                     AVStreams::VDev_out the_vdev, 
                                                     AVStreams::streamQoS &the_qos, 
                                                     CORBA::Boolean_out met_qos, 
                                                     char *&named_vdev, 
                                                     const AVStreams::flowSpec &the_spec,  
                                                     CORBA::Environment &env);
  // Called by StreamCtrl to create a "B" type streamandpoint and vdev

  virtual void destroy (AVStreams::StreamEndPoint_ptr the_ep,
                        const char *vdev_name,
                        CORBA::Environment &env);
  // Remove the StreamEndPoint and the related vdev

  virtual char * add_fdev (CORBA::Object_ptr the_fdev,
                           CORBA::Environment &env);
  // Not supported in the light profile, raises notsupported

  virtual CORBA::Object_ptr get_fdev (const char *flow_name,  
                                      CORBA::Environment &env);
  // Not supported in the light profile, raises notsupported

  virtual void remove_fdev (const char *flow_name,  
                            CORBA::Environment &env);
  // Not supported in the light profile, raises notsupported
  
  virtual ~TAO_MMDevice (void);
  // Destructor
};
#endif /* AVSTREAMS_I_H */
