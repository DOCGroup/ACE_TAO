// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   AVStreams_i.cpp
//
// = AUTHOR
//    Sumedh Mungee <sumedh@cs.wustl.edu>
//    
//
// ============================================================================

#include "AVStreams_i.h"

// ----------------------------------------------------------------------
// TAO_Basic_StreamCtrl
// ----------------------------------------------------------------------

TAO_Basic_StreamCtrl::TAO_Basic_StreamCtrl (void)
{
}

TAO_Basic_StreamCtrl::~TAO_Basic_StreamCtrl (void)
{
}

void 
TAO_Basic_StreamCtrl::stop (const AVStreams::flowSpec &the_spec,  
                            CORBA::Environment &env)
{
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (env);
}

void
TAO_Basic_StreamCtrl::start (const AVStreams::flowSpec &the_spec,  
                             CORBA::Environment &env)
{
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (env);
}

void
TAO_Basic_StreamCtrl::destroy (const AVStreams::flowSpec &the_spec,  
                               CORBA::Environment &env)
{
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (env);
}


CORBA::Boolean 
TAO_Basic_StreamCtrl::modify_QoS (AVStreams::streamQoS &new_qos, 
                                  const AVStreams::flowSpec &the_spec,  
                                  CORBA::Environment &env)
{
  ACE_UNUSED_ARG (new_qos);
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (env);
  return 0;
}

void
TAO_Basic_StreamCtrl::push_event (const char *the_event,  
                                  CORBA::Environment &env)
{
  ACE_UNUSED_ARG (the_event);
  ACE_UNUSED_ARG (env);
}

void
TAO_Basic_StreamCtrl::set_FPStatus (const AVStreams::flowSpec &the_spec, 
                                    const char *fp_name, 
                                    const CORBA::Any &fp_settings,  
                                    CORBA::Environment &env)
{
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (fp_name);
  ACE_UNUSED_ARG (fp_settings);
  ACE_UNUSED_ARG (env);
}

CORBA::Object_ptr 
TAO_Basic_StreamCtrl::get_flow_connection (const char *flow_name,  
                                                 CORBA::Environment &env)
{
  ACE_UNUSED_ARG (flow_name);
  ACE_UNUSED_ARG (env);
  return 0;
}

void
TAO_Basic_StreamCtrl::set_flow_connection (const char *flow_name, 
                                           CORBA::Object_ptr flow_connection,  
                                           CORBA::Environment &env)
{
  ACE_UNUSED_ARG (flow_name);
  ACE_UNUSED_ARG (flow_connection);
  ACE_UNUSED_ARG (env);
}

// ----------------------------------------------------------------------
// TAO_StreamCtrl
// ----------------------------------------------------------------------

TAO_StreamCtrl::TAO_StreamCtrl (void)
{
}

TAO_StreamCtrl::~TAO_StreamCtrl (void)
{
}

CORBA::Boolean 
TAO_StreamCtrl::bind_devs (AVStreams::MMDevice_ptr a_party, 
                           AVStreams::MMDevice_ptr b_party, 
                           AVStreams::streamQoS &the_qos, 
                           const AVStreams::flowSpec &the_flows,  
                           CORBA::Environment &env)
{
  return 0;
}
  
CORBA::Boolean 
TAO_StreamCtrl::bind (AVStreams::StreamEndPoint_A_ptr a_party, 
                      AVStreams::StreamEndPoint_B_ptr b_party, 
                      AVStreams::streamQoS &the_qos, 
                      const AVStreams::flowSpec &the_flows,  
                      CORBA::Environment &env)
{
  return 0;
}

void 
TAO_StreamCtrl::unbind_party (AVStreams::StreamEndPoint_ptr the_ep, 
                              const AVStreams::flowSpec &the_spec,  
                              CORBA::Environment &env)
{
}

// ----------------------------------------------------------------------
// TAO_StreamEndPoint
// ----------------------------------------------------------------------

TAO_StreamEndPoint::TAO_StreamEndPoint (void)
{
}

void 
TAO_StreamEndPoint::stop (const AVStreams::flowSpec &the_spec,  
                          CORBA::Environment &env)
{
}

void 
TAO_StreamEndPoint::start (const AVStreams::flowSpec &the_spec,  
                           CORBA::Environment &env)
{
}

