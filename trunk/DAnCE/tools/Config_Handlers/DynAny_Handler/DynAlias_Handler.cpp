// $Id$

#include "DynAlias_Handler.h"
#include "DynAny_Handler.h"
#include "dance/Logger/Log_Macros.h"
#include "Basic_Deployment_Data.hpp"
#include "Common.h"

#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Adapter_Impl.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/AnyTypeCode/Enum_TypeCode.h"
#include "tao/IFR_Client/IFR_BasicC.h"

namespace DAnCE
{
  namespace Config_Handlers
  {
    DynamicAny::DynAny_ptr
    DynAlias_Handler::extract_into_dynany (const DataType &type,
                                           const DataValue &value,
                                           CORBA::TypeCode_ptr req_tc)
    {
      DANCE_TRACE("DynAlias_Handler::extract_into_dynany");

      try
        {
          CORBA::TypeCode_var tc;

          if (req_tc)
            tc = req_tc;
          else
            tc = DynAlias_Handler::create_typecode (type);

          DynamicAny::DynAny_var alias = DYNANY_HANDLER->extract_into_dynany (
            type.alias ().elementType (), value, tc);
          return alias._retn ();
        }
      catch (const DynamicAny::DynAny::InvalidValue&)
        {
          DANCE_DEBUG (DANCE_LOG_TERMINAL_ERROR,
            (LM_ERROR,
              ACE_TEXT ("Invalid value provided in XML when trying to ")
              ACE_TEXT ("initialize an instance of enumerated type %s\n"),
              type.alias ().typeId ().c_str ()));
          throw Config_Error (type.alias ().typeId (),
              ACE_TEXT ("Invalid value provided in XML"));
        }
      catch (Config_Error &ex)
        {
          DANCE_DEBUG (DANCE_LOG_TERMINAL_ERROR,
            (LM_ERROR, ACE_TEXT ("DynAlias_Handler caught Config_Error\n")));
          if (type.alias_p ())
            ex.add_name (type.alias ().typeId ());

          throw ex;
        }
      catch (...)
        {
          DANCE_DEBUG (DANCE_LOG_TERMINAL_ERROR, (LM_ERROR,
            ACE_TEXT ("DynAlias_Handler caught unknown exception\n")));
          throw Config_Error (type.alias ().typeId (),
                              ACE_TEXT ("Unknown exception"));
        }
    }

    void
    DynAlias_Handler::extract_out_of_dynany (const DynamicAny::DynAny_ptr)
    {
      DANCE_TRACE("DynAlias_Handler::extract_out_of_dynany");

      DANCE_DEBUG (DANCE_LOG_NONFATAL_ERROR,
        (LM_ERROR, ACE_TEXT ("Extracting Enums not yet supported\n")));
    }

    CORBA::TypeCode_ptr
    DynAlias_Handler::create_typecode (const DataType &type)
    {
      DANCE_TRACE("DynAlias_Handler::create_typecode");

      if (!type.alias_p ())
        {
          DANCE_DEBUG (DANCE_LOG_TERMINAL_ERROR,
            (LM_ERROR, ACE_TEXT ("ERROR: Alias type description required")));
          throw Config_Error (ACE_TEXT (""),
            ACE_TEXT ("Did not find expected alias type description, tk_kind"\
                      "may be wrong."));
        }

      CORBA::TypeCode_var tc =
        DYNANY_HANDLER->orb ()->create_alias_tc
        (ACE_TEXT_ALWAYS_CHAR (type.alias ().typeId ().c_str ()),
         ACE_TEXT_ALWAYS_CHAR (type.alias ().name ().c_str ()),
         DYNANY_HANDLER->create_typecode (type.alias ().elementType ()));

      DYNANY_HANDLER->register_typecode (type.alias ().typeId (),
                                         tc.in ());

      return tc._retn ();
    }

  }
}

