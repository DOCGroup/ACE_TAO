// $Id$

#ifndef TAO_AV_AVSTREAMS_I_T_C
#define TAO_AV_AVSTREAMS_I_T_C

#include "AVStreams_i.h"

// ----------------------------------------------------------------------
// TAO_StreamEndPoint
// ----------------------------------------------------------------------

// constructor.
template <class IF>
TAO_StreamEndPoint<IF>::TAO_StreamEndPoint (void)
  :flow_count_ (1)
{
  //  this->handle_open ();
}

// Stop the physical flow of data on the stream
// Empty the_spec --> apply to all flows
template <class IF>
void
TAO_StreamEndPoint<IF>::stop (const AVStreams::flowSpec &the_spec,
                          CORBA::Environment &ACE_TRY_ENV)
{
  // Make the upcall into the app
  this->handle_stop (the_spec, ACE_TRY_ENV);
}

// Start the physical flow of data on the stream
// Empty the_spec --> apply to all flows
template <class IF>
void
TAO_StreamEndPoint<IF>::start (const AVStreams::flowSpec &flow_spec,
                           CORBA::Environment &ACE_TRY_ENV)
{
  // Make the upcall into the app
  this->handle_start (flow_spec, ACE_TRY_ENV);
}

// Close the connection
template <class IF>
void
TAO_StreamEndPoint<IF>::destroy (const AVStreams::flowSpec &the_spec,
                             CORBA::Environment &ACE_TRY_ENV)
{
  // Make the upcall into the app
  this->handle_destroy (the_spec, ACE_TRY_ENV);
}
// Called by our peer endpoint, requesting us to establish
// a connection
template <class IF>
CORBA::Boolean
TAO_StreamEndPoint<IF>::request_connection (AVStreams::StreamEndPoint_ptr initiator,
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
template <class IF>
CORBA::Boolean
TAO_StreamEndPoint<IF>::modify_QoS (AVStreams::streamQoS &new_qos,
                                const AVStreams::flowSpec &the_flows,
                                CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (new_qos);
  ACE_UNUSED_ARG (the_flows);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 0;
}

// @@ Need to throw not-supported exception here
template <class IF>
CORBA::Boolean
TAO_StreamEndPoint<IF>::set_protocol_restriction (const AVStreams::protocolSpec &the_pspec,
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

template <class IF>
void
TAO_StreamEndPoint<IF>::disconnect (const AVStreams::flowSpec &the_spec,
                                CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

// @@ Need to throw not-supported exception here
template <class IF>
void
TAO_StreamEndPoint<IF>::set_FPStatus (const AVStreams::flowSpec &the_spec,
                                  const char *fp_name,
                                  const CORBA::Any &fp_settings,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (fp_name);
  ACE_UNUSED_ARG (fp_settings);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}

template <class IF>
CORBA::Object_ptr
TAO_StreamEndPoint<IF>::get_fep (const char *flow_name,
                             CORBA::Environment &)
{
  TAO_String_Hash_Key fep_name_key (flow_name);
  FlowEndPoint_Map::ENTRY *fep_entry = 0;
  if (this->fep_map_.find (fep_name_key,fep_entry) == 0)
    return fep_entry->int_id_;
  return 0;
}

template <class IF>
char *
TAO_StreamEndPoint<IF>::add_fep (CORBA::Object_ptr the_fep,
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
      return 0;
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

template <class IF>
void
TAO_StreamEndPoint<IF>::remove_fep (const char *flow_name,
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
template <class IF>
void
TAO_StreamEndPoint<IF>::set_negotiator (AVStreams::Negotiator_ptr new_negotiator,
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
template <class IF>
void
TAO_StreamEndPoint<IF>::set_key (const char *flow_name,
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
template <class IF>
void
TAO_StreamEndPoint<IF>::set_source_id (CORBA::Long source_id,
                                   CORBA::Environment &)
{
  this->source_id_ = source_id;
}

template <class IF>
TAO_StreamEndPoint<IF>::~TAO_StreamEndPoint (void)
{
  //this->handle_close ();
}

#endif /*TAO_AV_AVSTREAMS_I_T_C*/
