// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/ORB_Core.h"
#include "tao/Connector_Registry.h"
#include "tao/Stub.h"
#include "tao/Environment.h"
#include "tao/GIOP.h"
#include "tao/debug.h"

TAO_Connector_Registry::TAO_Connector_Registry (void)
  : connectors_ ()
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
        return *connector;
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

  TAO_Connector *connector = 0;

  for ( ;
       factory != end;
       ++factory)
    {
      connector = (*factory)->factory ()->make_connector ();

      if (connector)
        {
          if (connector->open (orb_core) != 0)
            {
              delete connector;

              ACE_ERROR_RETURN ((LM_ERROR,
                                 "TAO (%P|%t) unable to open connector for "
                                 "<%s>.\n",
                                 (*factory)->protocol_name ().c_str ()),
                                -1);
            }

          if (connectors_.insert (connector) == -1)
            {
              delete connector;

              ACE_ERROR_RETURN ((LM_ERROR,
                                 "TAO (%P|%t) unable to add a <%s> connector "
                                 "to the connector registry.\n",
                                 (*factory)->protocol_name ().c_str ()),
                                -1);
            }
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

  for (TAO_ConnectorSetItor i = this->connectors_.begin ();
       i != end;
       ++i)
    {
      if (*i == 0)
        continue;

      (*i)->close ();

      delete *i;
    }

  this->connectors_.reset ();
  return 0;
}

int
TAO_Connector_Registry::preconnect (TAO_ORB_Core *orb_core,
                                    TAO_PreconnectSet &preconnections)
{
  // Put the preconnects in a form that makes it simple for protocol 
  // implementers to parse.
  if (this->preprocess_preconnects (orb_core, preconnections) != 0)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) Unable to preprocess the preconnections.\n"));

      return -1;
    }

  TAO_PreconnectSetIterator preconnects = preconnections.begin ();

  for (ACE_CString *i = 0;
       preconnects.next (i) != 0;
       preconnects.advance ())
    {
      TAO_ConnectorSetItor first_connector =
        this->connectors_.begin ();
      TAO_ConnectorSetItor last_connector =
        this->connectors_.end ();

      for (TAO_ConnectorSetItor connector = first_connector;
           connector != last_connector;
           ++connector)
        if (*connector)
          (*connector)->preconnect (i->c_str ());
    }

  return 0;  // Success
}

int
TAO_Connector_Registry::preprocess_preconnects (TAO_ORB_Core *orb_core,
                                                TAO_PreconnectSet &preconnects)
{
  // Organize all matching protocol endpoints and addrs into a single
  // endpoint for the given protocol.
  //
  // For example, the following endpoints:
  //
  //   uiop://1.1@/tmp/foobar,/tmp/chicken
  //   iiop://1.0@localhost
  //   uiop:///tmp/soup
  //   iiop://1.1@mopbucket
  //
  // will be merged to create the following preconnects:
  //
  //   uiop://1.1@/tmp/foobar,/tmp/chicken,/tmp/soup
  //   iiop://1.0@localhost,1.1@mopbucket
  //
  // The four elements in the preconnect set will be squeezed into two 
  // elements, in this case. This is done to simplify the preconnect
  // parsing code in each protocol specific connector and to make sure 
  // that all preconnections are established during the first
  // attempt.  Otherwise, secondary attempts to establish
  // preconnections will not be successful since all preconnections
  // will have been idled after during the first attempt, hence the
  // need to pass all preconnects during the first attempt.

  const size_t num_protocols =
    orb_core->protocol_factories ()->size ();

  ACE_CString *processed = 0;

  ACE_NEW_RETURN (processed,
                  ACE_CString[num_protocols],
                  -1);

  // Open one connector for each loaded protocol!
  TAO_ProtocolFactorySetItor begin =
    orb_core->protocol_factories ()->begin ();
  TAO_ProtocolFactorySetItor end =
    orb_core->protocol_factories ()->end ();

  ACE_CString *tmp = processed;

  // Iterate over the protocols, *not* the connectors!
  for (TAO_ProtocolFactorySetItor factory = begin;
       factory != end;
       ++factory, ++tmp)
    {
      (*tmp) =
        ACE_CString ((*factory)->factory ()->prefix ()) + ACE_CString ("://");

      TAO_PreconnectSetIterator p = preconnects.begin ();

      for (ACE_CString *i = 0;
           p.next (i) != 0;
           p.advance ())
        {
          const int slot = i->find ("://");

          ACE_CString protocol_name = i->substring (0, slot);

          if (slot != ACE_CString::npos &&
              slot != ACE_static_cast (int, (i->length () - 3)) &&
              (*factory)->factory ()->match_prefix (protocol_name.c_str ()))
            {
              (*tmp) += i->substring (slot + 3); // +3 due to "://"
              (*tmp) += ACE_CString (',');
            }
        }

      // Remove the trailing comma ','.
      if ((*tmp)[tmp->length () - 1] == ',')
        (*tmp) = tmp->substring (0, tmp->length () - 1);
    }

  // Empty the preconnect container.
  preconnects.reset ();

  // Now enqueue the re-formed preconnect strings.
  for (size_t n = 0; n < num_protocols; ++n)
    {
      // If no preconnects for the given protocol exist then don't
      // enqueue the empty preconnect list for that protocol.
      // Such an empty preconnect string should be of the form
      //
      //     protocol://
      //
      // so check for the forward slash '/' at the end of the string.
      if (processed[n][processed[n].length () - 1] != '/')
        {
          if (preconnects.enqueue_tail (processed[n]) != 0)
            {
              delete [] processed;

              return -1;
            }
        }
    }

  delete [] processed;

  return 0;
}


