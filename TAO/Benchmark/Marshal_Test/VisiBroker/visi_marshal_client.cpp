// ============================================================================
//
// = LIBRARY
//    CORBA Visibroker marshal benchmark
// 
// = FILENAME
//    visi_marshal_proxy.cpp
//
// = AUTHOR
//    Aniruddha Gokhale
// 
// ============================================================================

#include "visi_marshal_client.h"

#if !defined (__ACE_INLINE__)
#include "visi_marshal_client.i"
#endif /* __ACE_INLINE__ */

int
Visi_Marshal_Client_Proxy::run (CORBA_Marshal_Proxy::OPTIONS &options,
				CORBA_Marshal_Proxy::RESULTS &results)
{
  char **argv;
  int argc;
  CORBA::Object_ptr target;

  // get the argument list to be passed on to the ORB_init method
  argv = options.orb_init_args (argc);

  // get the orb. Pass any ORB_init arguments
  this->orb_ = CORBA::ORB_init (argc, (char *const *)argv);
  if (this->orb_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "ORB_init failed\n"));
      return -1;
    }

  // get the obj ref by binding it to the specified host using the specified obj_name
  target = Marshal::_bind (options.object_name (), options.hostname ());
  if (CORBA::is_nil (target))
    {
      ACE_DEBUG ((LM_DEBUG, "bind call failed\n"));
      return -1;
    }

  this->ref_ = Marshal::_narrow (target);
  if (CORBA::is_nil (this->ref_))
    {
      ACE_DEBUG ((LM_DEBUG, "narrow to Marshal failed\n"));
      return -1;
    }

  // tell the "results" object what file it must store the results into
  results.filename (options.filename ());

  // use SII or DII accordingly
  switch (options.policy ())
    {
    case CORBA_Marshal_Options::STATIC : // use SII
      return this->use_sii (options, results);
      break;
    case CORBA_Marshal_Options::DYNAMIC : // use DII
      return this->use_dii (options, results);
      break;
    default:
      ACE_DEBUG ((LM_DEBUG, "bad policy\n"));
      return -1;
    }
  // hopefully we are never here, else something is seriously wrong
  return -1;
}

int
Visi_Marshal_Client_Proxy::use_sii (CORBA_Marshal_Proxy::OPTIONS &options,
				    CORBA_Marshal_Proxy::RESULTS &results)
{
  int i;
  int status = 0;
  // This method tests the static invocation interface

  for (i=0; i < options.iterations () && !status; i++)
    {
      switch (options.data_type ())
	{
	  // shorts
	case CORBA_Marshal_Options::SHORT:
	  status = sii_test_short (options, results);
	  break;
	  // longs
	case CORBA_Marshal_Options::LONG:
	  status = sii_test_long (options, results);
	  break;
	  // octets
	case CORBA_Marshal_Options::OCTET:
	  status = sii_test_octet (options, results);
	  break;
	  // chars
	case CORBA_Marshal_Options::CHAR:
	  status = sii_test_char (options, results);
	  break;
	  // doubles
	case CORBA_Marshal_Options::DOUBLE:
	  status = sii_test_double (options, results);
	  break;
	  // Structs
	case CORBA_Marshal_Options::STRUCT:
	  status = sii_test_struct (options, results);
	  break;
	  // unions
	case CORBA_Marshal_Options::UNION:
	  status = sii_test_union (options, results);
	  break;
	  // anys
	case CORBA_Marshal_Options::ANY:
	  status = sii_test_any (options, results);
	  break;
	  // sequences
	case CORBA_Marshal_Options::SEQUENCE:
	  status = sii_test_sequence (options, results);
	  break;
	  // longs
	case CORBA_Marshal_Options::RECURSIVE:
	  //	  status = sii_test_recursive (options, results);
	  // doesn't work
	  break;
	default:
	  status = -1;
	} // end of switch
    } // for loop

  if (!status)
    return 0;
  else
    return -1;
}

int
Visi_Marshal_Client_Proxy::use_dii (CORBA_Marshal_Proxy::OPTIONS &options,
				    CORBA_Marshal_Proxy::RESULTS &results)
{
  int i;
  int status = 0;
  // This method tests the dynamic invocation interface

  for (i=0; i < options.iterations () && !status; i++)
    {
      switch (options.data_type ())
	{
	  // shorts
	case CORBA_Marshal_Options::SHORT:
	  status = dii_test_short (options, results);
	  break;
	  // longs
	case CORBA_Marshal_Options::LONG:
	  status = dii_test_long (options, results);
	  break;
	  // octets
	case CORBA_Marshal_Options::OCTET:
	  status = dii_test_octet (options, results);
	  break;
	  // chars
	case CORBA_Marshal_Options::CHAR:
	  status = dii_test_char (options, results);
	  break;
	  // doubles
	case CORBA_Marshal_Options::DOUBLE:
	  status = dii_test_double (options, results);
	  break;
	  // Structs
	case CORBA_Marshal_Options::STRUCT:
	  status = dii_test_struct (options, results);
	  break;
	  // unions
	case CORBA_Marshal_Options::UNION:
	  status = dii_test_union (options, results);
	  break;
	  // anys
	case CORBA_Marshal_Options::ANY:
	  status = dii_test_any (options, results);
	  break;
	  // sequences
	case CORBA_Marshal_Options::SEQUENCE:
	  status = dii_test_sequence (options, results);
	  break;
	  // longs
	case CORBA_Marshal_Options::RECURSIVE:
	  //	  status = dii_test_recursive (options, results);
	  // doesn't work
	  break;
	default:
	  status = -1;
	} // end of switch
    } // for loop

  if (!status)
    return 0;
  else
    return -1;
}


