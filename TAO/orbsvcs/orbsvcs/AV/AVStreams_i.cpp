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
#include "Transport.h"
#include "sfp.h"
#include "MCast.h"
#include "Policy.h"
#include "orbsvcs/Trader/Trader.h"


ACE_RCSID(AV, AVStreams_i, "$Id$")

int
deactivate_servant (PortableServer::Servant servant)
{
  // Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      PortableServer::POA_var poa = servant->_default_POA (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id = poa->servant_to_id (servant,
                                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa->deactivate_object (id.in (),
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"deactivate_servant");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

char *
get_flowname (const char *flow_spec_entry_str)
{
  ACE_CString flow_spec_entry (flow_spec_entry_str);
  int slash_pos = flow_spec_entry.find ('\\');
  ACE_CString flow_name;
  if (slash_pos != flow_spec_entry.npos)
    flow_name = flow_spec_entry.substring (0,slash_pos);
  else
    flow_name = flow_spec_entry_str;
  return CORBA::string_dup (flow_name.c_str ());
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
TAO_Basic_StreamCtrl::stop (const AVStreams::flowSpec &flow_spec,
                            CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow))
{
  ACE_TRY
    {
      // @@Call stop on the Related MediaCtrl.
      // call stop on the flow connections.
      if (this->flow_connection_map_.current_size () > 0)
        {
          if (flow_spec.length () > 0)
            for (u_int i=0;i<flow_spec.length ();i++)
              {
                char *flowname = get_flowname (flow_spec[i]);
                TAO_String_Hash_Key flow_name_key (flowname);
                FlowConnection_Map::ENTRY *flow_connection_entry = 0;
                if (this->flow_connection_map_.find (flow_name_key,flow_connection_entry) == 0)
                  {
                    flow_connection_entry->int_id_->stop (ACE_TRY_ENV);
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
                  entry->int_id_->stop (ACE_TRY_ENV);
                  ACE_TRY_CHECK;
                }
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_Basic_StreamCtrl::stop");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Start the transfer of data in the stream.
// Empty the_spec means apply operation to all flows
void
TAO_Basic_StreamCtrl::start (const AVStreams::flowSpec &flow_spec,
                             CORBA::Environment &ACE_TRY_ENV)
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
            for (u_int i=0;i<flow_spec.length ();i++)
              {
                char *flowname = get_flowname (flow_spec[i]);
                TAO_String_Hash_Key flow_name_key (flowname);
                FlowConnection_Map::ENTRY *flow_connection_entry = 0;
                if (this->flow_connection_map_.find (flow_name_key,flow_connection_entry) == 0)
                  {
                    flow_connection_entry->int_id_->start (ACE_TRY_ENV);
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
                  entry->int_id_->start (ACE_TRY_ENV);
                  ACE_TRY_CHECK;
                }
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_Basic_StreamCtrl::start");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Tears down the stream. This will close the connection, and delete
// the streamendpoint and vdev associated with this stream
// Empty the_spec means apply operation to all flows
void
TAO_Basic_StreamCtrl::destroy (const AVStreams::flowSpec &flow_spec,
                               CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::noSuchFlow))
{
  ACE_TRY
    {
      // call stop on the flow connections.
      if (this->flow_connection_map_.current_size () > 0)
        {
          if (flow_spec.length () > 0)
            for (u_int i=0;i<flow_spec.length ();i++)
              {
                char *flowname = get_flowname (flow_spec[i]);
                TAO_String_Hash_Key flow_name_key (flowname);
                FlowConnection_Map::ENTRY *flow_connection_entry = 0;
                if (this->flow_connection_map_.find (flow_name_key,flow_connection_entry) == 0)
                  flow_connection_entry->int_id_->destroy (ACE_TRY_ENV);
              }
          else
            {
              // call start on all the flows.
              FlowConnection_Map_Iterator iterator (this->flow_connection_map_);
              FlowConnection_Map_Entry *entry = 0;
              for (;iterator.next (entry) !=  0;iterator.advance ())
                {
                  entry->int_id_->destroy (ACE_TRY_ENV);
                  ACE_TRY_CHECK;
                }
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_Basic_StreamCtrl::destroy");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Changes the QoS associated with the stream
// Empty the_spec means apply operation to all flows
CORBA::Boolean
TAO_Basic_StreamCtrl::modify_QoS (AVStreams::streamQoS & new_qos,
                                  const AVStreams::flowSpec &the_spec,
                                  CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::noSuchFlow,
                     AVStreams::QoSRequestFailed))
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
TAO_Basic_StreamCtrl::push_event (const struct CosPropertyService::Property &the_event,
                                  CORBA::Environment & /* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,"\n(%P|%t) Recieved event \""));
}

// Sets the flow protocol status.
void
TAO_Basic_StreamCtrl::set_FPStatus (const AVStreams::flowSpec &flow_spec,
                                    const char  *fp_name,
                                    const CORBA::Any &fp_settings,
                                    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::FPError))

{
  if (!CORBA::is_nil (this->sep_a_.in ()))
    {
      this->sep_a_->set_FPStatus (flow_spec,fp_name,fp_settings,ACE_TRY_ENV);
      ACE_CHECK;
    }
}

// Gets the flow connection.
CORBA::Object_ptr
TAO_Basic_StreamCtrl::get_flow_connection (const char *flow_name,
                                           CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::noSuchFlow,
                     AVStreams::notSupported))
{
  TAO_String_Hash_Key flow_name_key (flow_name);
  FlowConnection_Map::ENTRY *flow_connection_entry = 0;
  if (this->flow_connection_map_.find (flow_name_key,flow_connection_entry) == 0)
    return AVStreams::FlowConnection::_duplicate (flow_connection_entry->int_id_);
  else
    ACE_THROW_RETURN (AVStreams::noSuchFlow (),CORBA::Object::_nil ());
}

// Sets the flow connection.
void
TAO_Basic_StreamCtrl::set_flow_connection (const char *flow_name,
                                           CORBA::Object_ptr flow_connection_obj,
                                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::notSupported))
{
  AVStreams::FlowConnection_ptr flow_connection = AVStreams::FlowConnection::_nil ();
  ACE_TRY
    {
      flow_connection = AVStreams::FlowConnection::_narrow (flow_connection_obj,ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_Basic_StreamCtrl::set_flow_connection");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
  // add the flowname and the flowconnection to the hashtable.
  this->flows_.length (this->flow_count_ + 1);
  this->flows_ [this->flow_count_++] = CORBA::string_dup (flow_name);
  TAO_String_Hash_Key flow_name_key (flow_name);
  if (this->flow_connection_map_.bind (flow_name_key,flow_connection) != 0)
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
                           const AVStreams::streamQoS &/* qos_spec */,
                           CORBA::Environment &/* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
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
        this->mmdevice_->_is_equivalent (hash_key.mmdevice_,ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"MMDevice_Map_Hash_Key::operator == ");
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
      CORBA::ULong left_hash = left.mmdevice_->_hash (left.hash_maximum_,
                                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::ULong right_hash = right.mmdevice_->_hash (right.hash_maximum_,
                                                        ACE_TRY_ENV);
      ACE_TRY_CHECK;
      result = left_hash < right_hash;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"operator < for MMDevice_Map_Hash_Key");
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
      result = this->mmdevice_->_hash (this->hash_maximum_,
                                       ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"MMDevice_Map_Hash_Key::hash");
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
      this->streamctrl_ = this->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_StreamCtrl::TAO_StreamCtrl");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

TAO_StreamCtrl::~TAO_StreamCtrl (void)
{
  delete this->mcastconfigif_;
}

// Stop the transfer of data of the stream
// Empty the_spec means apply operation to all flows
void
TAO_StreamCtrl::stop (const AVStreams::flowSpec &flow_spec,
                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow))
{
  ACE_TRY
    {
      TAO_Basic_StreamCtrl::stop (flow_spec,ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (this->flow_connection_map_.current_size () > 0)
        return;
      MMDevice_Map_Iterator a_iterator (this->mmdevice_a_map_);
      MMDevice_Map::ENTRY *entry = 0;
      for (;a_iterator.next (entry)!= 0;a_iterator.advance ())
        {
          entry->int_id_.sep_->stop (flow_spec, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      MMDevice_Map_Iterator b_iterator (this->mmdevice_b_map_);
      for (;b_iterator.next (entry)!= 0;b_iterator.advance ())
        {
          entry->int_id_.sep_->stop (flow_spec, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_Basic_StreamCtrl::stop");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Start the transfer of data in the stream.
// Empty the_spec means apply operation to all flows
void
TAO_StreamCtrl::start (const AVStreams::flowSpec &flow_spec,
                       CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow))
{
  ACE_TRY
    {
      TAO_Basic_StreamCtrl::start (flow_spec,ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (this->flow_connection_map_.current_size () > 0)
        return;
      MMDevice_Map_Iterator a_iterator (this->mmdevice_a_map_);
      MMDevice_Map::ENTRY *entry = 0;
      for (;a_iterator.next (entry)!= 0;a_iterator.advance ())
        {
          entry->int_id_.sep_->start (flow_spec, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      MMDevice_Map_Iterator b_iterator (this->mmdevice_b_map_);
      for (;b_iterator.next (entry)!= 0;b_iterator.advance ())
        {
          entry->int_id_.sep_->start (flow_spec, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_Basic_StreamCtrl::start");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Tears down the stream. This will close the connection, and delete
// the streamendpoint and vdev associated with this stream
// Empty the_spec means apply operation to all flows
void
TAO_StreamCtrl::destroy (const AVStreams::flowSpec &flow_spec,
                         CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::noSuchFlow))
{
  ACE_TRY
    {
      TAO_Basic_StreamCtrl::destroy (flow_spec,ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (this->flow_connection_map_.current_size () > 0)
        return;
      MMDevice_Map_Iterator a_iterator (this->mmdevice_a_map_);
      MMDevice_Map::ENTRY *entry = 0;
      for (;a_iterator.next (entry)!= 0;a_iterator.advance ())
        {
          entry->int_id_.sep_->destroy (flow_spec, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      MMDevice_Map_Iterator b_iterator (this->mmdevice_b_map_);
      for (;b_iterator.next (entry)!= 0;b_iterator.advance ())
        {
          entry->int_id_.sep_->destroy (flow_spec, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_Basic_StreamCtrl::destroy");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
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
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed))
{
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
          MMDevice_Map_Hash_Key find_key (a_party);
          MMDevice_Map_Entry find_entry;
          int result =
            this->mmdevice_a_map_.find (find_key,find_entry);
          if (result == 0)
            {
              // Already in the map.
              ACE_DEBUG ((LM_DEBUG,"mmdevice a_party is already bound\n"));
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
                                    the_flows,
                                    ACE_TRY_ENV);
              ACE_TRY_CHECK;

              ACE_DEBUG ((LM_DEBUG,"(%P|%t) TAO_StreamCtrl::create_A: succeeded\n"));
              // Define ourselves as the related_streamctrl property of the sep.
              CORBA::Any streamctrl_any;
              streamctrl_any <<= this->streamctrl_.in ();
              this->sep_a_->define_property ("Related_StreamCtrl",
                                             streamctrl_any,
                                             ACE_TRY_ENV);
              ACE_TRY_CHECK;
              // add the mmdevice,sep and vdev to the map.
              MMDevice_Map_Entry map_entry;
              MMDevice_Map_Hash_Key key (a_party);
              map_entry.sep_ = AVStreams::StreamEndPoint_A::_duplicate (this->sep_a_.in ());
              map_entry.vdev_ = AVStreams::VDev::_duplicate (this->vdev_a_.in ());
              map_entry.flowspec_ = the_flows;
              map_entry.qos_ = the_qos;
              result =
                this->mmdevice_a_map_.bind (key,map_entry);
              if (result < 0)
                ACE_DEBUG ((LM_DEBUG,"Error binding mmdevice entry in the a_map"));
            }
        }
      // Request b_party to create the endpoint and vdev
      if (!CORBA::is_nil (b_party))
        {
          MMDevice_Map_Hash_Key find_key (b_party);
          MMDevice_Map_Entry find_entry;
          int result =
            this->mmdevice_b_map_.find (find_key,find_entry);
          if (result == 0)
            {
              // Already in the map.
              ACE_DEBUG ((LM_DEBUG,"mmdevice b_party is already bound\n"));
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
                                    the_flows,
                                    ACE_TRY_ENV);
              ACE_TRY_CHECK;

              ACE_DEBUG ((LM_DEBUG,"(%P|%t) TAO_StreamCtrl::create_B: succeeded\n"));

              ACE_DEBUG ((LM_DEBUG,
                          "\n(%P|%t)stream_endpoint_b_ = %s",
                          TAO_ORB_Core_instance ()->orb ()->object_to_string (this->sep_b_.in (),
                                                                              ACE_TRY_ENV)));
              ACE_TRY_CHECK;
              // Define ourselves as the related_streamctrl property of the sep.
              CORBA::Any streamctrl_any;
              streamctrl_any <<= this->streamctrl_.in ();
              this->sep_b_->define_property ("Related_StreamCtrl",
                                             streamctrl_any,
                                             ACE_TRY_ENV);
              ACE_TRY_CHECK;

              // add the mmdevice,sep and vdev to the map.
              MMDevice_Map_Entry map_entry;
              MMDevice_Map_Hash_Key key (b_party);
              map_entry.sep_ = AVStreams::StreamEndPoint::_duplicate (this->sep_b_.in ());
              map_entry.vdev_ = AVStreams::VDev::_duplicate(this->vdev_b_.in ());
              map_entry.flowspec_ = the_flows;
              map_entry.qos_ = the_qos;
              int result =
                this->mmdevice_b_map_.bind (key,map_entry);
              if (result < 0)
                ACE_DEBUG ((LM_DEBUG,"Error binding mmdevice entry in the b_map"));
            }
        }

// In the full profile case there's no VDev.
      if (CORBA::is_nil (b_party) && (!CORBA::is_nil (this->vdev_a_.in ())))
        {
          if (!this->mcastconfigif_)
            {
              ACE_NEW_RETURN (this->mcastconfigif_,
                              TAO_MCastConfigIf,
                              0);
              this->mcastconfigif_ptr_ = this->mcastconfigif_->_this (ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          // Multicast source being added.
          CORBA::Boolean result = this->vdev_a_->set_Mcast_peer (this->streamctrl_.in (),
                                                                 this->mcastconfigif_ptr_.in (),
                                                                 the_qos,
                                                                 the_flows,
                                                                 ACE_TRY_ENV);
          if (!result)
            ACE_ERROR_RETURN ((LM_ERROR,"set_Mcast_peer failed\n"),0);
        }

      if (CORBA::is_nil (a_party))
        {
          if (!CORBA::is_nil (this->vdev_b_.in ()))
            {
              // Multicast sink being added.
              if (!this->mcastconfigif_)
                ACE_ERROR_RETURN ((LM_ERROR,"first add a source and then a sink\n"),0);
              this->mcastconfigif_->set_peer (this->vdev_b_.in (),
                                              the_qos,
                                              the_flows,
                                              ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }

          int connect_leaf_success = 0;
          ACE_TRY_EX (connect_leaf)
            {
              connect_leaf_success = this->sep_a_->connect_leaf (this->sep_b_.in (),
                                                                 the_qos,
                                                                 the_flows,
                                                                 ACE_TRY_ENV);
              ACE_TRY_CHECK_EX (connect_leaf);
              connect_leaf_success = 1;
            }
          ACE_CATCH (AVStreams::notSupported,ex)
            {
              ACE_DEBUG ((LM_DEBUG,"connect_leaf failed\n"));
              connect_leaf_success = 0;
            }
          ACE_CATCHANY
            {
              ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_StreamCtrl::bind_devs");
            }
          ACE_ENDTRY;
          ACE_CHECK_RETURN (0);
          if (!connect_leaf_success)
            {
              AVStreams::flowSpec connect_flows = the_flows;
              this->sep_a_->multiconnect (the_qos,connect_flows,ACE_TRY_ENV);
              ACE_TRY_CHECK;
              this->sep_b_->multiconnect (the_qos,connect_flows,ACE_TRY_ENV);
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
                                       the_flows,
                                       ACE_TRY_ENV);

              ACE_TRY_CHECK;
              this->vdev_b_->set_peer (this->streamctrl_.in (),
                                       this->vdev_a_.in (),
                                       the_qos,
                                       the_flows,
                                       ACE_TRY_ENV);

              ACE_TRY_CHECK;

              // Now connect the streams together. This will
              // establish the connection
              CORBA::Boolean result  =
                this->sep_a_->connect (this->sep_b_.in (),
                                       the_qos,
                                       the_flows,
                                       ACE_TRY_ENV);
              ACE_TRY_CHECK;
              if (result == 0)
                ACE_ERROR_RETURN ((LM_ERROR,"sep_a->connect (sep_b) failed\n"),0);
            }
          else
            {
              // Its full profile
              // we have feps in the sep then dont call connect instead call bind on the streamctrl.
              this->bind (this->sep_a_.in (),
                          this->sep_b_.in (),
                          the_qos,
                          the_flows,
                          ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
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
                      AVStreams::streamQoS &stream_qos,
                      const AVStreams::flowSpec &flow_spec,
                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed))
{
  this->sep_a_ = sep_a;
  this->sep_b_ = sep_b;

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
                              sep_any,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
      sep_any <<= sep_a;
      sep_b->define_property ("PeerAdapter",
                              sep_any,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
      // since its full profile we do the viable stream setup algorithm.
      // get the flows for the A streamendpoint.
      // the flows spec is empty and hence we do a exhaustive match.
      AVStreams::flowSpec a_flows,b_flows;
      CORBA::Any_var flows_any;
      flows_any = sep_a->get_property_value ("Flows",ACE_TRY_ENV);
      ACE_TRY_CHECK;
      AVStreams::flowSpec_ptr temp_flows;
      flows_any.in () >>= temp_flows;
      a_flows = *temp_flows;
      flows_any = sep_b->get_property_value ("Flows",ACE_TRY_ENV);
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
            sep_a->get_fep (flowname,
                            ACE_TRY_ENV);
          ACE_TRY_CHECK;
          AVStreams::FlowEndPoint_ptr fep =
            AVStreams::FlowEndPoint::_narrow (fep_obj.in (),
                                              ACE_TRY_ENV);
          ACE_TRY_CHECK;
          TAO_String_Hash_Key fep_key (flowname);
          result = a_fep_map->bind (fep_key,fep);
          if (result == -1)
            ACE_DEBUG ((LM_DEBUG,"TAO_StreamCtrl::bind failed for %s",flowname));
        }
      // get the flowendpoints for streamendpoint_b
      for (i=0;i<b_flows.length ();i++)
        {
          const char *flowname = b_flows[i];
          // get the flowendpoint references.
          CORBA::Object_var fep_obj =
            sep_b->get_fep (flowname,
                            ACE_TRY_ENV);
          ACE_TRY_CHECK;
          AVStreams::FlowEndPoint_ptr fep =
            AVStreams::FlowEndPoint::_narrow (fep_obj.in (),
                                              ACE_TRY_ENV);
          ACE_TRY_CHECK;
          TAO_String_Hash_Key fep_key (flowname);
          result = b_fep_map->bind (fep_key,fep);
          if (result == -1)
            ACE_DEBUG ((LM_DEBUG,"TAO_StreamCtrl::bind failed for %s",flowname));
        }
      FlowEndPoint_Map *map_a = 0,*map_b = 0;
      if (flow_spec.length () == 0)
        {
          map_a = a_fep_map;
          map_b = b_fep_map;
        }
      else
        {
          FlowEndPoint_Map *spec_fep_map_a,*spec_fep_map_b;
          ACE_NEW_RETURN (spec_fep_map_a,
                          FlowEndPoint_Map,
                          0);
          ACE_NEW_RETURN (spec_fep_map_b,
                          FlowEndPoint_Map,
                          0);
          for (u_int i=0; i< flow_spec.length ();i++)
            {
              TAO_Forward_FlowSpec_Entry *entry;
              ACE_NEW_RETURN (entry,
                              TAO_Forward_FlowSpec_Entry,
                              0);
              entry->parse (flow_spec[i].in ());
              TAO_String_Hash_Key fep_key (entry->flowname ());
              AVStreams::FlowEndPoint_ptr fep;
              result = a_fep_map->find (fep_key,fep);
              if (result == -1)
                ACE_ERROR_RETURN ((LM_ERROR,"Fep not found on A side for flowname: %s",flow_spec[i].in ()),0);

              result = spec_fep_map_a->bind (fep_key,fep);
              if (result == -1)
                ACE_DEBUG ((LM_DEBUG,"Bind faile for %s",flow_spec[i].in ()));

              result = b_fep_map->find (fep_key,fep);
              if (result == -1)
                ACE_ERROR_RETURN ((LM_ERROR,"Fep not found on B side for flowname: %s",flow_spec[i].in ()),0);

              result = spec_fep_map_b->bind (fep_key,fep);
              if (result == -1)
                ACE_DEBUG ((LM_DEBUG,"Bind faile for %s",flow_spec[i].in ()));
            }
          map_a = spec_fep_map_a;
          map_b = spec_fep_map_b;
        }

      TAO_AV_QoS qos (stream_qos);
      // Now go thru the list of flow endpoint and match them.
      // uses the first match policy.
      FlowEndPoint_Map_Iterator a_feps_iterator (*map_a), b_feps_iterator (*map_b);
      FlowEndPoint_Map_Entry *a_feps_entry, *b_feps_entry;
      ACE_TRY_EX (flow_connect)
        {

          for (;a_feps_iterator.next (a_feps_entry) != 0;a_feps_iterator.advance ())
            {
              for (;b_feps_iterator.next (b_feps_entry) != 0; b_feps_iterator.advance ())
                {
                  AVStreams::FlowEndPoint_ptr fep_a = a_feps_entry->int_id_;
                  AVStreams::FlowEndPoint_ptr fep_b = b_feps_entry->int_id_;
                  AVStreams::FlowConnection_var flow_connection;
                  if (CORBA::is_nil (fep_b->get_connected_fep ()))
                    {
                      if (fep_a->is_fep_compatible (fep_b,
                                                    ACE_TRY_ENV) == 1)
                        {
                          ACE_TRY_CHECK_EX (flow_connect);
                          // assume that flow names are same so that we
                          // can use either of them.
                          CORBA::Object_var flow_connection_obj;
                          CORBA::Any_var flowname_any = fep_a->get_property_value ("Flow",
                                                                                   ACE_TRY_ENV);
                          ACE_TRY_CHECK_EX (flow_connect);
                          char *flowname = 0;
                          flowname_any.in () >>= flowname;
                          ACE_TRY_EX (flow_connection)
                            {
                              flow_connection_obj = 
                              this->get_flow_connection (flowname,
                                                         ACE_TRY_ENV);
                              ACE_TRY_CHECK_EX (flow_connection);
                              flow_connection = AVStreams::FlowConnection::_narrow (flow_connection_obj.in (),
                                                                                    ACE_TRY_ENV);
                              ACE_TRY_CHECK_EX (flow_connection);
                            }
                          ACE_CATCHANY
                            {
                              TAO_FlowConnection *flowConnection;
                              ACE_NEW_RETURN (flowConnection,
                                              TAO_FlowConnection,
                                              0);
                              flow_connection = flowConnection->_this (ACE_TRY_ENV);
                              ACE_TRY_CHECK_EX (flow_connect);
                              this->set_flow_connection (flowname,
                                                         flow_connection,
                                                         ACE_TRY_ENV);
                              ACE_TRY_CHECK_EX (flow_connect);
                            }
                          ACE_ENDTRY;
                          ACE_CHECK_RETURN (0);

                          // make sure that a_feps is flow_producer and b_feps is flow_consumer
                          // There should be a way to find which flow endpoint is producer and which is consumer.
                          AVStreams::FlowProducer_var producer = AVStreams::FlowProducer::_nil ();
                          AVStreams::FlowConsumer_var consumer = AVStreams::FlowConsumer::_nil ();

                          ACE_TRY_EX (producer_check)
                            {
                              producer = AVStreams::FlowProducer::_narrow (fep_a,ACE_TRY_ENV);
                              ACE_TRY_CHECK_EX (producer_check);
                              consumer =
                                AVStreams::FlowConsumer::_narrow (fep_b,ACE_TRY_ENV);
                              ACE_TRY_CHECK_EX (producer_check);
                            }
                          ACE_CATCHANY
                            {
                              producer = AVStreams::FlowProducer::_narrow (fep_b,ACE_TRY_ENV);
                              ACE_TRY_CHECK_EX (flow_connect);
                              consumer = AVStreams::FlowConsumer::_narrow (fep_a,ACE_TRY_ENV);
                              ACE_TRY_CHECK_EX (flow_connect);
                            }
                          ACE_ENDTRY;
                          ACE_CHECK_RETURN (0);
                          CORBA::String_var fep_a_name,fep_b_name;
                          CORBA::String temp_name;
                          flowname_any = fep_a->get_property_value ("FlowName",ACE_TRY_ENV);
                          flowname_any.in () >>= temp_name;
                          fep_a_name = CORBA::string_dup (temp_name);
                          flowname_any = fep_b->get_property_value ("FlowName",ACE_TRY_ENV);
                          flowname_any.in () >>= temp_name;
                          fep_b_name = CORBA::string_dup (temp_name);
                          AVStreams::QoS flow_qos;
                          flow_qos.QoSType = fep_a_name;
                          flow_qos.QoSParams.length (0);
                          result = qos.get_flow_qos (fep_a_name.in (),flow_qos);
                          if (result == -1)
                            {
                              flow_qos.QoSType = fep_b_name;
                              result = qos.get_flow_qos (fep_b_name.in (),flow_qos);
                              if (result == -1)
                                ACE_DEBUG ((LM_DEBUG,"No QoS Specified for this flow"));
                            }
                          flow_connection->connect (producer,consumer,flow_qos,ACE_TRY_ENV);
                          ACE_TRY_CHECK_EX (flow_connect);
                        }
                    }
                }
            }
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_StreamCtrl::bind:flow_connect block");
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
                             flow_spec,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

void
TAO_StreamCtrl::unbind (CORBA::Environment &/* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed))
{
}

void
TAO_StreamCtrl::unbind_party (AVStreams::StreamEndPoint_ptr /* the_ep */,
                              const AVStreams::flowSpec &/* the_spec */,
                              CORBA::Environment &/* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::noSuchFlow))
{
}

void
TAO_StreamCtrl::unbind_dev (AVStreams::MMDevice_ptr /* dev */,
                            const AVStreams::flowSpec & /* the_spec */,
                            CORBA::Environment &/* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::noSuchFlow))
{
}

AVStreams::VDev_ptr
TAO_StreamCtrl::get_related_vdev (AVStreams::MMDevice_ptr adev,
                                  AVStreams::StreamEndPoint_out sep,
                                  CORBA::Environment &/* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed))
{
  MMDevice_Map_Hash_Key key (adev);
  MMDevice_Map_Entry entry;
  int result = -1;
  result = this->mmdevice_a_map_.find (key,entry);
  if (result < 0)
    {
      result = this->mmdevice_a_map_.find (key,entry);
      if (result < 0)
        return AVStreams::VDev::_nil ();
    }
  sep = AVStreams::StreamEndPoint::_duplicate (entry.sep_.in ());
  return AVStreams::VDev::_duplicate (entry.vdev_.in ());
}

CORBA::Boolean
TAO_StreamCtrl::modify_QoS (AVStreams::streamQoS &the_qos,
                            const AVStreams::flowSpec &the_spec,
                            CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed))
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

TAO_MCastConfigIf::TAO_MCastConfigIf (void)
  :peer_list_iterator_ (peer_list_)
{
}

// In future this should be a multicast message instead of point-to-point unicasts.
CORBA::Boolean
TAO_MCastConfigIf::set_peer (CORBA::Object_ptr peer,
                             AVStreams::streamQoS & qos,
                             const AVStreams::flowSpec & flow_spec,
                             CORBA::Environment &ACE_TRY_ENV)
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
      info->peer_ = AVStreams::VDev::_narrow (peer,ACE_TRY_ENV);
      ACE_TRY_CHECK;
      info->qos_ = qos;
      info->flow_spec_ = flow_spec;
      this->peer_list_.insert_tail (info);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_MCastConfigIf::set_peer");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

// In future this should be a multicast message instead of point-to-point unicasts.
void
TAO_MCastConfigIf::configure (const CosPropertyService::Property & a_configuration,
                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Peer_Info *info;
  ACE_TRY
    {
      this->peer_list_iterator_.first ();
      info = this->peer_list_iterator_.next ();
      while (info != 0)
        {
          info->peer_->configure (a_configuration,ACE_TRY_ENV);
          ACE_TRY_CHECK;
          info = this->peer_list_iterator_.next ();
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_MCastConfigIf::set_configure");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}


void
TAO_MCastConfigIf::set_initial_configuration (const CosPropertyService::Properties &initial,
                                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->initial_configuration_ = initial;
}

// In future this should be a multicast message instead of point-to-point unicasts.
void
TAO_MCastConfigIf::set_format (const char * flowName,
                               const char * format_name,
                               CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported))
{
  Peer_Info *info;
  ACE_TRY
    {
      this->peer_list_iterator_.first ();
      info = this->peer_list_iterator_.next ();
      while (info != 0)
        {
          if (this->in_flowSpec (info->flow_spec_,flowName))
            {
              info->peer_->set_format (flowName,format_name,ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          info = this->peer_list_iterator_.next ();
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_MCastConfigIf::set_format");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// In future this should be a multicast message instead of point-to-point unicasts.
void
TAO_MCastConfigIf::set_dev_params (const char * flowName,
                                   const CosPropertyService::Properties & new_params,
                                   CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::PropertyException,
                   AVStreams::streamOpFailed))
{
  Peer_Info *info;
  ACE_TRY
    {

      this->peer_list_iterator_.first ();
      info = this->peer_list_iterator_.next ();
      while (info != 0)
        {
          if (this->in_flowSpec (info->flow_spec_,flowName))
            {
              info->peer_->set_dev_params (flowName,new_params,ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          info = this->peer_list_iterator_.next ();
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_MCastConfigIf::set_dev_params");
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
    if (ACE_OS::strncmp (flow_spec[i],flow_name,len) == 0)
      {
        return 1;
      }
  return 0;
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

int
TAO_Base_StreamEndPoint::handle_open (void)
{
  return 0;
}

int
TAO_Base_StreamEndPoint::handle_stop (const AVStreams::flowSpec &the_spec,
                                      CORBA::Environment &env)
{
  return 0;
}

int
TAO_Base_StreamEndPoint::handle_start (const AVStreams::flowSpec &the_spec,
                                       CORBA::Environment &env)
{
  return 0;
}

int
TAO_Base_StreamEndPoint::handle_destroy (const AVStreams::flowSpec &the_spec,
                                         CORBA::Environment &env)
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
TAO_Base_StreamEndPoint::handle_connection_requested (AVStreams::flowSpec &,
                                                 CORBA::Environment &)
{
  return 1;
}

int
TAO_Base_StreamEndPoint::set_protocol_object (const char *flowname,
                                              TAO_AV_Protocol_Object *sfp_object)
{
  return -1;
}

int
TAO_Base_StreamEndPoint::get_callback (const char *flowname,
                                       TAO_AV_Callback *&sfp_callback)
{
  return -1;
}

// ----------------------------------------------------------------------
// TAO_StreamEndPoint
// ----------------------------------------------------------------------

// constructor.

TAO_StreamEndPoint::TAO_StreamEndPoint (void)
  :flow_count_ (0),
   flow_num_ (0),
   mcast_port_ (ACE_DEFAULT_MULTICAST_PORT)
{
  this->mcast_addr_ = ACE_OS::inet_addr (ACE_DEFAULT_MULTICAST_ADDR);
  ACE_DEBUG ((LM_DEBUG,"TAO_StreamEndPoint::TAO_StreamEndPoint::mcast_addr = %ud",this->mcast_addr_));
  //  this->handle_open ();
}


CORBA::Boolean
TAO_StreamEndPoint::connect (AVStreams::StreamEndPoint_ptr responder,
                             AVStreams::streamQoS &qos,
                             const AVStreams::flowSpec &the_spec,
                             CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Boolean retv = 0;
  this->peer_sep_ = AVStreams::StreamEndPoint::_duplicate (responder);
  ACE_TRY_EX (negotiate)
    {
      if (!CORBA::is_nil (this->negotiator_.in ()))
        {
          CORBA::Any_var negotiator_any = responder->get_property_value ("Negotiator");
          if (negotiator_any != 0)
            {
              AVStreams::Negotiator_ptr peer_negotiator;
              negotiator_any.in () >>= peer_negotiator;
              if (!CORBA::is_nil (peer_negotiator))
                {
                  CORBA::Boolean result =
                    this->negotiator_->negotiate (peer_negotiator,
                                                  qos,
                                                  ACE_TRY_ENV);
                  ACE_TRY_CHECK_EX (negotiate);
                  if (!result)
                    ACE_DEBUG ((LM_DEBUG,"TAO_StreamEndPoint::Connect (): negotiate failed\n"));
                }
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_StreamEndPoint::negotiate");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  ACE_TRY_EX (available_protocols)
    {
      // choose protocols based on what the remote endpoint can support.
      CORBA::Any_var protocols_any =
        responder->get_property_value ("AvailableProtocols",ACE_TRY_ENV);
      ACE_TRY_CHECK_EX (available_protocols);
      AVStreams::protocolSpec peer_protocols;
      AVStreams::protocolSpec_ptr temp_protocols;
      protocols_any.in () >>= temp_protocols;
      peer_protocols = *temp_protocols;
      for (u_int i=0;i<peer_protocols.length ();i++)
        {
          if (this->protocol_ != 0)
            break;
          for (u_int j=0;j<this->protocols_.length ();j++)
            if (ACE_OS::strcmp (peer_protocols [i],
                                this->protocols_[j]) == 0)
              {
                    // we'll agree upon the first protocol that matches.
                this->protocol_ = CORBA::string_copy (peer_protocols [i]);
                break;
              }
        }
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,"Availableprotocols property not defined\n"));
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  ACE_TRY
    {
      AVStreams::streamQoS network_qos;
      int result = this->translate_qos (qos,network_qos);
      if (result != 0)
        ACE_DEBUG ((LM_DEBUG,"QoS translation failed\n"));

      AVStreams::flowSpec flow_spec (the_spec);
      this->handle_preconnect (flow_spec);
      ACE_DEBUG ((LM_DEBUG,"TAO_StreamEndPoint::connect: flow_spec_length = %d",
                  flow_spec.length ()));
      u_int i;
      for (i=0;i<flow_spec.length ();i++)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO_StreamEndPoint::connect:%s\n",
                      flow_spec[i].in ()));
          TAO_Forward_FlowSpec_Entry *entry = 0;
          ACE_NEW_RETURN (entry,
                          TAO_Forward_FlowSpec_Entry,
                          0);
          if (entry->parse (flow_spec[i]) == -1)
            return 0;
          this->forward_flow_spec_set.insert (entry);
        }
      result =
        TAO_AV_CORE::instance ()->
        init_forward_flows (this,
                            this->forward_flow_spec_set,
                            TAO_AV_Core::TAO_AV_ENDPOINT_A,
                            flow_spec);
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_Core::init_forward_flows failed\n"),0);

      AVStreams::StreamEndPoint_var streamendpoint = this->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      retv = responder->request_connection (streamendpoint,
                                            0,
                                            network_qos,
                                            flow_spec,
                                            ACE_TRY_ENV);

      ACE_TRY_CHECK;
      for (i=0;i<flow_spec.length ();i++)
        {
          TAO_Reverse_FlowSpec_Entry *entry = 0;
          ACE_NEW_RETURN (entry,
                          TAO_Reverse_FlowSpec_Entry,
                          0);
          if (entry->parse (flow_spec[i].in ()) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,"Reverse_Flow_Spec_Set::parse failed\n"),0);
          this->reverse_flow_spec_set.insert (entry);
        }
      result = TAO_AV_CORE::instance ()->init_reverse_flows (this,
                                                             this->forward_flow_spec_set,
                                                             this->reverse_flow_spec_set,
                                                             TAO_AV_Core::TAO_AV_ENDPOINT_A);
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_Core::init_reverse_flows failed\n"),0);
      // Make the upcall to the app
      retv = this->handle_postconnect (flow_spec);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_StreamEndPoint::connect");
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
    }
  return 0;
}

// Stop the physical flow of data on the stream
// Empty the_spec --> apply to all flows

void
TAO_StreamEndPoint::stop (const AVStreams::flowSpec &flow_spec,
                          CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow))
{
  // Make the upcall into the app
  this->handle_stop (flow_spec, ACE_TRY_ENV);
  for (u_int i=0;i<flow_spec.length ();i++)
    {
      TAO_AV_FlowSpecSetItor end = this->forward_flow_spec_set.end ();
      for (TAO_AV_FlowSpecSetItor begin = this->forward_flow_spec_set.begin ();
           begin != end; ++begin)
        {
          if (ACE_OS::strcmp ((*begin)->flowname (),flow_spec [i].in ()) == 0)
            {
              (*begin)->protocol_object ()->stop ();
              break;
            }
        }
    }
}

// Start the physical flow of data on the stream
// Empty the_spec --> apply to all flows
void
TAO_StreamEndPoint::start (const AVStreams::flowSpec &flow_spec,
                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow))
{
  // Make the upcall into the app
  this->handle_start (flow_spec, ACE_TRY_ENV);
  // Now call start on all the flow handlers.
  for (u_int i=0;i<flow_spec.length ();i++)
    {
      TAO_AV_FlowSpecSetItor end = this->forward_flow_spec_set.end ();
      for (TAO_AV_FlowSpecSetItor begin = this->forward_flow_spec_set.begin ();
           begin != end; ++begin)
        {
          TAO_FlowSpec_Entry *entry = *begin;
          if (ACE_OS::strcmp (entry->flowname (),flow_spec [i]) == 0)
            {
              entry->protocol_object ()->start ();
              break;
            }
        }
    }
}

// Close the connection
void
TAO_StreamEndPoint::destroy (const AVStreams::flowSpec &/* the_spec */,
                             CORBA::Environment &/* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow))
{
  // Make the upcall into the app
  //  this->handle_destroy (the_spec, ACE_TRY_ENV);
  //
  int result = deactivate_servant (this);
  if (result < 0)
    ACE_DEBUG ((LM_DEBUG,"TAO_StreamEndPoint::destroy failed\n"));
}

// Called by our peer endpoint, requesting us to establish
// a connection
CORBA::Boolean
TAO_StreamEndPoint::request_connection (AVStreams::StreamEndPoint_ptr initiator,
                                        CORBA::Boolean is_mcast,
                                        AVStreams::streamQoS &qos,
                                        AVStreams::flowSpec &flow_spec,
                                        CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpDenied,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed,
                   AVStreams::FPError))

{
  int result = 0;
  ACE_TRY
    {
      ACE_DEBUG ((LM_DEBUG, "\n(%P|%t) TAO_StreamEndPoint::request_connection called"));
      ACE_DEBUG ((LM_DEBUG,
                  "\n(%P|%t) TAO_StreamEndPoint::request_connection: "
                  "flowspec has length = %d"
                  "and the strings are:",
                  flow_spec.length ()));
      u_int i;
      for (i = 0; i < flow_spec.length (); i++)
        ACE_DEBUG ((LM_DEBUG,flow_spec [i]));

      for (i=0;i<flow_spec.length ();i++)
        {
          TAO_Forward_FlowSpec_Entry *entry = 0;
          ACE_NEW_RETURN (entry,
                          TAO_Forward_FlowSpec_Entry,
                          0);
          if (entry->parse (flow_spec[i]) == -1)
            return 0;
          this->forward_flow_spec_set.insert (entry);
        }
      TAO_AV_CORE::instance ()->init_forward_flows (this,
                                                    this->forward_flow_spec_set,
                                                    TAO_AV_Core::TAO_AV_ENDPOINT_B,
                                                    flow_spec);
      // Make the upcall to the app
      result = this->handle_connection_requested (flow_spec,ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_StreamEndpoint::request_connection");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return result;
}

// Refers to modification of transport QoS.
CORBA::Boolean
TAO_StreamEndPoint::modify_QoS (AVStreams::streamQoS &/* new_qos */,
                                const AVStreams::flowSpec &/* the_flows */,
                                CORBA::Environment &/* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed))
{
  return 0;
}

// Sets the list of protocols this streamendpoint can understand.

CORBA::Boolean
TAO_StreamEndPoint::set_protocol_restriction (const AVStreams::protocolSpec &protocols,
                                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      CORBA::Any protocol_restriction_any;

      protocol_restriction_any <<= protocols;
      this->define_property ("ProtocolRestriction",
                             protocol_restriction_any,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
      this->protocols_ = protocols;
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
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::streamOpFailed))
{
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

// Sets the status of the flow protocol.

void
TAO_StreamEndPoint::set_FPStatus (const AVStreams::flowSpec &the_spec,
                                  const char *fp_name,
                                  const CORBA::Any &fp_settings,
                                  CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::FPError))
{
  if (ACE_OS::strcmp (fp_name,"SFP1.0") != 0)
    return;
  fp_settings >>= this->sfp_status_;
}


CORBA::Object_ptr
TAO_StreamEndPoint::get_fep (const char *flow_name,
                             CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported,
                   AVStreams::noSuchFlow))
{
  TAO_String_Hash_Key fep_name_key (flow_name);
  FlowEndPoint_Map::ENTRY *fep_entry = 0;
  if (this->fep_map_.find (fep_name_key,fep_entry) == 0)
    return fep_entry->int_id_;
  return 0;
}


char *
TAO_StreamEndPoint::add_fep (CORBA::Object_ptr fep_obj,
                             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::notSupported,
                     AVStreams::streamOpFailed))
{
  char *flow_name = 0;
  AVStreams::FlowEndPoint_ptr fep = AVStreams::FlowEndPoint::_nil ();
  ACE_TRY_EX (flow_name);
  {
    ACE_NEW_RETURN (flow_name,char [BUFSIZ],0);
    CORBA::Any_ptr flow_name_any;
    fep = AVStreams::FlowEndPoint::_narrow (fep_obj,ACE_TRY_ENV);
    ACE_TRY_CHECK_EX (flow_name);
    flow_name_any = fep->get_property_value ("FlowName",ACE_TRY_ENV);
    ACE_TRY_CHECK_EX (flow_name);
    *flow_name_any >>= flow_name;
  }
  ACE_CATCHANY
    {
      // exception implies the flow name is not defined and is system generated.
      ACE_OS::sprintf (flow_name,"flow%d",flow_num_++);
      ACE_TRY_EX (flow)
        {
          // exception implies the flow name is not defined and is system generated.
          ACE_OS::sprintf (flow_name,"flow%d",flow_num_++);
          CORBA::Any flowname_any;
          flowname_any <<= flow_name;
          fep->define_property ("Flow",flowname_any,ACE_TRY_ENV);
          ACE_TRY_CHECK_EX (flow);
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_StreamEndPoint::add_fep");
          return 0;
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (0);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  ACE_TRY
    {
      fep->lock (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      // Add it to the sequence of flowNames supported.
      // put the flowname and the flowendpoint in a hashtable.
      TAO_String_Hash_Key fep_name_key (CORBA::string_dup (flow_name));
      if (this->fep_map_.bind (fep_name_key,fep) != 0)
        {
          ACE_THROW_RETURN (AVStreams::streamOpFailed (),0);
        }
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
      ACE_TRY_CHECK;
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
    ACE_THROW_SPEC ((CORBA::SystemException,
                     AVStreams::notSupported,
                     AVStreams::streamOpFailed))
{
  ACE_TRY
    {
      TAO_String_Hash_Key fep_name_key (flow_name);
      AVStreams::FlowEndPoint_ptr fep_entry = 0;
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

// Sets the negotiator object.
void
TAO_StreamEndPoint::set_negotiator (AVStreams::Negotiator_ptr new_negotiator,
                                    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      CORBA::Any negotiator;
      negotiator <<= new_negotiator;
      this->define_property ("Negotiator",
                             negotiator,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
      this->negotiator_ = AVStreams::Negotiator::_duplicate (new_negotiator);
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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->key_ = the_key;
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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->source_id_ = source_id;
}

CORBA::Boolean
TAO_StreamEndPoint::multiconnect (AVStreams::streamQoS &the_qos,
                                  AVStreams::flowSpec &flow_spec,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  return 0;
}

TAO_StreamEndPoint::~TAO_StreamEndPoint (void)
{
  //this->handle_close ();
  TAO_AV_FlowSpecSetItor begin = this->forward_flow_spec_set.begin ();
  TAO_AV_FlowSpecSetItor end = this->forward_flow_spec_set.end ();
  for ( ; begin != end; ++begin)
    {
      TAO_FlowSpec_Entry *entry = *begin;
      delete entry;
    }
  begin = this->reverse_flow_spec_set.begin ();
  end = this->reverse_flow_spec_set.end ();
  for (; begin != end; ++begin)
    {
      TAO_FlowSpec_Entry *entry = *begin;
      delete entry;
    }
}


// ----------------------------------------------------------------------
// TAO_StreamEndPoint_A
// ----------------------------------------------------------------------

TAO_StreamEndPoint_A::TAO_StreamEndPoint_A (void)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) TAO_StreamEndPoint_A::TAO_StreamEndPoint_A: created\n"));
}

// IP Multicast style connect.
CORBA::Boolean
TAO_StreamEndPoint_A::multiconnect (AVStreams::streamQoS &stream_qos,
                                    AVStreams::flowSpec &flow_spec,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_DEBUG ((LM_DEBUG,"TAO_StreamEndPoint::multiconnect\n"));
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
          TAO_String_Hash_Key mcast_key (forward_entry->flowname ());
          AVStreams::FlowEndPoint_ptr flow_endpoint = AVStreams::FlowEndPoint::_nil ();
          if (this->fep_map_.find (mcast_key,flow_endpoint) == 0)
            {
              ACE_TRY_EX (narrow)
                {
                  AVStreams::QoS flow_qos;
                  result = qos.get_flow_qos (forward_entry->flowname (),flow_qos);
                  if (result < 0)
                    ACE_DEBUG ((LM_DEBUG,"QoS not found for %s",forward_entry->flowname ()));
                  // Narrow it to FlowProducer.
                  AVStreams::FlowProducer_var producer = AVStreams::FlowProducer::_narrow (flow_endpoint,ACE_TRY_ENV);
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
                                streamctrl_any = this->get_property_value ("Related_StreamCtrl",
                                                                           ACE_TRY_ENV);
                                ACE_TRY_CHECK;
                                AVStreams::StreamCtrl_ptr streamctrl;
                                streamctrl_any.in () >>= streamctrl;
                                this->streamctrl_ = AVStreams::StreamCtrl::_duplicate (streamctrl);
                              }

                          CORBA::Object_var flow_connection_obj = 
                            this->streamctrl_->get_flow_connection (forward_entry->flowname (),
                                                                    ACE_TRY_ENV);
                          ACE_TRY_CHECK_EX (flow_connection);
                          flow_connection = AVStreams::FlowConnection::_narrow (flow_connection_obj.in (),
                                                                                ACE_TRY_ENV);
                          ACE_TRY_CHECK_EX (flow_connection);
                        }
                      ACE_CATCHANY
                        {
                          TAO_FlowConnection *flowConnection;
                          ACE_NEW_RETURN (flowConnection,
                                          TAO_FlowConnection,
                                          0);
                          flowConnection->set_mcast_addr (this->mcast_addr_,this->mcast_port_++);
                          flow_connection = flowConnection->_this (ACE_TRY_ENV);
                          ACE_TRY_CHECK;
                          this->streamctrl_->set_flow_connection (forward_entry->flowname (),
                                                                  flow_connection,
                                                                  ACE_TRY_ENV);
                          ACE_TRY_CHECK;
                        }
                      ACE_ENDTRY;
                      ACE_CHECK_RETURN (0);
                      result = flow_connection->add_producer (producer.in (),
                                                             flow_qos,
                                                             ACE_TRY_ENV);
                      ACE_TRY_CHECK;
                      if (result == 0)
                        ACE_ERROR_RETURN ((LM_ERROR,"TAO_StreamEndPoint_A::multiconnect: add_producer failed\n"),0);
                    }
                }
              ACE_CATCHANY
                {
                  // Narrow failed and since its not a flowproducer its an error.
                  ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"FlowProducer::_narrow");
                  ACE_ERROR_RETURN ((LM_ERROR,"sep_a doesn't contain a flowproducer"),0);
                }
              ACE_ENDTRY;
              ACE_CHECK_RETURN (0);
            }
          ACE_INET_Addr *mcast_addr;
          TAO_FlowSpec_Entry *entry = 0;
          result = this->mcast_entry_map_.find (mcast_key,entry);
          if (result == 0)
            {
              mcast_addr = ACE_dynamic_cast (ACE_INET_Addr *,entry->get_local_addr ());
              char str_addr [BUFSIZ];
              result = mcast_addr->addr_to_string (str_addr,BUFSIZ);
              if (result < 0)
                ACE_ERROR_RETURN ((LM_ERROR,"TAO_StreamEndPoint::multiconnect ::addr_to_string failed\n"),0);
              ACE_DEBUG ((LM_DEBUG,"TAO_StreamEndPoint_A::multiconnect:%s\n",str_addr));
              TAO_Forward_FlowSpec_Entry new_entry (entry->flowname (),
                                                    entry->direction_str (),
                                                    entry->format (),
                                                    entry->flow_protocol_str (),
                                                    entry->carrier_protocol_str (),
                                                    entry->get_local_addr ());
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
                    mcast_addr->set (this->mcast_port_++,this->mcast_addr_);
                    char buf[BUFSIZ];
                    mcast_addr->addr_to_string (buf,BUFSIZ);
                    ACE_DEBUG ((LM_DEBUG,"%s\n",buf));
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

                    this->forward_flow_spec_set.insert (new_entry);
                    TAO_AV_Acceptor_Registry *acceptor_registry = TAO_AV_CORE::instance ()->acceptor_registry ();
                    result = acceptor_registry->open (this,
                                                      TAO_AV_CORE::instance (),
                                                      this->forward_flow_spec_set);
                    if (result < 0)
                      ACE_ERROR_RETURN ((LM_ERROR,"Acceptor_Registry::open failed\n"),0);
                    result = this->mcast_entry_map_.bind (mcast_key,new_entry);
                    if (result < 0)
                      ACE_ERROR_RETURN ((LM_ERROR,"mcast_entry::bind failed"),0);
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
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_StreamEndPoint_A::multiconnect");
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
                                    const AVStreams::flowSpec & /* the_flows */,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (AVStreams::notSupported (),0);
  return 0;
}

// Multicast not supported yet.
void
TAO_StreamEndPoint_A::disconnect_leaf (AVStreams::StreamEndPoint_B_ptr /* the_ep */,
                                            const AVStreams::flowSpec & /* theSpec */,
                                            CORBA::Environment &ACE_TRY_ENV)
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
  ACE_DEBUG ((LM_DEBUG,
              "\n(%P|%t) TAO_StreamEndPoint_B::TAO_StreamEndPoint_B: created"));
}

CORBA::Boolean
TAO_StreamEndPoint_B::multiconnect (AVStreams::streamQoS &stream_qos,
                                    AVStreams::flowSpec &flow_spec,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_DEBUG ((LM_DEBUG,"TAO_StreamEndPoint_B::multiconnect\n"));
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
          TAO_String_Hash_Key mcast_key (forward_entry->flowname ());
          AVStreams::FlowEndPoint_var flow_endpoint;
          if (this->fep_map_.find (mcast_key,flow_endpoint.out ()) == 0)
            {
              AVStreams::FlowConsumer_var consumer;
              ACE_TRY_EX (narrow)
                {
                  consumer = AVStreams::FlowConsumer::_narrow (flow_endpoint,ACE_TRY_ENV);
                  ACE_TRY_CHECK_EX (narrow);
                }
              ACE_CATCHANY
                {
                  ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"FlowConsumer::_narrow");
                  ACE_ERROR_RETURN ((LM_ERROR,"sep_b doesn't contain a flowconsumer"),0);
                }
              ACE_ENDTRY;
              ACE_CHECK_RETURN (0);
              AVStreams::QoS flow_qos;
              result = qos.get_flow_qos (forward_entry->flowname (),flow_qos);
              if (result < 0)
                ACE_DEBUG ((LM_DEBUG,"QoS not found for %s",forward_entry->flowname ()));
              AVStreams::FlowConnection_var flow_connection;
              ACE_TRY_EX (flow_connection)
                {
                  if (CORBA::is_nil (this->streamctrl_.in ()))
                    {
                      CORBA::Any_var streamctrl_any;
                      streamctrl_any = this->get_property_value ("Related_StreamCtrl",
                                                                 ACE_TRY_ENV);
                      ACE_TRY_CHECK;
                      AVStreams::StreamCtrl_ptr streamctrl;
                      streamctrl_any.in () >>= streamctrl;
                      this->streamctrl_ = AVStreams::StreamCtrl::_duplicate (streamctrl);
                    }
                  CORBA::Object_var flow_connection_obj = 
                    this->streamctrl_->get_flow_connection (forward_entry->flowname (),
                                                            ACE_TRY_ENV);
                  ACE_TRY_CHECK_EX (flow_connection);
                  flow_connection = AVStreams::FlowConnection::_narrow (flow_connection_obj.in (),
                                                                        ACE_TRY_ENV);
                  ACE_TRY_CHECK_EX (flow_connection);
                }
              ACE_CATCHANY
                {
                  ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_StreamEndPoint_B::multiconnect::get_flow_connection");
                  return 0;
                }
              ACE_ENDTRY;
              ACE_CHECK_RETURN (0);
              result = flow_connection->add_consumer (consumer.in (),
                                                      flow_qos,
                                                      ACE_TRY_ENV);
              ACE_TRY_CHECK;
              if (result == 0)
                ACE_ERROR_RETURN ((LM_ERROR,"TAO_StreamEndPoint_B::multiconnect:add_consumer failed\n"),0);
            }
          else
            {
              TAO_FlowSpec_Entry *mcast_entry = 0;
              ACE_INET_Addr *mcast_addr;
              mcast_addr = ACE_dynamic_cast (ACE_INET_Addr *,forward_entry->address ());
              if (mcast_addr == 0)
                ACE_ERROR_RETURN ((LM_ERROR,"TAO_StreamEndPoint_B::multiconnect::Address missing in flowspec_entry\n"),0);
              result = this->mcast_entry_map_.find (mcast_key,mcast_entry);
              if (result == 0)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,"TAO_StreamEndPoint_B::multiconnect::handler already found\n"),0);
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
                          ACE_ERROR_RETURN ((LM_ERROR,"connector_registry::open failed\n"),0);
                        result = this->mcast_entry_map_.bind (mcast_key,forward_entry);
                        if (result < 0)
                          ACE_ERROR_RETURN ((LM_ERROR,"dgram_mcast_handler::bind failed"),0);
                      }
                      break;
                    case TAO_FlowSpec_Entry::TAO_AV_DIR_OUT:
                      // OUT implies we're the source.,which is an error.
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
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_StreamEndPoint_B::multiconnect");
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

      this->streamctrl_ = AVStreams::StreamCtrl::_duplicate (the_ctrl);
      this->peer_ = AVStreams::VDev::_duplicate (the_peer_dev);

      CORBA::Any_var anyptr;
      CORBA::String media_ctrl_ior;
      anyptr = this->peer_->get_property_value ("Related_MediaCtrl",
                                                ACE_TRY_ENV);
      ACE_TRY_CHECK;

      anyptr.in () >>= media_ctrl_ior;
      ACE_DEBUG ((LM_DEBUG,"(%P|%t)The Media Control IOR is %s\n",
                  media_ctrl_ior));
      CORBA::Object_ptr media_ctrl_obj =
        TAO_ORB_Core_instance ()->orb ()->string_to_object
        (media_ctrl_ior,ACE_TRY_ENV);
      ACE_TRY_CHECK;

      result = this->set_media_ctrl (media_ctrl_obj,ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_VDev::set_peer");
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

// Sets the multicast VDev peer.
CORBA::Boolean
TAO_VDev::set_Mcast_peer (AVStreams::StreamCtrl_ptr /* the_ctrl */,
                          AVStreams::MCastConfigIf_ptr mcast_peer,
                          AVStreams::streamQoS &/* the_qos */,
                          const AVStreams::flowSpec &/* the_spec */,
                          CORBA::Environment &/* ACE_TRY_ENV */)
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
TAO_VDev::configure (const CosPropertyService::Property &/*the_config_mesg*/,
                     CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::PropertyException,
                   AVStreams::streamOpFailed))
{
}

// sets the media format used for the flowname as a property.
void
TAO_VDev::set_format (const char *flowName,
                      const char *format_name,
                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported))
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
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::PropertyException,
                   AVStreams::streamOpFailed))
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
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed))
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
                    const AVStreams::flowSpec &the_spec,
                    CORBA::Environment &ACE_TRY_ENV)
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
      AVStreams::MMDevice_var mmdevice = this->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      this->stream_ctrl_->bind_devs (peer_device,
                                     mmdevice.in (),
                                     the_qos,
                                     the_spec,
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
      streamctrl = this->stream_ctrl_->_this (ACE_TRY_ENV);
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
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::noSuchFlow,
                   AVStreams::QoSRequestFailed))
{
  ACE_UNUSED_ARG (first_peer);
  ACE_UNUSED_ARG (the_qos);
  ACE_UNUSED_ARG (is_met);
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

AVStreams::StreamEndPoint_ptr
TAO_MMDevice::create_A_B (MMDevice_Type type,
                          AVStreams::StreamCtrl_ptr streamctrl,
                          AVStreams::VDev_out the_vdev,
                          AVStreams::streamQoS &the_qos,
                          CORBA::Boolean_out met_qos,
                          char *&named_vdev,
                          const AVStreams::flowSpec &flow_spec,
                          CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::streamOpDenied,
                   AVStreams::notSupported,
                   AVStreams::QoSRequestFailed,
                   AVStreams::noSuchFlow))
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
                                                    the_vdev,
                                                    ACE_TRY_ENV) == -1)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%P|%t) Error in create_A\n"),
                                0);
            sep = sep_a;
          }
          break;
        case MMDEVICE_B:
          {
            if (this->endpoint_strategy_->create_B (sep_b,
                                                    the_vdev,
                                                    ACE_TRY_ENV) == -1)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%P|%t) Error in create_A\n"),
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
              TAO_String_Hash_Key flow_key (forward_entry.flowname ());
              AVStreams::FDev_ptr flow_dev;
              AVStreams::FlowConnection_var flowconnection = AVStreams::FlowConnection::_nil ();
              ACE_TRY_EX (flowconnection)
                {
                  // Get the flowconnection for this flow.
                  CORBA::Object_var flowconnection_obj =
                    streamctrl->get_flow_connection (forward_entry.flowname (),ACE_TRY_ENV);
                  ACE_TRY_CHECK_EX (flowconnection);
                  if (!CORBA::is_nil (flowconnection_obj.in ()))
                    {
                      flowconnection = AVStreams::FlowConnection::_narrow (flowconnection_obj,ACE_TRY_ENV);
                      ACE_TRY_CHECK_EX (flowconnection);
                    }
                }
              ACE_CATCHANY
                {
                  ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_MMDevice::create_a::get_flow_connection");
                }
              ACE_ENDTRY;
              ACE_CHECK_RETURN (0);

              int result = this->fdev_map_.find (flow_key,flow_dev);
              if (result < 0)
                ACE_ERROR_RETURN ((LM_ERROR,"fdev_map::find failed\n"),0);
              CORBA::String_var named_fdev;
              AVStreams::FlowEndPoint_var flow_endpoint;
              AVStreams::QoS flow_qos;
              result = qos.get_flow_qos (forward_entry.flowname (),flow_qos);
              if (result < 0)
                ACE_DEBUG ((LM_DEBUG,"get_flow_qos failed for %s\n",forward_entry.flowname ()));
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
                                                       named_fdev.inout (),
                                                       ACE_TRY_ENV);
                        }
                        break;
                      case MMDEVICE_B:
                        {
                          flow_endpoint =
                            flow_dev->create_consumer (flowconnection.in (),
                                                       flow_qos,
                                                       met_qos,
                                                       named_fdev.inout (),
                                                       ACE_TRY_ENV);
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
                            flow_dev->create_consumer (flowconnection,
                                                       flow_qos,
                                                       met_qos,
                                                       named_fdev.inout (),
                                                       ACE_TRY_ENV);
                        }
                        break;
                      case MMDEVICE_B:
                        {
                          // In implies flow is from A to B and
                          // hence A is the producer for this flow and B is the consumer for this flow.
                          // We have to create a producer from the FDev for this flow.
                          flow_endpoint =
                            flow_dev->create_producer (flowconnection,
                                                       flow_qos,
                                                       met_qos,
                                                       named_fdev.inout (),
                                                       ACE_TRY_ENV);
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
              flow_endpoint->define_property ("FlowName",flowname_any,ACE_TRY_ENV);
              ACE_TRY_CHECK;
              sep->add_fep (flow_endpoint,ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_MMDevice::create_A");
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
                        const AVStreams::flowSpec &flow_spec,
                        CORBA::Environment &ACE_TRY_ENV)
{
  AVStreams::StreamEndPoint_A_ptr sep_a;
  AVStreams::StreamEndPoint_ptr sep;
  ACE_TRY
    {
      sep = this->create_A_B (MMDEVICE_A,streamctrl,the_vdev,stream_qos,met_qos,named_vdev,flow_spec,ACE_TRY_ENV);
      ACE_TRY_CHECK;
      sep_a = AVStreams::StreamEndPoint_A::_narrow (sep,ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_MMDevice::create_A");
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
                        const AVStreams::flowSpec &flow_spec,
                        CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::streamOpDenied,
                   AVStreams::notSupported,
                   AVStreams::QoSRequestFailed,
                   AVStreams::noSuchFlow))
{
  AVStreams::StreamEndPoint_B_ptr sep_b = AVStreams::StreamEndPoint_B::_nil ();
  AVStreams::StreamEndPoint_ptr sep;
  ACE_TRY
    {
      sep = this->create_A_B (MMDEVICE_B,streamctrl,the_vdev,stream_qos,met_qos,named_vdev,flow_spec,ACE_TRY_ENV);
      ACE_TRY_CHECK;
      sep_b = AVStreams::StreamEndPoint_B::_narrow (sep,ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_MMDevice::create_A");
      return sep_b;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (sep_b);
  return sep_b;
}


// destroys the streamendpoint and the Vdev.
void
TAO_MMDevice::destroy (AVStreams::StreamEndPoint_ptr /* the_ep */,
                       const char * /* vdev_name */,
                       CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported))
{
  // Remove self from POA.  Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.
  int result = deactivate_servant (this);
  if (result < 0)
    ACE_DEBUG ((LM_DEBUG,"TAO_MMDevice::destroy failed\n"));
}

// Adds the fdev object to the MMDevice.
char *
TAO_MMDevice::add_fdev (CORBA::Object_ptr fdev_obj,
                        CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported,
                   AVStreams::streamOpFailed))
{
  char *flow_name = 0;
  AVStreams::FDev_ptr fdev = AVStreams::FDev::_nil ();
  ACE_TRY_EX (flow_name)
    {
      ACE_NEW_RETURN (flow_name,char [BUFSIZ],0);
      CORBA::Any_ptr flow_name_any;
      fdev = AVStreams::FDev::_narrow (fdev_obj,ACE_TRY_ENV);
      ACE_TRY_CHECK_EX (flow_name);
      flow_name_any = fdev->get_property_value ("Flow",ACE_TRY_ENV);
      ACE_TRY_CHECK_EX (flow_name);
      *flow_name_any >>= flow_name;
    }
  ACE_CATCHANY
    {
      ACE_TRY_EX (flow)
        {
          // exception implies the flow name is not defined and is system generated.
          ACE_OS::sprintf (flow_name,"flow%d",flow_num_++);
          CORBA::Any flowname_any;
          flowname_any <<= flow_name;
          fdev->define_property ("Flow",flowname_any,ACE_TRY_ENV);
          ACE_TRY_CHECK_EX (flow);
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_MMDevice::add_fdev");
          return 0;
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (0);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  if (CORBA::is_nil (fdev))
    return 0;

  // Add it to the sequence of flowNames supported.
  // put the flowname and the fdev in a hashtable.
  TAO_String_Hash_Key fdev_name_key (CORBA::string_dup (flow_name));
  if (this->fdev_map_.bind (fdev_name_key,fdev) != 0)
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
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported,
                   AVStreams::noSuchFlow))
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  TAO_String_Hash_Key fdev_name_key (flow_name);
  FDev_Map::ENTRY *fdev_entry = 0;
  if (this->fdev_map_.find (fdev_name_key,fdev_entry) == 0)
    return AVStreams::FDev::_duplicate (fdev_entry->int_id_);
  return 0;
}

// Removes the fdev from this MMDevice.
void
TAO_MMDevice::remove_fdev (const char *flow_name,
                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported,
                   AVStreams::noSuchFlow))
{
  ACE_TRY
    {
      TAO_String_Hash_Key fdev_name_key (flow_name);
      AVStreams::FDev_ptr fdev_entry = AVStreams::FDev::_nil ();
      // Remove the fep from the hash table.
      if (this->fdev_map_.unbind (fdev_name_key,fdev_entry)!= 0)
        ACE_THROW (AVStreams::streamOpFailed ());

      if (!CORBA::is_nil (fdev_entry))
        CORBA::release (fdev_entry);

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
   ip_multicast_ (1)
{
}

int
TAO_FlowConnection::set_mcast_addr (ACE_UINT32 mcast_addr, u_short mcast_port)
{
  this->mcast_addr_ = mcast_addr;
  this->mcast_port_ = mcast_port;
  return 0;
}

// stop this flow.
void
TAO_FlowConnection::stop (CORBA::Environment &ACE_TRY_ENV)
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
          (*producer_begin)->stop (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      FlowConsumer_SetItor consumer_begin = this->flow_consumer_set_.begin
        ();
      for (FlowConsumer_SetItor consumer_end =
             this->flow_consumer_set_.end ();
           consumer_begin != consumer_end; ++consumer_begin)
        {
          (*consumer_begin)->stop (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FlowConnection::stop");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// start this flow.
void
TAO_FlowConnection::start (CORBA::Environment &ACE_TRY_ENV)
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
          (*consumer_begin)->start (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      FlowProducer_SetItor producer_begin = this->flow_producer_set_.begin
        ();
      for (FlowProducer_SetItor producer_end =
             this->flow_producer_set_.end ();
           producer_begin != producer_end; ++producer_begin)
        {
          (*producer_begin)->start (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FlowConnection::start");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// destroy this flow.
void
TAO_FlowConnection::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      FlowProducer_SetItor producer_begin = this->flow_producer_set_.begin
        ();
      for (FlowProducer_SetItor producer_end =
             this->flow_producer_set_.end ();
           producer_begin != producer_end; ++producer_begin)
        {
          (*producer_begin)->start (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      FlowConsumer_SetItor consumer_begin = this->flow_consumer_set_.begin
        ();
      for (FlowConsumer_SetItor consumer_end =
             this->flow_consumer_set_.end ();
           consumer_begin != consumer_end; ++consumer_begin)
        {
          (*consumer_begin)->start (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FlowConnection::destroy");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
  int result = deactivate_servant (this);
  if (result < 0)
    ACE_DEBUG ((LM_DEBUG,"TAO_FlowConnection::destroy failed\n"));
}

// modify the QoS for this flow.
CORBA::Boolean
TAO_FlowConnection::modify_QoS (AVStreams::QoS & new_qos,
                                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::QoSRequestFailed))
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
        (fp_name,fp_settings,ACE_TRY_ENV); 
      ACE_CHECK_RETURN (0);
    }
  FlowConsumer_SetItor consumer_begin = this->flow_consumer_set_.begin
    ();
  for (FlowConsumer_SetItor consumer_end =
         this->flow_consumer_set_.end ();
       consumer_begin != consumer_end; ++consumer_begin)
    {
      (*consumer_begin)->use_flow_protocol
        (fp_name,fp_settings,ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }
  return 1;
}

void
TAO_FlowConnection::push_event (const AVStreams::streamEvent & the_event,
                                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (the_event);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

CORBA::Boolean
TAO_FlowConnection::connect_devs (AVStreams::FDev_ptr a_party,
                                  AVStreams::FDev_ptr b_party,
                                  AVStreams::QoS & flow_qos,
                                  CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::streamOpDenied,
                   AVStreams::QoSRequestFailed))
{
  CORBA::Boolean result = 0;
  ACE_TRY
    {
      AVStreams::FlowConnection_var flowconnection = this->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::Boolean met_qos;
      CORBA::String_var named_fdev ((const char *)"");
      AVStreams::FlowProducer_var producer =
        a_party->create_producer (flowconnection,
                                  flow_qos,
                                  met_qos,
                                  named_fdev.inout (),
                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;
      AVStreams::FlowConsumer_var consumer =
        b_party->create_consumer (flowconnection,
                                  flow_qos,
                                  met_qos,
                                  named_fdev.inout (),
                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;
      result = this->connect (producer.in (),
                              consumer.in (),
                              flow_qos,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FlowConnection::connect_devs");
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
                             AVStreams::QoS & the_qos,
                             CORBA::Environment &ACE_TRY_ENV)
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
        this->_this (ACE_TRY_ENV);
      flow_producer->set_peer (flowconnection.in (),
                               flow_consumer,
                               the_qos,
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      flow_consumer->set_peer (flowconnection.in (),
                               flow_producer,
                               the_qos,
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      char *consumer_address =
        flow_consumer->go_to_listen (the_qos,
                                     0,// false for is_mcast
                                     flow_producer,
                                     this->fp_name_.inout (),
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      flow_producer->connect_to_peer (the_qos,
                                      consumer_address,
                                      this->fp_name_.inout (),
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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return  0;
}

CORBA::Boolean
TAO_FlowConnection::add_producer (AVStreams::FlowProducer_ptr producer,
                                  AVStreams::QoS & the_qos,
                                  CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::alreadyConnected,
                   AVStreams::notSupported))
{
  ACE_TRY
    {
      if (this->producer_address_.in () == 0)
        {
          AVStreams::FlowProducer_ptr flow_producer =
            AVStreams::FlowProducer::_duplicate (producer);
          this->flow_producer_set_.insert (flow_producer);
          CORBA::Boolean met_qos;
          ACE_INET_Addr mcast_addr (this->mcast_port_,
                                    this->mcast_addr_);
          char mcast_address[BUFSIZ];
          mcast_addr.addr_to_string (mcast_address,BUFSIZ);
          char *address = flow_producer->connect_mcast (the_qos,
                                                               met_qos,
                                                               mcast_address,
                                                               this->fp_name_.in (),
                                                               ACE_TRY_ENV);
          ACE_TRY_CHECK;
          TAO_Forward_FlowSpec_Entry entry ("","","","",address);
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
          // set the multicast peer.
          if (CORBA::is_nil (this->mcastconfigif_.in ()))
            {
              ACE_NEW_RETURN (this->mcastconfigif_i_,
                              TAO_MCastConfigIf,
                              0);
              this->mcastconfigif_ = this->mcastconfigif_i_->_this (ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          AVStreams::FlowConnection_var flowconnection = this->_this (ACE_TRY_ENV);
          ACE_TRY_CHECK;
          flow_producer->set_Mcast_peer (flowconnection.in (),
                                         this->mcastconfigif_,
                                         the_qos,
                                         ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FlowConnection::add_producer");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

CORBA::Boolean
TAO_FlowConnection::add_consumer (AVStreams::FlowConsumer_ptr consumer,
                                  AVStreams::QoS & the_qos,
                                  CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::alreadyConnected))
{
  ACE_TRY
    {
      AVStreams::FlowConsumer_ptr flow_consumer = 
        AVStreams::FlowConsumer::_duplicate (consumer);
      this->flow_consumer_set_.insert (flow_consumer);
      FlowProducer_SetItor begin = this->flow_producer_set_.begin ();
      // @@Lets take that the first entry as the only producer. We're
      // not sure if we can have multiple flow producers in a
      // flowconnection.
      AVStreams::FlowProducer_ptr flow_producer = (*begin);

      AVStreams::protocolSpec protocols (1);
      protocols.length (1);
      protocols [0] = CORBA::string_dup (this->producer_address_);
      if (!this->ip_multicast_)
        {
          flow_consumer->set_protocol_restriction (protocols,
                                                          ACE_TRY_ENV);
          ACE_TRY_CHECK;
          char * address =
            flow_consumer->go_to_listen (the_qos,
                                         1,
                                         flow_producer,
                                         this->fp_name_.inout (),
                                         ACE_TRY_ENV);
          ACE_TRY_CHECK;
          CORBA::Boolean is_met;
          flow_producer->connect_mcast (the_qos,
                                        is_met,
                                        address,
                                        this->fp_name_.inout (),
                                        ACE_TRY_ENV);
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
                                                 this->fp_name_.inout (),
                                                 ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      if (CORBA::is_nil (this->mcastconfigif_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,"TAO_FlowConnection::add_consumer: first add a producer and then a consumer\n"),0);
      // @@ Is this the right place to do set_peer?
      AVStreams::flowSpec flow_spec;
      AVStreams::streamQoS stream_qos (1);
      stream_qos.length (1);
      stream_qos [0] = the_qos;
      this->mcastconfigif_->set_peer (flow_consumer,
                                      stream_qos,
                                      flow_spec,
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FlowConnection::add_consumer");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

CORBA::Boolean
TAO_FlowConnection::drop (AVStreams::FlowEndPoint_ptr target,
                          CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notConnected))
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
  :lock_ (0)
{
}

TAO_FlowEndPoint::TAO_FlowEndPoint (const char *flowname,
                                    AVStreams::protocolSpec &protocols,
                                    const char *format)
{
  this->open (flowname,protocols,format);
}

int
TAO_FlowEndPoint::open (const char *flowname,
                        AVStreams::protocolSpec &protocols,
                        const char *format)
{
  this->flowname_ = flowname;
  this->format_ = format;

  ACE_DEBUG ((LM_DEBUG,"TAO_FlowEndPoint::open\n"));
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::Any flowname_any;
      flowname_any <<= flowname;
      this->define_property ("Flow",
                             flowname_any,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
      this->set_format (format,
                        ACE_TRY_ENV);
      ACE_TRY_CHECK;
      this->protocol_addresses_ = protocols;
      AVStreams::protocolSpec protocol_spec (protocols.length ());
      protocol_spec.length (protocols.length ());
      ACE_DEBUG ((LM_DEBUG,"%N:%l\n"));
      for (u_int i=0;i<protocols.length ();i++)
        {
          CORBA::String_var address = CORBA::string_dup (protocols [i].in ());
          TAO_Forward_FlowSpec_Entry entry ("","","","",address.in ());
          protocol_spec [i] = CORBA::string_dup (entry.carrier_protocol_str ());
          ACE_DEBUG ((LM_DEBUG,"%s\n",protocol_spec[i].in ()));
        }
      this->set_protocol_restriction (protocol_spec,
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FlowEndPoint::open");
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
TAO_FlowEndPoint::lock (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // lock the current flowendpoint
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  if (this->lock_)
    return 0;
  this->lock_ = 1;
  return 1;
}

// unlocks the flowendpoint ,becomes free to be used in another flow.
void
TAO_FlowEndPoint::unlock (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  this->lock_ = 0;
}

// The start,stop and destroy are to be handled by the application.
void
TAO_FlowEndPoint::stop (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->protocol_object_->stop ();
}

void
TAO_FlowEndPoint::start (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->protocol_object_->start ();
}

void
TAO_FlowEndPoint::destroy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  int result = deactivate_servant (this);
  if (result < 0)
    ACE_DEBUG ((LM_DEBUG,"TAO_StreamEndPoint::destroy failed\n"));
}

AVStreams::StreamEndPoint_ptr
TAO_FlowEndPoint::related_sep (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return AVStreams::StreamEndPoint::_duplicate (this->related_sep_);
}

void
TAO_FlowEndPoint::related_sep (AVStreams::StreamEndPoint_ptr related_sep,
                               CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  this->related_sep_ = AVStreams::StreamEndPoint::_duplicate (related_sep);
}

AVStreams::FlowConnection_ptr
TAO_FlowEndPoint::related_flow_connection (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return AVStreams::FlowConnection::_duplicate (this->related_flow_connection_);
}

void
TAO_FlowEndPoint::related_flow_connection (AVStreams::FlowConnection_ptr related_flow_connection,
                                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  this->related_flow_connection_ = AVStreams::FlowConnection::_duplicate (related_flow_connection);
}

// returns the connected peer for this flow
AVStreams::FlowEndPoint_ptr
TAO_FlowEndPoint::get_connected_fep (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notConnected,
                   AVStreams::notSupported))
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return this->peer_fep_;
}

CORBA::Boolean
TAO_FlowEndPoint::use_flow_protocol (const char * fp_name,
                                     const CORBA::Any & fp_settings,
                                     CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::FPError,
                   AVStreams::notSupported))
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
TAO_FlowEndPoint::set_protocol_restriction (const AVStreams::protocolSpec & protocols,
                                            CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported))
{
  ACE_TRY
    {
      ACE_DEBUG ((LM_DEBUG,"%N:%l\n"));
      for (u_int i=0;i<protocols.length ();i++)
        {
          const char *protocol = (protocols)[i].in ();
          ACE_DEBUG ((LM_DEBUG,"%s\n",protocol));
        }
      CORBA::Any AvailableProtocols_property;
      AvailableProtocols_property <<= protocols;
      this->define_property ("AvailableProtocols",
                             AvailableProtocols_property,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
      AVStreams::protocolSpec *temp_spec;
      CORBA::Any_var temp_any = this->get_property_value ("AvailableProtocols",
                                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;
      temp_any.in () >>= temp_spec;
      ACE_DEBUG ((LM_DEBUG,"%N:%l\n"));
      for (i=0;i<temp_spec->length ();i++)
        {
          const char *protocol = (*temp_spec)[i].in ();
          ACE_DEBUG ((LM_DEBUG,"%s\n",protocol));
        }
      this->protocols_ = protocols;
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
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::formatMismatch,
                   AVStreams::deviceQosMismatch))
{
  ACE_TRY
    {
      // check whether the passed flowendpoint is compatible with this flowendpoint.
      // should we check for the availableFormats and choose one format.
      // get my format value
      CORBA::Any_var format_ptr;
      CORBA::String_var my_format,peer_format;
      CORBA::String temp_format;
      format_ptr = this->get_property_value ("Format",
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
      format_ptr.in () >>= temp_format;
      my_format = CORBA::string_dup (temp_format);
      // get my peer's format value
      format_ptr = peer_fep->get_property_value ("Format",
                                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;
      format_ptr.in () >>= temp_format;
      peer_format = CORBA::string_dup (temp_format);
      if (ACE_OS::strcmp (my_format.in (),
                          peer_format.in ()) != 0)
        return 0;

      // since formats are same, check for a common protocol
      CORBA::Any_var AvailableProtocols_ptr;
      AVStreams::protocolSpec my_protocol_spec,peer_protocol_spec;
      AVStreams::protocolSpec_ptr temp_protocols;;
      AvailableProtocols_ptr = this->get_property_value ("AvailableProtocols",
                                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      AvailableProtocols_ptr.in () >>= temp_protocols;
      my_protocol_spec = *temp_protocols;
      AvailableProtocols_ptr = peer_fep->get_property_value ("AvailableProtocols",
                                                             ACE_TRY_ENV);
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
              if (ACE_OS::strcmp (my_protocol_string,peer_protocol_string) == 0)
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
TAO_FlowEndPoint::set_peer (AVStreams::FlowConnection_ptr /* the_fc */,
                            AVStreams::FlowEndPoint_ptr the_peer_fep,
                            AVStreams::QoS & /* the_qos */,
                            CORBA::Environment &/* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::QoSRequestFailed,
                   AVStreams::streamOpFailed))
{
  this->peer_fep_ = the_peer_fep;
  return 1;
}

CORBA::Boolean
TAO_FlowEndPoint::set_Mcast_peer (AVStreams::FlowConnection_ptr /* the_fc */,
                                  AVStreams::MCastConfigIf_ptr mcast_peer,
                                  AVStreams::QoS & /* the_qos */,
                                  CORBA::Environment &/* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::QoSRequestFailed))
{
  this->mcast_peer_ = AVStreams::MCastConfigIf::_duplicate (mcast_peer);
  return 0;
}

char *
TAO_FlowEndPoint::go_to_listen (AVStreams::QoS & the_qos,
                                CORBA::Boolean is_mcast,
                                AVStreams::FlowProducer_ptr peer_fep,
                                char *& flowProtocol,
                                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::failedToListen,
                   AVStreams::FPError,
                   AVStreams::QoSRequestFailed))
{
  AVStreams::protocolSpec my_protocol_spec,peer_protocol_spec;
  AVStreams::protocolSpec_ptr temp_protocols;
  CORBA::Any_var AvailableProtocols_ptr =
    peer_fep->get_property_value ("AvailableProtocols",
                                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  AvailableProtocols_ptr.in () >>= temp_protocols;
  peer_protocol_spec = *temp_protocols;
  AvailableProtocols_ptr =
    this->get_property_value ("AvailableProtocols",
                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  AvailableProtocols_ptr.in () >>= temp_protocols;
  my_protocol_spec = *temp_protocols;
  int protocol_match = 0;
  CORBA::String_var listen_protocol;
  for (u_int i=0;i<my_protocol_spec.length ();i++)
    {
      CORBA::String_var my_protocol_string;
      for (u_int j=0;j<peer_protocol_spec.length ();j++)
        {
          CORBA::String_var peer_protocol_string;
          my_protocol_string = CORBA::string_dup (my_protocol_spec[i]);
          peer_protocol_string = CORBA::string_dup (peer_protocol_spec[j]);
          if (ACE_OS::strcmp (my_protocol_string,peer_protocol_string) == 0)
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
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_FlowEndPoint::go_to_listen failed: no protoocol match\n"),0);

  for (u_int j=0;j<this->protocol_addresses_.length ();j++)
    if (ACE_OS::strncmp (this->protocol_addresses_ [i],listen_protocol,ACE_OS::strlen (listen_protocol)) == 0)
      {
        // Now listen on that protocol.
        TAO_Forward_FlowSpec_Entry *entry;
        ACE_NEW_RETURN (entry,
                        TAO_Forward_FlowSpec_Entry (this->flowname_.in (),
                                                    CORBA::string_dup (""),
                                                    this->format_.in (),
                                                    flowProtocol,
                                                    this->protocol_addresses_ [i]),
                        0);

        TAO_AV_Acceptor_Registry *acceptor_registry = TAO_AV_CORE::instance ()->acceptor_registry ();
        TAO_AV_FlowSpecSet flow_spec_set;
        flow_spec_set.insert (entry);
        int result = acceptor_registry->open (this,
                                              TAO_AV_CORE::instance (),
                                              flow_spec_set);
        if (result < 0)
          return 0;
        char *listen_address = entry->get_local_addr_str ();
        char *address;
        ACE_NEW_RETURN (address,
                        char [BUFSIZ],
                        0);
        ACE_OS::sprintf (address,"%s=%s",listen_protocol.in (),listen_address);
        return address;
      }
  return 0;
}


CORBA::Boolean
TAO_FlowEndPoint::connect_to_peer (AVStreams::QoS & the_qos,
                                   const char * address,
                                   const char * use_flow_protocol,
                                   CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::failedToConnect,
                   AVStreams::FPError,
                   AVStreams::QoSRequestFailed))
{
  TAO_Forward_FlowSpec_Entry *entry;
  ACE_NEW_RETURN (entry,
                  TAO_Forward_FlowSpec_Entry (this->flowname_.in (),
                                              CORBA::string_dup (""),
                                              this->format_.in (),
                                              CORBA::string_dup (""),
                                              address),
                  0);
  TAO_AV_FlowSpecSet flow_spec_set;
  flow_spec_set.insert (entry);
  TAO_AV_Connector_Registry *connector_registry = TAO_AV_CORE::instance ()->connector_registry ();
  int result = connector_registry->open (this,
                                         TAO_AV_CORE::instance (),
                                         flow_spec_set);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_FlowEndPoint::connector_registry::open failed\n"),0);
  return 1;
}

int
TAO_FlowEndPoint::set_protocol_object (const char *flowname,
                                       TAO_AV_Protocol_Object *object)
{
  ACE_DEBUG ((LM_DEBUG,"TAO_FlowEndPoint::set_protocol_object\n"));
  this->protocol_object_ = object;
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
  this->open (flowname,protocols,format);
}

//  multicast is currently not supported
char *
TAO_FlowProducer::connect_mcast (AVStreams::QoS & /* the_qos */,
                                 CORBA::Boolean_out /* is_met */,
                                 const char *address,
                                 const char * use_flow_protocol,
                                 CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::failedToConnect,
                   AVStreams::notSupported,
                   AVStreams::FPError,
                   AVStreams::QoSRequestFailed))
{
  // Depending on our available protocols we can create one 
  // of the multicast protocol objects. Right now we'll create 
  // only UDP multicast addresses.
  // The address variable gives the multicast address to subscribe to.
  for (u_int i=0;i<this->protocols_.length ();i++)
    {
      // choose the protocol which supports multicast.
    }
  char full_address [BUFSIZ];
  ACE_OS::sprintf (full_address,"%s=%s","UDP",address);
  TAO_Forward_FlowSpec_Entry  entry (this->flowname_.in (),
                                     "",
                                     this->format_.in (),
                                     use_flow_protocol,
                                     full_address);
  TAO_AV_FlowSpecSet mcast_set;
  mcast_set.insert (&entry);
  TAO_AV_Acceptor_Registry *acceptor_registry =
    TAO_AV_CORE::instance ()->acceptor_registry ();
  int result = acceptor_registry->open (this,
                                        TAO_AV_CORE::instance (),
                                        mcast_set);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_FlowProducer::connect_mcast:acceptor_registry open failed\n"),0);
  return CORBA::string_dup (full_address);
}

// gets the reverse channel for feedback.
char *
TAO_FlowProducer::get_rev_channel (const char * /* pcol_name */,
                                   CORBA::Environment &/* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

// sets the key for this flow.
void
TAO_FlowProducer::set_key (const AVStreams::key & the_key,
                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
                                 CORBA::Environment &/* ACE_TRY_ENV */)
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
  this->open (flowname,protocols,format);
}

// ------------------------------------------------------------
// TAO_FDev
// ------------------------------------------------------------

// default constructor
TAO_FDev::TAO_FDev (void)
{
}

TAO_FDev::TAO_FDev (const char *flowname)
  :flowname_ (flowname)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::Any flowname_any;
      flowname_any <<= flowname;
      this->define_property ("Flow",
                             flowname_any,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FDev::TAO_FDev");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

AVStreams::FlowProducer_ptr
TAO_FDev::create_producer (AVStreams::FlowConnection_ptr the_requester,
                           AVStreams::QoS & the_qos,
                           CORBA::Boolean_out met_qos,
                           char *& named_fdev,
                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::streamOpDenied,
                   AVStreams::notSupported,
                   AVStreams::QoSRequestFailed))
{
  // call the bridge method.
  return this->make_producer (the_requester,
                              the_qos,
                              met_qos,
                              named_fdev,
                              ACE_TRY_ENV);
}

// hook for the applications to override the creation process.
AVStreams::FlowProducer_ptr
TAO_FDev::make_producer (AVStreams::FlowConnection_ptr /* the_requester */,
                         AVStreams::QoS & /* the_qos */,
                         CORBA::Boolean_out /* met_qos */,
                         char *& /* named_fdev */,
                         CORBA::Environment &ACE_TRY_ENV)
{
  AVStreams::FlowProducer_ptr producer = AVStreams::FlowProducer::_nil ();
  ACE_TRY
    {
      // Activate the producer implementation under a POA.
      TAO_FlowProducer *producer_i;
      ACE_NEW_RETURN (producer_i,
                      TAO_FlowProducer,
                      0);
      this->producer_list_.insert_tail (producer_i);
      producer = producer_i->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FDev::make_producer");
      return producer;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (producer);
  return producer;
}

// hook for the applications to override the consumer creation.
AVStreams::FlowConsumer_ptr
TAO_FDev::make_consumer (AVStreams::FlowConnection_ptr /* the_requester */,
                         AVStreams::QoS & /* the_qos */,
                         CORBA::Boolean_out /* met_qos */,
                         char *& /* named_fdev */,
                         CORBA::Environment &ACE_TRY_ENV)
{
  AVStreams::FlowConsumer_ptr consumer = AVStreams::FlowConsumer::_nil ();
  ACE_TRY
    {
      // Activate the consumer implementation under a POA.
      TAO_FlowConsumer *consumer_i;
      ACE_NEW_RETURN (consumer_i,
                      TAO_FlowConsumer,
                      0);
      this->consumer_list_.insert_tail (consumer_i);
      consumer = consumer_i->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_FDev::make_consumer");
      return consumer;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (consumer);
  return consumer;
}

AVStreams::FlowConsumer_ptr
TAO_FDev::create_consumer (AVStreams::FlowConnection_ptr the_requester,
                           AVStreams::QoS & the_qos,
                           CORBA::Boolean_out met_qos,
                           char *& named_fdev,
                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::streamOpDenied,
                   AVStreams::notSupported,
                   AVStreams::QoSRequestFailed))
{
  return this->make_consumer (the_requester,
                              the_qos,
                              met_qos,
                              named_fdev,
                              ACE_TRY_ENV);
}

// not implemented yet.
AVStreams::FlowConnection_ptr
TAO_FDev::bind (AVStreams::FDev_ptr peer_device,
                AVStreams::QoS & the_qos,
                CORBA::Boolean_out is_met,
                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::QoSRequestFailed))
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
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::streamOpFailed,
                   AVStreams::QoSRequestFailed))
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
  ACE_THROW_SPEC ((CORBA::SystemException,
                   AVStreams::notSupported))
{
  // @@ Shouldn't the parameters be made use of!
  // Destroy/delete all the producers and consumers.
  PRODUCER_LIST_ITERATOR producer_list_iterator (this->producer_list_);
  TAO_FlowProducer *producer_i = 0;

  while ((producer_i = producer_list_iterator.next ()) != 0)
    {
      deactivate_servant (producer_i);
      delete producer_i;
    }

  CONSUMER_LIST_ITERATOR consumer_list_iterator (this->consumer_list_);
  TAO_FlowConsumer *consumer_i = 0;

  while ((consumer_i = consumer_list_iterator.next ()) != 0)
    {
      deactivate_servant (producer_i);
      delete consumer_i;
    }
  int result = deactivate_servant (this);
  if (result < 0)
    ACE_DEBUG ((LM_DEBUG,"TAO_StreamEndPoint::destroy failed\n"));
}

//------------------------------------------------------------
// TAO_Tokenizer
//------------------------------------------------------------
TAO_Tokenizer::TAO_Tokenizer (const char *string, char delimiter)
  :token_array_ (10),
   count_ (0)
{
  this->parse (string,delimiter);
}

TAO_Tokenizer::~TAO_Tokenizer ()
{
//    ACE_Array_Iterator<char*> iterator (this->token_array_);
//    char **entry = 0;
//    for (; iterator.next (entry) != 0; iterator.advance ())
//      {
//        ACE_DEBUG ((LM_DEBUG,"%s\n",*entry));
//        CORBA::string_free (*entry);
//      }
}


int
TAO_Tokenizer::parse (const char *string,char delimiter)
{
  ACE_CString new_string (string);
  int pos =0, slash_pos = 0;
  int count=0,result;
  while (pos < new_string.length ())
    {
      slash_pos = new_string.find (delimiter,pos);
      ACE_CString substring;
      if (slash_pos != new_string.npos)
        {
          substring = new_string.substring (pos,slash_pos);
          pos += slash_pos+1;
        }
      else
        {
          substring = new_string.substring (pos);
          pos = new_string.length ();
        }
      char *token = CORBA::string_dup (substring.c_str ());
      result = this->token_array_.set (token,count);
      if (result == -1)
        {
          this->token_array_.size (this->token_array_.size ()*2);
          result = this->token_array_.set (token,count);
          if (result == -1)
            ACE_ERROR_RETURN ((LM_ERROR,"TAO_Tokenizer::parse error"),-1);
        }
      count++;
    }

  /*
  ACE_OS::strcpy (this->string_ , string);
  char delimiter_str [2] = {0,0};
  delimiter_str [0] = delimiter;
  char *token = ACE_OS::strtok (this->string_,delimiter_str);

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
      token = ACE_OS::strtok (0,delimiter_str);
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

char *
TAO_Tokenizer::operator [] (size_t index) const
{
  if (index >= this->num_tokens_)
    return 0;
  return CORBA::string_dup (this->token_array_[index]);
}

//------------------------------------------------------------
// TAO_FlowSpec_Entry
//------------------------------------------------------------

// constructor.
TAO_FlowSpec_Entry::TAO_FlowSpec_Entry (void)
  :address_ (0),
   address_str_ (CORBA::string_dup ("")),
   format_ (CORBA::string_dup ("")),
   direction_ (TAO_AV_INVALID),
   protocol_ (TAO_AV_Core::TAO_AV_NOPROTOCOL),
   carrier_protocol_ (CORBA::string_dup ("")),
   flow_protocol_ (CORBA::string_dup ("")),
   use_flow_protocol_ (0),
   entry_ (0),
   peer_addr_ (0),
   local_addr_ (0),
   transport_ (0),
   handler_ (0),
   protocol_object_ (0)
{
}

// constructor.
TAO_FlowSpec_Entry::TAO_FlowSpec_Entry (const char *flowname,
                                        const char *direction,
                                        const char *format_name,
                                        const char *flow_protocol,
                                        const char *carrier_protocol,
                                        ACE_Addr *address)
  :address_ (address),
   address_str_ (0),
   format_ (CORBA::string_dup (format_name)),
   direction_str_ (CORBA::string_dup (direction)),
   flowname_ (CORBA::string_dup (flowname)),
   carrier_protocol_ (CORBA::string_dup (carrier_protocol)),
   flow_protocol_ (CORBA::string_dup (flow_protocol)),
   use_flow_protocol_ (0),
   entry_ (0),
   peer_addr_ (0),
   local_addr_ (0),
   transport_ (0),
   handler_ (0),
   protocol_object_ (0)
{
  this->set_protocol ();
  this->set_direction (this->direction_str_);
}

TAO_FlowSpec_Entry::TAO_FlowSpec_Entry (const char *flowname,
                                        const char *direction,
                                        const char *format_name,
                                        const char *flow_protocol,
                                        const char *address)
  :address_ (0),
   address_str_ (CORBA::string_dup (address)),
   format_ (CORBA::string_dup (format_name)),
   direction_str_ (CORBA::string_dup (direction)),
   flowname_ (CORBA::string_dup (flowname)),
   carrier_protocol_ (0),
   flow_protocol_ (CORBA::string_dup (flow_protocol)),
   use_flow_protocol_ (0),
   entry_ (0),
   peer_addr_ (0),
   local_addr_ (0),
   transport_ (0),
   handler_ (0),
   protocol_object_ (0)
{
  ACE_CString cstring(this->address_str_,0,0);
  int colon_pos = cstring.find (':');
  if (colon_pos != cstring.npos)
    cstring[colon_pos] = ';';
  this->address_str_ = cstring.rep ();
  this->parse_address (this->address_str_);
}

// Destructor.
TAO_FlowSpec_Entry::~TAO_FlowSpec_Entry (void)
{
  CORBA::string_free (this->format_);
  CORBA::string_free (this->direction_str_);
  CORBA::string_free (this->flowname_);
  CORBA::string_free (this->carrier_protocol_);
}

int
TAO_FlowSpec_Entry::direction (void)
{
  return this->direction_;
}

char *
TAO_FlowSpec_Entry::direction_str (void)
{
  return this->direction_str_;
}

char *
TAO_FlowSpec_Entry::flow_protocol_str (void)
{
  return this->flow_protocol_;
}

TAO_AV_Core::Protocol
TAO_FlowSpec_Entry::carrier_protocol (void)
{
  return this->protocol_;
}

char *
TAO_FlowSpec_Entry::carrier_protocol_str (void)
{
  return this->carrier_protocol_;
}

ACE_Addr *
TAO_FlowSpec_Entry::address (void)
{
  return this->address_;
}

char *
TAO_FlowSpec_Entry::address_str (void)
{
  return this->address_str_;
}

char*
TAO_FlowSpec_Entry::format (void)
{
  return this->format_;
}

const char *
TAO_FlowSpec_Entry::flowname (void)
{
  return this->flowname_;
}
int
TAO_FlowSpec_Entry::set_direction (char *direction)
{
  this->direction_str_ = direction;
  if (direction == 0)
    {
      this->direction_ = TAO_AV_INVALID;
      return -1;
    }
  if (ACE_OS::strcasecmp (direction,"in") == 0)
    this->direction_ = TAO_AV_DIR_IN;
  else if (ACE_OS::strcasecmp (direction,"out") == 0)
    this->direction_ = TAO_AV_DIR_OUT;
  return 0;
}

int
TAO_FlowSpec_Entry::parse_flow_protocol_string (char *flow_string)
{
  if (flow_string == 0)
    return 0;
  if (ACE_OS::strncasecmp (flow_string,"sfp",3) == 0)
    {
      this->use_flow_protocol_ = 1;
      // do some flow protocol processing.
    }
  return 0;
}

int
TAO_FlowSpec_Entry::set_protocol (void)
{
  if (!this->use_flow_protocol_)
    {
      if (ACE_OS::strcmp (this->carrier_protocol_,"TCP") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_TCP;
      else if (ACE_OS::strcmp (this->carrier_protocol_,"UDP") == 0)
        {
          this->protocol_ = TAO_AV_Core::TAO_AV_UDP;
          if (this->address_ != 0)
            {
              ACE_INET_Addr *inet_addr = ACE_dynamic_cast (ACE_INET_Addr*,this->address_);
              char buf[BUFSIZ];
              inet_addr->addr_to_string (buf,BUFSIZ);
              ACE_DEBUG ((LM_DEBUG,"TAO_FlowSpec_Entry::set_protocol:%s\n",buf));
              if (IN_CLASSD (inet_addr->get_ip_address ()))
                this->protocol_ = TAO_AV_Core::TAO_AV_UDP_MCAST;
            }
        }
      else if (ACE_OS::strcmp (this->carrier_protocol_,"AAL5") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_AAL5;
      else if (ACE_OS::strcmp (this->carrier_protocol_,"AAL3_4") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_AAL3_4;
      else if (ACE_OS::strcmp (this->carrier_protocol_,"AAL1") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_AAL1;
      else if (ACE_OS::strcmp (this->carrier_protocol_,"RTP/UDP") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_RTP_UDP;
      else if (ACE_OS::strcmp (this->carrier_protocol_,"RTP/AAL5") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_RTP_AAL5;
      else if (ACE_OS::strcmp (this->carrier_protocol_,"IPX") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_IPX;
      else
        {
          this->protocol_ = TAO_AV_Core::TAO_AV_NOPROTOCOL;
          return -1;
        }
    }
  else
    {
      if (ACE_OS::strcmp (this->carrier_protocol_,"UDP") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_SFP_UDP;
      else
        {
          this->protocol_ = TAO_AV_Core::TAO_AV_NOPROTOCOL;
          return -1;
        }
    }
  return 0;
}

int
TAO_FlowSpec_Entry::parse_address (char *address)
{
  if (address == 0)
    return 0;
  if (ACE_OS::strcmp (address,"") == 0)
    return 0;
  TAO_Tokenizer protocol_tokenizer (address,'=');
  this->carrier_protocol_ = protocol_tokenizer[0];

  int result = this->set_protocol ();
  if (result < 0)
    return result;

  if (protocol_tokenizer [1] != 0)
    {
      TAO_Tokenizer address_tokenizer (protocol_tokenizer[1],';');
      char addr[BUFSIZ];
      // convert to the ACE_Addr format.
      ACE_OS::sprintf (addr,"%s:%s",address_tokenizer[0],address_tokenizer[1]);
      switch (this->protocol_)
        {
        case TAO_AV_Core::TAO_AV_SFP_UDP:
        case TAO_AV_Core::TAO_AV_RTP_UDP:
        case TAO_AV_Core::TAO_AV_TCP:
          {
            this->address_str_ = CORBA::string_dup (addr);
            ACE_NEW_RETURN (this->address_,
                            ACE_INET_Addr (addr),
                            -1);
          }
          break;
        case TAO_AV_Core::TAO_AV_UDP:
          {
            this->address_str_ = CORBA::string_dup (addr);
            ACE_INET_Addr *inet_addr;
            ACE_NEW_RETURN (inet_addr,
                            ACE_INET_Addr (addr),
                            -1);
            this->address_ = inet_addr;
            if (IN_CLASSD (inet_addr->get_ip_address ()))
              this->protocol_ = TAO_AV_Core::TAO_AV_UDP_MCAST;
          }
          break;
        default:
          ACE_DEBUG ((LM_DEBUG,"ATM support not added yet\n"));
        }
    }
  return 0;
}

int
TAO_FlowSpec_Entry::set_peer_addr (ACE_Addr *peer_addr)
{
  this->peer_addr_ = peer_addr;
  if (this->handler_ != 0)
    this->handler_->set_remote_address (peer_addr);
  return 0;
}

ACE_Addr *
TAO_FlowSpec_Entry::get_peer_addr (void)
{
  return this->peer_addr_;
}

int
TAO_FlowSpec_Entry::set_local_addr (ACE_Addr *local_addr)
{
  this->local_addr_ = local_addr;
  return 0;
}

ACE_Addr*
TAO_FlowSpec_Entry::get_local_addr (void)
{
  return this->local_addr_;
}

char *
TAO_FlowSpec_Entry::get_local_addr_str (void)
{
  switch (this->local_addr_->get_type ())
    {
    case AF_INET:
      {
        char *buf;
        ACE_NEW_RETURN (buf,
                        char [BUFSIZ],
                        0);
        ACE_INET_Addr *inet_addr = ACE_dynamic_cast (ACE_INET_Addr *,this->local_addr_);
        inet_addr->addr_to_string (buf,BUFSIZ);
        ACE_CString cstring (buf,0,0);
        int colon_pos = cstring.find (':');
        if (colon_pos != cstring.npos)
          cstring[colon_pos] = ';';
        return cstring.rep ();
      }
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,"Address family not supported"),0);
    }
}

TAO_AV_Transport*
TAO_FlowSpec_Entry::transport (void)
{
  return this->transport_;
}

void
TAO_FlowSpec_Entry::transport (TAO_AV_Transport *transport)
{
  this->transport_ = transport;
}

TAO_AV_Flow_Handler *
TAO_FlowSpec_Entry::handler (void)
{
  return this->handler_;
}

void
TAO_FlowSpec_Entry::handler (TAO_AV_Flow_Handler *handler)
{
  this->handler_ = handler;
}

TAO_AV_Protocol_Object*
TAO_FlowSpec_Entry::protocol_object (void)
{
  return this->protocol_object_;
}

void
TAO_FlowSpec_Entry::protocol_object (TAO_AV_Protocol_Object *object)
{
  this->protocol_object_ = object;
}

//------------------------------------------------------------
// TAO_Forward_FlowSpec_Entry
//------------------------------------------------------------

// default constructor.
TAO_Forward_FlowSpec_Entry::TAO_Forward_FlowSpec_Entry (void)
{
}

// constructor to construct the entry from the arguments.
TAO_Forward_FlowSpec_Entry::TAO_Forward_FlowSpec_Entry (const char *flowname,
                                                        const char *direction,
                                                        const char *format_name ,
                                                        const char *flow_protocol ,
                                                        const char *carrier_protocol ,
                                                        ACE_Addr *address )
  :TAO_FlowSpec_Entry (flowname,
                       direction,
                       format_name,
                       flow_protocol,
                       carrier_protocol,
                       address)
{
}

// constructor to construct the entry from the arguments.
TAO_Forward_FlowSpec_Entry::TAO_Forward_FlowSpec_Entry (const char *flowname,
                                                        const char *direction,
                                                        const char *format_name ,
                                                        const char *flow_protocol ,
                                                        const char *address )
  :TAO_FlowSpec_Entry (flowname,
                       direction,
                       format_name,
                       flow_protocol,
                       address)
{
}

int
TAO_Forward_FlowSpec_Entry::parse (const char *flowSpec_entry)
{
  TAO_Tokenizer tokenizer (flowSpec_entry,'\\');

  this->flowname_ = tokenizer [TAO_AV_FLOWNAME];

  if (tokenizer [TAO_AV_DIRECTION] != 0)
    this->set_direction (tokenizer [TAO_AV_DIRECTION]);

  if (tokenizer [TAO_AV_FORMAT] != 0)
    this->format_ = tokenizer [TAO_AV_FORMAT];

  if (tokenizer [TAO_AV_FLOW_PROTOCOL] != 0)
    if (this->parse_flow_protocol_string (tokenizer [TAO_AV_FLOW_PROTOCOL]) < 0)
      return -1;

  if (tokenizer [TAO_AV_ADDRESS] != 0)
    if (this->parse_address (tokenizer [TAO_AV_ADDRESS]) < 0)
      return -1;

  return 0;
}

char *
TAO_Forward_FlowSpec_Entry::entry_to_string (void)
{
  if (this->flowname_ == 0)
    return 0;

  ACE_NEW_RETURN (this->entry_,
                  char [BUFSIZ],
                  0);

  char address [BUFSIZ];
  char address_str [BUFSIZ];
  if (this->address_ != 0)
    {
      switch (this->protocol_)
        {
        case TAO_AV_Core::TAO_AV_SFP_UDP:
        case TAO_AV_Core::TAO_AV_RTP_UDP:
        case TAO_AV_Core::TAO_AV_UDP:
        case TAO_AV_Core::TAO_AV_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_TCP:
          {
            ACE_INET_Addr *inet_addr = ACE_dynamic_cast (ACE_INET_Addr*,this->address_);
            inet_addr->addr_to_string (address,BUFSIZ);
          }
          break;
        default:
          break;
        }
      ACE_CString cstring (address);
      int colon_pos = cstring.find (':');
      if (colon_pos != cstring.npos)
        cstring[colon_pos] = ';';


      ACE_OS::sprintf (address_str,"%s=%s",
                       this->carrier_protocol_,
                       cstring.c_str ());

    }
  else
    ACE_OS::strcpy (address_str,"");
  ACE_OS::sprintf (this->entry_,
                   "%s\\%s\\%s\\%s\\%s",
                   this->flowname_,
                   this->direction_str_,
                   this->format_,
                   this->flow_protocol_,
                   address_str);

  ACE_DEBUG ((LM_DEBUG,"entry_to_string: entry = %s\n",this->entry_));
  return this->entry_;
}

//------------------------------------------------------------
// TAO_Reverse_FlowSpec_Entry
//------------------------------------------------------------

//default constructor.
TAO_Reverse_FlowSpec_Entry::TAO_Reverse_FlowSpec_Entry (void)
{
}

// constructor to construct an entry from the arguments.
TAO_Reverse_FlowSpec_Entry::TAO_Reverse_FlowSpec_Entry (const char *flowname,
                                                        const char *direction,
                                                        const char *format_name ,
                                                        const char *flow_protocol ,
                                                        const char *carrier_protocol,
                                                        ACE_Addr *address)
  :TAO_FlowSpec_Entry (flowname,
                       direction,
                       format_name,
                       flow_protocol,
                       carrier_protocol,
                       address)
{
}

// constructor to construct an entry from the arguments.
TAO_Reverse_FlowSpec_Entry::TAO_Reverse_FlowSpec_Entry (const char *flowname,
                                                        const char *direction,
                                                        const char *format_name ,
                                                        const char *flow_protocol ,
                                                        const char *address)
  :TAO_FlowSpec_Entry (flowname,
                       direction,
                       format_name,
                       flow_protocol,
                       address)
{
}

int
TAO_Reverse_FlowSpec_Entry::parse (const char *flowSpec_entry)
{
  TAO_Tokenizer tokenizer (flowSpec_entry,'\\');
  this->flowname_ = tokenizer [TAO_AV_FLOWNAME];

  if (tokenizer [TAO_AV_FLOW_PROTOCOL] != 0)
    if (this->parse_flow_protocol_string (tokenizer [TAO_AV_FLOW_PROTOCOL]) < 0)
      return -1;

  if (tokenizer [TAO_AV_ADDRESS] != 0)
    if (this->parse_address (tokenizer [TAO_AV_ADDRESS]) < 0)
      return -1;

  if (tokenizer [TAO_AV_DIRECTION] != 0)
    this->set_direction (tokenizer [TAO_AV_DIRECTION]);

  if (tokenizer [TAO_AV_FORMAT] != 0)
    this->format_ = tokenizer [TAO_AV_FORMAT];

  return 0;
}


char *
TAO_Reverse_FlowSpec_Entry::entry_to_string (void)
{
  if (this->flowname_ == 0)
    return 0;

  ACE_NEW_RETURN (this->entry_,
                  char [BUFSIZ],
                  0);


  char address [BUFSIZ];
  char address_str [BUFSIZ];
  if (this->address_ != 0)
    {

      switch (this->protocol_)
        {
        case TAO_AV_Core::TAO_AV_RTP_UDP:
        case TAO_AV_Core::TAO_AV_UDP:
        case TAO_AV_Core::TAO_AV_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_TCP:
        case TAO_AV_Core::TAO_AV_SFP_UDP:
          {
            ACE_INET_Addr *inet_addr = ACE_dynamic_cast (ACE_INET_Addr*,this->address_);
            inet_addr->addr_to_string (address,BUFSIZ);
          }
          break;
        default:
          break;
        }
      ACE_CString cstring (address);
      int colon_pos = cstring.find (':');
      if (colon_pos != cstring.npos)
        cstring[colon_pos] = ';';

      ACE_OS::sprintf (address_str,"%s=%s",
                       this->carrier_protocol_,
                       cstring.c_str ());

    }
  else
    ACE_OS::strcpy (address_str,"");
  ACE_OS::sprintf (this->entry_,
                   "%s\\%s\\%s\\%s\\%s",
                   this->flowname_,
                   address_str,
                   this->flow_protocol_,
                   this->direction_str_,
                   this->format_);
  ACE_DEBUG ((LM_DEBUG,"entry_to_string: entry = %s\n",this->entry_));
  return this->entry_;
}

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
TAO_AV_QoS::set (AVStreams::streamQoS &stream_qos)
{
  this->stream_qos_ = stream_qos;

  for (u_int j=0;j<this->stream_qos_.length ();j++)
    {
      TAO_String_Hash_Key qos_key (CORBA::string_dup (this->stream_qos_[j].QoSType));
      int result = this->qos_map_.bind (qos_key,this->stream_qos_[j]);
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,"qos_map::bind failed\n"),-1);
    }
  return 0;
}

int
TAO_AV_QoS::get_flow_qos (const char *flowname,AVStreams::QoS &flow_qos)
{
  int result = this->qos_map_.find (flowname, flow_qos);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_DEBUG,"qos_map::find failed\n"),-1);
  return 0;
}

int
TAO_AV_QoS::convert (AVStreams::streamQoS &network_qos)
{
  return -1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<TAO_String_Hash_Key,AVStreams::FDev_ptr>;
template class ACE_Hash_Map_Manager<TAO_String_Hash_Key,AVStreams::FDev_ptr,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, AVStreams::FDev_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_String_Hash_Key,AVStreams::FDev_ptr,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, AVStreams::FDev_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, AVStreams::FDev_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,AVStreams::FDev_ptr,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, AVStreams::FDev_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<TAO_String_Hash_Key,AVStreams::FlowConnection_ptr>;
template class ACE_Hash_Map_Manager<TAO_String_Hash_Key,AVStreams::FlowConnection_ptr,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, AVStreams::FlowConnection_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_String_Hash_Key,AVStreams::FlowConnection_ptr,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, AVStreams::FlowConnection_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, AVStreams::FlowConnection_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,AVStreams::FlowConnection_ptr,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, AVStreams::FlowConnection_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<TAO_String_Hash_Key,AVStreams::FlowEndPoint_ptr>;
template class ACE_Hash_Map_Manager<TAO_String_Hash_Key,AVStreams::FlowEndPoint_ptr,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, AVStreams::FlowEndPoint_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_String_Hash_Key,AVStreams::FlowEndPoint_ptr,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, AVStreams::FlowEndPoint_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, AVStreams::FlowEndPoint_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,AVStreams::FlowEndPoint_ptr,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, AVStreams::FlowEndPoint_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<MMDevice_Map_Hash_Key,TAO_StreamCtrl::MMDevice_Map_Entry>;
template class ACE_Hash_Map_Manager<MMDevice_Map_Hash_Key,TAO_StreamCtrl::MMDevice_Map_Entry,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<MMDevice_Map_Hash_Key, TAO_StreamCtrl::MMDevice_Map_Entry, ACE_Hash<MMDevice_Map_Hash_Key>, ACE_Equal_To<MMDevice_Map_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<MMDevice_Map_Hash_Key,TAO_StreamCtrl::MMDevice_Map_Entry,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<MMDevice_Map_Hash_Key, TAO_StreamCtrl::MMDevice_Map_Entry, ACE_Hash<MMDevice_Map_Hash_Key>, ACE_Equal_To<MMDevice_Map_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<MMDevice_Map_Hash_Key, TAO_StreamCtrl::MMDevice_Map_Entry, ACE_Hash<MMDevice_Map_Hash_Key>, ACE_Equal_To<MMDevice_Map_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<MMDevice_Map_Hash_Key,TAO_StreamCtrl::MMDevice_Map_Entry,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<MMDevice_Map_Hash_Key, TAO_StreamCtrl::MMDevice_Map_Entry, ACE_Hash<MMDevice_Map_Hash_Key>, ACE_Equal_To<MMDevice_Map_Hash_Key>, ACE_Null_Mutex>;

// template class ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_AV_Dgram_Flow_Handler*>;
// template class ACE_Hash_Map_Manager<TAO_String_Hash_Key,TAO_AV_Dgram_Flow_Handler*,ACE_Null_Mutex>;
// template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_AV_Dgram_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
// template class ACE_Hash_Map_Iterator<TAO_String_Hash_Key,TAO_AV_Dgram_Flow_Handler*,ACE_Null_Mutex>;
// template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_Dgram_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
// template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_AV_Dgram_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
// template class ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,TAO_AV_Dgram_Flow_Handler*,ACE_Null_Mutex>;
// template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_Dgram_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_AV_Acceptor*>;
template class ACE_Hash_Map_Manager<TAO_String_Hash_Key,TAO_AV_Acceptor*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_AV_Acceptor*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_String_Hash_Key,TAO_AV_Acceptor*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_Acceptor*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_AV_Acceptor*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,TAO_AV_Acceptor*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_Acceptor*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;

// template class ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_AV_Connector*>;
// template class ACE_Hash_Map_Manager<TAO_String_Hash_Key,TAO_AV_Connector*,ACE_Null_Mutex>;
// template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_AV_Connector*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
// template class ACE_Hash_Map_Iterator<TAO_String_Hash_Key,TAO_AV_Connector*,ACE_Null_Mutex>;
// template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_Connector*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
// template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_AV_Connector*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
// template class ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,TAO_AV_Connector*,ACE_Null_Mutex>;
// template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_Connector*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_AV_TCP_Flow_Handler*>;
template class ACE_Hash_Map_Manager<TAO_String_Hash_Key,TAO_AV_TCP_Flow_Handler*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_AV_TCP_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_String_Hash_Key,TAO_AV_TCP_Flow_Handler*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_TCP_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_AV_TCP_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,TAO_AV_TCP_Flow_Handler*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_TCP_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_Forward_FlowSpec_Entry*>;
template class ACE_Hash_Map_Manager<TAO_String_Hash_Key,TAO_Forward_FlowSpec_Entry*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_Forward_FlowSpec_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_String_Hash_Key,TAO_Forward_FlowSpec_Entry*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_Forward_FlowSpec_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_Forward_FlowSpec_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,TAO_Forward_FlowSpec_Entry*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_Forward_FlowSpec_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_Reverse_FlowSpec_Entry*>;
template class ACE_Hash_Map_Manager<TAO_String_Hash_Key,TAO_Reverse_FlowSpec_Entry*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_Reverse_FlowSpec_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_String_Hash_Key,TAO_Reverse_FlowSpec_Entry*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_Reverse_FlowSpec_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_Reverse_FlowSpec_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,TAO_Reverse_FlowSpec_Entry*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_Reverse_FlowSpec_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_AV_UDP_MCast_Flow_Handler*>;
template class ACE_Hash_Map_Manager<TAO_String_Hash_Key,TAO_AV_UDP_MCast_Flow_Handler*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_AV_UDP_MCast_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_String_Hash_Key,TAO_AV_UDP_MCast_Flow_Handler*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_UDP_MCast_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_AV_UDP_MCast_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,TAO_AV_UDP_MCast_Flow_Handler*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_UDP_MCast_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<TAO_String_Hash_Key,AVStreams::QoS>;
template class ACE_Hash_Map_Manager<TAO_String_Hash_Key,AVStreams::QoS,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, AVStreams::QoS, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_String_Hash_Key,AVStreams::QoS,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, AVStreams::QoS, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, AVStreams::QoS, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,AVStreams::QoS,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, AVStreams::QoS, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;

// template class ACE_Acceptor <TAO_AV_TCP_Flow_Handler,ACE_SOCK_ACCEPTOR>;
// template class ACE_Connector <TAO_AV_TCP_Flow_Handler,ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Handler <ACE_SOCK_STREAM,ACE_NULL_SYNCH>;

template class ACE_Array<char*>;
template class ACE_Array_Base<char*>;
template class ACE_DLList<TAO_MCastConfigIf::Peer_Info>;
template class ACE_DLList_Iterator<TAO_MCastConfigIf::Peer_Info>;
template class ACE_DLList <TAO_FlowProducer>;
template class ACE_DLList_Iterator <TAO_FlowProducer>;
template class ACE_DLList <TAO_FlowConsumer>;
template class ACE_DLList_Iterator <TAO_FlowConsumer>;

template class ACE_Node <TAO_FlowSpec_Entry*>;
template class ACE_Unbounded_Set <TAO_FlowSpec_Entry*>;
template class ACE_Unbounded_Set_Iterator <TAO_FlowSpec_Entry*>;
template class ACE_Node <TAO_AV_Protocol_Item*>;
template class ACE_Unbounded_Set<TAO_AV_Protocol_Item*>;
template class ACE_Unbounded_Set_Iterator<TAO_AV_Protocol_Item*>;
//template class ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_AV_TCP_Flow_Handler> *, ACE_RW_Thread_Mutex>;
//template class ACE_Svc_Tuple<TAO_AV_TCP_Flow_Handler>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key,AVStreams::FDev_ptr>
#pragma instantiate ACE_Hash_Map_Manager<TAO_String_Hash_Key,AVStreams::FDev_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, AVStreams::FDev_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_String_Hash_Key,AVStreams::FDev_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, AVStreams::FDev_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, AVStreams::FDev_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,AVStreams::FDev_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, AVStreams::FDev_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key,AVStreams::FlowConnection_ptr>
#pragma instantiate ACE_Hash_Map_Manager<TAO_String_Hash_Key,AVStreams::FlowConnection_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, AVStreams::FlowConnection_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_String_Hash_Key,AVStreams::FlowConnection_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, AVStreams::FlowConnection_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, AVStreams::FlowConnection_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,AVStreams::FlowConnection_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, AVStreams::FlowConnection_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key,AVStreams::FlowEndPoint_ptr>
#pragma instantiate ACE_Hash_Map_Manager<TAO_String_Hash_Key,AVStreams::FlowEndPoint_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, AVStreams::FlowEndPoint_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_String_Hash_Key,AVStreams::FlowEndPoint_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, AVStreams::FlowEndPoint_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, AVStreams::FlowEndPoint_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,AVStreams::FlowEndPoint_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, AVStreams::FlowEndPoint_ptr, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<MMDevice_Map_Hash_Key,TAO_StreamCtrl::MMDevice_Map_Entry>
#pragma instantiate ACE_Hash_Map_Manager<MMDevice_Map_Hash_Key,TAO_StreamCtrl::MMDevice_Map_Entry,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<MMDevice_Map_Hash_Key, TAO_StreamCtrl::MMDevice_Map_Entry, ACE_Hash<MMDevice_Map_Hash_Key>, ACE_Equal_To<MMDevice_Map_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<MMDevice_Map_Hash_Key,TAO_StreamCtrl::MMDevice_Map_Entry,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<MMDevice_Map_Hash_Key, TAO_StreamCtrl::MMDevice_Map_Entry, ACE_Hash<MMDevice_Map_Hash_Key>, ACE_Equal_To<MMDevice_Map_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<MMDevice_Map_Hash_Key, TAO_StreamCtrl::MMDevice_Map_Entry, ACE_Hash<MMDevice_Map_Hash_Key>, ACE_Equal_To<MMDevice_Map_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<MMDevice_Map_Hash_Key,TAO_StreamCtrl::MMDevice_Map_Entry,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<MMDevice_Map_Hash_Key, TAO_StreamCtrl::MMDevice_Map_Entry, ACE_Hash<MMDevice_Map_Hash_Key>, ACE_Equal_To<MMDevice_Map_Hash_Key>, ACE_Null_Mutex>

// #pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_AV_Dgram_Flow_Handler*>
// #pragma instantiate ACE_Hash_Map_Manager<TAO_String_Hash_Key,TAO_AV_Dgram_Flow_Handler*,ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_AV_Dgram_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Iterator<TAO_String_Hash_Key,TAO_AV_Dgram_Flow_Handler*,ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_Dgram_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_AV_Dgram_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,TAO_AV_Dgram_Flow_Handler*,ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_Dgram_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_AV_Acceptor*>
#pragma instantiate ACE_Hash_Map_Manager<TAO_String_Hash_Key,TAO_AV_Acceptor*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_AV_Acceptor*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_String_Hash_Key,TAO_AV_Acceptor*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_Acceptor*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_AV_Acceptor*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,TAO_AV_Acceptor*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_Acceptor*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>

// #pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_AV_Connector*>
// #pragma instantiate ACE_Hash_Map_Manager<TAO_String_Hash_Key,TAO_AV_Connector*,ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_AV_Connector*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Iterator<TAO_String_Hash_Key,TAO_AV_Connector*,ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_Connector*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_AV_Connector*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,TAO_AV_Connector*,ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_Connector*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>

// #pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_AV_TCP_Flow_Handler*>
// #pragma instantiate ACE_Hash_Map_Manager<TAO_String_Hash_Key,TAO_AV_TCP_Flow_Handler*,ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_AV_TCP_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Iterator<TAO_String_Hash_Key,TAO_AV_TCP_Flow_Handler*,ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_TCP_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_AV_TCP_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,TAO_AV_TCP_Flow_Handler*,ACE_Null_Mutex>
// #pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_TCP_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_Forward_FlowSpec_Entry*>
#pragma instantiate ACE_Hash_Map_Manager<TAO_String_Hash_Key,TAO_Forward_FlowSpec_Entry*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_Forward_FlowSpec_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_String_Hash_Key,TAO_Forward_FlowSpec_Entry*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_Forward_FlowSpec_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_Forward_FlowSpec_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,TAO_Forward_FlowSpec_Entry*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_Forward_FlowSpec_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_Reverse_FlowSpec_Entry*>
#pragma instantiate ACE_Hash_Map_Manager<TAO_String_Hash_Key,TAO_Reverse_FlowSpec_Entry*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_Reverse_FlowSpec_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_String_Hash_Key,TAO_Reverse_FlowSpec_Entry*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_Reverse_FlowSpec_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_Reverse_FlowSpec_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,TAO_Reverse_FlowSpec_Entry*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_Reverse_FlowSpec_Entry*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key,TAO_AV_UDP_MCast_Flow_Handler*>
#pragma instantiate ACE_Hash_Map_Manager<TAO_String_Hash_Key,TAO_AV_UDP_MCast_Flow_Handler*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, TAO_AV_UDP_MCast_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_String_Hash_Key,TAO_AV_UDP_MCast_Flow_Handler*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_UDP_MCast_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, TAO_AV_UDP_MCast_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,TAO_AV_UDP_MCast_Flow_Handler*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, TAO_AV_UDP_MCast_Flow_Handler*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<TAO_String_Hash_Key,AVStreams::QoS>
#pragma instantiate ACE_Hash_Map_Manager<TAO_String_Hash_Key,AVStreams::QoS,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, AVStreams::QoS, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_String_Hash_Key,AVStreams::QoS,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, AVStreams::QoS, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, AVStreams::QoS, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key,AVStreams::QoS,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, AVStreams::QoS, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>

#pragma instantiate ACE_Acceptor <TAO_AV_TCP_Flow_Handler,ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Connector <TAO_AV_TCP_Flow_Handler,ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Svc_Handler <ACE_SOCK_STREAM,ACE_NULL_SYNCH>
#pragma instantiate ACE_Array<char*>
#pragma instantiate ACE_Array_Base<char*>
#pragma instantiate ACE_DLList<TAO_MCastConfigIf::Peer_Info>
#pragma instantiate ACE_DLList_Iterator<TAO_MCastConfigIf::Peer_Info>
#pragma instantiate ACE_DLList <TAO_FlowProducer>
#pragma instantiate ACE_DLList_Iterator <TAO_FlowProducer>
#pragma instantiate ACE_DLList <TAO_FlowConsumer>
#pragma instantiate ACE_DLList_Iterator <TAO_FlowConsumer>

#pragma instantiate ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_AV_TCP_Flow_Handler> *, ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Svc_Tuple<TAO_AV_TCP_Flow_Handler>

#pragma instantiate ACE_Node <TAO_FlowSpec_Entry*>
#pragma instantiate ACE_Unbounded_Set <TAO_FlowSpec_Entry*>
#pragma instantiate ACE_Unbounded_Set_Iterator <TAO_FlowSpec_Entry*>
#pragma instantiate ACE_Node <TAO_AV_Protocol_Item*>
#pragma instantiate ACE_Unbounded_Set<TAO_AV_Protocol_Item*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_AV_Protocol_Item*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
