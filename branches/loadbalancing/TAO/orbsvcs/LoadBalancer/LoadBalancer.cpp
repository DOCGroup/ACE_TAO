// -*- C++ -*-

// $Id$

ACE_RCSID(LoadBalancer, LoadBalancer, "$Id$")


TAO_LoadBalancer::TAO_LoadBalancer (void)
{
}

void
TAO_LoadBalancer::parse_args (int argc,
                              char *argv[],
                              CORBA::Environment &ACE_TRY_ENV)
{
  ACE_Get_Opt get_opts (argc, argv, "o:h");

  int c = 0;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'o': // Write the Load Balancer IOR to the specified file.
          this->ior_file_ =
            ACE_OS::fopen (get_opts.optarg, "w");

          if (this->ior_file_ == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "Unable to open %s for writing: %p\n",
                          get_opts.optarg));
              ACE_THROW (CORBA::UNKNOWN ());
            }

          break;

        case 'h':
          this->usage ();
          break;

        default:
          this->usage ();
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

  if (CORBA::is_nil (poa.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("(%P|%t) Unable to initialize the ")
                  ASYS_TEXT ("POA.\n")));
      ACE_CHECK;
    }

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
  this->load_balancer_impl_.init (ACE_TRY_ENV);
  ACE_CHECK;

  if (this->ior_output_file_ != 0)
    {
      CORBA::String_var str =
        this->my_naming_server_.naming_service_ior ();
      ACE_OS::fprintf (this->ior_output_file_,
                       "%s",
                       str.in ());
      ACE_OS::fclose (this->ior_output_file_);
    }
}

void
TAO_LoadBalancer::usage (void) const
{
  ACE_DEBUG ((LM_INFO,
              ASYS_TEXT ("Usage:  %s ")
              ASYS_TEXT ("-o <ior_output_file> ")
              ASYS_TEXT ("\n"),
              argv [0]));
}
