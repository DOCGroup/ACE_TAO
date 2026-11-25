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
    ssize_t recv (char *buffer, size_t length, ACE_Time_Value *timeout) override
    {
      return this->recv (buffer, length, 0, timeout);
    }
    ssize_t recv (iovec *, int, ACE_Time_Value *) override { return -1; }

    ssize_t recv (char *buffer, size_t length, int flags, ACE_Time_Value *) override
    {
      if ((flags != 0 && flags != MSG_PEEK) || size_ > static_cast<ssize_t> (length))
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

  TAO_SFP_Base sfp;
  flowProtocol::StartReply original {};
  std::memcpy (original.magic_number, "=STR", TAO_SFP_MAGIC_NUMBER_LEN);
  original.flags = 1;
  TAO_OutputCDR output;
  if (!(output << original))
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot serialize start reply\n")), 1);
    }

  Test_Transport reply_transport (output.buffer (), static_cast<ssize_t> (output.total_length ()));
  TAO_InputCDR input (size_t {});
  flowProtocol::StartReply decoded {};
  if (TAO_SFP_Base::read_start_reply_message (std::addressof (reply_transport), decoded, input) != 0
      || std::memcmp (decoded.magic_number, original.magic_number, TAO_SFP_MAGIC_NUMBER_LEN) != 0
      || decoded.flags != original.flags)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot decode start reply\n")), 1);
    }

  Test_Transport short_reply (output.buffer (), static_cast<ssize_t> (output.total_length () - 1));
  TAO_InputCDR short_input (size_t {});
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::SILENT);
  int const short_result = TAO_SFP_Base::read_start_reply_message (
    std::addressof (short_reply), decoded, short_input);
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::SILENT);
  if (short_result != -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Incomplete start reply was accepted\n")), 1);
    }
  return 0;
}
