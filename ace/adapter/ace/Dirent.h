// $Id$
#ifndef ACE_ADAPTER_DIRENT_H
#define ACE_ADAPTER_DIRENT_H
#include "ace/pre.h"

#include "../../Dirent.h"

class ACE_Dirent_W : public ACE_Dirent
{
public:
  ACE_EXPLICIT ACE_Dirent_W (const wchar_t *dirname)
    : ACE_Dirent (ACE_TEXT_WCHAR_TO_CHAR (dirname)) {}

  int open (const wchar_t *filename)
  { return ACE_Dirent::open (ACE_TEXT_WCHAR_TO_CHAR (filename)); };

  ~ACE_Dirent_W (void) {}
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_DIRENT_H */
