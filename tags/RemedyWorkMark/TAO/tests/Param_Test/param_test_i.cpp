// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    param_test_i.cpp
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "param_test_i.h"

#include "tao/debug.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_wchar.h"
#include "ace/OS_NS_string.h"

// ********* class Coffee_i ****************
// Constructor

Coffee_i::Coffee_i (const char *name)
  : name_ (name)
{
}

// Destructor

Coffee_i::~Coffee_i (void)
{
}

// get attribute
Coffee::Desc *
Coffee_i::description ( /*env*/)
{
  Coffee::Desc *desc = new Coffee::Desc;
  desc->name = CORBA::string_dup (this->name_.in ());
  return desc;
}

// set attribute
void
Coffee_i::description (const Coffee::Desc &description
 /*env*/)
{
  this->name_ = CORBA::string_dup (description.name);
}


// ********* class Param_Test_i ****************

// Constructor

Param_Test_i::Param_Test_i (const char *coffee_name,
                            CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    obj_ (coffee_name)
{
}

// Destructor

Param_Test_i::~Param_Test_i (void)
{
}

// test shorts
CORBA::Short
Param_Test_i::test_short (CORBA::Short s1,
                          CORBA::Short &s2,
                          CORBA::Short_out s3)
{
  s2 = (CORBA::Short) (s1 * 2);
  s3 = (CORBA::Short) (s1 * 3);
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*SERVER SIDE=*=*=*=*=*=*=\n"));
      ACE_DEBUG ((LM_DEBUG, " in = %d, inout = %d, out = %d\n",
                  s1, s2, s3));
    }
  return (CORBA::Short) (s1 * 4);
}

// test long long
CORBA::ULongLong
Param_Test_i::test_ulonglong (CORBA::ULongLong s1,
                              CORBA::ULongLong &s2,
                              CORBA::ULongLong_out s3)
{
  s2 = s1 * 2;
  s3 = s1 * 3;
  return s1 * 4;
}

// test unbounded strings. For return and out types, we return duplicates of
// the in string. For the inout, we append the same string to itself and send
// it back
char *
Param_Test_i::test_unbounded_string (const char *s1,
                                     char *&s2,
                                     CORBA::String_out s3)
{
  char *retstr = CORBA::string_dup (s1);
  s3 = CORBA::string_dup (s1);
  char *tmp = CORBA::string_alloc (2*ACE_OS::strlen (s2));
  ACE_OS::sprintf (tmp, "%s%s", s2, s2);
  CORBA::string_free (s2);
  s2 = tmp;
  return retstr;
}

// test bounded strings. For return and out types, we return duplicates of
// the in string. For the inout, we append the same string to itself and send
// it back
char *
Param_Test_i::test_bounded_string (const char *s1,
                                   char *&s2,
                                   CORBA::String_out s3)
{
  char *retstr = CORBA::string_dup (s1);
  s3 = CORBA::string_dup (s1);
  char *tmp = CORBA::string_alloc (2*ACE_OS::strlen (s2));
  ACE_OS::sprintf (tmp, "%s%s", s2, s2);
  CORBA::string_free (s2);
  s2 = tmp;
  return retstr;
}

// test unbounded strings. For return and out types, we return duplicates of
// the in string. For the inout, we append the same string to itself and send
// it back
CORBA::WChar *
Param_Test_i::test_unbounded_wstring (const CORBA::WChar *ws1,
                                      CORBA::WChar *&ws2,
                                      CORBA::WString_out ws3)
{
  CORBA::WChar *retwstr = CORBA::wstring_dup (ws1);
  ws3 = CORBA::wstring_dup (ws1);
  CORBA::ULong len = ACE_OS::wslen (ws2);
  CORBA::WChar *tmp = CORBA::wstring_alloc (2*len);
  for (CORBA::ULong i = 0; i < 2; i++)
    for (CORBA::ULong j = 0; j < len; j++)
      tmp[j + i*len] = ws2[j];
  tmp[2*len] = 0;
  CORBA::wstring_free (ws2);
  ws2 = tmp;
  return retwstr;
}

