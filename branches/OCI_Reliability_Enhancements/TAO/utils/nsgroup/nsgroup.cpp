//==========================================================================
/**
 *  @file    nsgroup.cpp
 *
 *  $Id:
 *
 *  @author Phillip LaBanca <labancap@ociweb.com>
 */
//==========================================================================

#include "ace/Get_Opt.h"
#include "nsgroup_svc.h"

//============================================================================
bool
show_help(int argc, ACE_TCHAR *argv[])
{
  static const ACE_TCHAR options[] = ACE_TEXT("h");
  static const int skip_args     = 1;
  static const int report_errors = 0;
  static const int ordering      = ACE_Get_Opt::RETURN_IN_ORDER;
  static const int long_only     = 0;

  // Not enough arguments indicates help is required
  if( argc < 3 )
  {
    return true;
  }

  // Did they ask for help ?
  ACE_Get_Opt get_opts (
    argc,
    argv,
    options,
    skip_args,
    report_errors,
    ordering,
    long_only
  );

  if (get_opts.long_option (ACE_TEXT ("help"),'h') != 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("  Unable to add long option 'h'\n")), true);
  }

  int c;
  while ((c = get_opts ()) != -1)
  {
    switch (c)
      {
        case 'h':  // help
          return true;
        break;
      }
  }
  return false;
}

//============================================================================
int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {

      NS_group_svc ns_group (argc, argv);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) nsgroup - started ns_group\n"));

      if ( show_help(argc,argv) )
      {
        return ns_group.show_usage();
      }

      if ( ns_group.start_orb () == -1)
      {
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) nsgroup - start_orb error\n"));
        return 1;
      }

      if ( ns_group.run_cmd() == -1 )
      {
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) nsgroup - command error\n"));
        return 1;
      }

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ns_group exception");
      return 1;
    }

  return 0;
}
