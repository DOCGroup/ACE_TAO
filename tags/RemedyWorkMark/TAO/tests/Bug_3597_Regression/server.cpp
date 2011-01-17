// $Id$

#include "tao/corba.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Msg_Backend.h"
#include "ace/Log_Record.h"

const ACE_TCHAR* mykey = ACE_TEXT("KEY");

class Backend : public ACE_Log_Msg_Backend
{
public:
  Backend ()
    : ok_ (false) {}

  virtual int open (const ACE_TCHAR *logger_key);

  virtual int reset (void);

  virtual int close (void);

  virtual ssize_t log (ACE_Log_Record &log_record);

  bool ok (void) const;

private:
  bool ok_;
};

int
Backend::open (const ACE_TCHAR *key)
{
  if (ACE_OS::strcmp (key, mykey) == 0)
    this->ok_ = true;
  return 0;
}

int
Backend::reset (void)
{
  return 0;
}

int
Backend::close (void)
{
  return 0;
}

ssize_t
Backend::log (ACE_Log_Record &)
{
  return 1;
}

bool
Backend::ok (void) const
{
  return this->ok_;
}

int ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  Backend b;
  ACE_Log_Msg_Backend *old_b = ACE_Log_Msg::msg_backend (&b);

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::CUSTOM);

  ACE_TCHAR *my_argv[3];
  my_argv[0] = argv[0];
  my_argv[1] = const_cast<ACE_TCHAR *> (ACE_TEXT ("-ORBServiceConfigLoggerKey"));
  my_argv[2] = const_cast<ACE_TCHAR *> (mykey);
  int my_argc = 3;

  CORBA::ORB_var orb2_ = CORBA::ORB_init (my_argc, my_argv, "ServerORB1");

  if (!b.ok ())
    {
      ACE_ERROR_RETURN ((LM_ERROR, "ERROR: Key not ok!\n"), 1);
    }

  // Reset the backend to avoid references to our soon-to-be-destroyed one.
  ACE_Log_Msg::msg_backend (old_b);

  return 0;
}