// test bounded strings. For return and out types, we return duplicates of
// the in string. For the inout, we append the same string to itself and send
// it back
CORBA::WChar *
Param_Test_i::test_bounded_wstring (const CORBA::WChar *ws1,
                                    CORBA::WChar *&ws2,
                                    CORBA::WString_out ws3)
{
  CORBA::WChar *retwstr = CORBA::wstring_dup (ws1);
  ws3 = CORBA::wstring_dup (ws1);
  CORBA::ULong len = ACE_OS::wslen (ws2);
  CORBA::WChar *tmp = CORBA::wstring_alloc (2*len);
  for (CORBA::ULong i = 0; i < 2; i++)
    for (CORBA::ULong j = 0; j < len; j++)
      tmp[j + i*len] = ws2[j];
  tmp[2*len] = 0;
  CORBA::wstring_free (ws2);
  ws2 = tmp;
  return retwstr;
}

// test for fixed structures. Just copy the in parameter into all the others
Param_Test::Fixed_Struct
Param_Test_i::test_fixed_struct (const Param_Test::Fixed_Struct &s1,
                                 Param_Test::Fixed_Struct &s2,
                                 Param_Test::Fixed_Struct_out s3)
{
  s2 = s1;
  s3 = s1;
  return s1;
}

// = Sequences

