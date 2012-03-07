//==============================================================
/**
 *  @file  Any_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_ANY_HANDLER_H
#define CIAO_CONFIG_HANDLERS_ANY_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_Common_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Config_Handlers/Common.h"
#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace CORBA
{
  class Any;
}
TAO_END_VERSIONED_NAMESPACE_DECL

namespace DAnCE
{
  namespace Config_Handlers
    {
      class Any;

      /*
       * @class Any_Handler
       *
       * @brief Handler class for <ComponentInterfaceDescription> types.
       *
       * This class defines handler methods to map values from
       * XSC Any objects, parsed from the descriptor files, to the
       * corresponding CORBA IDL Any type.
       *
       */
      class Config_Handlers_Common_Export Any_Handler
    {
    public:
      Any_Handler (void);
      virtual ~Any_Handler (void);

      static void extract_into_any (const Any& desc,
                                    ::CORBA::Any& toconfig);

      static Any get_any (const CORBA::Any &src);
    };
    }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_ANY_HANDLER_H*/
