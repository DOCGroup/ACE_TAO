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

TAO_Basic_StreamCtrl::TAO_Basic_StreamCtrl (CORBA::ORB_var orb)
: orb_ (orb)
{
}

TAO_Basic_StreamCtrl::~TAO_Basic_StreamCtrl (void)
{
}

void 
TAO_Basic_StreamCtrl::stop (const AVStreams::flowSpec &the_spec,  
                            CORBA::Environment &env)
{
   if (CORBA::is_nil (stream_endpoint_a_.in ()))
     return;
   this->stream_endpoint_a_->stop (the_spec, env);
}

void
TAO_Basic_StreamCtrl::start (const AVStreams::flowSpec &the_spec,  
                             CORBA::Environment &env)
{
  if (CORBA::is_nil (this->stream_endpoint_a_.in ()))
    return;
  this->stream_endpoint_a_->start (the_spec, env);
}

void
TAO_Basic_StreamCtrl::destroy (const AVStreams::flowSpec &the_spec,  
                               CORBA::Environment &env)
{
  if (CORBA::is_nil (this->stream_endpoint_a_.in ()))
    return;
  this->stream_endpoint_a_->destroy (the_spec, env);
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
  ACE_DEBUG ((LM_DEBUG, 
              "\n(%P|%t) Recieved event \"%s\"",
              the_event));
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

// @@ Need to throw not-supported exception here
CORBA::Object_ptr 
TAO_Basic_StreamCtrl::get_flow_connection (const char *flow_name,  
                                                 CORBA::Environment &env)
{
  ACE_UNUSED_ARG (flow_name);
  ACE_UNUSED_ARG (env);
  return 0;
}

// @@ Need to throw not-supported exception here
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

TAO_StreamCtrl::TAO_StreamCtrl (CORBA::ORB_var orb)
  : TAO_Basic_StreamCtrl (orb)
{
}

TAO_StreamCtrl::~TAO_StreamCtrl (void)
{
}

// request the two MMDevices to create vdev and stream endpoints. save
// the references returned.

// The interaction diagram for this method is on page 13 of the spec
CORBA::Boolean 
TAO_StreamCtrl::bind_devs (AVStreams::MMDevice_ptr a_party, 
                           AVStreams::MMDevice_ptr b_party, 
                           AVStreams::streamQoS &the_qos, 
                           const AVStreams::flowSpec &the_flows,  
                           CORBA::Environment &env)
{
  // Check to see if we have non-nil parties to bind!
  if (CORBA::is_nil (a_party) || 
      CORBA::is_nil (b_party))
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "\n(%P|%t) TAO_StreamCtrl::bind_devs: "
                       "a_party or b_party is null!\n"),
                      0);
  
  // Request a_party to create the endpoint and vdev
  CORBA::Boolean met_qos;
  CORBA::String_var named_vdev (CORBA::string_alloc (1024));

  this->stream_endpoint_a_ =
    a_party-> create_A (this->_this (env),
                        this->vdev_a_.out (),
                        the_qos,
                        met_qos,
                        named_vdev.inout (),
                        the_flows,
                        env);
  TAO_CHECK_ENV_RETURN (env, 0);
  
  ACE_DEBUG ((LM_DEBUG,
              "\n(%P|%t) TAO_StreamCtrl::create_A: succeeded"));

  // Request b_party to create the endpoint and vdev

  this->stream_endpoint_b_ =
     b_party-> create_B (this->_this (env),
			 this->vdev_b_.out (),
			 the_qos,
			 met_qos,
			 named_vdev.inout (),
			 the_flows,
			 env);
   TAO_CHECK_ENV_RETURN (env, 0);
  
   ACE_DEBUG ((LM_DEBUG,
	       "\n(%P|%t) TAO_StreamCtrl::create_B: succeeded"));
  
   ACE_DEBUG ((LM_DEBUG, 
	       "\nstream_endpoint_b_ = %s",
              this->orb_->object_to_string (this->stream_endpoint_b_,
                                            env)));
  TAO_CHECK_ENV_RETURN (env, 0);
  // Now connect the streams together
  this->stream_endpoint_a_->connect (this->stream_endpoint_b_,
                                     the_qos,
                                     the_flows,
                                     env);
  TAO_CHECK_ENV_RETURN (env, 0);
  return 1;
}
  
CORBA::Boolean 
TAO_StreamCtrl::bind (AVStreams::StreamEndPoint_A_ptr a_party, 
                      AVStreams::StreamEndPoint_B_ptr b_party, 
                      AVStreams::streamQoS &the_qos, 
                      const AVStreams::flowSpec &the_flows,  
                      CORBA::Environment &env)
{
  this->stream_endpoint_a_ = a_party;
  this->stream_endpoint_b_ = b_party;
  // Now connect the streams together
  this->stream_endpoint_a_->connect (this->stream_endpoint_b_,
                                     the_qos,
                                     the_flows,
                                     env);
  TAO_CHECK_ENV_RETURN (env, 0);
  return 1;
}

void 
TAO_StreamCtrl::unbind (CORBA::Environment &env)
{
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

// Call request_connection on the responder
CORBA::Boolean 
TAO_StreamEndPoint::connect (AVStreams::StreamEndPoint_ptr responder, 
                             AVStreams::streamQoS &qos_spec, 
                             const AVStreams::flowSpec &the_spec,  
                             CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, "\n(%P|%t) In TAO_StreamEndPoint::connect"));
  responder->request_connection (this->_this (env),
                                 0,
                                 qos_spec,
                                 the_spec,
                                 env);
  TAO_CHECK_ENV_RETURN (env, 1);
  return 0;
}
  
