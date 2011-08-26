// $Id$

#include "valueboxC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}


#define OBV_VERITY(Condition) \
{ \
    if ((Condition)==0) \
    { \
        fail++; \
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Failure at line %l\n")); \
    } \
}

//
// Templated function for testing various aspects of valuetypes.
// valuebox is a type created by the user, with UT as the underlying type.
// It is assumed that, on entry, valuebox->_value() != val1, and that
// val1 and val2 are distinct.
//
template <class BoxT, class UT>
int box_test1 (BoxT *valuebox, UT val1, UT val2)
{
    int fail = 0;
    BoxT *valuebox_clone = 0;
    ACE_NEW_RETURN (valuebox_clone,
                    BoxT (val1),
                    1);

    // should be a deep copy of val1...
    OBV_VERITY ( &valuebox_clone->_boxed_inout () != &valuebox->_boxed_inout () );

    // but values should be equal
    OBV_VERITY (ACE::is_equal (valuebox_clone->_value (), valuebox->_value ()));

    // Check that modifier is working.
    valuebox_clone->_value ( val2 );
    OBV_VERITY (!ACE::is_equal (valuebox_clone->_value (), valuebox->_value ()));

    // use operator=
    *valuebox = val2;
    OBV_VERITY (ACE::is_equal (valuebox_clone->_value (), valuebox->_value ()));

    // Check that _value and _boxed_in are the same.
    OBV_VERITY (ACE::is_equal (valuebox_clone->_value (), valuebox_clone->_boxed_in ()));
    OBV_VERITY (ACE::is_equal (valuebox->_value (), valuebox->_boxed_in ()));

    // Used _boxed_inout to change the value
    OBV_VERITY (!ACE::is_equal (valuebox->_value (), val1));
    valuebox->_boxed_inout () = val1;
    OBV_VERITY (ACE::is_equal (valuebox->_value (), val1));

    // Use _boxed_out to access the value
    OBV_VERITY (!ACE::is_equal (valuebox_clone->_value (), val1));
    valuebox_clone->_boxed_out () = val1;
    OBV_VERITY (ACE::is_equal (valuebox_clone->_value (), val1));

    // Test _copy_value
    CORBA::ValueBase *copy = valuebox->_copy_value ();
    OBV_VERITY ( copy != 0 );
    // check add_ref, remove_ref
    copy->_add_ref ();
    copy->_remove_ref ();

    // try downcast...then we can check that copy was correct.
    BoxT *down = BoxT::_downcast (copy);
    if (down == 0)
      {
        fail++;
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Failure at line %l\n"));
      }
    else
      {
        OBV_VERITY (ACE::is_equal (down->_value (), val1));
        down->_value ( val2 );
        OBV_VERITY (!ACE::is_equal (down->_value (), valuebox->_value ()));
        OBV_VERITY (ACE::is_equal (down->_value (), val2));
      }

    CORBA::remove_ref (copy);

    // cleanup. Use purify on the PC to check for leaks.
    CORBA::remove_ref (valuebox_clone);

    return fail;
}

template <class BoxT, class UT>
int simple_box_test ()
{
    int fail = 0;
    BoxT *p = 0;
    ACE_NEW_RETURN (p,
                    BoxT (101),
                    1);
    fail += box_test1<BoxT, UT> (p, 101, 202);

    CORBA::remove_ref (p);

    return fail;

}

//
// Test boxed values that use an underlying UT&
//
template <class BoxT, class UT>
int box_test_ref (BoxT *valuebox, UT &val1, UT &val2)
{
    int fail = 0;
    BoxT *p = 0;
    ACE_NEW_RETURN (p,
                    BoxT (val1),
                    1);

    // should be a deep copy of val1...
    OBV_VERITY ( &p->_boxed_inout () != &valuebox->_boxed_inout () );

    p->_value ( val2 ); // deep copy
    OBV_VERITY ( &p->_boxed_inout () != &valuebox->_boxed_inout () );

    *valuebox = val2;      // deep copy, too.
    OBV_VERITY ( &p->_boxed_inout () != &valuebox->_boxed_inout () );

    CORBA::remove_ref (p);

    return fail;
}

int test_basic (void)
{
    int fail = 0;

    // Basic types
    fail += simple_box_test<VBshort,  CORBA::Short> ();
    fail += simple_box_test<VBlong,  CORBA::Long> ();
    fail += simple_box_test<VBlonglong, CORBA::LongLong> ();
    fail += simple_box_test<VBushort, CORBA::UShort> ();
    fail += simple_box_test<VBulong, CORBA::ULong> ();
    fail += simple_box_test<VBulonglong, CORBA::ULongLong> ();
    fail += simple_box_test<VBwchar,  CORBA::WChar> ();
    fail += simple_box_test<VBoctet,  CORBA::Octet> ();
    fail += simple_box_test<VBfloat,  CORBA::Float> ();
    fail += simple_box_test<VBdouble,  CORBA::Double> ();

    VBchar *pchar = 0;
    ACE_NEW_RETURN (pchar,
                    VBchar ('A'),
                    1);
    fail += box_test1<VBchar, CORBA::Char> (pchar, 'A', 'Z');
    CORBA::remove_ref (pchar);

    VBboolean *pbool = 0;
    ACE_NEW_RETURN (pbool,
                    VBboolean (true),
                    1);
    fail += box_test1<VBboolean, CORBA::Boolean> (pbool, true, false);
    CORBA::remove_ref (pbool);

    // Typedef of basic types
    fail += simple_box_test<VBTDshort,  CORBA::Short> ();
    fail += simple_box_test<VBTDlong,  CORBA::Long> ();
    fail += simple_box_test<VBTDlonglong, CORBA::LongLong> ();
    fail += simple_box_test<VBTDushort, CORBA::UShort> ();
    fail += simple_box_test<VBTDulong, CORBA::ULong> ();
    fail += simple_box_test<VBTDulonglong, CORBA::ULongLong> ();
    fail += simple_box_test<VBTDwchar,  CORBA::WChar> ();
    fail += simple_box_test<VBTDoctet,  CORBA::Octet> ();
    fail += simple_box_test<VBTDfloat,  CORBA::Float> ();
    fail += simple_box_test<VBTDdouble,  CORBA::Double> ();

    VBTDchar *pchar2 = 0;
    ACE_NEW_RETURN (pchar2,
                    VBTDchar ('A'),
                    1);
    fail += box_test1<VBTDchar, CORBA::Char> (pchar2, 'A', 'Z');
    CORBA::remove_ref (pchar2);

    VBTDboolean *pbool2 = 0;
    ACE_NEW_RETURN (pbool2,
                    VBTDboolean (true),
                    1);
    fail += box_test1<VBTDboolean, CORBA::Boolean> (pbool2, true, false);
    CORBA::remove_ref (pbool2);

    // Enumerated type
    VBenum *penum = 0;
    ACE_NEW_RETURN (penum,
                    VBenum (yellow),
                    1);
    fail += box_test1<VBenum, Color> (penum, yellow, red);
    CORBA::remove_ref (penum);

    // Typedef of enumerated type
    VBTDenum *penum2 = 0;
    ACE_NEW_RETURN (penum2,
                    VBTDenum (yellow),
                    1);
    fail += box_test1<VBTDenum, Color> (penum2, yellow, red);
    CORBA::remove_ref (penum2);

    // Any
    CORBA::Any *a1 = 0;
    ACE_NEW_RETURN (a1,
                    CORBA::Any (),
                    1);
    CORBA::Any_var any1 (a1);

    CORBA::Any *a2 = 0;
    ACE_NEW_RETURN (a2,
                    CORBA::Any (),
                    1);
    CORBA::Any_var any2 (a2);

    VBany *pany = 0;
    ACE_NEW_RETURN (pany,
                    VBany (any1.in ()),
                    1);
    fail += box_test_ref<VBany, CORBA::Any> (pany, any1.inout (),
                                            any2.inout ());
    CORBA::remove_ref (pany);

    // Typedef of Any
    VBTDany *pany2 = 0;
    ACE_NEW_RETURN (pany2,
                    VBTDany (any1.in ()),
                    1);
    fail += box_test_ref<VBTDany, CORBA::Any> (pany2, any1.inout (),
                                              any2.inout ());
    CORBA::remove_ref (pany2);

    return fail;
}

int test_basic_invocations (Test * test_object)
{
    int fail = 0;

    VBlong *p1 = 0;
    VBlong *p2 = 0;
    VBlong *p3;

    vb_basic::M_VBlong *mp1 = 0;
    vb_basic::M_VBlong *mp2 = 0;
    vb_basic::M_VBlong *mp3;

    try
      {

        //============================================================
        // Test method invocation with boxed value
        //============================================================

        ACE_NEW_RETURN (p1,
                        VBlong(25),
                        1);
        ACE_NEW_RETURN (p2,
                        VBlong(53),
                        1);

        OBV_VERITY (p1->_value () == 25);
        OBV_VERITY (p2->_value () == 53);

        VBlong_var result =
          test_object->basic_op1(p1, p2, p3);

        OBV_VERITY (p2->_value () == (53*3));
        OBV_VERITY (p3->_value () == (53*5));
        OBV_VERITY (result->_value () == (p1->_value () *3));

        //============================================================
        // Test method invocation with boxed value from nested module
        //============================================================

        ACE_NEW_RETURN (mp1,
                        vb_basic::M_VBlong(25),
                        1);

        ACE_NEW_RETURN (mp2,
                        vb_basic::M_VBlong(53),
                        1);

        OBV_VERITY (mp1->_value () == 25);
        OBV_VERITY (mp2->_value () == 53);

        vb_basic::M_VBlong_var mresult =
          test_object->basic_op2(mp1, mp2, mp3);

        OBV_VERITY (mp2->_value () == (53*3));
        OBV_VERITY (mp3->_value () == (53*5));
        OBV_VERITY (mresult->_value () == (mp1->_value () *3));

        //============================================================
        // Test _boxed_in(), _boxed_inout(), and _boxed_out())
        //============================================================

        p1->_value(67);
        p2->_value(93);

        long lresult =
          test_object->basic_op3(p1->_boxed_in(), p2->_boxed_inout(),
                                 p3->_boxed_out());

        OBV_VERITY (p2->_value () == (93*3));
        OBV_VERITY (p3->_value () == (93*5));
        OBV_VERITY (lresult == (p1->_value()*3));
      }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_basic_invocations");
      fail = 1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) test_basic_invocations: caught a C++ exception\n"));
      fail = 1;
    }
  if (p1) p1->_remove_ref ();
  if (p2) p2->_remove_ref ();
  if (p3) p3->_remove_ref ();

  if (mp1) mp1->_remove_ref ();
  if (mp2) mp2->_remove_ref ();
  if (mp3) mp3->_remove_ref ();

  return fail;
}


