#include <iostream.h>
#include "ttcpS.h"
#include "corba/orb.h"
#include "params.h"
#include "iiopobj.h"
#include "corba/optable.h"  // for registering our methods
#include "corba/corbacom.h"

TAO_Dynamic_Hash_OpTable tao_ttcp_sequence_optable(9); // 8 + 1 operations on our
						       // interface 
// operation database
struct method_db
{
   CORBA_String opname;
   TAO_Skeleton skel_ptr;
};

static const method_db ttcp_sequence_operations[] = {
  {"_is_a", &_skel_ttcp_sequence::_ttcp_sequence_is_a_skel},
   { "sendShortSeq", &_skel_ttcp_sequence::_ttcp_sequence_sendShortSeq_skel},
   { "sendLongSeq", &_skel_ttcp_sequence::_ttcp_sequence_sendLongSeq_skel},
   { "sendDoubleSeq", &_skel_ttcp_sequence::_ttcp_sequence_sendDoubleSeq_skel},
   { "sendOctetSeq", &_skel_ttcp_sequence::_ttcp_sequence_sendOctetSeq_skel},
   { "sendCharSeq", &_skel_ttcp_sequence::_ttcp_sequence_sendCharSeq_skel},
   { "sendStructSeq", &_skel_ttcp_sequence::_ttcp_sequence_sendStructSeq_skel},
   { "start_timer", &_skel_ttcp_sequence::_ttcp_sequence_start_timer_skel},
   { "stop_timer", &_skel_ttcp_sequence::_ttcp_sequence_stop_timer_skel},
   { 0, 0 }
};

// constructor
_skel_ttcp_sequence::_skel_ttcp_sequence(const char *obj_name)
{
  CORBA_String_var  type_id = "IDL:ttcp_sequence:1.0"; // repository ID
  IIOP_Object *data;  // IIOP object 
  CORBA_BOA_ptr oa = TAO_OA_PARAMS::instance()->oa(); // underlying BOA
  this->optable_ = &tao_ttcp_sequence_optable;     // operation database
  CORBA_Long i;

  // insert operation names into the operation table
  for (i=0; i < 9; i++)
    {
      this->optable_->bind(ttcp_sequence_operations[i].opname,
			   ttcp_sequence_operations[i].skel_ptr);
    }

  // setup an IIOP object
  data = new IIOP_Object(type_id);
  data->profile.iiop_version.major = IIOP::MY_MAJOR;
  data->profile.iiop_version.minor = IIOP::MY_MINOR;
  data->profile.host = ACE_OS::strdup(oa->get_addr().get_host_name());
  data->profile.port = oa->get_addr().get_port_number();
  data->profile.object_key.length = ACE_OS::strlen(obj_name);
  data->profile.object_key.maximum = data->profile.object_key.length;
  data->profile.object_key.buffer = new CORBA_Octet [(size_t)data->profile.object_key.length+1];
  ACE_OS::memcpy(data->profile.object_key.buffer, obj_name,
		 data->profile.object_key.length); // set the object key
  this->set_parent(data);  // store the IIOP obj reference with ourselves
  this->sub_ = this;               // set the most derived class to be ourselves
  if (oa) oa->bind(data->profile.object_key, this);// register ourselves
}

// skeleton sendShortSeq
void _skel_ttcp_sequence::_ttcp_sequence_sendShortSeq_skel(CORBA_ServerRequest &req, 
							   CORBA_Object_ptr obj, 
							   CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  CORBA_NamedValue_ptr    nv;
  CORBA_Any               temp_value(ttcp_sequence::_tc_ShortSeq);
  ttcp_sequence_ptr       impl;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);
  nv = nvlist->add_value(0, temp_value, CORBA_ARG_IN, env);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  ttcp_sequence::ShortSeq *value = (ttcp_sequence::ShortSeq *)nv->value()->value();

  // now invoke the implementation
  impl = (ttcp_sequence_ptr)(obj->get_subclass());
  // now the magic of dynamic binding
  impl->sendShortSeq(*value, env);

  // result - NO result
  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, CORBA_B_TRUE);
  req.result(any, env);
}

// skeleton sendLongSeq
void _skel_ttcp_sequence::_ttcp_sequence_sendLongSeq_skel(CORBA_ServerRequest &req, 
							   CORBA_Object_ptr obj, 
							   CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  CORBA_NamedValue_ptr    nv;
  CORBA_Any               temp_value(ttcp_sequence::_tc_LongSeq);
  ttcp_sequence_ptr       impl;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);
  nv = nvlist->add_value(0, temp_value, CORBA_ARG_IN, env);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  ttcp_sequence::LongSeq *value = (ttcp_sequence::LongSeq *)nv->value()->value();

  // now invoke the implementation
  impl = (ttcp_sequence_ptr)(obj->get_subclass());
  // now the magic of dynamic binding
  impl->sendLongSeq(*value, env);

  // result - NO result
  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, CORBA_B_TRUE);
  req.result(any, env);
}

