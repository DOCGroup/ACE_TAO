// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION 
//    Test client for the AVStreams demo
//   
// = AUTHORS
//   Sumedh Mungee <sumedh@cs.wustl.edu>
//
// ============================================================================

#include "client.h"

Client::Client (void)
{
}

// initialize the ORB, get a grip on the remote mmdevice, and store it
// in this->remote_mmdevice_
// Also create a stream controlller and a local mmdevice

int
Client::init (int argc, 
              char **argv,
              CORBA::Environment &env)
{
  // init the ORB
  manager_.init (argc,
                 argv,
                 "avstreams_poa",
                 env);
  TAO_CHECK_ENV_RETURN (env, 1);

  // create the local mmdevice
  TAO_MMDevice *mmdevice_impl = new TAO_MMDevice;
    this->local_mmdevice_ = mmdevice_impl->_this (env);
  TAO_CHECK_ENV_RETURN (env, 1);

  // create the local streamctrl
  TAO_StreamCtrl *stream_ctrl = new TAO_StreamCtrl (manager_.orb ());
  this->stream_ctrl_ = stream_ctrl->_this (env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // bind to a remote mmdevice, as supplied by argc argv
  this->bind_to_remote_mmdevice (argc, argv, env);

  // create a local mmdevice for now..
  //  mmdevice_impl = new TAO_MMDevice;
  //  this->remote_mmdevice_ = mmdevice_impl->_this (env);

  TAO_CHECK_ENV_RETURN (env, 1);
}

// Use stream_ctrl_ to bind local_mmdevice_ and remote_mmdevice_
int
Client::run (CORBA::Environment &env)
{
  AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);
  AVStreams::flowSpec_var the_flows (new AVStreams::flowSpec);

  struct AVStreams::QoS qos_list;
  qos_list.QoSType = CORBA::string_alloc (64);
  qos_list.QoSParams = CORBA::string_alloc (64);
  strcpy (qos_list.QoSType, "foo");
  strcpy (qos_list.QoSParams, "bar");

  the_qos->length (1);
  the_qos [0] = qos_list;

  const char *flow_list [] =
  {
    "alpha",
    "beta",
    "gamma"
  };

  CORBA::ULong len = sizeof (flow_list)/sizeof (char *);

  // set the length of the sequence
  the_flows->length (len);


  // now set each individual element
  for (CORBA::ULong i=0; i < the_flows->length (); i++)
    {
      // generate some arbitrary string to be filled into the ith location in
      // the sequence
      //      char *str = gen->gen_string ();
      //this->in_[i] = str;
      the_flows [i] = flow_list [i];
    }

  this->stream_ctrl_->bind_devs (local_mmdevice_,
                                 remote_mmdevice_,
                                 the_qos.inout (),
                                 the_flows.in (),
                                 env);
  TAO_CHECK_ENV_RETURN (env, 1);
  return 0;
}

// bind to a remote mmdevice, as given by the command line arguments.
int
Client::bind_to_remote_mmdevice (int argc,
                                 char **argv,
                                 CORBA::Environment &env)
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;
  char *ior = 0;
  
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
	ior = 
          ACE_OS::strdup (get_opts.optarg);
	break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-k mmdevice_key]"
                           "\n",
                           argv [0]),
                          -1);
      }
  if (ior == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-k mmdevice_key]"
                           "\n",
                           argv [0]),
                          -1);
  
  CORBA::Object_var mmdevice_object = 
    this->manager_.orb ()->string_to_object (ior,
                                             env);
  TAO_CHECK_ENV_RETURN (env, 1);

  this->remote_mmdevice_ = 
    AVStreams::MMDevice::_narrow (mmdevice_object.in (),
                                  env);
  TAO_CHECK_ENV_RETURN (env, 1);

  if (CORBA::is_nil (this->remote_mmdevice_.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "invalid key <%s>\n",
                         ior),
                        -1);
    }

  ACE_DEBUG ((LM_DEBUG, "Object received OK\n"));
  
  ACE_DEBUG ((LM_DEBUG, 
              "\n(%P|%t) Bound to remote mmdevice"));

  return 0;
}

int
main (int argc, char **argv)
{
  TAO_TRY
    {
      Client client;
      if (client.init (argc, 
                       argv, 
                       TAO_TRY_ENV) == -1)
        return 1;
      TAO_CHECK_ENV;
      client.run (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("AVStreams: client");
      return -1;
    }
  TAO_ENDTRY;
}
