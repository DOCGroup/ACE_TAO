// $Id$

#include "Any_Handler.h"
#include "DataType_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "tao/Any.h"

namespace CIAO
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
      // Get the value that should be assigned to the Any.
      DataValue value =
        desc.value ();

      if (value.short_p ())
        {
          toconfig <<=
            static_cast<CORBA::Short> (value.short_ ());
        }
      else if (value.ushort_p ())
        {
          toconfig <<=
            static_cast<CORBA::UShort> (value.ushort ());

        }
      else if (value.long_p ())
        {
          toconfig <<=
            static_cast <CORBA::Long> (value.long_ ());

        }
      else if (value.ulong_p ())
        {
          CORBA::ULong val = value.ulong ();

          toconfig <<= val;

        }
      else if (value.boolean_p ())
        {
          CORBA::Boolean val = value.boolean ();

          toconfig <<= val;

        }
      else if (value.double_p ())
        {
          CORBA::Boolean val = value.double_ ();
          toconfig <<= val;

        }
      else if (value.float_p ())
        {
          CORBA::Float val = value.float_ ();
          toconfig <<= val;

        }
      else if (value.string_p ())
        {
          toconfig <<=
            value.string ().c_str ();

        }
      else if (value.octet_p ())
        {
          CORBA::Octet val = value.octet ();

          toconfig <<=
            CORBA::Any::from_octet (val);

        }
      else if (value.longlong_p ())
        {
          CORBA::LongLong val = value.longlong ();

          toconfig <<= val;

        }
      else if (value.ulonglong_p ())
        {
          CORBA::ULongLong val = value.ulonglong ();
          toconfig <<=val;

        }
    }

    Any Any_Handler::get_any (const ::CORBA::Any& src)
    {
      //MAJO
      //@Bala ... still haven't figured this one out yet
      //I was saving it for last...going from corba::any
      //to xsc DataValue
      DataType type = DataType_Handler::data_type (src.type ());
      DataValue value;
      Any any (type,value);
      return any;
    }
  }
}
