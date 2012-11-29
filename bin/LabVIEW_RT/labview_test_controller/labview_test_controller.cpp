// $Id$
//
// Defines the entry point for the LabVIEW RT test controller DLL application.
// This DLL is loaded at system boot by LabVIEW RT. The controller waits for
// TCP connections from the ACE+TAO test scripts. The test scripts will direct
// operation of the tests via commands sent over TCP. In order to be ready for
// connections without intervention via VI, the initial load will spawn a
// thread that sets up the listening socket.

#include "stdafx.h"
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <memory.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <Winsock2.h>

// NULL is the documented way to check DLL handles, and this is plain
// Windows code, not ACE, so we stick to the Microsoft way...
// FUZZ: disable check_for_NULL

// This is plain Windows code, not ACE. Therefore we disable
// the check for ACE_OS
// FUZZ: disable check_for_lack_ACE_OS

// TEST_FUNC is the prototype for the called test's main entrypoint. It's
// the normal C main.
typedef int (*TEST_FUNC) (int argc, char *argv[]);

// Thread entrypoints
static unsigned int __stdcall test_control (void *param);
static unsigned int __stdcall peer_svc (void *peer_p);
static unsigned int __stdcall run_test (void *test_p);

static const char *format_errmsg (unsigned int errcode, const char *prefix);

// Logging information
static const char *LogName = "acetao.log";
static HANDLE logf = INVALID_HANDLE_VALUE;

BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                      )
{
  if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
      return (0 != _beginthreadex (0,            // security
                                   8 * 1024,     // stack size
                                   test_control, // entrypoint
                                   0,            // param
                                   0,            // creation flags
                                   0));          // ptr to thread id
    }

  return TRUE;
}

class Test
{
public:
  Test () : dll_handle_ (NULL),
            thr_handle_ (0),
            entry_ (0),
            running_ (false),
            status_ (-1)
    {}
  ~Test ();

  HANDLE handle (void) { return this->thr_handle_; }
  int run (void);
  const char *start (const char *name);
  bool status (int *exit_status);
  int wait (void);
  void kill (void);

  // Clean up remnants of a test run.
  void cleanup (void);

private:
  HMODULE dll_handle_;
  HANDLE thr_handle_;
  TEST_FUNC entry_;
  bool running_;
  int status_;
  enum { CMDLINE_LEN = 1024, ARGV_SIZE = 100 };
  char name_[CMDLINE_LEN];
  char cmdline_[CMDLINE_LEN];
  int argc_;
  char *argv_[ARGV_SIZE];
};

class Peer
{
public:
  Peer (SOCKET h) : handle_ (h) {}

  // Run the Peer's session; intended to be called from a new thread devoted
  // to this peer's session.
  int svc (void);

private:
  Peer () {};

  // Process command input from socket.
  int command (void);

  // Send a reply string to the peer.
  int reply (const char *msg);

  SOCKET handle_;
  Test test_;
};

// Run a peer session; assume there's a thread for each session, so this
// object has all it needs for context located in 'this' object, and it can
// block at any point as long as one remembers that there is one or more
// test threads running and some attention must be paid to the encapsulated
// socket handle over which this object receives control commands from the
// host test driver.
int
Peer::svc (void)
{
  // Read commands until EOF (peer closed) or protocol error
  while (0 == this->command ())
    ;
  closesocket (this->handle_);
  this->handle_ = INVALID_SOCKET;
  return 0;
}

