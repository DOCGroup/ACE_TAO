// $Id$

#include <iostream.h>
#include "marshalS.h"

ACE_RCSID(TAO, marshalS, "$Id$")

static const TAO_operation_db_entry Marshal_operations[] = {
  {"_is_a", &_skel_Marshal::_Marshal_is_a_skel},
  { "test_short", &_skel_Marshal::_Marshal_test_short_skel},
  { "test_long", &_skel_Marshal::_Marshal_test_long_skel},
  { "test_octet", &_skel_Marshal::_Marshal_test_octet_skel},
  { "test_char", &_skel_Marshal::_Marshal_test_char_skel},
  { "test_double", &_skel_Marshal::_Marshal_test_double_skel},
  { "test_struct", &_skel_Marshal::_Marshal_test_struct_skel},
  { "test_union", &_skel_Marshal::_Marshal_test_union_skel},
  { "test_any", &_skel_Marshal::_Marshal_test_any_skel},
  { "test_sequence", &_skel_Marshal::_Marshal_test_sequence_skel},
  { 0, 0 }
};

TAO_Dynamic_Hash_OpTable tao_Marshal_optable(Marshal_operations, 10,
					     // 9 + 1 operations on our 
					     // interface 
					     20);
// constructor
_skel_Marshal::_skel_Marshal (const char *obj_name)
{
  const CORBA_String repoID = "IDL:Marshal:1.0"; // repository ID
  IIOP_Object *data;  // IIOP object 
  CORBA_BOA_ptr oa = TAO_OA_PARAMS::instance()->oa(); // underlying BOA
  this->optable_ = &tao_Marshal_optable;     // operation database
  CORBA_Long i;

  // setup an IIOP object
  data = new IIOP_Object (CORBA_string_dup (repoID));
  data->profile.iiop_version.major = IIOP::MY_MAJOR;
  data->profile.iiop_version.minor = IIOP::MY_MINOR;
  data->profile.host = ACE_OS::strdup(oa->get_addr().get_host_name());
  data->profile.port = oa->get_addr().get_port_number();
  data->profile.object_key.length = ACE_OS::strlen(obj_name);
  data->profile.object_key.maximum = data->profile.object_key.length;
  data->profile.object_key.buffer = new CORBA_Octet [(size_t)data->profile.object_key.length+1];
  ACE_OS::memcpy (data->profile.object_key.buffer, obj_name,
		  data->profile.object_key.length); // set the object key
  this->set_parent(data);  // store the IIOP obj reference with ourselves
  this->sub_ = this;               // set the most derived class to be ourselves
  if (oa) oa->bind(data->profile.object_key, this);// register ourselves
}

// skeletons
void _skel_Marshal::_Marshal_test_short_skel(CORBA_ServerRequest &req, 
					     CORBA_Object_ptr obj, 
					     CORBA_Environment &env)
{
#if 0
  CORBA_NVList_ptr        nvlist;
  CORBA_NamedValue_ptr    nv;
  CORBA_Any               temp_value(_tc_CORBA_Short);
  Marshal_ptr       impl;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);
  nv = nvlist->add_value(0, temp_value, CORBA_ARG_IN, env);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  Marshal::ShortSeq *value = (Marshal::ShortSeq *)nv->value()->value();

  // now invoke the implementation
  impl = (Marshal_ptr)(obj->get_subclass());
  // now the magic of dynamic binding
  impl->sendShortSeq(*value, env);

  // result - NO result
  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, 1);
  req.result(any, env);
#endif

  // One immediate thought is to do the following:
  // define a specialized marshal factory derived from TAO_Marshal_Factory
  // then:
  // CDR compiledStream (req._incoming.next, req._incoming.remaining,
  // byte_order, consume_buf = 0, instance of our marshal factory
}

void _skel_Marshal::_Marshal_test_long_skel(CORBA_ServerRequest &req, 
					    CORBA_Object_ptr obj, 
					    CORBA_Environment &env)
{
}

void _skel_Marshal::_Marshal_test_octet_skel(CORBA_ServerRequest &req, 
					     CORBA_Object_ptr obj, 
					     CORBA_Environment &env)
{
}

void _skel_Marshal::_Marshal_test_char_skel(CORBA_ServerRequest &req, 
					    CORBA_Object_ptr obj, 
					    CORBA_Environment &env)
{
}

void _skel_Marshal::_Marshal_test_double_skel(CORBA_ServerRequest &req, 
					      CORBA_Object_ptr obj, 
					      CORBA_Environment &env)
{
}

void _skel_Marshal::_Marshal_test_struct_skel(CORBA_ServerRequest &req, 
					      CORBA_Object_ptr obj, 
					      CORBA_Environment &env)
{
}

void _skel_Marshal::_Marshal_test_union_skel(CORBA_ServerRequest &req, 
					     CORBA_Object_ptr obj, 
					     CORBA_Environment &env)
{
}

void _skel_Marshal::_Marshal_test_any_skel(CORBA_ServerRequest &req, 
					   CORBA_Object_ptr obj, 
					   CORBA_Environment &env)
{
}

void _skel_Marshal::_Marshal_test_sequence_skel(CORBA_ServerRequest &req, 
						CORBA_Object_ptr obj, 
						CORBA_Environment &env)
{
}

void _skel_Marshal::_Marshal_is_a_skel (
					CORBA_ServerRequest	&req,
					CORBA_Object_ptr    obj,
					CORBA_Environment		&env
					)
{
  CORBA_NVList_ptr		nvlist;
  CORBA_NamedValue_ptr	nv;
  CORBA_Any			temp_value (_tc_CORBA_String);
  char *type_id = "IDL:Marshal:1.0";

  req.orb()->create_list (0, nvlist);
  nv = nvlist->add_value (0, temp_value, CORBA_ARG_IN, env);

  req.params (nvlist, env);
  if (env.exception () != 0) {
    dexc (env, "is_a_skel, get params");
    return;
  }

  CORBA_Boolean		*retval;
  CORBA_String		value = *(CORBA_String *)nv->value ()->value ();

  cout << "object id = " << ((char *)value) << endl;
  if (strcmp ((char *)value, (char *)type_id) == 0
      || strcmp ((char *)value, _tc_CORBA_Object->id(env)) == 0)
    retval = new CORBA_Boolean (1);
  else
    retval = new CORBA_Boolean (0);

  CORBA_Any  		*any =
    new CORBA_Any (_tc_CORBA_Boolean, retval, 1);

  req.result (any, env);
  dexc (env, "_is_a, result");
}

