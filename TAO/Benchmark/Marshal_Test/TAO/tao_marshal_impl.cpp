// $Id$

#include "tao_marshal_impl.h"

#if !defined (__ACE_INLINE__)
#include "tao_marshal_impl.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO, tao_marshal_impl, "$Id$")

// All the methods of the SSI class
void
Marshal_SSI_Impl::test_short (CORBA_Short s1,
			      CORBA_Short &s2,
			      CORBA_Environment &env)
{
  s2 = 2*s1;
}

void
Marshal_SSI_Impl::test_long (CORBA_Long l1,
			     CORBA_Long &l2,
			      CORBA_Environment &env)
{
  l2 = 3*l1;
}

void
Marshal_SSI_Impl::test_octet (CORBA_Octet o1,
			      CORBA_Octet &o2,
			      CORBA_Environment &env)
{
  o2 = o1;
}

void
Marshal_SSI_Impl::test_char (CORBA_Char c1,
			     CORBA_Char &c2,
			      CORBA_Environment &env)
{
  c2 = c1;
}

void
Marshal_SSI_Impl::test_double (CORBA_Double d1,
			       CORBA_Double &d2,
			      CORBA_Environment &env)
{
  d2 = d1/2;
}

void
Marshal_SSI_Impl::test_struct (const Marshal::Marshal_Struct& ms1,
			       Marshal::Marshal_Struct& ms2,
			      CORBA_Environment &env)
{
  ms2.s = ms1.s;
  ms2.l = ms1.l;
  ms2.c = ms1.c;
  ms2.o = ms1.o;
  ms2.d = ms1.d;
}

void
Marshal_SSI_Impl::test_union (const Marshal::Marshal_Union& u1,
			      Marshal::Marshal_Union& u2,
			      CORBA_Environment &env)
{
  u2._d (u1._d ()); // set the discriminant value

  switch (u1._d ())
    {
    case Marshal::e_0th:
      u2.s (u1.s ()); // set short
      break;
    case Marshal::e_1st:
      u2.l (u1.l ()); // set long
      break;
    case Marshal::e_2nd:
      u2.c (u1.c ()); // set char
      break;
    case Marshal::e_3rd:
      u2.o (u1.o ()); // set octet
      break;
    case Marshal::e_4th:
      u2.d (u1.d ()); // set double
      break;
    case Marshal::e_5th:
    default:
      u2.ms (u1. ms ()); // set structs
      break;
    }
}

void
Marshal_SSI_Impl::test_any (const CORBA_Any &a1,
			    CORBA_Any *&a2,
			      CORBA_Environment &env)
{
  a2 = new CORBA_Any (a1.type (), (void *)a1.value ()); // will do a deep copy
}

void 
Marshal_SSI_Impl::test_sequence (const Marshal::AnySeq& as1,
				 Marshal::AnySeq *& as2,
			      CORBA_Environment &env)
{
  as2 = new Marshal::AnySeq (as1);
}

#if 0
void 
Marshal_SSI_Impl::test_recursive (const Marshal::Marshal_Recursive &mr1,
				  Marshal::Marshal_Recursive *&mr2,
			      CORBA_Environment &env)
{
}
#endif

//----------------------------------------------------------------------------
//the methods of the DSI implementation class

static const TAO_operation_db_entry Marshal_DSI_operations[] = {
  {"_is_a", &Marshal_DSI_Impl::_Marshal_is_a_skel},
  { "test_short", &Marshal_DSI_Impl::_Marshal_test_short_skel},
  { "test_long", &Marshal_DSI_Impl::_Marshal_test_long_skel},
  { "test_octet", &Marshal_DSI_Impl::_Marshal_test_octet_skel},
  { "test_char", &Marshal_DSI_Impl::_Marshal_test_char_skel},
  { "test_double", &Marshal_DSI_Impl::_Marshal_test_double_skel},
  { "test_struct", &Marshal_DSI_Impl::_Marshal_test_struct_skel},
  { "test_union", &Marshal_DSI_Impl::_Marshal_test_union_skel},
  { "test_any", &Marshal_DSI_Impl::_Marshal_test_any_skel},
  { "test_sequence", &Marshal_DSI_Impl::_Marshal_test_sequence_skel},
  { 0, 0 }
};

