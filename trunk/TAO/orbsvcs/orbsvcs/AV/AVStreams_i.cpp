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
//    Nagarajan Surendran <naga@cs.wustl.edu>
//
//
// ============================================================================

#include "AVStreams_i.h"
#include "orbsvcs/Trader/Trader.h"

ACE_RCSID(AV, AVStreams_i, "$Id$")

// ----------------------------------------------------------------------
// AV_Null_MediaCtrl
// ----------------------------------------------------------------------
AV_Null_MediaCtrl::AV_Null_MediaCtrl (void)
{
}


// ----------------------------------------------------------------------
// TAO_Basic_StreamCtrl
// ----------------------------------------------------------------------

// Constructor
TAO_Basic_StreamCtrl::TAO_Basic_StreamCtrl (void)
  :flow_count_ (0)
{
}

TAO_Basic_StreamCtrl::~TAO_Basic_StreamCtrl (void)
{
}

// Stop the transfer of data of the stream
// Empty the_spec means apply operation to all flows
void
TAO_Basic_StreamCtrl::stop (const AVStreams::flowSpec &the_spec,
                            CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->sep_a_.in ()))
    return;

  // Make the upcall into the application
  this->sep_a_->stop (the_spec, ACE_TRY_ENV);
}

// Start the transfer of data in the stream.
// Empty the_spec means apply operation to all flows
void
TAO_Basic_StreamCtrl::start (const AVStreams::flowSpec &flow_spec,
                             CORBA::Environment &ACE_TRY_ENV)
{
  // call start on the flow connections.
  if (CORBA::is_nil (this->sep_a_.in ()))
    return;

  // Make the upcall into the application
  this->sep_a_->start (flow_spec, ACE_TRY_ENV);
}

// Tears down the stream. This will close the connection, and delete
// the streamendpoint and vdev associated with this stream
// Empty the_spec means apply operation to all flows
void
TAO_Basic_StreamCtrl::destroy (const AVStreams::flowSpec &the_spec,
                               CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->sep_a_.in ()))
    return;

  // Make the upcall into the application
  this->sep_a_->destroy (the_spec, ACE_TRY_ENV);
}