CORBA::LongSeq *
Param_Test_i::test_long_sequence (const CORBA::LongSeq & s1,
                                  CORBA::LongSeq & s2,
                                  CORBA::LongSeq_out s3)
{
  CORBA::LongSeq
    *ret = new CORBA::LongSeq,
    *out = new CORBA::LongSeq;

  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

CORBA::ShortSeq *
Param_Test_i::test_short_sequence (const CORBA::ShortSeq & s1,
                                   CORBA::ShortSeq & s2,
                                   CORBA::ShortSeq_out s3)
{
  CORBA::ShortSeq
    *ret = new CORBA::ShortSeq,
    *out = new CORBA::ShortSeq;

  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

Param_Test::Bounded_Short_Seq *
Param_Test_i::test_bounded_short_sequence (const Param_Test::Bounded_Short_Seq & s1,
                                           Param_Test::Bounded_Short_Seq & s2,
                                           Param_Test::Bounded_Short_Seq_out s3)
{
  Param_Test::Bounded_Short_Seq
    *ret = new Param_Test::Bounded_Short_Seq,
    *out = new Param_Test::Bounded_Short_Seq;

  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

Param_Test::Bounded_Long_Seq *
Param_Test_i::test_bounded_long_sequence (const Param_Test::Bounded_Long_Seq & s1,
                                          Param_Test::Bounded_Long_Seq & s2,
                                          Param_Test::Bounded_Long_Seq_out s3)
{
  Param_Test::Bounded_Long_Seq
    *ret = new Param_Test::Bounded_Long_Seq,
    *out = new Param_Test::Bounded_Long_Seq;

  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

CORBA::StringSeq *
Param_Test_i::test_strseq (const CORBA::StringSeq &s1,
                           CORBA::StringSeq &s2,
                           CORBA::StringSeq_out s3)
{
  // we copy the "in" sequences into all the inout, out and return sequences.

  CORBA::StringSeq
    *ret = new CORBA::StringSeq,
    *out = new CORBA::StringSeq;

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*SERVER SIDE=*=*=*=*=*=*=\n"));
      for (CORBA::ULong i=0; (i < s2.length ()); i++)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Element #%d\n"
                      "in : %s\n",
                      i,
                      (s2[i]? (const char *)s2[i]:"<nul>")));
        }
      if (s2.length () == 0)
        ACE_DEBUG ((LM_DEBUG, "\ninout sequence is NUL\n"));
    }

  // now copy all elements of s1 into the others using the assignment operator
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

Param_Test::Bounded_StrSeq *
Param_Test_i::test_bounded_strseq (const Param_Test::Bounded_StrSeq & s1,
                                   Param_Test::Bounded_StrSeq & s2,
                                   Param_Test::Bounded_StrSeq_out s3)
{
  // we copy the "in" sequences into all the inout, out and return sequences.

  Param_Test::Bounded_StrSeq
    *ret = new Param_Test::Bounded_StrSeq,
    *out = new Param_Test::Bounded_StrSeq;

  // now copy all elements of s1 into the others using the assignment operator
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

CORBA::WStringSeq *
Param_Test_i::test_wstrseq (const CORBA::WStringSeq &ws1,
                            CORBA::WStringSeq &ws2,
                            CORBA::WStringSeq_out ws3)
{
  // we copy the "in" sequences into all the inout, out and return sequences.

  CORBA::WStringSeq
    *ret = new CORBA::WStringSeq,
    *out = new CORBA::WStringSeq;

  // now copy all elements of s1 into the others using the assignment operator
  ws2 = ws1;
  *out = ws1;
  *ret = ws1;
  ws3 = out;
  return ret;
}

Param_Test::Bounded_WStrSeq *
Param_Test_i::test_bounded_wstrseq (const Param_Test::Bounded_WStrSeq & ws1,
                                    Param_Test::Bounded_WStrSeq & ws2,
                                    Param_Test::Bounded_WStrSeq_out ws3)
{
  // we copy the "in" sequences into all the inout, out and return sequences.

  Param_Test::Bounded_WStrSeq
    *ret = new Param_Test::Bounded_WStrSeq,
    *out = new Param_Test::Bounded_WStrSeq;

  // now copy all elements of s1 into the others using the assignment operator
  ws2 = ws1;
  *out = ws1;
  *ret = ws1;
  ws3 = out;
  return ret;
}

// test for struct sequences
Param_Test::StructSeq *
Param_Test_i::test_struct_sequence (const Param_Test::StructSeq &s1,
                                    Param_Test::StructSeq &s2,
                                    Param_Test::StructSeq_out s3)
{
  // we copy the "in" sequences into all the inout, out and return sequences.

  Param_Test::StructSeq
    *ret = new Param_Test::StructSeq,

    *out = new Param_Test::StructSeq;

  // now copy all elements of s1 into the others using the assignment operator
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

// test for bounded struct sequences
Param_Test::Bounded_StructSeq *
Param_Test_i::test_bounded_struct_sequence (const Param_Test::Bounded_StructSeq & s1,
                                            Param_Test::Bounded_StructSeq & s2,
                                            Param_Test::Bounded_StructSeq_out s3)
{
  Param_Test::Bounded_StructSeq
    *ret = new Param_Test::Bounded_StructSeq,
    *out = new Param_Test::Bounded_StructSeq;

  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}


Param_Test::PathSpec *
Param_Test_i::test_unbounded_struct_sequence (const Param_Test::PathSpec & s1,
                                              Param_Test::PathSpec & s2,
                                              Param_Test::PathSpec_out s3)
{
  Param_Test::PathSpec
    *ret = new Param_Test::PathSpec,
    *out = new Param_Test::PathSpec;


  Param_Test::PathSpec_var rPathSpec = new Param_Test::PathSpec;
  rPathSpec->length(2);

  rPathSpec[0u].name.id = CORBA::string_dup("staff");
  rPathSpec[0u].name.kind = CORBA::string_dup("staff");
  rPathSpec[0u].process = 1;

  rPathSpec[1u].name.id = CORBA::string_dup("john");
  rPathSpec[1u].name.kind = CORBA::string_dup("john");
  rPathSpec[1u].process = 1;

  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;

  return ret;
}


// test for array sequences
Param_Test::ArraySeq *
Param_Test_i::test_array_sequence (const Param_Test::ArraySeq &s1,
                                   Param_Test::ArraySeq &s2,
                                   Param_Test::ArraySeq_out s3)
{
  // we copy the "in" sequences into all the inout, out and return sequences.

  Param_Test::ArraySeq
    *ret = new Param_Test::ArraySeq,

    *out = new Param_Test::ArraySeq;

  // now copy all elements of s1 into the others using the assignment operator
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

// test for bounded array sequences
Param_Test::Bounded_ArraySeq *
Param_Test_i::test_bounded_array_sequence (const Param_Test::Bounded_ArraySeq & s1,
                                           Param_Test::Bounded_ArraySeq & s2,
                                           Param_Test::Bounded_ArraySeq_out s3)
{
  Param_Test::Bounded_ArraySeq
    *ret = new Param_Test::Bounded_ArraySeq,
    *out = new Param_Test::Bounded_ArraySeq;

  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

Param_Test::Coffee_Mix *
Param_Test_i::test_coffe_mix (const Param_Test::Coffee_Mix & s1,
                              Param_Test::Coffee_Mix & s2,
                              Param_Test::Coffee_Mix_out s3)
{
  Param_Test::Coffee_Mix
    *ret = new Param_Test::Coffee_Mix,
    *out = new Param_Test::Coffee_Mix;

#if 0
  ACE_DEBUG ((LM_DEBUG,
              "maximum = %d\n"
              "length = %d\n",
              s1.maximum (),
              s1.length ()));
  ACE_DEBUG ((LM_DEBUG, "Elements -\n"));
  for (CORBA::ULong i = 0; i < s1.length (); ++i)
    {
      Coffee_ptr c = s1[i];
      if (CORBA::is_nil (c))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Element #%d is nil\n", i));
          continue;
        }
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "\ttype = <%s>\n",
                  i,
                  c->_interface_repository_id ()));
    }
#endif /* 0 */


  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

Param_Test::Bounded_Coffee_Mix *
Param_Test_i::test_bounded_coffe_mix (const Param_Test::Bounded_Coffee_Mix & s1,
                                      Param_Test::Bounded_Coffee_Mix & s2,
                                      Param_Test::Bounded_Coffee_Mix_out s3)
{
  Param_Test::Bounded_Coffee_Mix
    *ret = new Param_Test::Bounded_Coffee_Mix,
    *out = new Param_Test::Bounded_Coffee_Mix;

#if 0
  ACE_DEBUG ((LM_DEBUG,
              "maximum = %d\n"
              "length = %d\n",
              s1.maximum (),
              s1.length ()));
  ACE_DEBUG ((LM_DEBUG, "Elements -\n"));
  for (CORBA::ULong i = 0; i < s1.length (); ++i)
    {
      Coffee_ptr c = s1[i];
      if (CORBA::is_nil (c))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Element #%d is nil\n", i));
          continue;
        }
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "\ttype = <%s>\n",
                  i,
                  c->_interface_repository_id ()));
    }
#endif /* 0 */


  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

CORBA::AnySeq *
Param_Test_i::test_anyseq (const CORBA::AnySeq &s1,
                           CORBA::AnySeq &s2,
                           CORBA::AnySeq_out s3)
{
  // we copy the "in" sequences into all the inout, out and return sequences.

  CORBA::AnySeq
    *ret = new CORBA::AnySeq,
    *out = new CORBA::AnySeq;

  // now copy all elements of s1 into the others using the assignment operator
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

// = end of sequences...

// test for variable structs
Param_Test::Var_Struct *
Param_Test_i::test_var_struct (const Param_Test::Var_Struct &s1,
                               Param_Test::Var_Struct &s2,
                               Param_Test::Var_Struct_out s3)
{
  // we copy the "in" sequences into all the inout, out and return sequences.

  Param_Test::Var_Struct
    *ret = new Param_Test::Var_Struct,
    *out = new Param_Test::Var_Struct;

  // now copy all elements of s1 into the others
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

// test for nested structs
Param_Test::Nested_Struct *
Param_Test_i::test_nested_struct (const Param_Test::Nested_Struct &s1,
                                  Param_Test::Nested_Struct &s2,
                                  Param_Test::Nested_Struct_out s3)
{
  // we copy the "in" sequences into all the inout, out and return sequences.

  Param_Test::Nested_Struct
    *ret = new Param_Test::Nested_Struct,
    *out = new Param_Test::Nested_Struct;

  // now copy all elements of s1 into the others
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

// test for recursive structs
Param_Test::Recursive_Struct *
Param_Test_i::test_recursive_struct (const Param_Test::Recursive_Struct &s1,
                                     Param_Test::Recursive_Struct &s2,
                                     Param_Test::Recursive_Struct_out s3)
{
  // we copy the "in" structs into all the inout, out and return sequences.

  Param_Test::Recursive_Struct
    *ret = new Param_Test::Recursive_Struct,
    *out = new Param_Test::Recursive_Struct;

  // now copy all elements of s1 into the others
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

Param_Test::Objref_Struct *
Param_Test_i::test_objref_struct (const Param_Test::Objref_Struct &s1,
                                  Param_Test::Objref_Struct &s2,
                                  Param_Test::Objref_Struct_out s3)
{
  // we copy the "in" sequences into all the inout, out and return sequences.

  Param_Test::Objref_Struct
    *ret = new Param_Test::Objref_Struct,
    *out = new Param_Test::Objref_Struct;

  // now copy all elements of s1 into the others
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

// make a Coffee object
Coffee_ptr
Param_Test_i::make_coffee (void)
{
  return this->obj_._this ();
}

// test for object references
Coffee_ptr
Param_Test_i::test_objref (Coffee_ptr o1,
                           Coffee_ptr &o2,
                           Coffee_out o3)
{
  Coffee_ptr ret = Coffee::_nil ();

  try
    {
      Coffee_var myobj = obj_._this ();

      if (!CORBA::is_nil (o2))
        CORBA::release (o2);

      CORBA::Boolean equiv = myobj->_is_equivalent (o1);

      if (equiv)
        {
          o2 = Coffee::_duplicate (myobj.in ());
          o3 = Coffee::_duplicate (myobj.in ());
          ret = Coffee::_duplicate (myobj.in ());
        }
      else
        {
          o2 = Coffee::_nil ();
          o3 = Coffee::_nil ();
        }
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
      // env.exception (TAO_TRY_ENV.exception ());
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("User Exception");
      // env.exception (TAO_TRY_ENV.exception ());
    }

  return ret;
}

// test for typecodes
CORBA::TypeCode_ptr
Param_Test_i::test_typecode (CORBA::TypeCode_ptr t1,
                             CORBA::TypeCode_ptr &t2,
                             CORBA::TypeCode_out t3
/*env*/)
{
  // we simply assign t1 to the others
  CORBA::TypeCode_ptr retval = CORBA::TypeCode::_duplicate (t1);
  t2 = CORBA::TypeCode::_duplicate (t1);
  t3 = CORBA::TypeCode::_duplicate (t1);
  return retval;
}

// test for Anys
CORBA::Any *
Param_Test_i::test_any (const CORBA::Any &a1,
                        CORBA::Any &a2,
                        CORBA::Any_out a3)
{
  CORBA::Any *ret;
  CORBA::Short short_in;
  const char *str_in;
  Coffee_ptr coffee;
  Param_Test::Fixed_Array_forany array;
  CORBA::ShortSeq *ub_short_sequence;
  Param_Test::Bounded_Short_Seq *bd_short_sequence;
  Param_Test::Fixed_Struct *fixed_structure;
  Param_Test::Big_Union *big_union;
  Param_Test::Small_Union *small_union;

  a2 = a1;
  a3 = new CORBA::Any (a1);
  ret = new CORBA::Any (a1);


  if (TAO_debug_level > 0)
    {
      CORBA::TypeCode_var tc = a1.type ();
      int kind = tc->kind ();

      ACE_DEBUG ((LM_DEBUG,
                  "Received any contents are <%d>\n",
                  kind));
    }

  // debug the incoming Any
  if (a1 >>= short_in)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Received short = %d\n", short_in));
      a2 >>= short_in;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "inout short = %d\n", short_in));
      *a3.ptr () >>= short_in;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "out short = %d\n", short_in));
      *ret >>= short_in;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "ret short = %d\n", short_in));
    }
  else if (a1 >>= str_in)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Received unbounded string = %s\n", str_in));
    }
  else if (a1 >>= coffee)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Received Coffee object\n"));
    }
  else if (a1 >>= array)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Received Fixed_Array:"));
          for (CORBA::ULong i = 0; i < Param_Test::DIM1; i++)
            ACE_DEBUG ((LM_DEBUG, " %d", array[i]));
          ACE_DEBUG ((LM_DEBUG, "\n"));
        }
      for (CORBA::ULong i = 0; i < Param_Test::DIM1; i++)
        array[i] = i * i;
      a2 <<= Param_Test::Fixed_Array_forany (array);
      *ret <<= Param_Test::Fixed_Array_forany (array);
    }
  else if (a1 >>= ub_short_sequence)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Received Unbounded Short_Seq:"));
          for (size_t i = 0; i < ub_short_sequence->length (); i++)
            ACE_DEBUG ((LM_DEBUG, " %d", (*ub_short_sequence)[i]));
          ACE_DEBUG ((LM_DEBUG, "\n"));
        }
      for (size_t i = 0; i < ub_short_sequence->length (); i++)
        (*ub_short_sequence)[i] = (CORBA::Short) (i * i);
      a2   <<= *ub_short_sequence;
      *ret <<= *ub_short_sequence;
    }
  else if (a1 >>= bd_short_sequence)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Received Bounded_Short_Seq:"));
          for (size_t i = 0; i < bd_short_sequence->length (); i++)
            ACE_DEBUG ((LM_DEBUG, " %d", (*bd_short_sequence)[i]));
          ACE_DEBUG ((LM_DEBUG, "\n"));
        }
      for (size_t i = 0; i < bd_short_sequence->length (); i++)
        (*bd_short_sequence)[i] = (CORBA::Short) (i * i);
      a2 <<= *bd_short_sequence;
      *ret <<= *bd_short_sequence;
    }
  else if (a1 >>= fixed_structure)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Received Fixed_Struct\n"));
    }
  else if (a1 >>= big_union)
    {
      Param_Test::Big_Union *bu_in, *bu_inout, *bu_out, *bu_ret;
      a1 >>= bu_in;

      // Insert copies....
      a2 <<= *bu_in;
      *a3 <<= *bu_in;
      *ret <<= *bu_in;

      // Extract the value to compare...
      a2 >>= bu_inout;
      *a3 >>= bu_out;
      *ret >>= bu_ret;

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Received Big Union\n"
                    "  in %d\n"
                    "  inout %d\n"
                    "  out %d\n"
                    "  ret %d\n",
                    bu_in->the_long (),
                    bu_inout->the_long (),
                    bu_out->the_long (),
                    bu_ret->the_long () ));
    }
  else if (a1 >>= small_union)
    {
      Param_Test::Small_Union *bu_in, *bu_inout, *bu_out, *bu_ret;
      a1 >>= bu_in;

      // Insert copies....
      a2 <<= *bu_in;
      *a3 <<= *bu_in;
      *ret <<= *bu_in;

      // Extract the value to compare...
      a2 >>= bu_inout;
      *a3 >>= bu_out;
      *ret >>= bu_ret;

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Received Small Union\n"
                    "  in %d\n"
                    "  inout %d\n"
                    "  out %d\n"
                    "  ret %d\n",
                    bu_in->the_long (),
                    bu_inout->the_long (),
                    bu_out->the_long (),
                    bu_ret->the_long () ));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Received UNKNOWN type\n"));
    }

  return ret;
}

