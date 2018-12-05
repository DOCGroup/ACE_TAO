#define ACE_DOESNT_DEFINE_MAIN

#include "test_config.h"

#include "ace/Init_ACE.h"
#include "ace/Singleton.h"

int restart_test_logging (const ACE_TCHAR *program)
{
  if (ACE_LOG_MSG->open (program, ACE_Log_Msg::OSTREAM |
                         ACE_Log_Msg::VERBOSE_LITE) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("open log_msg failed")), -1);
  if (ace_file_stream::instance()->set_output (program, 1) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("set_output failed")), -1);
  return 0;
}

void report_error (const ACE_TCHAR *lock)
{
  ACE_ERROR ((LM_ERROR, ACE_TEXT ("ACE_Singleton<int, %s> ")
              ACE_TEXT ("failed to allocate\n"), lock));
}

typedef ACE_Singleton<int, ACE_SYNCH_MUTEX> Singleton1;
typedef ACE_Singleton<int, ACE_SYNCH_RECURSIVE_MUTEX> Singleton2;

int main ()
{
  ACE::init ();
  ACE_START_TEST (ACE_TEXT ("Singleton_Restart_Test"));

  *Singleton1::instance () = 1;
  *Singleton2::instance () = 2;

  ACE::fini ();
  ACE::init ();
  if (restart_test_logging (program) != 0) // program defined in ACE_START_TEST
    return -1;

  int *i1 = Singleton1::instance ();
  if (!i1)
    report_error (ACE_TEXT ("ACE_SYNCH_MUTEX"));

  int *i2 = Singleton2::instance ();
  if (!i2)
    report_error (ACE_TEXT ("ACE_SYNCH_RECURSIVE_MUTEX"));

  ACE_END_TEST;
  ACE::fini ();

  return (i1 && i2) ? 0 : 1;
}