CORBA::Boolean 
TAO_StreamEndPoint::request_connection (AVStreams::StreamEndPoint_ptr initiator, 
                                        CORBA::Boolean is_mcast, 
                                        AVStreams::streamQoS &qos, 
                                        AVStreams::flowSpec &the_spec,  
                                        CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, "\n(%P|%t) TAO_StreamEndPoint::request_connection called"));
  ACE_DEBUG ((LM_DEBUG, 
              "\n(%P|%t) TAO_StreamEndPoint::request_connection: "
              "flowspec has length = %d"
              "and the strings are:",
              the_spec.length ()));
  for (int i = 0; i < the_spec.length (); i++)
    ACE_DEBUG ((LM_DEBUG,
                the_spec [i]));
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
  ACE_DEBUG ((LM_DEBUG, 
              "\n(%P|%t) TAO_StreamEndPoint_A::TAO_StreamEndPoint_A: created"));
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
  ACE_DEBUG ((LM_DEBUG, 
              "\n(%P|%t) TAO_StreamEndPoint_B::TAO_StreamEndPoint_B: created"));
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
  ACE_DEBUG ((LM_DEBUG, 
              "\n(%P|%t) TAO_VDev::TAO_VDev: created"));
}

// StreamCtrl will call this to give us a reference to itself, and to
// our peer vdev..
CORBA::Boolean 
TAO_VDev::set_peer (AVStreams::StreamCtrl_ptr the_ctrl, 
                    AVStreams::VDev_ptr the_peer_dev, 
                    AVStreams::streamQoS &the_qos, 
                    const AVStreams::flowSpec &the_spec,  
                    CORBA::Environment &env)
{
  this->streamctrl_ = the_ctrl;
  this->peer_ = the_peer_dev;
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
  ACE_DEBUG ((LM_DEBUG,"\n(%P|%t) TAO_MMDevice created"));
}

// We have been asked to create a new stream_endpoint and a vdev.
AVStreams::StreamEndPoint_A_ptr  
TAO_MMDevice::create_A (AVStreams::StreamCtrl_ptr the_requester, 
                        AVStreams::VDev_out the_vdev, 
                        AVStreams::streamQoS &the_qos, 
                        CORBA::Boolean_out met_qos, 
                        char *&named_vdev, 
                        const AVStreams::flowSpec &the_spec,  
                        CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, 
              "\n(%P|%t) TAO_MMDevice::create_A: called"));

  TAO_VDev *vdev = new TAO_VDev;
  the_vdev = AVStreams::VDev::_duplicate (vdev->_this (env));

  TAO_StreamEndPoint_A *stream_endpoint_a = 
    new TAO_StreamEndPoint_A;
  return AVStreams::StreamEndPoint_A::_duplicate 
    (stream_endpoint_a->_this (env));
}

// We have been asked to create a new stream_endpoint and a vdev.
AVStreams::StreamEndPoint_B_ptr  
TAO_MMDevice::create_B (AVStreams::StreamCtrl_ptr the_requester, 
                        AVStreams::VDev_out the_vdev, 
                        AVStreams::streamQoS &the_qos, 
                        CORBA::Boolean_out met_qos, 
                        char *&named_vdev, 
                        const AVStreams::flowSpec &the_spec,  
                        CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, 
              "\n(%P|%t) TAO_MMDevice::create_B: called"));

  TAO_VDev *vdev = new TAO_VDev;
  the_vdev = AVStreams::VDev::_duplicate (vdev->_this (env));

  TAO_StreamEndPoint_B *stream_endpoint_b = 
    new TAO_StreamEndPoint_B;
  return AVStreams::StreamEndPoint_B::_duplicate 
    (stream_endpoint_b->_this (env));
}

// create a streamctrl which is colocated with me, use that streamctrl
// to bind the peer_device with me.
AVStreams::StreamCtrl_ptr  
TAO_MMDevice::bind (AVStreams::MMDevice_ptr peer_device,
                    AVStreams::streamQoS &the_qos,
                    CORBA::Boolean_out is_met,
                    const AVStreams::flowSpec &the_spec,
                    CORBA::Environment &env)
{
#if 0
  TAO_TRY
    {
      TAO_StreamCtrl *stream_ctrl = new TAO_StreamCtrl ();
      if (stream_ctrl == 0)
        return 0;
      stream_ctrl->bind_devs (peer_device,
                              AVStreams::StreamCtrl::_duplicate 
                              (this->_this (TAO_TRY_ENV)),
                              the_qos,
                              the_spec,
                              TAO_TRY_ENV);
      TAO_CHECK_ENV;
      return stream_ctrl;
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;
#endif
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

// need to throw notsupported exception
char * 
TAO_MMDevice::add_fdev (CORBA::Object_ptr the_fdev,
                        CORBA::Environment &env)
{
  return 0;
}

// need to throw notsupported exception
CORBA::Object_ptr 
TAO_MMDevice::get_fdev (const char *flow_name,  
                        CORBA::Environment &env)
{
  return 0;
}

// need to throw notsupported exception
void 
TAO_MMDevice::remove_fdev (const char *flow_name,  
                           CORBA::Environment &env)
{
}


TAO_MMDevice::~TAO_MMDevice (void)
{
}