int
Peer::command (void)
{
  // The protocol exchanges with the peer are execpted to be lock-step
  // request-reply command lines, so we can make assumptions about a complete
  // line being available to make life easier.
  const int MAX_RECV = 1024;
  char line[MAX_RECV], *p;
  p = &line[0];
  int count = 0, len = 0;
  while ((count = recv (this->handle_, p, MAX_RECV - len, 0)) > 0)
    {
      p[count] = '\0';
      len += count;
      p += count;
      char *nl;
      if ((nl = strchr (line, '\n')) == 0)
        continue;

      // At this point we have a 0-terminated string with a newline ending
      // the command line. Break out and process the command.
      break;
    }
  if (count <= 0)
    return -1;         // Relay closed/error socket to caller

  char *cmd = strtok (line, "\t \n\r");
  if (cmd == 0)
    {
      char err[1024];
      sprintf (err, "Can't parse input: %s\n", line);
      this->reply (err);
      return -1;
    }
  // Which command is it? These commands are known:
  //
  //   run <test-dll> [args]
  //     Run test in the named test-dll; respond with "OK" or an error string.
  //   status
  //     If test still running return "RUNNING" else return exit status.
  //   wait
  //     Wait for test to exit; return "OK"
  //   kill
  //     Kill the thread with the most recent test; return "OK".
  //   snaplog
  //     Take a snapshot of the current stdout/stderr log to a new file
  //     name and reset the stdout/stderr log.
  if (strcmp ("run", cmd) == 0)
    {
      char *test = strtok (0, "\t \n\r");
      if (test == 0)
        {
          this->reply ("Malformed run command\n");
          return -1;
        }
      // start() pulls apart the rest of the command line...
      const char *errmsg = this->test_.start (test);
      if (errmsg == 0)
        this->reply ("OK\n");
      else
        this->reply (errmsg);
    }
  else if (strcmp ("status", cmd) == 0)
    {
      int status;
      if (this->test_.status (&status))
        {
          char retvalmsg[64];
          sprintf (retvalmsg, "%d\n", status);
          this->reply (retvalmsg);
        }
      else
        this->reply ("RUNNING\n");
    }
  else if (strcmp ("wait", cmd) == 0)
    {
      int status = this->test_.wait ();
      char retvalmsg[64];
      sprintf (retvalmsg, "%d\n", status);
      this->reply (retvalmsg);
    }
  else if (strcmp ("kill", cmd) == 0)
    {
      // Killing things is bad... say we can't and the host should reboot us.
      this->reply ("NO - please reboot me\n");
    }
  else if (strcmp ("waitforfile", cmd) == 0)
    {
      char *name = strtok (0, "\t \n\r");
      if (name == 0)
        {
          this->reply ("Malformed waitforfile command\n");
          return -1;
        }
      char *secs_s = strtok (0, "\t \n\r");
      int secs = 0;
      if (secs_s == 0 || (secs = atoi (secs_s)) <= 0)
        {
          this->reply ("Malformed waitforfile command\n");
          return -1;
        }
      struct _stat info;
      const char *msg = 0;
      bool found = false;
      while (secs > 0)
        {
          if (_stat (name, &info) == -1)   // No file yet
            {
              if (errno != ENOENT)
                {
                  // Something more serious than no file yet; bail out.
                  msg = format_errmsg (errno, name);
                  break;
                }
            }
          else
            {
              if (info.st_size > 0)
                {
                  found = true;
                  break;
                }
            }
          // Either no file yet, or it's there but with no content yet.
          Sleep (1 * 1000);     // arg is in msec
          --secs;
        }
      if (found)
        this->reply ("OK\n");
      else if (secs == 0)
        this->reply ("TIMEOUT\n");
      else
        this->reply (msg);
    }
  else if (strcmp ("snaplog", cmd) == 0)
    {
      if (logf == INVALID_HANDLE_VALUE)
        {
          this->reply ("NONE\n");
        }
      else
        {
          CloseHandle (logf);
          if (0 == rename (LogName, "snapshot.txt"))
            {
              char abspath[1024];
              if (_fullpath (abspath, "snapshot.txt", 1024))
                {
                  strcat (abspath, "\n");
                  this->reply (abspath);
                }
              else
                {
                  // Last ditch effort to get a name back to the client
                  this->reply ("\\ni-rt\\system\\snapshot.txt\n");
                }
            }
          else
            {
              this->reply ("NONE\n");
            }
          // Reset stdout/stderr to a new file
          logf = CreateFile (LogName,
                             FILE_ALL_ACCESS,
                             FILE_SHARE_READ,
                             0,    // security
                             CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL,
                             0);
          SetStdHandle (STD_OUTPUT_HANDLE, logf);
          SetStdHandle (STD_ERROR_HANDLE, logf);
        }
    }
  else
    {
      this->reply ("Unrecognized command\n");
      return -1;
    }
  return 0;
}

int
Peer::reply (const char *msg)
{
  int len = (int)strlen (msg);    // size_t -> int
  return send (this->handle_, msg, len, 0) > 0 ? 0 : -1;
}