int test_boxed_string()
{
  int fail = 0;
  const char *string1 = "First-string";
  const char *string2 = "Second-string";

  // Establish that we have data setup correctly...
  OBV_VERITY (ACE_OS::strcmp (string1, string2) <  0);
  OBV_VERITY (ACE_OS::strcmp (string2, string1) >  0);
  OBV_VERITY (ACE_OS::strcmp (string1, string1) == 0);

  // Make some objects, using our data
  VBstring *temp = 0;
  ACE_NEW_RETURN (temp,
                  VBstring(string1),
                  1);
  VBstring_var vbstring1 (temp);

  VBstring    *vbstring2 = 0;
  ACE_NEW_RETURN (vbstring2,
                  VBstring(string1), // tests const char * ctor.
                  1);

  OBV_VERITY (ACE_OS::strcmp (vbstring1->_value(), string1) == 0);
  OBV_VERITY (ACE_OS::strcmp (vbstring2->_value(), string1) == 0);

  // Test assignment operators
  char *carray1 = 0;
  ACE_NEW_RETURN (carray1,
                  char[15],
                  1);
  ACE_OS::memcpy(carray1, string2, ACE_OS::strlen(string2));
  *vbstring2 = carray1;              // char * (adopted by box)
  OBV_VERITY ((*vbstring2)[0] == 'S');
  *vbstring2 = string1;
  OBV_VERITY ((*vbstring2)[0] == 'F');
  CORBA::String_var svar(string2);
  *vbstring2 = svar;
  OBV_VERITY ((*vbstring2)[0] == 'S');

  // Test _value modifiers--like assignment drill above.
  char *carray2 = 0;
  ACE_NEW_RETURN (carray2,
                  char[15],
                  1);
  ACE_OS::memcpy(carray2, string1, ACE_OS::strlen(string1));
  vbstring2->_value(carray2);        // char * (adopted by box)
  OBV_VERITY ((*vbstring2)[0] == 'F');
  vbstring2->_value(string2);       // const char *
  OBV_VERITY ((*vbstring2)[0] == 'S');
  (*vbstring2)[0] = 'Y';
  OBV_VERITY ((*vbstring2)[0] != 'S');
  vbstring2->_value(svar);
  OBV_VERITY ((*vbstring2)[0] == 'S');
  // test value accessor
  OBV_VERITY ( (vbstring2->_value())[0] == 'S' );

  //
  // Test ctors.
  // const boxed string
  VBstring *vbstring3 = 0;
  ACE_NEW_RETURN (vbstring3,
                  VBstring(*vbstring2),
                  1);
  OBV_VERITY ((*vbstring3)[0] == 'S');
  (*vbstring3)[0] = 'W';
  OBV_VERITY ((*vbstring3)[0] == 'W' && (*vbstring2)[0] == 'S');
  vbstring3->_remove_ref ();

  //
  // char *
  char *carray3 = 0;
  ACE_NEW_RETURN (carray3,
                  char[15],
                  1);
  ACE_OS::memcpy(carray3, string1, ACE_OS::strlen(string1));
  VBstring *vbstring4 = 0;
  ACE_NEW_RETURN (vbstring4,
                  VBstring(carray3),
                  1);
  OBV_VERITY ((*vbstring4)[0] == 'F');
  vbstring4->_remove_ref ();

  //
  // test CORBA::String_var ctor
  VBstring *vbstring5 = 0;
  ACE_NEW_RETURN (vbstring5,
                  VBstring(svar),
                  1);
  OBV_VERITY ((*vbstring5)[0] == 'S');
  (*vbstring5)[0] = 'W';
  OBV_VERITY ((*vbstring5)[0] == 'W' && (svar.in())[0] == 'S');
  vbstring5->_remove_ref ();

  // release, as usual
  vbstring2->_remove_ref ();
  return fail;
}


