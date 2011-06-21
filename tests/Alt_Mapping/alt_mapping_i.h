
//=============================================================================
/**
 *  @file    alt_mapping_i.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef ALT_MAPPING_I_H
#define ALT_MAPPING_I_H

#include "alt_mappingS.h"

class ALT_MAPPING_SKEL_Export Alt_Mapping_i : public POA_Alt_Mapping
{
public:
  Alt_Mapping_i (CORBA::ORB_ptr orb);

  ~Alt_Mapping_i (void);

  virtual std::string
  test_unbounded_string (const std::string &s1,
                         std::string &s2,
                         std::string &s3);

  virtual Alt_Mapping::StructSeq test_struct_sequence (
    const Alt_Mapping::StructSeq & s1,
    Alt_Mapping::StructSeq & s2,
    Alt_Mapping::StructSeq & s3);
    
  virtual CORBA::StringSeq test_strseq (
    const CORBA::StringSeq & s1,
    CORBA::StringSeq & s2,
    CORBA::StringSeq & s3);
    
  virtual CORBA::LongSeq test_long_sequence (
      const CORBA::LongSeq & s1,
      CORBA::LongSeq & s2,
      CORBA::LongSeq & s3);

  virtual CORBA::OctetSeq test_octet_sequence (
      const CORBA::OctetSeq & s1,
      CORBA::OctetSeq & s2,
      CORBA::OctetSeq & s3);

  virtual Alt_Mapping::Var_Struct *
  test_var_struct (const Alt_Mapping::Var_Struct & s1,
                   Alt_Mapping::Var_Struct & s2,
                   Alt_Mapping::Var_Struct_out s3);
  
  virtual Alt_Mapping::Nested_Struct *
  test_nested_struct (const Alt_Mapping::Nested_Struct & s1,
                      Alt_Mapping::Nested_Struct & s2,
                      Alt_Mapping::Nested_Struct_out s3);

  void shutdown (void);

private:
  CORBA::ORB_var orb_;
};

#endif /* ALT_MAPPING_I_H */