int
TAO_Connector_Registry::connect (TAO_Profile *&profile,
                                 TAO_Transport *&transport,
                                 ACE_Time_Value *max_wait_time)
{
  // Find the appropriate connector object
  TAO_Connector *connector =
    this->get_connector (profile->tag ());

  if (connector == 0)
    return -1;

  return connector->connect (profile, transport, max_wait_time);
}

int
TAO_Connector_Registry::make_mprofile (const char *ior,
                                       TAO_MProfile &mprofile,
                                       CORBA::Environment &ACE_TRY_ENV)
{
  if (!ior)
    // Failure: Null IOR string pointer
    ACE_THROW_RETURN (CORBA::INV_OBJREF (
      CORBA_SystemException::_tao_minor_code (
        TAO_NULL_POINTER_MINOR_CODE,
        0),
      CORBA::COMPLETED_NO),
      -1);

  TAO_ConnectorSetItor first_connector =
    this->connectors_.begin ();
  TAO_ConnectorSetItor last_connector =
    this->connectors_.end ();

  for (TAO_ConnectorSetItor connector = first_connector;
       connector != last_connector;
       ++connector)
    {
      if (*connector)
        {
          int mp_result = (*connector)->make_mprofile (ior,
                                                       mprofile,
                                                       ACE_TRY_ENV);

          ACE_CHECK_RETURN (mp_result);

          if (mp_result == 0)
            return 0;  // Success
        }
      else
        // Failure: Null pointer to connector in connector registry.
        ACE_THROW_RETURN (CORBA::INV_OBJREF (
          CORBA_SystemException::_tao_minor_code (
            TAO_NULL_POINTER_MINOR_CODE,
            0),
          CORBA::COMPLETED_NO),
          -1);
    }

  // Failure: None of the connectors were able to parse the URL style
  // IOR into an MProfile.
  ACE_THROW_RETURN (CORBA::INV_OBJREF (
   CORBA_SystemException::_tao_minor_code (
      TAO_CONNECTOR_REGISTRY_NO_USABLE_PROTOCOL,
      0),
   CORBA::COMPLETED_NO),
   -1);
}

TAO_Profile *
TAO_Connector_Registry::create_profile (TAO_InputCDR &cdr)
{
  CORBA::ULong tag = 0;

  // If there is an error we abort.
  if ((cdr >> tag) == 0)
    return 0;

  TAO_Connector *connector =
    this->get_connector (tag);

  if (connector == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) unknown profile tag %d\n",
                      tag));
        }

      TAO_Profile *pfile = 0;
      ACE_NEW_RETURN (pfile,
                      TAO_Unknown_Profile (tag),
                      0);
      if (pfile->decode (cdr) == -1)
        {
          pfile->_decr_refcnt ();
          pfile = 0;
        }

      return pfile;
    }

  // OK, we've got a known profile.  It's going to be encapsulated
  // ProfileData.  Create a new decoding stream and context for it,
  // and skip the data in the parent stream

  // ProfileData is encoded as a sequence of octet. So first get the
  // length of the sequence.
  CORBA::ULong encap_len = 0;
  if ((cdr >> encap_len) == 0)
    return 0;

  // Create the decoding stream from the encapsulation in the buffer,
  // and skip the encapsulation.
  TAO_InputCDR str (cdr, encap_len);

  if (str.good_bit () == 0
      || cdr.skip_bytes (encap_len) == 0)
    return 0;

  return connector->create_profile (str);
}

char
TAO_Connector_Registry::object_key_delimiter (const char *ior)
{
  if (!ior)
    return 0; // Failure: Null IOR string pointer

  TAO_ConnectorSetItor first_connector =
    this->connectors_.begin ();
  TAO_ConnectorSetItor last_connector =
    this->connectors_.end ();

  for (TAO_ConnectorSetItor connector = first_connector;
       connector != last_connector;
       ++connector)
    {
      if (*connector)
        {
          if ((*connector)->check_prefix (ior) == 0)
            return (*connector)->object_key_delimiter ();
        }
    }

  // Failure: None of the connectors were able to match their protocol
  // against the provided string.
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<TAO_Connector*>;
template class ACE_Unbounded_Set<TAO_Connector*>;
template class ACE_Unbounded_Set_Iterator<TAO_Connector*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<TAO_Connector*>
#pragma instantiate ACE_Unbounded_Set<TAO_Connector*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Connector*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
