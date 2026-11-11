#include "ServerInterceptor.h"
#include "tao/PI_Server/ServerRequestInfoA.h"

#include <array>
#include <cstring>
#include <iostream>

constexpr IOP::ServiceId service_id = 0xdeed;
constexpr std::array<char const *, 4> allowed_users {
  "Ron Klein", "Scott Case", "Mark Hodge", "Greg Black"
};
constexpr char restricted_interface[] = "IDL:Messenger:1.0";

ServerInterceptor::ServerInterceptor ()
  : myname_ ("Server_Authentication_Interceptor")
{
  std::cout << "Calling ServerInterceptor constructor." << std::endl;
}

char *
ServerInterceptor::name ()
{
  std::cout << "Calling ServerInterceptor name() method" << std::endl;
  return CORBA::string_dup (this->myname_);
}

void
ServerInterceptor::destroy ()
{
  std::cout << "Calling destroy()." << std::endl;
}

void
ServerInterceptor::receive_request_service_contexts (
  PortableInterceptor::ServerRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG (ri);
  std::cout << "Calling receive_request_service_contexts()." << std::endl;
}

void
ServerInterceptor::receive_request (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  bool permission_granted {};
  std::cout << "Calling receive_request()." << std::endl;

  if (ri->target_is_a (restricted_interface))
    {
      IOP::ServiceContext_var const service_context =
        ri->get_request_service_context (service_id);
      char const * const user_name = reinterpret_cast<char const *> (
        service_context->context_data.get_buffer ());

      for (char const * const allowed_user : allowed_users)
        {
          if (std::strcmp (user_name, allowed_user) == 0)
            {
              permission_granted = true;
              break;
            }
        }
    }

  if (permission_granted)
    std::cout << "Permission Granted " << std::endl;
  else
    std::cout << "Permission Denied " << std::endl;
}

void
ServerInterceptor::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG (ri);
  std::cout << "Calling send_reply()." << std::endl;
}

void
ServerInterceptor::send_exception (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG (ri);
  std::cout << "Calling send_exception()." << std::endl;
}

void
ServerInterceptor::send_other (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG (ri);
  std::cout << "Calling send_other()." << std::endl;
}
