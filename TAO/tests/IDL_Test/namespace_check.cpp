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
//    Contains conditional include of generated code which contains
//    reopened and/or nested modules, for platforms without
//    namespaces.
//
// = AUTHORS
//    Jeff Parsons <parsons@cs.wustl.edu>.
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
#include "paramsC.cpp"
#include "paramsS.cpp"
#include "nested_scopeC.cpp"
#include "nested_scopeS.cpp"

#endif /* ACE_HAS_USING_KEYWORD */