// all helper functions : SII
int
Visi_Marshal_Client_Proxy::sii_test_short (CORBA_Marshal_Proxy::OPTIONS &options,
					   CORBA_Marshal_Proxy::RESULTS &results)
{
  CORBA::Short s1, s2;
  s1 = 3;
  ref_->test_short (s1, s2);
  if (s1 == s2)
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_short success: s1 = %d, s2 = %d\n", s1, s2));
      return 0;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_short failed: s1 = %d, s2 = %d\n", s1, s2));
      return -1;
    }
}

int
Visi_Marshal_Client_Proxy::sii_test_long (CORBA_Marshal_Proxy::OPTIONS &options,
					  CORBA_Marshal_Proxy::RESULTS &results)
{
  CORBA::Long l1, l2;
  l1 = 256;
  ref_->test_long (l1, l2);
  if (l1 == l2)
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_long success: l1 = %d, l2 = %d\n", l1, l2));
      return 0;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_long failed: l1 = %d, l2 = %d\n", l1, l2));
      return -1;
    }
}

int
Visi_Marshal_Client_Proxy::sii_test_octet (CORBA_Marshal_Proxy::OPTIONS &options,
					   CORBA_Marshal_Proxy::RESULTS &results)
  // octets
{
  CORBA::Octet o1, o2;
  o1 = (CORBA::Octet) 127;
  ref_->test_octet (o1, o2);
  if (o1 == o2)
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_octet success: o1 = %c, o2 = %c\n", o1, o2));
      return 0;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_octet failed: o1 = %c, o2 = %c\n", o1, o2));
      return -1;
    }
}

int
Visi_Marshal_Client_Proxy::sii_test_char (CORBA_Marshal_Proxy::OPTIONS &options,
					  CORBA_Marshal_Proxy::RESULTS &results)
  // chars
{
  CORBA::Char c1, c2;
  c1 = 'B';
  ref_->test_char (c1, c2);
  if (c1 == c2)
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_char success: c1 = %c, c2 = %c\n", c1, c2));
      return 0;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_char failed: c1 = %c, c2 = %c\n", c1, c2));
      return -1;
    }
}

int
Visi_Marshal_Client_Proxy::sii_test_double (CORBA_Marshal_Proxy::OPTIONS &options,
					    CORBA_Marshal_Proxy::RESULTS &results)
  // doubles
{
  CORBA::Double d1, d2;
  d1 = 3.1415;
  ref_->test_double (d1, d2);
  if (d1 == d2)
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_double success: d1 = %f, d2 = %f\n", d1, d2));
      return 0;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_double failed: d1 = %f, d2 = %f\n", d1, d2));
      return -1;
    }
}

int
Visi_Marshal_Client_Proxy::sii_test_struct (CORBA_Marshal_Proxy::OPTIONS &options,
					    CORBA_Marshal_Proxy::RESULTS &results)
  // structs
{
  Marshal::Marshal_Struct ms1, ms2;

  ms1.s = 3;
  ms1.l = 256;
  ms1.c = 'B';
  ms1.o = (CORBA::Octet) 127;
  ms1.d = 3.1415;

  ref_->test_struct (ms1, ms2);
  if (ms1.s == ms2.s && ms1.l == ms2.l && ms1.c == ms2.c && ms1.o == ms2.o &&
      ms1.d == ms2.d)
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_struct success\n"));
      return 0;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_struct failed\n"));
      return -1;
    }
}

int
Visi_Marshal_Client_Proxy::sii_test_union (CORBA_Marshal_Proxy::OPTIONS &options,
					   CORBA_Marshal_Proxy::RESULTS &results)
  // unions
{
  Marshal::Marshal_Union u1, u2;
  Marshal::Marshal_Struct ms1, ms2;


  // first test an enumerated case
  u1._d (Marshal::e_1st);
  u1.l (4567);

  ref_->test_union (u1, u2);
  if ( (u1._d () == u2._d()) && (u1.l () == u2.l ()))
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_union for long (case e_1st) success\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_union for long (case e_1st) failed\n"));
      return -1;
    }

  // now test the default case
  u1._d (Marshal::e_6th); // this case doesn't exist which will lead to using
			  // the default case

  u1.ms ().s = 3;
  u1.ms ().l = 256;
  u1.ms ().c = 'B';
  u1.ms ().o = (CORBA::Octet) 127;
  u1.ms ().d = 3.1415;

  ref_->test_union (u1, u2);

  if (u1.ms ().s == u2.ms ().s && u1.ms ().l == u2.ms ().l && u1.ms ().c == u2.ms ().c && u1.ms ().o == u2.ms ().o &&
      u1.ms ().d == u2.ms ().d)
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_union for default (case e_6th) success\n"));
      return 0;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_union for default (case e_6th) failed\n"));
      return -1;
    }
}

