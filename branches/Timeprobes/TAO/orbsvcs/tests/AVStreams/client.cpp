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

Video_Client_StreamEndPoint::Video_Client_StreamEndPoint (void)
{
}

void 
Video_Client_StreamEndPoint::handle_stop (const AVStreams::flowSpec &the_spec,
                                          CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) Video_Client_StreamEndPoint::handle_stop: called\n"));
}

void 
Video_Client_StreamEndPoint::handle_start (const AVStreams::flowSpec &the_spec,  
                                           CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) Video_Client_StreamEndPoint::handle_start: called\n"));
}

void 
Video_Client_StreamEndPoint::handle_destroy (const AVStreams::flowSpec &the_spec,  
                                             CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) Video_Client_StreamEndPoint::handle_destroy: called\n"));
}

CORBA::Boolean 
Video_Client_StreamEndPoint::handle_connection_established (AVStreams::StreamEndPoint_ptr responder, 
                                                            AVStreams::streamQoS &qos_spec, 
                                                            const AVStreams::flowSpec &the_spec,  
                                                            CORBA::Environment &env) 
{
  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) Video_Client_StreamEndPoint::handle_connection_established: called\n"));
  return 1;
}

Client::Client (void)
{
}

// initialize the ORB, get a grip on the remote mmdevice, and store it
// in this->remote_mmdevice_.  Also create a stream controlller and a
// local mmdevice.

int
Client::init (int argc, 
              char *argv[],
              CORBA::Environment &env)
{
  // Init the ORB.
  manager_.init (argc,
                 argv,
                 env);
  TAO_CHECK_ENV_RETURN (env, 1);

  // Create the local mmdevice.
  TAO_Client_MMDevice <Video_Client_StreamEndPoint> *mmdevice_impl;

  ACE_NEW_RETURN (mmdevice_impl,
                  TAO_Client_MMDevice <Video_Client_StreamEndPoint>,
                  -1);

  this->local_mmdevice_ = mmdevice_impl->_this (env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // Create the local streamctrl.
  TAO_StreamCtrl *stream_ctrl;

  ACE_NEW_RETURN (stream_ctrl,
                  TAO_StreamCtrl (manager_.orb ()),
                  -1);
  this->stream_ctrl_ = stream_ctrl->_this (env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // Bind to a remote mmdevice, as supplied by argc argv.
  this->bind_to_remote_mmdevice (argc, argv, env);

  manager_.orb ()->open ();
  // Create a locall mmdevice for now..
  //  mmdevice_impl = new TAO_MMDevice;
  //  this->remote_mmdevice_ = mmdevice_impl->_this (env);

  TAO_CHECK_ENV_RETURN (env, 1);
}

// Use stream_ctrl_ to bind local_mmdevice_ and remote_mmdevice_.

int
Client::run (CORBA::Environment &env)
{
  // Initialize the in parameters.

  // @@ Alex, please make sure you restructure this code so that you
  // test for allocation failures.
  AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);
  AVStreams::flowSpec_var the_flows (new AVStreams::flowSpec);

  // Allocate the qos_list.
  struct AVStreams::QoS qos_list;
  // @@ Alex, please take a careful look at this code and figure out
  // if you really need to use "magic numbers" like 64 here.
  qos_list.QoSType = CORBA::string_alloc (64);
  qos_list.QoSParams = CORBA::string_alloc (64);
  strcpy (qos_list.QoSType, "foo");
  strcpy (qos_list.QoSParams, "bar");

  the_qos->length (1);
  the_qos [0] = qos_list;

  // Allocate the flow_list.
  const char *flow_list [] =
  {
    "alpha",
    "beta",
    "gamma"
  };

  CORBA::ULong len = sizeof (flow_list)/sizeof (char *);

  // Set the length of the sequence.
  the_flows->length (len);

  // Now set each individual element.
  for (CORBA::ULong i=0; i < the_flows->length (); i++)
    {
      // Generate some arbitrary string to be filled into the ith
      // location in the sequence.
      //      char *str = gen->gen_string ();
      //this->in_[i] = str;
      the_flows [i] = flow_list [i];
    }

  // Bind the devices together.
  this->stream_ctrl_->bind_devs (local_mmdevice_,
                                 remote_mmdevice_,
                                 the_qos.inout (),
                                 the_flows.in (),
                                 env);
  
  TAO_CHECK_ENV_RETURN (env, 1);
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Devices bound successfully!\n"));
  
  // Start the flow of the stream.
  this->stream_ctrl_-> start (the_flows.in (),
                              env);

  TAO_CHECK_ENV_RETURN (env, 1);
  return 0;
}

// Bind to a remote mmdevice, as given by the command line arguments.

int
Client::bind_to_remote_mmdevice (int argc,
                                 char *argv[],
                                 CORBA::Environment &env)
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;
  char *ior = 0;
  
  // @@ Alex, can you please move this into a "parse_args()" helper
  // method?

  // Parse the command line.
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

  // We didn't get an IOR from the command line!
  if (ior == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-k mmdevice_key]"
                           "\n",
                           argv [0]),
                          -1);
  
  // Get the object reference.
  CORBA::Object_var mmdevice_object = 
    this->manager_.orb ()->string_to_object (ior,
                                             env);
  TAO_CHECK_ENV_RETURN (env, 1);

  // Narrow the reference.
  this->remote_mmdevice_ = 
    AVStreams::MMDevice::_narrow (mmdevice_object.in (),
                                  env);
  TAO_CHECK_ENV_RETURN (env, 1);

  if (CORBA::is_nil (this->remote_mmdevice_.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "invalid key <%s>\n",
                       ior),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              "Object received OK\n"));
  
  ACE_DEBUG ((LM_DEBUG, 
              "\n(%P|%t) Bound to remote mmdevice"));
  return 0;
}


