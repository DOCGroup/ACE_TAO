// $Id$

#include "tao/Dynamic_TP/Dynamic_TP_Config.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/Dynamic_TP/Dynamic_TP_ORBInitializer.h"

#include "tao/debug.h"
//#include "tao/ORB_Constants.h"
//#include "tao/ORBInitializer_Registry.h"
//#include "tao/SystemException.h"
#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_strings.h"
//#include "ace/Arg_Shifter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Dynamic_TP_Config_Registry_Installer::TAO_Dynamic_TP_Config_Registry_Installer (void)
{
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_Dynamic_TP_Config_Registry);
}


TAO_Dynamic_TP_Config_Registry::TAO_Dynamic_TP_Config_Registry (void)
{
}

TAO_Dynamic_TP_Config_Registry::~TAO_Dynamic_TP_Config_Registry (void)
{
}

int
TAO_Dynamic_TP_Config_Registry::init (int , ACE_TCHAR* [] )
{
  return 0;
}

bool
TAO_Dynamic_TP_Config_Registry::find (const ACE_CString& name, TAO_DTP_Definition &entry)
{
  return registry_.find (name, entry) == 0;
}

int
TAO_Dynamic_TP_Config_Registry::bind (const ACE_CString& name, TAO_DTP_Definition &entry)
{
  return registry_.bind (name, entry);
}

int
TAO_Dynamic_TP_Config_Registry::rebind (const ACE_CString& name, TAO_DTP_Definition &entry)
{
  return registry_.rebind (name, entry);
}

//--------------------------------------------------------------------------------------

TAO_Dynamic_TP_Config::TAO_Dynamic_TP_Config (void)
{
}

TAO_Dynamic_TP_Config::~TAO_Dynamic_TP_Config (void)
{
}

int
TAO_Dynamic_TP_Config::init (int argc, ACE_TCHAR* argv[])
{
  TAO_DTP_Definition entry =  {-1,5,-1,0,60,0};
  bool min_threads_set = false;
  bool init_threads_set = false;
  bool max_threads_set = false;
  bool timeout_set = false;

  ACE_TCHAR *name = 0;
  bool overwrite = false;

  int curarg = 0;
  int r = 0;

  for (curarg = 0; curarg < argc; ++curarg)
    {
      long val = 0;
      ACE_TCHAR *sval = 0;
      if ((r = this->parse_string (curarg, argc, argv, ACE_TEXT("-DTPName"),name )) != 0)
        {
          if (r < 0)
            {
              return -1;
            }
        }
      else if ((r = this->parse_bool (curarg, argc, argv, ACE_TEXT("-DTPOverwrite"), overwrite )) != 0)
        {
          if (r < 0)
            {
              return -1;
            }
        }
      else if ((r = this->parse_long (curarg, argc, argv, ACE_TEXT("-DTPMin"), val )) != 0)
        {
          if (r < 0)
            {
              return -1;
            }
          min_threads_set = true;
          entry.min_threads_ = val;
        }
      else if ((r = this->parse_long (curarg, argc, argv, ACE_TEXT("-DTPInit"), val )) != 0)
        {
          if (r < 0)
            {
              return -1;
            }
          init_threads_set = true;
          entry.init_threads_ = val;
        }
      else if ((r = this->parse_long (curarg, argc, argv, ACE_TEXT("-DTPMax"), val )) != 0)
        {
          if (r < 0)
            {
              return -1;
            }
          max_threads_set = true;
          entry.max_threads_ = val;
        }
      else if ((r = this->parse_long (curarg, argc, argv, ACE_TEXT("-DTPStack"), val )) != 0)
        {
          if (r < 0)
            {
              return -1;
            }
          entry.stack_size_ = val;
        }
      else if ((r = this->parse_long (curarg, argc, argv, ACE_TEXT("-DTPTImeout"), val )) != 0)
        {
          if (r < 0)
            {
              return -1;
            }
          timeout_set = true;
          entry.timeout_ = val;
        }
      else if ((r = this->parse_long (curarg, argc, argv, ACE_TEXT("-DTPQueue"), val )) != 0)
        {
          if (r < 0)
            {
              return -1;
            }
          entry.queue_depth_ = val;
        }
      else
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Config - Unrecognized argv[%d], %C\n"),
                         curarg, argv[curarg]));
            }
          return -1;
        }
    }

  ACE_CString name_str = name;

  bool valid = true;
  if (min_threads_set)
    {
      timeout_set = true;
      valid = (entry.min_threads_ > 0);
      if (valid)
        {
          if (init_threads_set)
            {
              valid = entry.init_threads_ >= entry.min_threads_;
            }
          else
            {
              entry.init_threads_ = entry.min_threads_;
            }
        }
    }
  else if (timeout_set)
    {
      entry.min_threads_ = 1;
    }

  if (valid)
    {
      if (max_threads_set)
        {
          valid = (entry.max_threads_ == -1) || (entry.max_threads_ > entry.init_threads_);
        }
      else
        {
          entry.max_threads_ = timeout_set ? -1 : entry.init_threads_;
        }
    }

  if (!valid)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Config - bad thread limits min = %d ")
                     ACE_TEXT ("initial = %d max = %d\n"),
                     entry.min_threads_, entry.init_threads_, entry.max_threads_));
        }
      return -1;
    }

  ACE_Service_Gestalt *current = ACE_Service_Config::current();
  TAO_Dynamic_TP_Config_Registry* registry =
    ACE_Dynamic_Service<TAO_Dynamic_TP_Config_Registry>::instance
    (current, "Dynamic_TP_Config_Registry", true);

  if (registry == 0)
    {
      current->process_directive (ace_svc_desc_TAO_Dynamic_TP_Config_Registry);
      registry = ACE_Dynamic_Service<TAO_Dynamic_TP_Config_Registry>::instance
        (current, "Dynamic_TP_Config_Registry", true);
      if (registry == 0)
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG((LM_DEBUG,
                         ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Config - cannot initialize registry\n")));
            }
          return -1;
        }
    }

  if (!overwrite)
    {
      return registry->bind(name_str, entry);
    }
  else
    {
      return registry->rebind(name_str, entry);
    }
  return 0;
}