int
Visi_Marshal_Client_Proxy::sii_test_any (CORBA_Marshal_Proxy::OPTIONS &options,
					 CORBA_Marshal_Proxy::RESULTS &results)
{
  // we send a struct thru the any
  CORBA::Any a1, *a2;
  Marshal::Marshal_Struct ms1, *ms2;

  ms1.s = 3;
  ms1.l = 256;
  ms1.c = 'B';
  ms1.o = (CORBA::Octet) 127;
  ms1.d = 3.1415;

  // populate the any with a MarshalStruct
  a1 <<= ms1;

  a2 = new CORBA::Any;

  // now test it
  ref_->test_any (a1, a2);
  
  // check if the two typecodes are equal
  if (a2->type ()->equal (Marshal::_tc_Marshal_Struct))
    {
      // now see if the values are same
      ms2 = (Marshal::Marshal_Struct *)a2->value ();
      if (ms1.s == ms2->s && ms1.l == ms2->l && ms1.c == ms2->c && ms1.o == ms2->o &&
	  ms1.d == ms2->d)
	{
	  ACE_DEBUG ((LM_DEBUG, "SII test_any success\n"));
	  return 0;
	}
      else
	{
	  ACE_DEBUG ((LM_DEBUG, "SII test_any value match failed\n"));
	  return -1;
	}
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_any failed due to typecode mismatch\n"));
    }
  return 0;
}

int
Visi_Marshal_Client_Proxy::sii_test_sequence (CORBA_Marshal_Proxy::OPTIONS &options,
					      CORBA_Marshal_Proxy::RESULTS &results)
{
  // make a sequence of Anys and fill each one with a different data type
  Marshal::AnySeq as1 (7), *as2;
  Marshal::Marshal_Struct ms;
  Marshal::Marshal_Union u;

  ms.s = 3;
  ms.l = 256;
  ms.c = 'B';
  ms.o = (CORBA::Octet) 127;
  ms.d = 3.1415;

  u._d (Marshal::e_5th); // somehow, the default case e_6th doesn't seem to
			 // work here
  u.ms ().s = 3;
  u.ms ().l = 256;
  u.ms ().c = 'B';
  u.ms ().o = (CORBA::Octet) 127;
  u.ms ().d = 3.1415;

  // it looks like we must tell the sequence how many elements it is going to
  // carry. By just fixing the max value does not work.

  as1.length (7);

  as1[0] <<= (CORBA::Short)3;
  as1[1] <<= (CORBA::Long) 256;
  as1[2] <<= CORBA::Any::from_octet ((CORBA::Octet)127);
  as1[3] <<= CORBA::Any::from_char ('B');
  as1[4] <<= (CORBA::Double)3.1415; // double
  as1[5] <<= ms;     // struct
  as1[6] <<= u;      // union

  as2 = new Marshal::AnySeq;

  ref_->test_sequence (as1, as2);

#if 0
  // the following is extremely weird. I do not understand why it is not the
  // same as _tc_Marshal_Union, but its _kind field does have _tk_union in it.
  if ((*as2)[6].type ()->equal (Marshal::_tc_Marshal_Union))
    {
      cerr << "6th elem has union typecode" << endl;
    }
  else
    {
      cerr << "6th elem doesnot have union typecode" << endl;
      cerr << "its typecode kind = " << (*as2)[6].type ()->kind () << endl;
    }
#endif

  ACE_DEBUG ((LM_DEBUG, "SII test_sequence\n"));
  if (as1.length () == as2->length ())
    {
      // now make sure that the elements are same
      for (int i=0; i < as1.length (); i++)
	{
#if 0
	  if (as1[i].type ()->equal ((*as2)[i].type ()))
	    {
#endif
	      // now see if the values are same
	      switch (i)
		{
		case 0:
		  if (*(CORBA::Short *)as1[i].value () == *(CORBA::Short
							    *)(*as2)[i].value ())
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tSII test_sequence, shorts matched\n"));
		    }
		  else
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tSII test_sequence failed on shorts match\n"));
		    }
		  break;
		case 1:
		  if (*(CORBA::Long *)as1[i].value () == *(CORBA::Long
							    *)(*as2)[i].value ())
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tSII test_sequence, longs matched\n"));
		    }
		  else
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tSII test_sequence failed on longs match\n"));
		    }
		  break;
		case 2:
		  if (*(CORBA::Octet *)as1[i].value () == *(CORBA::Octet
							    *)(*as2)[i].value ())
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tSII test_sequence, octets matched\n"));
		    }
		  else
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tSII test_sequence failed on octets match\n"));
		    }
		  break;
		case 3:
		  if (*(CORBA::Char *)as1[i].value () == *(CORBA::Char
							    *)(*as2)[i].value ())
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tSII test_sequence, chars matched\n"));
		    }
		  else
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tSII test_sequence failed on chars match\n"));
		    }
		  break;
		case 4:
		  if (*(CORBA::Double *)as1[i].value () == *(CORBA::Double
							    *)(*as2)[i].value ())
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tSII test_sequence, doubles matched\n"));
		    }
		  else
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tSII test_sequence failed on doubles match\n"));
		    }
		  break;
		case 5:
		  {
		    Marshal::Marshal_Struct *ms1, *ms2;
		    
		    ms1 = (Marshal::Marshal_Struct *)as1[i].value ();
		    ms2 = (Marshal::Marshal_Struct *)(*as2)[i].value ();

		    if (ms1->s == ms2->s && ms1->l == ms2->l && ms1->c == ms2->c && ms1->o == ms2->o &&
			ms1->d == ms2->d)
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tSII test_sequence, structs matched\n"));
		    }
		  else
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tSII test_sequence failed on structs match\n"));
		    }
		  }
		  break;
		case 6:
		  {
		    Marshal::Marshal_Union *u1, *u2;
		    
		    u1 = (Marshal::Marshal_Union *)as1[i].value ();
		    u2 = (Marshal::Marshal_Union *)(*as2)[i].value ();

		    if (u1->_d () == u2->_d () && u1->ms ().s == u2->ms ().s &&
			u1->ms ().l == u2->ms ().l && 
			u1->ms ().c == u2->ms ().c && u1->ms ().o == u2->ms ().o &&
			u1->ms ().d == u2->ms ().d)
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tSII test_sequence, unions matched\n"));
		    }
		  else
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tSII test_sequence failed on unions match\n"));
		    }
		  }
		  break;
		}
