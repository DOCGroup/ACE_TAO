// $Id$

#include "Test_Saver.h"

#include <ace/streams.h>
#include <ace/OS_NS_stdio.h>

Test_Saver::Test_Saver()
{
}

bool
Test_Saver::begin_object(CORBA::Long id,
      const ACE_CString &type,
      const TAO_Notify::NVPList& attrs,
      bool changed)
{
  ACE_OS::fprintf (stdout, "Test_Saver::begin_object type=\"%s\""
                   "  id=%d changed=\"%d\"", type.c_str(), id, changed);
  for (size_t idx = 0; idx < attrs.size(); idx++)
  {
    ACE_OS::fprintf (stdout, " %s=\"%s\"", attrs[idx].name.c_str(),
                     attrs[idx].value.c_str());
  }
  ACE_OS::fprintf (stdout, "\n");
  return true;
}

void
Test_Saver::end_object (CORBA::Long id,
      const ACE_CString &type)
{
  ACE_OS::fprintf (stdout, "Test_Saver::end_object type=\"%s\" id=%d\n",
                   type.c_str(), id);
}

