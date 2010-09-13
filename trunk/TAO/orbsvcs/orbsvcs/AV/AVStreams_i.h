// -*- C++ -*-

//=============================================================================
/**
 *  @file   AVStreams_i.h
 *
 *  $Id$
 *
 *  @author Sumedh Mungee <sumedh@cs.wustl.edu>
 *  @author Nagarajan Surendran <naga@cs.wustl.edu>
 */
//=============================================================================

#ifndef AVSTREAMS_I_H
#define AVSTREAMS_I_H
#include /**/ "ace/pre.h"

#include "orbsvcs/AV/AV_Core.h"

#include "orbsvcs/AV/AV_export.h"
#include "orbsvcs/CosPropertyS.h"
#include "orbsvcs/AVStreamsS.h"
#include "orbsvcs/Property/CosPropertyService_i.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/AV/AV_Core.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/Null_MediaCtrlS.h"
#include "orbsvcs/AV/FlowSpec_Entry.h"

#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/ATM_Addr.h"
#include "ace/Containers_T.h"
#include "ace/Process.h"
#include "ace/SOCK_CODgram.h"
#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Acceptor.h"

#define FLOWSPEC_MAX 5
// for the Hash_Map helper classes.

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef ACE_Hash_Map_Manager <ACE_CString,AVStreams::FlowEndPoint_var,ACE_Null_Mutex>  FlowEndPoint_Map;
typedef ACE_Hash_Map_Entry <ACE_CString,AVStreams::FlowEndPoint_var> FlowEndPoint_Map_Entry;
typedef ACE_Hash_Map_Iterator <ACE_CString,AVStreams::FlowEndPoint_var,ACE_Null_Mutex>  FlowEndPoint_Map_Iterator;

typedef ACE_Hash_Map_Manager <ACE_CString,TAO_AV_Flow_Handler*,ACE_Null_Mutex>  Flow_Handler_Map;
typedef ACE_Hash_Map_Entry <ACE_CString,TAO_AV_Flow_Handler*> Flow_Handler_Map_Entry;
typedef ACE_Hash_Map_Iterator <ACE_CString,TAO_AV_Flow_Handler*,ACE_Null_Mutex>  Flow_Handler_Map_Iterator;



class TAO_AV_Export AV_Null_MediaCtrl
  : public virtual POA_Null_MediaCtrl
{
public:
  AV_Null_MediaCtrl (void);

protected:

  virtual ~AV_Null_MediaCtrl (void);

};

/**
 * @class TAO_Basic_StreamCtrl
 * @brief Base class for StreamCtrl, implements basic stream start
 *        and stop functionality.
 */
class TAO_AV_Export TAO_Basic_StreamCtrl
  : public virtual POA_AVStreams::Basic_StreamCtrl,
    public virtual TAO_PropertySet
{

public:
  /// Default Constructor
  TAO_Basic_StreamCtrl (void);

  /// Stop the transfer of data of the stream
  /// Empty the_spec means apply operation to all flows
  virtual void stop (const AVStreams::flowSpec &the_spec);

  /// Start the transfer of data in the stream.
  /// Empty the_spec means apply operation to all flows
  virtual void start (const AVStreams::flowSpec &the_spec);

  /**
   * Tears down the stream. This will close the connection, and delete
   * the streamendpoint and vdev associated with this stream
   * Empty the_spec means apply operation to all flows
   */
  virtual void destroy (const AVStreams::flowSpec &the_spec);

  /// Changes the QoS associated with the stream
  /// Empty the_spec means apply operation to all flows
  virtual CORBA::Boolean modify_QoS (AVStreams::streamQoS &new_qos,
                                     const AVStreams::flowSpec &the_spec);

  /// Used by StreamEndPoint and VDev to inform StreamCtrl of events.
  /// E.g., loss of flow, reestablishment of flow, etc..
  virtual void push_event (const struct CosPropertyService::Property & the_event);

  /// Used to control the flow protocol parameters.
  virtual void set_FPStatus (const AVStreams::flowSpec &the_spec,
                             const char *fp_name,
                             const CORBA::Any &fp_settings);

  /// Not implemented in the light profile, will raise the notsupported
  /// exception
  virtual CORBA::Object_ptr get_flow_connection (const char *flow_name);

  /// Not implemented in the light profile, will raise the notsupported
  /// exception
  virtual void set_flow_connection (const char *flow_name,
                                    CORBA::Object_ptr flow_connection);

protected:

  /// Destructor.
  virtual ~TAO_Basic_StreamCtrl (void);


  /// The Virtual Devices for this stream
  AVStreams::VDev_var vdev_a_;
  AVStreams::VDev_var vdev_b_;

  /// The Endpoints for this stream
  AVStreams::StreamEndPoint_A_var sep_a_;
  AVStreams::StreamEndPoint_B_var sep_b_;

  /// Hash table for the flow names and its corresponding flowconnection object reference.
  typedef ACE_Hash_Map_Manager <ACE_CString,AVStreams::FlowConnection_var,ACE_Null_Mutex> FlowConnection_Map;
  typedef ACE_Hash_Map_Iterator <ACE_CString,AVStreams::FlowConnection_var,ACE_Null_Mutex> FlowConnection_Map_Iterator;
  typedef ACE_Hash_Map_Entry <ACE_CString,AVStreams::FlowConnection_var> FlowConnection_Map_Entry;
  FlowConnection_Map flow_connection_map_;
  AVStreams::FlowConnection_seq flowConnections_;

  ///sequence of flow names.
  u_int flow_count_;
  AVStreams::flowSpec flows_;
};