void 
TAO_StreamEndPoint::destroy (const AVStreams::flowSpec &the_spec,  
                             CORBA::Environment &env)
{
}

CORBA::Boolean 
TAO_StreamEndPoint::connect (AVStreams::StreamEndPoint_ptr responder, 
                             AVStreams::streamQoS &qos_spec, 
                             const AVStreams::flowSpec &the_spec,  
                             CORBA::Environment &env)
{
  return 0;
}
  
CORBA::Boolean 
TAO_StreamEndPoint::request_connection (AVStreams::StreamEndPoint_ptr initiator, 
                                        CORBA::Boolean is_mcast, 
                                        AVStreams::streamQoS &qos, 
                                        AVStreams::flowSpec &the_spec,  
                                        CORBA::Environment &env)
{
  return 0;
}
  
CORBA::Boolean 
TAO_StreamEndPoint::modify_QoS (AVStreams::streamQoS &new_qos, 
                                const AVStreams::flowSpec &the_flows,  
                                CORBA::Environment &env)
{
  return 0;
}
  
CORBA::Boolean 
TAO_StreamEndPoint::set_protocol_restriction (const AVStreams::protocolSpec &the_pspec,  
                                              CORBA::Environment &env)
{
  return 0;
}
  
void 
TAO_StreamEndPoint::disconnect (const AVStreams::flowSpec &the_spec,  
                                CORBA::Environment &env)
{
}
  
void 
TAO_StreamEndPoint::set_FPStatus (const AVStreams::flowSpec &the_spec, 
                                  const char *fp_name, 
                                  const CORBA::Any &fp_settings,  
                                  CORBA::Environment &env)
{
}

CORBA::Object_ptr 
TAO_StreamEndPoint::get_fep (const char *flow_name,  
                                   CORBA::Environment &env)
{
  return 0;
}
  
char * 
TAO_StreamEndPoint::add_fep (CORBA::Object_ptr the_fep,  
                        CORBA::Environment &env)
{
  return 0;
}
  
void 
TAO_StreamEndPoint::remove_fep (const char *fep_name,  
                                CORBA::Environment &env)
{
}
  
void 
TAO_StreamEndPoint::set_negotiator (AVStreams::Negotiator_ptr new_negotiator,  
                                    CORBA::Environment &env)
{
}

void 
TAO_StreamEndPoint::set_key (const char *flow_name, 
                             const char *the_key,  
                             CORBA::Environment &env)
{
}
  
void 
TAO_StreamEndPoint::set_source_id (CORBA::Long source_id,  
                                   CORBA::Environment &env)
{
}

TAO_StreamEndPoint::~TAO_StreamEndPoint (void)
{
}

// ----------------------------------------------------------------------
// TAO_StreamEndPoint_A
// ----------------------------------------------------------------------

TAO_StreamEndPoint_A::TAO_StreamEndPoint_A (void)
{
}

CORBA::Boolean 
TAO_StreamEndPoint_A::multiconnect (AVStreams::streamQoS &the_qos, 
                                    AVStreams::flowSpec &the_spec,  
                                    CORBA::Environment &env)
{
  return 0;
}
  
CORBA::Boolean 
TAO_StreamEndPoint_A::connect_leaf (AVStreams::StreamEndPoint_B_ptr the_ep, 
                                    AVStreams::streamQoS &the_qos, 
                                    const AVStreams::flowSpec &the_flows,  
                                    CORBA::Environment &env)
{
  return 0;
}
  
void 
TAO_StreamEndPoint_A::disconnect_leaf (AVStreams::StreamEndPoint_B_ptr the_ep, 
                                       const AVStreams::flowSpec &theSpec,  
                                       CORBA::Environment &env)
{
}
  
TAO_StreamEndPoint_A::~TAO_StreamEndPoint_A (void)
{
}

// ----------------------------------------------------------------------
// TAO_StreamEndPoint_B
// ----------------------------------------------------------------------

TAO_StreamEndPoint_B::TAO_StreamEndPoint_B (void)
{
}

CORBA::Boolean 
TAO_StreamEndPoint_B::multiconnect (AVStreams::streamQoS &the_qos, 
                                       AVStreams::flowSpec &the_spec,  
                                       CORBA::Environment &env)
{
  return 0;
}

