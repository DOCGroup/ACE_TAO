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

class TAO_PropertySet_Tester
{
  // = TITLE
  //     The testing code for the PropertySet interface are grouped
  //     under the functions of this class.
  //
  // = DESCRIPTION
  //     Go thru the functions to understand the usage of the various
  //     methods of the PropertySet interaface.
public:
  TAO_PropertySet_Tester (void);
  // Constructor.
  
  ~TAO_PropertySet_Tester (void);
  // Destructor.
  
  int test_define_property (void);
  // Defines a char,a short, a long, a float and a string property.
  
  int test_get_number_of_properties (void);
  // Gets the number of properties currently defined in the PropertySet.
  
  int test_delete_property (const char *property_name);
  // Delete a given property.
  
  int test_is_property_defined (void);
  // Check the following properties are defined or
  // no. "char_property", "short_property" and a "string_property".
  
  int test_get_property_value (void);
  // Get the "float_property" and "string_property" and print them
  // out.
  
  int test_delete_properties (void);
  // Delete char, short,long, and string properties.
  
  int test_delete_all_properties (void);
  // Delete all the properties.
  
  int test_define_properties (void);
  // Define a sequence of properties. a char, a short, a long, a float
  // and a string.

private:
  TAO_PropertySet property_set_;
  // The PropertySet.
};

// Constructor.
TAO_PropertySet_Tester::TAO_PropertySet_Tester (void)
{
}

// Destructor.
TAO_PropertySet_Tester::~TAO_PropertySet_Tester (void)
{
}

// Defines a char, a short, a long, a float and a string.