class TAO_AV_Export TAO_Negotiator
  : public POA_AVStreams::Negotiator
{
public:
  virtual CORBA::Boolean negotiate (AVStreams::Negotiator_ptr remote_negotiator,
                                    const AVStreams::streamQoS &qos_spec);
};

class TAO_MCastConfigIf;

class TAO_AV_Export MMDevice_Map_Hash_Key
{
public:
  /// default constructor.
  MMDevice_Map_Hash_Key (void);

  /// constructor.
  MMDevice_Map_Hash_Key (AVStreams::MMDevice_ptr mmdevice);

  /// copy constructor.
  MMDevice_Map_Hash_Key (const MMDevice_Map_Hash_Key&);

  /// destructor.
  ~MMDevice_Map_Hash_Key (void);

  /// operator== needed by  ACE_Hash_Map_Manager.
  bool operator == (const MMDevice_Map_Hash_Key &hash_key) const;

  /// operator== needed by  ACE_Hash_Map_Manager.
  friend bool operator < (const MMDevice_Map_Hash_Key &left,
                          const MMDevice_Map_Hash_Key &right);

  /// hash function for this mmdevice.
  u_long hash (void) const;

  static const int hash_maximum_;
protected:
  AVStreams::MMDevice_ptr mmdevice_;
};

/**
 * @class TAO_StreamCtrl
 * @brief Implementation the A/V StreamCtrl class. this class
 *        is used to control the stream. It should be subclassed
 *        by applications that want to provide more control features.
 */
class TAO_AV_Export TAO_StreamCtrl
  : public virtual POA_AVStreams::StreamCtrl,
    public virtual TAO_Basic_StreamCtrl
{

public:

  /// Default Constructor
  TAO_StreamCtrl (void);

  /// virtual destructor.
  virtual ~TAO_StreamCtrl (void);

  /// Stop the transfer of data of the stream
  /// Empty the_spec means apply operation to all flows
  virtual void stop (const AVStreams::flowSpec &the_spec);

  /// Start the transfer of data in the stream.
  /// Empty the_spec means apply operation to all flows
  virtual void start (const AVStreams::flowSpec &the_spec);

  /**
   * Tears down the stream. This will close the connection, and delete
   * the streamendpoint and vdev associated with this stream
   * Empty the_spec means apply operation to all flows
   */
  virtual void destroy (const AVStreams::flowSpec &the_spec);

  /**
   * Establish a stream between a_party and b_party,
   * with qos the_qos, and for the flows in the_flows
   * if the_flows is empty, bind all the flows
   * Causes a connection to be established between the StreamEndpoints.
   * Returns success/failure
   */
  virtual CORBA::Boolean bind_devs (AVStreams::MMDevice_ptr a_party,
                                    AVStreams::MMDevice_ptr b_party,
                                    AVStreams::streamQoS& the_qos,
                                    const AVStreams::flowSpec& the_flows);

  /**
   * Establish a connection between two streamendpoints. This can
   * be used if the streamendpoints have been created independent of
   * a MMDevice
   */
  virtual CORBA::Boolean bind (AVStreams::StreamEndPoint_A_ptr a_party,
                               AVStreams::StreamEndPoint_B_ptr b_party,
                               AVStreams::streamQoS &the_qos,
                               const AVStreams::flowSpec &the_flows);

  virtual void unbind_dev (AVStreams::MMDevice_ptr dev,
                           const AVStreams::flowSpec & the_spec);

  /// Unbind the_ep from the stream. Empty the_spec means apply to all flows.
  virtual void unbind_party (AVStreams::StreamEndPoint_ptr the_ep,
                             const AVStreams::flowSpec &the_spec);

  /// unbind the stream. Same effect as Basic_StreamCtrl::destroy ()
  virtual void unbind (void);

  virtual AVStreams::VDev_ptr get_related_vdev (AVStreams::MMDevice_ptr adev,
                                                AVStreams::StreamEndPoint_out sep);

  /// Changes the QoS associated with the stream
  /// Empty the_spec means apply operation to all flows
  virtual CORBA::Boolean modify_QoS (AVStreams::streamQoS &new_qos,
                                     const AVStreams::flowSpec &the_spec);

protected:

  struct MMDevice_Map_Entry
  {
    AVStreams::StreamEndPoint_var sep_;
    AVStreams::VDev_var vdev_;
    AVStreams::flowSpec flowspec_;
    AVStreams::streamQoS qos_;
  };

  typedef ACE_Hash_Map_Manager <MMDevice_Map_Hash_Key,MMDevice_Map_Entry,ACE_Null_Mutex> MMDevice_Map;
  typedef ACE_Hash_Map_Iterator <MMDevice_Map_Hash_Key,MMDevice_Map_Entry,ACE_Null_Mutex> MMDevice_Map_Iterator;

  MMDevice_Map mmdevice_a_map_;
  MMDevice_Map mmdevice_b_map_;
  TAO_MCastConfigIf *mcastconfigif_;
  AVStreams::MCastConfigIf_var mcastconfigif_ptr_;
  AVStreams::StreamCtrl_var streamctrl_;
  CORBA::ULong source_id_;
};

