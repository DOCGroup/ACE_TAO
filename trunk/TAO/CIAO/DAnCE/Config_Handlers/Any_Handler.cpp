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
            CORBA::Short (static_cast <short &> (value.short_ ()));
          
          //  static_cast<CORBA::Short> (value.short_ ());
        }
      else if (value.ushort_p ())
        {
          toconfig <<=
            CORBA::UShort (static_cast<unsigned short &> (value.ushort ()));

        }
      else if (value.long_p ())
        {
          toconfig <<=
            CORBA::Long (static_cast <int &> (value.long_ ()));

        }
      else if (value.ulong_p ())
        {
          toconfig <<=
            CORBA::ULong (static_cast <unsigned int &> (value.ulong ()));
        }
      else if (value.boolean_p ())
        {
          toconfig <<= 
            CORBA::Boolean (static_cast <bool &> (value.boolean ()));

        }
      else if (value.double_p ())
        {
          toconfig <<= 
            CORBA::Double (static_cast <double &> (value.double_ ()));
        }
      else if (value.float_p ())
        {
          toconfig <<= 
            CORBA::Float (static_cast <float &> (value.float_ ()));
        }
      else if (value.string_p ())
        {
          toconfig <<=
            value.string ().c_str ();

        }
      else if (value.octet_p ())
        {
          CORBA::Octet val (static_cast <unsigned char &> (value.octet ()));

          toconfig <<=
            CORBA::Any::from_octet (val);

        }
      else if (value.longlong_p ())
        {
          toconfig <<= 
            CORBA::LongLong (static_cast <long long &> (value.longlong ()));
        }
      else if (value.ulonglong_p ())
        {
          toconfig <<=
            CORBA::ULongLong (static_cast <unsigned long long &> (value.ulonglong ()));
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