// test for fixed arrays
Param_Test::Fixed_Array_slice *
Param_Test_i::test_fixed_array (const Param_Test::Fixed_Array a1,
                                Param_Test::Fixed_Array a2,
                                Param_Test::Fixed_Array_out a3)
{
  Param_Test::Fixed_Array_slice *ret;

  Param_Test::Fixed_Array_copy (a2, a1);
  Param_Test::Fixed_Array_copy (a3, a1);
  ret = Param_Test::Fixed_Array_dup (a1);
  return ret;
}

// test for var arrays
Param_Test::Var_Array_slice *
Param_Test_i::test_var_array (const Param_Test::Var_Array a1,
                              Param_Test::Var_Array a2,
                              Param_Test::Var_Array_out a3)
{
  Param_Test::Var_Array_slice *ret;

  Param_Test::Var_Array_copy (a2, a1);
  a3 = Param_Test::Var_Array_dup (a1);
  ret = Param_Test::Var_Array_dup (a1);
  return ret;
}

CORBA::ULong
Param_Test_i::test_exception (CORBA::ULong s1,
                              CORBA::ULong& s2,
                              CORBA::ULong_out s3)
{
  CORBA::ULong d = s1 % 4;

  // No exceptions to throw.
  if (d == 0)
    {
      s2 = s1 * 2;
      s3 = s1 * 3;
      return s1 * 4;
    }
  // Throw a known user exception type to test the user exception.
  else if (d == 1)
    {
      throw Param_Test::Ooops (" % 4 == 1", d);
    }
  // Throw a CORBA::SystemException type CORBA::NO_MEMORY to test
  // the system exception.
  else if (d == 2)
    {
      throw CORBA::NO_MEMORY ();
    }

  // This will avoid the compiler
  // warning that test_exception is throwing an exception
  // not in its THROW_SPEC, but still test TAO's
  // conversion of such an exception to UNKNOWN.
  this->throw_badboy ();

  return 0;
}