class TAO_AV_Export TAO_MCastConfigIf
  : public virtual POA_AVStreams::MCastConfigIf,
    public virtual TAO_PropertySet
{
public:

  enum Peer_Interface {VDEV = 0, FLOWENDPOINT=1};
  struct Peer_Info
  {
    AVStreams::VDev_var peer_;
    AVStreams::FlowEndPoint_var fep_;
    AVStreams::streamQoS qos_;
    AVStreams::flowSpec flow_spec_;
    Peer_Interface interface_;
  };

  /// Default constructor.
  TAO_MCastConfigIf (void);

  /// Dtor
  ~TAO_MCastConfigIf (void);

  virtual CORBA::Boolean set_peer (CORBA::Object_ptr peer,
                                   AVStreams::streamQoS & the_qos,
                                   const AVStreams::flowSpec & the_spec);

  virtual void configure (const CosPropertyService::Property & a_configuration);

  virtual void set_initial_configuration (const CosPropertyService::Properties & initial);

  virtual void set_format (const char * flowName,
                           const char * format_name);

  virtual void set_dev_params (const char * flowName,
                               const CosPropertyService::Properties & new_params)
;

protected:
  /// checks whether the flowname is  in the flow_spec.
  int in_flowSpec (const AVStreams::flowSpec& flow_spec, const char *flow_name);

  /// Multicast socket.
  ACE_SOCK_Dgram_Mcast sock_mcast_;

  /// Initial configuration to be distributed to all B parties when they join.
  CosPropertyService::Properties initial_configuration_;

  ACE_DLList<Peer_Info> peer_list_;
  ACE_DLList_Iterator<Peer_Info> peer_list_iterator_;
};

/**
 * @class TAO_AV_QoS
 * @brief Class for getting and setting the QoS characteristics of
 *        an AV Stream.
 */
class TAO_AV_Export TAO_AV_QoS
{
public:
  /// constructor.
  TAO_AV_QoS (void);

  /// constructor taking a stream qos parameter.
  TAO_AV_QoS (AVStreams::streamQoS &stream_qos);

  /// sets the maps with the QoS parameter.
  int set (AVStreams::streamQoS &stream_qos);

  /// gets the flow_qos.
  int get_flow_qos (const char *flowname,AVStreams::QoS &flow_qos);

  /// converts the application level QoS to Network-level QoS.
  int convert (AVStreams::streamQoS &network_qos);


protected:
  /// Stream Qos.
  AVStreams::streamQoS stream_qos_;

  ACE_Hash_Map_Manager<ACE_CString,AVStreams::QoS,ACE_Null_Mutex> qos_map_;
};


// Forward declarations.
class TAO_AV_TCP_Flow_Handler;
class TAO_AV_UDP_Flow_Handler;
class TAO_AV_UDP_MCast_Flow_Handler;
class TAO_AV_Protocol_Object;
class TAO_AV_Callback;
class TAO_AV_SourceManager;
class TAO_AV_Source;
class TAO_AV_RTP_State;
class TAO_AV_Flow_Handler;

/**
 * @class TAO_Base_StreamEndPoint
 *
 * Base class for the A/V StreamEndPoint class. this class
 * is used to control the stream. It should be subclassed
 * by applications that want to provide more control features.
 */
class TAO_AV_Export TAO_Base_StreamEndPoint
  :public virtual TAO_PropertySet
{

  // @@Naga: Rename this class to TAO_Base_EndPoint since both stream and flowendpoints derive from it.
public:
  TAO_Base_StreamEndPoint (void);
  virtual ~TAO_Base_StreamEndPoint (void);

  /// called when streamendpoint is instantiated
  virtual int handle_open (void);

  /// called when streamendpoint is being destructed
  virtual int handle_close (void);

  /// Application needs to define this
  virtual int handle_stop (const AVStreams::flowSpec &the_spec);

  /// Application needs to define this
  virtual int handle_start (const AVStreams::flowSpec &the_spec);

  /// Application needs to define this
  virtual int handle_destroy (const AVStreams::flowSpec &the_spec);

  /// Application needs to define this
  virtual CORBA::Boolean handle_preconnect (AVStreams::flowSpec &the_spec);

  /// Application needs to define this
  virtual CORBA::Boolean handle_postconnect (AVStreams::flowSpec &the_spec);

  /// Application needs to define this
  virtual CORBA::Boolean handle_connection_requested (AVStreams::flowSpec &the_spec);

  virtual int get_callback (const char *flowname,
                            TAO_AV_Callback *&callback);

  virtual int get_control_callback (const char *flowname,
                                    TAO_AV_Callback *&callback);

  virtual int set_protocol_object (const char *flowname,
                                   TAO_AV_Protocol_Object *object);

  virtual void set_flow_handler (const char *flowname,
                                 TAO_AV_Flow_Handler *handler);

  virtual void set_control_flow_handler (const char *flowname,
                                         TAO_AV_Flow_Handler *handler);

  TAO_AV_QoS &qos (void);

  void protocol_object_set (void);
  int is_protocol_object_set (void);

protected:

  TAO_AV_QoS qos_;
  Flow_Handler_Map flow_handler_map_;
  Flow_Handler_Map control_flow_handler_map_;
  int protocol_object_set_;
};