int
TAO_Dynamic_TP_Config::parse_long (int &curarg,
                                   int argc, ACE_TCHAR *argv[],
                                   const ACE_TCHAR *match, long &value)
{
  ACE_TCHAR *str;
  int result = this->parse_string (curarg, argc, argv, match, str);
  if (result < 1)
    return result;

  ACE_TCHAR *err = 0;
  value = ACE_OS::strtol (str, &err, 10);
  if (err && *err != 0)
    {
      this->report_option_value_error (match, str);
      return -1;
    }
  return 1;
}

int
TAO_Dynamic_TP_Config::parse_bool (int &curarg,
                                   int argc, ACE_TCHAR *argv[],
                                   const ACE_TCHAR *match, bool &value)
{
  long num;
  int result = this->parse_long (curarg, argc, argv, match, num);
  if (result < 1)
    return result;

  value = num != 0;

  return 1;
}

int
TAO_Dynamic_TP_Config::parse_string (int &curarg,
                                     int argc, ACE_TCHAR *argv[],
                                     const ACE_TCHAR *match, ACE_TCHAR *&value)
{
  if (ACE_OS::strcasecmp (argv[curarg], match) != 0)
    return 0;

  ++curarg;
  if (curarg >= argc)
    {
      this->report_option_value_error (match, ACE_TEXT("<missing>"));
      return -1;
    }

  value = argv[curarg];
  return 1;
}


void
TAO_Dynamic_TP_Config::report_option_value_error (const ACE_TCHAR* option_name,
                                                  const ACE_TCHAR* option_value)
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Config - unknown ")
                 ACE_TEXT ("argument <%s> for <%s>\n"),
                 option_value, option_name));
    }
}


/////////////////////////////////////////////////////////////////////

ACE_FACTORY_DEFINE (TAO_Dynamic_TP, TAO_Dynamic_TP_Config_Registry)
ACE_STATIC_SVC_DEFINE (TAO_Dynamic_TP_Config_Registry,
                       ACE_TEXT ("Dynamic_TP_Config_Registry"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Dynamic_TP_Config_Registry),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_Dynamic_TP, TAO_Dynamic_TP_Config)
ACE_STATIC_SVC_DEFINE (TAO_Dynamic_TP_Config,
                       ACE_TEXT ("Dynamic_TP_Config"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Dynamic_TP_Config),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0 */

