// $Id$

#include "XML_Utils.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_main.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR *softpkg_filename = 0;
  ACE_TCHAR *assembly_filename = 0;
  char * rtcad_filename = 0;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("a:s:r:"));
  ACE_TCHAR c;

  while ((c = get_opt ()) != -1)
    {
      switch (c)
        {
        case 's':
          softpkg_filename = get_opt.opt_arg ();
          break;
        case 'a':
          assembly_filename = get_opt.opt_arg ();
          break;
        case 'r':
          rtcad_filename = get_opt.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage: %s [-s <softpkg>]\n\t[-a <assembly>]\n\t"
                             "[-r <rtcad>]\n",
                             argv[0]),
                            -1);
        }
    };

  if (softpkg_filename == 0 && assembly_filename == 0 && rtcad_filename == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "No filename specified\n"), -1);

  if (softpkg_filename != 0)
    {
      CIAO::Softpkg_Handler::Softpkg_Info info;
      info.csd_path_ = softpkg_filename;

      if (CIAO::XML_Utils::parse_softpkg (&info) == 0)
        info.dump ();
    }

  if (assembly_filename != 0)
    {
      CIAO::Assembly_Spec assembly_spec;

      if (CIAO::XML_Utils::parse_componentassembly (assembly_filename,
                                                    &assembly_spec) == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "ComponentAssembly Done.\n"));
        }
    }

  if (rtcad_filename != 0)
    {
      CIAO::RTConfiguration::RTORB_Resource_Info resources;
      CIAO::RTConfiguration::Policy_Sets psets;

      if (CIAO::XML_Utils::parse_rtcad_extension (rtcad_filename,
                                                  resources,
                                                  psets) == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "RTCad extension Done.\n"));
        }

    }

  return 0;
}
