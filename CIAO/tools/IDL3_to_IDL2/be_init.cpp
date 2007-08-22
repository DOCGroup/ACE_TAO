// $Id$

#include "global_extern.h"
#include "be_extern.h"
#include "../../ciao/Version.h"

TAO_IDL3_TO_IDL2_BE_Export void
BE_version (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "%s %s\n",
              ACE_TEXT ("TAO_ID3_TO_IDL2_BE, version"),
              ACE_TEXT (CIAO_VERSION)));
}

TAO_IDL3_TO_IDL2_BE_Export int
BE_init (int & /* argc */, char * /*argv */ [])
{
  // Initialize BE global data object.
  ACE_NEW_RETURN (be_global,
                  BE_GlobalData,
                  -1);

  idl_global->pass_orb_idl (true);
  return 0;
}

TAO_IDL3_TO_IDL2_BE_Export void
BE_post_init (char * /* files */ [], long /* nfiles */)
{
}

