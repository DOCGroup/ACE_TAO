//
// $Id$
//
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

/// Run by the child process
static int child(int argc, char* argv[]);

static int max_child_time = 300;
const char *child_name = 0;

static int parse_args(int& argc, char* argv[]);

int
main(int argc, char* argv[])
{
  if (parse_args(argc, argv) != 0)
    return 1;

  pid_t pid = fork();
  if (pid == -1) {
    fprintf(stderr, "Error in fork\n");
    return 1;
  }

  if (pid == 0) {
    // In the child...
    return child(argc, argv);
  }

  int status;
  pid_t child = waitpid(pid, &status, 0);
  if (child == -1) {
    fprintf(stderr, "sandbox: error waiting for child\n");
    return 1;
  }
  // Return whatever status the child has...
  return status;
}

sig_atomic_t child_died = 0;

extern "C" void
sigchld_handler(int signal)
{
  child_died = 1;
}


static int
child (int /* argc */, char* argv[])
{
  // The child becomes its own session, implicitly this turns the
  // child process into its own process group leader, allowing us to
  // terminate a hierarchy of children easily.
  pid_t sid = setsid();
  if (sid == -1)
    {
      fprintf(stderr, "sandbox: error in setid\n");
      return 1;
    }

  // Setup the signal handler to receive SIGCHLD signals:
  (void) signal(SIGCHLD, sigchld_handler);

  // Now we must fork again to wait for the grandchild process...

  pid_t grandchild = fork();
  if (grandchild == 0) {
    // Exec the arguments...
    int r = execvp(child_name, argv);
    if (r == -1) {
      fprintf(stderr, "sandbox: error in execvp()\n");
      return 1;
    }
  }

  // As the process leader we wait until the child returns or there is
  // a timeout...

  int status;
  int remaining_time = max_child_time;

  while (remaining_time != 0)
    {
      // We use the signal handler to return from sleep before all the
      // time expires
      remaining_time = sleep(remaining_time);

      // We could exit
      if (child_died)
        {
          pid_t child = waitpid(grandchild, &status, WNOHANG);
          if (child == -1)
            {
              fprintf(stderr,
                      "sandbox: no grandchild process (%d), panic\n",
                      grandchild);
              return 1;
            }
          if (child != 0)
            {
              //printf("sandbox: grandchild %d exits with status %d\n",
              //       child, status);
              return status;
            }
        }
    }

  printf("sandbox: timeout for grandchild %d\n", grandchild);

  // timeout, try to kill the child
  (void) kill(grandchild, SIGTERM);

  // Second chance, if the child does not die, then really kill it:
  pid_t killed = waitpid(grandchild, &status, WNOHANG);
  if (killed == 0)
    {
      // TERM did not work, use the heavy duty signal
      (void) kill(grandchild, SIGQUIT); sleep(1);
      killed = waitpid(grandchild, &status, WNOHANG);
    }
  if (killed == -1)
    {
      fprintf(stderr, "No such child (%d), panic\n",
              grandchild);
      return 1;
    }
  sleep(1);

  printf("sandbox: killing session %d\n", sid);

  // OK, now commit suicide, kill the session ID and all their friends
  (void) killpg(sid, SIGQUIT);
  fprintf(stderr, "sandbox: ooops! I should be dead!\n");

  return 1;
}

static int
parse_args(int &argc, char* argv[])
{
  if (argc < 3) {
    fprintf(stderr, "Usage: sandbox max_time program arguments\n");
    return 1;
  }

  max_child_time = atoi(argv[1]);
  child_name = argv[2];
  argc -= 2;
  for (int i = 0; i != argc; ++i)
    {
      argv[i] = argv[i + 2];
    }
  argv[argc] = 0;

  if (child_name == 0)
    return 1;

  return 0;
}
