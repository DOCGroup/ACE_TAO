// -*- C++ -*-
//
// $Id$

#include "tao/Connector_Registry.h"
#include "tao/ORB_Core.h"
#include "tao/Profile.h"
#include "tao/Transport_Connector.h"
#include "tao/Protocol_Factory.h"
#include "tao/Endpoint.h"
#include "tao/Environment.h"
#include "tao/debug.h"
#include "tao/Transport_Descriptor_Interface.h"

#if !defined(__ACE_INLINE__)
#include "tao/Connector_Registry.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Connector_Registry,
           "$Id$")

TAO_Connector_Registry::TAO_Connector_Registry (void)
  : connectors_ (0),
    size_ (0)
{
}

TAO_Connector_Registry::~TAO_Connector_Registry (void)
{
  this->close_all ();

  delete [] this->connectors_;
  this->connectors_ = 0;
  this->size_ = 0;
}

TAO_Connector *
TAO_Connector_Registry::get_connector (CORBA::ULong tag)
{
  TAO_ConnectorSetIterator end =
    this->end ();
  TAO_ConnectorSetIterator connector =
    this->begin ();

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
  TAO_ProtocolFactorySet *pfs = orb_core->protocol_factories ();

  // Open one connector for each loaded protocol!
  TAO_ProtocolFactorySetItor end = pfs->end ();
  TAO_ProtocolFactorySetItor factory = pfs->begin ();

  TAO_Connector *connector = 0;

  // The array containing the TAO_Connectors will never contain more
  // than the number of loaded protocols in the ORB core.
  if (this->connectors_ == 0)
    ACE_NEW_RETURN (this->connectors_,
                    TAO_Connector *[pfs->size ()],
                    -1);

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
                                 ACE_LIB_TEXT ("TAO (%P|%t) unable to open connector for ")
                                 ACE_LIB_TEXT ("<%s>.\n"),
                                 ACE_TEXT_CHAR_TO_TCHAR((*factory)->protocol_name ().c_str ())),
                                -1);
            }

          this->connectors_[this->size_++] = connector;
        }
      else
        return -1;
    }

  return 0;
}

int
TAO_Connector_Registry::close_all (void)
{
  TAO_ConnectorSetIterator end = this->end ();

  for (TAO_ConnectorSetIterator i = this->begin ();
       i != end;
       ++i)
    {
      if (*i == 0)
        continue;

      (*i)->close ();

      delete *i;
    }

  this->size_ = 0;

  return 0;
}

int
TAO_Connector_Registry::connect (TAO_GIOP_Invocation *invocation,
                                 TAO_Transport_Descriptor_Interface *desc
                                 ACE_ENV_ARG_DECL)
{
  TAO_Endpoint *endpoint = desc->endpoint ();
  if (endpoint == 0)
    return -1;

  // Find the appropriate connector object.
  TAO_Connector *connector =
    this->get_connector (endpoint->tag ());

  if (connector == 0)
      return -1;

  return connector->connect (invocation,
                             desc
                              ACE_ENV_ARG_PARAMETER);
}

int
TAO_Connector_Registry::make_mprofile (const char *ior,
                                       TAO_MProfile &mprofile
                                       ACE_ENV_ARG_DECL)
{
  if (!ior)
    // Failure: Null IOR string pointer
    ACE_THROW_RETURN (CORBA::INV_OBJREF (
      CORBA::SystemException::_tao_minor_code (
        TAO_DEFAULT_MINOR_CODE,
        EINVAL),
      CORBA::COMPLETED_NO),
      -1);

  TAO_ConnectorSetIterator first_connector = this->begin ();
  TAO_ConnectorSetIterator last_connector = this->end ();

  for (TAO_ConnectorSetIterator connector = first_connector;
       connector != last_connector;
       ++connector)
    {
      if (*connector)
        {
          int mp_result = (*connector)->make_mprofile (ior,
                                                       mprofile
                                                        ACE_ENV_ARG_PARAMETER);

          ACE_CHECK_RETURN (mp_result);

          if (mp_result == 0)
            return 0;  // Success
        }
      else
        // Failure: Null pointer to connector in connector registry.
        ACE_THROW_RETURN (CORBA::INV_OBJREF (
          CORBA::SystemException::_tao_minor_code (
            TAO_DEFAULT_MINOR_CODE,
            EINVAL),
          CORBA::COMPLETED_NO),
          -1);
    }

  // Failure: None of the connectors were able to parse the URL style
  // IOR into an MProfile.
  ACE_THROW_RETURN (CORBA::INV_OBJREF (
   CORBA::SystemException::_tao_minor_code (
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
                      ACE_LIB_TEXT ("TAO (%P|%t) Unknown profile tag 0x%x\n"),
                      tag));
        }

      TAO_Profile *pfile = 0;
      ACE_NEW_RETURN (pfile,
                      TAO_Unknown_Profile (tag,
                                           cdr.orb_core ()),
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
    {
      errno = EINVAL;
      return 0; // Failure: Null IOR string pointer
    }

  TAO_ConnectorSetIterator first_connector = this->begin ();
  TAO_ConnectorSetIterator last_connector =  this->end ();

  for (TAO_ConnectorSetIterator connector = first_connector;
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
