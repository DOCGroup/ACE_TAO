// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/CosPropertyService
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//    Test client for the CosPropertyService.
//
// = AUTHORS
//    Alexander  Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#include "client.h"

ACE_RCSID(Property, client, "$Id$")

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
  TAO_CHECK_ENV_RETURN (env, -1);

  // Open the ORB.
  manager_.orb ()->open ();

  // Initialize the naming services
  if (my_name_client_.init (manager_.orb (), argc, argv) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to initialize "
                       "the TAO_Naming_Client. \n"),
                      -1);

  // Bind PropertySetDef Object.

  CosNaming::Name propsetdef_name (1);
  propsetdef_name.length (1);
  propsetdef_name [0].id = CORBA::string_dup ("PropertySetDef");
  CORBA::Object_var propsetdef_obj = my_name_client_->resolve (propsetdef_name,
							       env);
  TAO_CHECK_ENV_RETURN (env, 1);

  ACE_DEBUG ((LM_DEBUG, "Naming resolve done\n"));

  this->propsetdef_ = CosPropertyService::PropertySetDef::_narrow (propsetdef_obj.in (),
                                                                   env);
  TAO_CHECK_ENV_RETURN (env, -1);

  if (CORBA::is_nil (this->propsetdef_.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Couldnot resolve propsetdef in Naming server"),
                      -1);
}

// Testing the methods of the property service.