Param_Test::Big_Union*
Param_Test_i::test_big_union (const Param_Test::Big_Union& u1,
                              Param_Test::Big_Union& u2,
                              Param_Test::Big_Union_out u3)
{
  Param_Test::Big_Union_var ret (new Param_Test::Big_Union (u1));
  u2 = u1;
  u3 = new Param_Test::Big_Union (u1);
  return ret._retn ();
}

Param_Test::Small_Union
Param_Test_i::test_small_union (const Param_Test::Small_Union& u1,
                                Param_Test::Small_Union& u2,
                                Param_Test::Small_Union_out u3)
{
  u2 = u1;
  u3 = u1;
  return u1;
}

Param_Test::Recursive_Union*
Param_Test_i::test_recursive_union (const Param_Test::Recursive_Union& ru1,
                                    Param_Test::Recursive_Union& ru2,
                                    Param_Test::Recursive_Union_out ru3)
{
  Param_Test::Recursive_Union_var ret (new Param_Test::Recursive_Union (ru1));
  ru2 = ru1;
  ru3 = new Param_Test::Recursive_Union (ru1);
  return ret._retn ();
}

CORBA::Any*
Param_Test_i::test_complex_any (const CORBA::Any &a1,
                                CORBA::Any &a2,
                                CORBA::Any_out a3)
{
  CORBA::Any_var ret (new CORBA::Any (a1));
  a2 = a1;
  a3 = new CORBA::Any (a1);
  return ret._retn ();
}

Param_Test::Multdim_Array_slice *
Param_Test_i::test_multdim_array (const Param_Test::Multdim_Array a1,
                                  Param_Test::Multdim_Array a2,
                                  Param_Test::Multdim_Array_out a3)
{
  Param_Test::Multdim_Array_slice *ret;

  Param_Test::Multdim_Array_copy (a2, a1);
  Param_Test::Multdim_Array_copy (a3, a1);
  ret = Param_Test::Multdim_Array_dup (a1);
  return ret;
}

void
Param_Test_i::shutdown (void)
{
  this->orb_->shutdown ();
}

void
Param_Test_i::throw_badboy (void)
{
  throw Param_Test::BadBoy ();
}
