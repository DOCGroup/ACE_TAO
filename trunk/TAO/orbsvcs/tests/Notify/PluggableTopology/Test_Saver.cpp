// $Id$

#include "Test_Saver.h"

#include <ace/streams.h>

Test_Saver::Test_Saver()
{
}

bool
Test_Saver::begin_object(CORBA::Long id,
      const ACE_CString &type,
      const TAO_Notify::NVPList& attrs,
      bool changed
      ACE_ENV_ARG_DECL_NOT_USED)
{
  cout << "Test_Saver::begin_object type=\"" << type << "\" "
    << " id=" << id
    << " changed=\"" << changed << "\"";
  for (size_t idx = 0; idx < attrs.size(); idx++)
  {
    cout << " " << attrs[idx].name << "=\"" << attrs[idx].value << "\"";
  }
  cout << endl;
  return true;
}

void
Test_Saver::end_object (CORBA::Long id,
      const ACE_CString &type ACE_ENV_ARG_DECL_NOT_USED)
{
  cout << "Test_Saver::end_object type=\"" << type << "\""
  << " id=" << id
  << endl;
}

