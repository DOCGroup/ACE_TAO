// $Id$

#ifndef ACE_ADAPTER_SERVICE_CONFIG_H
#define ACE_ADAPTER_SERVICE_CONFIG_H
#include "ace/pre.h"

#include "../../Service_Config.h"

#if !defined (ACE_DEFAULT_LOGGER_KEY_W)
# if defined (ACE_HAS_STREAM_PIPES)
#   define ACE_DEFAULT_LOGGER_KEY_W L"/tmp/server_daemon"
# else
#   define ACE_DEFAULT_LOGGER_KEY_W L"localhost:10012"
# endif /* ACE_HAS_STREAM_PIPES */
#endif /* ACE_DEFAULT_LOGGER_KEY */

class ACE_Service_Config_W : public ACE_Service_Config
{
public:

  ACE_Service_Config_W (int ignore_static_svcs = 1,
                        size_t size = ACE_Service_Config_W::MAX_SERVICES,
                        int signum = SIGHUP)
    : ACE_Service_Config (ignore_static_svcs, size, signum)
  {}

  ACE_Service_Config_W (const wchar_t program_name[],
                        const wchar_t *logger_key = ACE_DEFAULT_LOGGER_KEY_W)
    : ACE_Service_Config (ACE_TEXT_WCHAR_TO_CHAR (program_name), ACE_TEXT_WCHAR_TO_CHAR (logger_key))
  {}

  static int open_i (const wchar_t program_name[],
                     const wchar_t *logger_key = ACE_DEFAULT_LOGGER_KEY_W,
                     int ignore_default_svc_conf_file = 0,
                     int ignore_debug_flag = 0)
  {
    return ACE_Service_Config::open_i (ACE_TEXT_WCHAR_TO_CHAR (program_name), 
                                       ACE_TEXT_WCHAR_TO_CHAR (logger_key),
                                       ignore_default_svc_conf_file,
                                       ignore_debug_flag);
  }

  static int open (const wchar_t program_name[],
                   const wchar_t *logger_key = ACE_DEFAULT_LOGGER_KEY_W,
                   int ignore_static_svcs = 1,
                   int ignore_default_svc_conf_file = 0,
                   int ignore_debug_flag = 0)
  {
    return ACE_Service_Config::open (ACE_TEXT_WCHAR_TO_CHAR (program_name), 
                                     ACE_TEXT_WCHAR_TO_CHAR (logger_key),
                                     ignore_static_svcs,
                                     ignore_default_svc_conf_file,
                                     ignore_debug_flag);
  }

  // @@ Implement:  static int open (int argc,
  //                                 wchar_t *argv[],
  //                                 const wchar_t *logger_key = ACE_DEFAULT_LOGGER_KEY_W,
  //                                 int ignore_static_svcs = 1,
  //                                 int ignore_default_svc_conf = 0,
  //                                 int ignore_debug_flag = 0);

  virtual ~ACE_Service_Config_W (void)
  {}

  // @@ Implement: static int initialize (const ACE_Service_Type *, wchar_t parameters[]);

  // @@ Implement:   static int initialize (const wchar_t svc_name[], wchar_t parameters[])
  
  static int resume (const wchar_t svc_name[])
  { return ACE_Service_Config::resume (ACE_TEXT_WCHAR_TO_CHAR (svc_name)); }
  
  static int suspend (const wchar_t svc_name[])
  { return ACE_Service_Config::suspend (ACE_TEXT_WCHAR_TO_CHAR (svc_name)); }
  
  static int remove (const wchar_t svc_name[])
  { return ACE_Service_Config::remove (ACE_TEXT_WCHAR_TO_CHAR (svc_name)); }
  
  static int process_directive (const wchar_t directive[])
  { return ACE_Service_Config::process_directive(ACE_TEXT_WCHAR_TO_CHAR (directive)); }

protected:
  // @@ Implement: static int parse_args (int, wchar_t *argv[])
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_SERVICE_CONFIG_H */
