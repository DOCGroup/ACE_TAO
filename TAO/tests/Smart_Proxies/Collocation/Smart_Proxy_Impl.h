#ifndef SMART_PROXY_IMPL_H
#define SMART_PROXY_IMPL_H

#include "DiamondC.h"
#include "stub_export.h"

class MY_Stub_Export Smart_Diamond_Top_Factory
  : public virtual Diamond::TAO_Diamond_Top_Default_Proxy_Factory
{
public:
  Smart_Diamond_Top_Factory ();
  Smart_Diamond_Top_Factory (Smart_Diamond_Top_Factory const &) = delete;
  Smart_Diamond_Top_Factory (Smart_Diamond_Top_Factory &&) = delete;
  Smart_Diamond_Top_Factory &operator= (Smart_Diamond_Top_Factory const &) = delete;
  Smart_Diamond_Top_Factory &operator= (Smart_Diamond_Top_Factory &&) = delete;

  /// Create the smart proxy.
  Diamond::Top_ptr create_proxy (Diamond::Top_ptr proxy) override;
};

class MY_Stub_Export Smart_Diamond_Top_Proxy
  : public virtual Diamond::TAO_Diamond_Top_Smart_Proxy_Base
{
public:
  explicit Smart_Diamond_Top_Proxy (Diamond::Top_ptr proxy);
  Smart_Diamond_Top_Proxy (Smart_Diamond_Top_Proxy const &) = delete;
  Smart_Diamond_Top_Proxy (Smart_Diamond_Top_Proxy &&) = delete;
  Smart_Diamond_Top_Proxy &operator= (Smart_Diamond_Top_Proxy const &) = delete;
  Smart_Diamond_Top_Proxy &operator= (Smart_Diamond_Top_Proxy &&) = delete;

  /// Intercept the shape call and log its result.
  char * shape () override;
};
#endif /*SMART_PROXY_IMPL_H*/
