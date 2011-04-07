// $Id$

#ifndef CIAO_CONFIG_VALUE_H
#define CIAO_CONFIG_VALUE_H
#include /**/ "ace/pre.h"

#include "ccm/CCM_StandardConfiguratorC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ConfigValue_Export.h"

namespace CIAO
{
  class ConfigValue_Factory_Export ConfigValue_impl
    : public virtual OBV_Components::ConfigValue,
      public virtual CORBA::DefaultValueRefCountBase
  {
  public:
    ConfigValue_impl();
    ConfigValue_impl(const char* the_name, const CORBA::Any& the_value);
    virtual ~ConfigValue_impl() throw();
    virtual CORBA::ValueBase* _copy_value();
  };

  class ConfigValueFactory : virtual public Components::ConfigValue_init
  {
  public:
     CORBA::ValueBase * create_for_unmarshal ();
  };
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_VALUE_H */
