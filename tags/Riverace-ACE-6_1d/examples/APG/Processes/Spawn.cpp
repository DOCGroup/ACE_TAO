// $Id$

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_pwd.h"
#include "ace/os_include/os_pwd.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_pwd.h"
#include "ace/Process.h"
#include "ace/Log_Msg.h"

// Listing 1 code/ch10
class Manager : public ACE_Process
{
public:
  Manager (const ACE_TCHAR* program_name)
  {
    ACE_TRACE ("Manager::Manager");
    ACE_OS::strcpy (programName_, program_name);
  }

  int doWork (void)
  {
    ACE_TRACE ("Manager::doWork");

    // Spawn the new process; prepare() hook is called first.
    ACE_Process_Options options;
    pid_t pid = this->spawn (options);
    if (pid == ACE_INVALID_PID)
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT ("%p\n"),
                        ACE_TEXT ("spawn")), -1);

    // Wait forever for my child to exit.
    if (this->wait () == -1)
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                         ACE_TEXT ("wait")), -1);

    // Dump whatever happened.
    this->dumpRun ();
    return 0;
  }
// Listing 1

private:
  // Listing 3 code/ch10
  int dumpRun (void)
  {
    ACE_TRACE ("Manager::dumpRun");

    if (ACE_OS::lseek (this->outputfd_, 0, SEEK_SET) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                         ACE_TEXT ("lseek")), -1);

    char buf[1024];
    ssize_t length = 0;

    // Read the contents of the error stream written
    // by the child and print it out.
    while ((length = ACE_OS::read (this->outputfd_,
                                   buf, sizeof(buf)-1)) > 0)
      {
        buf[length] = 0;
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%C\n"), buf));
      }

    ACE_OS::close (this->outputfd_);
    return 0;
  }
 // Listing 3

 // Listing 2 code/ch10
  // prepare() is inherited from ACE_Process.
  int prepare (ACE_Process_Options &options)
  {
    ACE_TRACE ("Manager::prepare");

    options.command_line (ACE_TEXT ("%s 1"), this->programName_);
    if (this->setStdHandles (options) == -1 ||
        this->setEnvVariable (options) == -1)
      return -1;
#if !defined (ACE_WIN32) && !defined (ACE_LACKS_PWD_FUNCTIONS)
    return this->setUserID (options);
#else
    return 0;
#endif
  }

  int setStdHandles (ACE_Process_Options &options)
  {
    ACE_TRACE ("Manager::setStdHandles");

    ACE_OS::unlink ("output.dat");
    this->outputfd_ =
      ACE_OS::open ("output.dat", O_RDWR | O_CREAT);
    return options.set_handles
      (ACE_STDIN, ACE_STDOUT, this->outputfd_);
  }

  int setEnvVariable (ACE_Process_Options &options)
  {
    ACE_TRACE ("Manager::setEnvVariables");
    return options.setenv
      (ACE_TEXT ("PRIVATE_VAR=/that/seems/to/be/it"));
  }
 // Listing 2

#if !defined (ACE_LACKS_PWD_FUNCTIONS)
 // Listing 10 code/ch10
  int setUserID (ACE_Process_Options &options)
  {
    ACE_TRACE ("Manager::setUserID");
    passwd* pw = ACE_OS::getpwnam ("nobody");
    if (pw == 0)
      return -1;
    options.seteuid (pw->pw_uid);
    return 0;
  }
 // Listing 10
#endif /* !ACE_LACKS_PWD_FUNCTIONS */

private:
  ACE_HANDLE outputfd_;
  ACE_TCHAR programName_[256];
};

// Listing 4 code/ch10
class Slave
{
public:
  Slave ()
  {
    ACE_TRACE ("Slave::Slave");
  }

  int doWork (void)
  {
    ACE_TRACE ("Slave::doWork");

    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("(%P) started at %T, parent is %d\n"),
                ACE_OS::getppid ()));
    this->showWho ();
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("(%P) the private environment is %s\n"),
                ACE_OS::getenv ("PRIVATE_VAR")));

    ACE_TCHAR str[128];
    ACE_OS::sprintf (str, ACE_TEXT ("(%d) Enter your command\n"),
                     static_cast<int>(ACE_OS::getpid ()));
    ACE_OS::write (ACE_STDOUT, str, ACE_OS::strlen (str));
    this->readLine (str);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P) Executed: %C\n"),
                str));
    return 0;
  }
// Listing 4

  void showWho (void)
  {
    ACE_TRACE ("Slave::showWho");
#if !defined (ACE_LACKS_PWD_FUNCTIONS)
    passwd *pw = ::getpwuid (ACE_OS::geteuid ());
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("(%P) Running this process as:%s\n"),
                pw->pw_name));
#endif
  }

  ACE_TCHAR* readLine (ACE_TCHAR* str)
  {
    ACE_TRACE ("Slave::readLine");

    int i = 0;
    while (true)
      {
        ssize_t retval = ACE_OS::read (ACE_STDIN, &str[i], 1);
        if (retval > 0)
          {
            if (str[i] == '\n')
              {
                str[++i] = 0;
                return str;
              }
            i++;
          }
        else
          return str;
      }
  }
};

// Listing 0 code/ch10
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc > 1)   // Slave mode
    {
      Slave s;
      return s.doWork ();
    }

  // Else, Master mode
  Manager m (argv[0]);
  return m.doWork ();
}
// Listing 0
