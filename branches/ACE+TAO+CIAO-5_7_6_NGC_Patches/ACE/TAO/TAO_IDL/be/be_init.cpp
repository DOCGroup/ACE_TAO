// $Id$

#include "global_extern.h"
#include "be_extern.h"
#include "../../tao/Version.h"

TAO_IDL_BE_Export void
BE_version (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TAO_IDL_BE, version ") ACE_TEXT (TAO_VERSION)
              ACE_TEXT ("\n")));
}

TAO_IDL_BE_Export int
BE_init (int &, ACE_TCHAR *[])
{
  // Initialize BE global data object.
  ACE_NEW_RETURN (be_global,
                  BE_GlobalData,
                  -1);

  return 0;
}

TAO_IDL_BE_Export void
BE_post_init (char *[], long)
{
}

