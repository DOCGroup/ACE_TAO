// $Id$

#define ACE_BUILD_DLL

#include "ace/Object_Manager.h"

ACE_Object_Manager *ACE_Object_Manager::instance_ = 0;

void *ACE_Object_Manager::preallocated_object[
  ACE_Object_Manager::ACE_PREALLOCATED_OBJECTS] = { 0 };

void *ACE_Object_Manager::preallocated_array[
  ACE_Object_Manager::ACE_PREALLOCATED_ARRAYS] = { 0 };
