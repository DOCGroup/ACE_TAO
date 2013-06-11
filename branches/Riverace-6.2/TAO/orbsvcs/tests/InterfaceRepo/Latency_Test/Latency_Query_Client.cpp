// -*- C++ -*-
// $Id$

#include "Latency_Query_Client.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/Sample_History.h"

const CORBA::ULong DEFAULT_NUMCALLS = 20000;

Latency_Query_Client::Latency_Query_Client (void)
  : debug_ (false),
    do_dump_history_ (0),
    iterations_ (DEFAULT_NUMCALLS)
{
}

Latency_Query_Client::~Latency_Query_Client (void)
{
}

int
Latency_Query_Client::init (int argc,
                            ACE_TCHAR *argv[])
{
  try
    {
      this->orb_ = CORBA::ORB_init (argc, argv);

      int retval = this->parse_args (argc, argv);

      if (retval != 0)
        {
          return retval;
        }

      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("InterfaceRepository");

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
        CORBA::Repository::_narrow (object.in ());

      if (CORBA::is_nil (this->repo_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "CORBA::Repository::_narrow failed\n"),
                            -1);
        }

      retval = this->populate_ifr ();

      if (retval != 0)
        {
          return retval;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Latency_Query_Client::init:");
      return -1;
    }

  return 0;
}

int
Latency_Query_Client::run (void)
{
//  CORBA::DefinitionKind dk;
  CORBA::AttributeMode am;

  try
    {
      for (int j = 0; j < 100; ++j)
        {
          am = this->attr_->mode ();

          if (am != CORBA::ATTR_NORMAL)
            {
              return -1;
            }
        }

      ACE_Sample_History history (this->iterations_);
      ACE_hrtime_t test_start = ACE_OS::gethrtime ();

      for (CORBA::ULong i = 0; i < this->iterations_; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();

          am = this->attr_->mode ();

          ACE_hrtime_t now = ACE_OS::gethrtime ();
          history.sample (now - start);
        }

      ACE_hrtime_t test_end = ACE_OS::gethrtime ();

      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "test finished\n"));
          ACE_DEBUG ((LM_DEBUG,
                      "High resolution timer calibration...."));
          ACE_High_Res_Timer::global_scale_factor_type gsf =
            ACE_High_Res_Timer::global_scale_factor ();
          ACE_DEBUG ((LM_DEBUG,
                      "done\n"));

          if (this->do_dump_history_)
            {
              history.dump_samples (ACE_TEXT("HISTORY"), gsf);
            }

          ACE_Basic_Stats stats;
          history.collect_basic_stats (stats);
          stats.dump_results (ACE_TEXT("Total"), gsf);

          ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"),
                                                 gsf,
                                                 test_end - test_start,
                                                 stats.samples_count ());
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Latency_Query_Client::run:");
      return -1;
    }

  return 0;
}

int
Latency_Query_Client::parse_args (int argc,
                                  ACE_TCHAR *argv[])
{
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("dhi:"));
  int c;
  int result = 0;

  while ((c = opts ()) != -1)
    {
      switch (c)
        {
        case 'd':
          this->debug_ = true;
          break;
        case 'h':
          this->do_dump_history_ = true;
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
    }

  return 0;
}

int
Latency_Query_Client::populate_ifr (void)
{
  CORBA::Contained_var irobj = this->repo_->lookup_id ("IDL:dummy/attr:1.0");

  if (! CORBA::is_nil (irobj.in ()))
    {
      this->attr_ = CORBA::AttributeDef::_narrow (irobj.in ());

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
                                   in_bases);

  CORBA::PrimitiveDef_var p_long =
    this->repo_->get_primitive (CORBA::pk_long);

  this->attr_ =
    iface->create_attribute ("IDL:dummt/attr:1.0",
                             "attr",
                             "1.0",
                             p_long.in (),
                             CORBA::ATTR_NORMAL);

  return 0;
}