// Forward declarations.
class TAO_AV_Acceptor;
class TAO_AV_Connector;
class TAO_Forward_FlowSpec_Entry;
class TAO_Reverse_FlowSpec_Entry;

/**
 * @class TAO_StreamEndPoint
 * @brief The Stream EndPoint. Used to implement one endpoint of a stream
 *        that implements the transport layer.
 */
class TAO_AV_Export TAO_StreamEndPoint
  : public virtual POA_AVStreams::StreamEndPoint,
    public virtual TAO_Base_StreamEndPoint
{

public:
  /// Constructor
  TAO_StreamEndPoint (void);

   /// Stop the stream. Empty the_spec means, for all the flows
  virtual void stop (const AVStreams::flowSpec &the_spec);

  /// Start the stream, Empty the_spec means, for all the flows
  virtual void start (const AVStreams::flowSpec &the_spec);

  /// Destroy the stream, Empty the_spec means, for all the flows
  virtual void destroy (const AVStreams::flowSpec &the_spec);

  /// Called by StreamCtrl. responder is the peer to connect to
  virtual CORBA::Boolean connect (AVStreams::StreamEndPoint_ptr responder,
                                  AVStreams::streamQoS& qos_spec,
                                  const AVStreams::flowSpec& the_spec);

  /// Called by the peer StreamEndPoint. The flow_spec indicates the
  /// flows (which contain transport addresses etc.)
  virtual CORBA::Boolean request_connection (AVStreams::StreamEndPoint_ptr initiator,
                                             CORBA::Boolean is_mcast,
                                             AVStreams::streamQoS &qos,
                                             AVStreams::flowSpec &the_spec);

  /// Change the transport qos on a stream
  virtual CORBA::Boolean modify_QoS (AVStreams::streamQoS &new_qos,
                                     const AVStreams::flowSpec &the_flows);

  virtual int change_qos (AVStreams::streamQoS &new_qos,
                          const AVStreams::flowSpec &the_flows);

  /// Used to restrict the set of protocols
  virtual CORBA::Boolean set_protocol_restriction (const AVStreams::protocolSpec &the_pspec);

  /// disconnect the flows
  virtual void disconnect (const AVStreams::flowSpec &the_spec);

  /// Used to control the flow
  virtual void set_FPStatus (const AVStreams::flowSpec &the_spec,
                             const char *fp_name,
                             const CORBA::Any &fp_settings);

  /// Not implemented in the light profile, throws notsupported
  virtual CORBA::Object_ptr get_fep (const char *flow_name);

  /// Not implemented in the light profile, throws notsupported
  virtual char * add_fep (CORBA::Object_ptr the_fep);

  /// Not implemented in the light profile, throws notsupported
  virtual void remove_fep (const char *fep_name);

  /// Used to "attach" a negotiator to the endpoint
  virtual void set_negotiator (AVStreams::Negotiator_ptr new_negotiator);

  /// Used for public key encryption.
  virtual void set_key (const char *flow_name,
                        const AVStreams::key & the_key);

  /// Used to set a unique id for packets sent by this streamendpoint
  virtual void set_source_id (CORBA::Long source_id);

  /// Destructor
  virtual ~TAO_StreamEndPoint (void);

  CORBA::Boolean multiconnect (AVStreams::streamQoS &the_qos,
                               AVStreams::flowSpec &the_spec);

protected:
  /// Helper methods to implement add_fep()
  char* add_fep_i (AVStreams::FlowEndPoint_ptr fep);
  char* add_fep_i_add_property (AVStreams::FlowEndPoint_ptr fep);

  /// translate from application level to network level qos.
  int translate_qos (const AVStreams::streamQoS& application_qos,
                     AVStreams::streamQoS& network_qos);

  /// Count of the number of flows in this streamendpoint, used to
  /// generate unique names for the flows.
  u_int flow_count_;

  /// current flow number used for system generation of flow names.
  u_int flow_num_;

  /// hash table for the flownames and its corresponding flowEndpoint reference.
  FlowEndPoint_Map fep_map_;

  /// sequence of supported flow names.
  AVStreams::flowSpec flows_;

  /// source id used for multicast.
  CORBA::Long source_id_;

  /// our local negotiator for QoS.
  AVStreams::Negotiator_var negotiator_;

  /// Our available list of protocols.
  AVStreams::protocolSpec protocols_;

  /// Chosen protocol for this streamendpoint based on availableprotocols property.
  CORBA::String_var protocol_;

  /// Key used for encryption.
  AVStreams::key key_;

///   TAO_Forward_FlowSpec_Entry forward_entries_ [FLOWSPEC_MAX];
///   TAO_Reverse_FlowSpec_Entry reverse_entries_ [FLOWSPEC_MAX];
  u_short mcast_port_;
  ACE_CString mcast_addr_;
  ACE_Hash_Map_Manager <ACE_CString, TAO_FlowSpec_Entry*,ACE_Null_Mutex> mcast_entry_map_;
  TAO_AV_FlowSpecSet forward_flow_spec_set;
  TAO_AV_FlowSpecSet reverse_flow_spec_set;
  AVStreams::StreamEndPoint_var peer_sep_;
  AVStreams::SFPStatus *sfp_status_;
  AVStreams::StreamCtrl_var streamctrl_;
};

