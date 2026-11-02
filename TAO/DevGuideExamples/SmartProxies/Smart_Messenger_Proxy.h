// Smart_Messenger_Proxy.h


#include "MessengerC.h"
#include "LoggerC.h"

class Smart_Messenger_Proxy_Factory : public TAO_Messenger_Default_Proxy_Factory
{
  public:
    explicit Smart_Messenger_Proxy_Factory (CORBA::ORB_ptr orb);
    Messenger_ptr create_proxy (
      Messenger_ptr proxy) override;

    Smart_Messenger_Proxy_Factory (Smart_Messenger_Proxy_Factory const &) = delete;
    Smart_Messenger_Proxy_Factory (Smart_Messenger_Proxy_Factory &&) = delete;
    Smart_Messenger_Proxy_Factory &operator= (Smart_Messenger_Proxy_Factory const &) = delete;
    Smart_Messenger_Proxy_Factory &operator= (Smart_Messenger_Proxy_Factory &&) = delete;
  private:
    Logger_var logger_;
};


class Smart_Messenger_Proxy : public TAO_Messenger_Smart_Proxy_Base
{
  public:
    Smart_Messenger_Proxy (Messenger_ptr proxy, Logger_ptr logger);
    CORBA::Boolean send_message (
      char const *user_name,
      char const *subject,
      char *&message) override;

    Smart_Messenger_Proxy (Smart_Messenger_Proxy const &) = delete;
    Smart_Messenger_Proxy (Smart_Messenger_Proxy &&) = delete;
    Smart_Messenger_Proxy &operator= (Smart_Messenger_Proxy const &) = delete;
    Smart_Messenger_Proxy &operator= (Smart_Messenger_Proxy &&) = delete;
  private:
    Logger_var logger_;
};

