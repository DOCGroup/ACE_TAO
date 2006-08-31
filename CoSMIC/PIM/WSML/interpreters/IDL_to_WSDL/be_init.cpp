// $Id$

#include "global_extern.h"
#include "be_extern.h"
#include "../../tao/Version.h"

IDL_TO_WSDL_BE_Export void
BE_version (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "%s %s\n",
              ACE_TEXT ("TAO_IDML_BE, version"),
              ACE_TEXT (TAO_VERSION)));
}

IDL_TO_WSDL_BE_Export int
BE_init (int &, char *[])
{
  // Initialize BE global data object.
  ACE_NEW_RETURN (be_global,
                  BE_GlobalData,
                  -1);

  return 0;
}

IDL_TO_WSDL_BE_Export void
BE_post_init (char *files[], long nfiles)
{
  be_global->xerces_init ();
  be_global->cache_files (files, nfiles);
  
  // We don't want to generate WSDL for implied IDL.
  idl_global->ignore_idl3 (true);
}

