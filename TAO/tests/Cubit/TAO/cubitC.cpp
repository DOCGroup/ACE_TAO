// @ (#)cubitC.cpp	05/14/97
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// Modified version of Cubit Example written by Sun Microsystems Inc.
// Modified by: Brian Mendel

#include "cubitC.h"		// for stubs ...

Cubit_ptr Cubit::_duplicate (Cubit_ptr obj)
{
  if (obj)
    obj->AddRef ();

  return obj;
}

Cubit_ptr Cubit::_narrow (CORBA::Object_ptr obj)
{
  CORBA::Environment env;

  if (obj->_is_a ("IDL:Cubit:1.0", env))
    {
      obj->Release (); // Release the reference grabbed by _is_a ()
      
      STUB_Object *istub;

      if (obj->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR)
	return Cubit::_nil ();

      obj->Release ();
      Cubit_ptr new_obj = new Cubit (istub);
      return Cubit::_duplicate (new_obj);
    }
  else
    ;// Do we need to do an obj->Release () here if _is_a () failed?

  return Cubit::_nil ();
}

Cubit_ptr Cubit::_nil (void)
{
  return (Cubit_ptr)NULL;
}

// CUBE OCTET

static const TAO_Param_Data Cubit_cube_octet_params [] = 
{
  { CORBA::_tc_octet, PARAM_RETURN, 0 },
  { CORBA::_tc_octet, PARAM_IN, 0 }
};
    
static const TAO_Call_Data Cubit_cube_octet_calldata = 
{
  "cube_octet", CORBA::B_TRUE,
  2, &Cubit_cube_octet_params [0],
  0, 0
};

CORBA::Octet
Cubit::Cubit_cube_octet (CORBA::Octet o,
			 CORBA::Environment &env)
{
  CORBA::Octet retval;

  STUB_Object *istub;

  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR)
    {
      env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));
      return 0;
    }

  this->Release ();
   
  istub->do_call (env, &Cubit_cube_octet_calldata,
		  &retval, &o);
  istub->Release ();

  return retval;
}

// CUBE SHORT

static const TAO_Param_Data Cubit_cube_short_params [] = 
{
  { CORBA::_tc_short, PARAM_RETURN, 0 },
  { CORBA::_tc_short, PARAM_IN, 0 }
};

static const TAO_Call_Data Cubit_cube_short_calldata = 
{
  "cube_short", CORBA::B_TRUE,
  2, &Cubit_cube_short_params [0],
  0, 0
};


CORBA::Short
Cubit::Cubit_cube_short (CORBA::Short s,
			 CORBA::Environment &env)
{
  CORBA::Short retval;

  STUB_Object *istub;

  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR)
    {
      env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));
      return 0;
    }

  this->Release ();

  istub->do_call (env, &Cubit_cube_short_calldata,
                  &retval, &s);
   
  istub->Release ();
   
  return retval;
}

// CUBE LONG

static const TAO_Param_Data Cubit_cube_long_params [] = 
{
  { CORBA::_tc_long, PARAM_RETURN, 0 },
  { CORBA::_tc_long, PARAM_IN, 0 }
};

static const TAO_Call_Data Cubit_cube_long_calldata = 
{
  "cube_long", CORBA::B_TRUE,
  2, &Cubit_cube_long_params [0],
  0, 0
};

CORBA::Long
Cubit::Cubit_cube_long (CORBA::Long l,
			CORBA::Environment &env)
{
  CORBA::Long retval;
   
  STUB_Object *istub;

  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR)
    {
      env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));
      return 0;
    }

  this->Release ();
   
  istub->do_call (env, &Cubit_cube_long_calldata,
                  &retval, &l);
  istub->Release ();
   
  return retval;
}

// Encapsulated parameters for struct "Cubit_Many" typecode.  None of
// these parameters is complicated, so this is just a linear sequence
// of element encodings
//
// NOTE:  it's important that this be longword aligned!!

static const CORBA::Long _oc_Cubit_Many [] = 
{
  1,          // byte order flag (TRICKY!)
      
  1, 0,       // empty string:  repository/type ID
  1, 0,       // empty string:  struct name
      
  3,          // three struct elements
      
  // First structure element:  name, typecode for Octet
  1, 0,       // empty string:  name "o"
  CORBA::tk_octet,
      
  // Second structure element:  name, typecode for Long
  1, 0,       // empty string:  name "l"
  CORBA::tk_long,
      
  // Third structure element:  name, typecode for Short
  1, 0,       // empty string:  name "s"
  CORBA::tk_short,
};

static CORBA::TypeCode _tc_Cubit_Many (CORBA::tk_struct,
                                      sizeof _oc_Cubit_Many,
				      (u_char *) &_oc_Cubit_Many,
                                      CORBA::B_FALSE);
CORBA::TypeCode_ptr TC_Cubit_Many = &_tc_Cubit_Many;

// CUBE STRUCT

static const TAO_Param_Data Cubit_cube_struct_params [] = 
{
  { &_tc_Cubit_Many, PARAM_RETURN, sizeof (Cubit_Many) },
  { &_tc_Cubit_Many, PARAM_IN, 0 }
};