// Note that we use a linear table because rest of the DSI implementations we
// compare with also use linear search in their "invoke" method.
TAO_Linear_OpTable tao_Marshal_DSI_optable (Marshal_DSI_operations, 10);

// constructor
Marshal_DSI_Impl::Marshal_DSI_Impl (CORBA_ORB_ptr orb, const char *obj_name)
  : orb_ (orb)
  //,CORBA_DynamicImplementation ("Marshal", obj_name) // interface name is necessary
{
  // Note that this is a HACK to achieve DSI behavior. But this way we can
  // cleanly integrate ourselves with the dispatch mechanism of the OA.

  const CORBA_String repoID = "IDL:Marshal:1.0"; // repository ID
  IIOP_Object *data;  // IIOP object 
  CORBA_BOA_ptr oa = TAO_OA_PARAMS::instance()->oa(); // underlying BOA
  this->optable_ = &tao_Marshal_DSI_optable;     // operation database
  CORBA_Long i;

  // setup an IIOP object
  data = new IIOP_Object (CORBA_string_dup (repoID));
  data->profile.iiop_version.major = IIOP::MY_MAJOR;
  data->profile.iiop_version.minor = IIOP::MY_MINOR;
  data->profile.host = ACE_OS::strdup (oa->get_addr ().get_host_name ());
  data->profile.port = oa->get_addr ().get_port_number ();
  data->profile.object_key.length = ACE_OS::strlen (obj_name);
  data->profile.object_key.maximum = data->profile.object_key.length;
  data->profile.object_key.buffer = new CORBA_Octet [(size_t)data->profile.object_key.length+1];
  ACE_OS::memcpy (data->profile.object_key.buffer, obj_name,
		  data->profile.object_key.length); // set the object key
  this->set_parent (data);  // store the IIOP obj reference with ourselves
  this->sub_ = this;               // set the most derived class to be ourselves
  if (oa) oa->bind (data->profile.object_key, this);// register ourselves
}

void
Marshal_DSI_Impl::invoke (CORBA_ServerRequest& req, CORBA_Environment &env)
{
  // parse the incoming request and find out for what operation it is. We use a
  // simple linear search here
  if (!ACE_OS::strcmp (req.op_name (), "test_short"))
    {
    }
  else if (!ACE_OS::strcmp (req.op_name (), "test_long"))
    {
    }
  else if (!ACE_OS::strcmp (req.op_name (), "test_octet"))
    {
    }
  else if (!ACE_OS::strcmp (req.op_name (), "test_char"))
    {
    }
  else if (!ACE_OS::strcmp (req.op_name (), "test_double"))
    {
    }
  else if (!ACE_OS::strcmp (req.op_name (), "test_struct"))
    {
    }
  else if (!ACE_OS::strcmp (req.op_name (), "test_union"))
    {
    }
  else if (!ACE_OS::strcmp (req.op_name (), "test_any"))
    {
    }
  else if (!ACE_OS::strcmp (req.op_name (), "test_sequence"))
    {
    }
}

// simulate the skeletons
void Marshal_DSI_Impl::_Marshal_is_a_skel(CORBA_ServerRequest &req, 
						CORBA_Object_ptr obj, 
						CORBA_Environment &env)
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

void Marshal_DSI_Impl::_Marshal_test_short_skel(CORBA_ServerRequest &req, 
						CORBA_Object_ptr obj, 
						CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  CORBA_NamedValue_ptr    nv1, nv2;
  CORBA_Any               any_s1 (_tc_CORBA_Short);
  CORBA_Any               any_s2 (_tc_CORBA_Short);
  Marshal_DSI_Impl*       impl;
  CORBA_Short s1, s2;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);
  nv1 = nvlist->add_value (0, any_s1, CORBA_ARG_IN, env);
  nv2 = nvlist->add_value (0, any_s2, CORBA_ARG_OUT, env);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  s1 = *(CORBA_Short *) nv1->value ()->value ();

  // now invoke the implementation
  impl = (Marshal_DSI_Impl *)(obj->get_subclass ());
  // now the magic of dynamic binding
  impl->test_short (s1, s2, env);

  nv2->value ()->replace (_tc_CORBA_Short, &s2, 0, env);

  // result - NO result
  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, 1);
  req.result(any, env);
}

