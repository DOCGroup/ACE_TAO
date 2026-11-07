#include "server_interceptor.h"
#include "tao/OctetSeqC.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Constants.h"
#include <cstring>

constexpr IOP::ServiceId service_id = 0x08154711;
constexpr char request_msg[] = "TEST 0123456789 TEST";
PortableInterceptor::SlotId slot_id {};

char *
Echo_Server_Request_Interceptor::name ()
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Server_Request_Interceptor::destroy ()
{
}

void
Echo_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var const operation = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "%C.receive_request_service_contexts from "
              "\"%C\"\n",
              this->myname_,
              operation.in ()));

  // Ignore the shutdown operation.
  if (std::strcmp ("shutdown", operation.in ()) == 0)
    return;

  // retrieve the context
  IOP::ServiceId const id = ::service_id;
  IOP::ServiceContext_var const sc =
    ri->get_request_service_context (id);

  char const *buf =
    reinterpret_cast<char const *> (sc->context_data.get_buffer ());
  ACE_DEBUG ((LM_DEBUG,
              "  Received service context: %C\n",
              buf));

  if (std::strcmp (buf, request_msg) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Echo_Server_Request_Interceptor::receive_request_service_contexts: "
                  "Expected request service context to be: %C\n",
                  request_msg));
    }
  else
    {
      // Put the request context into the slot for the servant to inspect.
      CORBA::Any data;
      data <<= buf;

      try
        {
          ri->set_slot (slot_id, data);
          ACE_DEBUG ((LM_DEBUG,
                      "receive_request_service_contexts filled Slot %d\n",
                      static_cast<int> (slot_id)));
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception (
            "Exception thrown in receive_request_service_contexts()\n");
          throw CORBA::INTERNAL ();
        }

    }
}


void
Echo_Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}

void
Echo_Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}

void
Echo_Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}

void
Echo_Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}