// Testing the methods of the property service.

int 
Client::property_tester (CORBA::Environment &env)
{
  // Testing define_property () of PropertySet interface.
  this->test_define_property (env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // Test the number of properties.
  this->test_get_number_of_properties (env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // Testing get_all_property_names.
  this->test_get_all_property_names (env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // Testing delete property.
  this->test_delete_property ("string_property",env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // Testing get_properties.
  this->test_get_properties (env);
  TAO_CHECK_ENV_RETURN (env, 1);

  // Testing delete_properties.
  this->test_delete_properties (env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // Test the number of properties.
  this->test_get_number_of_properties (env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // Testing define_properties.
  this->test_define_properties (env);
  TAO_CHECK_ENV_RETURN (env, 1);

  // Test the number of properties.
  this->test_get_number_of_properties (env);
  TAO_CHECK_ENV_RETURN (env, 1);

  // Testing get_all_property_names.
  this->test_get_all_property_names (env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // Testing get_all_properties.
  this->test_get_all_properties (env);
  TAO_CHECK_ENV_RETURN (env, 1);
}

// Testing define_property.

int
Client::test_define_property (CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nChecking define_property\n"));
  
  CORBA::Any anyval;
 
  // Prepare a char and "define" that in the PropertySet.
  CORBA::Char ch = '#';
  anyval <<= from_char (ch);
  ch = '*';
  anyval >>= to_char (ch);

  ACE_DEBUG ((LM_DEBUG,
              "Main : Char ch = %c\n",
              ch));
  this->remote_mmdevice_->define_property ("char_property",
                                           anyval,
                                           env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // Prepare a Short and "define" that in the PropertySet.
  CORBA::Short s = 3;
  anyval <<= s; 
  s = 7; 
  anyval >>= s;
  
  ACE_DEBUG ((LM_DEBUG,
              "Main : Short s = %d\n",
              s));
  remote_mmdevice_->define_property ("short_property",
                                     anyval,
                                     env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // Prepare a Long and "define" that in the PropertySet.
  CORBA::Long l = 931232;
  anyval <<= l;
  l = 931233;
  anyval >>= l;
  ACE_DEBUG ((LM_DEBUG,
              "Main : Long l = %d\n",
              l));
  CORBA::Any newany(anyval);
  remote_mmdevice_->define_property ("long_property",
                                     anyval,
                                     env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  
  // Prepare a Float and "define" that in the PropertySet.
  CORBA::Float f = 3.14;
  anyval <<= f;
  f = 4.14;
  anyval >>= f;
  ACE_DEBUG ((LM_DEBUG,
              "Main : Float f = %f\n",
              f));
  remote_mmdevice_->define_property ("float_property", 
                                     anyval, 
                                     env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // Prepare a String and "define" that in the PropertySet.
  ACE_DEBUG ((LM_DEBUG,
              "Main: Any holding String\n"));
  CORBA::String_var strvar (CORBA::string_dup ("Test_String"));
  anyval <<= strvar.in ();
  CORBA::String newstr;
  anyval >>= newstr;

  ACE_DEBUG ((LM_DEBUG,
              "Main: String :  %s, From any :  %s\n",
              strvar.in (),
              newstr));
  remote_mmdevice_->define_property ("string_property",
                                     anyval,
                                     env);
  TAO_CHECK_ENV_RETURN (env, 1);
}

// Testing get_all_property_names of the PropertySet.

int
Client::test_get_all_property_names (CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nTesting get_all_property_names ()\n"));

  // Get the size.
  CORBA::ULong num_of_properties =
    remote_mmdevice_->get_number_of_properties (env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // Get half on the names and half of on the iterator.
  CORBA::ULong how_many = num_of_properties / 2;
  CosPropertyService::PropertyNames_var names_var;
  CosPropertyService::PropertyNamesIterator_var iterator_var;
  remote_mmdevice_->get_all_property_names (how_many,
                                           names_var.out (),
                                           iterator_var.out (), 
                                           env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // Print out the names in the names-sequence.
  if (names_var.ptr () != 0)
    {
      CORBA::ULong len = names_var->length ();

      for (CORBA::ULong ni = 0; ni < len; ni++)
        ACE_DEBUG ((LM_DEBUG,
                    "%s\n",
                    (const char *) names_var [ni]));
    }

  // Iterate thru and print out the names in the iterator, if any.
  if (iterator_var.ptr () != 0)
    {
      CosPropertyService::PropertyName_var name_var;

      while (iterator_var->next_one (name_var.out (), env) == CORBA::B_TRUE)
        {
          TAO_CHECK_ENV_RETURN (env, 1);
          ACE_DEBUG ((LM_DEBUG, "%s\n", name_var.in ()));
        }

      TAO_CHECK_ENV_RETURN (env, 1);
    }
}
  
// Test get_properties. Give a sequence of names and get all their
// properties.

int   
Client::test_get_properties (CORBA::Environment &env)
{
  // Get float_property, string_property and no_property. If return
  // value is false and type is tc_void then that name is not there in
  // the PropertySet.
  ACE_DEBUG ((LM_DEBUG, "\nTesting get_properties\n"));
  CosPropertyService::PropertyNames_var names;
  CosPropertyService::PropertyNames_ptr names_ptr;
    
  ACE_NEW_RETURN (names_ptr,
                  CosPropertyService::PropertyNames,
                  -1);
  names = names_ptr;
  
  names->length (3);
  names [0] = CORBA::string_dup ("float_property");
  //names [1] = CORBA::string_dup ("string_property");
  names [1] = CORBA::string_dup ("long_property");
  //names [2] = CORBA::string_dup ("char_property");
  names [2] = CORBA::string_dup ("no_property");
  
  CosPropertyService::Properties_var properties;
  
  // Get the properties.
  CORBA::Boolean return_val = remote_mmdevice_->get_properties (names.in (),
                                                                properties.out (),
                                                                env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  if (properties.ptr () != 0)
    {
      // Go thru the properties and print them out, if they are not
      // _tc_void typed values.
      CORBA::ULong len = properties->length ();

      for (CORBA::ULong pi = 0; pi < len; pi++)
        {
          // Print the name.
          ACE_DEBUG ((LM_DEBUG,
                      "%s : ",
                      (const char *) properties [pi].property_name.in ())); 

          // Print the value if type is not tk_void.
          if (properties [pi].property_value.type () == CORBA::_tc_void) 
            ACE_DEBUG ((LM_DEBUG,"Void\n"));

          if (properties [pi].property_value.type () == CORBA::_tc_float)
            {
              CORBA::Float f;
              properties [pi].property_value >>= f;
              ACE_DEBUG ((LM_DEBUG,"%f\n", f));
            }

          if (properties [pi].property_value.type () == CORBA::_tc_string)
            {
              CORBA::String str;
              properties [pi].property_value >>= str;
              ACE_DEBUG ((LM_DEBUG,"%s\n", str));
            }

          if (properties [pi].property_value.type () == CORBA::_tc_long)
            {
              CORBA::Long l;
              properties [pi].property_value >>= l;
              ACE_DEBUG ((LM_DEBUG,"%d\n", l));
            }
          if (properties [pi].property_value.type () == CORBA::_tc_char)
            {
              CORBA::Char ch;
              properties [pi].property_value >>= to_char (ch);
              ACE_DEBUG ((LM_DEBUG,"%c\n", ch));
            }
        }
    }
  return 0;
}

// Testing, get_number_of_properties.

int
Client::test_get_number_of_properties (CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, 
              "\nNumber of props : %d\n",
              this->remote_mmdevice_->get_number_of_properties (env)));
  TAO_CHECK_ENV_RETURN (env, 1);
  
  return 0;
}

// Test delete_property.

int 
Client::test_delete_property (CORBA::String property_name,
                              CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, "\nDeleting %s\n",property_name));

  this->remote_mmdevice_->delete_property (property_name, env);
  TAO_CHECK_ENV_RETURN (env, 1);

  return 0;
}  

// Test delete_properties.
// Make a sequence of property names and delete them from the
// PropertySet.  Deleting char, short, long, float and string
// properties.

int
Client::test_delete_properties (CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nChecking delete_properties\n"));
  CosPropertyService::PropertyNames prop_names;
  prop_names.length (4);
  prop_names [0] = CORBA::string_dup ("char_property");
  prop_names [1] = CORBA::string_dup ("short_property");
  prop_names [2] = CORBA::string_dup ("long_property");
  prop_names [3] = CORBA::string_dup ("float_property");
  ACE_DEBUG ((LM_DEBUG,
              "Length of sequence %d, Maxlength : %d\n",
              prop_names.length (),
              prop_names.maximum ()));
  this->remote_mmdevice_->delete_properties (prop_names,
                                   env);
  TAO_CHECK_ENV_RETURN (env, 0);
  
  return 0;
}

// Defines a sequnce of properties containing, char, short, long,
// float in the property set.

int
Client::test_define_properties (CORBA::Environment &env)
{  
  ACE_DEBUG ((LM_DEBUG,
              "\nChecking define_properties\n"));
  CosPropertyService::Properties nproperties;
  nproperties.length (4);
  CORBA::Any anyval;
  // Prepare a char and "define" that in the PropertySet.
  CORBA::Char ch = '#';
  anyval <<= from_char (ch);
  ch = '*';
  anyval >>= to_char (ch);  
  nproperties[0].property_name  = CORBA::string_copy ("char_property");
  nproperties[0].property_value <<= from_char (ch);
  
  // Prepare a Short and "define" that in the PropertySet.
  CORBA::Short s = 3;
  anyval <<= s; 
  s = 7; 
  anyval >>= s;
  nproperties[1].property_name  = CORBA::string_copy ("short_property");
  nproperties[1].property_value <<= s;
  
  // Prepare a Long and "define" that in the PropertySet.
  CORBA::Long l = 931232;
  anyval <<= l;
  l = 931233;
  anyval >>= l;
  nproperties[2].property_name  = CORBA::string_copy ("long_property");
  nproperties[2].property_value <<= l;
  
  // Prepare a Float and "define" that in the PropertySet.
  CORBA::Float f = 3.14;
  anyval <<= f;
  f = 4.14;
  anyval >>= f;
  nproperties[3].property_name  = CORBA::string_copy ("float_property");
  nproperties[3].property_value <<= f;
  
  // Define this sequence of properties now.
  this->remote_mmdevice_->define_properties (nproperties, env);
  TAO_CHECK_ENV_RETURN (env, 1);

  return 0;
}

// Test get_all_properties.

int   
Client::test_get_all_properties (CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nTesting get_all_properties\n"));
  // Get the number of current properties.
  CORBA::ULong num_of_properties = 
    this->remote_mmdevice_->get_number_of_properties (env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // Get half on the properties and half of on the iterator.
  CORBA::ULong how_many = 0;
  CosPropertyService::Properties_var properties;
  CosPropertyService::PropertiesIterator_var iterator;
  remote_mmdevice_->get_all_properties (how_many,
                                        properties.out (),
                                        iterator.out (), 
                                        env);
  TAO_CHECK_ENV_RETURN (env, 1);
  
  // Print out the properties now.
  if (properties.ptr () != 0)
    {
      CORBA::ULong len = properties->length ();

      for (CORBA::ULong pi = 0; pi < len; pi++)
        {
          // Print the property_name.
          ACE_DEBUG ((LM_DEBUG,
                      "%s : ",
                      properties [pi].property_name.in ()));
          
          // Print the value if type is not tk_void.
          if (properties [pi].property_value.type () == CORBA::_tc_void) 
            ACE_DEBUG ((LM_DEBUG,"Void\n"));

          if (properties [pi].property_value.type () == CORBA::_tc_float)
            {
              CORBA::Float f;
              properties [pi].property_value >>= f;
              ACE_DEBUG ((LM_DEBUG,"%f\n", f));
            }

          if (properties [pi].property_value.type () == CORBA::_tc_string)
            {
              CORBA::String str;
              properties [pi].property_value >>= str;
              ACE_DEBUG ((LM_DEBUG,"%s\n", str));
            }

          if (properties [pi].property_value.type () == CORBA::_tc_long)
            {
              CORBA::Long l;
              properties [pi].property_value >>= l;
              ACE_DEBUG ((LM_DEBUG,"%d\n", l));
            }
        }
    }

  // Pass thru the iterator.
  if (iterator.ptr () != 0)
    {
      CosPropertyService::Property_var property;
      
      while (iterator->next_one (property.out (), env) != CORBA::B_FALSE)
        {
          ACE_DEBUG ((LM_DEBUG, "Iteration over PropertyIterartor"));
          TAO_CHECK_ENV_RETURN (env, 1);
          ACE_DEBUG ((LM_DEBUG,
                      "%s : ",
                      property->property_name.in ()));

          // Print the property_value.
          if (property->property_value.type () == CORBA::_tc_char)
            {
              CORBA::Char c;
              property->property_value >>= to_char (c);
              ACE_DEBUG ((LM_DEBUG,"%c\n", c));
            }

          if (property->property_value.type () == CORBA::_tc_short)
            {
              CORBA::Short s;
              property->property_value >>= s;
              ACE_DEBUG ((LM_DEBUG,"%d\n", s));
            }

          if (property->property_value.type () == CORBA::_tc_float)
            {
              CORBA::Float f;
              property->property_value >>= f;
              ACE_DEBUG ((LM_DEBUG,"%f\n", f));
            }

          if (property->property_value.type () == CORBA::_tc_string)
            {
              CORBA::String str;
              property->property_value >>= str;
              ACE_DEBUG ((LM_DEBUG,"%s\n", str));
            }

          if (property->property_value.type () == CORBA::_tc_long)
            {
              CORBA::Long l;
              property->property_value >>= l;
              ACE_DEBUG ((LM_DEBUG,"%d\n", l));
            }
        }
      TAO_CHECK_ENV_RETURN (env, 1);
    }  
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
      client.property_tester (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("AVStreams: client");
      return -1;
    }
  TAO_ENDTRY;
}
