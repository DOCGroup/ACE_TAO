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
// ============================================================================

#include "AVStreams_i.h"
#include "sfp.h"
#include "MCast.h"
#include "RTCP.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
#include "AVStreams_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (AV,
           AVStreams_i,
           "$Id$")

//------------------------------------------------------------
// TAO_AV_Qos
//------------------------------------------------------------

TAO_AV_QoS::TAO_AV_QoS (void)
{
}

TAO_AV_QoS::TAO_AV_QoS (AVStreams::streamQoS &stream_qos)
{
  this->set (stream_qos);
}

int
TAO_AV_QoS::convert (AVStreams::streamQoS &/*network_qos*/)
{
  return -1;
}


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


// Stop the transfer of data of the stream
// Empty the_spec means apply operation to all flows
void
TAO_Basic_StreamCtrl::stop (const AVStreams::flowSpec &flow_spec
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow))
{
  ACE_TRY
    {
      // @@Call stop on the Related MediaCtrl.  call stop on the flow
      // connections.
      if (this->flow_connection_map_.current_size () > 0)
        {
          if (flow_spec.length () > 0)
            for (u_int i=0;i<flow_spec.length ();i++)
              {
                char *flowname = TAO_AV_Core::get_flowname (flow_spec[i]);
                ACE_CString flow_name_key (flowname);
                FlowConnection_Map::ENTRY *flow_connection_entry = 0;
                if (this->flow_connection_map_.find (flow_name_key,
                                                     flow_connection_entry) == 0)
                  {
                    flow_connection_entry->int_id_->stop (ACE_ENV_SINGLE_ARG_PARAMETER);
                    ACE_TRY_CHECK;
                  }
              }
          else
            {
              // call stop on all the flows.
              FlowConnection_Map_Iterator iterator (this->flow_connection_map_);
              FlowConnection_Map_Entry *entry = 0;
              for (;iterator.next (entry) !=  0;iterator.advance ())
                {
                  entry->int_id_->stop (ACE_ENV_SINGLE_ARG_PARAMETER);
                  ACE_TRY_CHECK;
                }
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Basic_StreamCtrl::stop");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Start the transfer of data in the stream.
// Empty the_spec means apply operation to all flows
void
TAO_Basic_StreamCtrl::start (const AVStreams::flowSpec &flow_spec
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow))
{
  ACE_TRY
    {
      // @@Call start on the Related MediaCtrl.

      // call start on the flow connections.
      if (this->flow_connection_map_.current_size () > 0)
        {
          if (flow_spec.length () > 0)
            for (u_int i = 0; i < flow_spec.length (); i++)
              {
                char *flowname = TAO_AV_Core::get_flowname (flow_spec[i]);
                ACE_CString flow_name_key (flowname);
                FlowConnection_Map::ENTRY *flow_connection_entry = 0;
                if (this->flow_connection_map_.find (flow_name_key,
                                                     flow_connection_entry) == 0)
                  {
                    flow_connection_entry->int_id_->start (ACE_ENV_SINGLE_ARG_PARAMETER);
                    ACE_TRY_CHECK;
                  }
              }
          else
            {
              // call start on all the flows.
              FlowConnection_Map_Iterator iterator (this->flow_connection_map_);
              FlowConnection_Map_Entry *entry = 0;
              for (;iterator.next (entry) !=  0;iterator.advance ())
                {
                  entry->int_id_->start (ACE_ENV_SINGLE_ARG_PARAMETER);
                  ACE_TRY_CHECK;
                }
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Basic_StreamCtrl::start");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Tears down the stream. This will close the connection, and delete
// the streamendpoint and vdev associated with this stream Empty
// the_spec means apply operation to all flows

void
TAO_Basic_StreamCtrl::destroy (const AVStreams::flowSpec &flow_spec
                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::noSuchFlow))
{
  ACE_TRY
    {
      // call stop on the flow connections.
      if (this->flow_connection_map_.current_size () > 0)
        {
          if (flow_spec.length () > 0)
          {
            for (u_int i=0;i<flow_spec.length ();i++)
              {
                char *flowname = TAO_AV_Core::get_flowname (flow_spec[i]);
                ACE_CString flow_name_key (flowname);
                FlowConnection_Map::ENTRY *flow_connection_entry = 0;
                if (this->flow_connection_map_.find (flow_name_key, flow_connection_entry) == 0)
                  {
                    flow_connection_entry->int_id_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
                    ACE_TRY_CHECK;
                  }
              }
          }
          else
            {
              // call destroy on all the flows.
              FlowConnection_Map_Iterator iterator (this->flow_connection_map_);
              FlowConnection_Map_Entry *entry = 0;
              for (;iterator.next (entry) !=  0;iterator.advance ())
                {
                  entry->int_id_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
                  ACE_TRY_CHECK;
                }
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Basic_StreamCtrl::destroy");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Changes the QoS associated with the stream
// Empty the_spec means apply operation to all flows
CORBA::Boolean

TAO_Basic_StreamCtrl::modify_QoS (AVStreams::streamQoS & /*new_qos*/,
                                  const AVStreams::flowSpec &/*flowspec*/
                                  ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::noSuchFlow,
                     AVStreams::QoSRequestFailed))
{
  return 1;
}

// Used by StreamEndPoint and VDev to inform StreamCtrl of events.
// E.g., loss of flow, reestablishment of flow, etc..
void
TAO_Basic_StreamCtrl::push_event (const struct CosPropertyService::Property &/*the_event*/
                                  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "\n(%P|%t) Recieved event \""));
}

// Sets the flow protocol status.
void
TAO_Basic_StreamCtrl::set_FPStatus (const AVStreams::flowSpec &flow_spec,
                                    const char  *fp_name,
                                    const CORBA::Any &fp_settings
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::FPError))

{
  if (!CORBA::is_nil (this->sep_a_.in ()))
    {
      this->sep_a_->set_FPStatus (flow_spec, fp_name, fp_settings ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

// Gets the flow connection.
CORBA::Object_ptr
TAO_Basic_StreamCtrl::get_flow_connection (const char *flow_name
                                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::noSuchFlow,
                     AVStreams::notSupported))
{
  ACE_CString flow_name_key (flow_name);
  FlowConnection_Map::ENTRY *flow_connection_entry = 0;
  if (this->flow_connection_map_.find (flow_name_key, flow_connection_entry) == 0)
    return AVStreams::FlowConnection::_duplicate (flow_connection_entry->int_id_);
  else
    ACE_THROW_RETURN (AVStreams::noSuchFlow (), CORBA::Object::_nil ());
}

// Sets the flow connection.
void
TAO_Basic_StreamCtrl::set_flow_connection (const char *flow_name,
                                           CORBA::Object_ptr flow_connection_obj
                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::notSupported))
{
  AVStreams::FlowConnection_ptr flow_connection = AVStreams::FlowConnection::_nil ();
  ACE_TRY
    {
      flow_connection = AVStreams::FlowConnection::_narrow (flow_connection_obj ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Basic_StreamCtrl::set_flow_connection");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
  // add the flowname and the flowconnection to the hashtable.
  this->flows_.length (this->flow_count_ + 1);
  this->flows_ [this->flow_count_++] = CORBA::string_dup (flow_name);
  ACE_CString flow_name_key (flow_name);
  if (this->flow_connection_map_.bind (flow_name_key, flow_connection) != 0)
    ACE_THROW (AVStreams::noSuchFlow ());// is this right?
}

TAO_Basic_StreamCtrl::~TAO_Basic_StreamCtrl (void)
{
  FlowConnection_Map_Iterator iterator (this->flow_connection_map_);
  FlowConnection_Map_Entry *entry = 0;
  for (;iterator.next (entry) != 0;iterator.advance ())
    CORBA::release (entry->int_id_);
}

// ----------------------------------------------------------------------
// TAO_Negotiator
// ----------------------------------------------------------------------

CORBA::Boolean
TAO_Negotiator::negotiate (AVStreams::Negotiator_ptr /* remote_negotiator */,
                           const AVStreams::streamQoS &/* qos_spec */
                           ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "TAO_Negotiator::negotiate\n"));
  return 0;
}

// ----------------------------------------------------------------------
// MMDevice_Map_Hash_Key
// ----------------------------------------------------------------------

const int MMDevice_Map_Hash_Key::hash_maximum_ = 10000;

//default constructor.
MMDevice_Map_Hash_Key::MMDevice_Map_Hash_Key (void)
{
  this->mmdevice_ = AVStreams::MMDevice::_nil ();
}

// constructor.
MMDevice_Map_Hash_Key::MMDevice_Map_Hash_Key (AVStreams::MMDevice_ptr mmdevice)
{
  this->mmdevice_ = AVStreams::MMDevice::_duplicate (mmdevice);
}

// copy constructor.
MMDevice_Map_Hash_Key::MMDevice_Map_Hash_Key (const MMDevice_Map_Hash_Key& hash_key)
{
  this->mmdevice_ = AVStreams::MMDevice::_duplicate (hash_key.mmdevice_);
}

// destructor.
MMDevice_Map_Hash_Key::~MMDevice_Map_Hash_Key (void)
{
  CORBA::release (this->mmdevice_);
}

int
MMDevice_Map_Hash_Key::operator == (const MMDevice_Map_Hash_Key &hash_key) const
{
  CORBA::Boolean result = 0;
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      result =
        this->mmdevice_->_is_equivalent (hash_key.mmdevice_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "MMDevice_Map_Hash_Key::operator == ");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return result;
}

int
operator < (const MMDevice_Map_Hash_Key &left,
            const MMDevice_Map_Hash_Key &right)
{
  int result = 0;
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::ULong left_hash = left.mmdevice_->_hash (left.hash_maximum_
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::ULong right_hash = right.mmdevice_->_hash (right.hash_maximum_
                                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      result = left_hash < right_hash;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "operator < for MMDevice_Map_Hash_Key");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return result;
}

u_long
MMDevice_Map_Hash_Key::hash (void)  const
{
  u_long result = 0;
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      result = this->mmdevice_->_hash (this->hash_maximum_
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "MMDevice_Map_Hash_Key::hash");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return result;
}

// ----------------------------------------------------------------------
// TAO_StreamCtrl
// ----------------------------------------------------------------------

TAO_StreamCtrl::TAO_StreamCtrl (void)
  :mcastconfigif_ (0)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->streamctrl_ = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      char buf [BUFSIZ];
      int result = ACE_OS::hostname (buf, BUFSIZ);
      unsigned long ipaddr = 0;
      if (result == 0)
        ipaddr = ACE_OS::inet_addr (buf);
      this->source_id_ = TAO_AV_RTCP::alloc_srcid (ipaddr);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamCtrl::TAO_StreamCtrl");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

TAO_StreamCtrl::TAO_StreamCtrl (TAO_StreamCtrl const &)
{
  //no-op
}

void
TAO_StreamCtrl::operator= (TAO_StreamCtrl const &)
{
  //no-op
}

TAO_StreamCtrl::~TAO_StreamCtrl (void)
{
  delete this->mcastconfigif_;
}


// Stop the transfer of data of the stream
// Empty the_spec means apply operation to all flows
void
TAO_StreamCtrl::stop (const AVStreams::flowSpec &flow_spec
                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow))
{
  ACE_TRY
    {
      TAO_Basic_StreamCtrl::stop (flow_spec ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (this->flow_connection_map_.current_size () > 0)
        return;
      MMDevice_Map_Iterator a_iterator (this->mmdevice_a_map_);
      MMDevice_Map::ENTRY *entry = 0;
      for (;a_iterator.next (entry)!= 0;a_iterator.advance ())
        {
          entry->int_id_.sep_->stop (flow_spec ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      MMDevice_Map_Iterator b_iterator (this->mmdevice_b_map_);
      for (;b_iterator.next (entry)!= 0;b_iterator.advance ())
        {
          entry->int_id_.sep_->stop (flow_spec ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Basic_StreamCtrl::stop");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Start the transfer of data in the stream.
// Empty the_spec means apply operation to all flows
void
TAO_StreamCtrl::start (const AVStreams::flowSpec &flow_spec
                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow))
{
  ACE_TRY
    {
      TAO_Basic_StreamCtrl::start (flow_spec ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (this->flow_connection_map_.current_size () > 0)
        return;

      MMDevice_Map_Iterator a_iterator (this->mmdevice_a_map_);
      MMDevice_Map::ENTRY *entry = 0;
      for (;a_iterator.next (entry)!= 0;a_iterator.advance ())
        {
          entry->int_id_.sep_->start (flow_spec ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      MMDevice_Map_Iterator b_iterator (this->mmdevice_b_map_);
      for (;b_iterator.next (entry)!= 0;b_iterator.advance ())
        {
          entry->int_id_.sep_->start (flow_spec ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamCtrl::start");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Tears down the stream. This will close the connection, and delete
// the streamendpoint and vdev associated with this stream
// Empty the_spec means apply operation to all flows
void
TAO_StreamCtrl::destroy (const AVStreams::flowSpec &flow_spec
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::noSuchFlow))
{
  ACE_TRY
    {
      TAO_Basic_StreamCtrl::destroy (flow_spec ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (this->flow_connection_map_.current_size () > 0)
        return;

      MMDevice_Map_Iterator a_iterator (this->mmdevice_a_map_);
      MMDevice_Map::ENTRY *entry = 0;
      for (;a_iterator.next (entry)!= 0;a_iterator.advance ())
        {
          entry->int_id_.sep_->destroy (flow_spec ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      MMDevice_Map_Iterator b_iterator (this->mmdevice_b_map_);
      for (;b_iterator.next (entry)!= 0;b_iterator.advance ())
        {
          entry->int_id_.sep_->destroy (flow_spec ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamCtrl::destroy");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;

  int result = TAO_AV_Core::deactivate_servant (this);
  if (result < 0)
    if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "TAO_StreamCtrl::destroy failed\n"));
}

// request the two MMDevices to create vdev and stream endpoints. save
// the references returned.

// The interaction diagram for this method is on page 13 of the spec
CORBA::Boolean
TAO_StreamCtrl::bind_devs (AVStreams::MMDevice_ptr a_party,
                           AVStreams::MMDevice_ptr b_party,
                           AVStreams::streamQoS &the_qos,
                           const AVStreams::flowSpec &the_flows
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed))
{
  ACE_TRY
    {
      if (CORBA::is_nil (a_party) && CORBA::is_nil (b_party))
        ACE_ERROR_RETURN ((LM_ERROR, "Both parties are nil\n"), 0);
      // Check to see if we have non-nil parties to bind!
      if (TAO_debug_level > 0)
        if (CORBA::is_nil (a_party) ||
            CORBA::is_nil (b_party))
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) TAO_StreamCtrl::bind_devs: "
                        "a_party or b_party is null"
                        "Multicast mode\n"));

      // Request a_party to create the endpoint and vdev
      CORBA::Boolean met_qos;
      CORBA::String_var named_vdev;

      if (!CORBA::is_nil (a_party))
        {
          MMDevice_Map_Hash_Key find_key (a_party);
          MMDevice_Map_Entry find_entry;
          int result =
            this->mmdevice_a_map_.find (find_key, find_entry);
          if (result == 0)
            {
              if (TAO_debug_level > 0)
                {
                  // Already in the map.
                  if (TAO_debug_level > 0)
                    ACE_DEBUG ((LM_DEBUG, "mmdevice a_party is already bound\n"));
                }
              return 1;
            }
          else
            {
              this->sep_a_ =
                a_party-> create_A (this->streamctrl_.in (),
                                    this->vdev_a_.out (),
                                    the_qos,
                                    met_qos,
                                    named_vdev.inout (),
                                    the_flows
                                    ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG, "(%P|%t) TAO_StreamCtrl::create_A: succeeded\n"));
              // Define ourselves as the related_streamctrl property of the sep.
              CORBA::Any streamctrl_any;
              streamctrl_any <<= this->streamctrl_.in ();
              this->sep_a_->define_property ("Related_StreamCtrl",
                                             streamctrl_any
                                             ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              CORBA::Any vdev_a_any;
              vdev_a_any <<= this->vdev_a_.in ();
              this->sep_a_->define_property ("Related_VDev",
                                             vdev_a_any
                                             ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              CORBA::Any streamendpoint_a_any;
              streamendpoint_a_any <<= this->sep_a_.in ();
              this->vdev_a_->define_property ("Related_StreamEndpoint",
                                              streamendpoint_a_any
                                              ACE_ENV_ARG_PARAMETER);

              ACE_TRY_CHECK;

              CORBA::Any mmdevice_a_any;
              mmdevice_a_any <<= a_party;
              this->vdev_a_->define_property ("Related_MMDevice",
                                              mmdevice_a_any
                                              ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // add the mmdevice, sep and vdev to the map.
              MMDevice_Map_Entry map_entry;
              MMDevice_Map_Hash_Key key (a_party);
              map_entry.sep_ = AVStreams::StreamEndPoint_A::_duplicate (this->sep_a_.in ());
              map_entry.vdev_ = AVStreams::VDev::_duplicate (this->vdev_a_.in ());
              map_entry.flowspec_ = the_flows;
              map_entry.qos_ = the_qos;
              result =
                this->mmdevice_a_map_.bind (key, map_entry);
              if (result < 0)
                if (TAO_debug_level > 0)
                  ACE_DEBUG ((LM_DEBUG, "Error binding mmdevice entry in the a_map"));
            }
        }
      // Request b_party to create the endpoint and vdev
      if (!CORBA::is_nil (b_party))
        {
          MMDevice_Map_Hash_Key find_key (b_party);
          MMDevice_Map_Entry find_entry;
          int result =
            this->mmdevice_b_map_.find (find_key, find_entry);
          if (result == 0)
            {
              // Already in the map.
              if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "mmdevice b_party is already bound\n"));
              return 1;
            }
          else
            {
              this->sep_b_ =
                b_party-> create_B (this->streamctrl_.in (),
                                    this->vdev_b_.out (),
                                    the_qos,
                                    met_qos,
                                    named_vdev.inout (),
                                    the_flows
                                    ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "(%P|%t) TAO_StreamCtrl::create_B: succeeded\n"));

              if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,
                          "\n(%P|%t)stream_endpoint_b_ = %s",
                          TAO_ORB_Core_instance ()->orb ()->object_to_string (this->sep_b_.in ()
                                                                              ACE_ENV_ARG_PARAMETER)));
              ACE_TRY_CHECK;
              // Define ourselves as the related_streamctrl property of the sep.
              CORBA::Any streamctrl_any;
              streamctrl_any <<= this->streamctrl_.in ();
              this->sep_b_->define_property ("Related_StreamCtrl",
                                             streamctrl_any
                                             ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              CORBA::Any vdev_b_any;
              vdev_b_any <<= this->vdev_b_.in ();
              this->sep_b_->define_property ("Related_VDev",
                                             vdev_b_any
                                             ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              CORBA::Any streamendpoint_b_any;
              streamendpoint_b_any <<= this->sep_b_.in ();
              this->vdev_b_->define_property ("Related_StreamEndpoint",
                                              streamendpoint_b_any
                                              ACE_ENV_ARG_PARAMETER);

              ACE_TRY_CHECK;

              CORBA::Any mmdevice_b_any;
              mmdevice_b_any <<= b_party;
              this->vdev_b_->define_property ("Related_MMDevice",
                                              mmdevice_b_any
                                              ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              // add the mmdevice, sep and vdev to the map.
              MMDevice_Map_Entry map_entry;
              MMDevice_Map_Hash_Key key (b_party);
              map_entry.sep_ = AVStreams::StreamEndPoint::_duplicate (this->sep_b_.in ());
              map_entry.vdev_ = AVStreams::VDev::_duplicate(this->vdev_b_.in ());
              map_entry.flowspec_ = the_flows;
              map_entry.qos_ = the_qos;
              int result =
                this->mmdevice_b_map_.bind (key, map_entry);
              if (result < 0)
                if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "Error binding mmdevice entry in the b_map"));
            }
        }

      // Tell the endpoints about each other.
      if ((!CORBA::is_nil (a_party)) && (!CORBA::is_nil (b_party)))
        {
          CORBA::Any sep_a_peer_any;
          CORBA::Any sep_b_peer_any;

          sep_a_peer_any <<= this->sep_b_.in();
          sep_b_peer_any <<= this->sep_a_.in();
          this->sep_a_->define_property ("PeerAdapter",
                                          sep_a_peer_any
                                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->sep_b_->define_property ("PeerAdapter",
                                         sep_b_peer_any
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // In the full profile case there's no VDev.
      if (CORBA::is_nil (b_party) && (!CORBA::is_nil (this->vdev_a_.in ())))
        {
          // Now set the source id for this A endpoint.
          // If the sep contains flow producers then set the source ids for those
          // instead.
          ACE_TRY_EX (set_source_id)
            {
              CORBA::Any_ptr flows_any = this->sep_a_->get_property_value ("Flows"
                                                                           ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK_EX (set_source_id);
              AVStreams::flowSpec_var flows;
              *flows_any >>= flows.out ();
              for (u_int i=0; i< flows->length ();i++)
                {
                  CORBA::Object_var fep_obj =
                    this->sep_a_->get_fep (flows [i] ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK_EX (set_source_id);
                  ACE_TRY_EX (producer_check)
                    {
                      AVStreams::FlowProducer_var producer =
                        AVStreams::FlowProducer::_narrow (fep_obj.in ()
                                                          ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK_EX (producer_check);
                      producer->set_source_id (this->source_id_++);
                    }
                  ACE_CATCHANY
                    {
                      if (TAO_debug_level > 0)
                        ACE_DEBUG ((LM_DEBUG, " %s ", flows[i].in ()));

                      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "producer_check: not a producer");

                    }
                  ACE_ENDTRY;
                  ACE_CHECK_RETURN (0);
                }
            }
          ACE_CATCHANY
            {
              // Since the full profile failed try setting the source id
              // for the sep instead.
              // @@Naga: What do we do if in the light profile the sep has
              // many producers who do not have flow interfaces. Then
              // the streamctrl has to give an array of source ids to
              // the sep.
              this->sep_a_->set_source_id (this->source_id_++
                                           ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          ACE_ENDTRY;
          ACE_CHECK_RETURN (0);
          if (!this->mcastconfigif_)
            {
              ACE_NEW_RETURN (this->mcastconfigif_,
                              TAO_MCastConfigIf,
                              0);
              // @@: Deactivating the object thru poa means calling remove_ref after _this.
              this->mcastconfigif_ptr_ = this->mcastconfigif_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          // Multicast source being added.
          CORBA::Boolean result = this->vdev_a_->set_Mcast_peer (this->streamctrl_.in (),
                                                                 this->mcastconfigif_ptr_.in (),
                                                                 the_qos,
                                                                 the_flows
                                                                 ACE_ENV_ARG_PARAMETER);
          if (!result)
            ACE_ERROR_RETURN ((LM_ERROR, "set_Mcast_peer failed\n"), 0);
        }

      if (CORBA::is_nil (a_party))
        {
          if (!CORBA::is_nil (this->vdev_b_.in ()))
            {
              // Multicast sink being added.
              if (!this->mcastconfigif_)
                ACE_ERROR_RETURN ((LM_ERROR, "first add a source and then a sink\n"), 0);
              this->mcastconfigif_->set_peer (this->vdev_b_.in (),
                                              the_qos,
                                              the_flows
                                              ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          int connect_leaf_success = 0;
          ACE_TRY_EX (connect_leaf)
            {
              // @@: define null interfaces for Atm so that they can be implemented once
              //  ACE adds support for ATM multicast.
              connect_leaf_success = this->sep_a_->connect_leaf (this->sep_b_.in (),
                                                                 the_qos,
                                                                 the_flows
                                                                 ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK_EX (connect_leaf);
              connect_leaf_success = 1;
            }
          ACE_CATCH (AVStreams::notSupported, ex)
            {
              if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "connect_leaf failed\n"));
              connect_leaf_success = 0;
            }
          ACE_CATCHANY
            {
              ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamCtrl::bind_devs");
            }
          ACE_ENDTRY;
          ACE_CHECK_RETURN (0);
          if (!connect_leaf_success)
            {
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,"TAO_StreamCtrl::bind_devs Multiconnect\n"));
              AVStreams::flowSpec connect_flows = the_flows;
              this->sep_a_->multiconnect (the_qos, connect_flows ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              this->sep_b_->multiconnect (the_qos, connect_flows ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }

      if (!CORBA::is_nil (a_party) && !CORBA::is_nil (b_party))
        {
          if (!CORBA::is_nil (this->vdev_a_.in ()) && !CORBA::is_nil (this->vdev_b_.in ()))
            {
              // Tell the 2 VDev's about one another
              this->vdev_a_->set_peer (this->streamctrl_.in (),
                                       this->vdev_b_.in (),
                                       the_qos,
                                       the_flows
                                       ACE_ENV_ARG_PARAMETER);

              ACE_TRY_CHECK;
              this->vdev_b_->set_peer (this->streamctrl_.in (),
                                       this->vdev_a_.in (),
                                       the_qos,
                                       the_flows
                                       ACE_ENV_ARG_PARAMETER);

              ACE_TRY_CHECK;

              // Now connect the streams together. This will
              // establish the connection
              CORBA::Boolean result  =
                this->sep_a_->connect (this->sep_b_.in (),
                                       the_qos,
                                       the_flows
                                       ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              if (result == 0)
                ACE_ERROR_RETURN ((LM_ERROR, "sep_a->connect (sep_b) failed\n"), 0);
            }
          else
            {
              // Its full profile
              // we have feps in the sep then dont call connect instead call bind on the streamctrl.
              this->bind (this->sep_a_.in (),
                          this->sep_b_.in (),
                          the_qos,
                          the_flows
                          ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamCtrl::bind_devs");
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
                      AVStreams::streamQoS &stream_qos,
                      const AVStreams::flowSpec &flow_spec
                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed))
{
  this->sep_a_ = AVStreams::StreamEndPoint_A::_duplicate(sep_a);
  this->sep_b_ = AVStreams::StreamEndPoint_B::_duplicate(sep_b);

  int result = 0;
  ACE_TRY
    {
      if (CORBA::is_nil (sep_a) ||
          CORBA::is_nil (sep_b))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) TAO_StreamCtrl::bind:"
                           "a_party or b_party null!"),
                          0);

      // Define each other as their peers.
      CORBA::Any sep_any;
      sep_any <<= sep_b;
      sep_a->define_property ("PeerAdapter",
                              sep_any
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      sep_any <<= sep_a;
      sep_b->define_property ("PeerAdapter",
                              sep_any
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      // since its full profile we do the viable stream setup algorithm.
      // get the flows for the A streamendpoint.
      // the flows spec is empty and hence we do a exhaustive match.
      AVStreams::flowSpec a_flows, b_flows;
      CORBA::Any_var flows_any;
      flows_any = sep_a->get_property_value ("Flows" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      AVStreams::flowSpec *temp_flows;
      flows_any.in () >>= temp_flows;
      a_flows = *temp_flows;
      flows_any = sep_b->get_property_value ("Flows" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      flows_any.in () >>= temp_flows;
      b_flows = *temp_flows;
      u_int i;
      FlowEndPoint_Map *a_fep_map;
      FlowEndPoint_Map *b_fep_map;
      ACE_NEW_RETURN (a_fep_map,
                      FlowEndPoint_Map,
                      0);
      ACE_NEW_RETURN (b_fep_map,
                      FlowEndPoint_Map,
                      0);
      for (i=0;i<a_flows.length ();i++)
        {
          const char *flowname = a_flows[i];
          // get the flowendpoint references.
          CORBA::Object_var fep_obj =
            sep_a->get_fep (flowname
                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          AVStreams::FlowEndPoint_ptr fep =
            AVStreams::FlowEndPoint::_narrow (fep_obj.in ()
                                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          ACE_CString fep_key (flowname);
          result = a_fep_map->bind (fep_key, fep);
          if (result == -1)
            if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "TAO_StreamCtrl::bind failed for %s", flowname));
        }
      // get the flowendpoints for streamendpoint_b
      for (i=0;i<b_flows.length ();i++)
        {
          const char *flowname = b_flows[i];
          // get the flowendpoint references.
          CORBA::Object_var fep_obj =
            sep_b->get_fep (flowname
                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          AVStreams::FlowEndPoint_ptr fep =
            AVStreams::FlowEndPoint::_narrow (fep_obj.in ()
                                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          ACE_CString fep_key (flowname);
          result = b_fep_map->bind (fep_key, fep);
          if (result == -1)
            if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "TAO_StreamCtrl::bind failed for %s", flowname));
        }
      FlowEndPoint_Map *map_a = 0, *map_b = 0;
      if (flow_spec.length () == 0)
        {
          map_a = a_fep_map;
          map_b = b_fep_map;
        }
      else
        {
          FlowEndPoint_Map *spec_fep_map_a, *spec_fep_map_b;
          ACE_NEW_RETURN (spec_fep_map_a,
                          FlowEndPoint_Map,
                          0);
          ACE_NEW_RETURN (spec_fep_map_b,
                          FlowEndPoint_Map,
                          0);
          for (i=0; i< flow_spec.length ();i++)
            {
              TAO_Forward_FlowSpec_Entry *entry;
              ACE_NEW_RETURN (entry,
                              TAO_Forward_FlowSpec_Entry,
                              0);
              entry->parse (flow_spec[i].in ());
              ACE_CString fep_key (entry->flowname ());
              AVStreams::FlowEndPoint_ptr fep;
              result = a_fep_map->find (fep_key, fep);
              if (result == -1)
                ACE_ERROR_RETURN ((LM_ERROR, "Fep not found on A side for flowname: %s", flow_spec[i].in ()), 0);

              result = spec_fep_map_a->bind (fep_key, fep);
              if (result == -1)
                if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "Bind failed for %s", flow_spec[i].in ()));

              result = b_fep_map->find (fep_key, fep);
              if (result == -1)
                ACE_ERROR_RETURN ((LM_ERROR, "Fep not found on B side for flowname: %s", flow_spec[i].in ()), 0);

              result = spec_fep_map_b->bind (fep_key, fep);
              if (result == -1)
                if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "Bind failed for %s", flow_spec[i].in ()));
            }
          map_a = spec_fep_map_a;
          map_b = spec_fep_map_b;
        }

      TAO_AV_QoS qos (stream_qos);
      // Now go thru the list of flow endpoint and match them.
      // uses the first match policy.
      FlowEndPoint_Map_Iterator a_feps_iterator (*map_a);
      FlowEndPoint_Map_Entry *a_feps_entry, *b_feps_entry;
      ACE_TRY_EX (flow_connect)
        {

          for (;a_feps_iterator.next (a_feps_entry) != 0;
               a_feps_iterator.advance ())
            {
              AVStreams::FlowEndPoint_ptr fep_a = a_feps_entry->int_id_;
              AVStreams::FlowEndPoint_var connected_to =
                fep_a->get_connected_fep (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK_EX (flow_connect);

              if (!CORBA::is_nil (connected_to.in ()))
                {
                  // Skip this one, it is already connected...
                  continue;
                }

              FlowEndPoint_Map_Iterator b_feps_iterator (*map_b);
              for (;b_feps_iterator.next (b_feps_entry) != 0;
                   b_feps_iterator.advance ())
                {
                  AVStreams::FlowEndPoint_ptr fep_b = b_feps_entry->int_id_;
                  AVStreams::FlowConnection_var flow_connection;

                  AVStreams::FlowEndPoint_var connected_to =
                    fep_b->get_connected_fep (ACE_ENV_SINGLE_ARG_PARAMETER);
                  ACE_TRY_CHECK_EX (flow_connect);

                  if (!CORBA::is_nil (connected_to.in ()))
                    {
                      // Skip this one, it is already connected...
                      continue;
                    }

                  if (fep_a->is_fep_compatible (fep_b
                                                ACE_ENV_ARG_PARAMETER) == 1)
                    {
                      ACE_TRY_CHECK_EX (flow_connect);
                      // assume that flow names are same so that we
                      // can use either of them.
                      CORBA::Object_var flow_connection_obj;
                      CORBA::Any_var flowname_any =
                        fep_a->get_property_value ("Flow"
                                                   ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK_EX (flow_connect);
                      const char *flowname = 0;
                      flowname_any.in () >>= flowname;
                      ACE_TRY_EX (flow_connection)
                        {
                          flow_connection_obj =
                            this->get_flow_connection (flowname
                                                       ACE_ENV_ARG_PARAMETER);
                          ACE_TRY_CHECK_EX (flow_connection);
                          flow_connection =
                            AVStreams::FlowConnection::_narrow (flow_connection_obj.in ()
                                                                ACE_ENV_ARG_PARAMETER);
                          ACE_TRY_CHECK_EX (flow_connection);
                        }
                      ACE_CATCHANY
                        {
                          TAO_FlowConnection *flowConnection;
                          ACE_NEW_RETURN (flowConnection,
                                          TAO_FlowConnection,
                                          0);
                          flow_connection = flowConnection->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
                          ACE_TRY_CHECK_EX (flow_connect);
                          this->set_flow_connection (flowname,
                                                     flow_connection.in ()
                                                     ACE_ENV_ARG_PARAMETER);
                          ACE_TRY_CHECK_EX (flow_connect);
                        }
                      ACE_ENDTRY;
                      ACE_CHECK_RETURN (0);

                      // make sure that a_feps is flow_producer
                      // and b_feps is flow_consumer
                      // There should be a way to find which flow
                      // endpoint is producer and which is
                      // consumer.

                      AVStreams::FlowProducer_var producer;
                      AVStreams::FlowConsumer_var consumer;

                      ACE_TRY_EX (producer_check)
                        {
                          producer =
                            AVStreams::FlowProducer::_narrow (fep_a
                                                              ACE_ENV_ARG_PARAMETER);
                          ACE_TRY_CHECK_EX (producer_check);
                          consumer =
                            AVStreams::FlowConsumer::_narrow (fep_b
                                                              ACE_ENV_ARG_PARAMETER);
                          ACE_TRY_CHECK_EX (producer_check);

                          // If the types don't match then try in
                          // the opposite order
                          if (CORBA::is_nil (producer.in ()))
                            {
                              producer =
                                AVStreams::FlowProducer::_narrow (fep_b
                                                                  ACE_ENV_ARG_PARAMETER);
                              ACE_TRY_CHECK_EX (producer_check);
                              consumer =
                                AVStreams::FlowConsumer::_narrow (fep_a
                                                                  ACE_ENV_ARG_PARAMETER);
                              ACE_TRY_CHECK_EX (producer_check);
                            }
                          // At this point they should both be
                          // non-nil
                          // @@ raise an exception (which one?) if
                          // this is not true...
                          ACE_ASSERT (!CORBA::is_nil (producer.in ()));
                          ACE_ASSERT (!CORBA::is_nil (consumer.in ()));
                        }
                      ACE_CATCHANY
                        {
                          //Yamuna : Recheck this
                          ACE_RE_THROW;//_EX (producer_check);
                        }
                      ACE_ENDTRY;
                      ACE_CHECK_RETURN (0);
                      CORBA::String_var fep_a_name, fep_b_name;
                      flowname_any = fep_a->get_property_value ("FlowName"
                                                                ACE_ENV_ARG_PARAMETER);
                      const char *temp_name;
                      flowname_any.in () >>= temp_name;
                      fep_a_name = CORBA::string_dup (temp_name);
                      flowname_any = fep_b->get_property_value ("FlowName"
                                                                ACE_ENV_ARG_PARAMETER);
                      flowname_any.in () >>= temp_name;
                      fep_b_name = CORBA::string_dup (temp_name);
                      AVStreams::QoS flow_qos;
                      flow_qos.QoSType = fep_a_name;
                      flow_qos.QoSParams.length (0);
                      result = qos.get_flow_qos (fep_a_name.in (), flow_qos);
                      if (result == -1)
                        {
                          flow_qos.QoSType = fep_b_name;
                          result = qos.get_flow_qos (fep_b_name.in (),
                                                     flow_qos);
                          if (result == -1 && TAO_debug_level > 0)
                            ACE_DEBUG ((LM_DEBUG,
                                        "No QoS Specified for this flow <%s>\n", flowname));
                        }
                      flow_connection->connect (producer.in (),
                                                consumer.in (),
                                                flow_qos
                                                ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK_EX (flow_connect);
                    }
                }
            }
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "TAO_StreamCtrl::bind:flow_connect block");
          return 0;
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (0);
    }
  ACE_CATCHANY
    {
      // error was thrown because one of the streamendpoints is light profile.
      // Now connect the streams together
      this->sep_a_->connect (this->sep_b_.in (),
                             stream_qos,
                             flow_spec
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

void
TAO_StreamCtrl::unbind (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed))
{
  ACE_TRY
    {
      if (this->flow_connection_map_.current_size () > 0)
        return;

      AVStreams::flowSpec flow_spec;
      flow_spec.length(0);

      MMDevice_Map_Iterator a_iterator (this->mmdevice_a_map_);
      MMDevice_Map::ENTRY *entry = 0;
      for (;a_iterator.next (entry)!= 0;a_iterator.advance ())
        {
          entry->int_id_.sep_->destroy (flow_spec ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      MMDevice_Map_Iterator b_iterator (this->mmdevice_b_map_);
      for (;b_iterator.next (entry)!= 0;b_iterator.advance ())
        {
          entry->int_id_.sep_->destroy (flow_spec ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamCtrl::unbind");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
TAO_StreamCtrl::unbind_party (AVStreams::StreamEndPoint_ptr /* the_ep */,
                              const AVStreams::flowSpec &/* the_spec */
                              ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::noSuchFlow))
{
}

void
TAO_StreamCtrl::unbind_dev (AVStreams::MMDevice_ptr /* dev */,
                            const AVStreams::flowSpec & /* the_spec */
                            ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::noSuchFlow))
{
}

AVStreams::VDev_ptr
TAO_StreamCtrl::get_related_vdev (AVStreams::MMDevice_ptr adev,
                                  AVStreams::StreamEndPoint_out sep
                                  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed))
{
  MMDevice_Map_Hash_Key key (adev);
  MMDevice_Map_Entry entry;
  int result = -1;
  result = this->mmdevice_a_map_.find (key, entry);
  if (result < 0)
    {
      result = this->mmdevice_a_map_.find (key, entry);
      if (result < 0)
        return AVStreams::VDev::_nil ();
    }
  sep = AVStreams::StreamEndPoint::_duplicate (entry.sep_.in ());
  return AVStreams::VDev::_duplicate (entry.vdev_.in ());
}

CORBA::Boolean

TAO_StreamCtrl::modify_QoS (AVStreams::streamQoS &new_qos,
                            const AVStreams::flowSpec &the_spec
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed))
{
  if (TAO_debug_level > 0)
  ACE_DEBUG ((LM_DEBUG,
              "TAO_StreamCtrl::modify_QoS\n"));


  if (this->mcastconfigif_ != 0)
    {
      // call modify_Qos on the root VDev which is the mcast configif.
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "Cannot Modify the Qos for multipoint streams\n"));
    }
  else
    {
      ACE_TRY
        {
          AVStreams::flowSpec in_flowspec;
          AVStreams::flowSpec out_flowspec;

          in_flowspec.length (0);
          out_flowspec.length (0);

          int in_index = 0;
          int out_index = 0;

          AVStreams::flowSpec flowspec;
          if (the_spec.length () == 0)
            {
              // Apply modify_qos to all the flows
              flowspec = this->flows_;
              MMDevice_Map_Iterator iterator (this->mmdevice_a_map_);
              MMDevice_Map::ENTRY *entry = 0;
              for (;iterator.next (entry) !=  0;iterator.advance ())
                {
                  flowspec = entry->int_id_.flowspec_;
                }
            }
          else
            {
              flowspec = the_spec;
            }

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO_StreamCtrl::modify_QoS\n"));


          for (u_int i=0;i < flowspec.length ();i++)
            {
              TAO_Forward_FlowSpec_Entry entry;
              entry.parse (flowspec [i].in ());
              int direction = entry.direction ();
              if (direction == 0)
                {
                  in_flowspec.length (in_index + 1);
                  in_flowspec [in_index++] = CORBA::string_dup (entry.entry_to_string ());
                }
              else
                {
                  out_flowspec.length (out_index + 1);
                  out_flowspec [out_index++] = CORBA::string_dup (entry.entry_to_string ());
                }
            }

          if (in_flowspec.length () != 0)
            {
              this->vdev_a_->modify_QoS (new_qos, in_flowspec ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          if (out_flowspec.length () != 0)
            {
              this->vdev_b_->modify_QoS (new_qos, out_flowspec ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamCtrl::modify_QoS");
          return 0;
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (0);

    }
  return 1;
}

// ----------------------------------------------------------------------
// TAO_MCastConfigIf
// ----------------------------------------------------------------------

TAO_MCastConfigIf::TAO_MCastConfigIf (void)
  :peer_list_iterator_ (peer_list_)
{
}

TAO_MCastConfigIf::~TAO_MCastConfigIf (void)
{
  //no-op
}

// In future this should be a multicast message instead of point-to-point unicasts.
CORBA::Boolean
TAO_MCastConfigIf::set_peer (CORBA::Object_ptr peer,
                             AVStreams::streamQoS & qos,
                             const AVStreams::flowSpec & flow_spec
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::QoSRequestFailed,
                   AVStreams::streamOpFailed))
{
  ACE_TRY
    {
      Peer_Info *info;
      ACE_NEW_RETURN (info,
                      Peer_Info,
                      0);
      info->peer_ = AVStreams::VDev::_narrow (peer ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      info->qos_ = qos;
      info->flow_spec_ = flow_spec;
      this->peer_list_.insert_tail (info);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_MCastConfigIf::set_peer");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

// In future this should be a multicast message instead of point-to-point unicasts.
void
TAO_MCastConfigIf::configure (const CosPropertyService::Property & a_configuration
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Peer_Info *info;
  ACE_TRY
    {
      for (this->peer_list_iterator_.first ();
           (info = this->peer_list_iterator_.next ()) != 0;
           this->peer_list_iterator_.advance ())
        {
          info->peer_->configure (a_configuration ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_MCastConfigIf::set_configure");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}


void
TAO_MCastConfigIf::set_initial_configuration (const CosPropertyService::Properties &initial
                                              ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->initial_configuration_ = initial;
}

// In future this should be a multicast message instead of point-to-point unicasts.
void
TAO_MCastConfigIf::set_format (const char * flowName,
                               const char * format_name
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported))
{
  Peer_Info *info;
  ACE_TRY
    {
      for (this->peer_list_iterator_.first ();
           (info = this->peer_list_iterator_.next ()) != 0;
           this->peer_list_iterator_.advance ())
        {
          if (this->in_flowSpec (info->flow_spec_, flowName))
            {
              info->peer_->set_format (flowName, format_name ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_MCastConfigIf::set_format");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// In future this should be a multicast message instead of point-to-point unicasts.
void
TAO_MCastConfigIf::set_dev_params (const char * flowName,
                                   const CosPropertyService::Properties & new_params
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::PropertyException,
                   AVStreams::streamOpFailed))
{
  Peer_Info *info;
  ACE_TRY
    {

      for (this->peer_list_iterator_.first ();
           (info = this->peer_list_iterator_.next ()) != 0;
           this->peer_list_iterator_.advance ())
        {
          if (this->in_flowSpec (info->flow_spec_, flowName))
            {
              info->peer_->set_dev_params (flowName, new_params ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_MCastConfigIf::set_dev_params");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

int
TAO_MCastConfigIf::in_flowSpec (const AVStreams::flowSpec& flow_spec, const char *flow_name)
{
  int len = strlen (flow_name);
  for (u_int i=0;i<flow_spec.length ();i++)
    if (ACE_OS::strncmp (flow_spec[i], flow_name, len) == 0)
      {
        return 1;
      }
  return 0;
}

// ----------------------------------------------------------------------
// TAO_Base_StreamEndPoint
// ----------------------------------------------------------------------

TAO_Base_StreamEndPoint::TAO_Base_StreamEndPoint (void)
{
}

TAO_Base_StreamEndPoint::~TAO_Base_StreamEndPoint (void)
{
  Flow_Handler_Map_Iterator iter = this->flow_handler_map_.begin();
  Flow_Handler_Map_Entry *entry = 0;

  for (;iter.next (entry) != 0; iter.advance ())
    this->flow_handler_map_.unbind (entry->ext_id_);
}

int
TAO_Base_StreamEndPoint::handle_close (void)
{
  // This method should not be defined, but EGCS complains endlessly
  // about it.
  return -1;
}

int
TAO_Base_StreamEndPoint::handle_open (void)
{
  return 0;
}

int
TAO_Base_StreamEndPoint::handle_stop (const AVStreams::flowSpec &
                                      ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

int
TAO_Base_StreamEndPoint::handle_start (const AVStreams::flowSpec &
                                       ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

int
TAO_Base_StreamEndPoint::handle_destroy (const AVStreams::flowSpec &
                                         ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

// The following function is for backward compatibility.
CORBA::Boolean
TAO_Base_StreamEndPoint::handle_preconnect (AVStreams::flowSpec &)
{
  return 1;
}

// The following function is for backward compatibility.
CORBA::Boolean
TAO_Base_StreamEndPoint::handle_postconnect (AVStreams::flowSpec &)
{
  return 1;
}

// The following function is for backward compatibility.
CORBA::Boolean
TAO_Base_StreamEndPoint::handle_connection_requested (AVStreams::flowSpec &
                                                 ACE_ENV_ARG_DECL_NOT_USED)
{
  return 1;
}

int
TAO_Base_StreamEndPoint::set_protocol_object (const char * /*flowname*/,
                                              TAO_AV_Protocol_Object * /*sfp_object*/)
{
  return -1;
}

int
TAO_Base_StreamEndPoint::get_callback (const char * /*flowname*/,
                                       TAO_AV_Callback *&/*sfp_callback*/)
{
  return -1;
}

int
TAO_Base_StreamEndPoint::get_control_callback (const char * /*flowname*/,
                                               TAO_AV_Callback *&/*sfp_callback*/)
{
  return -1;
}

void
TAO_Base_StreamEndPoint::set_flow_handler (const char *flowname,
                                           TAO_AV_Flow_Handler *handler)
{
  if(TAO_debug_level > 1)
  {
     ACE_DEBUG ((LM_DEBUG, "(%N,%l) TAO_Base_StreamEndPoint::set_flow_handler(), flowname: %s\n", flowname));
  }
  ACE_CString flow_name_key (flowname);
  if (this->flow_handler_map_.bind (flow_name_key, handler) != 0)
    ACE_ERROR ((LM_ERROR,
                "Error in storing flow handler\n"));
}

void
TAO_Base_StreamEndPoint::set_control_flow_handler (const char *flowname,
                                                   TAO_AV_Flow_Handler *handler)
{
  ACE_CString flow_name_key (flowname);
  if (this->control_flow_handler_map_.bind (flow_name_key, handler) != 0)
    ACE_ERROR ((LM_ERROR,
                "Error in storing control flow handler\n"));
}

// ----------------------------------------------------------------------
// TAO_StreamEndPoint
// ----------------------------------------------------------------------

// constructor.

TAO_StreamEndPoint::TAO_StreamEndPoint (void)
  :flow_count_ (0),
   flow_num_ (0),
   mcast_port_ (ACE_DEFAULT_MULTICAST_PORT+1)
{
  //is->mcast_addr_ = ACE_OS::inet_addr (ACE_DEFAULT_MULTICAST_ADDR);
  this->mcast_addr_.set (ACE_DEFAULT_MULTICAST_ADDR);
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "TAO_StreamEndPoint::TAO_StreamEndPoint::mcast_addr = %s", this->mcast_addr_.c_str ()));
  //  this->handle_open ();
}


CORBA::Boolean
TAO_StreamEndPoint::connect (AVStreams::StreamEndPoint_ptr responder,
                             AVStreams::streamQoS &qos,
                             const AVStreams::flowSpec &the_spec
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed,
                   AVStreams::streamOpFailed))
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "TAO_StreamEndPoint::Connect ()\n"));
  CORBA::Boolean retv = 0;
  this->peer_sep_ = AVStreams::StreamEndPoint::_duplicate (responder);
  ACE_TRY_EX (negotiate)
    {
      if (!CORBA::is_nil (this->negotiator_.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "NEGOTIATOR AVIALABLE\n"));

          CORBA::Any_var negotiator_any = responder->get_property_value ("Negotiator");

          AVStreams::Negotiator_ptr peer_negotiator;
          negotiator_any.in () >>= peer_negotiator;
          if (!CORBA::is_nil (peer_negotiator))
            {
              CORBA::Boolean result =
                this->negotiator_->negotiate (peer_negotiator,
                                              qos
                                              ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK_EX (negotiate);
              if (!result)
                if (TAO_debug_level > 0)
                  ACE_DEBUG ((LM_DEBUG, "TAO_StreamEndPoint::Connect (): negotiate failed\n"));
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamEndPoint::negotiate");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  ACE_TRY_EX (available_protocols)
    {
      if (this->protocols_.length () > 0)
        {
          // choose protocols based on what the remote endpoint can support.
          CORBA::Any_var protocols_any =
            responder->get_property_value ("AvailableProtocols" ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (available_protocols);
          AVStreams::protocolSpec peer_protocols;
          AVStreams::protocolSpec *temp_protocols;
          protocols_any.in () >>= temp_protocols;
          peer_protocols = *temp_protocols;
          for (u_int i=0;i<peer_protocols.length ();i++)
            {
              for (u_int j=0;j<this->protocols_.length ();j++)
                if (ACE_OS::strcmp (peer_protocols [i],
                                    this->protocols_[j]) == 0)
                  {
                    // we'll agree upon the first protocol that matches.
                    this->protocol_ = CORBA::string_dup (peer_protocols [i]);
                    break;
                  }
            }
        }
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "Availableprotocols property not defined\n"));
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  ACE_TRY
    {
      AVStreams::streamQoS network_qos;
      if (qos.length () > 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "QoS is Specified\n"));

          int result = this->translate_qos (qos,
                                            network_qos);
          if (result != 0)
            if (TAO_debug_level > 0)
              ACE_DEBUG ((LM_DEBUG,
                          "QoS translation failed\n"));

          this->qos ().set (network_qos);
        }


      AVStreams::flowSpec flow_spec (the_spec);
      this->handle_preconnect (flow_spec);

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_StreamEndPoint::connect: flow_spec_length = %d\n",
                    flow_spec.length ()));
      u_int i;
      for (i=0;i<flow_spec.length ();i++)
        {
          TAO_Forward_FlowSpec_Entry *entry = 0;
          ACE_NEW_RETURN (entry,
                          TAO_Forward_FlowSpec_Entry,
                          0);

          if (entry->parse (flow_spec[i]) == -1)
            return 0;

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "TAO_StreamEndPoint::Connect: %s\n",  entry->entry_to_string ()));

          this->forward_flow_spec_set.insert (entry);
        }

      int result =TAO_AV_CORE::instance ()->init_forward_flows (this,
                                                                this->forward_flow_spec_set,
                                                                TAO_AV_Core::TAO_AV_ENDPOINT_A,
                                                                flow_spec);


      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR, "%N:%l TAO_AV_Core::init_forward_flows failed\n"), 0);


      AVStreams::StreamEndPoint_var streamendpoint = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      retv = responder->request_connection (streamendpoint.in (),
                                            0,
                                            network_qos,
                                            flow_spec
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (TAO_debug_level > 0)
         ACE_DEBUG ((LM_DEBUG, "%N:%l request_connection returned %d\n", retv));

      if (retv == 0)
        return retv;
      for (i=0;i<flow_spec.length ();i++)
        {
          TAO_Reverse_FlowSpec_Entry *entry = 0;
          ACE_NEW_RETURN (entry,
                          TAO_Reverse_FlowSpec_Entry,
                          0);
          if (entry->parse (flow_spec[i].in ()) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Reverse_Flow_Spec_Set::parse failed\n"),
                              0);

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO_StreamEndPoint::Connect: Reverse Flow Spec %s\n",
                        entry->entry_to_string ()));

          this->reverse_flow_spec_set.insert (entry);
        }

      result = TAO_AV_CORE::instance ()->init_reverse_flows (this,
                                                             this->forward_flow_spec_set,
                                                             this->reverse_flow_spec_set,
                                                             TAO_AV_Core::TAO_AV_ENDPOINT_A);
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "TAO_AV_Core::init_reverse_flows failed\n"),
                          0);

      // Make the upcall to the app
      retv = this->handle_postconnect (flow_spec);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamEndPoint::connect");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return retv;
}

int
TAO_StreamEndPoint::translate_qos (const AVStreams::streamQoS& application_qos,
                                   AVStreams::streamQoS& network_qos)
{
  u_int len = application_qos.length ();
  network_qos.length (len);
  for (u_int i=0;i<len;i++)
    {
      network_qos [i].QoSType = application_qos [i].QoSType;
      network_qos [i].QoSParams = application_qos [i].QoSParams;
    }
  return 0;
}

// Stop the physical flow of data on the stream
// Empty the_spec --> apply to all flows

void
TAO_StreamEndPoint::stop (const AVStreams::flowSpec &flow_spec
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow))
{
  // Make the upcall into the app
  this->handle_stop (flow_spec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (flow_spec.length () > 0)
    {

      for (u_int i=0;i<flow_spec.length ();i++)
        {
          TAO_AV_FlowSpecSetItor end = this->forward_flow_spec_set.end ();
          for (TAO_AV_FlowSpecSetItor begin = this->forward_flow_spec_set.begin ();
               begin != end; ++begin)
            {
              if (ACE_OS::strcmp ((*begin)->flowname (), flow_spec [i].in ()) == 0)
                {
                  TAO_FlowSpec_Entry *entry = *begin;
                  //                  (*begin)->protocol_object ()->stop ();
                  entry->handler ()->stop (entry->role ());
                  if (entry->control_handler () != 0)
                    entry->control_handler ()->stop (entry->role ());
                  break;
                }
            }
        }
    }
  else
    {
      TAO_AV_FlowSpecSetItor end = this->forward_flow_spec_set.end ();
      for (TAO_AV_FlowSpecSetItor begin = this->forward_flow_spec_set.begin ();
           begin != end; ++begin)
        {
          TAO_FlowSpec_Entry *entry = *begin;
          //          entry->protocol_object ()->stop ();
          entry->handler ()->stop (entry->role ());
          if (entry->control_handler () != 0)
            entry->control_handler ()->stop (entry->role ());
        }
    }
}

// Start the physical flow of data on the stream
// Empty the_spec --> apply to all flows
void
TAO_StreamEndPoint::start (const AVStreams::flowSpec &flow_spec
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow))
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "TAO_StreamEndPoint::start\n"));
  // Make the upcall into the app
  this->handle_start (flow_spec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (flow_spec.length () > 0)
    {
      // Now call start on all the flow handlers.
      for (u_int i=0;i<flow_spec.length ();i++)
        {
          TAO_AV_FlowSpecSetItor end = this->forward_flow_spec_set.end ();
          for (TAO_AV_FlowSpecSetItor forward_begin = this->forward_flow_spec_set.begin ();
               forward_begin != end; ++forward_begin)
            {
              TAO_FlowSpec_Entry *entry = *forward_begin;
              if (ACE_OS::strcmp (entry->flowname (), flow_spec [i]) == 0)
                {
                  //                  entry->protocol_object ()->start ();
                  if (entry->handler () != 0)
                  {
                    entry->handler ()->start (entry->role ());
                  }
                  if (entry->control_handler () != 0)
                  {
                    entry->control_handler ()->start (entry->role ());
                  }
                }
            }

          end = this->reverse_flow_spec_set.end ();
          for (TAO_AV_FlowSpecSetItor reverse_begin = this->reverse_flow_spec_set.begin ();
               reverse_begin != end; ++reverse_begin)
            {
              TAO_FlowSpec_Entry *entry = *reverse_begin;
              if (ACE_OS::strcmp (entry->flowname (), flow_spec [i]) == 0)
                {
                  //                  entry->protocol_object ()->start ();
                  if (entry->handler () != 0)
                  {
                    entry->handler ()->start (entry->role ());
                  }
                  if (entry->control_handler () != 0)
                  {
                    entry->control_handler ()->start (entry->role ());
                  }
                }
            }
        }
    }
  else
    {
      TAO_AV_FlowSpecSetItor end = this->forward_flow_spec_set.end ();
      for (TAO_AV_FlowSpecSetItor forwardbegin = this->forward_flow_spec_set.begin ();
           forwardbegin != end; ++forwardbegin)
        {
          TAO_FlowSpec_Entry *entry = *forwardbegin;
          if (entry->handler () != 0)
            {
              entry->handler ()->start (entry->role ());
            }
          if (entry->control_handler () != 0)
            {
              entry->control_handler ()->start (entry->role ());
            }
        }

      end = this->reverse_flow_spec_set.end ();
      for (TAO_AV_FlowSpecSetItor reversebegin = this->reverse_flow_spec_set.begin ();
           reversebegin != end; ++reversebegin)
        {
          TAO_FlowSpec_Entry *entry = *reversebegin;
          //          entry->protocol_object ()->start ();
          if (entry->handler () != 0)
            {
              entry->handler ()->start (entry->role ());
            }
          if (entry->control_handler () != 0)
            {
              entry->control_handler ()->start (entry->role ());
            }
        }
    }
}

// Close the connection
void
TAO_StreamEndPoint::destroy (const AVStreams::flowSpec &flow_spec
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow))
{
  CORBA::Any_var vdev_any = this->get_property_value ("Related_VDev"
                                                      ACE_ENV_ARG_PARAMETER);
  CORBA::Any_var mc_str_any;
  CORBA::Object_var mc_obj;
  AVStreams::VDev_ptr vdev;
  // TODO: what if a real AVStreams::MediaControl is used ??
  Null_MediaCtrl_var media_ctrl;
  const char *media_ctrl_str;
  vdev_any.in() >>= vdev;
  mc_str_any = vdev->get_property_value ("Related_MediaCtrl"
                                         ACE_ENV_ARG_PARAMETER);
  mc_str_any.in() >>= media_ctrl_str;
  mc_obj = TAO_AV_CORE::instance()->orb()->string_to_object (media_ctrl_str
                                                             ACE_ENV_ARG_PARAMETER);
  media_ctrl = Null_MediaCtrl::_narrow (mc_obj.in());

  // deactivate the associated vdev and media ctrl
  PortableServer::ServantBase_var vdev_servant =
      TAO_AV_CORE::instance()->poa()->reference_to_servant (vdev);
  TAO_AV_Core::deactivate_servant (vdev_servant.in());
  PortableServer::ServantBase_var mc_servant =
      TAO_AV_CORE::instance()->poa()->reference_to_servant (media_ctrl.in());
  TAO_AV_Core::deactivate_servant (mc_servant.in());

  int result = TAO_AV_Core::deactivate_servant (this);
  if (result < 0)
    if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "TAO_StreamEndPoint::destroy failed\n"));

  if (flow_spec.length () > 0)
    {
      for (u_int i=0;i<flow_spec.length ();i++)
        {
          {
            TAO_AV_FlowSpecSetItor end = this->forward_flow_spec_set.end ();
            for (TAO_AV_FlowSpecSetItor begin = this->forward_flow_spec_set.begin ();
                 begin != end; ++begin)
              {
                TAO_FlowSpec_Entry *entry = *begin;
                TAO_Tokenizer flow_name (flow_spec [i], '\\');
                if (ACE_OS::strcmp (entry->flowname (), flow_name[0]) == 0)
                  {
                    if (entry->protocol_object ())
                      {
                        entry->protocol_object ()->destroy ();
                      }
                    break;
                  }
              }
          }
          {
            TAO_AV_FlowSpecSetItor end = this->reverse_flow_spec_set.end ();
            for (TAO_AV_FlowSpecSetItor begin = this->reverse_flow_spec_set.begin ();
                 begin != end; ++begin)
              {
                TAO_FlowSpec_Entry *entry = *begin;
                TAO_Tokenizer flow_name (flow_spec [i], '\\');
                if (ACE_OS::strcmp (entry->flowname (), flow_name[0]) == 0)
                  {
                    if (entry->protocol_object ())
                      {
                        entry->protocol_object ()->destroy ();
                      }
                    break;
                  }
              }
          }
        }
    }
  else
    {
      {
        TAO_AV_FlowSpecSetItor end = this->forward_flow_spec_set.end ();
        for (TAO_AV_FlowSpecSetItor begin = this->forward_flow_spec_set.begin ();
             begin != end; ++begin)
          {
            TAO_FlowSpec_Entry *entry = *begin;
            if (entry->protocol_object ())
              {

                ACE_CString control_flowname =
                    TAO_AV_Core::get_control_flowname (entry->flowname ());
                TAO_AV_CORE::instance()->remove_acceptor(entry->flowname());
                TAO_AV_CORE::instance()->remove_acceptor(control_flowname.c_str());

                entry->protocol_object ()->destroy ();
              }
          }
      }
      {
        TAO_AV_FlowSpecSetItor end = this->reverse_flow_spec_set.end ();
        for (TAO_AV_FlowSpecSetItor begin = this->reverse_flow_spec_set.begin ();
             begin != end; ++begin)
          {
            TAO_FlowSpec_Entry *entry = *begin;
            if (entry->protocol_object ())
              {
                ACE_CString control_flowname =
                    TAO_AV_Core::get_control_flowname (entry->flowname ());
                TAO_AV_CORE::instance()->remove_connector(entry->flowname());
                TAO_AV_CORE::instance()->remove_connector(control_flowname.c_str());
                entry->protocol_object ()->destroy ();

              }
          }
      }
    }

  // Make the upcall into the app
  //  this->handle_destroy (the_spec ACE_ENV_ARG_PARAMETER);
  //
}

// Called by our peer endpoint, requesting us to establish
// a connection
CORBA::Boolean
TAO_StreamEndPoint::request_connection (AVStreams::StreamEndPoint_ptr /*initiator*/,
                                        CORBA::Boolean /*is_mcast*/,
                                        AVStreams::streamQoS &qos,
                                        AVStreams::flowSpec &flow_spec
                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpDenied,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed,
                   AVStreams::FPError))

{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "\n(%P|%t) TAO_StreamEndPoint::request_connection called"));


  int result = 0;
  ACE_TRY
    {
      AVStreams::streamQoS network_qos;
      if (qos.length () > 0)
        {
         if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "QoS is Specified\n"));

          int result = this->translate_qos (qos, network_qos);
          if (result != 0)
            if (TAO_debug_level > 0)
              ACE_DEBUG ((LM_DEBUG, "QoS translation failed\n"));

          this->qos ().set (network_qos);
        }

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "\n(%P|%t) TAO_StreamEndPoint::request_connection: "
                    "flowspec has length = %d"
                    "and the strings are:",
                    flow_spec.length ()));
      u_int i;

      for (i=0;i<flow_spec.length ();i++)
        {
          TAO_Forward_FlowSpec_Entry *entry = 0;
          ACE_NEW_RETURN (entry,
                          TAO_Forward_FlowSpec_Entry,
                          0);

          if(TAO_debug_level > 0)
             ACE_DEBUG(( LM_DEBUG, "%N:%l Parsing flow spec: %s\n", flow_spec[i].in() ));

          if (entry->parse (flow_spec[i]) == -1)
          {
            if (TAO_debug_level > 0)
              ACE_DEBUG ((LM_DEBUG, "%N:%l Error parsing flow_spec: %s\n", flow_spec[i].in() ));
            return 0;
          }
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO_StreamEndPoint::request_connection Flow Spec %s",
                        entry->entry_to_string ()));

          this->forward_flow_spec_set.insert (entry);
        }

      result = TAO_AV_CORE::instance ()->init_forward_flows (this,
                                                             this->forward_flow_spec_set,
                                                             TAO_AV_Core::TAO_AV_ENDPOINT_B,
                                                             flow_spec);

      if (result < 0)
        return 0;

      // Make the upcall to the app
      result = this->handle_connection_requested (flow_spec ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO_StreamEndpoint::request_connection");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return result;
}

int
TAO_StreamEndPoint::change_qos (AVStreams::streamQoS &new_qos,
                                const AVStreams::flowSpec &the_flows
                                ACE_ENV_ARG_DECL_NOT_USED)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_StreamEndPoint::change_qos\n"));

  TAO_AV_QoS qos (new_qos);
  for (int i = 0; (unsigned) i < the_flows.length (); i++)
    {
      TAO_Forward_FlowSpec_Entry entry;
      entry.parse (the_flows [i].in ());
      ACE_CString flow_name_key (entry.flowname ());
      Flow_Handler_Map_Entry *handler_entry;
      if (this->flow_handler_map_.find (flow_name_key,
                                        handler_entry) == 0)
        {
          AVStreams::QoS flow_qos;
          if (qos.get_flow_qos (entry.flowname (), flow_qos) != 0)
            ACE_DEBUG ((LM_DEBUG,
                        "New QoS for the flow %s is not specified\n",
                        entry.flowname ()));
          int result;
          result = handler_entry->int_id_->change_qos (flow_qos);
          if (result != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Modifying QoS Failed\n"),
                              -1);

        }
    }
  return 0;
}

// Refers to modification of transport QoS.
CORBA::Boolean
TAO_StreamEndPoint::modify_QoS (AVStreams::streamQoS &new_qos,
                                const AVStreams::flowSpec &the_flows
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed))
{
  if (TAO_debug_level > 0)
  ACE_DEBUG ((LM_DEBUG,
              "TAO_StreamEndPoint::modify_QoS\n"));

  int result =  this->change_qos (new_qos, the_flows ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (result != 0)
    return 0;

  return 1;

}

// Sets the list of protocols this streamendpoint can understand.

CORBA::Boolean
TAO_StreamEndPoint::set_protocol_restriction (const AVStreams::protocolSpec &protocols
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      CORBA::Any protocol_restriction_any;

      protocol_restriction_any <<= protocols;
      this->define_property ("ProtocolRestriction",
                             protocol_restriction_any
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->protocols_ = protocols;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamEndPoint::set_protocol_restriction");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}


void
TAO_StreamEndPoint::disconnect (const AVStreams::flowSpec &the_spec
                                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::streamOpFailed))
{
  ACE_UNUSED_ARG (the_spec);
}

// Sets the status of the flow protocol.

void
TAO_StreamEndPoint::set_FPStatus (const AVStreams::flowSpec &/*the_spec*/,
                                  const char *fp_name,
                                  const CORBA::Any &fp_settings
                                  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::FPError))
{
  if (ACE_OS::strcmp (fp_name, "SFP1.0") != 0)
    return;
  fp_settings >>= this->sfp_status_;
  // @@Naga: We should call set_FPStatus on all the protocol objects.
}


CORBA::Object_ptr
TAO_StreamEndPoint::get_fep (const char *flow_name
                             ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported,
                   AVStreams::noSuchFlow))
{
  ACE_CString fep_name_key (flow_name);
  FlowEndPoint_Map::ENTRY *fep_entry = 0;
  if (this->fep_map_.find (fep_name_key, fep_entry) == 0)
    return fep_entry->int_id_;
  return 0;
}

char*
TAO_StreamEndPoint::add_fep_i_add_property (AVStreams::FlowEndPoint_ptr fep
                                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::notSupported,
                     AVStreams::streamOpFailed))
{
  char* tmp;
  ACE_NEW_RETURN (tmp,
                  char[64],
                  0);
  CORBA::String_var flow_name = tmp;

  ACE_TRY
    {
      // exception implies the flow name is not defined and is system
      // generated.
      ACE_OS::sprintf (tmp,
                       "flow%d",
                       this->flow_num_++);
      CORBA::Any flowname_any;
      flowname_any <<= flow_name.in ();
      fep->define_property ("Flow",
                            flowname_any
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO_StreamEndPoint::add_fep");
      return 0;
    }
  ACE_ENDTRY;
  return flow_name._retn ();
}

char*
TAO_StreamEndPoint::add_fep_i (AVStreams::FlowEndPoint_ptr fep
                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::notSupported,
                     AVStreams::streamOpFailed))
{
  CORBA::String_var flow_name;
  ACE_TRY
    {
      CORBA::Any_var flow_name_any =
        fep->get_property_value ("FlowName" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      const char *tmp;
      flow_name_any >>= tmp;
      flow_name = CORBA::string_dup (tmp);
    }
  ACE_CATCHANY
    {
      flow_name =
        this->add_fep_i_add_property (fep ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  ACE_ENDTRY;
  return flow_name._retn ();
}

char *
TAO_StreamEndPoint::add_fep (CORBA::Object_ptr fep_obj
                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::notSupported,
                     AVStreams::streamOpFailed))
{
  AVStreams::FlowEndPoint_var fep =
    AVStreams::FlowEndPoint::_narrow (fep_obj ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::String_var flow_name =
    this->add_fep_i (fep.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_TRY
    {
      fep->lock (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      // Add it to the sequence of flowNames supported.
      // put the flowname and the flowendpoint in a hashtable.
      ACE_CString fep_name_key (CORBA::string_dup (flow_name.in ()));
      if (this->fep_map_.bind (fep_name_key, AVStreams::FlowEndPoint::_duplicate (fep.in ())) != 0)
        {
          ACE_THROW_RETURN (AVStreams::streamOpFailed (), 0);
        }
      // increment the flow count.
      this->flow_count_++;
      this->flows_.length (this->flow_count_);
      this->flows_[this->flow_count_-1] = flow_name;
      // define/modify the "Flows" property.
      CORBA::Any flows_any;
      flows_any <<= this->flows_;
      this->define_property ("Flows",
                             flows_any
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamEndPoint::add_fep");
      return 0;
    }
  ACE_ENDTRY;
  return flow_name._retn ();
}


void
TAO_StreamEndPoint::remove_fep (const char *flow_name
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::notSupported,
                     AVStreams::streamOpFailed))
{
  ACE_TRY
    {
      ACE_CString fep_name_key (flow_name);
      AVStreams::FlowEndPoint_ptr fep_entry = 0;
      // Remove the fep from the hash table.
      if (this->fep_map_.unbind (fep_name_key, fep_entry)!= 0)
        ACE_THROW (AVStreams::streamOpFailed ());
      // redefine the "Flows" property
      AVStreams::flowSpec new_flows (this->flows_.length ());
      for (u_int i=0, j=0 ; i <this->flows_.length (); i++)
        if (ACE_OS::strcmp (flow_name, this->flows_[i]) != 0)
          new_flows[j++] = this->flows_[i];

      CORBA::Any flows;
      flows <<= new_flows;
      this->flows_ = new_flows;
      this->define_property ("Flows",
                             flows
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamEndPoint::remove_fep");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Sets the negotiator object.
void
TAO_StreamEndPoint::set_negotiator (AVStreams::Negotiator_ptr new_negotiator
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      CORBA::Any negotiator;
      negotiator <<= new_negotiator;
      this->define_property ("Negotiator",
                             negotiator
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->negotiator_ = AVStreams::Negotiator::_duplicate (new_negotiator);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamEndPoint::set_negotiator");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}


// Sets the public key used for this streamendpoint.
void
TAO_StreamEndPoint::set_key (const char *flow_name,
                             const AVStreams::key & the_key
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->key_ = the_key;
      CORBA::Any PublicKey;
      PublicKey <<= the_key;
      char PublicKey_property [BUFSIZ];
      ACE_OS::sprintf (PublicKey_property, "%s_PublicKey", flow_name);
      this->define_property (PublicKey_property,
                             PublicKey
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamEndPoint::set_key");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Set the source id.
void
TAO_StreamEndPoint::set_source_id (CORBA::Long source_id
                                   ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->source_id_ = source_id;
}

CORBA::Boolean
TAO_StreamEndPoint::multiconnect (AVStreams::streamQoS &/*the_qos*/,
                                  AVStreams::flowSpec &/*flow_spec*/
                                  ACE_ENV_ARG_DECL_NOT_USED)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "TAO_StreamEndPoint::multiconnect\n"));
  return 0;
}

TAO_StreamEndPoint::~TAO_StreamEndPoint (void)
{
  //this->handle_close ();
  TAO_AV_FlowSpecSetItor begin = this->forward_flow_spec_set.begin ();
  TAO_AV_FlowSpecSetItor end = this->forward_flow_spec_set.end ();

  int i=0;
  // @@ Naga: Will the iterator always give the entries in the order of insertion.
  // or is it an implementation fact of ACE containers.
  for ( ; begin != end; ++begin, ++i)
    {
//       if (i >= FLOWSPEC_MAX)
//         {
          TAO_FlowSpec_Entry *entry = *begin;
          delete entry;
          //        }
    }
  begin = this->reverse_flow_spec_set.begin ();
  end = this->reverse_flow_spec_set.end ();
  i = 0;
  for (; begin != end; ++begin)
    {
//       if (i >= FLOWSPEC_MAX)
//         {
          TAO_FlowSpec_Entry *entry = *begin;
          delete entry;
          //        }
    }
}


// ----------------------------------------------------------------------
// TAO_StreamEndPoint_A
// ----------------------------------------------------------------------

TAO_StreamEndPoint_A::TAO_StreamEndPoint_A (void)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "(%P|%t) TAO_StreamEndPoint_A::TAO_StreamEndPoint_A: created\n"));
}

// IP Multicast style connect.
CORBA::Boolean
TAO_StreamEndPoint_A::multiconnect (AVStreams::streamQoS &stream_qos,
                                    AVStreams::flowSpec &flow_spec
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed,
                   AVStreams::streamOpFailed))
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "TAO_StreamEndPointA::multiconnect\n"));
  ACE_TRY
    {
      int result = 0;
      TAO_AV_QoS qos (stream_qos);
      for (u_int i=0;i< flow_spec.length ();i++)
        {
          TAO_Forward_FlowSpec_Entry *forward_entry;
          ACE_NEW_RETURN (forward_entry,
                          TAO_Forward_FlowSpec_Entry,
                          0);
          forward_entry->parse (flow_spec[i]);
          ACE_CString mcast_key (forward_entry->flowname ());
          AVStreams::FlowEndPoint_ptr flow_endpoint = AVStreams::FlowEndPoint::_nil ();
          // @@Naga: There is a problem in the full profile case for multiconnect. Since
          // multiconnect on sep_a is called everytime a sink is added and if called for
          // the same flow twice, the following code will just call add producer on the flow connection.
          // It is however very hard to find out if the flow producer is already in the flow connection
          // since comparing object references will not work and the flowproducer reference is
          // generated by _narrow. Our only hope is that _narrow on the same fep will return the same
          // pointer for the flowproducer in which case we can find out if the flowproducer exists in
          // fep set for that flowconnection.
          if (this->fep_map_.find (mcast_key, flow_endpoint) == 0)
            {
              ACE_TRY_EX (narrow)
                {
                  AVStreams::QoS flow_qos;
                  result = qos.get_flow_qos (forward_entry->flowname (), flow_qos);
                  if (result < 0)
                    if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "QoS not found for %s", forward_entry->flowname ()));
                  // Narrow it to FlowProducer.
                  AVStreams::FlowProducer_var producer = AVStreams::FlowProducer::_narrow (flow_endpoint ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK_EX (narrow);
                  // Else narrow succeeeded.
                  if (!CORBA::is_nil (producer.in ()))
                    {
                      AVStreams::FlowConnection_var flow_connection;
                      ACE_TRY_EX (flow_connection)
                        {
                          if (CORBA::is_nil (this->streamctrl_.in ()))
                              {
                                CORBA::Any_var streamctrl_any;
                                streamctrl_any = this->get_property_value ("Related_StreamCtrl"
                                                                           ACE_ENV_ARG_PARAMETER);
                                ACE_TRY_CHECK;
                                AVStreams::StreamCtrl_ptr streamctrl;
                                streamctrl_any.in () >>= streamctrl;
                                this->streamctrl_ = AVStreams::StreamCtrl::_duplicate (streamctrl);
                              }

                          CORBA::Object_var flow_connection_obj =
                            this->streamctrl_->get_flow_connection (forward_entry->flowname ()
                                                                    ACE_ENV_ARG_PARAMETER);
                          ACE_TRY_CHECK_EX (flow_connection);
                          flow_connection = AVStreams::FlowConnection::_narrow (flow_connection_obj.in ()
                                                                                ACE_ENV_ARG_PARAMETER);
                          ACE_TRY_CHECK_EX (flow_connection);
                        }
                      ACE_CATCHANY
                        {
                          TAO_FlowConnection *flowConnection;
                          ACE_NEW_RETURN (flowConnection,
                                          TAO_FlowConnection,
                                          0);
                          //@@ Strategize the multicast address allocation.
                          flowConnection->set_mcast_addr (this->mcast_addr_, this->mcast_port_);
                          this->mcast_port_++;
                          flowConnection->set_protocol (forward_entry->carrier_protocol_str ());
                          flow_connection = flowConnection->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
                          ACE_TRY_CHECK;
                          this->streamctrl_->set_flow_connection (forward_entry->flowname (),
                                                                  flow_connection.in ()
                                                                  ACE_ENV_ARG_PARAMETER);
                          ACE_TRY_CHECK;
                        }
                      ACE_ENDTRY;
                      ACE_CHECK_RETURN (0);
                      if (ACE_OS::strcmp (forward_entry->flow_protocol_str (), "") != 0)
                        {
                          CORBA::Any fp_settings;
                          flow_connection->use_flow_protocol (forward_entry->flow_protocol_str (),
                                                              fp_settings
                                                              ACE_ENV_ARG_PARAMETER);
                          ACE_TRY_CHECK;
                        }
                      result = flow_connection->add_producer (producer.in (),
                                                              flow_qos
                                                              ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK;
                      if (result == 0)
                        ACE_ERROR_RETURN ((LM_ERROR, "TAO_StreamEndPoint_A::multiconnect: add_producer failed\n"), 0);
                    }
                }
              ACE_CATCHANY
                {
                  // Narrow failed and since its not a flowproducer its an error.
                  ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "FlowProducer::_narrow");
                  ACE_ERROR_RETURN ((LM_ERROR, "sep_a doesn't contain a flowproducer"), 0);
                }
              ACE_ENDTRY;
              ACE_CHECK_RETURN (0);
            }
          else
            {
              ACE_INET_Addr *mcast_addr;
              TAO_FlowSpec_Entry *entry = 0;
              result = this->mcast_entry_map_.find (mcast_key, entry);
              if (result == 0)
                {
                  mcast_addr = ACE_dynamic_cast (ACE_INET_Addr *, entry->address ());
                  char str_addr [BUFSIZ];
                  result = mcast_addr->addr_to_string (str_addr, BUFSIZ);
                  if (result < 0)
                    ACE_ERROR_RETURN ((LM_ERROR, "TAO_StreamEndPointA::multiconnect ::addr_to_string failed\n"), 0);
                  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "TAO_StreamEndPoint_A::multiconnect:%s\n", str_addr));
                  TAO_Forward_FlowSpec_Entry new_entry (entry->flowname (),
                                                        entry->direction_str (),
                                                        entry->format (),
                                                        entry->flow_protocol_str (),
                                                        entry->carrier_protocol_str (),
                                                        entry->address ());
                  flow_spec[i] = CORBA::string_dup (new_entry.entry_to_string ());
                }
              else
                {

                  switch (forward_entry->direction ())
                    {
                    case TAO_FlowSpec_Entry::TAO_AV_DIR_IN:
                      {
                        ACE_NEW_RETURN (mcast_addr,
                                        ACE_INET_Addr,
                                        0);
                        mcast_addr->set (this->mcast_port_, this->mcast_addr_.c_str ());
                        this->mcast_port_++;
                        char buf[BUFSIZ];
                        mcast_addr->addr_to_string (buf, BUFSIZ);
                        if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "%s\n", buf));
                        TAO_Forward_FlowSpec_Entry *new_entry;
                        ACE_NEW_RETURN (new_entry,
                                        TAO_Forward_FlowSpec_Entry (forward_entry->flowname (),
                                                                    forward_entry->direction_str (),
                                                                    forward_entry->format (),
                                                                    forward_entry->flow_protocol_str (),
                                                                    forward_entry->carrier_protocol_str (),
                                                                    mcast_addr),
                                        0);
                        flow_spec[i] = CORBA::string_dup (new_entry->entry_to_string ());
                        //new_entry->is_multicast (1);

                        this->forward_flow_spec_set.insert (new_entry);
                        TAO_AV_Acceptor_Registry *acceptor_registry = TAO_AV_CORE::instance ()->acceptor_registry ();
                        result = acceptor_registry->open (this,
                                                          TAO_AV_CORE::instance (),
                                                          this->forward_flow_spec_set);
                        if (result < 0)
                          ACE_ERROR_RETURN ((LM_ERROR, "Acceptor_Registry::open failed\n"), 0);
                        result = this->mcast_entry_map_.bind (mcast_key, new_entry);
                        if (result < 0)
                          ACE_ERROR_RETURN ((LM_ERROR, "mcast_entry::bind failed"), 0);
                      }
                      break;
                    case TAO_FlowSpec_Entry::TAO_AV_DIR_OUT:
                      // OUT implies we're the sink.
                      break;
                    default:
                      break;
                    }
                }
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamEndPoint_A::multiconnect");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

// ATM style Multicast is not supported yet.
CORBA::Boolean
TAO_StreamEndPoint_A::connect_leaf (AVStreams::StreamEndPoint_B_ptr /* the_ep */,
                                    AVStreams::streamQoS & /* the_qos */,
                                    const AVStreams::flowSpec & /* the_flows */
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed,
                   AVStreams::notSupported))
{
  ACE_THROW_RETURN (AVStreams::notSupported (), 0);
}

// Multicast not supported yet.
void
TAO_StreamEndPoint_A::disconnect_leaf (AVStreams::StreamEndPoint_B_ptr /* the_ep */,
                                       const AVStreams::flowSpec & /* theSpec */
                                       ACE_ENV_ARG_DECL)

  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::noSuchFlow,
                   AVStreams::notSupported))
{

  ACE_THROW (AVStreams::notSupported ());

}

TAO_StreamEndPoint_A::~TAO_StreamEndPoint_A (void)
{
}

// ----------------------------------------------------------------------
// TAO_StreamEndPoint_B
// ----------------------------------------------------------------------

TAO_StreamEndPoint_B::TAO_StreamEndPoint_B (void)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,
                                       "\n(%P|%t) TAO_StreamEndPoint_B::TAO_StreamEndPoint_B: created"));
}

CORBA::Boolean
TAO_StreamEndPoint_B::multiconnect (AVStreams::streamQoS &stream_qos,
                                    AVStreams::flowSpec &flow_spec
                                    ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::streamOpFailed,
                     AVStreams::noSuchFlow,
                     AVStreams::QoSRequestFailed,
                     AVStreams::FPError))
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "TAO_StreamEndPoint_B::multiconnect\n"));
  ACE_TRY
    {
      int result = 0;
      TAO_AV_QoS qos (stream_qos);
      for (u_int i=0;i< flow_spec.length ();i++)
        {
          TAO_Forward_FlowSpec_Entry *forward_entry;
          ACE_NEW_RETURN (forward_entry,
                          TAO_Forward_FlowSpec_Entry,
                          0);
          forward_entry->parse (flow_spec[i]);
          ACE_CString mcast_key (forward_entry->flowname ());
          AVStreams::FlowEndPoint_var flow_endpoint;
          if (this->fep_map_.find (mcast_key, flow_endpoint.out ()) == 0)
            {
              AVStreams::FlowConsumer_var consumer;
              ACE_TRY_EX (narrow)
                {
                  consumer = AVStreams::FlowConsumer::_narrow (flow_endpoint.in () ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK_EX (narrow);
                }
              ACE_CATCHANY
                {
                  ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "FlowConsumer::_narrow");
                  ACE_ERROR_RETURN ((LM_ERROR, "sep_b doesn't contain a flowconsumer"), 0);
                }
              ACE_ENDTRY;
              ACE_CHECK_RETURN (0);
              AVStreams::QoS flow_qos;
              result = qos.get_flow_qos (forward_entry->flowname (), flow_qos);
              if (result < 0)
                if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "QoS not found for %s", forward_entry->flowname ()));
              AVStreams::FlowConnection_var flow_connection;
              ACE_TRY_EX (flow_connection)
                {
                  if (CORBA::is_nil (this->streamctrl_.in ()))
                    {
                      CORBA::Any_var streamctrl_any;
                      streamctrl_any = this->get_property_value ("Related_StreamCtrl"
                                                                 ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK;
                      AVStreams::StreamCtrl_ptr streamctrl;
                      streamctrl_any.in () >>= streamctrl;
                      this->streamctrl_ = AVStreams::StreamCtrl::_duplicate (streamctrl);
                    }
                  CORBA::Object_var flow_connection_obj =
                    this->streamctrl_->get_flow_connection (forward_entry->flowname ()
                                                            ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK_EX (flow_connection);
                  flow_connection = AVStreams::FlowConnection::_narrow (flow_connection_obj.in ()
                                                                        ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK_EX (flow_connection);
                }
              ACE_CATCHANY
                {
                  ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamEndPoint_B::multiconnect::get_flow_connection");
                  return 0;
                }
              ACE_ENDTRY;
              ACE_CHECK_RETURN (0);
              result = flow_connection->add_consumer (consumer.in (),
                                                      flow_qos
                                                      ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              if (result == 0)
                ACE_ERROR_RETURN ((LM_ERROR, "TAO_StreamEndPoint_B::multiconnect:add_consumer failed\n"), 0);
            }
          else
            {
              TAO_FlowSpec_Entry *mcast_entry = 0;
              ACE_INET_Addr *mcast_addr;
              mcast_addr = ACE_dynamic_cast (ACE_INET_Addr *, forward_entry->address ());
              if (mcast_addr == 0)
                ACE_ERROR_RETURN ((LM_ERROR, "TAO_StreamEndPoint_B::multiconnect::Address missing in flowspec_entry\n"), 0);
              result = this->mcast_entry_map_.find (mcast_key, mcast_entry);
              if (result == 0)
                {
                  ACE_ERROR_RETURN ((LM_ERROR, "TAO_StreamEndPoint_B::multiconnect::handler already found\n"), 0);
                }
              else
                {
                  switch (forward_entry->direction ())
                    {
                    case TAO_FlowSpec_Entry::TAO_AV_DIR_IN:
                      {
                        // IN means we're the sink.
                        // @@ We have to take care of this.
                        //                 result = this->make_dgram_mcast_flow_handler (mcast_dgram);
                        //                 if (result < 0)
                        //                   return 0;

                        this->forward_flow_spec_set.insert (forward_entry);
                        TAO_AV_Connector_Registry *connector_registry = TAO_AV_CORE::instance ()->connector_registry ();
                        result = connector_registry->open (this,
                                                           TAO_AV_CORE::instance (),
                                                           this->forward_flow_spec_set);
                        if (result < 0)
                          ACE_ERROR_RETURN ((LM_ERROR, "connector_registry::open failed\n"), 0);
                        result = this->mcast_entry_map_.bind (mcast_key, forward_entry);
                        if (result < 0)
                          ACE_ERROR_RETURN ((LM_ERROR, "dgram_mcast_handler::bind failed"), 0);
                      }
                      break;
                    case TAO_FlowSpec_Entry::TAO_AV_DIR_OUT:
                      // OUT implies we're the source., which is an error.
                      break;
                    default:
                      break;
                    }
                }
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_StreamEndPoint_B::multiconnect");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

TAO_StreamEndPoint_B::~TAO_StreamEndPoint_B (void)
{
}

// ----------------------------------------------------------------------
// TAO_VDev
// ----------------------------------------------------------------------

TAO_VDev::TAO_VDev (void)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) TAO_VDev::TAO_VDev: created\n"));
}

// StreamCtrl will call this to give us a reference to itself, and to
// our peer vdev..
CORBA::Boolean
TAO_VDev::set_peer (AVStreams::StreamCtrl_ptr the_ctrl,
                    AVStreams::VDev_ptr the_peer_dev,
                    AVStreams::streamQoS &the_qos,
                    const AVStreams::flowSpec &the_spec
                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed,
                   AVStreams::streamOpFailed))
{
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (the_spec);

  CORBA::Boolean result = 0;
  ACE_TRY
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) TAO_VDev::set_peer: called"));

      CORBA::String_var ior = TAO_ORB_Core_instance ()->orb ()->object_to_string (the_peer_dev
                                                                                  ACE_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;

      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) TAO_VDev::set_peer: my peer is %s\n",
                  ior.in ()));

      CORBA::Any anyval;
      anyval <<= ior.in ();
      this->define_property ("Related_VDev",
                             anyval
                             ACE_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;

      this->streamctrl_ = AVStreams::StreamCtrl::_duplicate (the_ctrl);
      this->peer_ = AVStreams::VDev::_duplicate (the_peer_dev);

      CORBA::Any_var anyptr;
      const char *media_ctrl_ior;
      anyptr = this->peer_->get_property_value ("Related_MediaCtrl"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      anyptr.in () >>= media_ctrl_ior;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "(%P|%t)The Media Control IOR is %s\n",
                    media_ctrl_ior));
      CORBA::Object_ptr media_ctrl_obj =
        TAO_ORB_Core_instance ()->orb ()->string_to_object
        (media_ctrl_ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      result = this->set_media_ctrl (media_ctrl_obj ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_VDev::set_peer");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return result;
}

CORBA::Boolean
TAO_VDev::set_media_ctrl (CORBA::Object_ptr media_ctrl
                          ACE_ENV_ARG_DECL)

{
//  ACE_UNUSED_ARG (media_ctrl);
  ACE_CHECK_RETURN (0);

  // FIX MEMORY LEAK:
  //  since the media ctrl is not stored or used, delete it.
  //  cleaned up several memory leaks (according to BoundsChecker)

  CORBA::release( media_ctrl);

  return 1;
}

// Sets the multicast VDev peer.
CORBA::Boolean
TAO_VDev::set_Mcast_peer (AVStreams::StreamCtrl_ptr /* the_ctrl */,
                          AVStreams::MCastConfigIf_ptr mcast_peer,
                          AVStreams::streamQoS &/* the_qos */,
                          const AVStreams::flowSpec &/* the_spec */
                          ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed,
                   AVStreams::streamOpFailed))
{
  this->mcast_peer_ = AVStreams::MCastConfigIf::_duplicate (mcast_peer);
  return 1;
}

// applications should override this to handle configuration changes.
void
TAO_VDev::configure (const CosPropertyService::Property &/*the_config_mesg*/
                     ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::PropertyException,
                   AVStreams::streamOpFailed))
{
}

// sets the media format used for the flowname as a property.
void
TAO_VDev::set_format (const char *flowName,
                      const char *format_name
                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported))
{
  ACE_TRY
    {
      if (flowName == 0 || format_name == 0)
        ACE_ERROR ((LM_ERROR, "TAO_VDev::set_format: flowName or format_name is null\n"));
      char format_property [BUFSIZ];
      ACE_OS::sprintf (format_property, "%s_currFormat", flowName);
      CORBA::Any format;
      format <<= format_name;
      this->define_property (format_property,
                             format
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_VDev::set_format");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
  return;
}

// sets the device parameters for the flowname as a property.
void
TAO_VDev::set_dev_params (const char *flowName,
                          const CosPropertyService::Properties &new_params
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::PropertyException,
                   AVStreams::streamOpFailed))
{
  ACE_TRY
    {
      if (flowName == 0)
        ACE_ERROR ((LM_ERROR, "TAO_VDev::set_dev_params:flowName is null\n"));
      char devParams_property[BUFSIZ];
      ACE_OS::sprintf (devParams_property, "%s_devParams", flowName);
      CORBA::Any devParams;
      devParams <<= new_params;
      this->define_property (devParams_property,
                             devParams
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_VDev::set_dev_params");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
  return;
}

// QoS Modification should be handled by the application currently.
CORBA::Boolean
TAO_VDev::modify_QoS (AVStreams::streamQoS &the_qos,
                      const AVStreams::flowSpec &flowspec
                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed))
{
  if (TAO_debug_level > 0)
  ACE_DEBUG ((LM_DEBUG,
             "TAO_VDev::modify_QoS\n"));

  if (flowspec.length () != 0)
    {
      TAO_Forward_FlowSpec_Entry entry;
      entry.parse (flowspec [0]);
      int direction = entry.direction ();
      if (direction == 0)
        {
          AVStreams::StreamEndPoint_A_ptr sep_a;

          CORBA::Any_ptr streamendpoint_a_any =
          this->get_property_value ("Related_StreamEndpoint"
                                    ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          *streamendpoint_a_any >>= sep_a;
          if (sep_a != 0)
            {
              sep_a->modify_QoS (the_qos, flowspec ACE_ENV_ARG_PARAMETER);
              ACE_CHECK_RETURN (0);
            }
          else ACE_DEBUG ((LM_DEBUG,
                           "Stream EndPoint Not Found\n"));
        }
      else
        {
          AVStreams::StreamEndPoint_B_ptr sep_b;

          CORBA::Any_ptr streamendpoint_b_any =
          this->get_property_value ("Related_StreamEndpoint"
                                    ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
          *streamendpoint_b_any >>= sep_b;
          sep_b->modify_QoS (the_qos, flowspec ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
        }
  }
  return 1;
}

TAO_VDev::~TAO_VDev (void)
{
}

// ----------------------------------------------------------------------
// TAO_MMDevice
// ----------------------------------------------------------------------


TAO_MMDevice::TAO_MMDevice (TAO_AV_Endpoint_Strategy *endpoint_strategy)
  : endpoint_strategy_ (endpoint_strategy),
    flow_count_ (0),
    flow_num_ (0),
    stream_ctrl_ (0)
{
}

// create a streamctrl which is colocated with me, use that streamctrl
// to bind the peer_device with me.
AVStreams::StreamCtrl_ptr
TAO_MMDevice::bind (AVStreams::MMDevice_ptr peer_device,
                    AVStreams::streamQoS &the_qos,
                    CORBA::Boolean_out is_met,
                    const AVStreams::flowSpec &the_spec
                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed))
{
  AVStreams::StreamCtrl_ptr streamctrl (AVStreams::StreamCtrl::_nil ());
  ACE_TRY
    {
      ACE_UNUSED_ARG (is_met);
      ACE_NEW_RETURN (this->stream_ctrl_,
                      TAO_StreamCtrl,
                      0);
      AVStreams::MMDevice_var mmdevice = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->stream_ctrl_->bind_devs (peer_device,
                                     mmdevice.in (),
                                     the_qos,
                                     the_spec
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      streamctrl = this->stream_ctrl_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_MMDevice::bind");
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
                          const AVStreams::flowSpec &the_spec
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed))
{
  ACE_UNUSED_ARG (first_peer);
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (is_met);
  ACE_UNUSED_ARG (the_spec);
  ACE_CHECK_RETURN (0);

  return 0;
}

AVStreams::StreamEndPoint_ptr
TAO_MMDevice::create_A_B (MMDevice_Type type,
                          AVStreams::StreamCtrl_ptr streamctrl,
                          AVStreams::VDev_out the_vdev,
                          AVStreams::streamQoS &the_qos,
                          CORBA::Boolean_out met_qos,
                          char *&/*named_vdev*/,
                          const AVStreams::flowSpec &flow_spec
                          ACE_ENV_ARG_DECL)
{
  AVStreams::StreamEndPoint_A_ptr sep_a (AVStreams::StreamEndPoint_A::_nil ());
  AVStreams::StreamEndPoint_B_ptr sep_b (AVStreams::StreamEndPoint_B::_nil ());
  AVStreams::StreamEndPoint_ptr sep (AVStreams::StreamEndPoint::_nil ());
  ACE_TRY
    {
      switch (type)
        {
        case MMDEVICE_A:
          {
            if (this->endpoint_strategy_->create_A (sep_a,
                                                    the_vdev
                                                    ACE_ENV_ARG_PARAMETER) == -1)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "TAO_MMDevice::create_A_B (%P|%t) - "
                                 "error in create_A\n"),
                                0);
            sep = sep_a;
          }
          break;
        case MMDEVICE_B:
          {
            if (this->endpoint_strategy_->create_B (sep_b,
                                                    the_vdev
                                                    ACE_ENV_ARG_PARAMETER) == -1)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "TAO_MMDevice::create_A_B (%P|%t) - "
                                 "error in create_B\n"),
                                0);
            sep = sep_b;
          }
          break;
        default:
          break;
        }
      ACE_TRY_CHECK;
      if (this->fdev_map_.current_size () > 0)
        {
          // first set the vdev to be a nil pointer.
          the_vdev = AVStreams::VDev::_nil ();
          TAO_AV_QoS qos (the_qos);
          // create flowendpoints from the FDevs.
          for (u_int i=0;i<flow_spec.length ();i++)
            {
              TAO_Forward_FlowSpec_Entry forward_entry;
              forward_entry.parse (flow_spec[i]);
              ACE_CString flow_key (forward_entry.flowname ());
              AVStreams::FDev_ptr flow_dev;
              AVStreams::FlowConnection_var flowconnection = AVStreams::FlowConnection::_nil ();
              ACE_TRY_EX (flowconnection)
                {
                  // Get the flowconnection for this flow.
                  CORBA::Object_var flowconnection_obj =
                    streamctrl->get_flow_connection (forward_entry.flowname () ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK_EX (flowconnection);
                  if (!CORBA::is_nil (flowconnection_obj.in ()))
                    {
                      flowconnection = AVStreams::FlowConnection::_narrow (flowconnection_obj.in ()
                                                                           ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK_EX (flowconnection);
                    }
                }
              ACE_CATCHANY
                {
                  if (TAO_debug_level > 0)
                    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_MMDevice::create_a::get_flow_connection");
                }
              ACE_ENDTRY;
              ACE_CHECK_RETURN (0);

              int result = this->fdev_map_.find (flow_key, flow_dev);
              if (result < 0)
                ACE_ERROR_RETURN ((LM_ERROR, "fdev_map::find failed\n"), 0);

              CORBA::String_var named_fdev;
              AVStreams::FlowEndPoint_var flow_endpoint;
              AVStreams::QoS flow_qos;
              result = qos.get_flow_qos (forward_entry.flowname (), flow_qos);
              if (result < 0)
                if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "get_flow_qos failed for %s\n", forward_entry.flowname ()));
              switch (forward_entry.direction ())
                {
                case TAO_FlowSpec_Entry::TAO_AV_DIR_IN:
                  {
                    switch (type)
                      {
                      case MMDEVICE_A:
                        {
                          // In implies flow is from A to B and
                          // hence A is the producer for this flow and B is the consumer for this flow.
                          // We have to create a producer from the FDev for this flow.
                          flow_endpoint =
                            flow_dev->create_producer (flowconnection.in (),
                                                       flow_qos,
                                                       met_qos,
                                                       named_fdev.inout ()
                                                       ACE_ENV_ARG_PARAMETER);
                        }
                        break;
                      case MMDEVICE_B:
                        {
                          flow_endpoint =
                            flow_dev->create_consumer (flowconnection.in (),
                                                       flow_qos,
                                                       met_qos,
                                                       named_fdev.inout ()
                                                       ACE_ENV_ARG_PARAMETER);
                        }
                        break;
                      }
                    ACE_TRY_CHECK;
                  }
                  break;
                case TAO_FlowSpec_Entry::TAO_AV_DIR_OUT:
                  {
                    switch (type)
                      {
                      case MMDEVICE_A:
                        {
                          // OUT implies flow is from B to A and
                          // hence B is the producer for this flow and A is the consumer for this flow.
                          // We have to create a consumer from the FDev for this flow.
                          flow_endpoint =
                            flow_dev->create_consumer (flowconnection.in (),
                                                       flow_qos,
                                                       met_qos,
                                                       named_fdev.inout ()
                                                       ACE_ENV_ARG_PARAMETER);
                        }
                        break;
                      case MMDEVICE_B:
                        {
                          // In implies flow is from A to B and
                          // hence A is the producer for this flow and B is the consumer for this flow.
                          // We have to create a producer from the FDev for this flow.
                          flow_endpoint =
                            flow_dev->create_producer (flowconnection.in (),
                                                       flow_qos,
                                                       met_qos,
                                                       named_fdev.inout ()
                                                       ACE_ENV_ARG_PARAMETER);
                        }
                        break;
                      }
                    ACE_TRY_CHECK;
                  }
                  break;
                default:
                  break;
                }
              CORBA::Any flowname_any;
              flowname_any <<= forward_entry.flowname ();
              flow_endpoint->define_property ("FlowName", flowname_any ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              sep->add_fep (flow_endpoint.in ()
                            ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_MMDevice::create_A");
      return sep;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (sep);
  return sep;
}

AVStreams::StreamEndPoint_A_ptr
TAO_MMDevice::create_A (AVStreams::StreamCtrl_ptr streamctrl,
                        AVStreams::VDev_out the_vdev,
                        AVStreams::streamQoS &stream_qos,
                        CORBA::Boolean_out met_qos,
                        char *&named_vdev,
                        const AVStreams::flowSpec &flow_spec
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::streamOpDenied,
                   AVStreams::notSupported,
                   AVStreams::QoSRequestFailed,
                   AVStreams::noSuchFlow))
{
  CORBA::String_var str_ctrl_ior =
       TAO_ORB_Core_instance ()->orb ()->object_to_string (streamctrl ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  AVStreams::StreamEndPoint_A_ptr sep_a = 0;
  AVStreams::StreamEndPoint_var sep;
  ACE_TRY
    {
      sep = this->create_A_B (MMDEVICE_A, streamctrl, the_vdev, stream_qos, met_qos, named_vdev, flow_spec ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      sep_a = AVStreams::StreamEndPoint_A::_narrow (sep.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_MMDevice::create_A");
      return sep_a;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (sep_a);
  return sep_a;
}


AVStreams::StreamEndPoint_B_ptr
TAO_MMDevice::create_B (AVStreams::StreamCtrl_ptr streamctrl,
                        AVStreams::VDev_out the_vdev,
                        AVStreams::streamQoS &stream_qos,
                        CORBA::Boolean_out met_qos,
                        char *&named_vdev,
                        const AVStreams::flowSpec &flow_spec
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::streamOpDenied,
                   AVStreams::notSupported,
                   AVStreams::QoSRequestFailed,
                   AVStreams::noSuchFlow))
{
  AVStreams::StreamEndPoint_B_ptr sep_b = AVStreams::StreamEndPoint_B::_nil ();
  AVStreams::StreamEndPoint_var sep;

  ACE_TRY
    {
      sep = this->create_A_B (MMDEVICE_B, streamctrl, the_vdev, stream_qos, met_qos, named_vdev, flow_spec ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      sep_b = AVStreams::StreamEndPoint_B::_narrow (sep.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_MMDevice::create_B");
      return sep_b;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (sep_b);
  return sep_b;
}


// destroys the streamendpoint and the Vdev.
void
TAO_MMDevice::destroy (AVStreams::StreamEndPoint_ptr /* the_ep */,
                       const char * /* vdev_name */
                       ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported))
{
  // Remove self from POA.  Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.
  int result = TAO_AV_Core::deactivate_servant (this);
  if (result < 0)
    if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "TAO_MMDevice::destroy failed\n"));
}

char *
TAO_MMDevice::add_fdev_i (AVStreams::FDev_ptr fdev
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported,
                   AVStreams::streamOpFailed))
{
  char* tmp;
  ACE_NEW_RETURN (tmp,
                  char[64],
                  0);
  CORBA::String_var flow_name = tmp;

  ACE_TRY
    {
      // exception implies the flow name is not defined and is system
      // generated.
      ACE_OS::sprintf (tmp, "flow%d", flow_num_++);
      CORBA::Any flowname_any;
      flowname_any <<= flow_name.in ();
      fdev->define_property ("Flow", flowname_any ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_MMDevice::add_fdev");
      return 0;
    }
  ACE_ENDTRY;
  return flow_name._retn ();
}

// Adds the fdev object to the MMDevice.
char *
TAO_MMDevice::add_fdev (CORBA::Object_ptr fdev_obj
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported,
                   AVStreams::streamOpFailed))
{
  CORBA::String_var flow_name;
  AVStreams::FDev_var fdev;
  ACE_TRY_EX (flow_name)
    {
      CORBA::Any_ptr flow_name_any;
      fdev = AVStreams::FDev::_narrow (fdev_obj ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (flow_name);

      if (CORBA::is_nil (fdev.in ()))
          return 0;


      flow_name_any = fdev->get_property_value ("Flow" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (flow_name);

      const char *tmp;
      *flow_name_any >>= tmp;
      flow_name = CORBA::string_dup (tmp);
    }
  ACE_CATCHANY
    {
      flow_name =
        this->add_fdev_i (fdev.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);


  // Add it to the sequence of flowNames supported.
  // put the flowname and the fdev in a hashtable.
  ACE_CString fdev_name_key (CORBA::string_dup (flow_name.in ()));


  if (this->fdev_map_.bind (fdev_name_key, AVStreams::FDev::_duplicate (fdev.in ())) != 0)
    ACE_THROW_RETURN (AVStreams::streamOpFailed (), 0);
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
                             flows_any
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (flows);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_MMDevice::add_fdev");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return flow_name._retn ();
}

// Gets the FDev object associated with this flow.
CORBA::Object_ptr
TAO_MMDevice::get_fdev (const char *flow_name
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported,
                   AVStreams::noSuchFlow))
{
  ACE_CHECK_RETURN (0);

  ACE_CString fdev_name_key (flow_name);
  FDev_Map::ENTRY *fdev_entry = 0;
  if (this->fdev_map_.find (fdev_name_key, fdev_entry) == 0)
    return AVStreams::FDev::_duplicate (fdev_entry->int_id_);
  return 0;
}

// Removes the fdev from this MMDevice.
void
TAO_MMDevice::remove_fdev (const char *flow_name
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported,
                   AVStreams::noSuchFlow,
                   AVStreams::streamOpFailed))
{
  ACE_TRY
    {
      ACE_CString fdev_name_key (flow_name);
      AVStreams::FDev_ptr fdev_entry = AVStreams::FDev::_nil ();
      // Remove the fep from the hash table.
      if (this->fdev_map_.unbind (fdev_name_key, fdev_entry)!= 0)
        ACE_THROW (AVStreams::streamOpFailed ());

      if (!CORBA::is_nil (fdev_entry))
        CORBA::release (fdev_entry);

      AVStreams::flowSpec new_flows (this->flows_.length ());
      for (u_int i=0, j=0 ; i <this->flows_.length (); i++)
        if (ACE_OS::strcmp (flow_name, this->flows_[i]) != 0)
          new_flows[j++] = this->flows_[i];

      CORBA::Any flows;
      flows <<= new_flows;
      this->flows_ = new_flows;
      this->define_property ("Flows",
                             flows
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_MMDevice::remove_fdev");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// destructor.
TAO_MMDevice::~TAO_MMDevice (void)
{
  delete this->stream_ctrl_;
  FDev_Map_Iterator iterator (fdev_map_);
  FDev_Map_Entry *entry = 0;
  for (;iterator.next (entry) != 0; iterator.advance ())
    CORBA::release (entry->int_id_);
}

//------------------------------------------------------------------
// TAO_FlowConnection
//------------------------------------------------------------------

// default constructor.
TAO_FlowConnection::TAO_FlowConnection (void)
  :fp_name_ (CORBA::string_dup ("")),
   ip_multicast_ (0)
{
}

// int
// TAO_FlowConnection::set_mcast_addr (ACE_UINT32 mcast_addr, u_short mcast_port)
// {
//   this->mcast_addr_ = mcast_addr;
//   this->mcast_port_ = mcast_port;
//   return 0;
// }

int
TAO_FlowConnection::set_mcast_addr (ACE_CString mcast_addr, u_short mcast_port)
{
  this->mcast_addr_ = mcast_addr;
  this->mcast_port_ = mcast_port;
  return 0;
}

void
TAO_FlowConnection::set_protocol (const char *protocol)
{
  this->protocol_ = protocol;
}

// stop this flow.
void
TAO_FlowConnection::stop (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      FlowProducer_SetItor producer_begin = this->flow_producer_set_.begin
        ();
      for (FlowProducer_SetItor producer_end =
             this->flow_producer_set_.end ();
           producer_begin != producer_end; ++producer_begin)
        {
          (*producer_begin)->stop (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      FlowConsumer_SetItor consumer_begin = this->flow_consumer_set_.begin
        ();
      for (FlowConsumer_SetItor consumer_end =
             this->flow_consumer_set_.end ();
           consumer_begin != consumer_end; ++consumer_begin)
        {
          (*consumer_begin)->stop (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_FlowConnection::stop");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// start this flow.
void
TAO_FlowConnection::start (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      FlowConsumer_SetItor consumer_begin = this->flow_consumer_set_.begin
        ();
      for (FlowConsumer_SetItor consumer_end =
             this->flow_consumer_set_.end ();
           consumer_begin != consumer_end; ++consumer_begin)
        {
          (*consumer_begin)->start (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      FlowProducer_SetItor producer_begin = this->flow_producer_set_.begin
        ();
      for (FlowProducer_SetItor producer_end =
             this->flow_producer_set_.end ();
           producer_begin != producer_end; ++producer_begin)
        {
          (*producer_begin)->start (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_FlowConnection::start");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// destroy this flow.
void
TAO_FlowConnection::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      FlowProducer_SetItor producer_begin = this->flow_producer_set_.begin
        ();
      for (FlowProducer_SetItor producer_end =
             this->flow_producer_set_.end ();
           producer_begin != producer_end; ++producer_begin)
        {
          (*producer_begin)->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      FlowConsumer_SetItor consumer_begin = this->flow_consumer_set_.begin
        ();
      for (FlowConsumer_SetItor consumer_end =
             this->flow_consumer_set_.end ();
           consumer_begin != consumer_end; ++consumer_begin)
        {
          (*consumer_begin)->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_FlowConnection::destroy");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
  int result = TAO_AV_Core::deactivate_servant (this);
  if (result < 0)
    if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "TAO_FlowConnection::destroy failed\n"));
}

// modify the QoS for this flow.
CORBA::Boolean
TAO_FlowConnection::modify_QoS (AVStreams::QoS & new_qos
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::QoSRequestFailed))
{
  ACE_UNUSED_ARG (new_qos);
  ACE_CHECK_RETURN (0);
  return 0;
}

// use the specified flow protocol for this flow.
CORBA::Boolean
TAO_FlowConnection::use_flow_protocol (const char * fp_name,
                                       const CORBA::Any & fp_settings
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::FPError,
                   AVStreams::notSupported))
{
  this->fp_name_ = fp_name;
  this->fp_settings_ = fp_settings;
  FlowProducer_SetItor producer_begin = this->flow_producer_set_.begin
    ();
  for (FlowProducer_SetItor producer_end =
         this->flow_producer_set_.end ();
       producer_begin != producer_end; ++producer_begin)
    {
      (*producer_begin)->use_flow_protocol
        (fp_name, fp_settings ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  FlowConsumer_SetItor consumer_begin = this->flow_consumer_set_.begin
    ();
  for (FlowConsumer_SetItor consumer_end =
         this->flow_consumer_set_.end ();
       consumer_begin != consumer_end; ++consumer_begin)
    {
      (*consumer_begin)->use_flow_protocol
        (fp_name, fp_settings ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  return 1;
}

void
TAO_FlowConnection::push_event (const AVStreams::streamEvent & the_event
                                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (the_event);
}

CORBA::Boolean
TAO_FlowConnection::connect_devs (AVStreams::FDev_ptr a_party,
                                  AVStreams::FDev_ptr b_party,
                                  AVStreams::QoS & flow_qos
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::streamOpDenied,
                   AVStreams::QoSRequestFailed))
{
  CORBA::Boolean result = 0;
  ACE_TRY
    {
      AVStreams::FlowConnection_var flowconnection = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::Boolean met_qos;
      CORBA::String_var named_fdev ((const char *)"");
      AVStreams::FlowProducer_var producer =
        a_party->create_producer (flowconnection.in (),
                                  flow_qos,
                                  met_qos,
                                  named_fdev.inout ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      AVStreams::FlowConsumer_var consumer =
        b_party->create_consumer (flowconnection.in (),
                                  flow_qos,
                                  met_qos,
                                  named_fdev.inout ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      result = this->connect (producer.in (),
                              consumer.in (),
                              flow_qos
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_FlowConnection::connect_devs");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return result;
}

// connect the producer and the consumer
CORBA::Boolean
TAO_FlowConnection::connect (AVStreams::FlowProducer_ptr producer,
                             AVStreams::FlowConsumer_ptr consumer,
                             AVStreams::QoS & the_qos
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::formatMismatch,
                   AVStreams::FEPMismatch,
                   AVStreams::alreadyConnected))
{
  ACE_TRY
    {

      AVStreams::FlowProducer_ptr flow_producer =
        AVStreams::FlowProducer::_duplicate (producer);
      AVStreams::FlowConsumer_ptr flow_consumer =
        AVStreams::FlowConsumer::_duplicate (consumer);

      this->flow_producer_set_.insert (flow_producer);
      this->flow_consumer_set_.insert (flow_consumer);
      AVStreams::FlowConnection_var flowconnection =
        this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      flow_producer->set_peer (flowconnection.in (),
                               flow_consumer,
                               the_qos
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      flow_consumer->set_peer (flowconnection.in (),
                               flow_producer,
                               the_qos
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      char *consumer_address =
        flow_consumer->go_to_listen (the_qos,
                                     0, // false for is_mcast
                                     flow_producer,
                                     this->fp_name_.inout ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (consumer_address, "") == 0)
        {
          // Consumer is not willing to listen, so try the producer.
          consumer_address = flow_producer->go_to_listen (the_qos,
                                                          0, // false for is_mcast
                                                          flow_consumer,
                                                          this->fp_name_.inout ()
                                                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          flow_consumer->connect_to_peer (the_qos,
                                          consumer_address,
                                          this->fp_name_.inout ()
                                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          // @@ Naga: We have to find means to set the reverse channel for the producer.
          // Its broken in the point-to_point case for UDP.
        }
      else
        {
          if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "TAO_FlowConnection::connect_to_peer addres: %s", consumer_address));
          flow_producer->connect_to_peer (the_qos,
                                          consumer_address,
                                          this->fp_name_.inout ()
                                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_FlowConnection::connect");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}


CORBA::Boolean
TAO_FlowConnection::disconnect (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_CHECK_RETURN (0);
  return  0;
}

CORBA::Boolean
TAO_FlowConnection::add_producer (AVStreams::FlowProducer_ptr producer,
                                  AVStreams::QoS & the_qos
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::alreadyConnected,
                   AVStreams::notSupported))
{
  ACE_TRY
    {
          AVStreams::FlowProducer_ptr flow_producer =
            AVStreams::FlowProducer::_duplicate (producer);
          // @@Naga:Sometimes the same producer could be added with a different object reference.
          // There's no portable way of comparing obj refs. but we have to do this till we find
          // a permanent solution.For eg. 2 different flowproducers for the same flow in a
          // Multipoint to Multipoint binding will have the same flowname and hence cannot be
          // used for resolving ties.
          FlowProducer_SetItor begin = this->flow_producer_set_.begin ();
          FlowProducer_SetItor end = this->flow_producer_set_.end ();
          for (; begin != end; ++begin)
            {
              if ((*begin)->_is_equivalent (producer
                                            ACE_ENV_ARG_PARAMETER))
              // producer exists in the set, a duplicate.
              ACE_ERROR_RETURN ((LM_WARNING, "TAO_FlowConnection::add_producer: producer already exists\n"), 1);
            }
          // We need to check the return value of the insert into the flow producer
          // set, since multiconnect could be called many times which will lead to
          // a call to add_producer every time a sink is added. If the producer is already
          // present in our list we just return immediately.
          int result = this->flow_producer_set_.insert (flow_producer);
          if (result == 1)
            {
              // producer exists in the set, a duplicate.
              ACE_ERROR_RETURN ((LM_WARNING, "TAO_FlowConnection::add_producer: producer already exists\n"), 1);
            }
          CORBA::Boolean met_qos;
          char mcast_address[BUFSIZ];
          if (this->producer_address_.in () == 0)
            {
              ACE_INET_Addr mcast_addr;
              mcast_addr.set (this->mcast_port_,
                              this->mcast_addr_.c_str ()
                              );

              char buf [BUFSIZ];
              mcast_addr.addr_to_string (buf, BUFSIZ);
              ACE_OS::sprintf (mcast_address, "%s=%s", this->protocol_.in (), buf);
            }
          else
            {
              ACE_OS::strcpy (mcast_address, this->producer_address_.in ());
            }
          char *address = flow_producer->connect_mcast (the_qos,
                                                        met_qos,
                                                        mcast_address,
                                                        this->fp_name_.in ()
                                                        ACE_ENV_ARG_PARAMETER);

          ACE_TRY_CHECK;
          if (this->producer_address_.in () == 0)
            {
              TAO_Forward_FlowSpec_Entry entry ("", "", "", "", address);
              if (entry.address () != 0)
                {
                  // Internet multicasting is in use.
                  this->producer_address_ = address;
                }
              else
                {
                  // ATM Multicasting is in use.
                  this->ip_multicast_ = 0;
                }
            }
          // set the multicast peer.
          if (CORBA::is_nil (this->mcastconfigif_.in ()))
            {
              ACE_NEW_RETURN (this->mcastconfigif_i_,
                              TAO_MCastConfigIf,
                              0);
              this->mcastconfigif_ = this->mcastconfigif_i_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          AVStreams::FlowConnection_var flowconnection = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          flow_producer->set_Mcast_peer (flowconnection.in (),
                                         this->mcastconfigif_.in (),
                                         the_qos
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_FlowConnection::add_producer");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

CORBA::Boolean
TAO_FlowConnection::add_consumer (AVStreams::FlowConsumer_ptr consumer,
                                  AVStreams::QoS & the_qos
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::alreadyConnected))
{
  ACE_TRY
    {
      AVStreams::FlowConsumer_ptr flow_consumer =
        AVStreams::FlowConsumer::_duplicate (consumer);
      FlowConsumer_SetItor begin = this->flow_consumer_set_.begin ();
      FlowConsumer_SetItor end = this->flow_consumer_set_.end ();
      for (; begin != end; ++begin)
        {
          if ((*begin)->_is_equivalent (consumer
                                        ACE_ENV_ARG_PARAMETER))
            // Consumer exists in the set, a duplicate.
            ACE_ERROR_RETURN ((LM_WARNING, "TAO_FlowConnection::add_Consumer: Consumer already exists\n"), 1);
        }
      int result = this->flow_consumer_set_.insert (flow_consumer);
      if (result == 1)
        {
          // consumer exists in the set, a duplicate.
          ACE_ERROR_RETURN ((LM_WARNING, "TAO_FlowConnection::add_consumer: consumer already exists\n"), 1);
        }

      FlowProducer_SetItor producer_begin = this->flow_producer_set_.begin ();
      // @@Lets take that the first entry as the only producer. We're
      // not sure if we can have multiple flow producers in a
      // flowconnection. We can have multiple producer in the MtM binding,
      // in which case the first producer that gets added is the leader.
      AVStreams::FlowProducer_ptr flow_producer = (*producer_begin);

      AVStreams::protocolSpec protocols (1);
      protocols.length (1);
      protocols [0] = CORBA::string_dup (this->producer_address_.in ());

      if (!this->ip_multicast_)
        {
          flow_consumer->set_protocol_restriction (protocols
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          char * address =
            flow_consumer->go_to_listen (the_qos,
                                         1,
                                         flow_producer,
                                         this->fp_name_.inout ()
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          CORBA::Boolean is_met;
          flow_producer->connect_mcast (the_qos,
                                        is_met,
                                        address,
                                        this->fp_name_.inout ()
                                        ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
       else
        {
          // The spec says go_to_listen is called with the multicast
          // address returned from the connect_mcast call called
          // during add_producer. But go_to_listen doesn't have a
          // address parameter. I guess it should be connect_to_peer.
          // IP Multicasting.
          flow_consumer->connect_to_peer (the_qos,
                                          this->producer_address_.in (),
                                          this->fp_name_.inout ()
                                          ACE_ENV_ARG_PARAMETER);

          //  char * address =
          //             flow_consumer->go_to_listen (the_qos,
          //                                          1,
          //                                          flow_producer,
          //                                          this->fp_name_.inout ()
          //                                          ACE_ENV_ARG_PARAMETER);

          //            ACE_TRY_CHECK;
        }
      if (CORBA::is_nil (this->mcastconfigif_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "TAO_FlowConnection::add_consumer: first add a producer and then a consumer\n"), 0);
      // @@ Is this the right place to do set_peer?
      AVStreams::flowSpec flow_spec;
      AVStreams::streamQoS stream_qos (1);
      stream_qos.length (1);
      stream_qos [0] = the_qos;
      this->mcastconfigif_->set_peer (flow_consumer,
                                      stream_qos,
                                      flow_spec
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_FlowConnection::add_consumer");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

CORBA::Boolean
TAO_FlowConnection::drop (AVStreams::FlowEndPoint_ptr target
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notConnected))
{
  ACE_UNUSED_ARG (target);
  ACE_CHECK_RETURN (0);
  return 0;
}

// -----------------------------------------------------------------
// TAO_FlowEndPoint
// -----------------------------------------------------------------

//default constructor.
TAO_FlowEndPoint::TAO_FlowEndPoint (void)
  :lock_ (0)
{
}

TAO_FlowEndPoint::TAO_FlowEndPoint (const char *flowname,
                                    AVStreams::protocolSpec &protocols,
                                    const char *format)
{
  this->open (flowname, protocols, format);
}

void
TAO_FlowEndPoint::set_flow_handler (const char * /*flowname*/,
                                    TAO_AV_Flow_Handler * /*handler*/)
{
}

int
TAO_FlowEndPoint::open (const char *flowname,
                        AVStreams::protocolSpec &protocols,
                        const char *format)
{
  this->flowname_ = flowname;
  this->format_ = format;

  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "TAO_FlowEndPoint::open\n"));
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::Any flowname_any;
      flowname_any <<= flowname;
      this->define_property ("Flow",
                             flowname_any
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->set_format (format
                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->protocol_addresses_ = protocols;
      AVStreams::protocolSpec protocol_spec (protocols.length ());
      protocol_spec.length (protocols.length ());
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "%N:%l\n"));
      for (u_int i=0;i<protocols.length ();i++)
        {
          CORBA::String_var address = CORBA::string_dup (protocols [i].in ());
          TAO_Forward_FlowSpec_Entry entry ("", "", "", "", address.in ());
          protocol_spec [i] = CORBA::string_dup (entry.carrier_protocol_str ());
          if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "%s\n", protocol_spec[i].in ()));
        }
      this->set_protocol_restriction (protocol_spec
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_FlowEndPoint::open");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}


int
TAO_FlowEndPoint::set_flowname (const char *flowname)
{
  this->flowname_ = flowname;
  return 0;
}

// used by one flowconnection so that multiple connections cant use
// the same flowendpoint.
CORBA::Boolean
TAO_FlowEndPoint::lock (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // lock the current flowendpoint
  ACE_CHECK_RETURN (0);

  if (this->lock_)
    return 0;
  this->lock_ = 1;
  return 1;
}

// unlocks the flowendpoint , becomes free to be used in another flow.
void
TAO_FlowEndPoint::unlock (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_CHECK;

  this->lock_ = 0;
}


void
TAO_FlowEndPoint::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  int result = TAO_AV_Core::deactivate_servant (this);
  if (result < 0)
    if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "TAO_StreamEndPoint::destroy failed\n"));
  TAO_AV_FlowSpecSetItor end = this->flow_spec_set_.end ();
  for (TAO_AV_FlowSpecSetItor begin = this->flow_spec_set_.begin ();
       begin != end; ++begin)
    (*begin)->protocol_object ()->destroy ();
}

AVStreams::StreamEndPoint_ptr
TAO_FlowEndPoint::related_sep (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  return AVStreams::StreamEndPoint::_duplicate (this->related_sep_.in ());
}

void
TAO_FlowEndPoint::related_sep (AVStreams::StreamEndPoint_ptr related_sep
                               ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->related_sep_ = AVStreams::StreamEndPoint::_duplicate (related_sep);
}

AVStreams::FlowConnection_ptr
TAO_FlowEndPoint::related_flow_connection (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return AVStreams::FlowConnection::_duplicate (this->related_flow_connection_.in ());
}

void
TAO_FlowEndPoint::related_flow_connection (AVStreams::FlowConnection_ptr related_flow_connection
                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_CHECK;

  this->related_flow_connection_ = AVStreams::FlowConnection::_duplicate (related_flow_connection);
}

// returns the connected peer for this flow
AVStreams::FlowEndPoint_ptr
TAO_FlowEndPoint::get_connected_fep (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notConnected,
                   AVStreams::notSupported))
{
  return AVStreams::FlowEndPoint::_duplicate (this->peer_fep_.in ());
}

CORBA::Boolean
TAO_FlowEndPoint::use_flow_protocol (const char * fp_name,
                                     const CORBA::Any &
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::FPError,
                   AVStreams::notSupported))
{
  ACE_TRY
    {
      // Define the property called FlowProtocol
      CORBA::Any flowname_property;
      flowname_property <<= fp_name;
      this->define_property ("FlowProtocol",
                             flowname_property
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_FlowEndPoint::use_flow_protocol");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

void
TAO_FlowEndPoint::set_format (const char * format
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported))
{
  this->format_ = format;
  ACE_TRY
    {
      // make this a property so that is_fep_compatible can query this and
      // check if 2 flowendpoints are compatible.
      CORBA::Any format_val;
      format_val <<= format;
      this->define_property ("Format",
                             format_val
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_FlowEndpoint::set_format");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
TAO_FlowEndPoint::set_dev_params (const CosPropertyService::Properties & new_settings
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::PropertyException,
                   AVStreams::streamOpFailed))
{
  this->dev_params_ = new_settings;
  ACE_TRY
    {
      CORBA::Any DevParams_property;
      DevParams_property <<= new_settings;
      this->define_property ("DevParams",
                             DevParams_property
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_FlowEndPoint::set_dev_params");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
TAO_FlowEndPoint::set_protocol_restriction (const AVStreams::protocolSpec & protocols
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported))
{
  ACE_TRY
    {
      u_int i = 0;
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "%N:%l\n"));
      for (i=0;i<protocols.length ();i++)
        {
          const char *protocol = (protocols)[i].in ();
          if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "%s\n", protocol));
        }
      CORBA::Any AvailableProtocols_property;
      AvailableProtocols_property <<= protocols;
      this->define_property ("AvailableProtocols",
                             AvailableProtocols_property
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      AVStreams::protocolSpec *temp_spec;
      CORBA::Any_var temp_any = this->get_property_value ("AvailableProtocols"
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      temp_any.in () >>= temp_spec;
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "%N:%l\n"));
      for (i=0;i<temp_spec->length ();i++)
        {
          const char *protocol = (*temp_spec)[i].in ();
          if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG, "%s\n", protocol));
        }
      this->protocols_ = protocols;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_FlowEndpoint::set_protocol_restriction");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

CORBA::Boolean
TAO_FlowEndPoint::is_fep_compatible (AVStreams::FlowEndPoint_ptr peer_fep
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::formatMismatch,
                   AVStreams::deviceQosMismatch))
{
  const char *exception_message = "";
  ACE_TRY
    {
      // check whether the passed flowendpoint is compatible with this flowendpoint.
      // should we check for the availableFormats and choose one format.
      // get my format value
      CORBA::Any_var format_ptr;
      CORBA::String_var my_format, peer_format;

      exception_message = "TAO_FlowEndPoint::is_fep_compatible - Format";
      format_ptr = this->get_property_value ("Format"
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      const char *temp_format;
      format_ptr.in () >>= temp_format;
      my_format = CORBA::string_dup (temp_format);
      // get my peer's format value
      exception_message = "TAO_FlowEndPoint::is_fep_compatible - Format[2]";
      format_ptr = peer_fep->get_property_value ("Format"
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      format_ptr.in () >>= temp_format;
      peer_format = CORBA::string_dup (temp_format);
      if (ACE_OS::strcmp (my_format.in (),
                          peer_format.in ()) != 0)
        return 0;

      // since formats are same, check for a common protocol
      CORBA::Any_var AvailableProtocols_ptr;
      AVStreams::protocolSpec my_protocol_spec, peer_protocol_spec;
      AVStreams::protocolSpec *temp_protocols;;

      exception_message =
        "TAO_FlowEndPoint::is_fep_compatible - AvailableProtocols";
      AvailableProtocols_ptr = this->get_property_value ("AvailableProtocols"
                                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      AvailableProtocols_ptr.in () >>= temp_protocols;
      my_protocol_spec = *temp_protocols;

      exception_message =
        "TAO_FlowEndPoint::is_fep_compatible - AvailableProtocols[2]";
      AvailableProtocols_ptr = peer_fep->get_property_value ("AvailableProtocols"
                                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      AvailableProtocols_ptr.in () >>= temp_protocols;
      peer_protocol_spec = *temp_protocols;

      int protocol_match = 0;
      for (u_int i=0;i<my_protocol_spec.length ();i++)
        {
          CORBA::String_var my_protocol_string;
          for (u_int j=0;j<peer_protocol_spec.length ();j++)
            {
              CORBA::String_var peer_protocol_string;
              my_protocol_string = CORBA::string_dup (my_protocol_spec[i]);
              peer_protocol_string = CORBA::string_dup (peer_protocol_spec[j]);
              if (ACE_OS::strcmp (my_protocol_string.in (), peer_protocol_string.in ()) == 0)
                {
                  protocol_match = 1;
                  break;
                }
            }
          if (protocol_match)
            break;
        }
      if (!protocol_match)
        return 0;
    }
  ACE_CATCH (CosPropertyService::PropertyNotFound, nf)
    {
      ACE_PRINT_EXCEPTION (nf,
                           exception_message);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO_FlowEndPoint::is_fep_compatible");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

CORBA::Boolean
TAO_FlowEndPoint::set_peer (AVStreams::FlowConnection_ptr /* the_fc */,
                            AVStreams::FlowEndPoint_ptr the_peer_fep,
                            AVStreams::QoS & /* the_qos */
                            ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::QoSRequestFailed,
                   AVStreams::streamOpFailed))
{
  this->peer_fep_ =
    AVStreams::FlowEndPoint::_duplicate (the_peer_fep);
  return 1;
}

CORBA::Boolean
TAO_FlowEndPoint::set_Mcast_peer (AVStreams::FlowConnection_ptr /* the_fc */,
                                  AVStreams::MCastConfigIf_ptr mcast_peer,
                                  AVStreams::QoS & /* the_qos */
                                  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::QoSRequestFailed))
{
  this->mcast_peer_ = AVStreams::MCastConfigIf::_duplicate (mcast_peer);
  return 0;
}

char *
TAO_FlowEndPoint::go_to_listen_i (TAO_FlowSpec_Entry::Role role,
                                  AVStreams::QoS & /*the_qos*/,
                                  CORBA::Boolean /*is_mcast*/,
                                  AVStreams::FlowEndPoint_ptr peer_fep,
                                  char *& flowProtocol
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::failedToListen,
                   AVStreams::FPError,
                   AVStreams::QoSRequestFailed))
{
  char direction [BUFSIZ];
  switch (role)
    {
    case TAO_FlowSpec_Entry::TAO_AV_PRODUCER:
      ACE_OS::strcpy (direction, "IN");
      break;
    case TAO_FlowSpec_Entry::TAO_AV_CONSUMER:
      ACE_OS::strcpy (direction, "OUT");
      break;
    default:
      break;
    }
  AVStreams::protocolSpec my_protocol_spec, peer_protocol_spec;
  AVStreams::protocolSpec *temp_protocols;
  CORBA::Any_var AvailableProtocols_ptr =
    peer_fep->get_property_value ("AvailableProtocols"
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  AvailableProtocols_ptr.in () >>= temp_protocols;
  peer_protocol_spec = *temp_protocols;
  AvailableProtocols_ptr =
    this->get_property_value ("AvailableProtocols"
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  AvailableProtocols_ptr.in () >>= temp_protocols;
  my_protocol_spec = *temp_protocols;
  int protocol_match = 0;
  CORBA::String_var listen_protocol;
  u_int i =0;
  for (i=0;i<my_protocol_spec.length ();i++)
    {
      CORBA::String_var my_protocol_string;
      for (u_int j=0;j<peer_protocol_spec.length ();j++)
        {
          CORBA::String_var peer_protocol_string;
          my_protocol_string = CORBA::string_dup (my_protocol_spec[i]);
          peer_protocol_string = CORBA::string_dup (peer_protocol_spec[j]);
          if (ACE_OS::strcmp (my_protocol_string.in (), peer_protocol_string.in ()) == 0)
            {
              listen_protocol = my_protocol_string;
              protocol_match = 1;
              break;
            }
        }
      if (protocol_match)
        break;
    }
  if (!protocol_match)
    ACE_ERROR_RETURN ((LM_ERROR, "TAO_FlowEndPoint::go_to_listen failed: no protoocol match\n"), 0);

  for (u_int j=0;j<this->protocol_addresses_.length ();j++)
    if (ACE_OS::strncmp (this->protocol_addresses_ [j], listen_protocol.in (), ACE_OS::strlen (listen_protocol.in ())) == 0)
      {
        // Now listen on that protocol.
        TAO_Forward_FlowSpec_Entry *entry;
        ACE_NEW_RETURN (entry,
                        TAO_Forward_FlowSpec_Entry (this->flowname_.in (),
                                                    direction,
                                                    this->format_.in (),
                                                    flowProtocol,
                                                    this->protocol_addresses_ [j]),
                        0);

        TAO_AV_Acceptor_Registry *acceptor_registry = TAO_AV_CORE::instance ()->acceptor_registry ();
        this->flow_spec_set_.insert (entry);
        int result = acceptor_registry->open (this,
                                              TAO_AV_CORE::instance (),
                                              this->flow_spec_set_);
        if (result < 0)
          return 0;
        char *listen_address = entry->get_local_addr_str ();
        char *address;
        ACE_NEW_RETURN (address,
                        char [BUFSIZ],
                        0);
        ACE_OS::sprintf (address, "%s=%s", listen_protocol.in (), listen_address);
        return address;
      }
  return 0;
}


CORBA::Boolean
TAO_FlowEndPoint::connect_to_peer_i (TAO_FlowSpec_Entry::Role role,
                                     AVStreams::QoS & /*the_qos*/,
                                     const char * address,
                                     const char * use_flow_protocol
                                     ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::failedToConnect,
                   AVStreams::FPError,
                   AVStreams::QoSRequestFailed))
{
  char direction [BUFSIZ];
  switch (role)
    {
    case TAO_FlowSpec_Entry::TAO_AV_PRODUCER:
      ACE_OS::strcpy (direction, "IN");
      break;
    case TAO_FlowSpec_Entry::TAO_AV_CONSUMER:
      ACE_OS::strcpy (direction, "OUT");
      break;
    default:
      break;
    }
  TAO_Forward_FlowSpec_Entry *entry;
  ACE_NEW_RETURN (entry,
                  TAO_Forward_FlowSpec_Entry (this->flowname_.in (),
                                              direction,
                                              this->format_.in (),
                                              use_flow_protocol,
                                              address),
                  0);
  this->flow_spec_set_.insert (entry);
  TAO_AV_Connector_Registry *connector_registry = TAO_AV_CORE::instance ()->connector_registry ();
  int result = connector_registry->open (this,
                                         TAO_AV_CORE::instance (),
                                         this->flow_spec_set_);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR, "TAO_FlowEndPoint::connector_registry::open failed\n"), 0);
  this->reverse_channel_ = entry->get_local_addr_str ();
  return 1;
}

int
TAO_FlowEndPoint::set_protocol_object (const char * /*flowname*/,
                                       TAO_AV_Protocol_Object * /*object*/)
{
  return 0;
}


// ------------------------------------------------------------
// TAO_FlowProducer class
// ------------------------------------------------------------

//default constructor
TAO_FlowProducer::TAO_FlowProducer (void)
{
}

TAO_FlowProducer::TAO_FlowProducer (const char *flowname,
                                    AVStreams::protocolSpec protocols,
                                    const char *format)
{
  this->open (flowname, protocols, format);
}

// gets the reverse channel for feedback.
char *
TAO_FlowProducer::get_rev_channel (const char * /*pcol_name*/
                                   ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

// The start, stop and destroy are to be handled by the application.
void
TAO_FlowProducer::stop (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_AV_FlowSpecSetItor end = this->flow_spec_set_.end ();
  for (TAO_AV_FlowSpecSetItor begin = this->flow_spec_set_.begin ();
       begin != end; ++begin)
    {
      TAO_FlowSpec_Entry *entry = (*begin);
      entry->handler ()->stop (TAO_FlowSpec_Entry::TAO_AV_PRODUCER);
    }
}

void
TAO_FlowProducer::start (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_AV_FlowSpecSetItor end = this->flow_spec_set_.end ();
  for (TAO_AV_FlowSpecSetItor begin = this->flow_spec_set_.begin ();
       begin != end; ++begin)
    {
      TAO_FlowSpec_Entry *entry = (*begin);
      if (entry->handler () != 0)
        {
          entry->handler ()->start (TAO_FlowSpec_Entry::TAO_AV_PRODUCER);
        }
      if (entry->control_handler () != 0)
        {
          entry->control_handler ()->start (TAO_FlowSpec_Entry::TAO_AV_PRODUCER);
        }
    }
}

char *
TAO_FlowProducer::go_to_listen (AVStreams::QoS & the_qos,
                                CORBA::Boolean is_mcast,
                                AVStreams::FlowEndPoint_ptr peer_fep,
                                char *& flowProtocol
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::failedToListen,
                   AVStreams::FPError,
                   AVStreams::QoSRequestFailed))
{
  return this->go_to_listen_i (TAO_FlowSpec_Entry::TAO_AV_PRODUCER,
                               the_qos,
                               is_mcast,
                               peer_fep,
                               flowProtocol
                               ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_FlowProducer::connect_to_peer (AVStreams::QoS & the_qos,
                                   const char * address,
                                   const char * use_flow_protocol
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::failedToConnect,
                   AVStreams::FPError,
                   AVStreams::QoSRequestFailed))
{
  return this->connect_to_peer_i (TAO_FlowSpec_Entry::TAO_AV_PRODUCER,
                                  the_qos,
                                  address,
                                  use_flow_protocol
                                  ACE_ENV_ARG_PARAMETER);
}
//  Connect to a IP multicast address.
char *
TAO_FlowProducer::connect_mcast (AVStreams::QoS & /* the_qos */,
                                 CORBA::Boolean_out /* is_met */,
                                 const char *address,
                                 const char * use_flow_protocol
                                 ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::failedToConnect,
                   AVStreams::notSupported,
                   AVStreams::FPError,
                   AVStreams::QoSRequestFailed))
{
  // The address variable gives the multicast address to subscribe to.
  for (u_int i=0;i<this->protocols_.length ();i++)
    {
      // choose the protocol which supports multicast.
    }

  if (address == 0)
    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "TAO_FlowProducer::connect_mcast address is 0\n"));
  TAO_Forward_FlowSpec_Entry  *entry;
  ACE_NEW_RETURN (entry,
                  TAO_Forward_FlowSpec_Entry(this->flowname_.in (),
                                             "IN",
                                             this->format_.in (),
                                             use_flow_protocol,
                                             address),
                  0);

  this->flow_spec_set_.insert (entry);
  TAO_AV_Acceptor_Registry *acceptor_registry =
    TAO_AV_CORE::instance ()->acceptor_registry ();
  int result = acceptor_registry->open (this,
                                        TAO_AV_CORE::instance (),
                                        this->flow_spec_set_);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR, "TAO_FlowProducer::connect_mcast:acceptor_registry open failed\n"), 0);
  // Now remove our handler from the reactor since we're a producer and dont want to get called for
  // multicast packets.
  ACE_Event_Handler *event_handler = entry->handler ()->event_handler ();
  event_handler->reactor ()->remove_handler (event_handler,
                                             ACE_Event_Handler::READ_MASK);
  return CORBA::string_dup (address);
}

// sets the key for this flow.
void
TAO_FlowProducer::set_key (const AVStreams::key & the_key
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      CORBA::Any anyval;
      anyval <<= the_key;
      this->define_property ("PublicKey",
                             anyval
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_FlowProducer::set_key");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// source id to be used to distinguish this source from others.
void
TAO_FlowProducer::set_source_id (CORBA::Long source_id
                                 ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->source_id_ = source_id;
}

// ------------------------------------------------------------
// TAO_FlowConsumer
// ------------------------------------------------------------


// default constructor.
TAO_FlowConsumer::TAO_FlowConsumer (void)
{
}

TAO_FlowConsumer::TAO_FlowConsumer (const char *flowname,
                                    AVStreams::protocolSpec protocols,
                                    const char *format)
{
  this->open (flowname, protocols, format);
}

// The start, stop and destroy are to be handled by the application.
void
TAO_FlowConsumer::stop (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_AV_FlowSpecSetItor end = this->flow_spec_set_.end ();
  for (TAO_AV_FlowSpecSetItor begin = this->flow_spec_set_.begin ();
       begin != end; ++begin)
    (*begin)->handler ()->stop (TAO_FlowSpec_Entry::TAO_AV_CONSUMER);
}

void
TAO_FlowConsumer::start (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_AV_FlowSpecSetItor end = this->flow_spec_set_.end ();
  for (TAO_AV_FlowSpecSetItor begin = this->flow_spec_set_.begin ();
       begin != end; ++begin)
    {
      (*begin)->handler ()->start (TAO_FlowSpec_Entry::TAO_AV_CONSUMER);
    }
}

char *
TAO_FlowConsumer::go_to_listen (AVStreams::QoS & the_qos,
                                CORBA::Boolean is_mcast,
                                AVStreams::FlowEndPoint_ptr peer_fep,
                                char *& flowProtocol
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::failedToListen,
                   AVStreams::FPError,
                   AVStreams::QoSRequestFailed))
{
  return this->go_to_listen_i (TAO_FlowSpec_Entry::TAO_AV_CONSUMER,
                               the_qos,
                               is_mcast,
                               peer_fep,
                               flowProtocol
                               ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_FlowConsumer::connect_to_peer (AVStreams::QoS & the_qos,
                                   const char * address,
                                   const char * use_flow_protocol
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::failedToConnect,
                   AVStreams::FPError,
                   AVStreams::QoSRequestFailed))
{
  return this->connect_to_peer_i (TAO_FlowSpec_Entry::TAO_AV_CONSUMER,
                                  the_qos,
                                  address,
                                  use_flow_protocol
                                  ACE_ENV_ARG_PARAMETER);
}

//------------------------------------------------------------
// TAO_Tokenizer
//------------------------------------------------------------
TAO_Tokenizer::TAO_Tokenizer (const char *string, char delimiter)
  :token_array_ (10),
   count_ (0)
{
  this->parse (string, delimiter);
}

TAO_Tokenizer::~TAO_Tokenizer ()
{
  for (unsigned int i=0; i<this->num_tokens_; i++)
    CORBA::string_free (this->token_array_[i]);
}


int
TAO_Tokenizer::parse (const char *string, char delimiter)
{
  ACE_CString new_string (string);
  u_int pos =0;
  int slash_pos = 0;
  u_int count = 0;
  int result;
  while (pos < new_string.length ())
    {
      slash_pos = new_string.find (delimiter, pos);
      ACE_CString substring;
      if (slash_pos != new_string.npos)
        {
          substring = new_string.substring (pos,
                                            slash_pos - pos);
          pos = slash_pos + 1;
        }
      else
        {
          substring = new_string.substring (pos);
          pos = new_string.length ();
        }
      char *token = CORBA::string_dup (substring.c_str ());
      result = this->token_array_.set (token, count);
      if (result == -1)
        {
          this->token_array_.size (this->token_array_.size ()*2);
          result = this->token_array_.set (token, count);
          if (result == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "TAO_Tokenizer::parse error"), -1);
        }
      count++;
    }

  /*
  ACE_OS::strcpy (this->string_ , string);
  char delimiter_str [2] = {0, 0};
  delimiter_str [0] = delimiter;
  char *token = ACE_OS::strtok (this->string_, delimiter_str);

  while (token != 0)
    {
      result = this->token_array_.set (token, count);
      if (result == -1)
        {
          this->token_array_.size (this->token_array_.size ()*2);
          result = this->token_array_.set (token, count);
          if (result == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "TAO_Tokenizer::parse error"), -1);
        }
      token = ACE_OS::strtok (0, delimiter_str);
      count++;
    }
  */
  this->num_tokens_ = count;
  return 0;
}

char*
TAO_Tokenizer::token (void)
{
  if (count_ < num_tokens_)
    return CORBA::string_dup (this->token_array_[this->count_++]);
  else
    return 0;
}

int
TAO_Tokenizer::num_tokens (void)
{
  return this->num_tokens_;
}

const char *
TAO_Tokenizer::operator [] (size_t index) const
{
  if (index >= this->num_tokens_)
    return 0;

  return this->token_array_[index];
}



#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)


template class ACE_Hash_Map_Entry<ACE_CString, AVStreams::FDev_ptr>;
template class ACE_Hash_Map_Entry<ACE_CString, AVStreams::FlowConnection_ptr>;
template class ACE_Hash_Map_Entry<ACE_CString, AVStreams::FlowEndPoint_ptr>;
template class ACE_Hash_Map_Entry<ACE_CString, TAO_FlowSpec_Entry *>;
template class ACE_Hash_Map_Entry<ACE_CString, TAO_AV_Flow_Handler *>;

template class ACE_Hash_Map_Manager<ACE_CString, AVStreams::FDev_ptr, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<ACE_CString, TAO_FlowSpec_Entry *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<ACE_CString, AVStreams::FlowConnection_ptr, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<ACE_CString, TAO_AV_Flow_Handler*, ACE_Null_Mutex>;

template class ACE_Hash_Map_Iterator<ACE_CString, AVStreams::FDev_ptr, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, AVStreams::FDev_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, AVStreams::FDev_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, AVStreams::FDev_ptr, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, AVStreams::FDev_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString, TAO_AV_Flow_Handler*, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_AV_Flow_Handler*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_AV_Flow_Handler*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, TAO_AV_Flow_Handler*, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_AV_Flow_Handler*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Manager_Ex<ACE_CString, AVStreams::FDev_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, AVStreams::FlowConnection_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_FlowSpec_Entry *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_AV_Flow_Handler *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Iterator<ACE_CString, AVStreams::FlowConnection_ptr, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, AVStreams::FlowConnection_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, AVStreams::FlowConnection_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, AVStreams::FlowConnection_ptr, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, AVStreams::FlowConnection_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Manager<ACE_CString, AVStreams::FlowEndPoint_ptr, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, AVStreams::FlowEndPoint_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString, AVStreams::FlowEndPoint_ptr, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, AVStreams::FlowEndPoint_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, AVStreams::FlowEndPoint_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString,AVStreams::FlowEndPoint_ptr,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, AVStreams::FlowEndPoint_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<MMDevice_Map_Hash_Key,TAO_StreamCtrl::MMDevice_Map_Entry>;
template class ACE_Hash_Map_Manager<MMDevice_Map_Hash_Key,TAO_StreamCtrl::MMDevice_Map_Entry,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<MMDevice_Map_Hash_Key, TAO_StreamCtrl::MMDevice_Map_Entry, ACE_Hash<MMDevice_Map_Hash_Key>, ACE_Equal_To<MMDevice_Map_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<MMDevice_Map_Hash_Key,TAO_StreamCtrl::MMDevice_Map_Entry,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<MMDevice_Map_Hash_Key, TAO_StreamCtrl::MMDevice_Map_Entry, ACE_Hash<MMDevice_Map_Hash_Key>, ACE_Equal_To<MMDevice_Map_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<MMDevice_Map_Hash_Key, TAO_StreamCtrl::MMDevice_Map_Entry, ACE_Hash<MMDevice_Map_Hash_Key>, ACE_Equal_To<MMDevice_Map_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<MMDevice_Map_Hash_Key,TAO_StreamCtrl::MMDevice_Map_Entry,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<MMDevice_Map_Hash_Key, TAO_StreamCtrl::MMDevice_Map_Entry, ACE_Hash<MMDevice_Map_Hash_Key>, ACE_Equal_To<MMDevice_Map_Hash_Key>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<ACE_CString,TAO_AV_Acceptor*>;
template class ACE_Hash_Map_Manager<ACE_CString,TAO_AV_Acceptor*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_AV_Acceptor*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString,TAO_AV_Acceptor*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_AV_Acceptor*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_AV_Acceptor*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString,TAO_AV_Acceptor*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_AV_Acceptor*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<ACE_CString,TAO_Forward_FlowSpec_Entry*>;
template class ACE_Hash_Map_Manager<ACE_CString,TAO_Forward_FlowSpec_Entry*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_Forward_FlowSpec_Entry*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString,TAO_Forward_FlowSpec_Entry*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_Forward_FlowSpec_Entry*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_Forward_FlowSpec_Entry*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString,TAO_Forward_FlowSpec_Entry*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_Forward_FlowSpec_Entry*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<ACE_CString,TAO_Reverse_FlowSpec_Entry*>;
template class ACE_Hash_Map_Manager<ACE_CString,TAO_Reverse_FlowSpec_Entry*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_Reverse_FlowSpec_Entry*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString,TAO_Reverse_FlowSpec_Entry*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_Reverse_FlowSpec_Entry*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_Reverse_FlowSpec_Entry*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString,TAO_Reverse_FlowSpec_Entry*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_Reverse_FlowSpec_Entry*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<ACE_CString,TAO_AV_UDP_MCast_Flow_Handler*>;
template class ACE_Hash_Map_Manager<ACE_CString,TAO_AV_UDP_MCast_Flow_Handler*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_AV_UDP_MCast_Flow_Handler*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString,TAO_AV_UDP_MCast_Flow_Handler*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_AV_UDP_MCast_Flow_Handler*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_AV_UDP_MCast_Flow_Handler*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString,TAO_AV_UDP_MCast_Flow_Handler*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_AV_UDP_MCast_Flow_Handler*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<ACE_CString,AVStreams::QoS>;
template class ACE_Hash_Map_Manager<ACE_CString,AVStreams::QoS,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, AVStreams::QoS, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString,AVStreams::QoS,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, AVStreams::QoS, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, AVStreams::QoS, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString,AVStreams::QoS,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, AVStreams::QoS, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

//template class ACE_Hash_Map_Manager<ACE_CString,TAO_FlowSpec_Entry *,ACE_Null_Mutex>;
//template class ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_FlowSpec_Entry *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString,TAO_FlowSpec_Entry *,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_FlowSpec_Entry *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_FlowSpec_Entry *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString,TAO_FlowSpec_Entry *,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_FlowSpec_Entry *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;


template class ACE_Equal_To<MMDevice_Map_Hash_Key>;
template class ACE_Hash<MMDevice_Map_Hash_Key>;
template class ACE_Array<char*>;
template class ACE_DLList<TAO_MCastConfigIf::Peer_Info>;
template class ACE_DLList_Iterator<TAO_MCastConfigIf::Peer_Info>;
template class ACE_DLList <TAO_FlowProducer>;
template class ACE_DLList_Iterator <TAO_FlowProducer>;
template class ACE_DLList <TAO_FlowConsumer>;
template class ACE_DLList_Iterator <TAO_FlowConsumer>;

template class ACE_Node <TAO_FlowSpec_Entry*>;
template class ACE_Unbounded_Set <TAO_FlowSpec_Entry*>;
template class ACE_Unbounded_Set_Iterator <TAO_FlowSpec_Entry*>;

template class ACE_Node <TAO_AV_Transport_Item*>;
template class ACE_Unbounded_Set<TAO_AV_Transport_Item*>;
template class ACE_Unbounded_Set_Iterator<TAO_AV_Transport_Item*>;

template class ACE_Node <TAO_AV_Flow_Protocol_Item*>;
template class ACE_Unbounded_Set<TAO_AV_Flow_Protocol_Item*>;
template class ACE_Unbounded_Set_Iterator<TAO_AV_Flow_Protocol_Item*>;

template class ACE_Node<AVStreams::FlowProducer *>;
template class ACE_Unbounded_Set<AVStreams::FlowProducer *>;
template class ACE_Unbounded_Set_Iterator<AVStreams::FlowProducer *>;

template class ACE_Node<AVStreams::FlowConsumer *>;
template class ACE_Unbounded_Set<AVStreams::FlowConsumer *>;
template class ACE_Unbounded_Set_Iterator<AVStreams::FlowConsumer *>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)


#pragma instantiate ACE_Hash_Map_Entry<ACE_CString,AVStreams::FDev_ptr>
#pragma instantaite ACE_Hash_Map_Entry<ACE_CString, TAO_FlowSpec_Entry *>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,AVStreams::FDev_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,TAO_FlowSpec_Entry *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, AVStreams::FDev_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,AVStreams::FDev_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, AVStreams::FDev_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, AVStreams::FDev_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString,AVStreams::FDev_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, AVStreams::FDev_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_FlowSpec_Entry *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString,AVStreams::FlowConnection_ptr>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,AVStreams::FlowConnection_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, AVStreams::FlowConnection_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,AVStreams::FlowConnection_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, AVStreams::FlowConnection_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, AVStreams::FlowConnection_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString,AVStreams::FlowConnection_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, AVStreams::FlowConnection_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString,TAO_AV_Flow_Handler*>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,TAO_AV_Flow_Handler*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_AV_Flow_Handler*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,TAO_AV_Flow_Handler*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_AV_Flow_Handler*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_AV_Flow_Handler*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString,TAO_AV_Flow_Handler*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_AV_Flow_Handler*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>


#pragma instantiate ACE_Hash_Map_Entry<ACE_CString,AVStreams::FlowEndPoint_ptr>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,AVStreams::FlowEndPoint_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, AVStreams::FlowEndPoint_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,AVStreams::FlowEndPoint_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, AVStreams::FlowEndPoint_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, AVStreams::FlowEndPoint_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString,AVStreams::FlowEndPoint_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, AVStreams::FlowEndPoint_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<MMDevice_Map_Hash_Key,TAO_StreamCtrl::MMDevice_Map_Entry>
#pragma instantiate ACE_Hash_Map_Manager<MMDevice_Map_Hash_Key,TAO_StreamCtrl::MMDevice_Map_Entry,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<MMDevice_Map_Hash_Key, TAO_StreamCtrl::MMDevice_Map_Entry, ACE_Hash<MMDevice_Map_Hash_Key>, ACE_Equal_To<MMDevice_Map_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<MMDevice_Map_Hash_Key,TAO_StreamCtrl::MMDevice_Map_Entry,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<MMDevice_Map_Hash_Key, TAO_StreamCtrl::MMDevice_Map_Entry, ACE_Hash<MMDevice_Map_Hash_Key>, ACE_Equal_To<MMDevice_Map_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<MMDevice_Map_Hash_Key, TAO_StreamCtrl::MMDevice_Map_Entry, ACE_Hash<MMDevice_Map_Hash_Key>, ACE_Equal_To<MMDevice_Map_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<MMDevice_Map_Hash_Key,TAO_StreamCtrl::MMDevice_Map_Entry,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<MMDevice_Map_Hash_Key, TAO_StreamCtrl::MMDevice_Map_Entry, ACE_Hash<MMDevice_Map_Hash_Key>, ACE_Equal_To<MMDevice_Map_Hash_Key>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString,TAO_AV_Acceptor*>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,TAO_AV_Acceptor*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_AV_Acceptor*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,TAO_AV_Acceptor*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_AV_Acceptor*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_AV_Acceptor*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString,TAO_AV_Acceptor*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_AV_Acceptor*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString,TAO_Forward_FlowSpec_Entry*>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,TAO_Forward_FlowSpec_Entry*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_Forward_FlowSpec_Entry*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,TAO_Forward_FlowSpec_Entry*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_Forward_FlowSpec_Entry*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_Forward_FlowSpec_Entry*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString,TAO_Forward_FlowSpec_Entry*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_Forward_FlowSpec_Entry*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString,TAO_Reverse_FlowSpec_Entry*>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,TAO_Reverse_FlowSpec_Entry*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_Reverse_FlowSpec_Entry*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,TAO_Reverse_FlowSpec_Entry*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_Reverse_FlowSpec_Entry*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_Reverse_FlowSpec_Entry*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString,TAO_Reverse_FlowSpec_Entry*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_Reverse_FlowSpec_Entry*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString,TAO_AV_UDP_MCast_Flow_Handler*>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,TAO_AV_UDP_MCast_Flow_Handler*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_AV_UDP_MCast_Flow_Handler*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,TAO_AV_UDP_MCast_Flow_Handler*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_AV_UDP_MCast_Flow_Handler*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_AV_UDP_MCast_Flow_Handler*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString,TAO_AV_UDP_MCast_Flow_Handler*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_AV_UDP_MCast_Flow_Handler*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString,AVStreams::QoS>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,AVStreams::QoS,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, AVStreams::QoS, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,AVStreams::QoS,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, AVStreams::QoS, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, AVStreams::QoS, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString,AVStreams::QoS,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, AVStreams::QoS, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString,TAO_FlowSpec_Entry *>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,TAO_FlowSpec_Entry *,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_FlowSpec_Entry *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,TAO_FlowSpec_Entry *,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_FlowSpec_Entry *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_FlowSpec_Entry *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString,TAO_FlowSpec_Entry *,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_FlowSpec_Entry *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Equal_To<MMDevice_Map_Hash_Key>
#pragma instantiate ACE_Hash<MMDevice_Map_Hash_Key>
#pragma instantiate ACE_Array<char*>
#pragma instantiate ACE_DLList<TAO_MCastConfigIf::Peer_Info>
#pragma instantiate ACE_DLList_Iterator<TAO_MCastConfigIf::Peer_Info>
#pragma instantiate ACE_DLList <TAO_FlowProducer>
#pragma instantiate ACE_DLList_Iterator <TAO_FlowProducer>
#pragma instantiate ACE_DLList <TAO_FlowConsumer>
#pragma instantiate ACE_DLList_Iterator <TAO_FlowConsumer>

#pragma instantiate ACE_Node <TAO_FlowSpec_Entry*>
#pragma instantiate ACE_Unbounded_Set <TAO_FlowSpec_Entry*>
#pragma instantiate ACE_Unbounded_Set_Iterator <TAO_FlowSpec_Entry*>

#pragma instantiate ACE_Node <TAO_AV_Transport_Item*>
#pragma instantiate ACE_Unbounded_Set<TAO_AV_Transport_Item*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_AV_Transport_Item*>

#pragma instantiate ACE_Node <TAO_AV_Flow_Protocol_Item*>
#pragma instantiate ACE_Unbounded_Set<TAO_AV_Flow_Protocol_Item*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_AV_Transport_Item*>

#pragma instantiate ACE_Node<AVStreams::FlowProducer *>
#pragma instantiate ACE_Unbounded_Set<AVStreams::FlowProducer *>
#pragma instantiate ACE_Unbounded_Set_Iterator<AVStreams::FlowProducer *>

#pragma instantiate ACE_Node<AVStreams::FlowConsumer *>
#pragma instantiate ACE_Unbounded_Set<AVStreams::FlowConsumer *>
#pragma instantiate ACE_Unbounded_Set_Iterator<AVStreams::FlowConsumer *>


#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