TAO_PropertySet_Tester::test_define_property (void)
{
  TAO_TRY 
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nChecking define_property\n"));

      CORBA::Any anyval;

      // Prepare a char and "define" that in the PropertySet.
      CORBA::Char ch = '#';
      anyval <<= ch;
      ch = '*';
      anyval >>= ch;  
      ACE_DEBUG ((LM_DEBUG,
                  "Main : Char ch = %c\n",
                  ch));

      property_set_.define_property ("char_property",
                                     anyval,
                                     TAO_TRY_ENV);
      TAO_CHECK_ENV; 
      
      // Prepare a Short and "define" that in the PropertySet.
      CORBA::Short s = 3;
      anyval <<= s; 
      s = 7; 
      anyval >>= s;
      ACE_DEBUG ((LM_DEBUG,
                  "Main : Short s = %d\n",
                  s));
      property_set_.define_property ("short_property",
                                     anyval,
                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Prepare a Long and "define" that in the PropertySet.
      CORBA::Long l = 931232;
      anyval <<= l;
      l = 931233;
      anyval >>= l;
      ACE_DEBUG ((LM_DEBUG,
                  "Main : Long l = %d\n",
                  l));
      property_set_.define_property ("long_property",
                                     anyval,
                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Prepare a Float and "define" that in the PropertySet.
      CORBA::Float f = 3.14;
      anyval <<= f;
      f = 4.14;
      anyval >>= f;
      ACE_DEBUG ((LM_DEBUG,
                  "Main : Float f = %f\n",
                  f));
      property_set_.define_property ("float_property", 
                                     anyval, 
                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
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
      property_set_.define_property ("string_property",
                                     anyval,
                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;
}

// Testing, get_number_of_properties.

int
TAO_PropertySet_Tester::test_get_number_of_properties (void)
{
  TAO_TRY
    {
      ACE_DEBUG ((LM_DEBUG, 
                  "Main : Number of props : %d\n",
                  property_set_.get_number_of_properties (TAO_TRY_ENV)));
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
      return-1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
      return-1;
    }
  TAO_ENDTRY;
}

// Testing the delete_property. Delets property, with the given name,
// if that exsists.

int
TAO_PropertySet_Tester::test_delete_property (const char *property_name)
{
  ACE_DEBUG ((LM_DEBUG, "\nChecking delete_property\n"));
  TAO_TRY
    {
      property_set_.delete_property (property_name,
                                TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
      return-1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
      return-1;
    }
  TAO_ENDTRY;
}

// Gets the value of "short_property" and "string_property".

int
TAO_PropertySet_Tester::test_get_property_value (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nChecking get_property_value\n"));
  TAO_TRY
    {
      CORBA::Any_ptr anyptr = property_set_.get_property_value ("short_property",
                                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;
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
      anyptr = property_set_.get_property_value ("string_property",
                                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
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
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;
}

// Check the following properties are defined or no. "short_property",
// "string_property" and "char_property".

int
TAO_PropertySet_Tester::test_is_property_defined (void)
{
  // Checking "is_property_defined" of no.
  TAO_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nChecking is_property_defined ()\n"));
      if (property_set_.is_property_defined ("short_property",
                                             TAO_TRY_ENV) == CORBA::B_FALSE)
        ACE_DEBUG ((LM_DEBUG,
                    "short_property not defined\n"));
      else 
        ACE_DEBUG ((LM_DEBUG,
                    "short_property defined\n"));
      TAO_CHECK_ENV;
      if (property_set_.is_property_defined ("string_property",
                                             TAO_TRY_ENV) == CORBA::B_FALSE)
        ACE_DEBUG ((LM_DEBUG,
                    "string_property not defined\n"));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "string_property defined\n"));
      TAO_CHECK_ENV;
      if (property_set_.is_property_defined ("char_property",
                                             TAO_TRY_ENV) == CORBA::B_FALSE)
        ACE_DEBUG ((LM_DEBUG,
                    "char_property not defined\n"));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "char_property defined\n"));
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;
}

// Make a sequence of property names and delete them from the
// PropertySet.  Deleting char, short, long, float and string
// properties.

int
TAO_PropertySet_Tester::test_delete_properties (void)
{
  TAO_TRY 
    {
      // Deleting a sequece of Properties.
      ACE_DEBUG ((LM_DEBUG,
                  "\nChecking delete_properties == Deleting a sequence of Properties\n"));
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
      property_set_.delete_properties (prop_names,
                                       TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;
}

// Defines a sequnce of properties containing, char, short, long,
// float and string property in the property set.

int
TAO_PropertySet_Tester::test_define_properties (void)
{  
  ACE_DEBUG ((LM_DEBUG,
              "\nChecking define_properties == Defining sequence of properties\n"));
  TAO_TRY
    {
      CosPropertyService::Properties nproperties;
      nproperties.length (5);
      CORBA::Any anyval;
      // Prepare a char and "define" that in the PropertySet.
      CORBA::Char ch = '#';
      anyval <<= ch;
      ch = '*';
      anyval >>= ch;  
      nproperties[0].property_name  = CORBA::string_copy ("char_property");
      nproperties[0].property_value = new CORBA::Any (anyval);

      // Prepare a Short and "define" that in the PropertySet.
      CORBA::Short s = 3;
      anyval <<= s; 
      s = 7; 
      anyval >>= s;
      nproperties[1].property_name  = CORBA::string_copy ("short_property");
      nproperties[1].property_value = new CORBA::Any (anyval);
      
      // Prepare a Long and "define" that in the PropertySet.
      CORBA::Long l = 931232;
      anyval <<= l;
      l = 931233;
      anyval >>= l;
      nproperties[2].property_name  = CORBA::string_copy ("long_property");
      nproperties[2].property_value = new CORBA::Any (anyval);
      
      // Prepare a Float and "define" that in the PropertySet.
      CORBA::Float f = 3.14;
      anyval <<= f;
      f = 4.14;
      anyval >>= f;
      nproperties[3].property_name  = CORBA::string_copy ("float_property");
      nproperties[3].property_value = new CORBA::Any (anyval);
      
      // Prepare a String and "define" that in the PropertySet.
      CORBA::String_var strvar (CORBA::string_dup ("Test_String"));
      anyval <<= strvar.in ();
      nproperties[4].property_name  = CORBA::string_copy ("string_property");
      nproperties[4].property_value = new CORBA::Any (anyval);
      
      // Define this sequence of properties now.
      property_set_.define_properties (nproperties, TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;
}

// Delete all the properties.

int
TAO_PropertySet_Tester::test_delete_all_properties (void)
{
  // Deleting all the properties 
  ACE_DEBUG ((LM_DEBUG,
              "\nChecking delete_all_properties\n"));
  TAO_TRY
    {
      if (property_set_.delete_all_properties (TAO_TRY_ENV) == CORBA::B_TRUE)
        ACE_DEBUG ((LM_DEBUG,
                    "All properties deleted, I guess\n"));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "delete_all_properties failed\n"));
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;
}

int 
main (int argc, char *argv []) 
{
  // Checking PropertySet interface.
  ACE_DEBUG ((LM_DEBUG,
              "\nTAO_PropertySet Testing\n"));
  TAO_PropertySet_Tester propertyset_tester;
  
  // Checking define_property. define a char, a short,a long, a float
  // and a string.
  propertyset_tester.test_define_property ();
  
  // Test the number of properties and print it out.
  propertyset_tester.test_get_number_of_properties ();
  
  // Test delete property. Delete "string_property"
  propertyset_tester.test_delete_property ("string_property");
  
  // Test the number of properties and print it out.
  propertyset_tester.test_get_number_of_properties ();

  // Test delete_properties. Delete char, short, long and float.
  propertyset_tester.test_delete_properties ();
  
  // Test the number of properties and print it out.
  propertyset_tester.test_get_number_of_properties ();
  
  // Define a sequence of properties. char, short, long, float and
  // string.
  propertyset_tester.test_define_properties ();

  // Test the number of properties and print it out.
  propertyset_tester.test_get_number_of_properties ();

  // Checking get_property_value. get the value of short and string.
  propertyset_tester.test_get_property_value ();

  // Checking delete_all_properties.
  propertyset_tester.test_delete_all_properties ();
  
  // Test the number of properties and print it out.
  propertyset_tester.test_get_number_of_properties ();
  
  return 0;
}  








