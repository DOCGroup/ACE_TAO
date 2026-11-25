#include "orbsvcs/AV/sfp.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_sys_socket.h"

#include <cstring>
#include <memory>

namespace
{
  class Test_Transport : public TAO_AV_Transport
  {
  public:
    Test_Transport (char const *data, ssize_t size)
      : data_ (data), size_ (size) {}

    int open (ACE_Addr *) override { return -1; }
    int close () override { return 0; }
    int mtu () override { return 0; }
    ACE_Addr *get_peer_addr () override { return nullptr; }
    ssize_t send (ACE_Message_Block const *, ACE_Time_Value *) override { return -1; }
    ssize_t send (char const *, size_t, ACE_Time_Value *) override { return -1; }
    ssize_t send (iovec const *, int, ACE_Time_Value *) override { return -1; }
    ssize_t recv (char *, size_t, ACE_Time_Value *) override { return -1; }
    ssize_t recv (iovec *, int, ACE_Time_Value *) override { return -1; }

    ssize_t recv (char *buffer, size_t length, int flags, ACE_Time_Value *) override
    {
      if (flags != MSG_PEEK || size_ > static_cast<ssize_t> (length))
        {
          return -1;
        }
      if (size_ > 0)
        {
          std::memcpy (buffer, data_, static_cast<size_t> (size_));
        }
      return size_;
    }

  private:
    char const *data_;
    ssize_t size_;
  };
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  char const frame[] = {'=', 'S', 'F', 'P', 0, static_cast<char> (flowProtocol::Frame_Msg)};
  flowProtocol::MsgType type = flowProtocol::Start_Msg;
  Test_Transport complete (frame, sizeof (frame));
  if (TAO_SFP_Base::peek_message_type (std::addressof (complete), type) != 0
      || type != flowProtocol::Frame_Msg)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Complete SFP header was rejected\n")), 1);
    }

  char const reply[] = {'=', 'S', 'T', 'R', 0};
  Test_Transport complete_reply (reply, sizeof (reply));
  if (TAO_SFP_Base::peek_message_type (std::addressof (complete_reply), type) != 0
      || type != flowProtocol::StartReply_Msg)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Complete start reply was rejected\n")), 1);
    }

  // Truncated headers are expected to fail; suppress their error messages.
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::SILENT);
  bool accepted_short {};
  for (ssize_t size = -1; size < static_cast<ssize_t> (sizeof (frame)); ++size)
    {
      Test_Transport short_header (frame, size);
      type = flowProtocol::Credit_Msg;
      if (TAO_SFP_Base::peek_message_type (std::addressof (short_header), type) != -1
          || type != flowProtocol::Credit_Msg)
        {
          accepted_short = true;
          break;
        }
    }
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::SILENT);
  if (accepted_short)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Incomplete SFP header was accepted\n")), 1);
    }
  return 0;
}