/**
 * @class TAO_StreamEndPoint_A
 * @brief  The "A" side of a streamendpoint
 */
class TAO_AV_Export TAO_StreamEndPoint_A :
  public virtual POA_AVStreams::StreamEndPoint_A,
  public virtual TAO_StreamEndPoint
{

public:
  /// Constructor
  TAO_StreamEndPoint_A (void);

  /// Used for ATM-style multicast
  virtual CORBA::Boolean multiconnect (AVStreams::streamQoS &the_qos,
                                       AVStreams::flowSpec &the_spec);

  /// Used for ATM-style multicast
  virtual CORBA::Boolean connect_leaf (AVStreams::StreamEndPoint_B_ptr the_ep,
                                       AVStreams::streamQoS &the_qos,
                                       const AVStreams::flowSpec &the_flows);

  /// Used to remove a multicast leaf
  virtual void disconnect_leaf (AVStreams::StreamEndPoint_B_ptr the_ep,
                                const AVStreams::flowSpec &theSpec);

  /// Destructor
  virtual ~TAO_StreamEndPoint_A (void);

};

// For backward compatibility.
#define TAO_Client_StreamEndPoint TAO_StreamEndPoint_A
#define TAO_Server_StreamEndPoint TAO_StreamEndPoint_B

/**
 * @class TAO_StreamEndPoint_B
 * @brief The "B" side of a streamendpoint
 */
class TAO_AV_Export TAO_StreamEndPoint_B :
  public virtual POA_AVStreams::StreamEndPoint_B,
  public virtual TAO_StreamEndPoint
{
  // = DESCRIPTION
  //     The "B" side of a streamendpoint
public:
  /// Constructor
  TAO_StreamEndPoint_B (void);

  /// Used for internet-style multicast
  virtual CORBA::Boolean multiconnect (AVStreams::streamQoS &the_qos,
                                       AVStreams::flowSpec &the_spec);

  /// Destructor
  virtual ~TAO_StreamEndPoint_B (void);
};

/**
 * @class TAO_VDev
 * @brief Implements the VDev interface. One of these is created per
 *        connection, and represents device-specific parameters.
 */
class TAO_AV_Export TAO_VDev
  :public virtual TAO_PropertySet,
   public virtual POA_AVStreams::VDev
{

public:
  /// Default Constructor
  TAO_VDev (void);

  /// Called to tell the vdev who the streamctrl, peer vdev is
  virtual CORBA::Boolean set_peer (AVStreams::StreamCtrl_ptr the_ctrl,
                                   AVStreams::VDev_ptr the_peer_dev,
                                   AVStreams::streamQoS &the_qos,
                                   const AVStreams::flowSpec &the_spec);

  /// Used to set the streamctrl and multicast device
  virtual CORBA::Boolean set_Mcast_peer (AVStreams::StreamCtrl_ptr the_ctrl,
                                         AVStreams::MCastConfigIf_ptr a_mcastconfigif,
                                         AVStreams::streamQoS &the_qos,
                                         const AVStreams::flowSpec &the_spec);

  /// Called by the peer VDev to configure the device (catch all)
  virtual void configure (const CosPropertyService::Property &the_config_mesg);

  /// Used to set a format on a flowname
  virtual void set_format (const char *flowName,
                           const char *format_name);

  /// Used to set device parameters
  virtual void set_dev_params (const char *flowName,
                               const CosPropertyService::Properties &new_params);

  /// Called to change QoS of the device
  virtual CORBA::Boolean modify_QoS (AVStreams::streamQoS &the_qos,
                                     const AVStreams::flowSpec &the_spec);

protected:
  /// Destructor for a servant should be protected or private.
  /// Use _remove_ref() to delete this servant.
  virtual ~TAO_VDev (void);

  /// hook called after set_peer is done to set the media ctrl of the peer vdev.
  virtual CORBA::Boolean set_media_ctrl (CORBA::Object_ptr media_ctrl);

  /// My stream controller
  AVStreams::StreamCtrl_var streamctrl_;

  /// My peer
  AVStreams::VDev_var peer_;

  /// The multicast VDev peer.
  AVStreams::MCastConfigIf_var mcast_peer_;
};

class TAO_AV_Endpoint_Strategy;

/**
 * @class TAO_MMDevice
 * @brief Implements a factory to create Endpoints and VDevs
 */
