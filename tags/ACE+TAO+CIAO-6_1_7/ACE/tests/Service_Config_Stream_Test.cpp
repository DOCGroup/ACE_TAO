// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This test exercises ACE_Service_Config assembling a stream.
//
// = AUTHOR
//    Steve Huston <shuston@riverace.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/Log_Msg.h"
#include "ace/Module.h"
#include "ace/Service_Config.h"
#include "ace/Task.h"



/*
 * The Decrypter service is static and is configured into a stream
 * dynamically.
 *
 * NOTE! This code is here, and it will trip up while being loaded. If
 * "static Decrypter" is inserted into the .conf file's module list, this
 * test will barf. This is arguably a weird test, but I don't know any
 * reason it should not work; however, this behavior has never been tested
 * and may not have ever worked... but if someone is feeling ambitious,
 * please go ahead and make this work. I'm out of time and energy.
 *    -Steve Huston
 */
class Static_Task : public ACE_Task<ACE_SYNCH>
{
public:
  int open (void *)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("opening %s\n"),
                this->name () ? this->name () : ACE_TEXT ("Static_Task")));
    return 0;
  }

  int close (u_long)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("closing %s\n"),
                this->name () ? this->name () : ACE_TEXT ("Static_Task")));
    return 0;
  }

  int init (int, ACE_TCHAR *[])
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("initializing %s\n"),
                this->name () ? this->name () : ACE_TEXT ("Static_Task")));
    return 0;
  }

  int fini (void)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("finalizing %s\n"),
                this->name () ? this->name () : ACE_TEXT ("Static_Task")));
    return 0;
  }
};

class Decrypter : public ACE_Service_Object, public ACE_Module<ACE_SYNCH, ACE_System_Time_Policy>
{
public:
  Decrypter ()
    : ACE_Module<ACE_SYNCH, ACE_System_Time_Policy> (
        ACE_TEXT ("Decrypter"),
        &writer_, &reader_,
        0,
        M_DELETE_NONE) // Tasks are members; don't delete
  {}

private:
  Static_Task writer_;
  Static_Task reader_;
};

ACE_FACTORY_DEFINE (ACE_Local_Service, Decrypter)
ACE_STATIC_SVC_DEFINE (Decrypter_Descriptor,
                       ACE_TEXT ("Decrypter"),
                       ACE_MODULE_T,
                       &ACE_SVC_NAME (Decrypter),
                       (ACE_Service_Type::DELETE_THIS |
                        ACE_Service_Type::DELETE_OBJ),
                       1)
ACE_STATIC_SVC_REQUIRE (Decrypter_Descriptor)


int
run_main (int, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Service_Config_Stream_Test"));

  ACE_STATIC_SVC_REGISTER (Decrypter);

  // If you want to try the static module in the stream test (comments at
  // the top of this file) it needs the -y in the argv list which enables
  // static services. Otherwise it's not really needed. Same with the
  // 'false' 4th arg to open() below - it allows static services.
  ACE_TCHAR *_argv[5] = { argv[0],
                          const_cast<ACE_TCHAR*> (ACE_TEXT ("-d")),
                          const_cast<ACE_TCHAR*> (ACE_TEXT ("-y")),
                          const_cast<ACE_TCHAR*> (ACE_TEXT ("-f")),
                          const_cast<ACE_TCHAR*>
                            (ACE_TEXT ("Service_Config_Stream_Test.conf")) };
  int status;
  if ((status = ACE_Service_Config::open (5,
                                          _argv,
                                          ACE_DEFAULT_LOGGER_KEY,
                                          false,
                                          true /*ignore def svc.conf*/)) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("open"),
                1));

  ACE_END_TEST;
  return status;
}