static const TAO_Call_Data Cubit_cube_struct_calldata = 
{
  "cube_struct", CORBA::B_TRUE,
  2, &Cubit_cube_struct_params [0],
  0, 0
};

Cubit_Many*
Cubit::Cubit_cube_struct (Cubit_Many &values,
                          CORBA::Environment &env)
{
  Cubit_Many *retval;

  STUB_Object *istub;

  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR)
    {
      env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));
      return 0;
    }

  this->Release ();
  
  istub->do_call (env, &Cubit_cube_struct_calldata,
		  &retval, &values);
  istub->Release ();
   
  return retval;
}

// CUBE UNION

// NOTE: not all union typecodes can be encoded as an array of "long
// "values, but this one can.  Ones with discriminants that are one or
// two bytes long can't easily be coded portably.
//
// The benefit of doing it as an array of "long" values is twofold:
// (a) easier to read; (b) on most systems it's then adequately
// aligned for the typecode interpreter to use, so no additional
// runtime copy needs to be made.

static const CORBA::Long _oc_Cubit_oneof [] = 
{
  1,          // byte order flag (TRICKY)
  1, 0,       // omitted repository/type ID
  1, 0,       // omitted struct name, "oneof"
   
  //
  // discriminant typecode:
  //
  CORBA::tk_enum,    // CORBA::tk_enum
  72,         // encapsulation length
   
  1,          // byte order flag (TRICKY)
  1, 0,       // omitted repository/type ID
  1, 0,       // omitted enum name, "discrim"
  6,          // 5 elements in the enum
   
  1, 0,       // omitted member name, "e_0th"
  1, 0,       // omitted member name, "e_1st"
  1, 0,       // omitted member name, "e_2nd"
  1, 0,       // omitted member name, "e_3rd" 
  1, 0,       // omitted member name, "e_4th" 
  1, 0,       // omitted member name, "e_5th" 
   
  4,       	// default member index (zero based)
  5,       	// number of union members
   
  // the 1st union branch arm
  e_0th,       // member label value
  1, 0,        // omitted member name, "o"
  CORBA::tk_octet,    // member typecode
   
  // the 2nd union branch arm
  e_1st,      // member label value
  1, 0,       // omitted member name, "s"
  CORBA::tk_short,   // member typecode
   
  // the 3rd union branch arm
  e_2nd,      // member label value
  1, 0,       // omitted member name, "l"
  CORBA::tk_long,    // member typecode
   
  // the 4th union branch arm
  e_3rd,      // member label value
  1, 0,       // omitted member name, "cm"
   
  // the 4th union member typecode
  CORBA::tk_struct,  // CORBA::tk_struct
  60,         // encap length
   
  1,          // byte order flag (TRICKY)
  1, 0,       // omitted repository/type ID
  1, 0,       // omitted struct name, "Many"
  3,          // three struct members
   
  // First structure element
  1, 0,       // omitted member name, "o"
  CORBA::tk_octet,   // member type, CORBA::tk_octet
   
  // Second structure element
  1, 0,       // omitted member name, "l"
  CORBA::tk_long,    // member type, CORBA::tk_long
   
  // Third structure element
  1, 0,       // omitted member name, "s"
  CORBA::tk_short,   // member type, CORBA::tk_short
   
  // the 5th union branch arm
  4,          // the 5th member label value
  1, 0,       // omitted member name, "cm"
  ~0,         // indirected typecode (~0)
  -88         // offset to struct "Many" typecode
};

static CORBA::TypeCode _tc_Cubit_oneof (CORBA::tk_union,
				       (sizeof _oc_Cubit_oneof),
				       (u_char *) &_oc_Cubit_oneof,
                                       CORBA::B_FALSE);
CORBA::TypeCode_ptr TC_Cubit_oneof = &_tc_Cubit_oneof;

static const TAO_Param_Data Cubit_cube_union_params [] = 
{
  { &_tc_Cubit_oneof, PARAM_RETURN, sizeof (Cubit_oneof) },
  { &_tc_Cubit_oneof, PARAM_IN, 0 }
};

static const TAO_Call_Data Cubit_cube_union_calldata = 
{
  "cube_union", CORBA::B_TRUE,
  2, &Cubit_cube_union_params [0],
  0, 0
};

Cubit_oneof *
Cubit::Cubit_cube_union (Cubit_oneof &values,
			 CORBA::Environment &env)
{
  Cubit_oneof *retval;

  STUB_Object *istub;

  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR)
    {
      env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));
      return 0;
    }

  this->Release ();

  istub->do_call (env, &Cubit_cube_union_calldata,
                  &retval, &values);
  istub->Release ();
   
  return retval;
}

// PLEASE EXIT

static const TAO_Call_Data Cubit_please_exit_calldata = 
{
  "please_exit", CORBA::B_FALSE,
  0, 0,
  0, 0
};

void
Cubit::Cubit_please_exit (CORBA::Environment &env)
{
  STUB_Object *istub;

  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR)
    {
      env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));
      return;
    }

  this->Release ();

  istub->do_call (env, &Cubit_please_exit_calldata);
  istub->Release ();
}
