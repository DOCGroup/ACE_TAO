// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Based_Pointer_Test_Lib.cpp
//
// = DESCRIPTION
//    This test confirms the function of the Based_Pointer_Repository
//
// = AUTHOR
//    Steve Williams  <steve@telxio>
//
// ============================================================================

#include "ace/ACE.h"
#include "ace/svc_export.h"
#include "ace/Based_Pointer_Repository.h"

ACE_RCSID (tests,
           Based_Pointer_Repository_DLL_Test,
           "$Id$")

extern "C" ACE_Svc_Export void *
get_based_pointer_repository_instance (void)
{
    void* baddr = ACE_BASED_POINTER_REPOSITORY::instance();
    return baddr;
}

