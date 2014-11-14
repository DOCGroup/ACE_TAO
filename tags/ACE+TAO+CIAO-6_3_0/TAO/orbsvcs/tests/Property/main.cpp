// $Id$

// ========================================================================
//
// = FILE
//     Program to test the property services.
//
// = DESCRIPTION
//     Here all the objects are at one address space. See the
//     client.cpp and server.cpp to see property calls on remote
//     objects.
//
// = AUTHOR
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ========================================================================

#include "orbsvcs/CosPropertyS.h"
#include "orbsvcs/Property/CosPropertyService_i.h"



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
  friend class TAO_PropertyNamesIterator_Tester;
  // He has to instantiate TAO_NamesIterator class using the instance
  // of TAO_PropertySet.

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

class TAO_PropertyNamesIterator_Tester
{
  // = TITLE
  //     The testing code for the PropertyNamesIterator interface are grouped
  //     under the functions of this class.
  //
  // = DESCRIPTION
  //     Go thru the functions to understand the usage of the various
  //     methods of the PropertyNamesIterator interaface.
public:
  TAO_PropertyNamesIterator_Tester (TAO_PropertySet_Tester &propertyset_tester);
  // Constructor. To make names iterator from the property set object.

  ~TAO_PropertyNamesIterator_Tester (void);
  // Destructor.

  int test_next_one (void);
  // Test next one method. Iterate thru all the names in the
  // PropertySet and print them out.

  int test_reset (void);
  // Test the reset method.

  int test_next_n (size_t n);
  // Test the next_n method.
private:
  TAO_PropertyNamesIterator iterator_;
  // Our names iterator.
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
int
TAO_PropertySet_Tester::test_define_property (void)
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
  property_set_.define_property ("char_property",
                                 anyval);

  // Prepare a Short and "define" that in the PropertySet.
  CORBA::Short s = 3;
  anyval <<= s;
  s = 7;
  anyval >>= s;
  ACE_DEBUG ((LM_DEBUG,
              "Main : Short s = %d\n",
              s));
  property_set_.define_property ("short_property",
                                 anyval);


  // Prepare a Long and "define" that in the PropertySet.
  CORBA::Long l = 931232;
  anyval <<= l;
  l = 931233;
  anyval >>= l;
  ACE_DEBUG ((LM_DEBUG,
              "Main : Long l = %d\n",
              l));
  CORBA::Any newany(anyval);

  property_set_.define_property ("long_property",
                                 anyval);

  // Prepare a Float and "define" that in the PropertySet.
  CORBA::Float f = 3.14;
  anyval <<= f;
  f = 4.14;
  anyval >>= f;
  ACE_DEBUG ((LM_DEBUG,
              "Main : Float f = %f\n",
              f));
  property_set_.define_property ("float_property",
                                 anyval);

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
                                 anyval);

  return 0;
}

// Testing, get_number_of_properties.
int
TAO_PropertySet_Tester::test_get_number_of_properties (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Main : Number of props : %d\n",
              property_set_.get_number_of_properties ()));

  return 0;
}

// Testing the delete_property. Delets property, with the given name,
// if that exsists.
int
TAO_PropertySet_Tester::test_delete_property (const char *property_name)
{
  ACE_DEBUG ((LM_DEBUG, "\nChecking delete_property\n"));
  property_set_.delete_property (property_name);

  return 0;
}

// Gets the value of "short_property" and "string_property".

int
TAO_PropertySet_Tester::test_get_property_value (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nChecking get_property_value\n"));

  CORBA::Any_ptr anyptr = property_set_.get_property_value ("short_property");

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
  anyptr = property_set_.get_property_value ("string_property");
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
  return 0;
}

// Check the following properties are defined or no. "short_property",
// "string_property" and "char_property".

