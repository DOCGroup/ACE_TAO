// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/ORB_Core.h"
#include "tao/Connector_Registry.h"
#include "tao/Stub.h"
#include "tao/Environment.h"
#include "tao/GIOP.h"
#include "tao/debug.h"

TAO_Connector_Registry::TAO_Connector_Registry (void)
{
}

TAO_Connector_Registry::~TAO_Connector_Registry (void)
{
  this->close_all ();
}

TAO_Connector *
TAO_Connector_Registry::get_connector (CORBA::ULong tag)
{

  TAO_ConnectorSetItor end =
                this->connectors_.end ();
  TAO_ConnectorSetItor connector =
                this->connectors_.begin ();

  for (;
       connector != end ;
       connector++)
    {
      if ((*connector)->tag () == tag)
        return (*connector);
    }
  return 0;
}

int
TAO_Connector_Registry::open (TAO_ORB_Core *orb_core)
{
  // Open one connector for each loaded protocol!
  TAO_ProtocolFactorySetItor end =
                orb_core->protocol_factories ()->end ();
  TAO_ProtocolFactorySetItor factory =
                orb_core->protocol_factories ()->begin ();

  for (TAO_Connector *connector = 0;
       factory != end;
       ++factory)
    {
      connector = (*factory)->factory ()->make_connector ();

      if (connector)
        {
          connectors_.insert (connector);
          connector->open (orb_core);
        }
      else
        return -1;
    }
  return 0;
}

int
TAO_Connector_Registry::close_all (void)
{

  TAO_ConnectorSetItor end =
                this->connectors_.end ();
  TAO_ConnectorSetItor connector =
                this->connectors_.begin ();

  for (;
       connector != end ;
       connector++)
    {
      if (*connector)
        (*connector)->close ();
    }
  return 0;
}

int
TAO_Connector_Registry::preconnect (TAO_EndpointSet &preconnections)
{
  TAO_EndpointSetIterator first_endpoint_set = preconnections.begin ();
  TAO_EndpointSetIterator last_endpoint_set  = preconnections.end ();

  for (TAO_EndpointSetIterator i = first_endpoint_set;
       i != last_endpoint_set;
       ++i)
    {
      TAO_ConnectorSetItor first_connector = this->connectors_.begin ();
      TAO_ConnectorSetItor last_connector  = this->connectors_.end ();

      for (TAO_ConnectorSetItor connector = first_connector;
           connector != last_connector ;
           ++connector)
        {
          if (*connector)
            (*connector)->preconnect ((*i).c_str ());
        }
    }

  return 0;  // Success
}

int
TAO_Connector_Registry::connect (TAO_Stub *&obj,
                                 TAO_Transport *&transport)
{
  TAO_Profile *profile = obj->profile_in_use ();

  // here is where we get the appropriate connector object but we are
  // the Connector Registry so call get_connector(tag)
  TAO_Connector *connector = this->get_connector (profile->tag ());

  return connector->connect (profile, transport);
}

int
TAO_Connector_Registry::make_mprofile (const char *ior,
                                       TAO_MProfile &mprofile,
                                       CORBA::Environment &ACE_TRY_ENV)
{
  if (!ior)
    {
      ACE_THROW_RETURN (CORBA::MARSHAL (), -1);
      // Failure: Null IOR string pointer
    }

  TAO_ConnectorSetItor first_connector = this->connectors_.begin ();
  TAO_ConnectorSetItor last_connector = this->connectors_.end ();

  for (TAO_ConnectorSetItor connector = first_connector;
       connector != last_connector;
       ++connector)
    {
      if (*connector)
        {
          if ((*connector)->make_mprofile (ior, mprofile, ACE_TRY_ENV) == 0)
            return 0;  // Success
        }
      else
        {
          ACE_THROW_RETURN (CORBA::MARSHAL (), -1);
          // Failure: Null pointer to connector in connector registry
        }
    }

  ACE_THROW_RETURN (CORBA::MARSHAL (), -1);
  // Failure:  None of the connectors were able to parse the
  //           URL style IOR into an MProfile.
}

TAO_Profile*
TAO_Connector_Registry::create_profile (TAO_InputCDR& cdr)
{
  CORBA::ULong tag;

  // If there is an error we abort
  if ((cdr >> tag) == 0)
    return 0;

  TAO_Connector *connector =
    this->get_connector (tag);

  if (connector == 0)
    {
      if (TAO_debug_level > 0)
        {
          // @@ TODO create a generic profile in this case...
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) unknown profile tag %d\n",
                      tag));
        }

      TAO_Profile* pfile;
      ACE_NEW_RETURN (pfile, TAO_Unknown_Profile (tag), 0);

      if (pfile->decode (cdr) == -1)
        {
          pfile->_decr_refcnt ();
          pfile = 0;
        }
      return pfile;
    }

  // OK, we've got known profile.
  // It's going to be encapsulated ProfileData.
  // Create a new decoding stream and context for it, and skip the
  // data in the parent stream

  // ProfileData is encoded as a sequence of octet. So first get
  // the length of the sequence.
  CORBA::ULong encap_len;
  if ((cdr >> encap_len) == 0)
    return 0;

  // Create the decoding stream from the encapsulation in the
  // buffer, and skip the encapsulation.
  TAO_InputCDR str (cdr, encap_len);

  if (str.good_bit () == 0
      || cdr.skip_bytes (encap_len) == 0)
    return 0;

  return connector->create_profile (str);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<TAO_Connector*>;

template class ACE_Unbounded_Set<TAO_Connector*>;

template class ACE_Unbounded_Set_Iterator<TAO_Connector*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<TAO_Connector*>;

#pragma instantiate ACE_Unbounded_Set<TAO_Connector*>;

#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Connector*>;

#endif