#if 0
	    }
	  else
	    {
	      ACE_DEBUG ((LM_DEBUG, 
			  "SII test_sequence: typecode mismatch for element %d\n", i));
	      break;
	    }
#endif
	}
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_sequence failed: as2->length = %d\n",
		  as2->length ()));
    }
  return 0;
}

int
Visi_Marshal_Client_Proxy::sii_test_recursive (CORBA_Marshal_Proxy::OPTIONS &options,
					       CORBA_Marshal_Proxy::RESULTS &results)
{
  // we will try a simple one here
  Marshal::Marshal_Recursive mr1, *elem1, *mr2, *elem2;

  mr1.value <<= (CORBA::Long)1;
  mr1.next.length (1);  // sequence of length 1

  // fill the element
  elem1 = &mr1.next[0];
  elem1->value <<= CORBA::Any::from_char ('D');
  elem1->next.length (0); // end of seq

  mr2 = new Marshal::Marshal_Recursive;

  try {
  ref_->test_recursive (mr1, mr2);
  }
  catch (CORBA::SystemException &se){
    cerr << "error invoking request: " << se << endl;
  }

  elem2 = &mr2->next[0];

  if ((*(CORBA::Long *)mr1.value.value () == *(CORBA::Long*)mr2->value.value ())
      && (mr1.next.length () == mr2->next.length ()) // same length sequence
      && (*(CORBA::Char *)elem1->value.value () == *(CORBA::Char *)elem2->value.value ())
      && (elem1->next.length () == elem2->next.length ()))
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_recursive success\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "SII test_recursive failure\n"));
    }
      
  return 0;
}

// -------------------------------------------------------
// all helper functions : DII
// -------------------------------------------------------

int
Visi_Marshal_Client_Proxy::dii_test_short (CORBA_Marshal_Proxy::OPTIONS &options,
					   CORBA_Marshal_Proxy::RESULTS &results)
{
  // DII variables
  CORBA::Request_ptr     request = CORBA::Request::_nil(); // DII request
  CORBA::NVList_ptr      arglist = CORBA::NVList::_nil();  // argument list
  CORBA::Context_ptr     ctx     = CORBA::Context::_nil(); // context
  CORBA::NVList_ptr result_list  = CORBA::NVList::_nil();  // result list
  CORBA::NamedValue_ptr result = CORBA::NamedValue::_nil();// actual result
  CORBA::Short s1, s2;

  s1 = 3;
  s2 = 0;

  orb_->create_list(0, arglist); // 2 arguments to be added
  *(arglist->add (CORBA::ARG_IN)->value ()) <<= s1;
  // this usage is also correct. All we care is to supply a storage area for
  // the OUT parameter
  //  arglist->add (CORBA::ARG_OUT)->value ()->replace (CORBA::_tc_short, &s2);
  *(arglist->add (CORBA::ARG_OUT)->value ()) <<= s2;
  orb_->create_list(1, result_list); // 1 result
  result = result_list->item (0);

  // create a request
  ref_->_create_request(ctx,
			"test_short",
			arglist,
			result,
			request,
			0);
  
  request->invoke (); // 2 way
  *(arglist->item(1)->value()) >>= s2;

  if (s1 == s2)
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_short success: s1 = %d, s2 = %d\n", s1, s2));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_short failed: s1 = %d, s2 = %d\n", s1, s2));
    }
  CORBA::release (request);
  CORBA::release (arglist);
  CORBA::release (result);
  CORBA::release (result_list);
  return 0;
}

