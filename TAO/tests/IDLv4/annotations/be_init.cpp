#include "tao/Version.h"
#include "ace/Log_Msg.h"

#include "global_extern.h"
#include "idl_version.h"
#include "drv_extern.h"

#include "be_extern.h"

void annotation_tests ();

void
BE_version ()
{
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("annotest_idl version ") ACE_TEXT (TAO_VERSION)
    ACE_TEXT ("\n")));
}

int
BE_init (int &, ACE_TCHAR *[])
{
  ACE_NEW_RETURN (be_global, BE_GlobalData, -1);

  // Enable Annotations
  idl_global->default_idl_version_ = IDL_VERSION_4;

  // Disable Trying to Parse File Arguments and creating File Output
  idl_global->ignore_files_ = true;

  return 0;
}

void
BE_post_init (char *[], long)
{
  annotation_tests ();
}