int test_boxed_string_invocations (Test * test_object)
{
    int fail = 0;

    VBstring *p1 = 0;
    VBstring *p2 = 0;
    VBstring *p3 = 0;

    try
      {

        //============================================================
        // Test method invocation with boxed value
        //============================================================

        ACE_NEW_RETURN (p1,
                        VBstring(CORBA::string_dup ("string1")),
                        1);
        ACE_NEW_RETURN (p2,
                        VBstring(CORBA::string_dup ("string2")),
                        1);

        OBV_VERITY (ACE_OS::strcmp(p1->_value (), "string1") == 0);
        OBV_VERITY (ACE_OS::strcmp(p2->_value (), "string2") == 0);

        VBstring_var result = test_object->string_op1(p1, p2, p3);

        OBV_VERITY (ACE_OS::strcmp(p2->_value (), "2string") == 0);
        OBV_VERITY (ACE_OS::strcmp(p3->_value (), "2string") == 0);
        OBV_VERITY (ACE_OS::strcmp(result->_value (), "1string") == 0);

        //============================================================
        // Test _boxed_in(), _boxed_inout(), and _boxed_out())
        //============================================================

        p2->_value(CORBA::string_dup ("second string2"));

        CORBA::String_var sresult =
          test_object->string_op2(p1->_boxed_in(), p2->_boxed_inout(),
                                  p3->_boxed_out());

        OBV_VERITY (ACE_OS::strcmp(p2->_value (), "2second string") == 0);
        OBV_VERITY (ACE_OS::strcmp(p3->_value (), "2second string") == 0);
        OBV_VERITY (ACE_OS::strcmp(sresult.in (), "1string") == 0);
      }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_boxed_string_invocations");
      fail = 1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) test_boxed_string_invocations: "
                  "caught a C++ exception\n"));
      fail = 1;
    }
  if (p1) p1->_remove_ref ();
  if (p2) p2->_remove_ref ();
  if (p3) p3->_remove_ref ();

  return fail;
}

