// $Id$

#ifndef ACE_ADAPTER_OS_DIRENT_H
#define ACE_ADAPTER_OS_DIRENT_H
#include "ace/pre.h"

#include "../../OS_Dirent.h"

class ACE_OS_Dirent_W : public ACE_OS_Dirent
{
public:
  static DIR *opendir (const wchar_t *filename)
  { return ACE_OS_Dirent::opendir (ACE_TEXT_WCHAR_TO_CHAR (filename)); }
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_OS_DIRENT_H */
