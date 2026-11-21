#include "testC.h"
#include "ace/SString.h"

class Smart_Test_Factory : public virtual TAO_Test_Default_Proxy_Factory
{
public:
  Smart_Test_Factory ();

  /// This method will create the smart_proxy.
  Test_ptr create_proxy (Test_ptr proxy) override;
};

class Smart_Test_Proxy : public virtual TAO_Test_Smart_Proxy_Base
{
public:
  Smart_Test_Proxy (Test_ptr proxy);

  bool can_convert_to_ior () const override;

  char *convert_to_ior (bool use_omg_ior_format,
                        char const *ior_prefix) const override;

  /// "Smartify" the method call!
  CORBA::Short method (CORBA::Short boo) override;

  static const ACE_CString& fake_ior ();

  // Don't need to add any extra functionality into <shutdown>, hence
  // we don't define it.
private:
  static ACE_CString fake_ior_;
};