class TAO_AV_Export TAO_MMDevice
  :public virtual POA_AVStreams::MMDevice,
   public virtual TAO_PropertySet
{

public:

  /// Constructor
  enum MMDevice_Type {MMDEVICE_A = 0,MMDEVICE_B = 1};
  TAO_MMDevice (TAO_AV_Endpoint_Strategy *endpoint_strategy_);

  virtual AVStreams::StreamEndPoint_ptr create_A_B (MMDevice_Type type,
                                                    AVStreams::StreamCtrl_ptr the_requester,
                                                     AVStreams::VDev_out the_vdev,
                                                     AVStreams::streamQoS &the_qos,
                                                     CORBA::Boolean_out met_qos,
                                                     char *&named_vdev,
                                                     const AVStreams::flowSpec &the_spec);

  /// Can be used to request the MMDevice to create a new StreamCtrl,
  /// and call bind_devs on it
  virtual AVStreams::StreamCtrl_ptr  bind (AVStreams::MMDevice_ptr peer_device,
                                           AVStreams::streamQoS &the_qos,
                                           CORBA::Boolean_out is_met,
                                           const AVStreams::flowSpec &the_spec);

  /// Multicast bind
  virtual AVStreams::StreamCtrl_ptr  bind_mcast (AVStreams::MMDevice_ptr first_peer,
                                                 AVStreams::streamQoS &the_qos,
                                                 CORBA::Boolean_out is_met,
                                                 const AVStreams::flowSpec &the_spec);

  /// Called by StreamCtrl to create a "A" type streamandpoint and vdev
  virtual AVStreams::StreamEndPoint_A_ptr  create_A (AVStreams::StreamCtrl_ptr the_requester,
                                                     AVStreams::VDev_out the_vdev,
                                                     AVStreams::streamQoS &the_qos,
                                                     CORBA::Boolean_out met_qos,
                                                     char *&named_vdev,
                                                     const AVStreams::flowSpec &the_spec);

  /// Called by StreamCtrl to create a "B" type streamandpoint and vdev
  virtual AVStreams::StreamEndPoint_B_ptr  create_B (AVStreams::StreamCtrl_ptr the_requester,
                                                     AVStreams::VDev_out the_vdev,
                                                     AVStreams::streamQoS &the_qos,
                                                     CORBA::Boolean_out met_qos,
                                                     char *&named_vdev,
                                                     const AVStreams::flowSpec &the_spec);

  /// Remove the StreamEndPoint and the related vdev
  virtual void destroy (AVStreams::StreamEndPoint_ptr the_ep,
                        const char *vdev_name);

  /// Not supported in the light profile, raises notsupported
  virtual char * add_fdev (CORBA::Object_ptr the_fdev);

  /// Not supported in the light profile, raises notsupported
  virtual CORBA::Object_ptr get_fdev (const char *flow_name);

  /// Not supported in the light profile, raises notsupported
  virtual void remove_fdev (const char *flow_name);

  /// Destructor
  virtual ~TAO_MMDevice (void);

protected:
  /// Helper method to implement add_fdev()
  char* add_fdev_i (AVStreams::FDev_ptr fdev);

protected:

  TAO_AV_Endpoint_Strategy *endpoint_strategy_;

  /// Count of the number of flows in this MMDevice , used to
  /// generate unique names for the flows.
  u_int flow_count_;

  /// current flow number used for system generation of flow names.
  u_int flow_num_;

  typedef ACE_Hash_Map_Manager <ACE_CString,AVStreams::FDev_var,ACE_Null_Mutex> FDev_Map;
  typedef ACE_Hash_Map_Iterator <ACE_CString,AVStreams::FDev_var,ACE_Null_Mutex> FDev_Map_Iterator;
  typedef ACE_Hash_Map_Entry <ACE_CString,AVStreams::FDev_var> FDev_Map_Entry;

  /// hash table for the flownames and its corresponding flowEndpoint
  /// reference.
  FDev_Map fdev_map_;

  /// sequence of supported flow names.
  AVStreams::flowSpec flows_;

  TAO_StreamCtrl *stream_ctrl_;
};

class TAO_FlowConsumer;
class TAO_FlowProducer;

/**
 * @class TAO_FlowConnection
 * @brief This class currently supports only one producer and one
 *        consumer per flow.
 */
class TAO_AV_Export TAO_FlowConnection
 : public virtual POA_AVStreams::FlowConnection,
   public virtual TAO_PropertySet
{

public:
  /// default constructor.
  TAO_FlowConnection (void);

  /// stop this flow.
  virtual void stop (void);

  /// start this flow.
  virtual void start (void);

  /// destroy this flow.
  virtual void destroy (void);

  /// modify the QoS for this flow.
  virtual CORBA::Boolean modify_QoS (AVStreams::QoS & new_qos);

  /// use the specified flow protocol for this flow.
  virtual CORBA::Boolean use_flow_protocol (const char * fp_name,
                                            const CORBA::Any & fp_settings);

  /// pushes an event , to be handled by the application.
  virtual void push_event (const AVStreams::streamEvent & the_event);


  /// connect 2 Flow Devices.
  virtual CORBA::Boolean connect_devs (AVStreams::FDev_ptr a_party,
                                       AVStreams::FDev_ptr b_party,
                                       AVStreams::QoS & the_qos);

  /// Connect a flow producer and consumer under this flow connection.
  virtual CORBA::Boolean connect (AVStreams::FlowProducer_ptr flow_producer,
                                  AVStreams::FlowConsumer_ptr flow_consumer,
                                  AVStreams::QoS & the_qos);

  /// disconnect this flow connection.
  virtual CORBA::Boolean disconnect (void);

  /// adds the producer to this flow connection.
  virtual CORBA::Boolean add_producer (AVStreams::FlowProducer_ptr flow_producer,
                                       AVStreams::QoS & the_qos);

  /// adds a consumer to this flow connection.
  virtual CORBA::Boolean add_consumer (AVStreams::FlowConsumer_ptr flow_consumer,
                                       AVStreams::QoS & the_qos);

  /// drops a flow endpoint from the flow.
  virtual CORBA::Boolean drop (AVStreams::FlowEndPoint_ptr target);

  int set_mcast_addr (ACE_CString addr,u_short port);
  void set_protocol (const char *protocol);
protected:
  typedef ACE_Unbounded_Set<AVStreams::FlowProducer_ptr> FlowProducer_Set;
  typedef ACE_Unbounded_Set_Iterator<AVStreams::FlowProducer_ptr> FlowProducer_SetItor;
  typedef ACE_Unbounded_Set<AVStreams::FlowConsumer_ptr> FlowConsumer_Set;
  typedef ACE_Unbounded_Set_Iterator<AVStreams::FlowConsumer_ptr> FlowConsumer_SetItor;

  /// The multicast address returned by the producer.
  FlowProducer_Set flow_producer_set_;
  FlowConsumer_Set flow_consumer_set_;
  CORBA::String_var fp_name_;
  CORBA::Any fp_settings_;
  CORBA::String_var producer_address_;

  /// IP Multicasting is used.
  int ip_multicast_;
  TAO_MCastConfigIf *mcastconfigif_i_;
  AVStreams::MCastConfigIf_var mcastconfigif_;
  u_short mcast_port_;
  ACE_CString mcast_addr_;
  CORBA::String_var protocol_;
};

