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

#include "orbsvcs/AVStreamsS.h"

class TAO_Basic_StreamCtrl : public POA_AVStreams::Basic_StreamCtrl
{
public:
  TAO_Basic_StreamCtrl (void);

  virtual void stop (const AVStreams::flowSpec &the_spec,  
                     CORBA::Environment &env);

  virtual void start (const AVStreams::flowSpec &the_spec,  
                      CORBA::Environment &env);

  virtual void destroy (const AVStreams::flowSpec &the_spec,  
                        CORBA::Environment &env);

  virtual CORBA::Boolean modify_QoS (AVStreams::streamQoS &new_qos, 
                                     const AVStreams::flowSpec &the_spec,  
                                     CORBA::Environment &env);

  virtual void push_event (const char *the_event,  
                           CORBA::Environment &env);

  virtual void set_FPStatus (const AVStreams::flowSpec &the_spec, 
                             const char *fp_name, 
                             const CORBA::Any &fp_settings,  
                             CORBA::Environment &env);

  virtual CORBA::Object_ptr get_flow_connection (const char *flow_name,  
                                                 CORBA::Environment &env);

  virtual void set_flow_connection (const char *flow_name, 
                                    CORBA::Object_ptr flow_connection,  
                                    CORBA::Environment &env);

  virtual ~TAO_Basic_StreamCtrl (void);
};

class TAO_StreamCtrl : public POA_AVStreams::StreamCtrl
{
public:
  TAO_StreamCtrl (void);

  virtual CORBA::Boolean bind_devs (AVStreams::MMDevice_ptr a_party, 
                                    AVStreams::MMDevice_ptr b_party, 
                                    AVStreams::streamQoS &the_qos, 
                                    const AVStreams::flowSpec &the_flows,  
                                    CORBA::Environment &env);
  
  virtual CORBA::Boolean bind (AVStreams::StreamEndPoint_A_ptr a_party, 
                               AVStreams::StreamEndPoint_B_ptr b_party, 
                               AVStreams::streamQoS &the_qos, 
                               const AVStreams::flowSpec &the_flows,  
                               CORBA::Environment &env);
  
  virtual void unbind_party (AVStreams::StreamEndPoint_ptr the_ep, 
                             const AVStreams::flowSpec &the_spec,  
                             CORBA::Environment &env);

  virtual ~TAO_StreamCtrl (void);

};  

class TAO_StreamEndPoint : public POA_AVStreams::StreamEndPoint
{
public:
  TAO_StreamEndPoint (void);

  virtual void stop (const AVStreams::flowSpec &the_spec,  
                     CORBA::Environment &env);

  virtual void start (const AVStreams::flowSpec &the_spec,  
                      CORBA::Environment &env);

  virtual void destroy (const AVStreams::flowSpec &the_spec,  
                        CORBA::Environment &env);

  virtual CORBA::Boolean connect (AVStreams::StreamEndPoint_ptr responder, 
                                  AVStreams::streamQoS &qos_spec, 
                                  const AVStreams::flowSpec &the_spec,  
                                  CORBA::Environment &env);
    
  virtual CORBA::Boolean request_connection (AVStreams::StreamEndPoint_ptr initiator, 
                                             CORBA::Boolean is_mcast, 
                                             AVStreams::streamQoS &qos, 
                                             AVStreams::flowSpec &the_spec,  
                                             CORBA::Environment &env);
    
  virtual CORBA::Boolean modify_QoS (AVStreams::streamQoS &new_qos, 
                                     const AVStreams::flowSpec &the_flows,  
                                     CORBA::Environment &env);
    
  virtual CORBA::Boolean set_protocol_restriction (const AVStreams::protocolSpec &the_pspec,  
                                                   CORBA::Environment &env);
    
  virtual void disconnect (const AVStreams::flowSpec &the_spec,  
                           CORBA::Environment &env);
    
  virtual void set_FPStatus (const AVStreams::flowSpec &the_spec, 
                             const char *fp_name, 
                             const CORBA::Any &fp_settings,  
                             CORBA::Environment &env);
    
  virtual CORBA::Object_ptr get_fep (const char *flow_name,  
                                     CORBA::Environment &env);
    
  virtual char * add_fep (CORBA::Object_ptr the_fep,  
                          CORBA::Environment &env);
    
  virtual void remove_fep (const char *fep_name,  
                           CORBA::Environment &env);
    
  virtual void set_negotiator (AVStreams::Negotiator_ptr new_negotiator,  
                               CORBA::Environment &env);
  
  virtual void set_key (const char *flow_name, 
                        const char *the_key,  
                        CORBA::Environment &env);
    
  virtual void set_source_id (CORBA::Long source_id,  
                              CORBA::Environment &env);

  virtual ~TAO_StreamEndPoint (void);

};

