// $Id$

#include "ANY_Handler.h"
#include "tao/Any.h"

namespace CIAO
{
  namespace Config_Handlers
  {

    ANY_Handler::ANY_Handler (void)
    {
    }

    ANY_Handler::~ANY_Handler (void)
    {
    }


    void
    ANY_Handler::get_Any (CORBA::Any& toconfig,
                          Any& desc)
    {

      // Get the value that should be assigned to the Any.
      DataValue value =
        desc.value ();

      // @@ Jules, you may want to align your coding standards to ACE
      // coding standards. Else, Dr. Schmidt is going to come down on
      // you heavily in CS291 :-)
      if (value.short_p ())
        {
          // @@ Jules, please try using regular C++ casts
          toconfig <<=
            static_cast <CORBA::Short> (value.short_ ());
        }
      else if (value.ushort_p ())
        {
          toconfig <<=
            static_cast <CORBA::UShort> (value.ushort ());
        }
      else if (value.long_p ())
        {
          toconfig <<=
            static_cast <CORBA::UShort> (value.long_ ());
        }
      else if (value.ulong_p ())
        {
          toconfig <<=
            static_cast <CORBA::UShort> (value.ulong ());
        }
      else if (value.boolean_p ())
        {
          toconfig <<=
            static_cast <CORBA::Boolean> (value.boolean ());
        }
      else if (value.double_p ())
        {
          toconfig <<=
            static_cast <CORBA::Double> (value.double_ ());
        }
      else if (value.float_p ())
        {
          toconfig <<=
            static_cast <CORBA::Float> (value.float_ ());
        }
      else if (value.string_p ())
        {
          toconfig <<=
            value.string ().c_str ();
        }
      else if (value.octet_p ())
        {
          toconfig <<=
            CORBA::Any::from_octet (static_cast<CORBA::Octet> (value.octet ()));
        }
      else if (value.longlong_p ())
        {
          toconfig <<=
            static_cast <CORBA::LongLong> (value.longlong ());
        }
      else if (value.ulonglong_p ())
        {
          toconfig <<=
            static_cast <CORBA::ULongLong> (value.ulonglong ());
        }
        
    }
  }
}