/**
 * @class TAO_FlowEndPoint
 * @brief This class is used per flow e.g video flow and an audio flow
 *        to encapsulate the transport details.
 */
class TAO_AV_Export TAO_FlowEndPoint :
  public virtual POA_AVStreams::FlowEndPoint,
  public virtual TAO_Base_StreamEndPoint
{

public:

  ///default constructor.
  TAO_FlowEndPoint (void);

  TAO_FlowEndPoint (const char *flowname,
                    AVStreams::protocolSpec &protocols,
                    const char *format);

  int open (const char *flowname,
            AVStreams::protocolSpec &protocols,
            const char *format);

  int set_flowname (const char *flowname);

  virtual void set_flow_handler (const char *flowname,
                                 TAO_AV_Flow_Handler *handler);

  virtual int set_protocol_object (const char *flowname,
                                   TAO_AV_Protocol_Object *object);

  /// lock the flow endpoint for a particular flow.
  virtual CORBA::Boolean lock (void);

  /// unlock the flow endpoint for subsequent use.
  virtual void unlock (void);


  /// destroy this flow.
  virtual void destroy (void);

  /// get method for the related streamendpoint under which this
  /// flowendpoint is.
  virtual AVStreams::StreamEndPoint_ptr related_sep(void);

  /// set method for the related streamendpoint under which this
  /// flowendpoint is.
  virtual void related_sep (AVStreams::StreamEndPoint_ptr related_sep);

  virtual AVStreams::FlowConnection_ptr related_flow_connection(void);

  // accessor for the related flow connection attribute.

  /// set method for the related flow connection attribute.
  virtual void related_flow_connection (AVStreams::FlowConnection_ptr related_flow_connection);

  /// returns the other flowendpoint to which this is connected.
  virtual AVStreams::FlowEndPoint_ptr get_connected_fep (void);

  //// use the specified flow protocol.
  virtual CORBA::Boolean use_flow_protocol (const char * fp_name,
                                            const CORBA::Any & fp_settings);

  /// sets the data format.
  virtual void set_format (const char * format);

  /// sets the device parameters.
  virtual void set_dev_params (const CosPropertyService::Properties & new_settings);

  /// sets the list of protocols to be used.
  virtual void set_protocol_restriction (const AVStreams::protocolSpec & the_spec);

  /// checks whether the passed flowendpoint is compatible with this.
  virtual CORBA::Boolean is_fep_compatible (AVStreams::FlowEndPoint_ptr fep);

  /// sets the peer flowendpoint.
  virtual CORBA::Boolean set_peer (AVStreams::FlowConnection_ptr the_fc,
                                   AVStreams::FlowEndPoint_ptr the_peer_fep,
                                   AVStreams::QoS & the_qos);

  /// sets the multicast peer flowendpoint, not implemented.
  virtual CORBA::Boolean set_Mcast_peer (AVStreams::FlowConnection_ptr the_fc,
                                         AVStreams::MCastConfigIf_ptr a_mcastconfigif,
                                         AVStreams::QoS & the_qos);


  /**
   * This should be implemented in both the FlowProducer and consumer and hence is
   * pure virtual since we need to know the role of the flowendpoint to create appropriate
   * protocol objects. eg. in SFP to create Producer Object/ Consumer Object.
   */
  virtual CORBA::Boolean connect_to_peer (AVStreams::QoS & the_qos,
                                          const char * address,
                                          const char * use_flow_protocol)
    = 0;

  /// connect to the peer endpoint.
  virtual CORBA::Boolean connect_to_peer_i (TAO_FlowSpec_Entry::Role role,
                                            AVStreams::QoS & the_qos,
                                            const char * address,
                                            const char * use_flow_protocol);

  /**
   * This should be implemented in both the FlowProducer and consumer and hence is
   * pure virtual since we need to know the role of the flowendpoint to create appropriate
   * protocol objects. eg. in SFP to create Producer Object/ Consumer Object.
   */
  virtual char * go_to_listen (AVStreams::QoS & the_qos,
                               CORBA::Boolean is_mcast,
                               AVStreams::FlowEndPoint_ptr peer,
                               char *& flowProtocol)
    = 0;

  /// listen request from the peer.
  virtual char * go_to_listen_i (TAO_FlowSpec_Entry::Role role,
                                 AVStreams::QoS & the_qos,
                                 CORBA::Boolean is_mcast,
                                 AVStreams::FlowEndPoint_ptr peer,
                                 char *& flowProtocol);


protected:
  /// The related streamendpoint.
  AVStreams::StreamEndPoint_var related_sep_;

  /// The related flow connection reference
  AVStreams::FlowConnection_var related_flow_connection_;

  /// The peer flowendpoint reference.
  AVStreams::FlowEndPoint_var peer_fep_;

  /// Available protocols for this flowendpoint.
  AVStreams::protocolSpec protocols_;

  /// Address information for the protocols.
  AVStreams::protocolSpec protocol_addresses_;

  /// The multicast peer endpoint.
  AVStreams::MCastConfigIf_var mcast_peer_;

  /// Lock.
  CORBA::Boolean lock_;

  CORBA::String_var format_;
  CORBA::String_var flowname_;
  CosPropertyService::Properties dev_params_;
  TAO_AV_FlowSpecSet flow_spec_set_;
  CORBA::String_var reverse_channel_;
};

