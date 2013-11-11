// $Id$

#include "ConfigValue.h"

namespace CIAO
{
   ConfigValue_impl::ConfigValue_impl()
   {
      CORBA::Any any;
      any <<= CORBA::Short(0);
      name((const char*)"");
      value(any);
   }


   ConfigValue_impl::ConfigValue_impl(const char*       the_name,
                                      const CORBA::Any& the_value)
      : OBV_Components::ConfigValue(the_name, the_value)
   {
   }

   CORBA::ValueBase* ConfigValue_impl::_copy_value()
   {
      return new ConfigValue_impl(name(),value());
   }

   ConfigValue_impl::~ConfigValue_impl()
      throw ()
   {
   }

   CORBA::ValueBase *
   ConfigValueFactory::create_for_unmarshal ()
   {
      return new ConfigValue_impl();
   }
}
