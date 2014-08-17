// $Id$

#include "testC.h"
#include "ace/SString.h"

class Smart_Test_Factory : public virtual TAO_Test_Default_Proxy_Factory
{
public:
  Smart_Test_Factory (void);

  virtual Test_ptr create_proxy (Test_ptr proxy);
  // This method will create the smart_proxy.
};

class Smart_Test_Proxy : public virtual TAO_Test_Smart_Proxy_Base
{
public:
  Smart_Test_Proxy (Test_ptr proxy);

  virtual bool can_convert_to_ior (void) const;

  virtual char* convert_to_ior (bool use_omg_ior_format,
                                const char* ior_prefix) const;

  virtual CORBA::Short method (CORBA::Short boo);
  // "Smartify" the method call!

  static const ACE_CString& fake_ior (void);

  // Don't need to add any extra functionality into <shutdown>, hence
  // we don't define it.
private:
  static ACE_CString fake_ior_;
};
