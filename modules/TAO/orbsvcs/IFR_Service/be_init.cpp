// $Id$

#include "global_extern.h"
#include "be_extern.h"
#include "../../tao/Version.h"

int
BE_save_orb_args (int &argc, char *argv[])
{
  int i = 1;
  ACE_CString holder;

  while (i < argc)
    {
      if (ACE_OS::strncmp (argv[i], "-ORB", 4) == 0)
        {
          holder += ACE_CString (argv[i]);
          holder += " ";

          // Could be another -ORBxxx arg or an IDL compiler arg.
          if (*argv[i + 1] == '-')
            {
              ++i;
              continue;
            }

          // No-copy constructor.
          ACE_CString tmp (argv[i + 1],
                           0,
                           false);

          // If the arg ends with either .idl or .pidl, we're done.

          size_t len = tmp.length ();
          ssize_t pos = tmp.find (".idl");

          if (len - pos == 4)
            {
              return 0;
            }

          pos = tmp.find (".pidl");

          if (len - pos == 5)
            {
              return 0;
            }

          // If we're here, the next arg goes with the preceding -ORBxxx.
          holder += tmp;
          holder += " ";
          i += 2;
        }
      else
        {
          ++i;
        }
    }

  be_global->orb_args (holder);

  return 0;
}

// 'ac' must be passed in by reference, because it is also
// passed by reference to ORB_init, which may modify it.
// After BE_ifr_init returns to main() the modified argc
// must be passed to DRV_parse_args().
int
BE_ifr_orb_init (int &ac, char *av[])
{
  try
    {
      be_global->orb (CORBA::ORB_init (ac,
                                       av,
                                       0));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("BE_ifr_orb_init"));

      return 1;
    }

  return 0;
}

TAO_IFR_BE_Export void
BE_version (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "%s %s\n",
              ACE_TEXT ("TAO_IFR_BE, version"),
              ACE_TEXT (TAO_VERSION)));
}

TAO_IFR_BE_Export int
BE_init (int &argc, char *argv[])
{
  // Initialize BE global data object.
  ACE_NEW_RETURN (be_global,
                  BE_GlobalData,
                  -1);

  int status = BE_save_orb_args (argc, argv);

  if (status != 0)
    {
      return status;
    }

  idl_global->using_ifr_backend (true);

  return BE_ifr_orb_init (argc, argv);
}

TAO_IFR_BE_Export void
BE_post_init (char *[], long)
{
}

