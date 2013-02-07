//==========================================================================
/**
 *  @file    nsgroup.cpp
 *
 *  $Id$
 *
 *  @author Phillip LaBanca <labancap@ociweb.com>
 */
//==========================================================================


#include "orbsvcs/Naming/FaultTolerant/nsgroup_svc.h"
#include "ace/Get_Opt.h"

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
  if( argc < 2 )
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
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("  Unable to add long option 'H'\n")),
                      true);
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

/**
 * @class nsgroup
 *
 * @brief Encapsulate the NS group command line operations in a class.
 *
 *
 */

class  NSGROUP
{
public:

  enum NSGROUP_COMMAND {
    NSGROUP_NONE,
    NSGROUP_HELP,
    NSGROUP_GROUP_CREATE,
    NSGROUP_GROUP_BIND,
    NSGROUP_GROUP_UNBIND,
    NSGROUP_GROUP_MODIFY,
    NSGROUP_GROUP_LIST,
    NSGROUP_GROUP_REMOVE,
    NSGROUP_MEMBER_LIST,
    NSGROUP_MEMBER_ADD,
    NSGROUP_MEMBER_REMOVE,
    NSGROUP_MEMBER_SHOW
  };

  /// Constructor
  NSGROUP (int argc, ACE_TCHAR **argv);

  /// start the ORB.
  int start_orb (void);

  /// parse command line, validate arguments and run the command
  int run_cmd (void);

  /// Display command line interface usage
  int show_usage( void );

  const ACE_TCHAR * group_arg(void) const { return group_arg_; }
  const ACE_TCHAR * policy_arg(void) const { return policy_arg_; }
  const ACE_TCHAR * location_arg(void) const { return location_arg_; }
  const ACE_TCHAR * ior_arg(void) const { return ior_arg_; }
  const ACE_TCHAR * namepath_arg(void) const { return namepath_arg_; }

private:

  /// parse command line arguments
  NSGROUP_COMMAND parse_command_line (void);

private:

  NS_group_svc svc_;

  int argc_;
  ACE_TCHAR **argv_;

  /// parsed command result
  NSGROUP_COMMAND nsgroup_cmd_;

  /// parsed command line arguments
  const ACE_TCHAR *group_arg_;
  const ACE_TCHAR *policy_arg_;
  const ACE_TCHAR *typeid_arg_;
  const ACE_TCHAR *location_arg_;
  const ACE_TCHAR *ior_arg_;
  const ACE_TCHAR *namepath_arg_;
};


 NSGROUP::NSGROUP (int argc, ACE_TCHAR **argv)
  : argc_ (argc),
    argv_ (argv),
    nsgroup_cmd_(NSGROUP::NSGROUP_NONE)
{
}

int
NSGROUP::start_orb (void)
{
  const int RC_SUCCESS =  0;
  const int RC_ERROR   = -1;

  try
  {

#if 0
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("start_orb::argc(%u)\n"), this->argc_));
    for( int i = 0; i < this->argc_; ++i){
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("start_orb::argv(%u:%s)\n"),
                  i,
                  this->argv_[i]));
    }
#endif //

    // Initialise the ORB.

    CORBA::ORB_var orb_ = CORBA::ORB_init (this->argc_, this->argv_);

    if (RC_SUCCESS != svc_.set_orb (orb_))
    {

      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT (" (%P|%t) Unable to initialize the ORB.\n")),
                          RC_ERROR);
    }

    //////////////////////////////////////////////////////////////////////////
    //
    //////////////////////////////////////////////////////////////////////////
    CORBA::Object_var naming_manager_object =
      orb_->resolve_initial_references ("NamingManager");

    FT_Naming::NamingManager_var naming_manager_ =
      FT_Naming::NamingManager::_narrow (naming_manager_object.in ());

    if (RC_SUCCESS != svc_.set_naming_manager (naming_manager_))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT (" (%P|%t) Unable to get Naming ")
                          ACE_TEXT ("Manager Reference\n")),
                        RC_ERROR);
    }
    //////////////////////////////////////////////////////////////////////////
    //
    //////////////////////////////////////////////////////////////////////////
    CORBA::Object_var naming_object =
      orb_->resolve_initial_references("NameService");

    CosNaming::NamingContextExt_var name_service_ =
      CosNaming::NamingContextExt::_narrow (naming_object.in ());

    if (RC_SUCCESS != svc_.set_name_context (name_service_))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT (" (%P|%t) Unable to get Name Service ")
                          ACE_TEXT ("Reference\n")),
                        RC_ERROR);
    }
    //////////////////////////////////////////////////////////////////////////
    //
    //////////////////////////////////////////////////////////////////////////

  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (ACE_TEXT("\nException raised initialising ORB\n"));
    return RC_ERROR;
  }

  return RC_SUCCESS;

}

