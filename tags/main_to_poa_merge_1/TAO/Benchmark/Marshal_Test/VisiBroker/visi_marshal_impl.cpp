#include "visi_marshal_impl.h"

#if !defined (__ACE_INLINE__)
#include "visi_marshal_impl.i"
#endif /* __ACE_INLINE__ */

// All the methods of the SSI class
void
Marshal_SSI_Impl::test_short (CORBA::Short s1,
			      CORBA::Short &s2)
{
  s2 = s1;
}

void
Marshal_SSI_Impl::test_long (CORBA::Long l1,
			     CORBA::Long &l2)
{
  l2 = l1;
}

void
Marshal_SSI_Impl::test_octet (CORBA::Octet o1,
			      CORBA::Octet &o2)
{
  o2 = o1;
}

void
Marshal_SSI_Impl::test_char (CORBA::Char c1,
			     CORBA::Char &c2)
{
  c2 = c1;
}

void
Marshal_SSI_Impl::test_double (CORBA::Double d1,
			       CORBA::Double &d2)
{
  d2 = d1;
}

void
Marshal_SSI_Impl::test_struct (const Marshal::Marshal_Struct& ms1,
			       Marshal::Marshal_Struct& ms2)
{
  ms2.s = ms1.s;
  ms2.l = ms1.l;
  ms2.c = ms1.c;
  ms2.o = ms1.o;
  ms2.d = ms1.d;
}

void
Marshal_SSI_Impl::test_union (const Marshal::Marshal_Union& u1,
			      Marshal::Marshal_Union& u2)
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
Marshal_SSI_Impl::test_any (const CORBA::Any &a1,
			    CORBA::Any *&a2)
{
  a2 = new CORBA::Any (a1.type (), (void *)a1.value ()); // will do a deep copy
}

void 
Marshal_SSI_Impl::test_sequence (const Marshal::AnySeq& as1,
				 Marshal::AnySeq *& as2)
{
  as2 = new Marshal::AnySeq (as1);
}

void 
Marshal_SSI_Impl::test_recursive (const Marshal::Marshal_Recursive &mr1,
				  Marshal::Marshal_Recursive *&mr2)
{
}

//----------------------------------------------------------------------------
//the methods of the DSI implementation class

void
Marshal_DSI_Impl::invoke (CORBA::ServerRequest_ptr req)
{
  // parse the incoming request and find out for what operation it is. We use a
  // simple linear search here
  if (!ACE_OS::strcmp (req->op_name (), "test_short"))
    {
      test_short_skel (req);
    }
  else if (!ACE_OS::strcmp (req->op_name (), "test_long"))
    {
    }
  else if (!ACE_OS::strcmp (req->op_name (), "test_octet"))
    {
    }
  else if (!ACE_OS::strcmp (req->op_name (), "test_char"))
    {
    }
  else if (!ACE_OS::strcmp (req->op_name (), "test_double"))
    {
    }
  else if (!ACE_OS::strcmp (req->op_name (), "test_struct"))
    {
    }
  else if (!ACE_OS::strcmp (req->op_name (), "test_union"))
    {
    }
  else if (!ACE_OS::strcmp (req->op_name (), "test_any"))
    {
    }
  else if (!ACE_OS::strcmp (req->op_name (), "test_sequence"))
    {
    }
}

void
Marshal_DSI_Impl::test_short (CORBA::Short s1,
			      CORBA::Short &s2)
{
  s2 = 5*s1;
}

void
Marshal_DSI_Impl::test_long (CORBA::Long l1,
			     CORBA::Long &l2)
{
  l2 = l1;
}

void
Marshal_DSI_Impl::test_octet (CORBA::Octet o1,
			      CORBA::Octet &o2)
{
  o2 = o1;
}

void
Marshal_DSI_Impl::test_char (CORBA::Char c1,
			     CORBA::Char &c2)
{
  c2 = c1;
}

void
Marshal_DSI_Impl::test_double (CORBA::Double d1,
			       CORBA::Double &d2)
{
  d2 = d1;
}

void
Marshal_DSI_Impl::test_struct (const Marshal::Marshal_Struct& ms1,
			       Marshal::Marshal_Struct& ms2)
{
  ms2.s = ms1.s;
  ms2.l = ms1.l;
  ms2.c = ms1.c;
  ms2.o = ms1.o;
  ms2.d = ms1.d;
}

