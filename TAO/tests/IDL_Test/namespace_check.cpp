// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/IDL_Test
//
// = FILENAME
//    namespace_check.cpp
//
// = DESCRIPTION
//    Contains conditional include of reopened_modulesC.cpp
//    and reopened_modulesS.cpp, for platforms without
//    namespaces.
//
// = AUTHORS
//    Jeff Parsons <parsons@cs.wustl.edu> and TAO users.
//
// ============================================================================


#include "ace/OS.h"

#if defined (ACE_HAS_USING_KEYWORD)

#include "reopened_modulesC.cpp"
#include "reopened_modulesS.cpp"
#include "reopen_include1C.cpp"
#include "reopen_include1S.cpp"
#include "reopen_include2C.cpp"
#include "reopen_include2S.cpp"

#endif /* ACE_HAS_USING_KEYWORD */
