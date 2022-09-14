#include "DLL_Service.h"

ACE_DLL_Service::ACE_DLL_Service ()
{
}

ACE_DLL_Service::~ACE_DLL_Service ()
{
}

int ACE_DLL_Service::init (int, ACE_TCHAR *[])
{
  return 0;
}

int ACE_DLL_Service::fini ()
{
  return 0;
}

void ACE_DLL_Service::destroy ()
{
  delete this;
}
