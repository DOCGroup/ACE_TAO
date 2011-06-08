// $Id$

// ============================================================================
//
// = FILENAME
//    alt_mapping_i.cpp
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include "alt_mapping_i.h"

#include "tao/debug.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_wchar.h"
#include "ace/OS_NS_string.h"

Alt_Mapping_i::Alt_Mapping_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Alt_Mapping_i::~Alt_Mapping_i (void)
{
}

// test unbounded strings. For return and out types, we return duplicates of
// the in string. For the inout, we append the same string to itself and send
// it back
std::string
Alt_Mapping_i::test_unbounded_string (const std::string &s1,
                                      std::string &s2,
                                      std::string &s3)
{
  std::string retstr = s1;
  s3 = s1;
  s2 = s1 + s1;
  return retstr;
}

// test for struct sequences
Alt_Mapping::StructSeq
Alt_Mapping_i::test_struct_sequence (
  const Alt_Mapping::StructSeq & s1,
  Alt_Mapping::StructSeq & s2,
  Alt_Mapping::StructSeq & s3)
{
  s2 = s1;
  s3 = s1;
  return s1;
}

CORBA::StringSeq
Alt_Mapping_i::test_strseq (
  const CORBA::StringSeq & s1,
  CORBA::StringSeq & s2,
  CORBA::StringSeq & s3)
{
  s2 = s1;
  s3 = s1;
  return s1;
}

CORBA::LongSeq
Alt_Mapping_i::test_long_sequence (
  const CORBA::LongSeq & s1,
  CORBA::LongSeq & s2,
  CORBA::LongSeq & s3)
{
  s2 = s1;
  s3 = s1;
  CORBA::LongSeq ret (s1);
  
  return ret;
}

CORBA::OctetSeq
Alt_Mapping_i::test_octet_sequence (
  const CORBA::OctetSeq & s1,
  CORBA::OctetSeq & s2,
  CORBA::OctetSeq & s3)
{
  s2 = s1;
  s3 = s1;
  CORBA::OctetSeq ret (s1);
  
  return ret;
}

Alt_Mapping::Var_Struct *
Alt_Mapping_i::test_var_struct (
  const Alt_Mapping::Var_Struct & s1,
  Alt_Mapping::Var_Struct & s2,
  Alt_Mapping::Var_Struct_out s3)
{
  Alt_Mapping::Var_Struct
    *ret = new Alt_Mapping::Var_Struct,
    *out = new Alt_Mapping::Var_Struct;

  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

Alt_Mapping::Nested_Struct *
Alt_Mapping_i::test_nested_struct (
  const Alt_Mapping::Nested_Struct & s1,
  Alt_Mapping::Nested_Struct & s2,
  Alt_Mapping::Nested_Struct_out s3)
{
  Alt_Mapping::Nested_Struct
    *ret = new Alt_Mapping::Nested_Struct,
    *out = new Alt_Mapping::Nested_Struct;

  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

void
Alt_Mapping_i::shutdown (void)
{
  this->orb_->shutdown ();
}