int
NSGROUP::run_cmd(void)
{
  const int RC_ERROR   = -1;
  const int RC_BADARG  = -2;

  int rc = RC_ERROR;

  switch( parse_command_line () )
  {

    case NSGROUP_HELP:
      return show_usage ();
    break;

    case NSGROUP_GROUP_CREATE:
      rc = svc_.group_create ( group_arg(), policy_arg() );
    break;

    case NSGROUP_GROUP_BIND:
      rc = svc_.group_bind ( group_arg(), namepath_arg() );
    break;

    case NSGROUP_GROUP_UNBIND:
      rc = svc_.group_unbind ( namepath_arg() );
    break;

    case NSGROUP_GROUP_MODIFY:
      rc = svc_.group_modify ( group_arg(), policy_arg() );
    break;

    case NSGROUP_GROUP_LIST:
      rc = svc_.group_list ();
    break;

    case NSGROUP_GROUP_REMOVE:
      rc = svc_.group_remove ( group_arg() );
    break;

    case NSGROUP_MEMBER_LIST:
      rc = svc_.member_list ( group_arg() );
    break;

    case NSGROUP_MEMBER_ADD:
      rc = svc_.member_add ( group_arg(), location_arg(), ior_arg() );
    break;

    case NSGROUP_MEMBER_REMOVE:
      rc = svc_.member_remove ( group_arg(), location_arg() );
    break;

    case NSGROUP_MEMBER_SHOW:
      rc = svc_.member_show ( group_arg(), location_arg() );
    break;

    default:
      show_usage ();
      return rc;
    break;
  }

  if (rc == RC_BADARG)
  {
    show_usage ();
  }

  return rc;
}