Test::~Test ()
{
  this->cleanup ();
}

int
Test::run (void)
{
  this->running_ = true;
  try
    {
      this->status_ = (this->entry_) (this->argc_, this->argv_);
    }
  catch (...)
    {
      // Try to note this exception then save the log file before bailing out.
      DWORD bl;
      char msg[256];
      sprintf (msg, "Exception in %s caught by labview_test_controller\n",
               this->name_);
      WriteFile (logf, msg, (DWORD)strlen(msg), &bl, 0);
      FlushFileBuffers (logf);
      CloseHandle (logf);
      throw;
    }
  this->running_ = false;
  // It's possible to cleanup() here; however, that would introduce a race
  // with start() following beginthreadex(). So do all the cleanup on user
  // action - either getting status, waiting, killing, or running another
  // test. Or, terminating the connection.
  return 0;
}

const char *
Test::start (const char *name)
{
  if (this->running_)
    return "Already running\n";

  const char *msg = 0;

  // Reset test status to not inadvertantly report a previous test.
  this->status_ = -1;
  this->cleanup ();     // Resets cmdline_, argc_, argv_

  // The command line is part-way through being tokenized by strtok(). It
  // left off after the program name. Anything remaining are the command
  // line arguments for the program. Pick off whatever is there, copy it
  // to the cmdline_ array and fill in argc_/argv_ for the eventual run.
  strcpy (this->name_, name);
  this->argv_[0] = this->name_;
  this->argc_ = 1;
  size_t cmdchars = 0;
  for (char *token = strtok (0, "\t \n\r");
       token != 0 && (cmdchars + strlen (token) + 1) < CMDLINE_LEN;
       token = strtok (0, "\t \n\r"))
    {
      // We have a new token and it will fit in cmdline_. Copy it to the
      // next spot in cmdline_, add it to argv_/argc_ then update cmdchars
      // to account for the copied-in token and its nul terminator.
      strcpy (&this->cmdline_[cmdchars], token);
      this->argv_[this->argc_] = &this->cmdline_[cmdchars];
      ++this->argc_;
      cmdchars += (strlen (token) + 1);
    }
  char libspec[1024];
  sprintf (libspec, "%s.dll", name);
  if ((this->dll_handle_ = LoadLibrary (libspec)) == NULL)
    return format_errmsg (GetLastError (), libspec);

  this->entry_ = (TEST_FUNC) GetProcAddress (this->dll_handle_, "main");
  if (this->entry_ == NULL)
    {
      msg = format_errmsg (GetLastError (), "main");
      this->cleanup ();
      return msg;
    }
  else
    {
      unsigned int thread_id; /* unused */
      uintptr_t h = _beginthreadex (0,             // security
                                    1024 * 1024,   // stack size
                                    run_test,      // entrypoint
                                    (void *)this,  // arglist
                                    0,             // initflag
                                    &thread_id);   // thread ID
      this->thr_handle_ = (HANDLE) h;
      if (h == 0)         // Test thread may have access to thr_handle_
        {
          msg = format_errmsg (GetLastError (), "spawn");
          this->cleanup ();
          return msg;
        }
    }
  return 0;
}

bool
Test::status (int *exit_status)
{
  if (this->running_)
    return false;   // still running

  *exit_status = this->status_;
  this->cleanup ();
  return true;
}

int
Test::wait (void)
{
  WaitForSingleObject (this->thr_handle_, INFINITE);
  if (!this->running_)
    this->cleanup ();
  return this->status_;
}

void
Test::kill (void)
{
  TerminateThread (this->thr_handle_, -1);
  this->cleanup ();
  this->running_ = false;
  this->status_  = -1;
}

// Clean up remnants of a test run.
void
Test::cleanup (void)
{
  if (this->dll_handle_ != NULL)
    {
      FreeLibrary (this->dll_handle_);
      this->dll_handle_ = NULL;
    }
  if (this->thr_handle_ != 0)
    {
      CloseHandle (this->thr_handle_);
      this->thr_handle_ = 0;
    }
  this->entry_   = 0;
  this->argc_ = 0;
  for (int i = 0; i < ARGV_SIZE; ++i)
    this->argv_[i] = 0;
  memset (this->cmdline_, 0, CMDLINE_LEN);
}

