// $Id$

#ifndef ACE_ADAPTER_ARGV_H
#define ACE_ADAPTER_ARGV_H
#include "ace/pre.h"

#include "../../ARGV.h"

class ACE_ARGV_W : public ACE_ARGV
{
public:
  ACE_ARGV_W (const wchar_t buf[], int substitute_env_args = 1) 
    : ACE_ARGV (ACE_TEXT_WCHAR_TO_CHAR (buf), substitute_env_args) {}

  // @@ Implement: ACE_ARGV_W (wchar_t *argv[], int substitute_env_args = 1)

  // @@ Implement: ACE_ARGV_W (wchar_t *first_argv[], wchar_t *second_argv[], int substitute_env_args =1)

  ~ACE_ARGV_W (void) {}

  const wchar_t *operator[] (size_t index)
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE_ARGV::operator[] (index))); }
  
  // @@ Implement: wchar_t **argv (void);

  // @@ Implement: const wchar_t *buf (void);

  int add (const wchar_t *next_arg)
  { return ACE_ARGV::add (ACE_TEXT_WCHAR_TO_CHAR (next_arg)); };

  // @@ Implement: int add (wchar_t *argv[]);
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_ARGV_H */
