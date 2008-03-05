// $Id$


#ifndef CIAO_CONFIG_VALUE_H
#define CIAO_CONFIG_VALUE_H
#include /**/ "ace/pre.h"

#include "CCM_StandardConfiguratorC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CIAO_Client_Export.h"
// #include "ace/Active_Map_Manager.h"

namespace CIAO
{
  class CIAO_CLIENT_Export ConfigValue_impl
    : public virtual OBV_Components::ConfigValue,
      public virtual CORBA::DefaultValueRefCountBase
  {
  public:
    ConfigValue_impl();
    ConfigValue_impl(const char* the_name, const CORBA::Any& the_value);
    ~ConfigValue_impl() throw();
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
