\// $:Id$
//
// FILE  
//   Program to test the property services.
//
//
// AUTHOR
//   Alexander Babu Arulanthu
//
// DESCRIPTION
//   - Instantiate TAO_PropertySet
//   - Use the fns implemented
//   - Show the results
// 

#include "orbsvcs/CosPropertyService_i.h"
#include "tao/corbacom.h"  // to know the type codes
#include "tao/any.h"       // I am using Any type.


int 
main (int argc, char *argv []) 
{
  ACE_DEBUG ( (LM_DEBUG, "TAO_PropertySet Testing \n"));
  
  TAO_TRY {
    TAO_PropertySet propSet;
    CORBA::Any testval;

    // Number of properties in the PropertyaSet
    ACE_DEBUG ( (LM_DEBUG, "Number of props : %d \n", propSet.get_number_of_properties (TAO_TRY_ENV)) );

    // prepare a Short and  "define" that in the PropertySet 
    CORBA::Short s = 3;
    testval.replace ( CORBA::_tc_short, &s, CORBA::B_FALSE, TAO_TRY_ENV);
    ACE_DEBUG ( (LM_DEBUG, "Short s = %d \n", ( *(short *)testval.value ())) );
 
    propSet.define_property ("short_property", testval, TAO_TRY_ENV);
    
    // prepare a Long and  "define" that in the PropertySet 
    CORBA::Long l = 931232;
    testval.replace (CORBA::_tc_long, &l, CORBA::B_FALSE, TAO_TRY_ENV);
    ACE_DEBUG ( (LM_DEBUG, "Long l = %d \n", (*(long *)testval.value ())) );
    
    propSet.define_property ("long_property", testval, TAO_TRY_ENV);
     
    // prepare a Long and  "define" that in the PropertySet 
    float f = 3.14;
    testval.replace (CORBA::_tc_float, &f, CORBA::B_FALSE, TAO_TRY_ENV);
    f =   (*((float *)testval.value ()));
    ACE_DEBUG ( (LM_DEBUG, "Float f = %f \n", f));
    
    propSet.define_property ("float_property", testval, TAO_TRY_ENV);

    // Let us test the number of properties now
    ACE_DEBUG ( (LM_DEBUG, "Number of props : %d \n", propSet.get_number_of_properties (TAO_TRY_ENV)) );
    
    // delete a few properties
    propSet.delete_property ("short_property", TAO_TRY_ENV);
    ACE_DEBUG ( (LM_DEBUG, "\nshort_property deleted !!") );

    // Let us test the number of properties now
    ACE_DEBUG ( (LM_DEBUG, "\n Number of props : %d \n", propSet.get_number_of_properties (TAO_TRY_ENV)) );
    

    // Let us try to "get" back those values
    testval =  * propSet.get_property_value ("float_property", TAO_TRY_ENV);
    testval =  * propSet.get_property_value ("short_property", TAO_TRY_ENV);
  }
  TAO_CATCH (CORBA::SystemException, sysex) {
    TAO_TRY_ENV.print_exception ("System Exception");
    return 1;
  }
  TAO_CATCH (CORBA::UserException, userex) {
    TAO_TRY_ENV.print_exception ("User Exception");
    return 1;
  }
  TAO_ENDTRY;

  return 0;
}  

/*
  static TypeCode_ptr          _tc_short;
  static TypeCode_ptr          _tc_long;
  static TypeCode_ptr          _tc_ushort;
  static TypeCode_ptr          _tc_ulong;
  static TypeCode_ptr          _tc_float;
  static TypeCode_ptr          _tc_double;
  static TypeCode_ptr          _tc_boolean;
  static TypeCode_ptr          _tc_char;
  static TypeCode_ptr          _tc_octet;
  static TypeCode_ptr          _tc_any;
  static TypeCode_ptr          _tc_TypeCode;
  static TypeCode_ptr          _tc_Principal;
  static TypeCode_ptr          _tc_Object;
  static TypeCode_ptr          _tc_struct;
  static TypeCode_ptr          _tc_union;
  static TypeCode_ptr          _tc_enum;
  static TypeCode_ptr          _tc_string;
  static TypeCode_ptr          _tc_sequence;
  static TypeCode_ptr          _tc_array;
  static TypeCode_ptr          _tc_alias;
  static TypeCode_ptr          _tc_except;
  static TypeCode_ptr          _tc_longlong;
  static TypeCode_ptr          _tc_ulonglong;
  static TypeCode_ptr          _tc_longdouble;
  static TypeCode_ptr          _tc_wchar;
  static TypeCode_ptr          _tc_wstring;
  */

