// @(#)cubitS.cpp	05/14/97
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// TEST:	hand-written C-style "Cubit" stubs and "skeletons"
//

#include "tao/corba.h"

#include "cubitS.h"
#include "cubitC.cpp"

#include "method_db.i"

TAO_Dynamic_Hash_OpTable tao_cubit_optable (cubit_operations, 7, 14);  // Dynamic Operation Table

_skel_Cubit::_skel_Cubit (const char *obj_name)
{
  const char* mn = "_skel_Cubit::_skel_Cubit ()";
  
  char *type_id = "IDL:Cubit:1.0";
  IIOP_Object *data;

  CORBA_BOA_ptr oa = TAO_OA_Parameters::instance ()->oa ();
  if (oa == 0)
    {
      // We just have to assume that oa will be good, or we have to
      // throw an exception.  For now we "assume", but we'll
      ACE_ERROR ((LM_ERROR, " (%P|%t) %s Unable to locate a valid object adapter\n", mn));
      return;
    }
   
  this->optable_ = &tao_cubit_optable;
  data = new IIOP_Object (type_id);

  data->profile.iiop_version.major = IIOP::MY_MAJOR;
  data->profile.iiop_version.minor = IIOP::MY_MINOR;
  data->profile.host = ACE_OS::strdup (oa->get_addr ().get_host_name ());
  data->profile.port = oa->get_addr ().get_port_number ();
  data->profile.object_key.length =  ACE_OS::strlen (obj_name);
  data->profile.object_key.maximum = data->profile.object_key.length;
  data->profile.object_key.buffer =
    new CORBA_Octet[(size_t)data->profile.object_key.length + 1];
   
  ACE_OS::memcpy (data->profile.object_key.buffer,
		  obj_name,
		  data->profile.object_key.length+1);

  this->set_parent (data);
  this->sub_ = this;

  if (oa->bind (data->profile.object_key, this) == -1)
    {
      ACE_ERROR ((LM_ERROR,
		  " (%P|%t) %s Unable to bind object to key '%s': %p\n",
		  mn,
		  data->profile.object_key.buffer));
      return;
    }
}

void
_skel_Cubit::_cube_octet_skel (CORBA_ServerRequest &req,
			       CORBA_Object_ptr    obj,
			       CORBA_Environment   &env)
{
   CORBA_NVList_ptr nvlist;
   CORBA_NamedValue_ptr	nv;
   CORBA_Any temp_value (_tc_CORBA_Octet);
   
   req.orb ()->create_list (0, nvlist);
   nv = nvlist->add_value (0, temp_value, CORBA_ARG_IN, env);
   dexc (env, "cube_octet, add value");
   
   req.params (nvlist, env);
   dexc (env, "cube_octet, get params");
   
   // Call Implementation
   CORBA_Octet *value = new CORBA_Octet;
   Cubit *the_cubit = (Cubit *) obj->get_subclass ();

   *value = the_cubit->Cubit_cube_octet (*(CORBA_Octet *) nv->value ()->value (), env);

   CORBA_Any *any = new CORBA_Any (_tc_CORBA_Octet, value, CORBA_B_TRUE);
   
   req.result (any, env);
   dexc (env, "cube_octet, result");
}

void
_skel_Cubit::_cube_short_skel (CORBA_ServerRequest &req,
                               CORBA_Object_ptr    obj,
                               CORBA_Environment   &env)
{
   CORBA_NVList_ptr		nvlist;
   CORBA_NamedValue_ptr	nv;
   CORBA_Any			temp_value (_tc_CORBA_Short);
   
   req.orb ()->create_list (0, nvlist);
   nv = nvlist->add_value (0, temp_value, CORBA_ARG_IN, env);
   dexc (env, "cube_short, add_value");
   
   req.params (nvlist, env);
   dexc (env, "cube_short, get params");
   
   // Call Implementation
   CORBA_Short *value = new CORBA_Short;
   Cubit *the_cubit = (Cubit *) obj->get_subclass ();
   *value = the_cubit->Cubit_cube_short (*(CORBA_Short *) nv->value ()->value (), env);
   
   CORBA_Any *any = new CORBA_Any (_tc_CORBA_Short, value, CORBA_B_TRUE);
   
   req.result (any, env);
   dexc (env, "cube_short, result");
}