void Marshal_DSI_Impl::_Marshal_test_long_skel(CORBA_ServerRequest &req, 
						CORBA_Object_ptr obj, 
						CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  CORBA_NamedValue_ptr    nv1, nv2;
  CORBA_Any               any_l1 (_tc_CORBA_Long);
  CORBA_Any               any_l2 (_tc_CORBA_Long);
  Marshal_DSI_Impl*       impl;
  CORBA_Long l1, l2;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);
  nv1 = nvlist->add_value (0, any_l1, CORBA_ARG_IN, env);
  nv2 = nvlist->add_value (0, any_l2, CORBA_ARG_OUT, env);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  l1 = *(CORBA_Long *) nv1->value ()->value ();

  // now invoke the implementation
  impl = (Marshal_DSI_Impl *)(obj->get_subclass ());
  // now the magic of dynamic binding
  impl->test_long (l1, l2, env);

  nv2->value ()->replace (_tc_CORBA_Long, &l2, 0, env);

  // result - NO result
  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, 1);
  req.result(any, env);
}

void Marshal_DSI_Impl::_Marshal_test_octet_skel(CORBA_ServerRequest &req, 
						CORBA_Object_ptr obj, 
						CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  CORBA_NamedValue_ptr    nv1, nv2;
  CORBA_Any               any_o1 (_tc_CORBA_Octet);
  CORBA_Any               any_o2 (_tc_CORBA_Octet);
  Marshal_DSI_Impl*       impl;
  CORBA_Octet o1, o2;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);
  nv1 = nvlist->add_value (0, any_o1, CORBA_ARG_IN, env);
  nv2 = nvlist->add_value (0, any_o2, CORBA_ARG_OUT, env);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  o1 = *(CORBA_Octet *) nv1->value ()->value ();

  // now invoke the implementation
  impl = (Marshal_DSI_Impl *)(obj->get_subclass ());
  // now the magic of dynamic binding
  impl->test_octet (o1, o2, env);

  nv2->value ()->replace (_tc_CORBA_Octet, &o2, 0, env);

  // result - NO result
  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, 1);
  req.result(any, env);
}

void Marshal_DSI_Impl::_Marshal_test_char_skel(CORBA_ServerRequest &req, 
					       CORBA_Object_ptr obj, 
					       CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  CORBA_NamedValue_ptr    nv1, nv2;
  CORBA_Any               any_c1 (_tc_CORBA_Char);
  CORBA_Any               any_c2 (_tc_CORBA_Char);
  Marshal_DSI_Impl*       impl;
  CORBA_Char c1, c2;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);
  nv1 = nvlist->add_value (0, any_c1, CORBA_ARG_IN, env);
  nv2 = nvlist->add_value (0, any_c2, CORBA_ARG_OUT, env);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  c1 = *(CORBA_Char *) nv1->value ()->value ();

  // now invoke the implementation
  impl = (Marshal_DSI_Impl *)(obj->get_subclass ());
  // now the magic of dynamic binding
  impl->test_char (c1, c2, env);

  nv2->value ()->replace (_tc_CORBA_Char, &c2, 0, env);

  // result - NO result
  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, 1);
  req.result(any, env);
}

void Marshal_DSI_Impl::_Marshal_test_double_skel(CORBA_ServerRequest &req, 
						CORBA_Object_ptr obj, 
						CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  CORBA_NamedValue_ptr    nv1, nv2;
  CORBA_Any               any_d1 (_tc_CORBA_Double);
  CORBA_Any               any_d2 (_tc_CORBA_Double);
  Marshal_DSI_Impl*       impl;
  CORBA_Double d1, d2;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);
  nv1 = nvlist->add_value (0, any_d1, CORBA_ARG_IN, env);
  nv2 = nvlist->add_value (0, any_d2, CORBA_ARG_OUT, env);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  d1 = *(CORBA_Double *) nv1->value ()->value ();

  // now invoke the implementation
  impl = (Marshal_DSI_Impl *)(obj->get_subclass ());
  // now the magic of dynamic binding
  impl->test_double (d1, d2, env);

  nv2->value ()->replace (_tc_CORBA_Double, &d2, 0, env);

  // result - NO result
  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, 1);
  req.result(any, env);
}