int
TAO_PropertySet_Tester::test_is_property_defined (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nChecking is_property_defined ()\n"));
  if (property_set_.is_property_defined ("short_property") == 0)
    ACE_DEBUG ((LM_DEBUG,
                "short_property not defined\n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "short_property defined\n"));
  if (property_set_.is_property_defined ("string_property") == 0)
    ACE_DEBUG ((LM_DEBUG,
                "string_property not defined\n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "string_property defined\n"));
  if (property_set_.is_property_defined ("char_property") == 0)
    ACE_DEBUG ((LM_DEBUG,
                "char_property not defined\n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "char_property defined\n"));

  return 0;
}

// Make a sequence of property names and delete them from the
// PropertySet.  Deleting char, short, long, float and string
// properties.
int
TAO_PropertySet_Tester::test_delete_properties (void)
{
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
  property_set_.delete_properties (prop_names);

  return 0;
}

// Defines a sequnce of properties containing, char, short, long,
// float and string property in the property set.
int
TAO_PropertySet_Tester::test_define_properties (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nChecking define_properties == Defining sequence of properties\n"));
  //  TAO_TRY
  //  {
  CosPropertyService::Properties nproperties;
  nproperties.length (5);
  CORBA::Any anyval;
  // Prepare a char and "define" that in the PropertySet.
  CORBA::Char ch = '#';
  anyval <<= from_char (ch);
  ch = '*';
  anyval >>= to_char (ch);
  nproperties[0].property_name  = CORBA::string_dup ("char_property");
  nproperties[0].property_value <<= from_char (ch);

  // Prepare a Short and "define" that in the PropertySet.
  CORBA::Short s = 3;
  anyval <<= s;
  s = 7;
  anyval >>= s;
  nproperties[1].property_name  = CORBA::string_dup ("short_property");
  nproperties[1].property_value <<= s;

  // Prepare a Long and "define" that in the PropertySet.
  CORBA::Long l = 931232;
  anyval <<= l;
  l = 931233;
  anyval >>= l;
  nproperties[2].property_name  = CORBA::string_dup ("long_property");
  nproperties[2].property_value <<= l;

  // Prepare a Float and "define" that in the PropertySet.
  CORBA::Float f = 3.14;
  anyval <<= f;
  f = 4.14;
  anyval >>= f;
  nproperties[3].property_name  = CORBA::string_dup ("float_property");
  nproperties[3].property_value <<= f;

  // Prepare a String and "define" that in the PropertySet.
  CORBA::String_var strvar (CORBA::string_dup ("Test_String"));
  anyval <<= strvar.in ();
  nproperties[4].property_name  = CORBA::string_dup ("string_property");
  nproperties[4].property_value <<= strvar.in ();

  // Define this sequence of properties now.
  property_set_.define_properties (nproperties);

  return 0;
}

// Delete all the properties.
int
TAO_PropertySet_Tester::test_delete_all_properties (void)
{
  // Deleting all the properties
  ACE_DEBUG ((LM_DEBUG,
              "\nChecking delete_all_properties\n"));
  int ret = property_set_.delete_all_properties ();
  if (ret == 1)
    ACE_DEBUG ((LM_DEBUG,
                "All properties deleted, I guess\n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "delete_all_properties failed\n"));
  return 0;
}

// Constructor. To make names iterator from the property set object.
TAO_PropertyNamesIterator_Tester::TAO_PropertyNamesIterator_Tester (TAO_PropertySet_Tester &propertyset_tester)
  : iterator_ (propertyset_tester.property_set_)
{
}

// Destructor.
TAO_PropertyNamesIterator_Tester::~TAO_PropertyNamesIterator_Tester (void)
{
}

// Test next one method. Iterate thru all the names in the
// PropertySet and print them out.
int
TAO_PropertyNamesIterator_Tester::test_next_one (void)
{
  CORBA::String_var strvar;
  ACE_DEBUG ((LM_DEBUG,
              "\nTesting next_one of NamesIterator, Iterating thru names.\n"));
  // Let us iterate, now.
  int ret = iterator_.next_one (strvar.out ());
  while (ret != 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Str : %s\n", strvar.in ()));
      ret = iterator_.next_one (strvar.out ());
    }
  return 0;
}

// Reset the names iterator.
int
TAO_PropertyNamesIterator_Tester::test_reset (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Resetting (reset ()) the NamesIterator."));
  iterator_.reset ();
  return 0;
}

// Test the next_n method. Get the next n names and print them all.
int
TAO_PropertyNamesIterator_Tester::test_next_n (size_t n)
{
  CosPropertyService::PropertyNames_var pnames_var;
  ACE_DEBUG ((LM_DEBUG,
              "Checking next_n (), next %d\n",
              n));
  int ret = iterator_.next_n (n, pnames_var.out ());
  if (ret == 0)
    {
      // Return if no more items in the iterator.
      ACE_DEBUG ((LM_DEBUG,
                  "Iterator has no more items\n"));
      return 0;
    }
  for (size_t i = 0; i < pnames_var.in ().length (); i++)
    ACE_DEBUG ((LM_DEBUG,
                "str %s\n",
                (const char *) pnames_var[i]));
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb_var = CORBA::ORB_init (argc,
                                                argv,
                                                "internet");

      // = Checking PropertySet interface.

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

      // Define a sequence of properties. char, short, long, float and
      // string.
      propertyset_tester.test_define_properties ();

      // Test the number of properties and print it out.
      propertyset_tester.test_get_number_of_properties ();

      // = Checking the PropertyNamesIterator interface.

      ACE_DEBUG ((LM_DEBUG,
                  "\nTAO_PropertyNamesIterator Testing\n"));

      // Construct the names iterator from the PropertySet object.
      TAO_PropertyNamesIterator_Tester names_iterator_tester (propertyset_tester);

      // Checking next_one. Iterate thru the property names.
      names_iterator_tester.test_next_one ();

      // Checking the reset () method.
      names_iterator_tester.test_reset ();

      // Iterating again thru the property names.
      names_iterator_tester.test_next_one ();

      // Reset again.
      names_iterator_tester.test_reset ();

      // Checking next_n. Prints out all the names it has got.
      names_iterator_tester.test_next_n (6);

      // Try next_n without resetting.
      names_iterator_tester.test_next_n (6);
    }
  catch (const CORBA::SystemException& sysex)
    {
.print_exception ("System Exception");
      return -1;
    }
  catch (const CORBA::UserException& userex)
    {
.print_exception ("User Exception");
      return -1;
    }
  return 0;
}
