// $Id$

#include "ace/Memory_Pool.h"
#include "ace/Shared_Memory_MM.h"
#include "ace/Malloc.h"
#include "ace/Malloc_T.h"
#include "ace/Thread_Manager.h"

#define DATA_SIZE 100
#define MESSAGE1 "Hiya over there client process"
#define MESSAGE2  "Did you hear me the first time?"
const ACE_TCHAR *poolname="My_Pool";

typedef ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex> Malloc_Allocator;

static void *
server (void * = 0)
{
  ACE_MMAP_Memory_Pool_Options opt;

  //Create the memory allocator passing it the shared memory
  //pool that you want to use
  Malloc_Allocator shm_allocator(poolname,poolname,&opt);

  //Create a message, allocate memory for it and bind it with
  //a name so that the client can the find it in the memory
  //pool
  char* Message1=(char*)shm_allocator.malloc(strlen(MESSAGE1)+1);

  ACE_OS::strcpy(Message1,MESSAGE1);
  shm_allocator.bind("FirstMessage",Message1);
  ACE_DEBUG((LM_DEBUG,"<<%s\n",Message1));

  //How about a second message
  char* Message2=(char*)shm_allocator.malloc(strlen(MESSAGE2)+1);
  ACE_OS::strcpy(Message2,MESSAGE2);
  shm_allocator.bind("SecondMessage",Message2);
  ACE_DEBUG((LM_DEBUG,"<<%s\n",Message2));

  //Set the Server to go to sleep for a while so that the client has
  //a chance to do its stuff
  ACE_DEBUG((LM_DEBUG, "Server done writing.. going to sleep zzz..\n\n\n"));
  ACE_OS::sleep(10); 

  ACE_DEBUG ((LM_DEBUG, "server exit\n"));

  return 0;
}

static void *
client (void * = 0)
{
  ACE_MMAP_Memory_Pool_Options opt;

  //Create the memory allocator passing it the shared memory
  //pool that you want to use
  Malloc_Allocator shm_allocator(poolname,poolname,&opt);

  //Lets get that first message.  Notice that the find is looking up the
  //memory based on the "name" that was bound to it by the server.
  void *Message1 = 0;
  if(shm_allocator.find("FirstMessage") == -1 )
    {
      ACE_ERROR((LM_ERROR,
                 "Client ack\n"));
      return 0;
    }
  if(shm_allocator.find("FirstMessage",Message1)==-1)
    {
      ACE_ERROR((LM_ERROR,
                 "Client: Problem cant find data that server has sent\n"));
      return 0;
    }

  ACE_OS::printf(">>%s\n",(char*) Message1);
  ACE_OS::fflush(stdout);

  //Lets get that second message now.
  void *Message2;
  if(shm_allocator.find("SecondMessage",Message2)==-1)
    {
      ACE_ERROR((LM_ERROR,
                 "Client: Problem cant find data that server has sent\n"));
      ACE_OS::exit(1);
    }
  ACE_OS::printf(">>%s\n",(char*)Message2);
  ACE_OS::fflush(stdout);

  ACE_DEBUG((LM_DEBUG,"Client done reading! BYE NOW\n"));
  ACE_OS::fflush(stdout);

  //Get rid of all resources allocated by the server. In other
  //words get rid of the shared memory pool that had been
  //previously allocated
  shm_allocator.remove();

  return 0;
}

int main (int, char *argv[])
{
  switch (*argv[1])
    {
    case 's':
      server ();
      break;
    default:
      client ();
      break;
    }

  return 0;
}