int
Visi_Marshal_Client_Proxy::dii_test_long (CORBA_Marshal_Proxy::OPTIONS &options,
					  CORBA_Marshal_Proxy::RESULTS &results)
{
  // DII variables
  CORBA::Request_ptr     request = CORBA::Request::_nil(); // DII request
  CORBA::NVList_ptr      arglist = CORBA::NVList::_nil();  // argument list
  CORBA::Context_ptr     ctx     = CORBA::Context::_nil(); // context
  CORBA::NVList_ptr result_list  = CORBA::NVList::_nil();  // result list
  CORBA::NamedValue_ptr result = CORBA::NamedValue::_nil();// actual result
  CORBA::Long l1, l2;

  l1 = 256;

  orb_->create_list(0, arglist); // 2 arguments to be added
  *(arglist->add(CORBA::ARG_IN)->value()) <<= l1;
  arglist->add(CORBA::ARG_OUT)->value()->replace (CORBA::_tc_long, &l2);
  orb_->create_list(1, result_list); // 1 result
  result = result_list->item(0);

  // create a request
  ref_->_create_request(ctx,
			"test_long",
			arglist,
			result,
			request,
			0);
  
  request->invoke (); // 2 way
  *(arglist->item(1)->value()) >>= l2;

  if (l1 == l2)
    {
      ACE_DEBUG ((LM_DEBUG, "DII: test_long success: l1 = %d, l2 = %d\n", l1, l2));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "DII: test_long failed: l1 = %d, l2 = %d\n", l1, l2));
    }
  CORBA::release (request);
  CORBA::release (arglist);
  CORBA::release (result);
  CORBA::release (result_list);
  return 0;
}

int
Visi_Marshal_Client_Proxy::dii_test_octet (CORBA_Marshal_Proxy::OPTIONS &options,
					   CORBA_Marshal_Proxy::RESULTS &results)
  // octets
{
  // DII variables
  CORBA::Request_ptr     request = CORBA::Request::_nil(); // DII request
  CORBA::NVList_ptr      arglist = CORBA::NVList::_nil();  // argument list
  CORBA::Context_ptr     ctx     = CORBA::Context::_nil(); // context
  CORBA::NVList_ptr result_list  = CORBA::NVList::_nil();  // result list
  CORBA::NamedValue_ptr result = CORBA::NamedValue::_nil();// actual result
  CORBA::Octet o1, o2;

  o1 = (CORBA::Octet) 127;

  orb_->create_list(0, arglist); // 2 arguments to be added
  *(arglist->add(CORBA::ARG_IN)->value()) <<= CORBA::Any::from_octet (o1);
  arglist->add(CORBA::ARG_OUT)->value()->replace (CORBA::_tc_octet, &o2);
  orb_->create_list(1, result_list); // 1 result
  result = result_list->item(0);

  // create a request
  ref_->_create_request(ctx,
			"test_octet",
			arglist,
			result,
			request,
			0);
  
  request->invoke (); // 2 way
  *(arglist->item(1)->value()) >>= CORBA::Any::to_octet (o2);

  if (o1 == o2)
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_octet success: o1 = %c, o2 = %c\n", o1, o2));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_octet failed: o1 = %c, o2 = %c\n", o1, o2));
    }
  CORBA::release (request);
  CORBA::release (arglist);
  CORBA::release (result);
  CORBA::release (result_list);
  return 0;
}

int
Visi_Marshal_Client_Proxy::dii_test_char (CORBA_Marshal_Proxy::OPTIONS &options,
					  CORBA_Marshal_Proxy::RESULTS &results)
  // chars
{
  // DII variables
  CORBA::Request_ptr     request = CORBA::Request::_nil(); // DII request
  CORBA::NVList_ptr      arglist = CORBA::NVList::_nil();  // argument list
  CORBA::Context_ptr     ctx     = CORBA::Context::_nil(); // context
  CORBA::NVList_ptr result_list  = CORBA::NVList::_nil();  // result list
  CORBA::NamedValue_ptr result = CORBA::NamedValue::_nil();// actual result
  CORBA::Char c1, c2;

  c1 = 'B';

  orb_->create_list(0, arglist); // 2 arguments to be added
  *(arglist->add(CORBA::ARG_IN)->value()) <<= CORBA::Any::from_char (c1);
  arglist->add(CORBA::ARG_OUT)->value()->replace (CORBA::_tc_char, &c2);
  orb_->create_list(1, result_list); // 1 result
  result = result_list->item(0);

  // create a request
  ref_->_create_request(ctx,
			"test_char",
			arglist,
			result,
			request,
			0);
  
  request->invoke (); // 2 way
  *(arglist->item(1)->value()) >>= CORBA::Any::to_char (c2);

  if (c1 == c2)
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_char success: c1 = %c, c2 = %c\n", c1, c2));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_char failed: c1 = %c, c2 = %c\n", c1, c2));
    }
  CORBA::release (request);
  CORBA::release (arglist);
  CORBA::release (result);
  CORBA::release (result_list);
  return 0;
}

int
Visi_Marshal_Client_Proxy::dii_test_double (CORBA_Marshal_Proxy::OPTIONS &options,
					    CORBA_Marshal_Proxy::RESULTS &results)
  // doubles
{
  // DII variables
  CORBA::Request_ptr     request = CORBA::Request::_nil(); // DII request
  CORBA::NVList_ptr      arglist = CORBA::NVList::_nil();  // argument list
  CORBA::Context_ptr     ctx     = CORBA::Context::_nil(); // context
  CORBA::NVList_ptr result_list  = CORBA::NVList::_nil();  // result list
  CORBA::NamedValue_ptr result = CORBA::NamedValue::_nil();// actual result
  CORBA::Double d1, d2;

  d1 = 3.1415;

  orb_->create_list(0, arglist); // 2 arguments to be added
  *(arglist->add(CORBA::ARG_IN)->value()) <<= d1;
  arglist->add(CORBA::ARG_OUT)->value()->replace (CORBA::_tc_double, &d2);
  orb_->create_list(1, result_list); // 1 result
  result = result_list->item(0);

  // create a request
  ref_->_create_request(ctx,
			"test_double",
			arglist,
			result,
			request,
			0);
  
  request->invoke (); // 2 way
  *(arglist->item(1)->value()) >>= d2;

  if (d1 == d2)
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_double success: d1 = %f, d2 = %f\n", d1, d2));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_double failed: d1 = %f, d2 = %f\n", d1, d2));
    }
  CORBA::release (request);
  CORBA::release (arglist);
  CORBA::release (result);
  CORBA::release (result_list);
  return 0;
}