//
// Test boxed sequence types.
//
int test_boxed_sequence (void)
{
  int fail = 0;
  try
    {
      VBseqlong     *vbseqlong1 = 0;
      ACE_NEW_RETURN (vbseqlong1,
                      VBseqlong (),
                      1);

      VBseqlong     *temp = 0;
      ACE_NEW_RETURN (temp,
                      VBseqlong (),
                      1);

      VBseqlong_var  vbseqlong2 (temp);
      OBV_VERITY (vbseqlong1->length() == 0);
      OBV_VERITY (vbseqlong2->length() == 0);
      CORBA::Long *longarray = 0;
      ACE_NEW_RETURN (longarray,
                      CORBA::Long[3],
                      1);
      longarray[0] = 101;
      longarray[1] = 202;
      longarray[2] = 303;

      // Create a sequence
      TDseqlong *temp2 = 0;
      ACE_NEW_RETURN (temp2,
                      TDseqlong(10, 3, longarray, 1),
                      1);
      TDseqlong_var seqlong1 (temp2);
      OBV_VERITY (seqlong1[0] == 101 && seqlong1[2] == 303);

      VBseqlong *vbseqlong3 = 0;
      ACE_NEW_RETURN (vbseqlong3,
                      VBseqlong(seqlong1.in()),
                      1);

      // Test sequence ctor.
      VBseqlong *vbseqlong4 = 0;
      ACE_NEW_RETURN (vbseqlong4,
                      VBseqlong(10, 3, longarray, 0),
                      1);

      // Test assignment and subscript operators
      vbseqlong2 = vbseqlong3;
      OBV_VERITY (vbseqlong2->length() == 3);
      VBseqlong &vbseqlong5 = *vbseqlong2.inout();
      OBV_VERITY (vbseqlong5[2] == 303);
      vbseqlong5[2] = 444;
      OBV_VERITY (vbseqlong5[2] == 444);
      OBV_VERITY (seqlong1[0] == 101 && seqlong1[2] == 303);
      OBV_VERITY ((*vbseqlong4)[0] == 101 && (*vbseqlong4)[2] == 303);
      seqlong1[0] = 111;
      OBV_VERITY ((*vbseqlong4)[0] == 111);
      OBV_VERITY (vbseqlong4->maximum() == 10);
      *vbseqlong4 = vbseqlong1->_value();
      OBV_VERITY (vbseqlong4->length() == 0);

      // Test copy_value
      VBseqlong *vbseqlong6 = VBseqlong::_downcast( vbseqlong4->_copy_value() );
      if (vbseqlong6 == 0)
        {
          fail++;
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Failure at line %l\n"));
        }
      else
        {
          OBV_VERITY (vbseqlong6->length() == 0);
          vbseqlong6->_remove_ref ();
        }

      // release
      vbseqlong1->_remove_ref ();
      vbseqlong4->_remove_ref ();
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_boxed_sequence");
      fail = 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "test_boxed_sequence : unexpected exception caught\n"));
    }
  return fail;
}



int test_boxed_sequence_invocations (Test * test_object)
{
  int fail = 0;
  VBseqlong *p1 = 0;
  VBseqlong *p2 = 0;
  VBseqlong *p3;

  try
    {

      //============================================================
      // Test method invocation with boxed value
      //============================================================

      ACE_NEW_RETURN (p1,
                      VBseqlong(4),
                      1);
      ACE_NEW_RETURN (p2,
                      VBseqlong(3),
                      1);
      p1->length(4);
      p2->length(3);

      (*p1)[0] = 10;
      (*p1)[1] = 9;
      (*p1)[2] = 8;
      (*p1)[3] = 7;

      (*p2)[0] = 100;
      (*p2)[1] = 99;
      (*p2)[2] = 98;

      OBV_VERITY ((*p1)[0] == 10);
      OBV_VERITY ((*p1)[1] ==  9);
      OBV_VERITY ((*p1)[2] ==  8);
      OBV_VERITY ((*p1)[3] ==  7);

      VBseqlong_var result = test_object->seq_op1(p1, p2, p3);

      OBV_VERITY ((*p2)[0] == 100*3);
      OBV_VERITY ((*p2)[1] == 99*3);
      OBV_VERITY ((*p2)[2] == 98*3);
      OBV_VERITY ((*p3)[0] == 100*5);
      OBV_VERITY ((*p3)[1] == 99*5);
      OBV_VERITY ((*p3)[2] == 98*5);
      OBV_VERITY ((*result.in ())[0] == 10);
      OBV_VERITY ((*result.in ())[1] ==  9);
      OBV_VERITY ((*result.in ())[2] ==  8);
      OBV_VERITY ((*result.in ())[3] ==  7);

      //============================================================
      // Test _boxed_in(), _boxed_inout(), and _boxed_out())
      //============================================================

      test_object->seq_op2(p1->_boxed_in(), p2->_boxed_inout(),
                            p3->_boxed_out());

      OBV_VERITY ((*p2)[0] == 100*3*3);
      OBV_VERITY ((*p2)[1] == 99*3*3);
      OBV_VERITY ((*p2)[2] == 98*3*3);
      OBV_VERITY ((*p3)[0] == (*p1)[0]*5);
      OBV_VERITY ((*p3)[1] == (*p1)[1]*5);
      OBV_VERITY ((*p3)[2] == (*p1)[2]*5);
      OBV_VERITY ((*p3)[3] == (*p1)[3]*5);

      p1->_remove_ref ();
      p2->_remove_ref ();
      p3->_remove_ref ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_boxed_sequence_invocations");
      fail = 1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) test_boxed_sequence_invocations: "
                  "caught a C++ exception\n"));
      fail = 1;
    }

  if (p1) p1->_remove_ref ();
  if (p2) p2->_remove_ref ();
  if (p3) p3->_remove_ref ();

  return fail;
}


