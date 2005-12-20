/*
** $Id$
**
** Copyright 2001 Addison Wesley. All Rights Reserved.
*/

#include "ace/Functor.h"
#include "ace/Log_Msg.h"

#include "Reactive_Logging_Server_Ex.h"

int main (int argc, char *argv[])
{
  Reactive_Logging_Server_Ex server;

  if (server.run (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "server.run()"), 1);
  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<ACE_HANDLE, ACE_FILE_IO *>;
template class ACE_Hash_Map_Manager<ACE_HANDLE, ACE_FILE_IO *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_HANDLE, ACE_FILE_IO *, ACE_Hash<ACE_HANDLE>, ACE_Equal_To<ACE_HANDLE>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_HANDLE, ACE_FILE_IO *, ACE_Hash<ACE_HANDLE>, ACE_Equal_To<ACE_HANDLE>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_HANDLE, ACE_FILE_IO *, ACE_Hash<ACE_HANDLE>, ACE_Equal_To<ACE_HANDLE>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_HANDLE, ACE_FILE_IO *, ACE_Hash<ACE_HANDLE>, ACE_Equal_To<ACE_HANDLE>, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Entry<ACE_HANDLE, ACE_FILE_IO *>
#pragma instantiate ACE_Hash_Map_Manager<ACE_HANDLE, ACE_FILE_IO *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_HANDLE, ACE_FILE_IO *, ACE_Hash<ACE_HANDLE>, ACE_Equal_To<ACE_HANDLE>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_HANDLE, ACE_FILE_IO *, ACE_Hash<ACE_HANDLE>, ACE_Equal_To<ACE_HANDLE>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_HANDLE, ACE_FILE_IO *, ACE_Hash<ACE_HANDLE>, ACE_Equal_To<ACE_HANDLE>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_HANDLE, ACE_FILE_IO *, ACE_Hash<ACE_HANDLE>, ACE_Equal_To<ACE_HANDLE>, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