NSGROUP::NSGROUP_COMMAND
NSGROUP::parse_command_line (void)
{
  #if 0
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("parse_command_line::argc(%u)\n"),
              this->argc_));
  for( int i = 0; i < this->argc_; ++i){
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("parse_command_line::argv(%u:%s)\n"),
                i,
                this->argv_[i]));
  }
  #endif

  static const ACE_TCHAR options[] = ACE_TEXT("g:p:t:l:i:n:h");
  static const int skip_args     = 1;
  static const int report_errors = 0;
  static const int ordering      = ACE_Get_Opt::PERMUTE_ARGS;
  static const int long_only     = 1;

  ACE_Get_Opt get_opts (
    this->argc_,
    this->argv_,
    options,
    skip_args,
    report_errors,
    ordering,
    long_only
  );

  this->group_arg_ = 0;
  if (get_opts.long_option (ACE_TEXT ("group"),
                           'g',
                           ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to add long option 'g'\n")),
                      NSGROUP_NONE);

  this->policy_arg_ = 0;
  if (get_opts.long_option (ACE_TEXT ("policy"),
                           'p',
                           ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to add long option 'p'\n")),
                      NSGROUP_NONE);

  this->location_arg_ = 0;
  if (get_opts.long_option (ACE_TEXT ("location"),
                           'l',
                           ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to add long option 'l'\n")),
                      NSGROUP_NONE);

  this->ior_arg_ = 0;
  if (get_opts.long_option (ACE_TEXT ("ior"),
                           'i',
                           ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to add long option 'i'\n")),
                      NSGROUP_NONE);

  this->namepath_arg_ = 0;
  if (get_opts.long_option (ACE_TEXT ("name"),
                           'n',
                           ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to add long option 'n'\n")),
                      NSGROUP_NONE);

  if (get_opts.long_option (ACE_TEXT ("help"), 'h') != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to add long option 'h'\n")),
                      NSGROUP_NONE);

  int c;
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'g':  // group
        this->group_arg_ =  get_opts.opt_arg ();
        break;
      case 'p':  // policy
        this->policy_arg_ = get_opts.opt_arg ();
        break;
      case 'l':  // location
        this->location_arg_ = get_opts.opt_arg ();
        break;
      case 'i': // ior
        this->ior_arg_ = get_opts.opt_arg ();
        break;
      case 'n': // name
        this->namepath_arg_ = get_opts.opt_arg ();
        break;
      case 'h':
        return NSGROUP_HELP;
      }

  // handle non-option arguments
  int non_option_arg_count = 0;
  for( int i = get_opts.opt_ind (); i < this->argc_; ++i)
  {

    non_option_arg_count++;


    if( ACE_OS::strncmp (this->argv_[i],ACE_TEXT("group_create"),
                         ACE_OS::strlen (ACE_TEXT("group_create"))) == 0 )
      {
        nsgroup_cmd_ =  NSGROUP_GROUP_CREATE;
      }
    else if(ACE_OS::strncmp (this->argv_[i], ACE_TEXT("group_bind"),
                             ACE_OS::strlen (ACE_TEXT("group_bind"))) == 0 )
      {
        nsgroup_cmd_ =  NSGROUP_GROUP_BIND;
      }
    else if(ACE_OS::strncmp (this->argv_[i], ACE_TEXT("group_unbind"),
                             ACE_OS::strlen (ACE_TEXT("group_unbind")))
            == 0 )
      {
        nsgroup_cmd_ =  NSGROUP_GROUP_UNBIND;
      }
    else if(ACE_OS::strncmp (this->argv_[i], ACE_TEXT("group_modify"),
                             ACE_OS::strlen (ACE_TEXT("group_modify"))) == 0 )
      {
      nsgroup_cmd_ =  NSGROUP_GROUP_MODIFY;
      }
    else if(ACE_OS::strncmp (this->argv_[i], ACE_TEXT("group_list"),
                               ACE_OS::strlen (ACE_TEXT("group_list"))) == 0 )
      {
        nsgroup_cmd_ =  NSGROUP_GROUP_LIST;
      }
    else if(ACE_OS::strncmp (this->argv_[i], ACE_TEXT("group_remove"),
                             ACE_OS::strlen (ACE_TEXT("group_remove"))) == 0 )
      {
        nsgroup_cmd_ =  NSGROUP_GROUP_REMOVE;
      }
    else if(ACE_OS::strncmp (this->argv_[i], ACE_TEXT("member_list"),
                               ACE_OS::strlen (ACE_TEXT("member_list"))) == 0 )
      {
        nsgroup_cmd_ =  NSGROUP_MEMBER_LIST;
      }
    else if(ACE_OS::strncmp (this->argv_[i], ACE_TEXT("member_add"),
                             ACE_OS::strlen (ACE_TEXT("member_add"))) == 0 )
      {
        nsgroup_cmd_ =  NSGROUP_MEMBER_ADD;
      }
    else if(ACE_OS::strncmp (this->argv_[i], ACE_TEXT("member_remove"),
                             ACE_OS::strlen (ACE_TEXT("member_remove"))) == 0 )
      {
        nsgroup_cmd_ =  NSGROUP_MEMBER_REMOVE;

      }
    else if(ACE_OS::strncmp (this->argv_[i], ACE_TEXT("member_show"),
                               ACE_OS::strlen (ACE_TEXT("member_show"))) == 0 )
      {
        nsgroup_cmd_ =  NSGROUP_MEMBER_SHOW;
      }
    else
      {
        nsgroup_cmd_ =  NSGROUP_NONE;
      }
  }

  // The command should be the only non option argument
  if ( non_option_arg_count > 1 ) {
      nsgroup_cmd_ =  NSGROUP_NONE;
  }

  return nsgroup_cmd_;
}


int
NSGROUP::show_usage ( void )
{
  const int RC_SUCCESS =  0;

  ACE_DEBUG ((LM_INFO,
    ACE_TEXT ("Usage:\n")
    ACE_TEXT ("  %s\n")
    ACE_TEXT ("  group_create  -group <group> -policy <round | rand | least> \n")
    ACE_TEXT ("  group_bind    -group <group> -name <name>\n")
    ACE_TEXT ("  group_unbind  -name <name>\n")
    ACE_TEXT ("  group_modify  -group <group> -policy <round | rand | least> \n")
    ACE_TEXT ("  group_list\n")
    ACE_TEXT ("  group_remove  -group <group>\n")
    ACE_TEXT ("  member_list   -group <group>\n")
    ACE_TEXT ("  member_add    -group <group> -location <location> -ior <IOR>\n")
    ACE_TEXT ("  member_remove -group <group> -location <location>\n")
    ACE_TEXT ("  member_show   -group <group> -location <location>\n")
    ACE_TEXT ("  -help\n")
    ACE_TEXT ("\n"),
    this->argv_[0]));
  return RC_SUCCESS;
}


//============================================================================
int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  const int RC_ERROR   = -1;
  const int RC_SUCCESS =  0;

  int rc = RC_ERROR;

  try
    {

      NSGROUP ns_group (argc, argv);

      if ( show_help (argc, argv) )
      {
        rc = ns_group.show_usage ();
      }
      else if ( RC_SUCCESS == ns_group.start_orb ())
      {
        rc = ns_group.run_cmd ();
      }

    }
  catch (const CORBA::Exception&)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Unable to run %C\n"),
                         argv[0]),
                         1);
    }

  return (rc == RC_SUCCESS) ? 0 : 1;
}
