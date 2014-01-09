// $Id$

#include "tao/Connector_Registry.h"
#include "tao/ORB_Core.h"
#include "tao/Profile.h"
#include "tao/Transport_Connector.h"
#include "tao/Protocol_Factory.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"
#include "ace/Auto_Ptr.h"

#if !defined(__ACE_INLINE__)
#include "tao/Connector_Registry.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Connector_Registry::TAO_Connector_Registry (void)
  : connectors_ (0),
    size_ (0)
{
}

TAO_Connector_Registry::~TAO_Connector_Registry (void)
{
  this->close_all ();

  delete [] this->connectors_;
}

TAO_Connector *
TAO_Connector_Registry::get_connector (CORBA::ULong tag) const
{
  const TAO_ConnectorSetIterator end = this->end ();

  for (TAO_ConnectorSetIterator connector = this->begin ();
       connector != end;
       ++connector)
    {
      if ((*connector)->tag () == tag)
        return *connector;
    }

  return 0;
}

int
TAO_Connector_Registry::open (TAO_ORB_Core *orb_core)
{
  TAO_ProtocolFactorySet * const pfs =
    orb_core->protocol_factories ();

  // The array containing the TAO_Connectors will never contain more
  // than the number of loaded protocols in the ORB core.
  if (this->connectors_ == 0)
    ACE_NEW_RETURN (this->connectors_,
                    TAO_Connector *[pfs->size ()],
                    -1);

  // Open one connector for each loaded protocol!
  const TAO_ProtocolFactorySetItor end = pfs->end ();

  for (TAO_ProtocolFactorySetItor factory = pfs->begin ();
       factory != end;
       ++factory)
    {
      auto_ptr <TAO_Connector> connector (
        (*factory)->factory ()->make_connector ());

      if (connector.get ())
        {
         if (connector->open (orb_core) != 0)
           {
             TAOLIB_ERROR_RETURN ((LM_ERROR,
                                ACE_TEXT ("TAO (%P|%t) - TAO_Connector_Registry")
                                ACE_TEXT ("::open: unable to open connector for ")
                                ACE_TEXT ("<%C>.\n"),
                                (*factory)->protocol_name ().c_str ()),
                               -1);
           }

         this->connectors_[this->size_++] =
           connector.release ();
        }
      else
        return -1;
    }

  return 0;
}

int
TAO_Connector_Registry::close_all (void)
{
  const TAO_ConnectorSetIterator end = this->end ();

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
TAO_Connector_Registry::make_mprofile (const char *ior,
                                       TAO_MProfile &mprofile
                                       )
{
  if (!ior)
    // Failure: Null IOR string pointer
    throw ::CORBA::INV_OBJREF (
      CORBA::SystemException::_tao_minor_code (
        0,
        EINVAL),
      CORBA::COMPLETED_NO);

  const TAO_ConnectorSetIterator first_connector = this->begin ();
  const TAO_ConnectorSetIterator last_connector = this->end ();

  for (TAO_ConnectorSetIterator connector = first_connector;
       connector != last_connector;
       ++connector)
    {
      if (*connector)
        {
          const int mp_result =
            (*connector)->make_mprofile (ior,
                                         mprofile
                                        );

          if (mp_result == 0)
            return 0;  // Success
        }
      else
        // Failure: Null pointer to connector in connector registry.
        throw ::CORBA::INV_OBJREF (
          CORBA::SystemException::_tao_minor_code (
            0,
            EINVAL),
          CORBA::COMPLETED_NO);
    }

  // Failure: None of the connectors were able to parse the URL style
  // IOR into an MProfile.
  throw ::CORBA::INV_OBJREF (
    CORBA::SystemException::_tao_minor_code (
      TAO_CONNECTOR_REGISTRY_NO_USABLE_PROTOCOL,
      0),
    CORBA::COMPLETED_NO);
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
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - TAO_Connector_Registry::")
                      ACE_TEXT ("create_profile: Unknown profile tag 0x%x\n"),
                      tag));
        }

      TAO_ORB_Core *orb_core = cdr.orb_core ();
      if (orb_core == 0)
        {
          orb_core = TAO_ORB_Core_instance ();
          if (TAO_debug_level > 0)
            {
              TAOLIB_DEBUG ((LM_WARNING,
                          ACE_TEXT ("TAO (%P|%t) - TAO_Connector_Registry")
                          ACE_TEXT ("::create_profile: ")
                          ACE_TEXT ("WARNING: extracting object from ")
                          ACE_TEXT ("default ORB_Core\n")));
            }
        }


      TAO_Profile *pfile = 0;
      ACE_NEW_RETURN (pfile,
                      TAO_Unknown_Profile (tag,
                                           orb_core),
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

  const TAO_ConnectorSetIterator first_connector = this->begin ();
  const TAO_ConnectorSetIterator last_connector =  this->end ();

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

TAO_END_VERSIONED_NAMESPACE_DECL
