/* $Id$ */

#if PACE_LYNXOS != 0x300
#include "pace/stdio.h"
#include "pace/fcntl.h"
#include "pace/mqueue.h"
#include "pace/stdlib.h"
#include "pace/signal.h"
#include "pace/string.h"

#if defined (PACE_VXWORKS) && PACE_VXWORKS != 0
#include "vxworks_stub.c"
#endif /* VXWORKS */

/* Most of this code is borowed from Konstantin Knizhnik <http://www.ispras.ru/~knizhnik/posix1b/> */

pace_mqd_t mqdes;

void
msg_handler (int handler_arg)
{
  char buf[1024];
  unsigned int priority;
  int len = pace_mq_receive(mqdes, buf, sizeof buf, &priority);

  PACE_UNUSED_ARG (handler_arg);

  if (len < 0)
  {
    perror("mq_receive");
  }
  pace_printf("\nReceive message with priority %d: %s\n", priority, buf);
}

#define QUEUE_NAME "/testmsg1"

int
main (int argc, char** argv)
{
#if PACE_LYNXOS > 0
  pace_printf ("mqueue_test not supported on LynxOS.\n");
  PACE_UNUSED_ARG (argc);
  PACE_UNUSED_ARG (argv);
  return EXIT_SUCCESS;
#else  /* ! PACE_LYNXOS */
  char buf[1024];
  int choice, len;
  unsigned int priority;
  pace_sigevent notification;
  static pace_sigaction_s sa;
  int one = 1;

  mqdes = pace_mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0777, 0);
  if (mqdes == (pace_mqd_t)-1)
  {
    perror("mq_open");
    return EXIT_FAILURE;
  }

  notification.sigev_notify = SIGEV_SIGNAL;
  notification.sigev_signo = SIGUSR1;

  sa.sa_handler = msg_handler;
  sa.sa_flags = SA_RESTART;
  pace_sigaction(SIGUSR1, &sa, NULL);

  while (one)
  {
    printf("\t1. Send\n"
           "\t2. Asynchronouse receive\n"
           "\t3. Synchronouse receive\n"
           "\t4. Close queue and exit\n"
           "\t5. Remove queue and exit\n"
           "> ");
    if (!pace_fgets(buf, sizeof buf, stdin))
    {
      return EXIT_FAILURE;
    }
    if (pace_sscanf(buf, "%d", &choice) != 1)
    {
      pace_printf("Please select 1..5\n");
      continue;
    }
    switch (choice)
    {
    case 1:
      do
      {
        pace_printf("Message priority: ");
        pace_fgets(buf, sizeof buf, stdin);
      } while (pace_sscanf(buf, "%d", &priority) != 1);
      pace_printf("Message to send: ");
      pace_fgets(buf, sizeof buf, stdin);
      if (pace_mq_send(mqdes, buf, pace_strlen(buf)+1, priority) == -1)
      {
        perror("mq_send");
        return EXIT_FAILURE;
      }
      break;
    case 2:
      pace_mq_notify(mqdes, &notification);
      pace_printf("Waiting for notifications...\n");
      break;
    case 3:
      len = pace_mq_receive(mqdes, buf, sizeof buf, &priority);
      if (len < 0)
      {
        perror("mq_receive");
      }
      pace_printf("Receive message with priority %d: %s\n", priority, buf);
      break;
    case 4:
      pace_mq_close(mqdes);
      return EXIT_SUCCESS;
    case 5:
      pace_mq_close(mqdes);
      pace_mq_unlink(QUEUE_NAME);
      return EXIT_SUCCESS;
    default:
      pace_printf("Please select 1..5\n");
    }
  }
  PACE_UNUSED_ARG (argc);
  PACE_UNUSED_ARG (argv);
  return EXIT_SUCCESS;
#endif /* ! PACE_LYNXOS */
}
#else
int
main (int argc, char **argv)
{
  printf("PACE does not support LynxOS 3.0.0.\n");
}
#endif /* PACE_LYNXOS == 0x300 */
