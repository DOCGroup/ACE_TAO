// $Id$

#ifndef ACE_ADAPTER_ARG_SHIFTER_H
#define ACE_ADAPTER_ARG_SHIFTER_H
#include "ace/pre.h"

#include "ACE.h"
#include "../../Arg_Shifter.h"


class ACE_Arg_Shifter_W : public ACE_Arg_Shifter
{
public:
  // @@ Implement ACE_Arg_Shifter_W (int& argc, wchar_t **argv, wchar_t **temp = 0);

  ~ACE_Arg_Shifter_W (void)
  {}

  wchar_t *get_current (void) const
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE_Arg_Shifter::get_current ())); }

  wchar_t *get_the_parameter (const wchar_t* flag)
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE_Arg_Shifter::get_the_parameter (ACE_TEXT_WCHAR_TO_CHAR (flag)))); }

  int cur_arg_strncasecmp (const wchar_t *flag)
  { return ACE_Arg_Shifter::cur_arg_strncasecmp (ACE_TEXT_WCHAR_TO_CHAR (flag)); }
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_ARG_SHIFTER_H */
