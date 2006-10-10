/**
 * $Id$
 *
 * Home Automation Status server. Sample code from The ACE Programmer's Guide,
 * Copyright 2003 Addison-Wesley. All Rights Reserved.
 */

#include "ace/OS_NS_string.h"
#include "ace/Configuration.h"
#include "ace/Configuration_Import_Export.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/INET_Addr.h"
#include "ace/Service_Object.h"

class HA_Status : public ACE_Service_Object
{
public:
  virtual int init (int argc, ACE_TCHAR *argv[]);

private:
  ACE_INET_Addr listen_addr_;
};


int
HA_Status::init (int argc, ACE_TCHAR *argv[])
{

  // Do ACE_Get_Opt and get conf file name, read out the sections
  // and print the names.

  // Listing 1 code/ch04
  static const ACE_TCHAR options[] = ACE_TEXT (":f:");
  ACE_Get_Opt cmd_opts (argc, argv, options);
  if (cmd_opts.long_option
      (ACE_TEXT ("config"), 'f', ACE_Get_Opt::ARG_REQUIRED) == -1)
    return -1;
  int option;
  ACE_TCHAR config_file[MAXPATHLEN];
  ACE_OS::strcpy (config_file, ACE_TEXT ("HAStatus.conf"));
  while ((option = cmd_opts ()) != EOF)
    switch (option) {
    case 'f':
      ACE_OS::strncpy (config_file,
                       cmd_opts.opt_arg (),
                       MAXPATHLEN);
      break;
    case ':':
      ACE_ERROR_RETURN
        ((LM_ERROR, ACE_TEXT ("-%c requires an argument\n"),
          cmd_opts.opt_opt ()), -1);
    default:
      ACE_ERROR_RETURN
        ((LM_ERROR, ACE_TEXT ("Parse error.\n")), -1);
    }
  // Listing 1

  // Listing 2 code/ch04
  ACE_Configuration_Heap config;
  if (config.open () == -1)
    ACE_ERROR_RETURN
      ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("config")), -1);
  ACE_Registry_ImpExp config_importer (config);
  if (config_importer.import_config (config_file) == -1)
    ACE_ERROR_RETURN
      ((LM_ERROR, ACE_TEXT ("%p\n"), config_file), -1);

  ACE_Configuration_Section_Key status_section;
  if (config.open_section (config.root_section (),
                           ACE_TEXT ("HAStatus"),
                           0,
                           status_section) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Can't open HAStatus section")),
                      -1);

  u_int status_port;
  if (config.get_integer_value (status_section,
                                ACE_TEXT ("ListenPort"),
                                status_port) == -1)
    ACE_ERROR_RETURN
      ((LM_ERROR,
        ACE_TEXT ("HAStatus ListenPort does not exist\n")),
       -1);
  this->listen_addr_.set (static_cast<u_short> (status_port));
  // Listing 2

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  HA_Status status;
  status.init (argc, argv);
  return 0;
}