// skeleton sendDoubleSeq
void _skel_ttcp_sequence::_ttcp_sequence_sendDoubleSeq_skel(CORBA_ServerRequest &req, 
							   CORBA_Object_ptr obj, 
							   CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  CORBA_NamedValue_ptr    nv;
  CORBA_Any               temp_value(ttcp_sequence::_tc_DoubleSeq);
  ttcp_sequence_ptr       impl;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);
  nv = nvlist->add_value(0, temp_value, CORBA_ARG_IN, env);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  ttcp_sequence::DoubleSeq *value = (ttcp_sequence::DoubleSeq *)nv->value()->value();

  // now invoke the implementation
  impl = (ttcp_sequence_ptr)(obj->get_subclass());
  // now the magic of dynamic binding
  impl->sendDoubleSeq(*value, env);

  // result - NO result
  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, CORBA_B_TRUE);
  req.result(any, env);
}

// skeleton sendOctetSeq
void _skel_ttcp_sequence::_ttcp_sequence_sendOctetSeq_skel(CORBA_ServerRequest &req, 
							   CORBA_Object_ptr obj, 
							   CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  CORBA_NamedValue_ptr    nv;
  CORBA_Any               temp_value(ttcp_sequence::_tc_OctetSeq);
  ttcp_sequence_ptr       impl;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);
  nv = nvlist->add_value(0, temp_value, CORBA_ARG_IN, env);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  ttcp_sequence::OctetSeq *value = (ttcp_sequence::OctetSeq *)nv->value()->value();

  // now invoke the implementation
  impl = (ttcp_sequence_ptr)(obj->get_subclass());
  // now the magic of dynamic binding
  impl->sendOctetSeq(*value, env);

  // result - NO result
  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, CORBA_B_TRUE);
  req.result(any, env);
}

// skeleton sendCharSeq
void _skel_ttcp_sequence::_ttcp_sequence_sendCharSeq_skel(CORBA_ServerRequest &req, 
							   CORBA_Object_ptr obj, 
							   CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  CORBA_NamedValue_ptr    nv;
  CORBA_Any               temp_value(ttcp_sequence::_tc_CharSeq);
  ttcp_sequence_ptr       impl;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);
  nv = nvlist->add_value(0, temp_value, CORBA_ARG_IN, env);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  ttcp_sequence::CharSeq *value = (ttcp_sequence::CharSeq *)nv->value()->value();

  // now invoke the implementation
  impl = (ttcp_sequence_ptr)(obj->get_subclass());
  // now the magic of dynamic binding
  impl->sendCharSeq(*value, env);

  // result - NO result
  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, CORBA_B_TRUE);
  req.result(any, env);
}

// skeleton sendStructSeq
void _skel_ttcp_sequence::_ttcp_sequence_sendStructSeq_skel(CORBA_ServerRequest &req, 
							   CORBA_Object_ptr obj, 
							   CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  CORBA_NamedValue_ptr    nv;
  CORBA_Any               temp_value(ttcp_sequence::_tc_StructSeq);
  ttcp_sequence_ptr       impl;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);
  nv = nvlist->add_value(0, temp_value, CORBA_ARG_IN, env);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  ttcp_sequence::StructSeq *value = (ttcp_sequence::StructSeq *)nv->value()->value();

  // now invoke the implementation
  impl = (ttcp_sequence_ptr)(obj->get_subclass());
  // now the magic of dynamic binding
  impl->sendStructSeq(*value, env);

  // result - NO result
  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, CORBA_B_TRUE);
  req.result(any, env);
}

// skeleton start_timer
void _skel_ttcp_sequence::_ttcp_sequence_start_timer_skel(CORBA_ServerRequest &req, 
							   CORBA_Object_ptr obj, 
							   CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  ttcp_sequence_ptr       impl;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  // now invoke the implementation
  impl = (ttcp_sequence_ptr)(obj->get_subclass());
  // now the magic of dynamic binding
  impl->start_timer(env);

  // result - NO result
  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, CORBA_B_TRUE);
  req.result(any, env);
}

// skeleton stop_timer
void _skel_ttcp_sequence::_ttcp_sequence_stop_timer_skel(CORBA_ServerRequest &req, 
							   CORBA_Object_ptr obj, 
							   CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  ttcp_sequence_ptr       impl;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  // now invoke the implementation
  impl = (ttcp_sequence_ptr)(obj->get_subclass());
  // now the magic of dynamic binding
  impl->stop_timer(env);

  // result - NO result
  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, CORBA_B_TRUE);
  req.result(any, env);
}

void _skel_ttcp_sequence::_ttcp_sequence_is_a_skel (
						    CORBA_ServerRequest	&req,
						    CORBA_Object_ptr    obj,
						    CORBA_Environment		&env
						    )
{
  CORBA_NVList_ptr		nvlist;
  CORBA_NamedValue_ptr	nv;
  CORBA_Any			temp_value (_tc_CORBA_String);
  char *type_id = "IDL:ttcp_sequence:1.0";

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
    retval = new CORBA_Boolean (CORBA_B_TRUE);
  else
    retval = new CORBA_Boolean (CORBA_B_FALSE);

  CORBA_Any  		*any =
    new CORBA_Any (_tc_CORBA_Boolean, retval, CORBA_B_TRUE);

  req.result (any, env);
  dexc (env, "_is_a, result");
}

