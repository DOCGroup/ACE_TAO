// $Id$

//=============================================================================
/**
 *  @file    aix_hack_for_main.cpp
 *
 *  $Id$
 *
 *  This file is simply to force the Makefile to compile Main.cpp
 *  before it's needed in a test program that requires templates.
 *  Why is this needed? Because AIX Visual Age C++ has a template
 *  instantiation mechanism unlike any other in the way it remembers
 *  where templates are needed. We've previously worked around this
 *  by forcibly deleting the tempinc directory before compiling each
 *  test's source file. This worked because every test was in one file.
 *  When Main.cpp was added, this broke the scheme. This file simply
 *  gets Main.cpp to compile, then the resulting Main.o can be reused
 *  for each test, and it's again safe to delete the tempinc directory
 *  before each test compile.
 *  Hopefully, MPC will save us from this wretchedness...
 *
 *  @author Steve Huston <shuston@riverace.com>
 */
//=============================================================================

#include "test_config.h"

int
run_main (int, ACE_TCHAR *[])
{
  return 0;
}
