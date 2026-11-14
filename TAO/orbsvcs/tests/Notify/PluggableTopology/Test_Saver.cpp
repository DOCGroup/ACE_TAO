#include "Test_Saver.h"

#include <ace/streams.h>
#include <ace/OS_NS_stdio.h>

bool
Test_Saver::begin_object (CORBA::Long id,
                          ACE_CString const& type,
                          TAO_Notify::NVPList const& attrs,
                          bool changed)
{
  ACE_OS::fprintf (stdout, "Test_Saver::begin_object type=\"%s\"  id=%d changed=\"%d\"",
                   type.c_str (), id, changed);
  for (size_t idx {}; idx < attrs.size (); ++idx)
    ACE_OS::fprintf (stdout, " %s=\"%s\"", attrs[idx].name.c_str (), attrs[idx].value.c_str ());
  ACE_OS::fprintf (stdout, "\n");
  return true;
}

void
Test_Saver::end_object (CORBA::Long id,
                        ACE_CString const& type)
{
  ACE_OS::fprintf (stdout, "Test_Saver::end_object type=\"%s\" id=%d\n", type.c_str (), id);
}

