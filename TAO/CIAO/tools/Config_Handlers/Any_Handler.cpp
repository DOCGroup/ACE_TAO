// $Id$

#include /**/ "ace/pre.h"

#include "Any_Handler.h"
#include "DataType_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "tao/AnyTypeCode/Any.h"


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
            CORBA::Short (static_cast < ::XMLSchema::short_ const & > (value.short_ ()));

          //  static_cast<CORBA::Short> (value.short_ ());
        }
      else if (value.ushort_p ())
        {
          toconfig <<=
            CORBA::UShort (static_cast< ::XMLSchema::unsignedShort const & > (value.ushort ()));

        }
      else if (value.long_p ())
        {
          toconfig <<=
            CORBA::Long (static_cast < ::XMLSchema::int_ const& > (value.long_ ()));

        }
      else if (value.ulong_p ())
        {
          toconfig <<=
            CORBA::ULong (static_cast < ::XMLSchema::unsignedInt const& > (value.ulong ()));
        }
      else if (value.boolean_p ())
        {
          toconfig <<=
	    CORBA::Any::from_boolean (static_cast < ::XMLSchema::boolean const& > (value.boolean ()));
        }
      else if (value.double_p ())
        {
          toconfig <<=
#ifdef __BORLANDC__
            CORBA::Double (static_cast < double &> (value.double_ ()));
#else
            CORBA::Double (static_cast < ::XMLSchema::double_ const& > (value.double_ ()));
#endif
        }
      else if (value.float_p ())
        {
          toconfig <<=
#ifdef __BORLANDC__
            CORBA::Float (static_cast < float & > (value.float_ ()));
#else
            CORBA::Float (static_cast < ::XMLSchema::float_ const& > (value.float_ ()));
#endif
        }
      else if (value.string_p ())
        {
          toconfig <<=
            value.string ().c_str ();

        }
      else if (value.octet_p ())
        {
          CORBA::Octet val (static_cast <const unsigned char &> (value.octet ()));

          toconfig <<=
            CORBA::Any::from_octet (val);

        }
      else if (value.longlong_p ())
        {
          toconfig <<=
            CORBA::LongLong (static_cast < ::XMLSchema::long_ const& > (value.longlong ()));
	}
      else if (value.ulonglong_p ())
        {
          toconfig <<=
            CORBA::ULongLong (static_cast < ::XMLSchema::unsignedLong const& > (value.ulonglong ()));
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
#include /**/ "ace/post.h"
