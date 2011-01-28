// $Id$

#include "tao/DLL_Parser.h"

# if (TAO_HAS_DDL_PARSER == 1)

#include "tao/Object_Loader.h"
#include "tao/Object.h"
#include "tao/ORB_Constants.h"
#include "tao/SystemException.h"
#include "tao/ORB_Core.h"

#include "ace/Dynamic_Service.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

static const char dll_prefix[] = "DLL:";

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DLL_Parser::~TAO_DLL_Parser (void)
{
}


bool
TAO_DLL_Parser::match_prefix (const char *ior_string) const
{
  return (ACE_OS::strncmp (ior_string,
                           ::dll_prefix,
                           sizeof (::dll_prefix) - 1) == 0);
}

CORBA::Object_ptr
TAO_DLL_Parser::parse_string (const char *ior,
                              CORBA::ORB_ptr orb)
{
  // Skip the prefix, we know it is there because this method in only
  // called if <match_prefix> returns 1.
  const char *name =
    ior + sizeof (::dll_prefix) - 1;

  TAO_ORB_Core *oc = orb->orb_core ();

  TAO_Object_Loader *loader =
    ACE_Dynamic_Service<TAO_Object_Loader>::instance
      (oc->configuration(), name);

  if (loader == 0)
    {
      throw
         CORBA::INV_OBJREF
         (CORBA::SystemException::_tao_minor_code (
            0,
            EINVAL),
          CORBA::COMPLETED_NO);
    }

  return loader->create_object (orb, 0, 0);
}


ACE_STATIC_SVC_DEFINE (TAO_DLL_Parser,
                       ACE_TEXT ("DLL_Parser"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_DLL_Parser),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)


ACE_FACTORY_DEFINE (TAO, TAO_DLL_Parser)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_DDL_PARSER == 1 */