// Changes the QoS associated with the stream
// Empty the_spec means apply operation to all flows
CORBA::Boolean
TAO_Basic_StreamCtrl::modify_QoS (AVStreams::streamQoS & new_qos,
                                  const AVStreams::flowSpec &the_spec,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      this->vdev_a_->modify_QoS (new_qos,the_spec,ACE_TRY_ENV);
      ACE_TRY_CHECK;
      this->vdev_b_->modify_QoS (new_qos,the_spec,ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_Basic_StreamCtrl::modify_QoS");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

// Used by StreamEndPoint and VDev to inform StreamCtrl of events.
// E.g., loss of flow, reestablishment of flow, etc..
void
TAO_Basic_StreamCtrl::push_event (const struct CosPropertyService::Property & /* the_event */,
                                  CORBA::Environment & /* ACE_TRY_ENV */)
{
  ACE_DEBUG ((LM_DEBUG,"\n(%P|%t) Recieved event \""));
}

// @@ Need to throw not-supported exception here
void
TAO_Basic_StreamCtrl::set_FPStatus (const AVStreams::flowSpec &/* the_spec */,
                                    const char  * /* fp_name */,
                                    const CORBA::Any &/* fp_settings */,
                                    CORBA::Environment &/* ACE_TRY_ENV */)
{
}

// @@ Need to throw not-supported exception here
CORBA::Object_ptr
TAO_Basic_StreamCtrl::get_flow_connection (const char *flow_name,
                                           CORBA::Environment &ACE_TRY_ENV)
{
  TAO_String_Hash_Key flow_name_key (flow_name);
  FlowConnection_Map::ENTRY *flow_connection_entry = 0;
  if (this->flow_map_.find (flow_name_key,flow_connection_entry) == 0)
    return flow_connection_entry->int_id_;
  else
    ACE_THROW_RETURN (AVStreams::noSuchFlow (),CORBA::Object::_nil ());
}

// @@ Need to throw not-supported exception here
void
TAO_Basic_StreamCtrl::set_flow_connection (const char *flow_name,
                                           CORBA::Object_ptr flow_connection,
                                           CORBA::Environment &ACE_TRY_ENV)
{
  // add the flowname and the flowconnection to the hashtable.
  this->flows_ [this->flow_count_++] = CORBA::string_dup (flow_name);
  TAO_String_Hash_Key flow_name_key (flow_name);
  if (this->flow_map_.bind (flow_name_key,flow_connection) != 0)
    ACE_THROW (AVStreams::noSuchFlow ());// is this right?
}


// ----------------------------------------------------------------------
// TAO_Negotiator
// ----------------------------------------------------------------------

CORBA::Boolean
TAO_Negotiator::negotiate (AVStreams::Negotiator_ptr remote_negotiator,
                           const AVStreams::streamQoS &qos_spec,
                           CORBA::Environment &ACE_TRY_ENV)
{
  return 0;
}

// ----------------------------------------------------------------------
// TAO_StreamCtrl
// ----------------------------------------------------------------------

TAO_StreamCtrl::TAO_StreamCtrl (void)
  :mcastconfigif_ (0),
   mcastconfigif_ptr_ (0)
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
                           CORBA::Environment &ACE_TRY_ENV)
{
  // do a Qos Translation from application level Qos to Network level Qos??
  ACE_TRY
    {
      if (CORBA::is_nil (a_party) && CORBA::is_nil (b_party))
        ACE_ERROR_RETURN ((LM_ERROR,"Both parties are nil\n"),0);
      // Check to see if we have non-nil parties to bind!
      if (CORBA::is_nil (a_party) ||
          CORBA::is_nil (b_party))
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) TAO_StreamCtrl::bind_devs: "
                    "a_party or b_party is null"
                    "Multicast mode\n"));

      // Request a_party to create the endpoint and vdev
      CORBA::Boolean met_qos;
      CORBA::String_var named_vdev;

      if (!CORBA::is_nil (a_party))
        {
          this->sep_a_ =
            a_party-> create_A (this->_this (ACE_TRY_ENV),
                                this->vdev_a_.out (),
                                the_qos,
                                met_qos,
                                named_vdev.inout (),
                                the_flows,
                                ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) TAO_StreamCtrl::create_A: succeeded\n"));
        }

      // Request b_party to create the endpoint and vdev

      if (!CORBA::is_nil (b_party))
        {
          this->sep_b_ =
            b_party-> create_B (this->_this (ACE_TRY_ENV),
                                this->vdev_b_.out (),
                                the_qos,
                                met_qos,
                                named_vdev.inout (),
                                the_flows,
                                ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) TAO_StreamCtrl::create_B: succeeded\n"));
          ACE_DEBUG ((LM_DEBUG,
                      "\n(%P|%t)stream_endpoint_b_ = %s",
                      TAO_ORB_Core_instance ()->orb ()->object_to_string (this->sep_b_.in (),
                                                                          ACE_TRY_ENV)));
          ACE_TRY_CHECK;
        }

      if (CORBA::is_nil (b_party))
        {
          if (this->mcastconfigif_ != 0)
            {
              ACE_NEW_RETURN (this->mcastconfigif_,
                              TAO_MCastConfigIf,
                              0);
              this->mcastconfigif_ptr_ = this->mcastconfigif_->_this (ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          // Multicast source being added.
          CORBA::Boolean result = this->vdev_a_->set_Mcast_peer (this->_this (ACE_TRY_ENV),
                                                                 this->mcastconfigif_ptr_,
                                                                 the_qos,
                                                                 the_flows,
                                                                 ACE_TRY_ENV);
          if (!result)
            ACE_ERROR_RETURN ((LM_ERROR,"set_Mcast_peer failed\n"),0);
        }

      if (CORBA::is_nil (a_party))
        {
          // Multicast sink being added.
          if (this->mcastconfigif_ != 0)
            ACE_ERROR_RETURN ((LM_ERROR,"first add a source and then a sink\n"),0);
          this->mcastconfigif_->set_peer (this->vdev_b_.in (),
                                          the_qos,
                                          the_flows,
                                          ACE_TRY_ENV);
          ACE_TRY_CHECK;

          this->sep_a_->connect_leaf (this->sep_b_.in (),
                                      the_qos,
                                      the_flows,
                                      ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      if (!CORBA::is_nil (a_party) && !CORBA::is_nil (b_party))
        {
          // Tell the 2 VDev's about one another
          this->vdev_a_->set_peer (this->_this (ACE_TRY_ENV),
                                   this->vdev_b_.in (),
                                   the_qos,
                                   the_flows,
                                   ACE_TRY_ENV);

          ACE_TRY_CHECK;

          this->vdev_b_->set_peer (this->_this (ACE_TRY_ENV),
                                   this->vdev_a_.in (),
                                   the_qos,
                                   the_flows,
                                   ACE_TRY_ENV);

          ACE_TRY_CHECK;


          // Now connect the streams together. This will
          // establish the connection
          this->sep_a_->connect (this->sep_b_.in (),
                                             the_qos,
                                             the_flows,
                                             ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_StreamCtrl::bind_devs");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

// Used to establish a connection between two endpoints
// directly, i.e. without a MMDevice
CORBA::Boolean
TAO_StreamCtrl::bind (AVStreams::StreamEndPoint_A_ptr sep_a,
                      AVStreams::StreamEndPoint_B_ptr sep_b,
                      AVStreams::streamQoS &the_qos,
                      const AVStreams::flowSpec &the_flows,
                      CORBA::Environment &ACE_TRY_ENV)
{
  this->sep_a_ = sep_a;
  this->sep_b_ = sep_b;

  ACE_TRY
    {
      if (CORBA::is_nil (sep_a) ||
          CORBA::is_nil (sep_b))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) TAO_StreamCtrl::bind:"
                           "a_party or b_party null!"),
                          0);

      // since its full profile we do the viable stream setup algorithm.
      // get the flows for the A streamendpoint.
      if (the_flows.length () == 0)
        {
          // the flows spec is empty and hence we do a exhaustive match.
          AVStreams::flowSpec *a_flows = 0,*b_flows = 0;
          AVStreams::FlowEndPoint_seq a_feps,b_feps;
          CORBA::Any_ptr flows_any;
          flows_any = sep_a->get_property_value ("Flows",ACE_TRY_ENV);
          ACE_TRY_CHECK;
          *flows_any >>= a_flows;
          flows_any = sep_b->get_property_value ("Flows",ACE_TRY_ENV);
          ACE_TRY_CHECK;
          *flows_any >>= b_flows;
          u_int i;
          for (i=0;i<a_flows->length ();i++)
            {
              // get the flowendpoint references.
              CORBA::Object_ptr fep_obj;
              fep_obj = sep_a->get_fep ((*a_flows)[i],
                                        ACE_TRY_ENV);
              ACE_TRY_CHECK;
              AVStreams::FlowEndPoint_ptr fep;
              fep = AVStreams::FlowEndPoint::_narrow (fep_obj,
                                                      ACE_TRY_ENV);
              ACE_TRY_CHECK;
              a_feps [i] = fep;
            }
          // get the flowendpoints for streamendpoint_b
          for (i=0;i<b_flows->length ();i++)
            {
              // get the flowendpoint references.
              CORBA::Object_ptr fep_obj;
              fep_obj = sep_b->get_fep ((*b_flows)[i],
                                        ACE_TRY_ENV);
              ACE_TRY_CHECK;
              AVStreams::FlowEndPoint_ptr fep;
              fep = AVStreams::FlowEndPoint::_narrow (fep_obj,
                                                      ACE_TRY_ENV);
              ACE_TRY_CHECK;
              b_feps [i] = fep;
            }
          // Now go thru the list of flow endpoint and match them.
          // uses the first match policy.
          for (i=0;i<a_feps.length ();i++)
            {
              for (u_int j=0;j<b_feps.length ();j++)
                {
                  AVStreams::FlowEndPoint_ptr fep_a =
                    AVStreams::FlowEndPoint::_narrow (a_feps [i],ACE_TRY_ENV);
                  ACE_TRY_CHECK;
                  AVStreams::FlowEndPoint_ptr fep_b =
                    AVStreams::FlowEndPoint::_narrow (b_feps [j],ACE_TRY_ENV);

                  if (fep_b->get_connected_fep () != 0)
                    {
                      if (fep_a->is_fep_compatible (fep_b,
                                                   ACE_TRY_ENV) == 1)
                        {
                          // assume that flow names are same so that we
                          // can use either of them.
                          CORBA::Object_ptr flow_connection_obj;
                          AVStreams::FlowConnection_ptr flow_connection;
                          if ((flow_connection_obj
                               = this->get_flow_connection ((*a_flows)[i],ACE_TRY_ENV))!= 0)
                            {
                              flow_connection = AVStreams::FlowConnection::_narrow (flow_connection_obj,ACE_TRY_ENV);
                              ACE_TRY_CHECK;
                            }
                          else
                            {
                              TAO_FlowConnection *flowConnection;
                              ACE_NEW_RETURN (flowConnection,TAO_FlowConnection,0);
                              flow_connection = flowConnection->_this (ACE_TRY_ENV);
                              ACE_TRY_CHECK;
                            }
                          // make sure that a_feps is flow_producer and b_feps is flow_consumer
                          // There should be a way to find which flow endpoint is producer and which is consumer.
                          AVStreams::FlowProducer_ptr producer =
                            AVStreams::FlowProducer::_narrow (fep_a,ACE_TRY_ENV);
                          ACE_TRY_CHECK;
                          AVStreams::FlowConsumer_ptr consumer =
                            AVStreams::FlowConsumer::_narrow (fep_b,ACE_TRY_ENV);
                          ACE_TRY_CHECK;
                          // what should be passed to QoS?
                          flow_connection->connect (producer,consumer,the_qos [0],ACE_TRY_ENV);
                          fep_a->set_peer (flow_connection,fep_b,the_qos[0],ACE_TRY_ENV);
                          fep_b->set_peer (flow_connection,fep_a,the_qos[0],ACE_TRY_ENV);
                        }
                    }
                }
            }
        }
      else
        {
          // since the flow spec is not empty we need to query for the
          // flows mentioned on both the A and B sides.
          // not implemented yet.
        }
    }
  ACE_CATCHANY
    {
      // error was thrown because one of the streamendpoints is light profile.
      // Now connect the streams together
      this->sep_a_->connect (this->sep_b_.in (),
                                         the_qos,
                                         the_flows,
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

void
TAO_StreamCtrl::unbind (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

void
TAO_StreamCtrl::unbind_party (AVStreams::StreamEndPoint_ptr the_ep,
                              const AVStreams::flowSpec &the_spec,
                              CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_ep);
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

void
TAO_StreamCtrl::unbind_dev (AVStreams::MMDevice_ptr dev,
                            const AVStreams::flowSpec & the_spec,
                            CORBA::Environment &ACE_TRY_ENV)
{

}

AVStreams::VDev_ptr
TAO_StreamCtrl::get_related_vdev (AVStreams::MMDevice_ptr adev,
                                  AVStreams::StreamEndPoint_out sep,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  return AVStreams::VDev::_nil ();
}

CORBA::Boolean
TAO_StreamCtrl::modify_QoS (AVStreams::streamQoS &the_qos,
                            const AVStreams::flowSpec &the_spec,
                            CORBA::Environment &ACE_TRY_ENV)
{
  if (this->mcastconfigif_ != 0)
    {
      // call modify_Qos on the root VDev which is the mcast configif.
      ACE_DEBUG ((LM_DEBUG,"Cannot Modify the Qos for multipoint streams\n"));
    }
  else
    {
      TAO_Basic_StreamCtrl::modify_QoS (the_qos,the_spec,ACE_TRY_ENV);
    }
  return 1;
}

// ----------------------------------------------------------------------
// TAO_MCastConfigIf
// ----------------------------------------------------------------------

CORBA::Boolean
TAO_MCastConfigIf::set_peer (CORBA::Object_ptr peer,
                             AVStreams::streamQoS & the_qos,
                             const AVStreams::flowSpec & the_spec,
                             CORBA::Environment &ACE_TRY_ENV)
{
  return 0;
}

void
TAO_MCastConfigIf::configure (const CosPropertyService::Property & a_configuration,
                              CORBA::Environment &ACE_TRY_ENV)
{
}

void
TAO_MCastConfigIf::set_initial_configuration (const CosPropertyService::Properties & initial,
                                              CORBA::Environment &ACE_TRY_ENV)
{
}

void
TAO_MCastConfigIf::set_format (const char * flowName,
                               const char * format_name,
                               CORBA::Environment &ACE_TRY_ENV)
{
}

void
TAO_MCastConfigIf::set_dev_params (const char * flowName,
                                   const CosPropertyService::Properties & new_params,
                                   CORBA::Environment &ACE_TRY_ENV)
{
}

// ----------------------------------------------------------------------
// TAO_Base_StreamEndPoint
// ----------------------------------------------------------------------

TAO_Base_StreamEndPoint::~TAO_Base_StreamEndPoint (void)
{
}

int
TAO_Base_StreamEndPoint::handle_close (void)
{
  // This method should not be defined, but EGCS complains endlessly
  // about it.
  return -1;
}


// ----------------------------------------------------------------------
// TAO_Client_StreamEndPoint
// ----------------------------------------------------------------------

TAO_Client_StreamEndPoint::TAO_Client_StreamEndPoint (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) TAO_Client_StreamEndPoint::TAO_Client_StreamEndPoint: created\n"));
}

CORBA::Boolean
TAO_Client_StreamEndPoint::connect (AVStreams::StreamEndPoint_ptr responder,
                                    AVStreams::streamQoS &qos_spec,
                                    const AVStreams::flowSpec &the_spec,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Boolean retv = 0;

  ACE_TRY
    {
      AVStreams::flowSpec flow_spec (the_spec);
      this->handle_preconnect (flow_spec);

      // Use the base class implementation of connect
      responder->request_connection (this->_this (ACE_TRY_ENV),
                                     0,
                                     qos_spec,
                                     flow_spec,
                                     ACE_TRY_ENV);

      ACE_TRY_CHECK;

      // Make the upcall to the app
      retv = this->handle_postconnect (flow_spec);
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return retv;
}


// @@ Need to throw not-supported exception here
CORBA::Boolean
TAO_Client_StreamEndPoint::multiconnect (AVStreams::streamQoS &the_qos,
                                         AVStreams::flowSpec &the_spec,
                                         CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

// Multicast is not supported yet.
CORBA::Boolean
TAO_Client_StreamEndPoint::connect_leaf (AVStreams::StreamEndPoint_B_ptr the_ep,
                                         AVStreams::streamQoS &the_qos,
                                         const AVStreams::flowSpec &the_flows,
                                         CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_ep);
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (the_flows);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

// Multicast not supported yet.
void
TAO_Client_StreamEndPoint::disconnect_leaf (AVStreams::StreamEndPoint_B_ptr the_ep,
                                            const AVStreams::flowSpec &theSpec,
                                            CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_ep);
  ACE_UNUSED_ARG (theSpec);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

TAO_Client_StreamEndPoint::~TAO_Client_StreamEndPoint (void)
{
}

// ----------------------------------------------------------------------
// TAO_Server_StreamEndPoint
// ----------------------------------------------------------------------

TAO_Server_StreamEndPoint::TAO_Server_StreamEndPoint (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n(%P|%t) TAO_Server_StreamEndPoint::TAO_Server_StreamEndPoint: created"));
}


// Called by streamctrl, requesting us to call request_connection
// on the responder (to initiate a connection)
CORBA::Boolean
TAO_Server_StreamEndPoint::connect (AVStreams::StreamEndPoint_ptr responder,
                                    AVStreams::streamQoS &qos_spec,
                                    const AVStreams::flowSpec &the_spec,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (responder);
  ACE_UNUSED_ARG (qos_spec);
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  ACE_ERROR_RETURN ((LM_ERROR,
                     "(%P|%t) Calling TAO_Server_StreamEndPoint::connect"
                     " is not compatible with the spec!"
                     "\n"),
                    0);
}

CORBA::Boolean
TAO_Server_StreamEndPoint::request_connection (AVStreams::StreamEndPoint_ptr initiator,
                                               CORBA::Boolean is_mcast,
                                               AVStreams::streamQoS &qos,
                                               AVStreams::flowSpec &the_spec,
                                               CORBA::Environment &ACE_TRY_ENV)

{
  int result = 0;
  ACE_TRY
    {
      // Use the base class implementation of request_connection
      TAO_StreamEndPoint::request_connection (initiator,
                                              is_mcast,
                                              qos,
                                              the_spec,
                                              ACE_TRY_ENV);

      ACE_TRY_CHECK;
      // Make the upcall to the app
      result = this->handle_connection_requested (the_spec,ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_Server_StreamEndpoint::request_connection");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (1);
  return result;
}

CORBA::Boolean
TAO_Server_StreamEndPoint::multiconnect (AVStreams::streamQoS &the_qos,
                                         AVStreams::flowSpec &the_spec,
                                         CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

TAO_Server_StreamEndPoint::~TAO_Server_StreamEndPoint (void)
{
}


// ----------------------------------------------------------------------
// TAO_StreamEndPoint
// ----------------------------------------------------------------------

// constructor.

TAO_StreamEndPoint::TAO_StreamEndPoint (void)
  :flow_count_ (1)
{
  //  this->handle_open ();
}

// Stop the physical flow of data on the stream
// Empty the_spec --> apply to all flows

void
TAO_StreamEndPoint::stop (const AVStreams::flowSpec &the_spec,
                          CORBA::Environment &ACE_TRY_ENV)
{
  // Make the upcall into the app
  this->handle_stop (the_spec, ACE_TRY_ENV);
}

// Start the physical flow of data on the stream
// Empty the_spec --> apply to all flows

void
TAO_StreamEndPoint::start (const AVStreams::flowSpec &flow_spec,
                           CORBA::Environment &ACE_TRY_ENV)
{
  // Make the upcall into the app
  this->handle_start (flow_spec, ACE_TRY_ENV);
}

// Close the connection

void
TAO_StreamEndPoint::destroy (const AVStreams::flowSpec &the_spec,
                                 CORBA::Environment &ACE_TRY_ENV)
{
//   // Make the upcall into the app
//   this->handle_destroy (the_spec, ACE_TRY_ENV);
  //
  // Remove self from POA.  Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.
  //
  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var id = poa->servant_to_id (this,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;
}

// Called by our peer endpoint, requesting us to establish
// a connection

CORBA::Boolean
TAO_StreamEndPoint::request_connection (AVStreams::StreamEndPoint_ptr initiator,
                                        CORBA::Boolean is_mcast,
                                        AVStreams::streamQoS &qos,
                                        AVStreams::flowSpec &the_spec,
                                        CORBA::Environment &)
{
  ACE_UNUSED_ARG (initiator);
  ACE_UNUSED_ARG (is_mcast);
  ACE_UNUSED_ARG (qos);

  ACE_DEBUG ((LM_DEBUG, "\n(%P|%t) TAO_StreamEndPoint::request_connection called"));
  ACE_DEBUG ((LM_DEBUG,
              "\n(%P|%t) TAO_StreamEndPoint::request_connection: "
              "flowspec has length = %d"
              "and the strings are:",
              the_spec.length ()));
  for (u_int i = 0; i < the_spec.length (); i++)
    ACE_DEBUG ((LM_DEBUG,
                the_spec [i]));

  return 0;
}

// @@ Need to throw not-supported exception here

CORBA::Boolean
TAO_StreamEndPoint::modify_QoS (AVStreams::streamQoS &new_qos,
                                const AVStreams::flowSpec &the_flows,
                                CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (new_qos);
  ACE_UNUSED_ARG (the_flows);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

// @@ Need to throw not-supported exception here

CORBA::Boolean
TAO_StreamEndPoint::set_protocol_restriction (const AVStreams::protocolSpec &the_pspec,
                                              CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      CORBA::Any protocol_restriction_any;

      protocol_restriction_any <<= the_pspec;
      this->define_property ("ProtocolRestriction",
                             protocol_restriction_any,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_StreamEndPoint::set_protocol_restriction");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}


void
TAO_StreamEndPoint::disconnect (const AVStreams::flowSpec &the_spec,
                                CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

// @@ Need to throw not-supported exception here

void
TAO_StreamEndPoint::set_FPStatus (const AVStreams::flowSpec &the_spec,
                                  const char *fp_name,
                                  const CORBA::Any &fp_settings,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (fp_name);
  ACE_UNUSED_ARG (fp_settings);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}


CORBA::Object_ptr
TAO_StreamEndPoint::get_fep (const char *flow_name,
                             CORBA::Environment &)
{
  TAO_String_Hash_Key fep_name_key (flow_name);
  FlowEndPoint_Map::ENTRY *fep_entry = 0;
  if (this->fep_map_.find (fep_name_key,fep_entry) == 0)
    return fep_entry->int_id_;
  return 0;
}


char *
TAO_StreamEndPoint::add_fep (CORBA::Object_ptr the_fep,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  char *flow_name = 0;
  ACE_TRY_EX (flow_name);
  {
    ACE_NEW_RETURN (flow_name,char [BUFSIZ],0);
    CORBA::Any_ptr flow_name_any;
    AVStreams::FlowEndPoint_ptr fep =
      AVStreams::FlowEndPoint::_narrow (the_fep,ACE_TRY_ENV);
    ACE_TRY_CHECK_EX (flow_name);
    flow_name_any = fep->get_property_value ("FlowName",ACE_TRY_ENV);
    ACE_TRY_CHECK_EX (flow_name);
    *flow_name_any >>= flow_name;
  }
  ACE_CATCHANY
    {
      // exception implies the flow name is not defined and is system generated.
      ACE_OS::sprintf (flow_name,"flow%d",flow_num_++);
    }
  ACE_ENDTRY;
  // Add it to the sequence of flowNames supported.
  // put the flowname and the flowendpoint in a hashtable.
  TAO_String_Hash_Key fep_name_key (flow_name);
  if (this->fep_map_.bind (fep_name_key,the_fep) != 0)
    {
      ACE_THROW_RETURN (AVStreams::streamOpFailed (),0);
    }
  ACE_TRY_EX (flows)
    {
      // increment the flow count.
      this->flow_count_++;
      this->flows_.length (this->flow_count_);
      this->flows_ [this->flow_count_-1] = flow_name;
      // define/modify the "Flows" property.
      CORBA::Any flows_any;
      flows_any <<= this->flows_;
      this->define_property ("Flows",
                             flows_any,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK_EX (flows);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_StreamEndPoint::add_fep");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return flow_name;
}


void
TAO_StreamEndPoint::remove_fep (const char *flow_name,
                                CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      TAO_String_Hash_Key fep_name_key (flow_name);
      CORBA::Object_ptr fep_entry = 0;
      // Remove the fep from the hash table.
      if (this->fep_map_.unbind (fep_name_key,fep_entry)!= 0)
        TAO_THROW_ENV (AVStreams::streamOpFailed (),ACE_TRY_ENV);
      // redefine the "Flows" property
      AVStreams::flowSpec new_flows (this->flows_.length ());
      for (u_int i=0,j=0 ; i <this->flows_.length (); i++)
        if (ACE_OS::strcmp (flow_name,this->flows_[i]) != 0)
          new_flows[j++] = this->flows_[i];

      CORBA::Any flows;
      flows <<= new_flows;
      this->flows_ = new_flows;
      this->define_property ("Flows",
                             flows,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_StreamEndPoint::remove_fep");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// @@ Need to throw not-supported exception here

void
TAO_StreamEndPoint::set_negotiator (AVStreams::Negotiator_ptr new_negotiator,
                                        CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      CORBA::Any negotiator;
      negotiator <<= new_negotiator;
      this->define_property ("Negotiator",
                             negotiator,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_StreamEndPoint::set_negotiator");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Sets the public key used for this streamendpoint.

void
TAO_StreamEndPoint::set_key (const char *flow_name,
                             const AVStreams::key & the_key,
                             CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {

      CORBA::Any PublicKey;
      PublicKey <<= the_key;
      char PublicKey_property [BUFSIZ];
      ACE_OS::sprintf (PublicKey_property,"%s_PublicKey",flow_name);
      this->define_property (PublicKey_property,
                             PublicKey,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_StreamEndPoint::set_key");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Set the source id.

void
TAO_StreamEndPoint::set_source_id (CORBA::Long source_id,
                                   CORBA::Environment &)
{
  this->source_id_ = source_id;
}


TAO_StreamEndPoint::~TAO_StreamEndPoint (void)
{
  //this->handle_close ();
}

// ----------------------------------------------------------------------
// TAO_VDev
// ----------------------------------------------------------------------

TAO_VDev::TAO_VDev (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) TAO_VDev::TAO_VDev: created\n"));
}

// StreamCtrl will call this to give us a reference to itself, and to
// our peer vdev..
CORBA::Boolean
TAO_VDev::set_peer (AVStreams::StreamCtrl_ptr the_ctrl,
                    AVStreams::VDev_ptr the_peer_dev,
                    AVStreams::streamQoS &the_qos,
                    const AVStreams::flowSpec &the_spec,
                    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (the_spec);

  CORBA::Boolean result = 0;
  ACE_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) TAO_VDev::set_peer: called"));

      CORBA::String_var ior = TAO_ORB_Core_instance ()->orb ()->object_to_string (the_peer_dev,
                                                                                  ACE_TRY_ENV);

      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) TAO_VDev::set_peer: my peer is %s\n",
                  ior.in ()));

      CORBA::Any anyval;
      anyval <<= ior.in ();
      this->define_property ("Related_VDev",
                             anyval,
                             ACE_TRY_ENV);

      ACE_TRY_CHECK;

      this->streamctrl_ = the_ctrl;
      this->peer_ = the_peer_dev;

      CORBA::Any_ptr anyptr;
      CORBA::String media_ctrl_ior;
      anyptr = this->peer_->get_property_value ("Related_MediaCtrl",
                                                ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (anyptr != 0)
        {
          *anyptr >>= media_ctrl_ior;
          ACE_DEBUG ((LM_DEBUG,"(%P|%t)The Media Control IOR is %s\n",
                      media_ctrl_ior));
        }
      CORBA::Object_ptr media_ctrl_obj =
        TAO_ORB_Core_instance ()->orb ()->string_to_object
        (media_ctrl_ior,ACE_TRY_ENV);
      ACE_TRY_CHECK;

      result = this->set_media_ctrl (media_ctrl_obj,ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_Server_StreamEndpoint::request_connection");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return result;
}

CORBA::Boolean
TAO_VDev::set_media_ctrl (CORBA::Object_ptr media_ctrl,
                          CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (media_ctrl);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 1;
}

// Multicast is not supported yet.
CORBA::Boolean
TAO_VDev::set_Mcast_peer (AVStreams::StreamCtrl_ptr the_ctrl,
                          AVStreams::MCastConfigIf_ptr a_mcastconfigif,
                          AVStreams::streamQoS &the_qos,
                          const AVStreams::flowSpec &the_spec,
                          CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_ctrl);
  ACE_UNUSED_ARG (a_mcastconfigif);
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

// applications should override this to handle configuration changes.
void
TAO_VDev::configure (const CosPropertyService::Property &the_config_mesg,
                     CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_config_mesg);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

// sets the media format used for the flowname as a property.
void
TAO_VDev::set_format (const char *flowName,
                      const char *format_name,
                      CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      if (flowName == 0 || format_name == 0)
        ACE_ERROR ((LM_ERROR,"TAO_VDev::set_format: flowName or format_name is null\n"));
      char format_property [BUFSIZ];
      ACE_OS::sprintf (format_property,"%s_currFormat",flowName);
      CORBA::Any format;
      format <<= format_name;
      this->define_property (format_property,
                             format,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_VDev::set_format");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
  return;
}

// sets the device parameters for the flowname as a property.
void
TAO_VDev::set_dev_params (const char *flowName,
                          const CosPropertyService::Properties &new_params,
                          CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {

      if (flowName == 0)
        ACE_ERROR ((LM_ERROR,"TAO_VDev::set_dev_params:flowName is null\n"));
      char devParams_property[BUFSIZ];
      ACE_OS::sprintf (devParams_property,"%s_devParams",flowName);
      CORBA::Any devParams;
      devParams <<= new_params;
      this->define_property (devParams_property,
                             devParams,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_VDev::set_dev_params");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
  return;
}

// QoS Modification should be handled by the application currently.
CORBA::Boolean
TAO_VDev::modify_QoS (AVStreams::streamQoS &the_qos,
                      const AVStreams::flowSpec &the_spec,
                      CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

TAO_VDev::~TAO_VDev (void)
{
}

// ----------------------------------------------------------------------
// TAO_MMDevice
// ----------------------------------------------------------------------


TAO_MMDevice::TAO_MMDevice (TAO_AV_Endpoint_Strategy *endpoint_strategy)
  : endpoint_strategy_ (endpoint_strategy)
{
}

// create a streamctrl which is colocated with me, use that streamctrl
// to bind the peer_device with me.
AVStreams::StreamCtrl_ptr
TAO_MMDevice::bind (AVStreams::MMDevice_ptr peer_device,
                    AVStreams::streamQoS &the_qos,
                    CORBA::Boolean_out is_met,
                    const AVStreams::flowSpec &the_spec,
                    CORBA::Environment &ACE_TRY_ENV)
{
  AVStreams::StreamCtrl_ptr streamctrl (AVStreams::StreamCtrl::_nil ());
  ACE_TRY
    {
      ACE_UNUSED_ARG (is_met);
      TAO_StreamCtrl *stream_ctrl;
      ACE_NEW_RETURN (stream_ctrl,
                      TAO_StreamCtrl,
                      0);
      stream_ctrl->bind_devs (peer_device,
                              AVStreams::MMDevice::_duplicate (this->_this (ACE_TRY_ENV)),
                              the_qos,
                              the_spec,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
      streamctrl = stream_ctrl->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_MMDevice::bind");
      return streamctrl;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (streamctrl);
  return streamctrl;
}

// Multicast is not supported yet.
AVStreams::StreamCtrl_ptr
TAO_MMDevice::bind_mcast (AVStreams::MMDevice_ptr first_peer,
                          AVStreams::streamQoS &the_qos,
                          CORBA::Boolean_out is_met,
                          const AVStreams::flowSpec &the_spec,
                          CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (first_peer);
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (is_met);
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

AVStreams::StreamEndPoint_A_ptr
TAO_MMDevice::create_A (AVStreams::StreamCtrl_ptr the_requester,
                                   AVStreams::VDev_out the_vdev,
                                   AVStreams::streamQoS &the_qos,
                                   CORBA::Boolean_out met_qos,
                                   char *&named_vdev,
                                   const AVStreams::flowSpec &the_spec,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_requester);
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (met_qos);
  ACE_UNUSED_ARG (named_vdev);
  ACE_UNUSED_ARG (the_spec);

  AVStreams::StreamEndPoint_A_ptr ptr (AVStreams::StreamEndPoint_A::_nil ());
  ACE_TRY
    {
      // In full profile application can override this to use FDevs.

      if (this->endpoint_strategy_->create_A (ptr,
                                              the_vdev,
                                              ACE_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Error in create_A\n"),
                          0);

      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_MMDevice::create_A");
      return ptr;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (ptr);
  return ptr;
}


AVStreams::StreamEndPoint_B_ptr
TAO_MMDevice::create_B (AVStreams::StreamCtrl_ptr the_requester,
                        AVStreams::VDev_out the_vdev,
                        AVStreams::streamQoS &the_qos,
                        CORBA::Boolean_out met_qos,
                        char *&named_vdev,
                        const AVStreams::flowSpec &the_spec,
                        CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_requester);
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (met_qos);
  ACE_UNUSED_ARG (named_vdev);
  ACE_UNUSED_ARG (the_spec);

  // In full profile application can override this to use FDevs.
  AVStreams::StreamEndPoint_B_ptr ptr (AVStreams::StreamEndPoint_B::_nil ());

  ACE_TRY
    {
  if (this->endpoint_strategy_->create_B (ptr,
                                          the_vdev,
                                          ACE_TRY_ENV) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Error in create_B\n"),
                      0);

  ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_MMDevice::create_B");
      return ptr;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (ptr);
  return ptr;
}

// destroys the streamendpoint and the Vdev.
void
TAO_MMDevice::destroy (AVStreams::StreamEndPoint_ptr the_ep,
                       const char *vdev_name,
                       CORBA::Environment &ACE_TRY_ENV)
{
  // Remove self from POA.  Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.
  //
  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var id = poa->servant_to_id (this,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;
}

// Adds the fdev object to the MMDevice.
char *
TAO_MMDevice::add_fdev (CORBA::Object_ptr the_fdev,
                        CORBA::Environment &ACE_TRY_ENV)
{
  char *flow_name = 0;
  ACE_TRY_EX (flow_name)
    {
      ACE_NEW_RETURN (flow_name,char [BUFSIZ],0);
      CORBA::Any_ptr flow_name_any;
      AVStreams::FDev_ptr fdev =
        AVStreams::FDev::_narrow (the_fdev,ACE_TRY_ENV);
      ACE_TRY_CHECK_EX (flow_name);
      flow_name_any = fdev->get_property_value ("Flow",ACE_TRY_ENV);
      ACE_TRY_CHECK_EX (flow_name);
      *flow_name_any >>= flow_name;
    }
  ACE_CATCHANY
    {
      // exception implies the flow name is not defined and is system generated.
      ACE_OS::sprintf (flow_name,"flow%d",flow_num_++);
    }
  ACE_ENDTRY;
  // Add it to the sequence of flowNames supported.
  // put the flowname and the fdev in a hashtable.
  TAO_String_Hash_Key fdev_name_key (flow_name);
  if (this->fdev_map_.bind (fdev_name_key,the_fdev) != 0)
    ACE_THROW_RETURN (AVStreams::streamOpFailed (),0);

  // increment the flow count.
  this->flow_count_++;
  this->flows_.length (this->flow_count_);
  this->flows_ [this->flow_count_-1] = flow_name;
  // define/modify the "Flows" property.
  CORBA::Any flows_any;
  flows_any <<= this->flows_;
  ACE_TRY_EX (flows)
    {
      this->define_property ("Flows",
                             flows_any,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK_EX (flows);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_MMDevice::add_fdev");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return flow_name;
}

// Gets the FDev object associated with this flow.
CORBA::Object_ptr
TAO_MMDevice::get_fdev (const char *flow_name,
                        CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  TAO_String_Hash_Key fdev_name_key (flow_name);
  FDev_Map::ENTRY *fdev_entry = 0;
  if (this->fdev_map_.find (fdev_name_key,fdev_entry) == 0)
    return fdev_entry->int_id_;
  return 0;
}

// Removes the fdev from this MMDevice.
void
TAO_MMDevice::remove_fdev (const char *flow_name,
                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      TAO_String_Hash_Key fdev_name_key (flow_name);
      CORBA::Object_ptr fdev_entry = 0;
      // Remove the fep from the hash table.
      if (this->fdev_map_.unbind (flow_name,fdev_entry)!= 0)
        ACE_THROW (AVStreams::streamOpFailed ());

      AVStreams::flowSpec new_flows (this->flows_.length ());
      for (u_int i=0,j=0 ; i <this->flows_.length (); i++)
        if (ACE_OS::strcmp (flow_name,this->flows_[i]) != 0)
          new_flows[j++] = this->flows_[i];

      CORBA::Any flows;
      flows <<= new_flows;
      this->flows_ = new_flows;
      this->define_property ("Flows",
                             flows,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_MMDevice::remove_fdev");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// destructor.
TAO_MMDevice::~TAO_MMDevice (void)
{
}

//------------------------------------------------------------------
// TAO_FlowConnection
//------------------------------------------------------------------

// default constructor.
TAO_FlowConnection::TAO_FlowConnection (void)
  :producer_ (0),
   consumer_ (0),
   fp_name_ (0)
{
}

// stop this flow.
void
TAO_FlowConnection::stop (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

// start this flow.
void
TAO_FlowConnection::start (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

// destroy this flow.
void
TAO_FlowConnection::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  // Remove self from POA.  Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.
  //
  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var id = poa->servant_to_id (this,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;
}

// modify the QoS for this flow.
CORBA::Boolean
TAO_FlowConnection::modify_QoS (AVStreams::QoS & new_qos,
                                CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (new_qos);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

// use the specified flow protocol for this flow.
CORBA::Boolean
TAO_FlowConnection::use_flow_protocol (const char * fp_name,
                                       const CORBA::Any & fp_settings,
                                       CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (fp_settings);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  this->fp_name_ = (char *)fp_name;
  return 1;
}

void
TAO_FlowConnection::push_event (const AVStreams::streamEvent & the_event,
                                CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_event);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

CORBA::Boolean
TAO_FlowConnection::connect_devs (AVStreams::FDev_ptr a_party,
                                  AVStreams::FDev_ptr b_party,
                                  AVStreams::QoS & the_qos,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (a_party);
  ACE_UNUSED_ARG (b_party);
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

// connect the producer and the consumer
CORBA::Boolean
TAO_FlowConnection::connect (AVStreams::FlowProducer_ptr flow_producer,
                             AVStreams::FlowConsumer_ptr flow_consumer,
                             AVStreams::QoS & the_qos,
                             CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      this->producer_ = flow_producer;
      this->consumer_ = flow_consumer;

      this->producer_->set_peer (this->_this (ACE_TRY_ENV),
                                 this->consumer_,
                                 the_qos,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->consumer_->set_peer (this->_this (ACE_TRY_ENV),
                                 this->producer_,
                                 the_qos,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      char *consumer_address =
        this->consumer_->go_to_listen (the_qos,
                                       0,// false for is_mcast
                                       this->producer_,
                                       this->fp_name_,
                                       ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->producer_->connect_to_peer (the_qos,
                                        consumer_address,
                                        this->fp_name_,
                                        ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FlowConnection::connect");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}


CORBA::Boolean
TAO_FlowConnection::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return  0;
}

CORBA::Boolean
TAO_FlowConnection::add_producer (AVStreams::FlowProducer_ptr flow_producer,
                                  AVStreams::QoS & the_qos,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  this->producer_ = flow_producer;
  return 1;
}

CORBA::Boolean
TAO_FlowConnection::add_consumer (AVStreams::FlowConsumer_ptr flow_consumer,
                                  AVStreams::QoS & the_qos,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  this->consumer_ = flow_consumer;
  return 1;
}

CORBA::Boolean
TAO_FlowConnection::drop (AVStreams::FlowEndPoint_ptr target,
                          CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (target);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

// -----------------------------------------------------------------
// TAO_FlowEndPoint
// -----------------------------------------------------------------

//default constructor.
TAO_FlowEndPoint::TAO_FlowEndPoint (void)
  :related_sep_ (0),
   related_flow_connection_ (0)
{
}

// used by one flowconnection so that multiple connections cant use
// the same flowendpoint.
CORBA::Boolean
TAO_FlowEndPoint::lock (CORBA::Environment &ACE_TRY_ENV)
{
  // lock the current flowendpoint
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

// unlocks the flowendpoint ,becomes free to be used in another flow.
void
TAO_FlowEndPoint::unlock (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

// The start,stop and destroy are to be handled by the application.
void
TAO_FlowEndPoint::stop (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

void
TAO_FlowEndPoint::start (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

void
TAO_FlowEndPoint::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  // Remove self from POA.  Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.
  //
  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var id = poa->servant_to_id (this,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;
}

AVStreams::StreamEndPoint_ptr
TAO_FlowEndPoint::related_sep (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return this->related_sep_;
}

void
TAO_FlowEndPoint::related_sep (AVStreams::StreamEndPoint_ptr related_sep,
                               CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  this->related_sep_ = related_sep;
}

AVStreams::FlowConnection_ptr
TAO_FlowEndPoint::related_flow_connection (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return this->related_flow_connection_;
}

void
TAO_FlowEndPoint::related_flow_connection (AVStreams::FlowConnection_ptr related_flow_connection,
                                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  this->related_flow_connection_ = related_flow_connection;
}

// returns the connected peer for this flow
AVStreams::FlowEndPoint_ptr
TAO_FlowEndPoint::get_connected_fep (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return this->peer_fep_;
}

CORBA::Boolean
TAO_FlowEndPoint::use_flow_protocol (const char * fp_name,
                                     const CORBA::Any & fp_settings,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (fp_settings);
  ACE_TRY
    {
      // Define the property called FlowProtocol
      CORBA::Any flowname_property;
      flowname_property <<= fp_name;
      this->define_property ("FlowProtocol",
                             flowname_property,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FlowEndPoint::use_flow_protocol");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

void
TAO_FlowEndPoint::set_format (const char * format,
                              CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      // make this a property so that is_fep_compatible can query this and
      // check if 2 flowendpoints are compatible.
      CORBA::Any format_val;
      format_val <<= format;
      this->define_property ("Format",
                             format_val,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FlowEndpoint::set_format");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
TAO_FlowEndPoint::set_dev_params (const CosPropertyService::Properties & new_settings,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      CORBA::Any DevParams_property;
      DevParams_property <<= new_settings;
      this->define_property ("DevParams",
                             DevParams_property,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FlowEndPoint::set_dev_params");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
TAO_FlowEndPoint::set_protocol_restriction (const AVStreams::protocolSpec & the_spec,
                                            CORBA::Environment &ACE_TRY_ENV)


{
  ACE_TRY
    {
      CORBA::Any AvailableProtocols_property;
      AvailableProtocols_property <<= the_spec;
      this->define_property ("AvailableProtocols",
                             AvailableProtocols_property,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FlowEndpoint::set_protocol_restriction");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

CORBA::Boolean
TAO_FlowEndPoint::is_fep_compatible (AVStreams::FlowEndPoint_ptr peer_fep,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      // check whether the passed flowendpoint is compatible with this flowendpoint.
      // should we check for the availableFormats and choose one format.
      // get my format value
      CORBA::Any_ptr format_ptr;
      CORBA::String my_format,peer_format;
      format_ptr = this->get_property_value ("Format",
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (format_ptr != 0)
        *format_ptr >>= my_format;
      else
        // property is not defined
        ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) TAO_FlowEndPoint::is_fep_compatible"),0);

      // get my peer's format value

      format_ptr = peer_fep->get_property_value ("Format",
                                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (format_ptr != 0)
        *format_ptr >>= peer_format;
      else
        // property is not defined
        ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) TAO_FlowEndPoint::is_fep_compatible"),0);

      // can we use strcmp??
      if (ACE_OS::strcmp (my_format,peer_format) != 0)
        return 0;
      // since formats are same, check for a common protocol
      CORBA::Any* AvailableProtocols_ptr;
      AVStreams::protocolSpec *my_protocolSpec,*peer_protocolSpec;

      AvailableProtocols_ptr = this->get_property_value ("AvailableProtocols",
                                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (AvailableProtocols_ptr != 0)
        *AvailableProtocols_ptr >>= my_protocolSpec;
      else
        ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) TAO_FlowEndPoint::is_fep_compatible"),0);

      AvailableProtocols_ptr = peer_fep->get_property_value ("AvailableProtocols",
                                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (AvailableProtocols_ptr != 0)
        *AvailableProtocols_ptr >>= peer_protocolSpec;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FlowEndPoint::is_fep_compatible");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

CORBA::Boolean
TAO_FlowEndPoint::set_peer (AVStreams::FlowConnection_ptr the_fc,
                            AVStreams::FlowEndPoint_ptr the_peer_fep,
                            AVStreams::QoS & the_qos,
                            CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_fc);
  ACE_UNUSED_ARG (the_peer_fep);
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  this->peer_fep_ = the_peer_fep;
  return 1;
}

CORBA::Boolean
TAO_FlowEndPoint::set_Mcast_peer (AVStreams::FlowConnection_ptr the_fc,
                                  AVStreams::MCastConfigIf_ptr a_mcastconfigif,
                                  AVStreams::QoS & the_qos,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_fc);
  ACE_UNUSED_ARG (a_mcastconfigif);
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

char *
TAO_FlowEndPoint::go_to_listen (AVStreams::QoS & the_qos,
                               CORBA::Boolean is_mcast,
                               AVStreams::FlowProducer_ptr peer,
                               char *& flowProtocol,
                               CORBA::Environment &ACE_TRY_ENV)
{
  return this->handle_go_to_listen (the_qos,is_mcast,peer,flowProtocol,ACE_TRY_ENV);
}

char *
TAO_FlowEndPoint::handle_go_to_listen (AVStreams::QoS & the_qos,
                               CORBA::Boolean is_mcast,
                               AVStreams::FlowProducer_ptr peer,
                               char *& flowProtocol,
                               CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (is_mcast);
  ACE_UNUSED_ARG (peer);
  ACE_UNUSED_ARG (flowProtocol);
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  return 0;
}

CORBA::Boolean
TAO_FlowEndPoint::connect_to_peer (AVStreams::QoS & the_qos,
                                   const char * address,
                                   const char * use_flow_protocol,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  // Right now since the A/V framework doesnt bother about the
  // protocols we leave it to the application to handle the connection
  // to its peer. When A/V Streams implements common protocol
  // interaction like UDP and TCP this will be handled by the
  // framework.

  return this->handle_connect_to_peer (the_qos,address,use_flow_protocol,ACE_TRY_ENV);
}

CORBA::Boolean
TAO_FlowEndPoint::handle_connect_to_peer (AVStreams::QoS & the_qos,
                                          const char * address,
                                          const char * use_flow_protocol,
                                          CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (address);
  ACE_UNUSED_ARG (use_flow_protocol);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}


// ------------------------------------------------------------
// TAO_FlowProducer class
// ------------------------------------------------------------

//default constructor
TAO_FlowProducer::TAO_FlowProducer (void)
{
}

//  multicast is currently not supported
char *
TAO_FlowProducer::connect_mcast (AVStreams::QoS & the_qos,
                                           CORBA::Boolean_out is_met,
                                           const char * address,
                                           const char * use_flow_protocol,
                                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (is_met);
  ACE_UNUSED_ARG (address);
  ACE_UNUSED_ARG (use_flow_protocol);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

// gets the reverse channel for feedback.
char *
TAO_FlowProducer::get_rev_channel (const char * pcol_name,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (pcol_name);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

// sets the key for this flow.
void
TAO_FlowProducer::set_key (const AVStreams::key & the_key,
                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      CORBA::Any anyval;
      anyval <<= the_key;
      this->define_property ("PublicKey",
                             anyval,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FlowProducer::set_key");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// source id to be used to distinguish this source from others.
void
TAO_FlowProducer::set_source_id (CORBA::Long source_id,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  this->source_id_ = source_id;
}

// ------------------------------------------------------------
// TAO_FlowConsumer
// ------------------------------------------------------------


// default constructor.
TAO_FlowConsumer::TAO_FlowConsumer (void)
{
}

// ------------------------------------------------------------
// TAO_FDev
// ------------------------------------------------------------

// default constructor
TAO_FDev::TAO_FDev (void)
  :producer_ (0),
   consumer_ (0)
{
}

AVStreams::FlowProducer_ptr
TAO_FDev::create_producer (AVStreams::FlowConnection_ptr the_requester,
                           AVStreams::QoS & the_qos,
                           CORBA::Boolean_out met_qos,
                           char *& named_fdev,
                           CORBA::Environment &ACE_TRY_ENV)
{
  // call the bridge method.
  this->producer_ = this->make_producer (the_requester,
                                         the_qos,
                                         met_qos,
                                         named_fdev,
                                         ACE_TRY_ENV);
  return this->producer_;
}

// hook for the applications to override the creation process.
AVStreams::FlowProducer_ptr
TAO_FDev::make_producer (AVStreams::FlowConnection_ptr the_requester,
                         AVStreams::QoS & the_qos,
                         CORBA::Boolean_out met_qos,
                         char *& named_fdev,
                         CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_requester);
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (met_qos);
  ACE_UNUSED_ARG (named_fdev);

  // memory leak??
  TAO_FlowProducer *producer;
  ACE_NEW_RETURN (producer,
                  TAO_FlowProducer,
                  0);
  return producer->_this (ACE_TRY_ENV);
}

// hook for the applications to override the consumer creation.
AVStreams::FlowConsumer_ptr
TAO_FDev::make_consumer (AVStreams::FlowConnection_ptr the_requester,
                         AVStreams::QoS & the_qos,
                         CORBA::Boolean_out met_qos,
                         char *& named_fdev,
                         CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_requester);
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (met_qos);
  ACE_UNUSED_ARG (named_fdev);

  // memory leak??
  TAO_FlowConsumer *consumer;
  ACE_NEW_RETURN (consumer,
                  TAO_FlowConsumer,
                  0);
  return consumer->_this (ACE_TRY_ENV);
}

AVStreams::FlowConsumer_ptr
TAO_FDev::create_consumer (AVStreams::FlowConnection_ptr the_requester,
                           AVStreams::QoS & the_qos,
                           CORBA::Boolean_out met_qos,
                           char *& named_fdev,
                           CORBA::Environment &ACE_TRY_ENV)
{
  this->consumer_ = this->make_consumer (the_requester,
                                         the_qos,
                                         met_qos,
                                         named_fdev,
                                         ACE_TRY_ENV);
  return this->consumer_;
}

// not implemented yet.
AVStreams::FlowConnection_ptr
TAO_FDev::bind (AVStreams::FDev_ptr peer_device,
                AVStreams::QoS & the_qos,
                CORBA::Boolean_out is_met,
                CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (peer_device);
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (is_met);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

// multicast is not supported yet.
AVStreams::FlowConnection_ptr
TAO_FDev::bind_mcast (AVStreams::FDev_ptr first_peer,
                      AVStreams::QoS & the_qos,
                      CORBA::Boolean_out is_met,
                      CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (first_peer);
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (is_met);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

void
TAO_FDev::destroy (AVStreams::FlowEndPoint_ptr /* the_ep */,
                   const char * /* fdev_name */,
                   CORBA::Environment &ACE_TRY_ENV)
{
  // Remove self from POA.  Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.
  //
  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var id = poa->servant_to_id (this,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;
}

//------------------------------------------------------------
// TAO_Tokenizer
//------------------------------------------------------------
TAO_Tokenizer::TAO_Tokenizer (char *string, char delimiter)
  :token_array_ (10),
   count_ (0)
{
  this->parse (string,delimiter);
}

int
TAO_Tokenizer::parse (char *string,char delimiter)
{
  char delimiter_str [2] = {0,0};
  delimiter_str [0] = delimiter;
  char *token = ACE_OS::strtok (string,delimiter_str);
  int count=0,result;
  while (token != 0)
    {
      result = this->token_array_.set (token,count);
      if (result == -1)
        {
          this->token_array_.size (this->token_array_.size ()*2);
          result = this->token_array_.set (token,count);
          if (result == -1)
            ACE_ERROR_RETURN ((LM_ERROR,"TAO_Tokenizer::parse error"),-1);
        }
      token = ACE_OS::strtok (string,0);
      count++;
    }
  this->num_tokens_ = count;
  return 0;
}

char*
TAO_Tokenizer::token (void)
{
  if (count_ < num_tokens_)
    return this->token_array_[this->count_++];
  else
    return 0;
}

int
TAO_Tokenizer::num_tokens (void)
{
  return this->num_tokens_;
}

char *
TAO_Tokenizer::operator [] (size_t index) const
{
  if (index >= this->num_tokens_)
    return 0;
  return this->token_array_[index];
}

//------------------------------------------------------------
// TAO_FlowSpec_Entry
//------------------------------------------------------------

// constructor.
TAO_FlowSpec_Entry::TAO_FlowSpec_Entry (void)
  :sfp_ (0),
   address_ (0),
   format_ (0),
   direction_ (INVALID)
{
}

// Destructor.
TAO_FlowSpec_Entry::~TAO_FlowSpec_Entry (void)
{
}

int
TAO_FlowSpec_Entry::direction (void)
{
  return this->direction_;
}

TAO_SFP*
TAO_FlowSpec_Entry::flow_protocol (void)
{
  return this->sfp_;
}

ACE_Addr*
TAO_FlowSpec_Entry::carrier_protocol (void)
{
  return address_;
}

char*
TAO_FlowSpec_Entry::format (void)
{
  return this->format_;
}

int
TAO_Forward_FlowSpec_Entry::parse (char *flowSpec_entry)
{
  TAO_Tokenizer tokenizer (flowSpec_entry,'\\');
  if (this->parse_flow_protocol_string (tokenizer [FLOW_PROTOCOL]) < 0)
    return -1;
  this->set_direction (tokenizer [DIRECTION]);
  if (this->parse_address (tokenizer [ADDRESS]) < 0)
    return -1;
  this->format_ = tokenizer [FORMAT];
  this->flowname_ = tokenizer [FLOWNAME];
  return 0;
}

int
TAO_FlowSpec_Entry::set_direction (char *direction)
{
  if (ACE_OS::strcasecmp (direction,"in") == 0)
    this->direction_ = DIR_IN;
  else if (ACE_OS::strcasecmp (direction,"out") == 0)
    this->direction_ = DIR_OUT;
  else if (ACE_OS::strcasecmp (direction,"inout") == 0)
    this->direction_ = DIR_INOUT;
  return 0;
}

int
TAO_FlowSpec_Entry::parse_flow_protocol_string (char *flow_string)
{
  if (ACE_OS::strncasecmp (flow_string,"sfp",3) == 0)
    {
      // do some flow protocol processing.
    }
  return 0;
}

int
TAO_FlowSpec_Entry::parse_address (char *address)
{
  TAO_Tokenizer protocol_tokenizer (address,'=');
  this->protocol_ = protocol_tokenizer[0];
  TAO_Tokenizer address_tokenizer (protocol_tokenizer[1],';');
  char addr[BUFSIZ];
  ACE_OS::sprintf (addr,"%s:%s",address_tokenizer[0],address_tokenizer[1]);
  if (ACE_OS::strncasecmp (this->protocol_,"AAL",3) == 0)
    ACE_DEBUG ((LM_DEBUG,"ATM support not added yet\n"));
//      ACE_NEW_RETURN (this->address_,
//                      ACE_ATM_Addr (addr),
//                      -1);
  else // we assume everything else is INET addr.
    {
      ACE_NEW_RETURN (this->address_,
                      ACE_INET_Addr (addr),
                      -1);
    }
  return 0;
}

int
TAO_Reverse_FlowSpec_Entry::parse (char *flowSpec_entry)
{
  TAO_Tokenizer tokenizer (flowSpec_entry,'\\');
  if (this->parse_flow_protocol_string (tokenizer [FLOW_PROTOCOL]) < 0)
    return -1;
  this->set_direction (tokenizer [DIRECTION]);
  if (this->parse_address (tokenizer [ADDRESS]) < 0)
    return -1;
  this->format_ = tokenizer [FORMAT];
  this->flowname_ = tokenizer [FLOWNAME];
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<TAO_String_Hash_Key,CORBA::Object_ptr>;
template class ACE_Hash_Map_Manager<TAO_String_Hash_Key,CORBA::Object_ptr,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, CORBA::Object_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_String_Hash_Key,CORBA::Object_ptr,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, CORBA::Object_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, CORBA::Object_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,CORBA::Object_ptr,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, CORBA::Object_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Array<char*>;
template class ACE_Array_Base<char*>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key,CORBA::Object_ptr>
#pragma instantiate ACE_Hash_Map_Manager<TAO_String_Hash_Key,CORBA::Object_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, CORBA::Object_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_String_Hash_Key,CORBA::Object_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, CORBA::Object_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, CORBA::Object_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,CORBA::Object_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, CORBA::Object_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Array<char*>
#pragma instantiate ACE_Array_Base<char*>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
