// -*- C++ -*-
// $Id$

#include "Latency_Query_Client.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"

ACE_RCSID (Latency_Test, 
           Latency_Query_Client, 
           "$Id$")

const CORBA::ULong DEFAULT_NUMCALLS = 20000;

Latency_Query_Client::Latency_Query_Client (void)
  : debug_ (0),
    do_dump_history_ (0),
    iterations_ (DEFAULT_NUMCALLS)
{
}

Latency_Query_Client::~Latency_Query_Client (void)
{
}

int
Latency_Query_Client::init (int argc,
                            char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    0
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int retval = this->parse_args (argc,
                                     argv);

      if (retval != 0)
        {
          return retval;
        }

      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("InterfaceRepository"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (object.in ()))
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "Null objref from resolve_initial_references\n"
            ),
            -1
          );
        }

      this->repo_ =
        CORBA::Repository::_narrow (object.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (this->repo_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "CORBA::Repository::_narrow failed\n"),
                            -1);
        }

      retval = this->populate_ifr (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (retval != 0)
        {
          return retval;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Latency_Query_Client::init:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
Latency_Query_Client::run (void)
{
//  CORBA::DefinitionKind dk;
  CORBA::AttributeMode am;

  ACE_TRY_NEW_ENV
    {
      for (int j = 0; j < 100; ++j)
        {
          am = this->attr_->mode (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (am != CORBA::ATTR_NORMAL)
            {
              return -1;
            }
/*
          dk = this->tdef_->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (dk != CORBA::dk_Alias)
            {
              return -1;
            }
*/
        }

      ACE_Sample_History history (this->iterations_);
      ACE_hrtime_t test_start = ACE_OS::gethrtime ();

      for (CORBA::ULong i = 0; i < this->iterations_; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();

          am = this->attr_->mode (ACE_ENV_SINGLE_ARG_PARAMETER);
//          dk = this->tdef_->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_hrtime_t now = ACE_OS::gethrtime ();
          history.sample (now - start);
        }

      ACE_hrtime_t test_end = ACE_OS::gethrtime ();

      ACE_DEBUG ((LM_DEBUG, 
                  "test finished\n"));
      ACE_DEBUG ((LM_DEBUG, 
                  "High resolution timer calibration...."));
      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, 
                  "done\n"));

      if (this->do_dump_history_)
        {
          history.dump_samples ("HISTORY", gsf);
        }

      ACE_Basic_Stats stats;
      history.collect_basic_stats (stats);
      stats.dump_results ("Total", gsf);

      ACE_Throughput_Stats::dump_throughput ("Total", 
											                       gsf,
                                             test_end - test_start,
                                             stats.samples_count ());

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, 
                          "Latency_Query_Client::run:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
Latency_Query_Client::parse_args (int argc,
                                  char *argv[])
{
  ACE_Get_Opt opts (argc, argv, "dhi:");
  int c;
  int result = 0;

  while ((c = opts ()) != -1)
    switch (c)
      {
      case 'd':
        this->debug_ = 1;
        break;
      case 'h': 
        this->do_dump_history_ = 1;
        break;
      case 'i': 
        result = ACE_OS::atoi (opts.opt_arg ());

        if (result > 0)
          {
            this->iterations_ = result;
          }

        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s"
                           " [-d]"
                           " [-i iterations]"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
Latency_Query_Client::populate_ifr (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Contained_var irobj = this->repo_->lookup_id ("IDL:dummy/attr:1.0"
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (! CORBA::is_nil (irobj.in ()))
    {
      this->attr_ = CORBA::AttributeDef::_narrow (irobj.in ()
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (CORBA::is_nil (this->attr_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Latency_Query_Client::populate_ifr - "
                             "AttributeDef::_narrow returned null\n"),
                            -1);
        }

      return 0;
    }

  CORBA::InterfaceDefSeq in_bases (0);
  in_bases.length (0);

  CORBA::InterfaceDef_var iface =
    this->repo_->create_interface ("IDL:dummy:1.0",
                                   "dummy",
                                   "1.0",
                                   in_bases
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::PrimitiveDef_var p_long =
    this->repo_->get_primitive (CORBA::pk_long
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->attr_ =
    iface->create_attribute ("IDL:dummt/attr:1.0",
                             "attr",
                             "1.0",
                             p_long.in (),
                             CORBA::ATTR_NORMAL
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
/*
  CORBA::Contained_var irobj = this->repo_->lookup_id ("IDL:longtype:1.0"
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (! CORBA::is_nil (irobj.in ()))
    {
      this->tdef_ = CORBA::AliasDef::_narrow (irobj.in ()
                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (CORBA::is_nil (this->tdef_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Latency_Query_Client::populate_ifr - "
                             "AliasDef::_narrow returned null\n"),
                            -1);
        }

      return 0;
    }

  CORBA::PrimitiveDef_var longdef = 
    this->repo_->get_primitive (CORBA::pk_long
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (longdef.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Latency_Query_Client::populate_ifr - "
                         "get_primitive returned null\n"),
                        -1);
    }

  this->tdef_ = 
    this->repo_->create_alias ("IDL:longtype:1.0",
                               "longtype",
                               "1.0",
                               longdef.in ()
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (this->tdef_.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Latency_Query_Client::populate_ifr - "
                         "create_alias returned null\n"),
                        -1);
    }
*/
  return 0;
}