int
Visi_Marshal_Client_Proxy::dii_test_struct (CORBA_Marshal_Proxy::OPTIONS &options,
					    CORBA_Marshal_Proxy::RESULTS &results)
  // structs
{
  // DII variables
  CORBA::Request_ptr     request = CORBA::Request::_nil(); // DII request
  CORBA::NVList_ptr      arglist = CORBA::NVList::_nil();  // argument list
  CORBA::Context_ptr     ctx     = CORBA::Context::_nil(); // context
  CORBA::NVList_ptr result_list  = CORBA::NVList::_nil();  // result list
  CORBA::NamedValue_ptr result = CORBA::NamedValue::_nil();// actual result
  Marshal::Marshal_Struct ms1, ms2;

  ms1.s = 3;
  ms1.l = 256;
  ms1.c = 'B';
  ms1.o = (CORBA::Octet) 127;
  ms1.d = 3.1415;

  orb_->create_list(0, arglist); // 2 arguments to be added
  *(arglist->add(CORBA::ARG_IN)->value()) <<= ms1;
  arglist->add(CORBA::ARG_OUT)->value()->replace (Marshal::_tc_Marshal_Struct, &ms2);
  orb_->create_list(1, result_list); // 1 result
  result = result_list->item(0);

  // create a request
  ref_->_create_request(ctx,
			"test_struct",
			arglist,
			result,
			request,
			0);
  
  request->invoke (); // 2 way
  *(arglist->item(1)->value()) >>= ms2;

  if (ms1.s == ms2.s && ms1.l == ms2.l && ms1.c == ms2.c && ms1.o == ms2.o &&
      ms1.d == ms2.d)
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_struct success\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_struct failed\n"));
    }
  CORBA::release (request);
  CORBA::release (arglist);
  CORBA::release (result);
  CORBA::release (result_list);
  return 0;
}

int
Visi_Marshal_Client_Proxy::dii_test_union (CORBA_Marshal_Proxy::OPTIONS &options,
					   CORBA_Marshal_Proxy::RESULTS &results)
  // unions
{
  // DII variables
  CORBA::Request_ptr     request = CORBA::Request::_nil(); // DII request
  CORBA::NVList_ptr      arglist = CORBA::NVList::_nil();  // argument list
  CORBA::Context_ptr     ctx     = CORBA::Context::_nil(); // context
  CORBA::NVList_ptr result_list  = CORBA::NVList::_nil();  // result list
  CORBA::NamedValue_ptr result = CORBA::NamedValue::_nil();// actual result
  Marshal::Marshal_Union u1, u2;
  Marshal::Marshal_Struct ms1, ms2;


  // first test an enumerated case
  u1._d (Marshal::e_1st);
  u1.l (4567);

  orb_->create_list(0, arglist); // 2 arguments to be added
  *(arglist->add(CORBA::ARG_IN)->value()) <<= u1;
  arglist->add(CORBA::ARG_OUT)->value()->replace (Marshal::_tc_Marshal_Union, &u2);
  orb_->create_list(1, result_list); // 1 result
  result = result_list->item(0);

  // create a request
  ref_->_create_request(ctx,
			"test_union",
			arglist,
			result,
			request,
			0);
  
  request->invoke (); // 2 way
  *(arglist->item(1)->value()) >>= u2;

  if ( (u1._d () == u2._d()) && (u1.l () == u2.l ()))
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_union for long (case e_1st) success\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_union for long (case e_1st) failed\n"));
    }
  CORBA::release (request);
  CORBA::release (arglist);
  CORBA::release (result);
  CORBA::release (result_list);

  // now test the default case
  u1._d (Marshal::e_5th); // e_6th doesn't work

  u1.ms ().s = 3;
  u1.ms ().l = 256;
  u1.ms ().c = 'B';
  u1.ms ().o = (CORBA::Octet) 127;
  u1.ms ().d = 3.1415;


  orb_->create_list(0, arglist); // 2 arguments to be added
  *(arglist->add(CORBA::ARG_IN)->value()) <<= u1;
  arglist->add(CORBA::ARG_OUT)->value()->replace (Marshal::_tc_Marshal_Union, &u2);
  orb_->create_list(1, result_list); // 1 result
  result = result_list->item(0);

  // create a request
  ref_->_create_request(ctx,
			"test_union",
			arglist,
			result,
			request,
			0);
  
  request->invoke (); // 2 way
  *(arglist->item(1)->value()) >>= u2;

  if (u1.ms ().s == u2.ms ().s && u1.ms ().l == u2.ms ().l && u1.ms ().c == u2.ms ().c && u1.ms ().o == u2.ms ().o &&
      u1.ms ().d == u2.ms ().d)
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_union for default (case e_6th) success\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_union for default (case e_6th) failed\n"));
    }
  CORBA::release (request);
  CORBA::release (arglist);
  CORBA::release (result);
  CORBA::release (result_list);
  return 0;
}

