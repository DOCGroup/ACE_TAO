
//=============================================================================
/**
 *  @file    Based_Pointer_Test_Lib.cpp
 *
 *  This test confirms the function of the Based_Pointer_Repository
 *
 *  @author Steve Williams  <steve@telxio>
 */
//=============================================================================


#include "ace/ACE.h"
#include "ace/svc_export.h"
#include "ace/Based_Pointer_Repository.h"

extern "C" ACE_Svc_Export void *get_based_pointer_repository_instance ()
{
  void* baddr = ACE_BASED_POINTER_REPOSITORY::instance();
  return baddr;
}