class TAO_StreamEndPoint_A : public POA_AVStreams::StreamEndPoint_A
{
public:
  TAO_StreamEndPoint_A (void);

  virtual CORBA::Boolean multiconnect (AVStreams::streamQoS &the_qos, 
                                       AVStreams::flowSpec &the_spec,  
                                       CORBA::Environment &env);
  
  virtual CORBA::Boolean connect_leaf (AVStreams::StreamEndPoint_B_ptr the_ep, 
                                       AVStreams::streamQoS &the_qos, 
                                       const AVStreams::flowSpec &the_flows,  
                                       CORBA::Environment &env);
  
  virtual void disconnect_leaf (AVStreams::StreamEndPoint_B_ptr the_ep, 
                                const AVStreams::flowSpec &theSpec,  
                                CORBA::Environment &env);
  
  virtual ~TAO_StreamEndPoint_A (void);

};

class TAO_StreamEndPoint_B : public POA_AVStreams::StreamEndPoint_B
{
public:
  TAO_StreamEndPoint_B (void);

  virtual CORBA::Boolean multiconnect (AVStreams::streamQoS &the_qos, 
                                       AVStreams::flowSpec &the_spec,  
                                       CORBA::Environment &env);

  virtual ~TAO_StreamEndPoint_B (void);

};

class TAO_VDev : public POA_AVStreams::VDev
{
public:
  TAO_VDev (void);

  virtual CORBA::Boolean set_peer (AVStreams::StreamCtrl_ptr the_ctrl, 
                                   AVStreams::VDev_ptr the_peer_dev, 
                                   AVStreams::streamQoS &the_qos, 
                                   const AVStreams::flowSpec &the_spec,  
                                   CORBA::Environment &env);

  virtual CORBA::Boolean set_Mcast_peer (AVStreams::StreamCtrl_ptr the_ctrl, 
                                         AVStreams::MCastConfigIf_ptr a_mcastconfigif, 
                                         AVStreams::streamQoS &the_qos, 
                                         const AVStreams::flowSpec &the_spec,  
                                         CORBA::Environment &env);
  
  virtual void configure (const CosPropertyService::Property &the_config_mesg,  
                          CORBA::Environment &env);
  
  virtual void set_format (const char *flowName, 
                           const char *format_name,  
                           CORBA::Environment &env);
  
  virtual void set_dev_params (const char *flowName, 
                               const CosPropertyService::Properties &new_params,  
                               CORBA::Environment &env);
  
  virtual CORBA::Boolean modify_QoS (AVStreams::streamQoS &the_qos, 
                                     const AVStreams::flowSpec &the_spec,  
                                     CORBA::Environment &env);
  
  virtual ~TAO_VDev (void);
};

class TAO_MMDevice : public POA_AVStreams::MMDevice
{
public:
  TAO_MMDevice (void);

  virtual AVStreams::StreamEndPoint_A_ptr  create_A (AVStreams::StreamCtrl_ptr the_requester, 
                                                     AVStreams::VDev_out the_vdev, 
                                                     AVStreams::streamQoS &the_qos, 
                                                     CORBA::Boolean_out met_qos, 
                                                     char *&named_vdev, 
                                                     const AVStreams::flowSpec &the_spec,  
                                                     CORBA::Environment &env);
  
  virtual AVStreams::StreamEndPoint_B_ptr  create_B (AVStreams::StreamCtrl_ptr the_requester, 
                                                     AVStreams::VDev_out the_vdev, 
                                                     AVStreams::streamQoS &the_qos, 
                                                     CORBA::Boolean_out met_qos, 
                                                     char *&named_vdev, 
                                                     const AVStreams::flowSpec &the_spec,  
                                                     CORBA::Environment &env);
  
  virtual AVStreams::StreamCtrl_ptr  bind (AVStreams::MMDevice_ptr peer_device,
                                           AVStreams::streamQoS &the_qos,
                                           CORBA::Boolean_out is_met,
                                           const AVStreams::flowSpec &the_spec,
                                           CORBA::Environment &env);

  virtual AVStreams::StreamCtrl_ptr  bind_mcast (AVStreams::MMDevice_ptr first_peer,
                                                 AVStreams::streamQoS &the_qos,
                                                 CORBA::Boolean_out is_met,
                                                 const AVStreams::flowSpec &the_spec,
                                                 CORBA::Environment &env);

  virtual void destroy (AVStreams::StreamEndPoint_ptr the_ep,
                        const char *vdev_name,
                        CORBA::Environment &env);

  virtual char * add_fdev (CORBA::Object_ptr the_fdev,
                           CORBA::Environment &env);

  virtual CORBA::Object_ptr get_fdev (const char *flow_name,  
                                      CORBA::Environment &env);

  virtual void remove_fdev (const char *flow_name,  
                            CORBA::Environment &env);
  
  
  virtual ~TAO_MMDevice (void);
};



#endif /* AVSTREAMS_I_H */
