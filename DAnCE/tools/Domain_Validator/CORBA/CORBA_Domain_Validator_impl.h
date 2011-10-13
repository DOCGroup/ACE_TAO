// -*- C++ -*-
// $Id$

/**
 * @file CORBA_Domain_Validator_impl.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * CORBA-based domain validator
 */

#ifndef DOMAIN_VALIDATORI_TI3IMY_H_
#define DOMAIN_VALIDATORI_TI3IMY_H_

#include "tools/Domain_Validator/Domain_ValidatorS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>
#include <list>

class DAnCE_Domain_Validator_Stub_Export DAnCE_Domain_Validator_i
  : public virtual DAnCE::Domain_Validator,
    public virtual ::CORBA::LocalObject
{
public:
  // Constructor
  DAnCE_Domain_Validator_i (CORBA::ORB_ptr orb);

  // Destructor
  virtual ~DAnCE_Domain_Validator_i (void);

  virtual
    bool load_domain_from_file (const char * filename);

  virtual
  void load_domain (const ::Deployment::Domain & domain);

  virtual
  ::CORBA::Boolean validate_node (const char * node_name);

  virtual
  ::CORBA::Boolean block_for_node (const char * node_name,
  const ::DAnCE::Time_Value & max_block_time);

  virtual
  ::CORBA::Boolean validate_domain (::CORBA::StringSeq_out late_nodes);

  virtual
  ::CORBA::Boolean block_for_domain (const ::DAnCE::Time_Value & max_block_time,
  ::CORBA::StringSeq_out late_nodes);

  virtual
  ::CORBA::Boolean validate_plan (const ::Deployment::DeploymentPlan & plan,
  ::CORBA::StringSeq_out late_nodes);

  virtual
  ::CORBA::Boolean block_for_plan (const ::Deployment::DeploymentPlan & plan,
  const ::DAnCE::Time_Value & max_block_time,
  ::CORBA::StringSeq_out late_nodes);



 private:
  typedef std::list < std::string > NODE_LIST;

  ACE_Time_Value convert_time_value (const ::DAnCE::Time_Value &time_value);

  bool create_node_table (void);


  void build_late_list (const NODE_LIST &nodelist,
                        ::CORBA::StringSeq_out &latelist);

  ::CORBA::ORB_var orb_;

  const ::Deployment::Domain *domain_;

  // Maps node names to strings
  typedef std::map < std::string, std::string > NODE_MAP;
    NODE_MAP node_map_;
};


#endif /* CORBA_DOMAIN_VALIDATOR_IMPL_H_  */
