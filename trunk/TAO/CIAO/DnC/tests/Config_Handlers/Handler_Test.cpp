// $Id$

#include "Handler_Test.h"

#ifdef ACE_HAS_BROKEN_NESTED_TEMPLATES

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("DaNCe internal testing doesn't work with compilers ")
              ACE_TEXT ("not supporting nested templates (like VC6).\n")));
  return -1;
}

#endif // ACE_HAS_BROKEN_NESTED_TEMPLATES