void
Marshal_DSI_Impl::test_union (const Marshal::Marshal_Union& u1,
			      Marshal::Marshal_Union& u2)
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
Marshal_DSI_Impl::test_any (const CORBA::Any &a1,
			    CORBA::Any *&a2)
{
  a2 = new CORBA::Any (a1.type (), (void *)a1.value ()); // will do a deep copy
}

void 
Marshal_DSI_Impl::test_sequence (const Marshal::AnySeq& as1,
				 Marshal::AnySeq *& as2)
{
  as2 = new Marshal::AnySeq (as1);
}

void 
Marshal_DSI_Impl::test_recursive (const Marshal::Marshal_Recursive &mr1,
				  Marshal::Marshal_Recursive *&mr2)
{
}

// all the helper functions for the DSI class
void 
Marshal_DSI_Impl::test_short_skel (CORBA::ServerRequest_ptr req)
{
  CORBA::NVList_var short_nv_;
  CORBA::Any *any_s1, *any_s2;
  CORBA::Short s1=0, 
    *s2=0;

  orb_->create_list (0, short_nv_);

  ACE_DEBUG ((LM_DEBUG, "short_nv_ created with %d nodes\n", short_nv_->count ()));

  any_s1 = new CORBA::Any (CORBA::_tc_short, &s1, 0);
  s2 = new CORBA::Short;
  any_s2 = new CORBA::Any (CORBA::_tc_short, s2, 1);

  short_nv_->add_value ("s1", *any_s1, CORBA::ARG_IN);
  // if I do the next line, I get CORBA::Marshal exception from
  // req->params. I don't know why
  short_nv_->add_value ("s2", *any_s2, CORBA::ARG_OUT);
  // However, the following line works. I think I know what is the
  // problem. Since the 2nd argument is a ARG_OUT, we cannot add_value. We must
  // just tell it what type to expect and leave it to the ORB to allocate the
  // memory 
  //short_nv_->add_item ("s2", CORBA::ARG_OUT);
  // My explanation is confirmed if I use the following in which case I get a
  // marshaling exception. Thus, for OUT args, only add_item works.
  // *(short_nv_->add (CORBA::ARG_OUT)->value ()) <<= s2;

  ACE_DEBUG ((LM_DEBUG, "short_nv_ created with %d nodes\n", short_nv_->count ()));

  try {
    req->params (short_nv_);
  }
  catch (CORBA::SystemException &se)
    {
      cerr << "System Exception doing params: " << se << endl;
    }
  cout << "after params" << endl;
  *(short_nv_->item (0)->value ()) >>= s1;
  ACE_DEBUG ((LM_DEBUG, "s1 = %d\n", s1));
  test_short (s1, *s2);
  //  *(short_nv_->item (1)->value ()) <<= s2;
  ACE_DEBUG ((LM_DEBUG, "s1 = %d, s2 = %d\n", s1, *s2));
  ACE_DEBUG ((LM_DEBUG, "short_nv_ after the call has %d nodes\n", short_nv_->count ()));
}

void 
Marshal_DSI_Impl::test_long_skel (CORBA::ServerRequest_ptr req)
{
}

void 
Marshal_DSI_Impl::test_octet_skel (CORBA::ServerRequest_ptr req)
{
}

void 
Marshal_DSI_Impl::test_char_skel (CORBA::ServerRequest_ptr req)
{
}

void 
Marshal_DSI_Impl::test_double_skel (CORBA::ServerRequest_ptr req)
{
}

void 
Marshal_DSI_Impl::test_struct_skel (CORBA::ServerRequest_ptr req)
{
}

void 
Marshal_DSI_Impl::test_union_skel (CORBA::ServerRequest_ptr req)
{
}

void 
Marshal_DSI_Impl::test_any_skel (CORBA::ServerRequest_ptr req)
{
}

void 
Marshal_DSI_Impl::test_sequence_skel (CORBA::ServerRequest_ptr req)
{
}

void 
Marshal_DSI_Impl::test_recursive_skel (CORBA::ServerRequest_ptr req)
{
}
