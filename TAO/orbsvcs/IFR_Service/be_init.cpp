#include "orbsvcs/Log_Macros.h"
#include "global_extern.h"
#include "be_extern.h"
#include "../../tao/Version.h"

#include <cstddef>
#include <string>

int
BE_save_orb_args (int &argc, ACE_TCHAR *argv[])
{
  int i = 1;
  std::basic_string<ACE_TCHAR> holder;

  while (i < argc)
    {
      if (ACE_OS::strncmp (argv[i], ACE_TEXT("-ORB"), 4) == 0)
        {
          holder += argv[i];
          holder += ACE_TEXT (" ");

          // Could be another -ORBxxx arg or an IDL compiler arg.
          if (i + 1 == argc || *argv[i + 1] == '-')
            {
              ++i;
              continue;
            }

          std::basic_string<ACE_TCHAR> const tmp (argv[i + 1]);

          // If the arg ends with either .idl or .pidl, we're done.

          std::size_t const len = tmp.length ();
          if (len >= 4 && tmp.compare (len - 4, 4, ACE_TEXT (".idl")) == 0)
            {
              return 0;
            }

          if (len >= 5 && tmp.compare (len - 5, 5, ACE_TEXT (".pidl")) == 0)
            {
              return 0;
            }

          // If we're here, the next arg goes with the preceding -ORBxxx.
          holder += tmp;
          holder += ACE_TEXT (" ");
          i += 2;
        }
      else
        {
          ++i;
        }
    }

  std::string const orb_args (ACE_TEXT_ALWAYS_CHAR (holder.c_str ()));
  be_global->orb_args (orb_args);

  return 0;
}

// 'ac' must be passed in by reference, because it is also
// passed by reference to ORB_init, which may modify it.
// After BE_ifr_init returns to main() the modified argc
// must be passed to DRV_parse_args().
int
BE_ifr_orb_init (int &ac, ACE_TCHAR *av[])
{
  try
    {
      ACE_TCHAR *name {};
      be_global->orb (CORBA::ORB_init (ac,
                                       av,
                                       name));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("BE_ifr_orb_init"));

      return 1;
    }

  return 0;
}

TAO_IFR_BE_Export void
BE_version ()
{
  ORBSVCS_DEBUG ((LM_DEBUG,
              "%s %s%s\n",
              ACE_TEXT ("TAO_IFR_BE, version"),
              ACE_TEXT (TAO_VERSION),
              ACE_TEXT (TAO_PATCH)));
}

TAO_IFR_BE_Export int
BE_init (int &argc, ACE_TCHAR *argv[])
{
  // Initialize BE global data object.
  ACE_NEW_RETURN (be_global,
                  BE_GlobalData,
                  -1);

  int const status = BE_save_orb_args (argc, argv);

  if (status != 0)
    {
      return status;
    }

  idl_global->using_ifr_backend (true);

  return BE_ifr_orb_init (argc, argv);
}

TAO_IFR_BE_Export void
BE_post_init (std::vector<std::string> const &)
{
}