//
// Test a boxed struct type. This is not templated since the struct
// members are accessed by name, so this is specific to a certain IDL.
//
int test_boxed_struct (void)
{
  int fail = 0;

  Fixed_Struct1 *fixed_struct_a = 0;
  ACE_NEW_RETURN (fixed_struct_a,
                  Fixed_Struct1,
                  1);
  fixed_struct_a->l = 3233;
  fixed_struct_a->abstruct.s1 = 73;
  fixed_struct_a->abstruct.s2 = 37;

  // Test the VBfixed_struct1 constructor
  VBfixed_struct1 *valuebox1 = 0;
  ACE_NEW_RETURN (valuebox1,
                  VBfixed_struct1 (*fixed_struct_a),
                  1);

  // Test boxed copy ctor.
  VBfixed_struct1* valuebox2_ptr = 0;
  ACE_NEW_RETURN (valuebox2_ptr,
                  VBfixed_struct1 (*valuebox1),
                  1);
  VBfixed_struct1_var valuebox2 = valuebox2_ptr;

  OBV_VERITY (valuebox1->l () == valuebox2->l ());
  OBV_VERITY ((valuebox1->abstruct ()).s1 == (valuebox2->abstruct ()).s1 );
  OBV_VERITY ((valuebox1->abstruct ()).s2 == (valuebox2->abstruct ()).s2 );

  // Change an element
  valuebox1->l (505);
  OBV_VERITY (valuebox1->l () != valuebox2->l ());

  // Change some more, to test other types.
  (valuebox2->abstruct ()).s1 = 667;
  OBV_VERITY ((valuebox1->abstruct ()).s1 != (valuebox2->abstruct ()).s1 );
  (valuebox2->abstruct ()).s2 = 1667;
  OBV_VERITY ((valuebox1->abstruct ()).s2 != (valuebox2->abstruct ()).s2 );

  Fixed_Struct1 *fixed_struct_b = 0;
  ACE_NEW_RETURN (fixed_struct_b,
                  Fixed_Struct1,
                  1);
  fixed_struct_b->l = 7372;
  fixed_struct_b->abstruct.s1 = 11;
  fixed_struct_b->abstruct.s2 = 51;

  // Make another VBfixed_struct1
  VBfixed_struct1 *valuebox3 = 0;
  ACE_NEW_RETURN (valuebox3,
                  VBfixed_struct1 (),
                  1);

  // Test assignment operator
  *valuebox3 = *fixed_struct_b;

  OBV_VERITY (valuebox3->l () == fixed_struct_b->l);
  OBV_VERITY ((valuebox3->abstruct ()).s1 == fixed_struct_b->abstruct.s1);
  OBV_VERITY ((valuebox3->abstruct ()).s2 == fixed_struct_b->abstruct.s2);

  // Test _value modifier method
  valuebox2->_value (*fixed_struct_b);
  OBV_VERITY (valuebox2->l () == fixed_struct_b->l);
  OBV_VERITY ((valuebox2->abstruct ()).s1 == fixed_struct_b->abstruct.s1);
  OBV_VERITY ((valuebox2->abstruct ()).s2 == fixed_struct_b->abstruct.s2);

  // Test _copy_value and _downcast
  VBfixed_struct1_var valuebox4 =
    VBfixed_struct1::_downcast (valuebox3->_copy_value ());
  if (valuebox4.in () == 0)
    {
      fail++;
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Failure at line %l\n"));
    }
  else
    {
      OBV_VERITY (valuebox4->l () == fixed_struct_b->l);
      OBV_VERITY ((valuebox4->abstruct ()).s1 == fixed_struct_b->abstruct.s1);
      OBV_VERITY ((valuebox4->abstruct ()).s2 == fixed_struct_b->abstruct.s2);
    }

  //
  // valuebox1 and valuebox3 must be explicitly removed.
  CORBA::remove_ref (valuebox1);
  CORBA::remove_ref (valuebox3);

  //
  // as well as the structs we new'ed.
  delete fixed_struct_a;
  delete fixed_struct_b;

  //
  // Other types are _var so their dtors will clean up remaining
  // allocations.

  return fail;
}



int test_boxed_struct_invocations (Test * test_object)
{
  int fail = 0;

  try
    {

      //============================================================
      // Fixed struct
      // Test method invocation with boxed value
      //============================================================
      Fixed_Struct1 fs1;
      fs1.l = 29;
      fs1.abstruct.s1 = 117;
      fs1.abstruct.s2 = 21;

      VBfixed_struct1 *p1 = 0;
      ACE_NEW_RETURN (p1,
                      VBfixed_struct1(fs1),
                      1);

      Fixed_Struct1 fs2;
      fs2.l = 92;
      fs2.abstruct.s1 = 171;
      fs2.abstruct.s2 = 12;

      VBfixed_struct1 *p2 = 0;
      ACE_NEW_RETURN (p2,
                      VBfixed_struct1(fs2),
                      1);

      VBfixed_struct1 *p3;

      OBV_VERITY (p1->l() == 29);
      OBV_VERITY ((p1->abstruct()).s1 ==  117);
      OBV_VERITY ((p1->abstruct()).s2 ==  21);

      VBfixed_struct1_var result = test_object->struct_op1(p1, p2, p3);

      OBV_VERITY (p2->l() == 92*3);
      OBV_VERITY ((p2->abstruct()).s1 == 171*3);
      OBV_VERITY ((p2->abstruct()).s2 == 12*3);

      OBV_VERITY (p3->l() == 92*5);
      OBV_VERITY ((p3->abstruct()).s1 == 171*5);
      OBV_VERITY ((p3->abstruct()).s2 == 12*5);

      OBV_VERITY (result->l() == fs1.l);
      OBV_VERITY ((result->abstruct()).s1 == fs1.abstruct.s1);
      OBV_VERITY ((result->abstruct()).s2 == fs1.abstruct.s2);

      //============================================================
      // Fixed struct
      // Test _boxed_in(), _boxed_inout(), and _boxed_out())
      //============================================================

      test_object->struct_op2(p1->_boxed_in(), p2->_boxed_inout(),
                              p3->_boxed_out());

      OBV_VERITY (p2->l() == 92*3*3);
      OBV_VERITY ((p2->abstruct()).s1 == 171*3*3);
      OBV_VERITY ((p2->abstruct()).s2 == 12*3*3);

      OBV_VERITY (p3->l() == fs1.l);
      OBV_VERITY ((p3->abstruct()).s1 == fs1.abstruct.s1);
      OBV_VERITY ((p3->abstruct()).s2 == fs1.abstruct.s2);


      p1->_remove_ref ();
      p2->_remove_ref ();
      p3->_remove_ref ();


      //============================================================
      // Variable struct
      // Test method invocation with boxed value
      //============================================================

      Variable_Struct1 vs1;
      vs1.l = 29;
      vs1.str = CORBA::string_dup ("variable1");

      VBvariable_struct1 *p4 = 0;
      ACE_NEW_RETURN (p4,
                      VBvariable_struct1 (vs1),
                      1);

      Variable_Struct1 vs2;
      vs2.l = 37;
      vs2.str = "variable2";

      VBvariable_struct1 *p5 = 0;
      ACE_NEW_RETURN (p5,
                      VBvariable_struct1 (vs2),
                      1);

      VBvariable_struct1 *p6;


      OBV_VERITY (p4->l() == 29);
      OBV_VERITY (ACE_OS::strcmp(p4->str(), "variable1") == 0);

      VBvariable_struct1_var result2 = test_object->struct_op3(p4, p5, p6);

      OBV_VERITY (p5->l() == vs2.l*3);
      OBV_VERITY (ACE_OS::strcmp(p5->str(), "2variable") == 0);

      OBV_VERITY (p6->l() == vs2.l*3);
      OBV_VERITY (ACE_OS::strcmp(p6->str(), "2variable") == 0);

      OBV_VERITY (result2->l() == vs1.l);
      OBV_VERITY (ACE_OS::strcmp(result2->str(), vs1.str) == 0);


      //============================================================
      // Variable struct
      // Test _boxed_in(), _boxed_inout(), and _boxed_out())
      //============================================================

      test_object->struct_op4(p4->_boxed_in(), p5->_boxed_inout(),
                              p6->_boxed_out());

      OBV_VERITY (p5->l() == vs2.l*3*3);
      OBV_VERITY (ACE_OS::strcmp(p5->str(), "e2variabl") == 0);

      OBV_VERITY (p6->l() == vs1.l);
      OBV_VERITY (ACE_OS::strcmp(p6->str(), vs1.str) == 0);


      p4->_remove_ref ();
      p5->_remove_ref ();
      p6->_remove_ref ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_boxed_struct_invocations");
      fail = 1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) test_boxed_struct_invocations: "
                  "caught a C++ exception\n"));
      fail = 1;
    }

  return fail;
}

