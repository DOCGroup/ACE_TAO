#include "pace/stdio.h"
#include "pace/fcntl.h"
#include "pace/mqueue.h"
#include "pace/stdlib.h"

int main (int argc, char** argv)
{
  char* m1 = "Message1\n    ";
  int s1 = 10;
  char* m2 = "Message 2\n   ";
  int s2 = 11;
  pace_mqd_t mqd;
  pace_mq_attr attr;
  int flags = O_RDWR | O_CREAT;

  attr.mq_flags = O_NONBLOCK;
  attr.mq_msgsize = 51;
  attr.mq_maxmsg = 50;
  mqd = pace_mq_open (("/hello3", flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, &attr));
  if ((int)mqd == -1)
  {
    perror ("Open");
    return -1;
  }

  PACE_UNUSED_ARG (argc);
  PACE_UNUSED_ARG (argv);

  printf ("Sending Message 1\n");

  if (pace_mq_send (mqd, m1, s1, s1) == -1)
  {
    perror ("send");
    return -1;
  }

  printf ("Sending Message 2\n");


  if (pace_mq_send (mqd, m2, s2, s2) == -1)
  {
    perror ("send");
    return -1;
  }

  m1 = malloc (60);
  m2 = malloc (60);

  if (pace_mq_receive (mqd, m1, 60, 0) == -1)
  {
    perror ("receive");
    return -1;
  }
  printf (m1);
  if (pace_mq_receive (mqd, m2, 60, 0) == -1)
  {
    perror ("receive");
    return -1;
  }
  printf (m2);
  exit (EXIT_SUCCESS);
}
