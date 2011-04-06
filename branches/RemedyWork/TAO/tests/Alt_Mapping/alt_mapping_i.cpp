
//=============================================================================
/**
 *  @file    alt_mapping_i.cpp
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================


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
Alt_Mapping_i::test_unbounded_string (const std::string s1,
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
  // We copy the "in" sequences into all the inout, out and return sequences.

  // now copy all elements of s1 into the others using the assignment operator
  s2 = s1;
  s3 = s1;
  return s1;
}

void
Alt_Mapping_i::shutdown (void)
{
  this->orb_->shutdown ();
}