class TAO_AV_Export TAO_FlowProducer:
  public virtual POA_AVStreams::FlowProducer,
  public virtual TAO_FlowEndPoint
{
public:
  /// default constructor
  TAO_FlowProducer (void);

  TAO_FlowProducer (const char *flowname,
                    AVStreams::protocolSpec protocols,
                    const char *format);

  /**
   * get the reverse channel, to be used for feedback for protocols like UDP.
   * @@Naga: In the spec this is defined in the TAO_FlowProducer but it seems more reasonable for this
   * to be in a FlowEndPoint since any of the flowendpoints can be made to listen. So in the case of
   * UDP if the producer is listening and the consumer connects (logically) then the producer needs to
   * know the reverse channel on its peer fep to send data to.
   */
  virtual char * get_rev_channel (const char * pcol_name);

  /// stop this flow, to be overridden by the application.
  virtual void stop (void);

  /// start this flow, to be overridden by the application.
  virtual void start (void);

  virtual char * go_to_listen (AVStreams::QoS & the_qos,
                               CORBA::Boolean is_mcast,
                               AVStreams::FlowEndPoint_ptr peer,
                               char *& flowProtocol);

  virtual CORBA::Boolean connect_to_peer (AVStreams::QoS & the_qos,
                                          const char * address,
                                          const char * use_flow_protocol);

  /// connect to the multicast address, not implemented.
  virtual char * connect_mcast (AVStreams::QoS & the_qos,
                                CORBA::Boolean_out is_met,
                                const char * address,
                                const char * use_flow_protocol);



  /// sets the public key  to be used for encryption of the data.
  virtual void set_key (const AVStreams::key & the_key);

  /// sets the source id of this flow producer so that it can be used
  /// to distinguish this producer from others in the multicast case.
  virtual void set_source_id (CORBA::Long source_id);

protected:
  /// source id of this producer.
  CORBA::Long source_id_;
  CORBA::String_var peer_address_;
};

class TAO_AV_Export TAO_FlowConsumer :
  public virtual POA_AVStreams::FlowConsumer,
  public virtual TAO_FlowEndPoint
{
public:
  /// default constructor.
  TAO_FlowConsumer (void);

  TAO_FlowConsumer (const char *flowname,
                    AVStreams::protocolSpec protocols,
                    const char *format);

  /// stop this flow, to be overridden by the application.
  virtual void stop (void);

  /// start this flow, to be overridden by the application.
  virtual void start (void);

  virtual char * go_to_listen (AVStreams::QoS & the_qos,
                               CORBA::Boolean is_mcast,
                               AVStreams::FlowEndPoint_ptr peer,
                               char *& flowProtocol);

  virtual CORBA::Boolean connect_to_peer (AVStreams::QoS & the_qos,
                                          const char * address,
                                          const char * use_flow_protocol);
};

/**
 * @class TAO_MediaControl
 * @brief Abstract Mediacontrol class.
 *        The following are to be handled by the specialized media control
 *        for the specific media like camera, speaker.
 */
class TAO_AV_Export TAO_MediaControl
  :public virtual POA_AVStreams::MediaControl
{

public:
  /// default constructor
  TAO_MediaControl (void);

  virtual AVStreams::Position get_media_position (AVStreams::PositionOrigin an_origin,
                                                  AVStreams::PositionKey a_key)
    =0;

  virtual void set_media_position (const AVStreams::Position & a_position)
    =0;

  virtual void start (const AVStreams::Position & a_position)
    =0;

  virtual void pause (const AVStreams::Position & a_position)
    =0;

  virtual void resume (const AVStreams::Position & a_position)
    =0;

  virtual void stop (const AVStreams::Position & a_position)
    =0;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include "orbsvcs/AV/Transport.h"

#if defined (__ACE_INLINE__)
#include "tao/debug.h"
#include "orbsvcs/AV/AVStreams_i.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/AV/Flows_T.h"

#include /**/ "ace/post.h"
#endif /* AVSTREAMS_I_H */