int
Visi_Marshal_Client_Proxy::dii_test_any (CORBA_Marshal_Proxy::OPTIONS &options,
					 CORBA_Marshal_Proxy::RESULTS &results)
{
  // DII variables
  CORBA::Request_ptr     request = CORBA::Request::_nil(); // DII request
  CORBA::NVList_ptr      arglist = CORBA::NVList::_nil();  // argument list
  CORBA::Context_ptr     ctx     = CORBA::Context::_nil(); // context
  CORBA::NVList_ptr result_list  = CORBA::NVList::_nil();  // result list
  CORBA::NamedValue_ptr result = CORBA::NamedValue::_nil();// actual result
  // we send a struct thru the any
  CORBA::Any a1, *a2;
  Marshal::Marshal_Struct ms1, *ms2;

  ms1.s = 3;
  ms1.l = 256;
  ms1.c = 'B';
  ms1.o = (CORBA::Octet) 127;
  ms1.d = 3.1415;

  // populate the any with a MarshalStruct
  a1 <<= ms1;

  a2 = new CORBA::Any;

  // now test it
  orb_->create_list(0, arglist); // 2 arguments to be added
  *(arglist->add(CORBA::ARG_IN)->value()) <<= a1;
  arglist->add(CORBA::ARG_OUT)->value()->replace (CORBA::_tc_any, a2);
  orb_->create_list(1, result_list); // 1 result
  result = result_list->item(0);

  // create a request
  ref_->_create_request(ctx,
			"test_any",
			arglist,
			result,
			request,
			0);
  
  request->invoke (); // 2 way
  *(arglist->item(1)->value()) >>= (*a2);

  // check if the two typecodes are equal
  if (a2->type ()->equal (Marshal::_tc_Marshal_Struct))
    {
      // now see if the values are same
      ms2 = (Marshal::Marshal_Struct *)a2->value ();
      if (ms1.s == ms2->s && ms1.l == ms2->l && ms1.c == ms2->c && ms1.o == ms2->o &&
	  ms1.d == ms2->d)
	{
	  ACE_DEBUG ((LM_DEBUG, "DII test_any success\n"));
	}
      else
	{
	  ACE_DEBUG ((LM_DEBUG, "DII test_any value match failed\n"));
	}
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_any failed due to typecode mismatch\n"));
    }
  CORBA::release (request);
  CORBA::release (arglist);
  CORBA::release (result);
  CORBA::release (result_list);
  return 0;
}

