#include "ace/Event_Handler_T.h"
#include "ace/Log_Msg.h"

#include <type_traits>

static int destruction_count {};

class ACE_Test_Sig_Handler
{
public:
  ACE_Test_Sig_Handler () = default;
  virtual ~ACE_Test_Sig_Handler () { ++destruction_count; }
  virtual ACE_HANDLE get_handle () const { return 0; }
  virtual void set_handle (ACE_HANDLE) {}
  virtual int handle_async_io (ACE_HANDLE) { return 0; }

  //FUZZ: disable check_for_lack_ACE_OS
  virtual int shutdown (ACE_HANDLE, ACE_Reactor_Mask) { return 0; }
  //FUZZ: enable check_for_lack_ACE_OS

  virtual int signal_handler (int /* signum */, siginfo_t * = nullptr, ucontext_t * = nullptr)
  {
    return 0;
  }
};

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  using EH_SH = ACE_Event_Handler_T<ACE_Test_Sig_Handler>;
  static_assert (!std::is_copy_constructible<EH_SH>::value);
  static_assert (!std::is_copy_assignable<EH_SH>::value);
  static_assert (!std::is_move_constructible<EH_SH>::value);
  static_assert (!std::is_move_assignable<EH_SH>::value);

  {
    ACE_Test_Sig_Handler handler;
    {
      // Tie the ACE_Event_Handler_T together with the methods from ACE_Test_Sig_Handler.
      EH_SH tied_sh (&handler, false,
                     &ACE_Test_Sig_Handler::get_handle,
                     &ACE_Test_Sig_Handler::handle_async_io,
                     &ACE_Test_Sig_Handler::shutdown,
                     &ACE_Test_Sig_Handler::signal_handler);
    }
    if (destruction_count != 0)
      return 1;
  }
  if (destruction_count != 1)
    return 1;

  {
    EH_SH owned_sh (new ACE_Test_Sig_Handler, true);
    owned_sh.op_handler (owned_sh.op_handler ());
    if (destruction_count != 1)
      return 1;
    owned_sh.op_handler (new ACE_Test_Sig_Handler);
    if (destruction_count != 2)
      return 1;
  }
  if (destruction_count != 3)
    return 1;

  return 0;
}
