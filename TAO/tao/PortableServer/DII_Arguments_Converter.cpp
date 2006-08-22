// @(#) $Id$

#include "tao/PortableServer/DII_Arguments_Converter.h"
#include "ace/Service_Config.h"

ACE_RCSID(PortableServer,
          DII_Arguments_Converter,
          "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void TAO_DII_Arguments_Converter::convert (
TAO_ServerRequest & ,
TAO::Argument * const [],
size_t
ACE_ENV_ARG_DECL )
{
#if defined (ACE_HAS_EXCEPTIONS)
      ACE_THROW (CORBA::INTERNAL ());
#else
      ACE_OS::abort ();
#endif /* ACE_HAS_EXCEPTIONS */
}


int
TAO_DII_Arguments_Converter::Initializer (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_DII_Arguments_Converter);
}


TAO_END_VERSIONED_NAMESPACE_DECL


ACE_STATIC_SVC_DEFINE (
  TAO_DII_Arguments_Converter,
  ACE_TEXT ("DII_Arguments_Converter"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (TAO_DII_Arguments_Converter),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0
  )

ACE_FACTORY_DEFINE (TAO_PortableServer, TAO_DII_Arguments_Converter)