int
Visi_Marshal_Client_Proxy::dii_test_sequence (CORBA_Marshal_Proxy::OPTIONS &options,
					      CORBA_Marshal_Proxy::RESULTS &results)
{
  // DII variables
  CORBA::Request_ptr     request = CORBA::Request::_nil(); // DII request
  CORBA::NVList_ptr      arglist = CORBA::NVList::_nil();  // argument list
  CORBA::Context_ptr     ctx     = CORBA::Context::_nil(); // context
  CORBA::NVList_ptr result_list  = CORBA::NVList::_nil();  // result list
  CORBA::NamedValue_ptr result = CORBA::NamedValue::_nil();// actual result
  // make a sequence of Anys and fill each one with a different data type
  Marshal::AnySeq as1 (7), *as2;
  Marshal::Marshal_Struct ms;
  Marshal::Marshal_Union u;

  ms.s = 3;
  ms.l = 256;
  ms.c = 'B';
  ms.o = (CORBA::Octet) 127;
  ms.d = 3.1415;

  u._d (Marshal::e_5th); // somehow, the default case e_6th doesn't seem to
			 // work here
  u.ms ().s = 3;
  u.ms ().l = 256;
  u.ms ().c = 'B';
  u.ms ().o = (CORBA::Octet) 127;
  u.ms ().d = 3.1415;

  // it looks like we must tell the sequence how many elements it is going to
  // carry. By just fixing the max value does not work.

  as1.length (7);

  as1[0] <<= (CORBA::Short)3;
  as1[1] <<= (CORBA::Long) 256;
  as1[2] <<= CORBA::Any::from_octet ((CORBA::Octet)127);
  as1[3] <<= CORBA::Any::from_char ('B');
  as1[4] <<= (CORBA::Double)3.1415; // double
  as1[5] <<= ms;     // struct
  as1[6] <<= u;      // union

  as2 = new Marshal::AnySeq;

  orb_->create_list(0, arglist); // 2 arguments to be added
  *(arglist->add(CORBA::ARG_IN)->value()) <<= as1;
  arglist->add(CORBA::ARG_OUT)->value()->replace (Marshal::_tc_AnySeq, as2);
  orb_->create_list(1, result_list); // 1 result
  result = result_list->item(0);

  // create a request
  ref_->_create_request(ctx,
			"test_sequence",
			arglist,
			result,
			request,
			0);
  
  request->invoke (); // 2 way
  *(arglist->item(1)->value()) >>= (*as2);

#if 0
  // the following is extremely weird. I do not understand why it is not the
  // same as _tc_Marshal_Union, but its _kind field does have _tk_union in it.
  if ((*as2)[6].type ()->equal (Marshal::_tc_Marshal_Union))
    {
      cerr << "6th elem has union typecode" << endl;
    }
  else
    {
      cerr << "6th elem doesnot have union typecode" << endl;
      cerr << "its typecode kind = " << (*as2)[6].type ()->kind () << endl;
    }
#endif

  ACE_DEBUG ((LM_DEBUG, "DII test_sequence\n"));
  if (as1.length () == as2->length ())
    {
      // now make sure that the elements are same
      for (int i=0; i < as1.length (); i++)
	{
#if 0
	  if (as1[i].type ()->equal ((*as2)[i].type ()))
	    {
#endif
	      // now see if the values are same
	      switch (i)
		{
		case 0:
		  if (*(CORBA::Short *)as1[i].value () == *(CORBA::Short
							    *)(*as2)[i].value ())
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tDII test_sequence, shorts matched\n"));
		    }
		  else
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tDII test_sequence failed on shorts match\n"));
		    }
		  break;
		case 1:
		  if (*(CORBA::Long *)as1[i].value () == *(CORBA::Long
							    *)(*as2)[i].value ())
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tDII test_sequence, longs matched\n"));
		    }
		  else
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tDII test_sequence failed on longs match\n"));
		    }
		  break;
		case 2:
		  if (*(CORBA::Octet *)as1[i].value () == *(CORBA::Octet
							    *)(*as2)[i].value ())
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tDII test_sequence, octets matched\n"));
		    }
		  else
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tDII test_sequence failed on octets match\n"));
		    }
		  break;
		case 3:
		  if (*(CORBA::Char *)as1[i].value () == *(CORBA::Char
							    *)(*as2)[i].value ())
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tDII test_sequence, chars matched\n"));
		    }
		  else
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tDII test_sequence failed on chars match\n"));
		    }
		  break;
		case 4:
		  if (*(CORBA::Double *)as1[i].value () == *(CORBA::Double
							    *)(*as2)[i].value ())
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tDII test_sequence, doubles matched\n"));
		    }
		  else
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tDII test_sequence failed on doubles match\n"));
		    }
		  break;
		case 5:
		  {
		    Marshal::Marshal_Struct *ms1, *ms2;
		    
		    ms1 = (Marshal::Marshal_Struct *)as1[i].value ();
		    ms2 = (Marshal::Marshal_Struct *)(*as2)[i].value ();

		    if (ms1->s == ms2->s && ms1->l == ms2->l && ms1->c == ms2->c && ms1->o == ms2->o &&
			ms1->d == ms2->d)
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tDII test_sequence, structs matched\n"));
		    }
		  else
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tDII test_sequence failed on structs match\n"));
		    }
		  }
		  break;
		case 6:
		  {
		    Marshal::Marshal_Union *u1, *u2;
		    
		    u1 = (Marshal::Marshal_Union *)as1[i].value ();
		    u2 = (Marshal::Marshal_Union *)(*as2)[i].value ();

		    if (u1->_d () == u2->_d () && u1->ms ().s == u2->ms ().s &&
			u1->ms ().l == u2->ms ().l && 
			u1->ms ().c == u2->ms ().c && u1->ms ().o == u2->ms ().o &&
			u1->ms ().d == u2->ms ().d)
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tDII test_sequence, unions matched\n"));
		    }
		  else
		    {
		      ACE_DEBUG ((LM_DEBUG, "\tDII test_sequence failed on unions match\n"));
		    }
		  }
		  break;
		}
#if 0
	    }
	  else
	    {
	      ACE_DEBUG ((LM_DEBUG, 
			  "DII test_sequence: typecode mismatch for element %d\n", i));
	      break;
	    }
#endif
	}
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_sequence failed: as2->length = %d\n",
		  as2->length ()));
    }
  CORBA::release (request);
  CORBA::release (arglist);
  CORBA::release (result);
  CORBA::release (result_list);
  return 0;
}

int
Visi_Marshal_Client_Proxy::dii_test_recursive (CORBA_Marshal_Proxy::OPTIONS &options,
					       CORBA_Marshal_Proxy::RESULTS &results)
{
  // we will try a simple one here
  Marshal::Marshal_Recursive mr1, *elem1, *mr2, *elem2;

  mr1.value <<= (CORBA::Long)1;
  mr1.next.length (1);  // sequence of length 1

  // fill the element
  elem1 = &mr1.next[0];
  elem1->value <<= CORBA::Any::from_char ('D');
  elem1->next.length (0); // end of seq

  mr2 = new Marshal::Marshal_Recursive;

  try {
    ref_->test_recursive (mr1, mr2);
  }
  catch (CORBA::SystemException &se){
    cerr << "error invoking request: " << se << endl;
  }

  elem2 = &mr2->next[0];

  if ((*(CORBA::Long *)mr1.value.value () == *(CORBA::Long*)mr2->value.value ())
      && (mr1.next.length () == mr2->next.length ()) // same length sequence
      && (*(CORBA::Char *)elem1->value.value () == *(CORBA::Char *)elem2->value.value ())
      && (elem1->next.length () == elem2->next.length ()))
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_recursive success\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "DII test_recursive failure\n"));
    }
      
  return 0;
}