//
// Test boxed array types.
//
int test_boxed_array()
{
  int fail = 0;
  LongArray la;
  la[0] = 101;
  la[1] = 202;
  la[2] = 303;

  // three ctors
  VBlongarray *valuebox1 = 0;
  ACE_NEW_RETURN (valuebox1,
                  VBlongarray,
                  1);
  VBlongarray *valuebox2 = 0;
  ACE_NEW_RETURN (valuebox2,
                  VBlongarray(la),
                  1);
  VBlongarray *valuebox3 = 0;
  ACE_NEW_RETURN (valuebox3,
                  VBlongarray(*valuebox2),
                  1);

  OBV_VERITY ((*valuebox2)[0] == 101
          && valuebox2->_value()[1] == 202
          && valuebox2->_value()[2] == 303);

  OBV_VERITY ((*valuebox3)[0] == 101
          && (*valuebox3)[1] == 202
          && (*valuebox3)[2] == 303);

  (*valuebox3)[0] = 111;
  valuebox3->_value()[1] = 222;

  OBV_VERITY ((*valuebox2)[0] == 101
          && (*valuebox2)[1] == 202
          && (*valuebox2)[2] == 303);

  OBV_VERITY ((*valuebox3)[0] == 111
          && (*valuebox3)[1] == 222
          && (*valuebox3)[2] == 303);

  *valuebox1 = la;

  OBV_VERITY ((*valuebox1)[0] == 101
          && valuebox1->_value()[1] == 202
          && valuebox1->_value()[2] == 303);

  valuebox2->_value(la);

  OBV_VERITY ((*valuebox2)[0] == 101
          && valuebox2->_value()[1] == 202
          && valuebox2->_value()[2] == 303);

  LongArray_var lv_la(LongArray_dup(la));
  *valuebox2 = lv_la.in();

  *valuebox2 = valuebox3->_value();
  valuebox3->_value()[1] = 777;
  OBV_VERITY ((*valuebox2)[0] == 111
          && valuebox2->_value()[1] == 222
          && valuebox2->_value()[2] == 303);

  // release
  valuebox1->_remove_ref ();
  valuebox2->_remove_ref ();
  valuebox3->_remove_ref ();

  return fail;
}



