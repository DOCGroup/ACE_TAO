// $Id$

#include "Test_impl.h"

Test_impl::Test_impl (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
{
}

//***************************************************************************
// Rotate characters in string, last character becomes the first and
// all others are shifted to the right.
//***************************************************************************
void rotate_string(char *s)
{
   unsigned char c;
   size_t l;

   l = ACE_OS::strlen(s);
   if (l>1)
   {
      c = s[l-1];
      ACE_OS::memmove(&s[1], &s[0], l-1);
      s[0] = c;
   }
}


VBlong *
Test_impl::basic_op1 (VBlong * p1, VBlong *& p2, VBlong_out p3)
{
  ACE_NEW_RETURN (p3,
                  VBlong (p2->_value()*5),
                  0);

  p2->_value (p2->_value() * 3);

  VBlong *retval = 0;
  ACE_NEW_RETURN (retval,
                  VBlong (p1->_value()*3),
                  0);

  return retval;
}

vb_basic::M_VBlong *
Test_impl::basic_op2 (vb_basic::M_VBlong * p1,
                      vb_basic::M_VBlong *& p2,
                      vb_basic::M_VBlong_out p3)
{
  ACE_NEW_RETURN (p3,
                  vb_basic::M_VBlong (p2->_value()*5),
                  0);

  p2->_value (p2->_value() * 3);

  vb_basic::M_VBlong *retval = 0;
  ACE_NEW_RETURN (retval,
                  vb_basic::M_VBlong (p1->_value()*3),
                  0);

  return retval;
}

::CORBA::Long
Test_impl::basic_op3 (::CORBA::Long p1, ::CORBA::Long& p2,
                      ::CORBA::Long_out p3)
{
  p3 = p2 * 5;

  p2 = p2 * 3;

  return p1 * 3;
}


VBstring *
Test_impl::string_op1 (::VBstring * p1,
                       ::VBstring *& p2,
                       ::VBstring_out p3)
{
  rotate_string (p2->_boxed_inout ());

  ACE_NEW_RETURN (p3,
                  VBstring (p2->_value ()),
                  0);

  rotate_string (p1->_boxed_inout ());

  VBstring *retval = 0;
  ACE_NEW_RETURN (retval,
                  VBstring (p1->_value ()),
                  0);

  return retval;
}

char *
Test_impl::string_op2 (const char * p1,
                       char *& p2,
                       CORBA::String_out p3)
{
  rotate_string (p2);

  p3 = CORBA::string_alloc (ACE_OS::strlen(p2));
  ACE_OS::strcpy (p3, p2);

  rotate_string ((char *) p1);

  char *return_string;
  return_string = CORBA::string_alloc (ACE_OS::strlen(p1));
  ACE_OS::strcpy (return_string, p1);

  return return_string;
}

::VBseqlong *
Test_impl::seq_op1 (::VBseqlong * p1,
                    ::VBseqlong *& p2,
                    ::VBseqlong_out p3)
{
  ACE_NEW_RETURN (p3,
                  VBseqlong(*p2),
                  0);
  CORBA::ULong i;
  for (i=0; i<p3->_value().length(); i++)
    {
      (*p3)[i] *= 5;
    }

  for (i=0; i<p2->_value().length(); i++)
    {
      (*p2)[i] *= 3;
    }

  VBseqlong *retval = 0;
  ACE_NEW_RETURN (retval,
                  VBseqlong(*p1),
                  0);

  return retval;
}

void
Test_impl::seq_op2 (const ::TDseqlong & p1,
                    ::TDseqlong & p2,
                    ::TDseqlong_out p3)
{
  ACE_NEW (p3,
           TDseqlong(p1));
  CORBA::ULong i;
  for (i=0; i<p3->length(); i++)
    {
      (*p3)[i] *= 5;
    }

  for (i=0; i<p2.length(); i++)
    {
      p2[i] *= 3;
    }
}

::VBfixed_struct1 *
Test_impl::struct_op1 (::VBfixed_struct1 * p1,
                       ::VBfixed_struct1 *& p2,
                       ::VBfixed_struct1_out p3)
{
  ACE_NEW_RETURN (p3,
                  VBfixed_struct1 (p2->_value()),
                  0);
  // transform p3 values
  p3->l (p3->l() * 5);
  p3->abstruct().s1 = (p3->abstruct()).s1 * 5;
  p3->abstruct().s2 = (p3->abstruct()).s2 * 5;

  // transform p2 values
  p2->l (p2->l() * 3);
  p2->abstruct().s1 = (p2->abstruct()).s1 * 3;
  p2->abstruct().s2 = (p2->abstruct()).s2 * 3;

  VBfixed_struct1 *retval = 0;
  ACE_NEW_RETURN (retval,
                  VBfixed_struct1 (p1->_value()),
                  0);

  return retval;
}

void
Test_impl::struct_op2 (const ::Fixed_Struct1 & p1,
                       ::Fixed_Struct1 & p2,
                       ::Fixed_Struct1_out p3)
{
  p3 = p1;

  p2.l *=  3;
  p2.abstruct.s1 *= 3;
  p2.abstruct.s2 *= 3;
}

::VBvariable_struct1 *
Test_impl::struct_op3 (::VBvariable_struct1 * p1,
                       ::VBvariable_struct1 *& p2,
                       ::VBvariable_struct1_out p3)
{
  // transform p2 values
  p2->l (p2->l() * 3);
  rotate_string ((CORBA::Char *)p2->str());

  ACE_NEW_RETURN (p3,
                  VBvariable_struct1 (p2->_value()),
                  0);

  VBvariable_struct1 *retval = 0;
  ACE_NEW_RETURN (retval,
                  VBvariable_struct1 (p1->_value()),
                  0);

  return retval;
}


void
Test_impl::struct_op4 (const ::Variable_Struct1 & p1,
                       ::Variable_Struct1 & p2,
                       ::Variable_Struct1_out p3)
{
  ACE_NEW (p3,
           Variable_Struct1());
  p3->l = p1.l;
  p3->str = p1.str;

  p2.l *=  3;
  rotate_string ((CORBA::Char *)p2.str.in());
}


::VBlongarray *
Test_impl::array_op1 (::VBlongarray * p1,
                      ::VBlongarray *& p2,
                      ::VBlongarray_out p3)
{
  long array_len = sizeof(LongArray) / sizeof(CORBA::Long);

  for (long i=0; i<array_len; i++)
    {
      (*p2)[i] *= 3;
    }

  ACE_NEW_RETURN (p3,
                  VBlongarray(*p2),
                  0);

  VBlongarray *retval = 0;
  ACE_NEW_RETURN (retval,
                  VBlongarray(*p1),
                  0);

  return retval;
}

void
Test_impl::array_op2 (const ::LongArray p1,
                      ::LongArray p2,
                      ::LongArray_out p3)
{
  long array_len = sizeof(LongArray) / sizeof(CORBA::Long);

  long i;
  for (i=0; i<array_len; i++)
    {
      p2[i] *= 3;
    }

  for (i=0; i<array_len; i++)
    {
      p3[i] = p1[i];
    }
}


::VBstringarray *
Test_impl::array_op3 (::VBstringarray * p1,
                      ::VBstringarray *& p2,
                      ::VBstringarray_out p3)
{
  long array_len = sizeof( ::StringArray  ) / sizeof( StringArray_slice );

  char * p;
  for (long i=0; i<array_len; i++)
    {
      p = (CORBA::Char *)((*p2)[i]).in();
      rotate_string (p);
    }

  ACE_NEW_RETURN (p3,
                  VBstringarray(*p2),
                  0);

  VBstringarray *retval = 0;
  ACE_NEW_RETURN (retval,
                  VBstringarray(*p1),
                  0);

  return retval;
}

void
Test_impl::array_op4 (const ::StringArray p1,
                      ::StringArray p2,
                      ::StringArray_out p3)
{
  long array_len = sizeof( ::StringArray  ) / sizeof( StringArray_slice );

  char * p;
  long i;
  for (i=0; i<array_len; i++)
    {
      p = (CORBA::Char *)(p2[i]).in();
      rotate_string (p);
    }

  StringArray_slice *sa = StringArray_alloc ();

  for (i=0; i<array_len; i++)
    {
      sa[i] = p1[i];
    }

  p3 = sa;

}


::VBfixed_union1 *
Test_impl::union_op1 (::VBfixed_union1 * p1,
                      ::VBfixed_union1 *& p2,
                      ::VBfixed_union1_out p3)
{
  CORBA::Long longValue;
  CORBA::Short shortValue;
  ACE_NEW_RETURN (p3,
                  VBfixed_union1 (),
                  0);
  VBfixed_union1 *rv = 0;
  ACE_NEW_RETURN (rv,
                  VBfixed_union1 (),
                  0);

  // Set output and return values
  switch (p1->_d ())
    {
    case 1: // Long
      longValue = p1->m1 () * 3;
      p3->m1 (longValue);
      rv->m1 (longValue);
      break;
    case 2: // Short
      shortValue = p1->m2 () * 3;
      p3->m2 (shortValue);
      rv->m2 (shortValue);
      break;
    }

  // Set new "inout" parameter value
  switch (p2->_d())
    {
    case 1: // Long
      p2->m1 (p2->m1 () * 3);
      break;
    case 2: // Short
      p2->m2 (p2->m2 () * 3);
      break;
    }

  return rv;
}

void
Test_impl::union_op2 (const ::Fixed_Union1 & p1,
                      ::Fixed_Union1 & p2,
                      ::Fixed_Union1_out p3)
{
  p3 = p1;

  // Set new "inout" parameter value
  switch (p2._d())
    {
    case 1: // Long
      p2.m1 (p2.m1 () * 3);
      break;
    case 2: // Short
      p2.m2 (p2.m2 () * 3);
      break;
    }
}

::VBvariable_union1 *
Test_impl::union_op3 (::VBvariable_union1 * p1,
                      ::VBvariable_union1 *& p2,
                      ::VBvariable_union1_out p3)
{
  CORBA::Long longValue;
  CORBA::String_var stringValue;
  ACE_NEW_RETURN (p3,
                  VBvariable_union1 (),
                  0);
  VBvariable_union1 *rv = 0;
  ACE_NEW_RETURN (rv,
                  VBvariable_union1 (),
                  0);


  // Set output and return values
  switch (p1->_d ())
    {
    case 1: // Long
      longValue = p1->m1 ();
      p3->m1 (longValue);
      rv->m1 (longValue);
      break;
    case 2: // String
      stringValue = p1->m2 ();
      p3->m2 (stringValue);
      rv->m2 (stringValue);
      break;
    }

  // Set new "inout" parameter value
  switch (p2->_d())
    {
    case 1: // Long
      p2->m1 (p2->m1 () * 3);
      break;
    case 2: // String
      stringValue = p2->m2 ();
      rotate_string (stringValue);
      p2->m2 (stringValue);
      break;
    }

  return rv;
}

void
Test_impl::union_op4 (const ::Variable_Union1 & p1,
                      ::Variable_Union1 & p2,
                      ::Variable_Union1_out p3)
{
  CORBA::String_var stringValue;

  // Return value of "in" parameter to "out" parameter
  ACE_NEW (p3,
           Variable_Union1());

  switch (p1._d ())
    {
    case 1: // Long
      p3->m1 (p1.m1 ());
      break;
    case 2: // String
      p3->m2 (p1.m2 ());
      break;
    }

  // Set new "inout" parameter value
  switch (p2._d())
    {
    case 1: // Long
      p2.m1 (p2.m1 () * 3);
      break;
    case 2: // String
      stringValue = p2.m2 ();
      rotate_string (stringValue);
      p2.m2 (stringValue);
      break;
    }
}


void
Test_impl::shutdown (void)
{
  this->orb_->shutdown (0);
}
