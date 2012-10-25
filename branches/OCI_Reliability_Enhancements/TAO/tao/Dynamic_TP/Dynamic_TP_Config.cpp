// $Id$

#include "tao/Dynamic_TP/Dynamic_TP_Config.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/Dynamic_TP/Dynamic_TP_ORBInitializer.h"

#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/SystemException.h"
#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_strings.h"
#include "ace/Arg_Shifter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Dynamic_TP_Config::TAO_Dynamic_TP_Config (void)
{
}

TAO_Dynamic_TP_Config::~TAO_Dynamic_TP_Config (void)
{
}

int
TAO_Dynamic_TP_Config::init (int argc, ACE_TCHAR* argv[])
{
  TP_Definition entry = {5,5,5,0,TP_INFINITE,60,0};
  bool min_threads_set = false;
  bool init_threads_set = false;
  bool max_threads_set = false;

  ACE_TCHAR *name = 0;
  bool overwrite = false;

  int curarg = 0;

  for (curarg = 0; curarg < argc; ++curarg)
    {
      int r = 0;
      long val = 0;
      ACE_TCHAR *sval = 0;
      if ((r = this->parse_string (curarg, argc, argv, ACE_TEXT("-DTPName"),name )) != 0)
        {
          if (r < 0)
            {
              break;
            }
        }
      else if ((r = this->parse_bool (curarg, argc, argv, ACE_TEXT("-DTPOverwrite"), overwrite )) != 0)
        {
          if (r < 0)
            {
              break;
            }
        }
      else if ((r = this->parse_long (curarg, argc, argv, ACE_TEXT("-DTPMin"), val )) != 0)
        {
          if (r < 0)
            {
              break;
            }
          min_threads_set = true;
          entry.min_threads_ = val;
          if (!init_threads_set)
            {
              entry.init_threads_ = entry.min_threads_;
              if (!max_threads_set)
                {
                  entry.max_threads_ = entry.init_threads_;
                }
            }
        }
      else if ((r = this->parse_long (curarg, argc, argv, ACE_TEXT("-DTPInit"), val )) != 0)
        {
          if (r < 0)
            {
              break;
            }
          entry.init_threads_ = val;
          if (!max_threads_set)
            {
              entry.max_threads_ = entry.init_threads_;
            }
        }
      else if ((r = this->parse_long (curarg, argc, argv, ACE_TEXT("-DTPMax"), val )) != 0)
        {
          if (r < 0)
            {
              break;
            }
          max_threads_set = true;
          entry.max_threads_ = val;
        }
      else if ((r = this->parse_long (curarg, argc, argv, ACE_TEXT("-DTPStack"), val )) != 0)
        {
          if (r < 0)
            {
              break;
            }
          entry.stack_size_ = val;
        }
      else if ((r = this->parse_string (curarg, argc, argv, ACE_TEXT("-DTPLifespan"), sval)) != 0)
        {
          if (r < 0)
            {
              break;
            }

          if (ACE_OS::strcasecmp (argv[curarg], ACE_TEXT ("infinite")) == 0)
            {
              entry.lifespan_ = TP_INFINITE;
            }
          else if (ACE_OS::strcasecmp (argv[curarg], ACE_TEXT ("idle")) == 0)
            {
              entry.lifespan_ = TP_IDLE;
            }
          else if (ACE_OS::strcasecmp (argv[curarg], ACE_TEXT ("fixed")) == 0)
            {
              entry.lifespan_ = TP_FIXED;
            }
          else
            {
              this->report_option_value_error (ACE_TEXT("-DTPLifespan"), argv[curarg]);
              break;
            }
        }
      else if ((r = this->parse_long (curarg, argc, argv, ACE_TEXT("-DTPTimeout"), val )) != 0)
        {
          if (r < 0)
            {
              break;
            }
          entry.timeout_ = val;
        }
      else if ((r = this->parse_long (curarg, argc, argv, ACE_TEXT("-DTPQueue"), val )) != 0)
        {
          if (r < 0)
            {
              break;
            }
          entry.queue_depth_ = val;
        }
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
    return 1;

  ACE_TCHAR *err = 0;
  value = ACE_OS::strtol (str, &err, 10);
  if (err)
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
    return 1;

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
  ACE_DEBUG((LM_DEBUG,
             ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Config - unknown ")
             ACE_TEXT ("argument <%s> for <%s>\n"),
             option_value, option_name));
}



/////////////////////////////////////////////////////////////////////

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

