/* $Id$ */
#include "pace/sys/mman.h"
#include "pace/stdio.h"
#include "pace/fcntl.h"
#include "pace/string.h"
#include "pace/stdlib.h"
#include "pace/sys/types.h"
#include "pace/pthread.h"
#include "pace/emulation/mqueue.h"

typedef struct mqd* pace_mqd_t;

typedef struct
{
  pace_mq_attr attr;
  pace_size_t num_open;
  pace_pthread_mutex_t mutex;
  pace_size_t head;
  pace_size_t freelist;
} mqfile;

typedef struct
{
  pace_size_t next; /* Index of next element */
  unsigned int priority;
  pace_size_t length;
} message_header;

struct mq_attr attrdefault = { 0, 32, 256, 0 };

pace_mqd_t mq_open (const char* name, int oflag, pace_mode_t mode, pace_mq_attr* attr)
{
  int m_padding = sizeof (message_header); /* How much extra space per message do we need */
  int f_padding = sizeof (mqfile); /* How much fixed padding is needed */
  int mflags, mprot;
  int fd;
  pace_size_t i;
  pace_size_t mapsize;
  char* mmaploc;
  int create_mmap = 0; /* 1 if the file has never be inited */
  pace_pthread_mutexattr_t mattr;
  message_header* temp = 0; /*Used in initializaiton of mqueue*/
  long index; /* index into the file */
  pace_mqd_t result = pace_malloc (sizeof (struct mqd));

  if (attr == 0)
  {
    attr = &attrdefault;
  }

  /* Fix alignment */
  if (attr->mq_msgsize % sizeof (long) != 0)
  {
    attr->mq_msgsize += 8 - (attr->mq_msgsize % sizeof (long));
  }
  if (oflag & PACE_O_CREAT)
  {
    /* We need to protect access without the help of O_RDONLY in the fs */
    fd = pace_open ((name, PACE_O_RDWR | PACE_O_CREAT | PACE_O_EXCL, mode));

    if (fd == -1 && errno != EEXIST)
    {
      /* An error other than EEXIST has occurred. */
      return (pace_mqd_t)-1;
    }
    else if (fd != -1)
    {
      /* If a new file was created successfully */
      create_mmap = 1;
    }
    else if (oflag & PACE_O_EXCL)
    {
      /* If the file exists and we don't want it */
      errno = EEXIST;
      return (pace_mqd_t)-1;
    }
    else
    {
      /* We want the existing file */
      fd = pace_open ((name, PACE_O_RDWR));
    }
  }
  else
  {
    fd = pace_open ((name, PACE_O_RDWR));
    if (fd == -1)
    {
      errno = ENOENT;
      return (pace_mqd_t)-1;
    }
  }

  mapsize = f_padding + (attr->mq_msgsize + m_padding) * (attr->mq_maxmsg);
  mprot = PACE_PROT_READ | PACE_PROT_WRITE;
  mflags = PACE_MAP_SHARED;

  if (create_mmap)
  {
    /* Create and 0 out the file */
    if (pace_lseek (fd, mapsize, PACE_SEEK_SET) == -1)
    {
      return (pace_mqd_t)-1;
    }
    if (pace_write (fd, "", 1) != 1)
    {
      return (pace_mqd_t)-1;
    }

    mmaploc = mmap (0, mapsize, mprot, mflags, fd, 0);
    if ((int)mmaploc == -1)
    {
      return (pace_mqd_t)-1;
    }
    pace_memset (mmaploc, 0, mapsize);

    if (pace_pthread_mutexattr_init (&mattr) == -1)
    {
      return (pace_mqd_t)-1;
    }
    if (pace_pthread_mutex_init (&(((mqfile*)mmaploc)->mutex), &mattr) == -1)
    {
      return (pace_mqd_t)-1;
    }
    if (pace_pthread_mutex_lock (&(((mqfile*)mmaploc)->mutex)) == -1)
    {
      return (pace_mqd_t)-1;
    }

    index = sizeof (mqfile);
    ((mqfile*)mmaploc)->freelist = index;
    ((mqfile*)mmaploc)->head = 0;

    for (i = 0; i < attr->mq_maxmsg; ++i)
    {
      temp = (message_header *) &mmaploc[index];
      index += sizeof (message_header) + attr->mq_msgsize;
      temp->next = index;
    }
    temp->next = 0;
    attr->mq_curmsgs = 0;
    ((mqfile*)mmaploc)->attr = *attr;
  }
  else
  {
    /* Just open the existing map */
    mmaploc = mmap (0, mapsize, mprot, mflags, fd, 0);
    if ((int)mmaploc == -1)
    {
      return (pace_mqd_t)-1;
    }

    /* ???? Test here for race */

    if (pace_pthread_mutex_lock (&(((mqfile*)mmaploc)->mutex)) == -1)
    {
      return (pace_mqd_t)-1;
    }
    ((mqfile*)mmaploc)->attr.mq_flags = attr->mq_flags;
  }

  ((mqfile*)mmaploc)->num_open++;


  if (pace_pthread_mutex_unlock (&(((mqfile*)mmaploc)->mutex)) == -1)
  {
    return (pace_mqd_t)-1;
  }

  result->fd = fd;
  result->mptr = mmaploc;
  result->length = mapsize;
  result->oflag = oflag;

  return result;
}

