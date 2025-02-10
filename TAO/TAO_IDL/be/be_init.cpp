#include "global_extern.h"
#include "be_extern.h"
#include "../../tao/Version.h"

TAO_IDL_BE_Export void
BE_version ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TAO_IDL_BE, version ") ACE_TEXT (TAO_VERSION) ACE_TEXT (TAO_PATCH)
              ACE_TEXT ("\n")));
}

TAO_IDL_BE_Export int
BE_init (int &, ACE_TCHAR *[])
{
  // Initialize BE global data object.
  ACE_NEW_RETURN (be_global,
                  BE_GlobalData,
                  -1);

  idl_global->default_idl_version_ = IDL_VERSION_3;

  return 0;
}

TAO_IDL_BE_Export void
BE_post_init (char *[], long)
{
}