static unsigned int __stdcall
test_control (void * /* param */)
{
  // cd to ace dir?? (can this be an env variable?)

  // redirect stdout/stderr to a file
  logf = CreateFile (LogName,
                     FILE_ALL_ACCESS,
                     FILE_SHARE_READ,
                     0,                  // security
                     OPEN_ALWAYS,        // Don't crush a previous one
                     FILE_ATTRIBUTE_NORMAL,
                     0);
  if (logf == INVALID_HANDLE_VALUE)
    perror (LogName);
  else
    {
      SetFilePointer (logf, 0, 0, FILE_END);   // Append new content
      SetStdHandle (STD_OUTPUT_HANDLE, logf);
      SetStdHandle (STD_ERROR_HANDLE, logf);
    }

  WORD want;
  WSADATA offer;
  want = MAKEWORD (2, 2);
  if (0 != WSAStartup (want, &offer))
    {
      perror ("WSAStartup");
      CloseHandle (logf);
      return WSAGetLastError ();
    }

  // listen on port 8888 (can I set an env variable for this?)
  SOCKET acceptor = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
  sockaddr_in listen_addr;
  memset (&listen_addr, 0, sizeof (listen_addr));
  listen_addr.sin_family = AF_INET;
  listen_addr.sin_addr.s_addr = INADDR_ANY;
  listen_addr.sin_port = htons (8888);
  if (SOCKET_ERROR == bind (acceptor,
                            (struct sockaddr *)&listen_addr,
                            sizeof (listen_addr)))
    {
      perror ("bind");
    }
  else
    {
      listen (acceptor, 10);
      SOCKET peer;
      while ((peer = accept (acceptor, 0, 0)) != INVALID_SOCKET)
        {
          Peer *p = new Peer (peer);
          if (p == 0)
            {
              perror ("Out of memory");
              closesocket (peer);
              peer = INVALID_SOCKET;
              continue;
            }
          if (0 == _beginthreadex (0,            // security
                                   64 * 1024,    // stack size
                                   peer_svc,     // entrypoint
                                   (void *)p,    // param
                                   0,            // creation flags
                                   0))           // ptr to thread id
            {
              perror ("beginthreadex peer");
              closesocket (peer);
              delete p;
            }
          p = 0;
          peer = INVALID_SOCKET;
        }
      perror ("accept");
    }

  closesocket (acceptor);
  WSACleanup ();
  return 0;
}

// Entrypoint for thread that's spawned to run a peer's session. Direct
// control to the peer class.
static unsigned int __stdcall
peer_svc (void *peer_p)
{
  Peer *p = (Peer *)peer_p;
  DWORD status = p->svc ();
  delete p;
  return status;
}

// Entrypoint for the thread spawned to run a test. The thread arg is the
// Test * - call back to the test's run() method; return the test exit code
// as the thread's return value.
static unsigned int __stdcall
run_test (void *test_p)
{
  Test *t = (Test *)test_p;
  return t->run ();
}

// Format a Windows system or Winsock error message given an error code.
static const char *
format_errmsg (unsigned int errcode, const char *prefix)
{
  static const size_t errmsgsize = 1024;
  static char errmsg[errmsgsize];

  sprintf (errmsg, "%s: ", prefix);
  size_t len = strlen (errmsg);
  char *next = &errmsg[len];
  size_t max_fmt = errmsgsize - len;
  if (0 != FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM |
                          FORMAT_MESSAGE_IGNORE_INSERTS,
                          0,
                          errcode,
                          0,                 // Use default language
                          next,
                          (DWORD)max_fmt,
                          0))
    {
      strcat (errmsg, "\n");
      return errmsg;
    }

  errno = errcode;
  char *msg = _strerror (prefix);
  sprintf (errmsg, "err %d: %s", errcode, msg);
  return errmsg;
}

#ifdef TEST_RUNNER_EXPORTS
#define TEST_RUNNER_API __declspec(dllexport)
#else
#define TEST_RUNNER_API __declspec(dllimport)
#endif

__declspec(dllexport) int test_entry(void)
{
  return 0;
}
