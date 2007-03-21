// $Id$

#include "Smart_Proxy_Impl.h"
#include "ace/Service_Object.h"
#include "ace/svc_export.h"

class ACE_Svc_Export Smart_Test_Factory
  : public virtual TAO_Test_Default_Proxy_Factory,
    public virtual ACE_Service_Object

{
public:

  Smart_Test_Factory (void);

  // This method will create the smart_proxy.
  virtual Test_ptr create_proxy (Test_ptr proxy);
};

// The following Factory is used by the ACE_Service_Config to
// dynamically initialize the state of the Smart_Proxy_Factory
ACE_SVC_FACTORY_DECLARE (Smart_Test_Factory)