void Marshal_DSI_Impl::_Marshal_test_struct_skel(CORBA_ServerRequest &req, 
						CORBA_Object_ptr obj, 
						CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  CORBA_NamedValue_ptr    nv1, nv2;
  CORBA_Any               any_ms1 (Marshal::_tc_Marshal_Struct);
  CORBA_Any               any_ms2 (Marshal::_tc_Marshal_Struct);
  Marshal_DSI_Impl*       impl;
  Marshal::Marshal_Struct ms1, *ms2;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);
  nv1 = nvlist->add_value (0, any_ms1, CORBA_ARG_IN, env);
  nv2 = nvlist->add_value (0, any_ms2, CORBA_ARG_OUT, env);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  // note that the second parameter being an out parameter, we are allocating
  // it from heap and filling it up. We will then insert it into the Any that
  // will get written to the stream. We should have done this for the
  // primitives too, but somehow they worked (because nothing got written onto
  // the stack frame onto which they existed, but this phenomenon was occuring
  // here and possibly will occur for the rest of the case
  ms1 = *(Marshal::Marshal_Struct *) nv1->value ()->value ();
  ms2 = new Marshal::Marshal_Struct;

  // now invoke the implementation
  impl = (Marshal_DSI_Impl *)(obj->get_subclass ());
  // now the magic of dynamic binding
  impl->test_struct (ms1, *ms2, env);

  nv2->value ()->replace (Marshal::_tc_Marshal_Struct, ms2, 1, env);

  // result - NO result
  //  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, 1);
  //  req.result(any, env);
}

void Marshal_DSI_Impl::_Marshal_test_union_skel(CORBA_ServerRequest &req, 
						CORBA_Object_ptr obj, 
						CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  CORBA_NamedValue_ptr    nv1, nv2;
  CORBA_Any               any_u1 (Marshal::_tc_Marshal_Union);
  CORBA_Any               any_u2 (Marshal::_tc_Marshal_Union);
  Marshal_DSI_Impl*       impl;
  Marshal::Marshal_Union u1, *u2;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);
  nv1 = nvlist->add_value (0, any_u1, CORBA_ARG_IN, env);
  nv2 = nvlist->add_value (0, any_u2, CORBA_ARG_OUT, env);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  u1 = *(Marshal::Marshal_Union *) nv1->value ()->value ();
  u2 = new Marshal::Marshal_Union;

  // now invoke the implementation
  impl = (Marshal_DSI_Impl *)(obj->get_subclass ());
  // now the magic of dynamic binding
  impl->test_union (u1, *u2, env);

  nv2->value ()->replace (Marshal::_tc_Marshal_Union, u2, 0, env);

  // result - NO result
  //  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, 1);
  //  req.result(any, env);
}

void Marshal_DSI_Impl::_Marshal_test_any_skel(CORBA_ServerRequest &req, 
					      CORBA_Object_ptr obj, 
					      CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  CORBA_NamedValue_ptr    nv1, nv2;
  CORBA_Any *a1=0, *a2=0;
  CORBA_Any               any_a1 (_tc_CORBA_Any, &a1);
  CORBA_Any               any_a2 (_tc_CORBA_Any);
  Marshal_DSI_Impl*       impl;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);
  nv1 = nvlist->add_value (0, any_a1, CORBA_ARG_IN, env);
  nv2 = nvlist->add_value (0, any_a2, CORBA_ARG_OUT, env);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  a1 = (CORBA_Any *) nv1->value ()->value ();

  // now invoke the implementation
  impl = (Marshal_DSI_Impl *)(obj->get_subclass ());
  // now the magic of dynamic binding
  impl->test_any (*a1, a2, env);

  nv2->value ()->replace (_tc_CORBA_Any, a2, 1, env);

  // result - NO result
  //  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, 1);
  //  req.result(any, env);
}

