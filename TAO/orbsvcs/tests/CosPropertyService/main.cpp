// $Id$

// =========================================================================================================
//
// = FILE  
//     Program to test the property services.
//
// = DESCRIPTION
//     - Instantiates TAO_PropertySet
//     - Uses the fns implemented
//     - Shows the results
//
// = AUTHOR
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ========================================================================================================= 

#include "orbsvcs/Property/CosPropertyService_i.h"
#include "tao/any.h"      
#include "tao/corbacom.h" 

int 
main (int argc, char *argv []) 
{
  ACE_DEBUG ((LM_DEBUG,
              "TAO_PropertySet Testing\n"));
  
  // @@ Alex, rather than having one giant main() function, can you
  // please break this up so that it is a serious of method calls on
  // some class called "Property_Tester" or something (you can pick a
  // better name, I'm sure!).  This will make the test program much
  // easier to understand.
  TAO_TRY 
    {
      TAO_PropertySet prop_set;
      {
        ACE_DEBUG ((LM_DEBUG, "\nChecking define_property\n"));
        CORBA::Any anyval;
        // Prepare a Short and "define" that in the PropertySet.
        CORBA::Short s = 3;
        anyval <<= s; 
        s = 7; 
        anyval >>= s;
        ACE_DEBUG ((LM_DEBUG,
                    "Main : Short s = %d\n",
                    s));
        prop_set.define_property ("short_property",
                                  anyval,
                                  TAO_TRY_ENV);
        // @@ Alex, should you be checking the result of TAO_TRY_ENV?
        // Please make sure this is done consistently here!
        
        // Prepare a Long and "define" that in the PropertySet.
        CORBA::Long l = 931232;
        anyval <<= l;
        l = 931233;
        anyval >>= l;
        ACE_DEBUG ((LM_DEBUG,
                    "Main : Long l = %d\n",
                    l));
        prop_set.define_property ("long_property",
                                  anyval,
                                  TAO_TRY_ENV);
        
        // Prepare a Float and "define" that in the PropertySet.
        CORBA::Float f = 3.14;
        anyval <<= f;
        f = 4.14;
        anyval >>= f;
        ACE_DEBUG ((LM_DEBUG,
                    "Main : Float f = %f\n",
                    f));
        prop_set.define_property ("float_property", anyval, TAO_TRY_ENV);
        
        // Let's test the number of properties now.
        ACE_DEBUG ((LM_DEBUG, 
                    "Main : Number of props : %d\n",
                    prop_set.get_number_of_properties (TAO_TRY_ENV)));
      }
      {
        ACE_DEBUG ((LM_DEBUG, "\nChecking delete_property\n"));
        // Delete a few properties.
        prop_set.delete_property ("long_property",
                                  TAO_TRY_ENV);
        // Let us test the number of properties now.
        ACE_DEBUG ((LM_DEBUG, 
                    "Main :  Number of props : %d\n",
                    prop_set.get_number_of_properties (TAO_TRY_ENV)));
      }
      {
        ACE_DEBUG ((LM_DEBUG,
                    "\nChecking define_property with Any holding string\n"));
        // Any holding a string. 
        ACE_DEBUG ((LM_DEBUG,
                    "Main: Any holding String\n"));
        CORBA::String_var strvar (CORBA::string_dup ("Test_String"));
        CORBA::Any anyval;
        anyval <<= strvar.in ();
        CORBA::String newstr;
        anyval >>= newstr;
        ACE_DEBUG ((LM_DEBUG,
                    "Main: String :  %s, From any :  %s\n",
                    strvar.in (),
                    newstr));
        prop_set.define_property ("string_property",
                                  anyval,
                                  TAO_TRY_ENV);
        // Let's test the number of properties now.
        ACE_DEBUG ((LM_DEBUG, 
                    "Main :  Number of props : %d\n",
                    prop_set.get_number_of_properties (TAO_TRY_ENV)));
      }
      { 
        // Checking get_property_value.
        ACE_DEBUG ((LM_DEBUG,
                    "\nChecking get_property_value\n"));
        CORBA::Any_ptr anyptr = prop_set.get_property_value ("short_property",
                                                             TAO_TRY_ENV);
        // Get the short value.
        if (anyptr != 0)
          {
            CORBA::Short s;
            *anyptr >>= s;
            ACE_DEBUG ((LM_DEBUG,
                        "Short %d\n",
                        s));
          }
        else
          ACE_DEBUG ((LM_DEBUG,
                      "Short property not found\n"));
        // Get the string.
        anyptr = prop_set.get_property_value ("string_property",
                                              TAO_TRY_ENV);
        if (anyptr != 0)
          {
            CORBA::String str;
            *anyptr >>= str;
            ACE_DEBUG ((LM_DEBUG,
                        "Str %s\n", str));
          }
        else 
          ACE_DEBUG ((LM_DEBUG,
                      "string_property not found\n"));
      }
      {
        // Checking "if_property_deifined" of no.
        ACE_DEBUG ((LM_DEBUG,
                    "\nChecking is_property_defined ()\n"));
        if (prop_set.is_property_defined ("short_property",
                                          TAO_TRY_ENV) == CORBA::B_FALSE)
          ACE_DEBUG ((LM_DEBUG,
                      "short_property not defined\n"));
        else 
          ACE_DEBUG ((LM_DEBUG,
                      "short_property defined\n"));
        if (prop_set.is_property_defined ("string_property",
                                          TAO_TRY_ENV) == CORBA::B_FALSE)
          ACE_DEBUG ((LM_DEBUG,
                      "string_property not defined\n"));
        else
          ACE_DEBUG ((LM_DEBUG,
                      "string_property defined\n"));
      }
      {
        // Deleting a sequece of Properties.
        ACE_DEBUG ((LM_DEBUG,
                    "\nChecking delete_properties == Deleting a sequence of Properties\n"));
        CosPropertyService::PropertyNames prop_names;
        prop_names.length (3);
        prop_names [0] = "long_property";
        prop_names [1] = "float_property";
        prop_names [2] = "short_property";
        ACE_DEBUG ((LM_DEBUG,
                    "Length of sequence %d, Maxlength : %d\n",
                    prop_names.length (),
                    prop_names.maximum ()));
        prop_set.delete_properties (prop_names,
                                    TAO_TRY_ENV);
        ACE_DEBUG ((LM_DEBUG, 
                    "Main : Number of props : %d\n",
                    prop_set.get_number_of_properties (TAO_TRY_ENV)));
        // Try to find whether a property is defined or no.
        if (prop_set.is_property_defined ("string_property",
                                          TAO_TRY_ENV) == CORBA::B_FALSE)
          ACE_DEBUG ((LM_DEBUG,
                      "string_property not defined\n"));
        else
          ACE_DEBUG ((LM_DEBUG,
                      "string_property defined\n"));
      }
      {
        // Deleting all the properties 
        ACE_DEBUG ((LM_DEBUG,
                    "\nChecking delete_all_properties\n"));
        if (prop_set.delete_all_properties (TAO_TRY_ENV) == CORBA::B_TRUE)
          ACE_DEBUG ((LM_DEBUG,
                      "All properties deleted, I guess\n"));
        else
          ACE_DEBUG ((LM_DEBUG,
                      "delete_all_properties failed\n"));
        // Check the number of properties
        ACE_DEBUG ((LM_DEBUG, 
                    "Main : Number of props : %d\n",
                    prop_set.get_number_of_properties (TAO_TRY_ENV)));
      }     
      {
        // All the properties are deleted. Lets define a sequence of
        // properties, now
        ACE_DEBUG ((LM_DEBUG,
                    "\nChecking define_properties == Defining sequence of properties\n"));
        CosPropertyService::Properties nproperties;
        nproperties.length (3);
        CORBA::Any anyval;
        // A float property.
        anyval <<= 3.14;
        nproperties[0].property_name =
          CORBA::string_copy ("float_property");
        nproperties[0].property_value =
          new CORBA::Any (anyval);
        // A char property.
        anyval <<= '#';
        nproperties[1].property_name =
          CORBA::string_copy ("char_property");
        nproperties[1].property_value =
          new CORBA::Any (anyval);
        // A string property.
        anyval <<= "Test string";
        nproperties[2].property_name =
          CORBA::string_copy ("string_property");
        nproperties[2].property_value =
          new CORBA::Any (anyval);

        prop_set.define_properties (nproperties,
                                    TAO_TRY_ENV);

        // Check the number of properties.
        ACE_DEBUG ((LM_DEBUG, 
                    "Main : Number of props : %d\n",
                    prop_set.get_number_of_properties (TAO_TRY_ENV)));
      }
      {
        // Checking Names Iterator.
        ACE_DEBUG ((LM_DEBUG,
                    "\nChecking NamesIterator\n"));
        TAO_PropertyNamesIterator iterator (prop_set,
                                            TAO_TRY_ENV);
        CORBA::String_var strvar;
        CORBA::Boolean ret;

        // @@ Alex, please add a comment here explaining what you are
        // testing.
        while ((ret = iterator.next_one (strvar.out (), TAO_TRY_ENV)) != CORBA::B_FALSE)
          ACE_DEBUG ((LM_DEBUG, "Str : %s\n", strvar.in ()));
        
        // Checking reset ().
        ACE_DEBUG ((LM_DEBUG,
                    "Resetting (reset ()) the Iterator, Iterating again :-)\n"));
        iterator.reset (TAO_TRY_ENV);

        // @@ Alex, please add a comment here explaining what you are
        // testing.
        while ((ret = iterator.next_one (strvar.out (), TAO_TRY_ENV)) != CORBA::B_FALSE)
          ACE_DEBUG ((LM_DEBUG, "Str : %s\n", strvar.in ()));
        
        // Checking next_n.
        ACE_DEBUG ((LM_DEBUG,
                    "Resetting (reset ()) the Iterator.:-)\n"));
        iterator.reset (TAO_TRY_ENV);

        CosPropertyService::PropertyNames_var pnames_var;

        ACE_DEBUG ((LM_DEBUG,
                    "Checking next_n (), next 2\n"));

        CORBA::ULong n = 2;
        if (iterator.next_n (n, pnames_var.out (), TAO_TRY_ENV) == CORBA::B_TRUE)
          ACE_DEBUG ((LM_DEBUG,
                      "Iterator still has items\n"));

        if (pnames_var.ptr () != 0)
          for (size_t i = 0; i < pnames_var.in ().length (); i++)
            ACE_DEBUG ((LM_DEBUG,
                        "str %s \n",
                        (const char *) pnames_var[i]));
        
        ACE_DEBUG ((LM_DEBUG,
                    "next_n (), next 3\n"));
        n = 3;

        if (iterator.next_n (3, pnames_var.out (), TAO_TRY_ENV) != CORBA::B_TRUE)
          ACE_DEBUG ((LM_DEBUG,
                      "Iterator has no more items\n"));

        if (pnames_var.ptr () != 0)
          for (size_t i = 0; i < pnames_var.in ().length (); i++)
            ACE_DEBUG ((LM_DEBUG,
                        "str %s \n",
                        (const char *) pnames_var[i]));
      }
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
      return 1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
      return 1;
    }
  TAO_ENDTRY;
  
  return 0;
}  







