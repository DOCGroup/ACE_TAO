// $Id$

// =========================================================================================================
//
// = FILE  
//     Program to test the property services.
//
// = DESCRIPTION
//     - Instantiate TAO_PropertySet
//     - Use the fns implemented
//     - Show the results
//
// = AUTHOR
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ========================================================================================================== 

#include "orbsvcs/CosPropertyService_i.h"
#include "tao/corbacom.h" 
#include "tao/any.h"      

// Prototypes
void print_any (CORBA::Any anyval);

int 
main (int argc, char *argv []) 
{
  ACE_DEBUG ((LM_DEBUG,
              "TAO_PropertySet Testing \n"));
  
  TAO_TRY 
    {
      TAO_PropertySet prop_set;
      CORBA::Any testval;
      
      // Number of properties in the PropertyaSet
      ACE_DEBUG ((LM_DEBUG, "Main : Number of props : %d \n",
                  prop_set.get_number_of_properties (TAO_TRY_ENV)));
      
      // prepare a Short and  "define" that in the PropertySet 
      CORBA::Short s = 3;
      testval.replace (CORBA::_tc_short,
                       &s,
                       CORBA::B_FALSE,
                       TAO_TRY_ENV);
      ACE_DEBUG ((LM_DEBUG,
                  "Main : Short s = %d \n", ( *(short *)testval.value ())));
      
      prop_set.define_property ("short_property", testval, TAO_TRY_ENV);
      
      // Prepare a Long and  "define" that in the PropertySet.
      CORBA::Long l = 931232;
      testval.replace (CORBA::_tc_long,
                       &l,
                       CORBA::B_FALSE,
                       TAO_TRY_ENV);

      ACE_DEBUG ((LM_DEBUG,
                  "Main : Long l = %d \n",
                  (*(long *) testval.value ())));
      
      prop_set.define_property ("long_property", testval, TAO_TRY_ENV);
      
      // Prepare a Float and  "define" that in the PropertySet.
      CORBA::Float f = 3.14;
      testval.replace (CORBA::_tc_float,
                       &f,
                       CORBA::B_FALSE,
                       TAO_TRY_ENV);
      f =   (*((float *) testval.value ()));

      ACE_DEBUG ((LM_DEBUG,
                  "Main : Float f = %f \n", f));
      
      prop_set.define_property ("float_property", testval, TAO_TRY_ENV);
      
      // Let us test the number of properties now.
      ACE_DEBUG ((LM_DEBUG, 
                  "Main : Number of props : %d \n",
                  prop_set.get_number_of_properties (TAO_TRY_ENV)));
      
      // Delete a few properties.
      prop_set.delete_property ("short_property",
                                TAO_TRY_ENV);
      
      // Let us test the number of properties now
      ACE_DEBUG ((LM_DEBUG, 
                  "\n Main :  Number of props : %d \n",
                  prop_set.get_number_of_properties (TAO_TRY_ENV)));
      
      // Let us try to "get" back those values.
      CORBA::Any_ptr anyptr;

      anyptr = prop_set.get_property_value ("float_property", TAO_TRY_ENV);

      if (anyptr->type () == CORBA::_tc_float) 
        ACE_DEBUG ((LM_DEBUG, "Main : Float = %f \n", (*(CORBA::Float *)anyptr->value ())));

      //CORBA::Any_var anyvar (anyptr);
      //print_any (anyvar.in ());
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