int mq_close (pace_mqd_t mqdes)
{
  if (pace_pthread_mutex_lock (&( ((mqfile*)mqdes->mptr)->mutex)) == -1)
  {
    errno = EBADF;
    return -1;
  }
  ((mqfile*)mqdes->mptr)->num_open--;
  if (pace_pthread_mutex_unlock (&(((mqfile*)mqdes->mptr)->mutex)) == -1)
  {
    errno = EBADF;
    return -1;
  }
  if (munmap (mqdes->mptr, mqdes->length) == -1)
  {
    return -1;
  }
  free (mqdes);
  return 0;
}

int mq_unlink (const char* name)
{
  return pace_unlink (name);
}

int mq_send (pace_mqd_t mqdes, const char* ptr, pace_size_t length, unsigned int priority)
{
  mqfile* queue = ((mqfile*)mqdes->mptr);
  long index, old_index;
  if (mqdes->oflag & O_RDONLY)
  {
    /* Incorrect access priviledges */
    errno = EBADF;
    return -1;
  }
  if (queue->attr.mq_msgsize < length)
  {
    /* Message too long */
    errno = EMSGSIZE;
    return -1;
  }

  /* If the queue is full... */
  if (queue->attr.mq_curmsgs >= queue->attr.mq_maxmsg)
  {
    if (queue->attr.mq_flags & O_NONBLOCK)
    {
      errno = EAGAIN;
      return -1;
    }
    else
    {
      /* ???? */
    }
  }
  else
  {
    if (pace_pthread_mutex_lock (&queue->mutex) == -1)
    {
      return -1;
    }

    queue->attr.mq_curmsgs++;
    /* Fill in the fields of the header */
    ((message_header*)(&mqdes->mptr[queue->freelist]))->priority = priority;
    ((message_header*)(&mqdes->mptr[queue->freelist]))->length = length;
    pace_memcpy (((void*)(&mqdes->mptr[queue->freelist + sizeof (message_header)])),
                 ptr, length);

    /* Update the linked list */
    old_index = 0;
    index = queue->head;
    while (index != 0 && ((message_header*)(&mqdes->mptr[index]))->priority >= priority)
    {
      old_index = index;
      index = ((message_header*)(&mqdes->mptr[index]))->next;
    }

    /* If the msg goes at the head */
    if (old_index == 0)
    {
      queue->head = queue->freelist;
      queue->freelist = ((message_header*)(&mqdes->mptr[queue->freelist]))->next;
      ((message_header*)(&mqdes->mptr[queue->head]))->next = index;
    }
    else
    {
      ((message_header*)(&mqdes->mptr[old_index]))->next = queue->freelist;
      old_index = queue->freelist;
      queue->freelist = ((message_header*)(&mqdes->mptr[queue->freelist]))->next;
      ((message_header*)(&mqdes->mptr[old_index]))->next = index;
    }
    if (pace_pthread_mutex_unlock (&queue->mutex) == -1)
    {
      return -1;
    }
  }
  return 0;
}

