// @ (#)method_db.i 05/14/97

// This file provides a way of initializing the method database
// Written by: Brian Mendel

#include	"cubitS.h"

static void
is_a_skel (CORBA_ServerRequest &req,
           CORBA_Object_ptr    obj,
           CORBA_Environment   &env)
{
  ACE_UNUSED_ARG (obj);
  
  CORBA_NVList_ptr		nvlist;
  CORBA_NamedValue_ptr	nv;
  CORBA_Any			temp_value (_tc_CORBA_String);

  req.orb ()->create_list (0, nvlist);
  nv = nvlist->add_value (0, temp_value, CORBA_ARG_IN, env);

  req.params (nvlist, env);
  if (env.exception () != 0) 
    {
      dexc (env, "is_a_skel, get params");
      return;
    }

  CORBA_Boolean *retval;
  CORBA_String value = * (CORBA_String *) nv->value ()->value ();

  if (ACE_OS::strcmp ( (char*)value, "IDL:Cubit:1.0") == 0
      || ACE_OS::strcmp ( (char *)value, _tc_CORBA_Object->id (env)) == 0)
    retval = new CORBA_Boolean (CORBA_B_TRUE);
  else
    retval = new CORBA_Boolean (CORBA_B_FALSE);

  CORBA_Any *any = new CORBA_Any (_tc_CORBA_Boolean, retval, CORBA_B_TRUE);

  req.result (any, env);
  dexc (env, "_is_a, result");
}

static const TAO_operation_db_entry cubit_operations[] = {
   { "_is_a", &is_a_skel },
   { "cube_octet", &_skel_Cubit::_cube_octet_skel },
   { "cube_short", &_skel_Cubit::_cube_short_skel },
   { "cube_long", &_skel_Cubit::_cube_long_skel },
   { "cube_struct", &_skel_Cubit::_cube_struct_skel },
   { "cube_union", &_skel_Cubit::_cube_union_skel },
   { "please_exit", &_skel_Cubit::_please_exit_skel },
   { 0, 0 }
};
   