int test_boxed_array_invocations (Test * test_object)
{
  int fail = 0;

  try
    {
      //============================================================
      // Array (fixed)
      // Test method invocation with boxed value
      //============================================================

      LongArray la;
      la[0] = 101;
      la[1] = 202;
      la[2] = 303;

      VBlongarray *p1 = 0;
      ACE_NEW_RETURN (p1,
                      VBlongarray (la),
                      1);

      LongArray la2;
      la2[0] = 3101;
      la2[1] = 3202;
      la2[2] = 3303;

      VBlongarray *p2 = 0;
      ACE_NEW_RETURN (p2,
                      VBlongarray (la2),
                      1);

      OBV_VERITY ((*p1)[0] == 101
              && (*p1)[1] == 202
              && (*p1)[2] == 303);

      VBlongarray *p3;

      VBlongarray_var result = test_object->array_op1 (p1, p2, p3);

      OBV_VERITY ((*p2)[0] == (3101*3)
              && (*p2)[1] == (3202*3)
              && (*p3)[2] == (3303*3));

      OBV_VERITY ((*p3)[0] == (3101*3)
              && (*p3)[1] == (3202*3)
              && (*p3)[2] == (3303*3));

      OBV_VERITY ((*result.in ())[0] == 101
              && (*result.in ())[1] == 202
              && (*result.in ())[2] == 303);

      //============================================================
      // Array (fixed)
      // Test _boxed_in(), _boxed_inout(), and _boxed_out())
      //============================================================

// Exclude the following test for now until issues with _boxed_out()
// for arrays are resolved.
#if 0
      test_object->array_op2(p1->_boxed_in(), p2->_boxed_inout(),
                              p3->_boxed_out());

      OBV_VERITY ((*p2)[0] == (3101*3*3)
              && (*p2)[1] == (3202*3*3)
              && (*p2)[2] == (3303*3*3));

      OBV_VERITY ((*p3)[0] == (*p1)[0]
              && (*p3)[1] == (*p1)[1]
              && (*p3)[2] == (*p1)[2]);
#endif

      p1->_remove_ref ();
      p2->_remove_ref ();
      p3->_remove_ref ();

      //============================================================
      // Array (variable)
      // Test method invocation with boxed value
      //============================================================

      StringArray sa;
      sa[0] = CORBA::string_dup ("in string1");
      sa[1] = CORBA::string_dup ("in string2");

      VBstringarray *p4 = 0;
      ACE_NEW_RETURN (p4,
                      VBstringarray (sa),
                      1);

      StringArray sa2;
      sa2[0] = CORBA::string_dup ("inout string1");
      sa2[1] = CORBA::string_dup ("inout string2");

      VBstringarray *p5 = 0;
      ACE_NEW_RETURN (p5,
                      VBstringarray (sa2),
                      1);

      OBV_VERITY (ACE_OS::strcmp((*p4)[0], sa[0]) == 0);
      OBV_VERITY (ACE_OS::strcmp((*p4)[1], sa[1]) == 0);

      VBstringarray *p6;

      VBstringarray_var result2 = test_object->array_op3 (p4, p5, p6);

      OBV_VERITY (ACE_OS::strcmp((*p5)[0], "1inout string") == 0);
      OBV_VERITY (ACE_OS::strcmp((*p5)[1], "2inout string") == 0);
      OBV_VERITY (ACE_OS::strcmp((*p6)[0], "1inout string") == 0);
      OBV_VERITY (ACE_OS::strcmp((*p6)[1], "2inout string") == 0);
      OBV_VERITY (ACE_OS::strcmp((*result2.in ())[0], sa[0]) == 0);
      OBV_VERITY (ACE_OS::strcmp((*result2.in ())[1], sa[1]) == 0);

      //============================================================
      // Array (variable)
      // Test _boxed_in(), _boxed_inout(), and _boxed_out())
      //============================================================

// Disable the following for now.  Need to troubleshoot it.
#if 0
// Following gets compilation error on parameter 3
//         test_object->array_op4(p4->_boxed_in(), p5->_boxed_inout(),
//                                p6->_boxed_out());

// Trying the following variation to troubleshoot.  No compilation error
// but p6 is unchanged after return from method.
      StringArray sa_experimental;
      StringArray_slice *slice = p6->_boxed_out();
      StringArray_out an_out (slice);

      test_object->array_op4(p4->_boxed_in(), p5->_boxed_inout(),
                              an_out);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) after array_op4\n"));
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) p5[0]=%s\n", (const char *)((*p5)[0])));
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) p5[1]=%s\n", (const char *)((*p5)[1])));
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) p6[0]=%s\n", (const char *)((*p6)[0])));
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) p6[1]=%s\n", (const char *)((*p6)[1])));
      OBV_VERITY (ACE_OS::strcmp((*p5)[0], "g1inout strin") == 0);
      OBV_VERITY (ACE_OS::strcmp((*p5)[1], "g2inout strin") == 0);
      OBV_VERITY (ACE_OS::strcmp((*p6)[0],  sa[0]) == 0);
      OBV_VERITY (ACE_OS::strcmp((*p6)[1],  sa[1]) == 0);
#endif

      p4->_remove_ref ();
      p5->_remove_ref ();
      p6->_remove_ref ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_boxed_array_invocations");
      fail = 1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) test_boxed_array_invocations: "
                  "caught a C++ exception\n"));
      fail = 1;
    }

  return fail;
}


//
// Test a boxed union type.
//
int test_boxed_union()
{
  int fail = 0;

  VBfixed_union1 *ptemp;
  ACE_NEW_RETURN (ptemp,
                  VBfixed_union1 (),
                  1);
  VBfixed_union1_var valuebox1(ptemp);


  Fixed_Union1 *ptemp2;
  ACE_NEW_RETURN (ptemp2,
                  Fixed_Union1 (),
                  1);
  Fixed_Union1_var fixed_union1(ptemp2);

  // Test modifiers, accessors, discriminant access
  valuebox1->m1 (37);
  OBV_VERITY (valuebox1->m1 () == 37);
  OBV_VERITY (valuebox1->_d () == 1 || valuebox1->_d () == 2);

  // Explicitly set discriminant, make sure thats the only thing
  // that changes.
  valuebox1->_d (2);
  OBV_VERITY (valuebox1->_d () == 2);
  OBV_VERITY (valuebox1->m1 () == 37);
  valuebox1->_d (1);
  OBV_VERITY (valuebox1->_d () == 1);
  OBV_VERITY (valuebox1->m1 () == 37);

  // Use _value() to access
  valuebox1->_value ()._d (2);
  OBV_VERITY (valuebox1->_d () == 2);

  // Use _value as modifier.
  valuebox1->_value (fixed_union1.in());
  OBV_VERITY (valuebox1->_d () != 1 && valuebox1->_d () != 2);

  //
  VBfixed_union1* valuebox2_ptr = 0;
  ACE_NEW_RETURN (valuebox2_ptr,
                  VBfixed_union1 (),
                  1);
  VBfixed_union1_var valuebox2 (valuebox2_ptr);
  valuebox2->m2(333);
  OBV_VERITY (valuebox2->_d () == 2);

  // Test copy ctor
  VBfixed_union1* valuebox3_ptr = 0;
  ACE_NEW_RETURN (valuebox3_ptr,
                  VBfixed_union1 (*valuebox2.in ()),
                  1);
  VBfixed_union1_var valuebox3 (valuebox3_ptr);
  OBV_VERITY (valuebox3->_d () == 2);
  OBV_VERITY (valuebox3->m2 () == 333);

  // Test assignment op
  valuebox3->m2 (456);
  *valuebox3.in () = valuebox2->_value ();
  OBV_VERITY (valuebox3->_d () == 2);
  OBV_VERITY (valuebox3->m2 () == 333);

  // Test constructor taking union argument
  fixed_union1->m2 (137);
  VBfixed_union1 *valuebox4_ptr = 0;
  ACE_NEW_RETURN (valuebox4_ptr,
                  VBfixed_union1 (fixed_union1.in ()),
                  1);
  VBfixed_union1_var valuebox4 (valuebox4_ptr);
  OBV_VERITY (valuebox4->m2 () == 137);
  OBV_VERITY (valuebox4->_d () == 1 || valuebox4->_d () == 2);

  return fail;
}