pace_ssize_t mq_receive (pace_mqd_t mqdes,
                              char * msg_ptr,
                              pace_size_t msg_len,
                              unsigned int * nmsg_prio)
{
  mqfile* queue = ((mqfile*)mqdes->mptr);
  pace_size_t temp;

  if (queue->attr.mq_msgsize > msg_len)
  {
    errno = EMSGSIZE;
    return -1;
  }
  if (queue->attr.mq_curmsgs == 0)
  {
    if (queue->attr.mq_flags & O_NONBLOCK)
    {
      errno = EAGAIN;
      return -1;
    }
    else
    {
      errno = ENOTSUP;
      return -1;
      /*???? wait on cond var */
    }
  }

  if (pace_pthread_mutex_lock (&queue->mutex) == -1)
  {
    errno = EBADMSG;
    return -1;
  }

  queue->attr.mq_curmsgs--;
  if (nmsg_prio != 0)
  {
    *nmsg_prio = ((message_header*)(&mqdes->mptr[queue->head]))->priority;
  }

  pace_memcpy (msg_ptr, ((void*)(&mqdes->mptr[queue->head + sizeof (message_header)])),
               ((message_header*)(&mqdes->mptr[queue->head]))->length);
  temp = queue->head;
  queue->head = ((message_header*)(&mqdes->mptr[queue->head]))->next;
  ((message_header*)(&mqdes->mptr[temp]))->next = queue->freelist;
  queue->freelist = temp;

  if (pace_pthread_mutex_unlock (&queue->mutex) == -1)
  {
    errno = EBADMSG;
    return -1;
  }
  return ((message_header*)(&mqdes->mptr[queue->head]))->length;
}

int mq_getattr (pace_mqd_t mqdes, pace_mq_attr * mqstat)
{
  *mqstat = ((mqfile*)mqdes->mptr)->attr;
  return 0;
}

int mq_setattr(pace_mqd_t mqdes, const pace_mq_attr * mqstat, pace_mq_attr * omqstat)
{
  if (omqstat != 0)
  {
    *omqstat = ((mqfile*)mqdes->mptr)->attr;
  }
  if (mqstat == 0 || mqdes == 0)
  {
    /* You eediot*/
    errno = EFAULT;
    return -1;
  }

  ((mqfile*)mqdes->mptr)->attr.mq_flags = mqstat->mq_flags;

  return 0;
}

int mq_notify (pace_mqd_t mqd, const pace_sigevent* notification)
{
  errno = ENOSYS;
  PACE_UNUSED_ARG (mqd);
  PACE_UNUSED_ARG (notification);
  return -1;
}

void print_queue (pace_mqd_t mqd)
{

  mqfile* queue = ((mqfile*)(mqd->mptr));
  pace_size_t i, index;
  i=0;
  index = queue->freelist;
  while (index != 0)
    {
      i++;
      index = ((message_header*)(&mqd->mptr[index]))->next;
    }
  printf ("There are %i total blacks of size %i.\n", queue->attr.mq_maxmsg, queue->attr.mq_msgsize);
  printf ("There are %i free blocks left.\n", i);
  printf ("There are %i messages on the queue.\n", queue->attr.mq_curmsgs);

  i=0;
  index = queue->head;
  while (index != 0)
    {
      i++;
      printf ("Message %i has a prio of %i and len of %i.\n", i,
              ((message_header*)(&mqd->mptr[index]))->priority,
              ((message_header*)(&mqd->mptr[index]))->length);
      index = ((message_header*)(&mqd->mptr[index]))->next;
    }
  printf ("\n");
}

int main (int argc, char** argv)
{
  int flags;
  pace_mqd_t mqd;
  pace_mq_attr attr;
  char* string;
  char* s2;
  flags = O_RDWR | O_CREAT | O_NONBLOCK;
  attr.mq_flags |= O_NONBLOCK;
  attr.mq_msgsize = 51;
  attr.mq_maxmsg = 50;

  mqd = mq_open ("hello", flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, &attr);

  PACE_UNUSED_ARG (argc);
  PACE_UNUSED_ARG (argv);

  string = "Message\n";
  s2 = malloc (1600);

  print_queue (mqd);

  printf ("Sending message 1\n");
  if (mq_send (mqd, string, 9, 11) == -1)
  {
    perror ("send");
    return -1;
  }
  print_queue (mqd);

  printf ("Sending message 2\n");
  string = "Message2\n";
  if (mq_send (mqd, string, 10, 10) == -1)
    {
    perror ("send");
    return -1;
    }
print_queue (mqd);

  printf ("Getting message one\n");
  flags = mq_receive (mqd, s2, 1600, 0);
  if (flags == -1)
    {
      perror ("receiev:");
      exit (0);
    }
print_queue (mqd);
  printf (s2);
  flags = mq_receive (mqd, s2, 1600, 0);
  if (flags == -1)
    {
      perror ("receiev:");
      exit (0);
    }
print_queue (mqd);
  printf (s2);

  return mq_close (mqd);
}
