// $Id$

#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_string.h"
#include "ace/Mem_Map.h"
#include "ace/Log_Msg.h"

// Listing 1 code/ch17
int ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  ACE_HANDLE srcHandle = ACE_OS::open (argv[1], O_RDONLY);
  ACE_ASSERT(srcHandle != ACE_INVALID_HANDLE);

  ACE_Mem_Map srcMap (srcHandle, static_cast<size_t> (-1), PROT_READ, ACE_MAP_PRIVATE);
  ACE_ASSERT(srcMap.addr () != 0);

  ACE_Mem_Map destMap (argv[2],
                       srcMap.size (),
                       O_RDWR | O_CREAT,
                       ACE_DEFAULT_FILE_PERMS,
                       PROT_RDWR,
                       ACE_MAP_SHARED);
  ACE_ASSERT(destMap.addr () != 0);

  ACE_OS::memcpy (destMap.addr (),
                  srcMap.addr (),
                  srcMap.size ());
  destMap.sync ();

  srcMap.close ();
  destMap.close ();
  return 0;
}
// Listing 1

