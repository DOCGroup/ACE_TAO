// -*- C++ -*-
#include "ORB_DLL.h"
#include "ace/Service_Config.h"

#include "ace/Arg_Shifter.h"
#include "ace/Log_Msg.h"

Abstract_Worker::Abstract_Worker (ACE_TCHAR const *ior)
  : ior_file_ (ior)
{
}

Abstract_Worker::~Abstract_Worker () = default;

Service_Config_ORB_DLL::Service_Config_ORB_DLL () = default;

Service_Config_ORB_DLL::~Service_Config_ORB_DLL () = default;

int
Service_Config_ORB_DLL::init (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT ("-c")) == 0)
        {
          this->worker_type_ = Worker_Type::client;
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT ("-s")) == 0)
        {
          this->worker_type_ = Worker_Type::server;
          arg_shifter.consume_arg ();
        }
      else
        arg_shifter.ignore_arg ();
    }

  if (this->worker_type_ == Worker_Type::unspecified)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) You must specify -c(lient) or -s(erver).\n")),
                      -1);

  // ACE_NEW_RETURN needs a raw output pointer; argv_ takes ownership below.
  ACE_ARGV *tmp {};
  ACE_NEW_RETURN (tmp, ACE_ARGV (argc, argv), -1);
  this->argv_.reset (tmp);

  // ACE_NEW_RETURN needs a raw output pointer; worker_ takes ownership below.
  Abstract_Worker *worker {};
  if (this->worker_type_ == Worker_Type::server)
    ACE_NEW_RETURN (worker, Server_Worker, -1);
  else
    ACE_NEW_RETURN (worker, Client_Worker, -1);
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
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("(%P|%t) Threading support is required for this test.\n")),
                    -1);
#endif /* ACE_HAS_THREADS */
}

int
Service_Config_ORB_DLL::svc ()
{
  ACE_ASSERT (this->worker_ != nullptr);
  ACE_ASSERT (this->argv_ != nullptr);

  try
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) %@ %s is active, argv[%d]=\'%s\'\n"),
                  this,
                  this->worker_->kind (),
                  this->argv_->argc (),
                  this->argv_->buf ()));

      int const ret = this->worker_->test_main (this->argv_->argc (),
                                                this->argv_->argv ());

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) %@ %s bows out - so long, cruel world! (%d)\n"),
                  this,
                  this->worker_->kind (),
                  ret));
      return ret;
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Failure:"));
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) Aborting.\n")),
                        -1);
    }
}

// Define our service for using with the Service Configurator
ACE_FACTORY_DEFINE (Service_Config_ORB_DLL, Service_Config_ORB_DLL)