int
Client::property_tester (CORBA::Environment &env)
{
  // = Testing PropertySet & Iterators.

  // Testing define_property () of PropertySet interface.
  this->test_define_property (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Test the number of properties.
  this->test_get_number_of_properties (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Testing get_all_property_names.
  this->test_get_all_property_names (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Test get_property_value.
  this->test_get_property_value (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Testing delete property.
  this->test_delete_property ("no_property",env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Testing get_properties.
  this->test_get_properties (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Testing delete_properties.
  this->test_delete_properties (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Test the number of properties.
  this->test_get_number_of_properties (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Testing define_properties.
  this->test_define_properties (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Test the number of properties.
  this->test_get_number_of_properties (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Testing get_all_property_names.
  this->test_get_all_property_names (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Testing get_all_properties.
  this->test_get_all_properties (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Get the modes of all the properties defined.
  this->test_get_property_modes (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Testing delete_all_properties.
  this->test_delete_all_properties (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Test the number of properties.
  this->test_get_number_of_properties (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Try to get all the properties.
  this->test_get_all_properties (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // = Testing PropertySetDef & Iterators.

  // Testing define_property_with_mode.
  this->test_define_property_with_mode (env);
  TAO_CHECK_ENV_RETURN (env, -1);
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
  this->propsetdef_->define_property ("char_property",
				      anyval,
				      env);

  // Check if that is an user exception, if so, print it out.
  if ((env.exception () != 0) &&
      (CORBA::UserException::_narrow (env.exception ()) != 0))
    {
      env.print_exception ("char_property");
      env.clear ();
    }

  // Prepare a Short and "define" that in the PropertySet.
  CORBA::Short s = 3;
  anyval <<= s;
  s = 7;
  anyval >>= s;

  ACE_DEBUG ((LM_DEBUG,
              "Main : Short s = %d\n",
              s));
  propsetdef_->define_property ("short_property",
				anyval,
				env);

  // Check if that is an user exception, if so, print it out.
  if ((env.exception () != 0) &&
      (CORBA::UserException::_narrow (env.exception ()) != 0))
    {
      env.print_exception ("char_property");
      env.clear ();
    }

  // Prepare a Long and "define" that in the PropertySet.
  CORBA::Long l = 931232;
  anyval <<= l;
  l = 931233;
  anyval >>= l;
  ACE_DEBUG ((LM_DEBUG,
              "Main : Long l = %d\n",
              l));
  CORBA::Any newany(anyval);
  propsetdef_->define_property ("long_property",
				anyval,
				env);

  // Check if that is an user exception, if so, print it out.
  if ((env.exception () != 0) &&
      (CORBA::UserException::_narrow (env.exception ()) != 0))
    {
      env.print_exception ("char_property");
      env.clear ();
    }

  // Prepare a Float and "define" that in the PropertySet.
  CORBA::Float f = 3.14;
  anyval <<= f;
  f = 4.14;
  anyval >>= f;
  ACE_DEBUG ((LM_DEBUG,
              "Main : Float f = %f\n",
              f));
  propsetdef_->define_property ("float_property",
				anyval,
				env);

  // Check if that is an user exception, if so, print it out.
  if ((env.exception () != 0) &&
      (CORBA::UserException::_narrow (env.exception ()) != 0))
    {
      env.print_exception ("char_property");
      env.clear ();
    }

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
  propsetdef_->define_property ("string_property",
				anyval,
				env);

  // Check if that is an user exception, if so, print it out.
  if ((env.exception () != 0) &&
      (CORBA::UserException::_narrow (env.exception ()) != 0))
    {
      env.print_exception ("string_property");
      env.clear ();
    }

  return 0;
}

// Testing get_all_property_names of the PropertySet.

int
Client::test_get_all_property_names (CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nTesting get_all_property_names ()\n"));

  // Get the size.
  CORBA::ULong num_of_properties =
    propsetdef_->get_number_of_properties (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Get half on the names and half of on the iterator.
  CORBA::ULong how_many = num_of_properties / 2;
  CosPropertyService::PropertyNames_var names_var;
  CosPropertyService::PropertyNamesIterator_var iterator_var;
  propsetdef_->get_all_property_names (how_many,
				       names_var.out (),
				       iterator_var.out (),
				       env);
  TAO_CHECK_ENV_RETURN (env, -1);

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
          TAO_CHECK_ENV_RETURN (env, -1);
          ACE_DEBUG ((LM_DEBUG, "%s\n", name_var.in ()));
        }

      TAO_CHECK_ENV_RETURN (env, -1);
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
  names [2] = CORBA::string_dup ("char_property");
  //names [2] = CORBA::string_dup ("no_property");

  CosPropertyService::Properties_var properties;

  // Get the properties.
  CORBA::Boolean return_val = propsetdef_->get_properties (names.in (),
							   properties.out (),
							   env);
  TAO_CHECK_ENV_RETURN (env, -1);

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

          // Print the value.
          CORBA::Any::dump (properties [pi].property_value);
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
              this->propsetdef_->get_number_of_properties (env)));
  TAO_CHECK_ENV_RETURN (env, -1);

  return 0;
}

// Test delete_property.

int
Client::test_delete_property (CORBA::String property_name,
                              CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, "\nDeleting %s\n",property_name));

  TAO_TRY
    {
      this->propsetdef_->delete_property (property_name, TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::UserException, ex)
    {
      TAO_TRY_ENV.print_exception ("User exception");
      return -1;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Not an user exception");
      return -1;
    }
  TAO_ENDTRY;

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
  prop_names.length (3);
  prop_names [0] = CORBA::string_dup ("char_property");
  prop_names [1] = CORBA::string_dup ("short_property");
  prop_names [2] = CORBA::string_dup ("long_property");
  //prop_names [3] = CORBA::string_dup ("no_property");
  ACE_DEBUG ((LM_DEBUG,
              "Length of sequence %d, Maxlength : %d\n",
              prop_names.length (),
              prop_names.maximum ()));
  this->propsetdef_->delete_properties (prop_names,
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
  this->propsetdef_->define_properties (nproperties, env);
  TAO_CHECK_ENV_RETURN (env, -1);

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
    this->propsetdef_->get_number_of_properties (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Get half on the properties and half of on the iterator.
  CORBA::ULong how_many = 0;
  CosPropertyService::Properties_var properties;
  CosPropertyService::PropertiesIterator_var iterator;
  propsetdef_->get_all_properties (how_many,
				   properties.out (),
				   iterator.out (),
				   env);
  TAO_CHECK_ENV_RETURN (env, -1);

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

          // Print the value.
          CORBA::Any::dump (properties [pi].property_value);
        }
    }

  // Pass thru the iterator.
  if (iterator.ptr () != 0)
    {
      CosPropertyService::Property_var property;

      while (iterator->next_one (property.out (), env) != CORBA::B_FALSE)
        {
          TAO_CHECK_ENV_RETURN (env, -1);
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
      TAO_CHECK_ENV_RETURN (env, -1);
    }
}

// Testing define property with mode.
// Defines char, short, long and float properties with different modes.
int
Client::test_define_property_with_mode (CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nChecking define_property_with_mode\n"));

  CORBA::Any anyval;

  // Prepare a char and "define" that in the PropertySet.
  CORBA::Char ch = '#';
  anyval <<= from_char (ch);
  ch = '*';
  anyval >>= to_char (ch);

  ACE_DEBUG ((LM_DEBUG,
              "Main : Char ch = %c\n",
              ch));
  this->propsetdef_->define_property_with_mode ("char_property",
                                                anyval,
                                                CosPropertyService::normal,
                                                env);
  // Check if that is an user exception, if so, print it out.
  if ((env.exception () != 0) &&
      (CORBA::UserException::_narrow (env.exception ()) != 0))
    {
      env.print_exception ("char_property");      env.clear ();
    }

  // Prepare a Short and "define" that in the PropertySet.
  CORBA::Short s = 3;
  anyval <<= s;
  s = 7;
  anyval >>= s;

  ACE_DEBUG ((LM_DEBUG,
              "Main : Short s = %d\n",
              s));
  propsetdef_->define_property_with_mode ("short_property",
                                          anyval,
                                          CosPropertyService::read_only,
                                          env);
  // Check if that is an user exception, if so, print it out.
  if (env.exception () != 0)
    {
      env.print_exception ("char_property");
      if (CORBA::UserException::_narrow (env.exception ()) != 0)
        env.clear ();
      else
        return -1;
    }


  // Prepare a Long and "define" that in the PropertySet.
  CORBA::Long l = 931232;
  anyval <<= l;
  l = 931233;
  anyval >>= l;
  ACE_DEBUG ((LM_DEBUG,
              "Main : Long l = %d\n",
              l));
  CORBA::Any newany(anyval);
  propsetdef_->define_property_with_mode ("long_property",
					  anyval,
					  CosPropertyService::fixed_normal,
					  env);
  TAO_CHECK_ENV_RETURN (env, -1);


  // Prepare a Float and "define" that in the PropertySet.
  CORBA::Float f = 3.14;
  anyval <<= f;
  f = 4.14;
  anyval >>= f;
  ACE_DEBUG ((LM_DEBUG,
              "Main : Float f = %f\n",
              f));
  propsetdef_->define_property_with_mode ("float_property",
					  anyval,
					  CosPropertyService::fixed_readonly,
					  env);
  TAO_CHECK_ENV_RETURN (env, -1);

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
  propsetdef_->define_property  ("string_property",
                                 anyval,
                                 env);
  TAO_CHECK_ENV_RETURN (env, -1);
}

int
Client::test_get_property_value (CORBA::Environment &env)
{
  TAO_TRY
    {
      // Get the ior property.
      CORBA::Any_ptr any_ptr = this->propsetdef_->get_property_value ("PropertySetDef_IOR",
                                                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "Property value received successfully\n"));

      // Check whether the IOR is fine.
      CORBA::Object_var propsetdef_object;
      (*any_ptr) >>= to_object (propsetdef_object);

      CosPropertyService::PropertySetDef_var propsetdef =
        CosPropertyService::PropertySetDef::_narrow (propsetdef_object.in (),
                                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (propsetdef.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid object reference\n"),
                          -1);

      ACE_DEBUG ((LM_DEBUG, "IOR retrieved. Seems to be in good shape\n"));
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("get_property_value");
      return -1;
    }
  TAO_ENDTRY;
}

int
Client::test_delete_all_properties (CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, "\nTesting delete_all_properties\n"));

  TAO_TRY
    {
      // Delete all properties.
      this->propsetdef_->delete_all_properties (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, 0);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("test_delete_all_properties");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

int
Client::test_get_property_modes (CORBA::Environment &env)
{
  // Get all the names and then get all their modes, print'em.
  ACE_DEBUG ((LM_DEBUG,
              "\nTesting get_property_modes ()\n"));

  // Get the size.
  CORBA::ULong num_of_properties =
    propsetdef_->get_number_of_properties (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Get all the names in the sequence.
  CORBA::ULong how_many = num_of_properties;
  CosPropertyService::PropertyNames_var names_var;
  CosPropertyService::PropertyNamesIterator_var iterator_var;
  propsetdef_->get_all_property_names (how_many,
                                       names_var.out (),
                                       iterator_var.out (),
                                       env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Get modes for all these names.
  CosPropertyService::PropertyModes_var modes;
  propsetdef_->get_property_modes (names_var.in (),
                                   modes.out (),
                                   env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Print the names and the modes.
  if (modes.ptr () != 0)
    {
      CORBA::ULong len = modes->length ();

      for (CORBA::ULong mi = 0; mi < len; mi++)
        // Print the property_name and the mode.
        ACE_DEBUG ((LM_DEBUG,
                    "%s, Mode %d\n",
                    modes [mi].property_name.in (),
                    modes [mi].property_mode));
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

      //  client.run (TAO_TRY_ENV);
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
