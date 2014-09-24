// $Id$

#include "DynString_Handler.h"
#include "DynAny_Handler.h"
#include "dance/Logger/Log_Macros.h"
#include "Basic_Deployment_Data.hpp"
#include "Common.h"

#include "ace/Null_Mutex.h"

#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Adapter_Impl.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/AnyTypeCode/Sequence_TypeCode.h"
#include "tao/IFR_Client/IFR_BasicC.h"

namespace DAnCE
{
  namespace Config_Handlers
  {
    DynamicAny::DynAny_ptr
    DynString_Handler::extract_into_dynany (const DataType &type,
                                            const DataValue &value,
                                            CORBA::TypeCode_ptr req_tc)
    {
      DANCE_TRACE("DynString_Handler::extract_into_dynany");

      CORBA::TypeCode_var tc;
      if (req_tc)
        tc = req_tc;
      else
        tc = DynString_Handler::create_typecode (type);

      // Make the actual DynAny
      DynamicAny::DynAny_var retval =
        DYNANY_HANDLER->daf ()->create_dyn_any_from_type_code (tc);

      const char* s = 0;
      if (value.count_string ())
        {
          s = ACE_TEXT_ALWAYS_CHAR ((*value.begin_string ())->c_str ());
        }
      retval->insert_string (s);

      return retval._retn ();
    }

    void
    DynString_Handler::extract_out_of_dynany (const DynamicAny::DynAny_ptr)
    {
      DANCE_TRACE("DynString_Handler::extract_out_of_dynany");

      DANCE_DEBUG (DANCE_LOG_NONFATAL_ERROR,
        (LM_ERROR, ACE_TEXT ("Extracting Sequences not yet supported\n")));
    }

    CORBA::TypeCode_ptr
    DynString_Handler::create_typecode (const DataType &type)
    {
      DANCE_TRACE("DynString_Handler::create_typecode");

      if (type.kind ().integral () != TCKind::tk_string_l)
        {
          DANCE_DEBUG (DANCE_LOG_TERMINAL_ERROR,
            (LM_ERROR, ACE_TEXT (
              "ERROR: String type description required")));
          throw Config_Error (ACE_TEXT (""),
            ACE_TEXT ("Expected <string> element, incorrect tc_kind."));
        }

      CORBA::ULong bound (0);
      if (type.boundedString_p ())
        {
          if (type.boundedString ().count_bound ())
          {
            bound = *(*type.boundedString ().begin_bound ());
          }
        }

      CORBA::TypeCode_var tc =
        DYNANY_HANDLER->orb ()->create_string_tc (bound);

      return tc._retn ();
    }
  }
}



