// $Id$

// ============================================================================
//
// = FILENAME
//    alt_mapping_i.h
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef ALT_MAPPING_I_H
#define ALT_MAPPING_I_H

#include "alt_mappingS.h"

class Alt_Mapping_i : public POA_Alt_Mapping
{
  // = TITLE
  //    Alt_Mapping_i
  // = DESCRIPTION
  //    Implementation of the Alt_Mapping_i test suite.
public:
  Alt_Mapping_i (CORBA::ORB_ptr orb);

  ~Alt_Mapping_i (void);

  virtual std::string
  test_unbounded_string (const std::string s1,
                         std::string &s2,
                         std::string &s3);

  virtual Alt_Mapping::StructSeq test_struct_sequence (
    const Alt_Mapping::StructSeq & s1,
    Alt_Mapping::StructSeq & s2,
    Alt_Mapping::StructSeq & s3);

  void shutdown (void);

private:
  CORBA::ORB_var orb_;
};

#endif /* ALT_MAPPING_I_H */