TAO_StreamEndPoint_B::~TAO_StreamEndPoint_B (void)
{
}

// ----------------------------------------------------------------------
// TAO_VDev
// ----------------------------------------------------------------------

TAO_VDev::TAO_VDev (void)
{
}

CORBA::Boolean 
TAO_VDev::set_peer (AVStreams::StreamCtrl_ptr the_ctrl, 
                    AVStreams::VDev_ptr the_peer_dev, 
                    AVStreams::streamQoS &the_qos, 
                    const AVStreams::flowSpec &the_spec,  
                    CORBA::Environment &env)
{
  return 0;
}

CORBA::Boolean 
TAO_VDev::set_Mcast_peer (AVStreams::StreamCtrl_ptr the_ctrl, 
                          AVStreams::MCastConfigIf_ptr a_mcastconfigif, 
                          AVStreams::streamQoS &the_qos, 
                          const AVStreams::flowSpec &the_spec,  
                          CORBA::Environment &env)
{
  return 0;
}

void 
TAO_VDev::configure (const CosPropertyService::Property &the_config_mesg,  
                     CORBA::Environment &env)
{
}

void 
TAO_VDev::set_format (const char *flowName, 
                      const char *format_name,  
                      CORBA::Environment &env)
{
}

void 
TAO_VDev::set_dev_params (const char *flowName, 
                          const CosPropertyService::Properties &new_params,  
                          CORBA::Environment &env)
{
}

CORBA::Boolean 
TAO_VDev::modify_QoS (AVStreams::streamQoS &the_qos, 
                      const AVStreams::flowSpec &the_spec,  
                      CORBA::Environment &env)
{
  return 0;
}

TAO_VDev::~TAO_VDev (void)
{
}

// ----------------------------------------------------------------------
// TAO_MMDevice
// ----------------------------------------------------------------------

TAO_MMDevice::TAO_MMDevice (void)
{
}

AVStreams::StreamEndPoint_A_ptr  
TAO_MMDevice::create_A (AVStreams::StreamCtrl_ptr the_requester, 
                                                   AVStreams::VDev_out the_vdev, 
                                                   AVStreams::streamQoS &the_qos, 
                                                   CORBA::Boolean_out met_qos, 
                                                   char *&named_vdev, 
                                                   const AVStreams::flowSpec &the_spec,  
                                                   CORBA::Environment &env)
{
  return 0;
}

AVStreams::StreamEndPoint_B_ptr  
TAO_MMDevice::create_B (AVStreams::StreamCtrl_ptr the_requester, 
                        AVStreams::VDev_out the_vdev, 
                        AVStreams::streamQoS &the_qos, 
                        CORBA::Boolean_out met_qos, 
                        char *&named_vdev, 
                        const AVStreams::flowSpec &the_spec,  
                        CORBA::Environment &env)
{
  return 0;
}

AVStreams::StreamCtrl_ptr  
TAO_MMDevice::bind (AVStreams::MMDevice_ptr peer_device,
                    AVStreams::streamQoS &the_qos,
                    CORBA::Boolean_out is_met,
                    const AVStreams::flowSpec &the_spec,
                    CORBA::Environment &env)
{
  return 0;
}

AVStreams::StreamCtrl_ptr  
TAO_MMDevice::bind_mcast (AVStreams::MMDevice_ptr first_peer,
                          AVStreams::streamQoS &the_qos,
                          CORBA::Boolean_out is_met,
                          const AVStreams::flowSpec &the_spec,
                          CORBA::Environment &env)
{
  return 0;
}

void 
TAO_MMDevice::destroy (AVStreams::StreamEndPoint_ptr the_ep,
                       const char *vdev_name,
                       CORBA::Environment &env)
{
}

char * 
TAO_MMDevice::add_fdev (CORBA::Object_ptr the_fdev,
                        CORBA::Environment &env)
{
  return 0;
}

CORBA::Object_ptr 
TAO_MMDevice::get_fdev (const char *flow_name,  
                        CORBA::Environment &env)
{
  return 0;
}

void 
TAO_MMDevice::remove_fdev (const char *flow_name,  
                           CORBA::Environment &env)
{
}


TAO_MMDevice::~TAO_MMDevice (void)
{
}
