// $Id$

// Smart_Messenger_Proxy.h


#include "MessengerC.h"
#include "LoggerC.h"

class Smart_Messenger_Proxy_Factory : public TAO_Messenger_Default_Proxy_Factory
{
  public:
    Smart_Messenger_Proxy_Factory(CORBA::ORB_ptr orb);
    virtual Messenger_ptr create_proxy (
      Messenger_ptr proxy);
  private:
    Logger_var logger_;
};


class Smart_Messenger_Proxy : public TAO_Messenger_Smart_Proxy_Base
{
  public:
    Smart_Messenger_Proxy(Messenger_ptr proxy, Logger_ptr logger);
    virtual CORBA::Boolean send_message(
      const char * user_name,
      const char * subject,
      char *& message);
  private:
    Logger_var logger_;
};

