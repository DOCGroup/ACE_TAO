// @(#)cubit.cpp	1.2 95/09/29
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// TEST:	hand-written C-style "Cubit" stubs and "skeletons"
//
// NOTE:  these "skeletons" are really the methods, using DSI.  No real
// ORB would be implemented in this particular way.  Several things would
// be more typical of real (static) skeletons:
//
//	* Most of the "in" (and much of the "out") parameter data would
//	  be preallocated on the stack, not heap allocated.  (Static
//	  preallocation doesnt' really work in a multithreaded system,
//	  and moreover can waste a lot of space.)
//
//	* The ORB core wouldn't be told about parameters using heap
//	  allocated data structures (e.g. NVList).
//
//	* Skeletons would need to some kind of "marshal the response NOW"
//	  API so that stack-allocated "out" values wouldn't become invalid
//	  up until they were safely marshaled.
//
//	* They'd handle exceptions rather than just generating debugging
//	  messages when they happen.
//
//	* Method code would be called by the skeletons, not written as
//	  part of the "skeleton" itself!
//
// A key part of turning this code into a complete ORB would be to ensure
// that skeletons were always efficient and correct.  They might not need
// to be sharable between different implementations of the same OMG-IDL
// object interface, but many ORBs choose to be structured that way.

#include "cubit.h"		// for stubs ...
#include "tao/corba.h"		// ... and skeletons

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
Cubit_cube_octet (Cubit_ptr target,
		  CORBA::Octet o,
		  CORBA::Environment &env)
{
  CORBA::Octet retval = 0;
  STUB_Object *data;

  if (target->QueryInterface (IID_STUB_Object, (void **) &data) != NOERROR)
    env.exception (new CORBA::INV_OBJREF (COMPLETED_NO));
  else 
    {
      data->do_call (env, &Cubit_cube_octet_calldata,
		     &retval, &o);
      data->Release ();
    }

  return retval;
}

