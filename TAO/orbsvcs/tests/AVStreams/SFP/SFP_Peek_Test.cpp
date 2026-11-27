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
    Test_Transport (char const *data, ssize_t size, int flags = MSG_PEEK)
      : data_ (data), size_ (size), flags_ (flags) {}

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
      this->received_ = true;
      if (flags != flags_ || size_ > static_cast<ssize_t> (length))
        {
          return -1;
        }
      if (size_ > 0)
        {
          std::memcpy (buffer, data_, static_cast<size_t> (size_));
        }
      return size_;
    }

    bool received () const { return this->received_; }

  private:
    char const *data_;
    ssize_t size_;
    int flags_;
    bool received_ {};
  };

  template <typename T>
  bool check_message (T const &original, T &decoded,
                      int (*reader) (TAO_AV_Transport *, T &, TAO_InputCDR &), int flags)
  {
    TAO_OutputCDR output;
    if (!(output << original))
      {
        return false;
      }
    Test_Transport complete (output.buffer (), static_cast<ssize_t> (output.total_length ()), flags);
    TAO_InputCDR input (size_t {});
    if (reader (std::addressof (complete), decoded, input) != 0)
      {
        return false;
      }

    Test_Transport truncated (output.buffer (), static_cast<ssize_t> (output.total_length () - 1), flags);
    TAO_InputCDR short_input (size_t {});
    ACE_LOG_MSG->set_flags (ACE_Log_Msg::SILENT);
    int const result = reader (std::addressof (truncated), decoded, short_input);
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::SILENT);
    return result == -1;
  }
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

  Test_Transport reply_transport (output.buffer (), static_cast<ssize_t> (output.total_length ()), 0);
  TAO_InputCDR input (size_t {});
  flowProtocol::StartReply decoded {};
  if (TAO_SFP_Base::read_start_reply_message (std::addressof (reply_transport), decoded, input) != 0
      || std::memcmp (decoded.magic_number, original.magic_number, TAO_SFP_MAGIC_NUMBER_LEN) != 0
      || decoded.flags != original.flags)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot decode start reply\n")), 1);
    }

  Test_Transport short_reply (output.buffer (), static_cast<ssize_t> (output.total_length () - 1), 0);
  TAO_InputCDR short_input (size_t {});
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::SILENT);
  int const short_result = TAO_SFP_Base::read_start_reply_message (
    std::addressof (short_reply), decoded, short_input);
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::SILENT);
  if (short_result != -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Incomplete start reply was accepted\n")), 1);
    }

  flowProtocol::Start start {};
  flowProtocol::Start decoded_start {};
  start.major_version = 1;
  flowProtocol::credit credit {};
  flowProtocol::credit decoded_credit {};
  credit.cred_num = 42;
  flowProtocol::frameHeader header {};
  flowProtocol::frameHeader decoded_header {};
  header.message_size = 123;
  flowProtocol::fragment fragment {};
  flowProtocol::fragment decoded_fragment {};
  fragment.frag_number = 17;
  if (!check_message (start, decoded_start, TAO_SFP_Base::read_start_message, 0)
      || decoded_start.major_version != start.major_version
      || !check_message (credit, decoded_credit, TAO_SFP_Base::read_credit_message, 0)
      || decoded_credit.cred_num != credit.cred_num
      || !check_message (header, decoded_header, TAO_SFP_Base::read_endofstream_message, 0)
      || decoded_header.message_size != header.message_size
      || !check_message (header, decoded_header, TAO_SFP_Base::peek_frame_header, MSG_PEEK)
      || decoded_header.message_size != header.message_size
      || !check_message (fragment, decoded_fragment, TAO_SFP_Base::peek_fragment_header, MSG_PEEK)
      || decoded_fragment.frag_number != fragment.frag_number)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("SFP message reader failed\n")), 1);
    }

  char data[64] {};
  TAO_SFP_Frame_State state;
  if (state.static_frame_.size (sizeof (data)) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot allocate frame buffer\n")), 1);
    }
  TAO_AV_frame_info *frame_info {};
  header.flags = 0;
  header.message_type = flowProtocol::SimpleFrame_Msg;
  header.message_size = TAO_SFP_Base::frame_header_len + 1;
  data[TAO_SFP_Base::frame_header_len] = 'x';
  Test_Transport frame_transport (data, header.message_size, 0);
  if (TAO_SFP_Base::read_frame (std::addressof (frame_transport), header, state, frame_info) != 0
      || state.frame_block_ != std::addressof (state.static_frame_)
      || state.frame_block_->length () != 1 || *state.frame_block_->rd_ptr () != 'x')
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Complete frame was not decoded\n")), 1);
    }

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::SILENT);
  header.message_size = TAO_SFP_Base::frame_header_len - 1;
  Test_Transport too_small (data, sizeof (data), 0);
  int const small_result = TAO_SFP_Base::read_frame (std::addressof (too_small), header, state, frame_info);
  header.message_size = sizeof (data) + 1;
  Test_Transport too_large (data, sizeof (data), 0);
  int const large_result = TAO_SFP_Base::read_frame (std::addressof (too_large), header, state, frame_info);
  header.message_size = TAO_SFP_Base::frame_header_len + 1;
  Test_Transport short_frame (data, header.message_size - 1, 0);
  int const short_frame_result = TAO_SFP_Base::read_frame (
    std::addressof (short_frame), header, state, frame_info);
  fragment.frag_sz = TAO_SFP_Base::fragment_len - 1;
  Test_Transport invalid_fragment (data, sizeof (data), 0);
  int const invalid_fragment_result = TAO_SFP_Base::read_fragment (
    std::addressof (invalid_fragment), fragment, state, frame_info);
  fragment.frag_sz = TAO_SFP_Base::fragment_len + 1;
  Test_Transport short_fragment (data, fragment.frag_sz - 1, 0);
  int const short_fragment_result = TAO_SFP_Base::read_fragment (
    std::addressof (short_fragment), fragment, state, frame_info);
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::SILENT);
  if (small_result != -1 || too_small.received ()
      || large_result != -1 || too_large.received ()
      || short_frame_result != -1 || invalid_fragment_result != -1 || invalid_fragment.received ()
      || short_fragment_result != -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Invalid or truncated SFP data was accepted\n")), 1);
    }
  return 0;
}
