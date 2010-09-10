// $Id$

#include /**/ "ace/pre.h"

#include "Any_Handler.h"
#include "DataType_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "tao/AnyTypeCode/Any.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DynAny_Handler/DynAny_Handler.h"

#include "ace/Null_Mutex.h"

namespace DAnCE
{
  namespace Config_Handlers
  {

    Any_Handler::Any_Handler (void)
    {
    }

    Any_Handler::~Any_Handler (void)
    {
    }

    void
    Any_Handler::extract_into_any (const Any& desc,
                                   CORBA::Any& toconfig)
    {
      DANCE_TRACE("Any_Handler::extract_into_any");
      try
        {
          DynamicAny::DynAny_var dyn = DYNANY_HANDLER->extract_into_dynany (desc.type (),
                                                                            desc.value ());
          CORBA::Any_var any_safe (dyn->to_any ());
          toconfig = *any_safe;

          dyn->destroy ();
        }
      catch (Config_Error &ex)
        {
          throw ex;
        }
      catch (...)
        {
          throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Caught error whilst parsing XML into Any\n"));
        }
    }

    Any Any_Handler::get_any (const ::CORBA::Any& src)
    {
      DANCE_TRACE("Any_Handler::extract_into_any - reverse");

      return DYNANY_HANDLER->extract_from_dynany (src);
    }
  }
}
#include /**/ "ace/post.h"