int test_boxed_union_invocations (Test * test_object)
{
  int fail = 0;

  try
    {
      //============================================================
      // Union (fixed)
      // Test method invocation with boxed value
      //============================================================

      Fixed_Union1 *ptemp = 0;
      ACE_NEW_RETURN (ptemp,
                      Fixed_Union1 (),
                      1);
      Fixed_Union1_var fixed_union1(ptemp);

      fixed_union1->m1 (321);
      VBfixed_union1 *p1 = 0;
      ACE_NEW_RETURN (p1,
                      VBfixed_union1 (fixed_union1.in ()),
                      1);

      Fixed_Union1 *ptemp2 = 0;
      ACE_NEW_RETURN (ptemp2,
                      Fixed_Union1 (),
                      1);
      Fixed_Union1_var fixed_union2(ptemp2);
      fixed_union2->m2 (789);
      VBfixed_union1 *p2 = 0;
      ACE_NEW_RETURN (p2,
                      VBfixed_union1 (fixed_union2.in ()),
                      1);

      OBV_VERITY (p1->_d () == 1);
      OBV_VERITY (p1->m1 () == 321);
      OBV_VERITY (p2->_d () == 2);
      OBV_VERITY (p2->m2 () == 789);

      VBfixed_union1 * p3;

      VBfixed_union1_var result = test_object->union_op1 (p1, p2, p3);

      OBV_VERITY (p2->_d () == 2);
      OBV_VERITY (p2->m2 () == 789*3);
      OBV_VERITY (p3->_d () == 1);
      OBV_VERITY (p3->m1 () == 321*3);
      OBV_VERITY (result->_d () == 1);
      OBV_VERITY (result->m1 () == 321*3);


      //============================================================
      // Union (fixed)
      // Test _boxed_in(), _boxed_inout(), and _boxed_out())
      //============================================================

      test_object->union_op2(p1->_boxed_in(), p2->_boxed_inout(),
                              p3->_boxed_out());

      OBV_VERITY (p2->_d () == 2);
      OBV_VERITY (p2->m2 () == 789*3*3);

      OBV_VERITY (p3->_d () == 1);
      OBV_VERITY (p3->m1 () == 321);

      p1->_remove_ref ();
      p2->_remove_ref ();
      p3->_remove_ref ();

      //============================================================
      // Union (variable)
      // Test method invocation with boxed value
      //============================================================

      Variable_Union1_var variable_union1;
      ACE_NEW_RETURN (variable_union1,
                      Variable_Union1 (),
                      1);
      variable_union1->m1 (321);
      // TODO : resource leak
      VBvariable_union1 *p4 = 0;
      ACE_NEW_RETURN (p4,
                      VBvariable_union1 (variable_union1.in ()),
                      1);

      Variable_Union1_var variable_union2;
      ACE_NEW_RETURN (variable_union2,
                      Variable_Union1 (),
                      1);
      variable_union2->m2 (CORBA::string_dup ("abracadabra"));
      VBvariable_union1 *p5 = 0;
      ACE_NEW_RETURN (p5,
                      VBvariable_union1 (variable_union2.in ()),
                      1);

      OBV_VERITY (p4->_d () == 1);
      OBV_VERITY (p4->m1 () == 321);
      OBV_VERITY (p5->_d () == 2);
      OBV_VERITY (ACE_OS::strcmp(p5->m2 (), "abracadabra") == 0);

      VBvariable_union1 * p6;

      VBvariable_union1_var result2 = test_object->union_op3 (p4, p5, p6);

      OBV_VERITY (p5->_d () == 2);
      OBV_VERITY (ACE_OS::strcmp(p5->m2 (), "aabracadabr") == 0);
      OBV_VERITY (p6->_d () == 1);
      OBV_VERITY (p6->m1 () == 321);
      OBV_VERITY (result2->_d () == 1);
      OBV_VERITY (result2->m1 () == 321);

      //============================================================
      // Union (variable)
      // Test _boxed_in(), _boxed_inout(), and _boxed_out())
      //============================================================

      p4->m1 (1722);

      test_object->union_op4(p4->_boxed_in(), p5->_boxed_inout(),
                              p6->_boxed_out());

      OBV_VERITY (p5->_d () == 2);
      OBV_VERITY (ACE_OS::strcmp(p5->m2 (), "raabracadab") == 0);

      OBV_VERITY (p6->_d () == 1);
      OBV_VERITY (p6->m1 () == 1722);

      p4->_remove_ref ();
      p5->_remove_ref ();
      p6->_remove_ref ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_boxed_union_invocations");
      fail = 1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) test_boxed_union_invocations: "
                  "caught a C++ exception\n"));
      fail = 1;
    }

  return fail;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Test_var test_object;
  CORBA::ORB_var orb;


  try
    {
      orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      // Obtain reference to the object.
      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      test_object = Test::_narrow(tmp.in ());

      if (CORBA::is_nil (test_object.in ()))
      {
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Nil Test reference <%s>\n",
                           ior),
                          1);
      }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Initialization failure");
      return 1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) Initialization failure: caught a C++ exception\n"));
      return 1;
    }

  int fail = 0;

  fail = test_basic ();

  fail += test_basic_invocations (test_object.in ());

  fail += test_boxed_string ();

  fail += test_boxed_string_invocations (test_object.in ());

  fail += test_boxed_sequence ();

  fail += test_boxed_sequence_invocations (test_object.in ());

  fail += test_boxed_struct ();

  fail += test_boxed_struct_invocations (test_object.in ());

  fail += test_boxed_array ();

  fail += test_boxed_array_invocations (test_object.in ());

  fail += test_boxed_union();

  fail += test_boxed_union_invocations (test_object.in ());


  try
    {
      test_object->shutdown ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - test finished\n"));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return (fail) ?  1 : 0;
}
