#include "tao/corba.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Msg_Backend.h"
#include "ace/Log_Record.h"
#include <array>
#include <string>

ACE_TCHAR const mykey[] = ACE_TEXT ("KEY");

class Backend : public ACE_Log_Msg_Backend
{
public:
  Backend () = default;
  Backend (Backend const &) = delete;
  Backend (Backend &&) = delete;
  Backend &operator= (Backend const &) = delete;
  Backend &operator= (Backend &&) = delete;

  int open (ACE_TCHAR const *logger_key) override;

  int reset () override;

  int close () override;

  ssize_t log (ACE_Log_Record &log_record) override;

  bool ok () const;

private:
  bool ok_ {};
};

int
Backend::open (ACE_TCHAR const *key)
{
  if (ACE_OS::strcmp (key, mykey) == 0)
    this->ok_ = true;
  return 0;
}

int
Backend::reset ()
{
  return 0;
}

int
Backend::close ()
{
  return 0;
}

ssize_t
Backend::log (ACE_Log_Record &)
{
  return 1;
}

bool
Backend::ok () const
{
  return this->ok_;
}

int ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  Backend b;
  ACE_Log_Msg_Backend *const old_backend = ACE_Log_Msg::msg_backend (&b);

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::CUSTOM);

  std::array<std::basic_string<ACE_TCHAR>, 2> orb_options {
    ACE_TEXT ("-ORBServiceConfigLoggerKey"), mykey
  };
  std::array<ACE_TCHAR *, 4> orb_argv {argv[0], orb_options[0].data (), orb_options[1].data (), nullptr};
  int orb_argc {3};

  CORBA::ORB_var const orb = CORBA::ORB_init (orb_argc, orb_argv.data (), "ServerORB1");

  if (!b.ok ())
    {
      ACE_ERROR_RETURN ((LM_ERROR, "ERROR: Key not ok!\n"), 1);
    }

  // Reset the backend to avoid references to our soon-to-be-destroyed one.
  ACE_Log_Msg::msg_backend (old_backend);

  return 0;
}
