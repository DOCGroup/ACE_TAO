// $Id$

#ifndef ACE_ADAPTER_DLL_H
#define ACE_ADAPTER_DLL_H
#include "ace/pre.h"

#include "ace/ACE.h"
#include "../../DLL.h"

class ACE_DLL_W : public ACE_DLL
{
public:
  ACE_DLL_W (int close_on_destruction = 1)
    : ACE_DLL (close_on_destruction)
  {}

  ACE_DLL_W (const wchar_t *dll_name,
             int open_mode = ACE_DEFAULT_SHLIB_MODE,
             int close_on_destruction = 1)
    : ACE_DLL (ACE_TEXT_WCHAR_TO_CHAR (dll_name), open_mode, close_on_destruction)
  {}

  int open (const wchar_t *dll_name,
            int open_mode = ACE_DEFAULT_SHLIB_MODE,
            int close_on_destruction = 1)
  { return ACE_DLL::open (ACE_TEXT_WCHAR_TO_CHAR (dll_name), open_mode, close_on_destruction); }

  ~ACE_DLL_W (void)
  {}

  void *symbol (const wchar_t *symbol_name)
  { return ACE_DLL::symbol (ACE_TEXT_WCHAR_TO_CHAR (symbol_name)); }

  wchar_t *error (void)
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE_DLL::error ())); }
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_DLL_H */