void
_skel_Cubit::_cube_long_skel (CORBA_ServerRequest &req,
                              CORBA_Object_ptr    obj,
                              CORBA_Environment   &env)
{
   CORBA_NVList_ptr		nvlist;
   CORBA_NamedValue_ptr	nv;
   CORBA_Any			temp_value (_tc_CORBA_Long);
   
   req.orb ()->create_list (0, nvlist);
   nv = nvlist->add_value (0, temp_value, CORBA_ARG_IN, env);
   dexc (env, "cube_long, add_value");
   
   req.params (nvlist, env);
   dexc (env, "cube_long, get params");
   
   // Call Implementation
   CORBA_Long		*value = new CORBA_Long;
   Cubit *the_cubit = (Cubit *) obj->get_subclass ();
   *value = the_cubit->Cubit_cube_long (*(CORBA_Long *) nv->value ()->value (), env);
   
   CORBA_Any  		*any =
      new CORBA_Any (_tc_CORBA_Long, value, CORBA_B_TRUE);
   
   req.result (any, env);
   dexc (env, "cube_long, result");
}

void
_skel_Cubit::_cube_struct_skel (CORBA_ServerRequest &req,
                                CORBA_Object_ptr    obj,
                                CORBA_Environment   &env)
{
  CORBA_NVList_ptr nvlist;
  CORBA_NamedValue_ptr nv;
  CORBA_Any temp_value (TC_Cubit_Many);
   
  req.orb ()->create_list (0, nvlist);
  nv = nvlist->add_value (0, temp_value, CORBA_ARG_IN, env);
  dexc (env, "cube_struct, add_value");
   
  req.params (nvlist, env);
  dexc (env, "cube_struct, get params");
   
  // Call Implementation
  Cubit_Many  *value;
  Cubit *the_cubit = (Cubit *) obj->get_subclass ();
  value = the_cubit->Cubit_cube_struct (*(Cubit_Many *) nv->value ()->value (), env);
   
  Cubit_Many  *retval = new Cubit_Many;
  retval->o = (CORBA_Octet)value->o;
  retval->s = (CORBA_Short)value->s;
  retval->l = value->l;
   
  // dmsg2 ("cube struct.o, %d -> %d", value->o, retval->o);
  // dmsg2 ("cube struct.s, %d -> %d", value->s, retval->s);
  // dmsg2 ("cube struct.l, %d -> %d", value->l, retval->l);
   
  CORBA_Any *any = new CORBA_Any (TC_Cubit_Many, retval, CORBA_B_TRUE);
   
  req.result (any, env);
  dexc (env, "cube_struct, result");
}

void
_skel_Cubit::_cube_union_skel (CORBA_ServerRequest	&req,
                               CORBA_Object_ptr    obj,
                               CORBA_Environment   &env)
{
  CORBA_NVList_ptr nvlist;
  CORBA_NamedValue_ptr nv;
  CORBA_Any temp_value (TC_Cubit_oneof);
   
  req.orb ()->create_list (0, nvlist);
  nv = nvlist->add_value (0, temp_value, CORBA_ARG_IN, env);
  dexc (env, "cube_union_3rd, add_value");
   
  req.params (nvlist, env);
  dexc (env, "cube_union_3rd, get params");
   
  // Call Implementation
  Cubit_oneof    *v;
  Cubit *the_cubit = (Cubit *)obj->get_subclass ();
  v = the_cubit->Cubit_cube_union (*(Cubit_oneof *) nv->value ()->value (), env);

  Cubit_oneof    *r = new Cubit_oneof;
  r->_disc = v->_disc;
  r->o = (CORBA_Octet) (v->o);
  r->s = (CORBA_Short) (v->s);
  r->l = v->l;
  r->cm.o = (CORBA_Octet) (v->cm.o);
  r->cm.s = (CORBA_Short) (v->cm.s);
  r->cm.l =  v->cm.l;
   
  CORBA_Any *any = new CORBA_Any (TC_Cubit_oneof, r, CORBA_B_TRUE);
   
  req.result (any, env);
  dexc (env, "cube_struct, result");
}

void
_skel_Cubit::_please_exit_skel (CORBA_ServerRequest &req,
                                CORBA_Object_ptr    obj,
                                CORBA_Environment   &env)
{
  ACE_UNUSED_ARG (req);
  
  Cubit *the_cubit = (Cubit *) obj->get_subclass ();
  the_cubit->Cubit_please_exit (env);
}
