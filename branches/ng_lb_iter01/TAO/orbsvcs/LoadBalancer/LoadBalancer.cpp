// -*- C++ -*-

#include "LoadBalancer.h"
#include "ace/Get_Opt.h"

#include "orbsvcs/LoadBalancing/LoadBalancingI.h"

ACE_RCSID (LoadBalancer,
           LoadBalancer,
           "$Id$")


TAO_LoadBalancer::TAO_LoadBalancer (void)
  : load_balancer_file_ ("lb.ior"),
    balancer_ (0)
{
}

TAO_LoadBalancer::~TAO_LoadBalancer (void)
{
  // No-op
}

void
TAO_LoadBalancer::parse_args (int argc,
                              char *argv[],
                              CORBA::Environment &ACE_TRY_ENV)
{
  ACE_Get_Opt get_opts (argc, argv, "o:s:h");

  int c = 0;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'o':
          this->load_balancer_file_ = get_opts.optarg;
          break;

//         case 's':
//           if (ACE_OS::strcasecmp (get_opts.optarg, "rr") == 0)
//             this->strategy_ = 0;
//           else if (ACE_OS::strcasecmp (get_opts.optarg, "md") == 0)
//             this->strategy_ = 1;
//           else
//             ACE_DEBUG ((LM_DEBUG,
//                         ACE_TEXT ("Unknown strategy, using rr\n")));
//           break;

        case 'h':
          this->usage (argv[0]);
          break;

        default:
          this->usage (argv[0]);
          ACE_THROW (CORBA::BAD_PARAM ());
        }
    }
}

void
TAO_LoadBalancer::init (int argc,
                        char *argv[],
                        CORBA::Environment &ACE_TRY_ENV)
{
  this->orb_ = CORBA::ORB_init (argc, argv, 0, ACE_TRY_ENV);
  ACE_CHECK;

  // Get the POA from the ORB.
  CORBA::Object_var poa =
    this->orb_->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
  ACE_CHECK;

  // Get the POA object.
  this->root_poa_ = PortableServer::POA::_narrow (poa.in (),
                                                  ACE_TRY_ENV);
  ACE_CHECK;

  // Get the POA_Manager.
  PortableServer::POAManager_var poa_manager =
    this->root_poa_->the_POAManager (ACE_TRY_ENV);
  ACE_CHECK;

  poa_manager->activate (ACE_TRY_ENV);
  ACE_CHECK;

  // Check the non-ORB arguments.
  this->parse_args (argc, argv, ACE_TRY_ENV);
  ACE_CHECK;

  // Initialize the LoadBalancer servant.
//   TAO_LB_LoadBalancing_Strategy *strategy =
//     &this->round_robin_;
//   if (this->strategy_ == 1)
//     strategy = &this->minimum_dispersion_;

  ACE_NEW (this->balancer_,
           TAO_LoadBalancing_ReplicationManager_i (this->root_poa_.in ()));

  CORBA::Object_var obj =
    this->balancer_->_this (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::String_var str =
    this->orb_->object_to_string (obj.in (), ACE_TRY_ENV);
  ACE_CHECK;

  FILE *lb_ior = ACE_OS::fopen (this->load_balancer_file_, "w");
  ACE_OS::fprintf (lb_ior, "%s", str.in ());
  ACE_OS::fclose (lb_ior);
}

void
TAO_LoadBalancer::usage (const char *cmd) const
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Usage:  %s ")
              ACE_TEXT ("-o <ior_output_file> ")
              ACE_TEXT ("-s <rr|md> ")
              ACE_TEXT ("-h ")
              ACE_TEXT ("\n"),
              cmd));
}

void
TAO_LoadBalancer::run (CORBA::Environment &ACE_TRY_ENV)
{
  this->orb_->run (ACE_TRY_ENV);
}
