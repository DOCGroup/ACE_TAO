// $Id$
//
#ifndef SMART_PROXY_IMPL_H
#define SMART_PROXY_IMPL_H

#include "DiamondC.h"
#include "stub_export.h"

class MY_Stub_Export Smart_Diamond_Top_Factory : public virtual Diamond::TAO_Diamond_Top_Default_Proxy_Factory
{
public:
  Smart_Diamond_Top_Factory (void);

  virtual Diamond::Top_ptr create_proxy (Diamond::Top_ptr proxy);

  // This method will create the smart_proxy.
};


class MY_Stub_Export Smart_Diamond_Top_Proxy : public virtual Diamond::TAO_Diamond_Top_Smart_Proxy_Base
{
public:
  Smart_Diamond_Top_Proxy (Diamond::Top_ptr proxy);

  virtual char * shape ();
  // Smartify the method call!

  // Dont need to add any extra functionality into shutdown()
  // hence neednt define it.
};
#endif /*SMART_PROXY_IMPL_H*/