void Marshal_DSI_Impl::_Marshal_test_sequence_skel(CORBA_ServerRequest &req, 
						   CORBA_Object_ptr obj, 
						   CORBA_Environment &env)
{
  CORBA_NVList_ptr        nvlist;
  CORBA_NamedValue_ptr    nv1, nv2;
  CORBA_Any               any_as1 (Marshal::_tc_AnySeq);
  CORBA_Any               any_as2 (Marshal::_tc_AnySeq);
  Marshal_DSI_Impl*       impl;
  Marshal::AnySeq  *as1, *as2;

  // now create a request and populate it with the typecodes
  req.orb()->create_list (0, nvlist);
  nv1 = nvlist->add_value (0, any_as1, CORBA_ARG_IN, env);
  nv2 = nvlist->add_value (0, any_as2, CORBA_ARG_OUT, env);

  // retrieve params - call the interpreter to interpret the parameters for us.
  req.params(nvlist, env);

  as1 = (Marshal::AnySeq *) nv1->value ()->value ();

  // now invoke the implementation
  impl = (Marshal_DSI_Impl *)(obj->get_subclass ());
  // now the magic of dynamic binding
  impl->test_sequence (*as1, as2, env);

  nv2->value ()->replace (Marshal::_tc_AnySeq, as2, 1, env);

  // result - NO result
  //  CORBA_Any *any = new CORBA_Any(_tc_CORBA_Void, 0, 1);
  //  req.result(any, env);
}

// private methods of our DSI class
void
Marshal_DSI_Impl::test_short (CORBA_Short s1,
			      CORBA_Short &s2,
			      CORBA_Environment &env)
{
  s2 = 2*s1;
}

void
Marshal_DSI_Impl::test_long (CORBA_Long l1,
			     CORBA_Long &l2,
			     CORBA_Environment &env)
{
  l2 = 3*l1;
}

void
Marshal_DSI_Impl::test_octet (CORBA_Octet o1,
			      CORBA_Octet &o2,
			      CORBA_Environment &env)
{
  o2 = o1;
}

void
Marshal_DSI_Impl::test_char (CORBA_Char c1,
			     CORBA_Char &c2,
			     CORBA_Environment &env)
{
  c2 = c1;
}

void
Marshal_DSI_Impl::test_double (CORBA_Double d1,
			       CORBA_Double &d2,
			       CORBA_Environment &env)
{
  d2 = d1/2;
}

void
Marshal_DSI_Impl::test_struct (const Marshal::Marshal_Struct& ms1,
			       Marshal::Marshal_Struct& ms2,
			       CORBA_Environment &env)
{
  ms2.s = ms1.s;
  ms2.l = ms1.l;
  ms2.c = ms1.c;
  ms2.o = ms1.o;
  ms2.d = ms1.d;
}

void
Marshal_DSI_Impl::test_union (const Marshal::Marshal_Union& u1,
			      Marshal::Marshal_Union& u2,
			      CORBA_Environment &env)
{
  u2._d (u1._d ()); // set the discriminant value

  switch (u1._d ())
    {
    case Marshal::e_0th:
      u2.s (u1.s ()); // set short
      break;
    case Marshal::e_1st:
      u2.l (u1.l ()); // set long
      break;
    case Marshal::e_2nd:
      u2.c (u1.c ()); // set char
      break;
    case Marshal::e_3rd:
      u2.o (u1.o ()); // set octet
      break;
    case Marshal::e_4th:
      u2.d (u1.d ()); // set double
      break;
    case Marshal::e_5th:
    default:
      u2.ms (u1. ms ()); // set structs
      break;
    }
}

void
Marshal_DSI_Impl::test_any (const CORBA_Any &a1,
			    CORBA_Any *&a2,
			    CORBA_Environment &env)
{
  a2 = new CORBA_Any (a1.type (), (void *)a1.value ()); // will do a deep copy
}

void 
Marshal_DSI_Impl::test_sequence (const Marshal::AnySeq& as1,
				 Marshal::AnySeq *& as2,
				 CORBA_Environment &env)
{
  as2 = new Marshal::AnySeq (as1);
}

#if 0
void 
Marshal_DSI_Impl::test_recursive (const Marshal::Marshal_Recursive &mr1,
				  Marshal::Marshal_Recursive *&mr2,
				  CORBA_Environment &env)
{
}
#endif