static void
_cube_octet_skel (CORBA::ServerRequest &req,    
		  CORBA::Object_ptr obj,
		  CORBA::Environment &env)
{
  CORBA::NVList_ptr nvlist;
  CORBA::NamedValue_ptr nv;
  CORBA::Any temp_value (CORBA::_tc_octet);

  req.orb ()->create_list (0, nvlist);
  nv = nvlist->add_value (0, temp_value, CORBA::ARG_IN, env);
  dexc (env, "cube_octet, add value");

  req.params (nvlist, env);
  dexc (env, "cube_octet, get params");

  CORBA::Octet* value = new CORBA::Octet;

  *value = *(CORBA::Octet *)nv->value ()->value ();
  // dmsg1 ("cube octet, parameter '%d'", *value);

  *value = (CORBA::Octet) ((*value) * (*value) * (*value));
  // dmsg1 ("cube octet, result '%d'", *value);

  CORBA::Any* any =
    new CORBA::Any (CORBA::_tc_octet, value, CORBA::B_TRUE);

  req.result (any, env);
  dexc (env, "cube_octet, result");
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
Cubit_cube_short (Cubit_ptr target,
		  CORBA::Short s,
		  CORBA::Environment &env)
{
  CORBA::Short retval;
  STUB_Object *data;

  if (target->QueryInterface (IID_STUB_Object, (void **) &data) != NOERROR)
    env.exception (new CORBA::INV_OBJREF (COMPLETED_NO));
  else 
    {
      data->do_call (env, &Cubit_cube_short_calldata,
		     &retval, &s);
      data->Release ();
    }
  return retval;
}

static void
_cube_short_skel (CORBA::ServerRequest &req,
		  CORBA::Object_ptr obj,
		  CORBA::Environment &env)
{
  CORBA::NVList_ptr nvlist;
  CORBA::NamedValue_ptr nv;
  CORBA::Any temp_value (CORBA::_tc_short);

  req.orb ()->create_list (0, nvlist);
  nv = nvlist->add_value (0, temp_value, CORBA::ARG_IN, env);
  dexc (env, "cube_short, add_value");

  req.params (nvlist, env);
  dexc (env, "cube_short, get params");

  CORBA::Short* value = new CORBA::Short;

  *value = *(CORBA::Short *)nv->value ()->value ();
  // dmsg1 ("cube short, parameter '%d'", *value);

  *value =(CORBA::Short) ((*value) * (*value) * (*value));
  // dmsg1 ("cube short, result '%d'", *value);

  CORBA::Any* any =
    new CORBA::Any (CORBA::_tc_short, value, CORBA::B_TRUE);

  req.result (any, env);
  dexc (env, "cube_short, result");
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
Cubit_cube_long (Cubit_ptr target,
		 CORBA::Long l,
		 CORBA::Environment &env)
{
  CORBA::Long retval;
  STUB_Object *data;

  if (target->QueryInterface (IID_STUB_Object, (void **)&data) != NOERROR)
    env.exception (new CORBA::INV_OBJREF (COMPLETED_NO));
  else
    {
      data->do_call (env, &Cubit_cube_long_calldata,
		     &retval, &l);
      data->Release ();
    }
  return retval;
}

static void
_cube_long_skel (CORBA::ServerRequest &req,
		 CORBA::Object_ptr obj,
		 CORBA::Environment &env)
{
  CORBA::NVList_ptr nvlist;
  CORBA::NamedValue_ptr	nv;
  CORBA::Any temp_value (CORBA::_tc_long);

  req.orb ()->create_list (0, nvlist);
  nv = nvlist->add_value (0, temp_value, CORBA::ARG_IN, env);
  dexc (env, "cube_long, add_value");

  req.params (nvlist, env);
  dexc (env, "cube_long, get params");

  CORBA::Long* value = new CORBA::Long;

  *value = *(CORBA::Long *)nv->value ()->value ();
  // dmsg1 ("cube long, parameter '%d'", *value);

  *value = (*value) * (*value) * (*value);
  // dmsg1 ("cube long, result '%d'", *value);

  CORBA::Any* any =
    new CORBA::Any (CORBA::_tc_long, value, CORBA::B_TRUE);

  req.result (any, env);
  dexc (env, "cube_long, result");
}

// Encapsulated parameters for struct "Cubit_Many" typecode.
// None of these parameters is complicated, so this is just
// a linear sequence of element encodings
//
// NOTE:  it's important that this be longword aligned!!

static const CORBA::Long _oc_Cubit_Many [] = 
{ 
  1,				// byte order flag (TRICKY!)

  1, 0,			// empty string:  repository/type ID
  1, 0,			// empty string:  struct name

  3,				// three struct elements

  // First structure element:  name, typecode for Octet
  1, 0,			// empty string:  name "o"
  tk_octet,

  // Second structure element:  name, typecode for Long
  1, 0,			// empty string:  name "l"
  tk_long,

  // Third structure element:  name, typecode for Short
  1, 0,			// empty string:  name "s"
  tk_short,
};

static CORBA::TypeCode _tc_Cubit_Many (tk_struct,
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

Cubit_Many *
Cubit_cube_struct (Cubit_ptr target,
		   Cubit_Many &values,
		   CORBA::Environment &env)
{
  Cubit_Many *retval;
  STUB_Object *data;

  if (target->QueryInterface (IID_STUB_Object, (void **) &data) != NOERROR)
    env.exception (new CORBA::INV_OBJREF (COMPLETED_NO));
  else
    {
      data->do_call (env, &Cubit_cube_struct_calldata,
		     &retval, &values);
      data->Release ();
    }
  return retval;
}

static void
_cube_struct_skel (CORBA::ServerRequest &req,
		   CORBA::Object_ptr obj,
		   CORBA::Environment &env)
{
  CORBA::NVList_ptr nvlist;
  CORBA::NamedValue_ptr	nv;
  CORBA::Any temp_value (TC_Cubit_Many);

  req.orb ()->create_list (0, nvlist);
  nv = nvlist->add_value (0, temp_value, CORBA::ARG_IN, env);
  dexc (env, "cube_struct, add_value");

  req.params (nvlist, env);
  dexc (env, "cube_struct, get params");

  Cubit_Many *value;
  Cubit_Many *retval = new Cubit_Many;

  value = (Cubit_Many *)nv->value ()->value ();

  retval->o = (CORBA::Octet) (value->o * value->o * value->o);
  retval->s = (CORBA::Short) (value->s * value->s * value->s);
  retval->l = value->l * value->l * value->l;

  // dmsg2 ("cube struct.o, %d -> %d", value->o, retval->o);
  // dmsg2 ("cube struct.s, %d -> %d", value->s, retval->s);
  // dmsg2 ("cube struct.l, %d -> %d", value->l, retval->l);

  CORBA::Any *any =
    new CORBA::Any (TC_Cubit_Many, retval, CORBA::B_TRUE);

  req.result (any, env);
  dexc (env, "cube_struct, result");
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
  1,				// byte order flag (TRICKY)
  1, 0,			// omitted repository/type ID
  1, 0,			// omitted struct name, "oneof"

  // discriminant typecode:
  tk_enum,		// tk_enum
  72,			// encapsulation length

  1,			// byte order flag (TRICKY)
  1, 0,			// omitted repository/type ID
  1, 0,			// omitted enum name, "discrim"
  6,			// 5 elements in the enum

  1, 0,			// omitted member name, "e_0th"
  1, 0,			// omitted member name, "e_1st"
  1, 0,			// omitted member name, "e_2nd"
  1, 0,			// omitted member name, "e_3rd" 
  1, 0,			// omitted member name, "e_4th" 
  1, 0,			// omitted member name, "e_5th" 

  4,				// default member index (zero based)
  5,				// number of union members

  // the 1st union branch arm
  e_0th,			// member label value
  1, 0,			// omitted member name, "o"
  tk_octet,		// member typecode

  // the 2nd union branch arm
  e_1st,			// member label value
  1, 0,			// omitted member name, "s"
  tk_short,		// member typecode

  // the 3rd union branch arm
  e_2nd,			// member label value
  1, 0,			// omitted member name, "l"
  tk_long,		// member typecode

  // the 4th union branch arm
  e_3rd,			// member label value
  1, 0,			// omitted member name, "cm"

  // the 4th union member typecode
  tk_struct,		// tk_struct
  60,			// encap length

  1,			// byte order flag (TRICKY)
  1, 0,			// omitted repository/type ID
  1, 0,			// omitted struct name, "Many"
  3,			// three struct members

  // First structure element
  1, 0,			// omitted member name, "o"
  tk_octet,	// member type, tk_octet

  // Second structure element
  1, 0,			// omitted member name, "l"
  tk_long,	// member type, tk_long

  // Third structure element
  1, 0,			// omitted member name, "s"
  tk_short,	// member type, tk_short

  // the 5th union branch arm
  4,				// the 5th member label value
  1, 0,			// omitted member name, "cm"
  ~0,				// indirected typecode (~0)
  -84			// offset to struct "Many" typecode
};

static CORBA::TypeCode _tc_Cubit_oneof (tk_union,
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
Cubit_cube_union (Cubit_ptr target,
		  Cubit_oneof &values,
		  CORBA::Environment &env)
{
  Cubit_oneof *retval;
  STUB_Object *data;

  if (target->QueryInterface (IID_STUB_Object, (void **) &data) != NOERROR)
    env.exception (new CORBA::INV_OBJREF (COMPLETED_NO));
  else
    {
      data->do_call (env, &Cubit_cube_union_calldata,
		     &retval, &values);
      data->Release ();
    }
  return retval;
}

static void
cube_union_skel (CORBA::ServerRequest &req,
		 CORBA::Object_ptr obj,
		 CORBA::Environment &env)
{
  CORBA::NVList_ptr nvlist;
  CORBA::NamedValue_ptr nv;
  CORBA::Any temp_value (TC_Cubit_oneof);

  req.orb ()->create_list (0, nvlist);
  nv = nvlist->add_value (0, temp_value, CORBA::ARG_IN, env);
  dexc (env, "cube_union_3rd, add_value");

  req.params (nvlist, env);
  dexc (env, "cube_union_3rd, get params");

  Cubit_oneof *v;
  Cubit_oneof *r = new Cubit_oneof;

  v = (Cubit_oneof *)nv->value ()->value ();
  r->_disc = v->_disc;

  switch (v->_disc) 
    {
    case e_0th:
      r->o = (CORBA::Octet) (v->o * v->o * v->o);
      break;

    case e_1st:
      r->s = (CORBA::Short) (v->s * v->s * v->s);
      break;

    case e_2nd:
      r->l = v->l * v->l * v->l;
      break;

    case e_3rd:
    default:
      r->cm.o = (CORBA::Octet) (v->cm.o * v->cm.o * v->cm.o);
      r->cm.s = (CORBA::Short) (v->cm.s * v->cm.s * v->cm.s);
      r->cm.l = v->cm.l * v->cm.l * v->cm.l;
      break;
    }

  CORBA::Any *any = new CORBA::Any (TC_Cubit_oneof, r, CORBA::B_TRUE);

  req.result (any, env);
  dexc (env, "cube_struct, result");
}

// PLEASE EXIT

static const TAO_Call_Data Cubit_please_exit_calldata = 
{
  "please_exit", CORBA::B_FALSE,
  0, 0,
  0, 0
};

void
Cubit_please_exit (Cubit_ptr target,
		   CORBA::Environment &env)
{
  STUB_Object *data;

  if (target->QueryInterface (IID_STUB_Object, (void **) &data) != NOERROR)
    env.exception (new CORBA::INV_OBJREF (COMPLETED_NO));
  else 
    {
      data->do_call (env, &Cubit_please_exit_calldata);
      data->Release ();
    }
}

static void
_please_exit_skel (CORBA::ServerRequest &req,
		   CORBA::Object_ptr obj,
		   CORBA::Environment &env)
{
  dmsg ("I've been asked to shut down...");
  req.oa ()->please_shutdown (env);
  dexc (env, "please_exit, please_shutdown");
}

const CORBA::Char *Cubit__id = (CORBA::Char *) "IDL:Eng.SUN.COM/Cubit:1.1";

// table of all operations, used by operation dispatch to get to the
// right skeleton ... could be sorted by the IDL compiler so bsearch
// is effective, perhaps with help from opname hashes and a small
// cache (e.g. like Obj-C?).  for now, just lsearch.

const TAO_Skel_Entry Cubit_operations [] = 
{
  { &Cubit_cube_octet_calldata, _cube_octet_skel },
  { &Cubit_cube_short_calldata, _cube_short_skel },
  { &Cubit_cube_long_calldata, _cube_long_skel },
  { &Cubit_cube_struct_calldata, _cube_struct_skel },
  { &Cubit_cube_union_calldata, _cube_union_skel },
  { &Cubit_please_exit_calldata, _please_exit_skel },
  { 0, 0 } // last entry
};
