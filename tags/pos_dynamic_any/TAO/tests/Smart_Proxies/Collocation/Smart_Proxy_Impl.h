// $Id$

#define ACE_BUILD_SVC_DLL

#include "DiamondC.h"

class ACE_Svc_Export Smart_Diamond_Top_Factory : public virtual Diamond::TAO_Diamond_Top_Default_Proxy_Factory
{
 public:

  Smart_Diamond_Top_Factory (void);
  
  virtual Diamond::Top_ptr create_proxy (Diamond::Top_ptr proxy,
										 CORBA::Environment &env = 
										 TAO_default_environment ()
										 );
  // This method will create the smart_proxy.   
};


class ACE_Svc_Export Smart_Diamond_Top_Proxy : public virtual Diamond::TAO_Diamond_Top_Smart_Proxy_Base 
{
 public:

  Smart_Diamond_Top_Proxy (Diamond::Top_ptr proxy);

  virtual char * shape (
                       CORBA::Environment &ACE_TRY_ENV = 
                       TAO_default_environment ()
                       )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  // Smartify the method call!

  // Dont need to add any extra functionality into shutdown()
  // hence neednt define it.
};
