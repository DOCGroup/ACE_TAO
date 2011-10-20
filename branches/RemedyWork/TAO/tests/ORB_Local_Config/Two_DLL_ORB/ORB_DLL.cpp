// -*- C++ -*-
// $Id$

#include "ORB_DLL.h"
#include "ace/Service_Config.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_strings.h"

#include "ace/Arg_Shifter.h"
#include "ace/Log_Msg.h"

Abstract_Worker::Abstract_Worker (const ACE_TCHAR *s)
  : ior_file_ (s)
{
}

//
Abstract_Worker::~Abstract_Worker (void)
{
}

//
Service_Config_ORB_DLL::Service_Config_ORB_DLL (void)
  : is_server_ (-1)
  , worker_ (0)
  , argv_ (0)
{
}

//
Service_Config_ORB_DLL::~Service_Config_ORB_DLL (void)
{
}

//
int
Service_Config_ORB_DLL::init (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT ("-c")) == 0)
        {
          this->is_server_ = 0;
          arg_shifter.consume_arg();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT ("-s")) == 0)
        {
          this->is_server_ = 1;
          arg_shifter.consume_arg();
        }
      else
        arg_shifter.ignore_arg();
    }

  if (this->is_server_ < 0)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) You must specify -c(lient) or -s(erver) argument. Aborting."), -1);

  ACE_TCHAR **temp_argv = new ACE_TCHAR*[argc+1];
  for (int i = 0; i < argc; i++)
    {
      temp_argv[i] = new ACE_TCHAR[ACE_OS::strlen(argv[i])+1];
      ACE_OS::strcpy (temp_argv[i],argv[i]);
    }
  temp_argv[argc] = 0;

  ACE_ARGV* tmp = 0;
  ACE_NEW_RETURN (tmp, ACE_ARGV (temp_argv), -1);
  this->argv_.reset (tmp);
  for (int i = 0; i < argc; i++)
    delete [] temp_argv[i];
  delete [] temp_argv;

  Abstract_Worker* worker = 0;
  if (this->is_server_)
  {
    ACE_NEW_RETURN (worker, Server_Worker, -1);
  }
  else
  {
    ACE_NEW_RETURN (worker, Client_Worker, -1);
  }
  this->worker_.reset (worker);

#if defined (ACE_HAS_THREADS)
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) About to activate %s, argv[%d]=\'%s\' ...\n"),
              this->worker_->kind (),
              this->argv_->argc (),
              this->argv_->buf ()));

  // Become an Active Object if more than one argument passed.
  // Two arguments indicate two "test objects" to be dynamically
  // loaded.
  return this->activate ();
#else
  ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("(%P|%t) Threading support is required for this test. Aborting.")), -1);
#endif  /* ACE_HAS_THREADS */
}

int
Service_Config_ORB_DLL::svc (void)
{
  ACE_ASSERT (this->worker_.get () != 0);
  ACE_ASSERT (this->argv_.get () != 0);

  try
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) %@ %s is active, argv[%d]=\'%s\'\n"),
                this,
                this->worker_->kind (),
                this->argv_->argc (),
                this->argv_->buf ()));

    int ret = this->worker_->test_main (this->argv_->argc (),
                                        this->argv_->argv ());

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) %@ %s bows out - so long, cruel world! (%d)\n"),
                this,
                this->worker_->kind (),
                ret));
    return ret;
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (ACE_TEXT("Failure:"));
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Aborting.\n")),
                       -1);
  }
}

// Define our service for using with the Service Configurator
ACE_FACTORY_DEFINE (Service_Config_ORB_DLL, Service_Config_ORB_DLL)
