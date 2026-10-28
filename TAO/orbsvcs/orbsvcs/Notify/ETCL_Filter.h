/* -*- C++ -*- */
/**
 *  @file ETCL_Filter.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_ETCL_FILTER_H
#define TAO_Notify_ETCL_FILTER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers_T.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Atomic_Op.h"
#include "orbsvcs/CosNotifyFilterS.h"
#include "orbsvcs/Notify/Notify_Constraint_Interpreter.h"
#include "orbsvcs/Notify/Topology_Object.h"
#include "ace/Null_Mutex.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_ETCL_Filter;

class TAO_Notify_Constraint_Expr : public TAO_Notify::Topology_Object
{
public:
  friend class TAO_Notify_ETCL_Filter;

  TAO_Notify_Constraint_Expr ();
  ~TAO_Notify_Constraint_Expr () override;

  TAO_Notify_Constraint_Expr (TAO_Notify_Constraint_Expr const &) = delete;
  TAO_Notify_Constraint_Expr (TAO_Notify_Constraint_Expr &&) = delete;
  TAO_Notify_Constraint_Expr &operator= (TAO_Notify_Constraint_Expr const &) = delete;
  TAO_Notify_Constraint_Expr &operator= (TAO_Notify_Constraint_Expr &&) = delete;

  void save_persistent (
    TAO_Notify::Topology_Saver &saver) override;


  void load_attrs(
    TAO_Notify::NVPList const &attrs) override;

  TAO_Notify::Topology_Object* load_child (
    ACE_CString const &type,
    CORBA::Long id,
    TAO_Notify::NVPList const &attrs) override;


private:
  /// Release this object.
  void release () override;

  // = DESCRIPTION
  //   Structure for associating ConstraintInfo with an interpreter.
  //
  CosNotifyFilter::ConstraintExp constr_expr;
  // Constraint Expression.

  TAO_Notify_Constraint_Interpreter interpreter;
  // Constraint Interpreter.
};

/**
 * @class TAO_ETCL_Filter
 *
 * @brief Implementation of CosNotifyFilter::Filter servant.
 */
class TAO_Notify_Serv_Export TAO_Notify_ETCL_Filter
  : public POA_CosNotifyFilter::Filter,
    public TAO_Notify::Topology_Object
{
public:
  /// Constructor
  TAO_Notify_ETCL_Filter (PortableServer::POA_ptr poa,
                          const char *constraint_grammar,
                          const TAO_Notify_Object::ID& id);

  /// Destructor
  ~TAO_Notify_ETCL_Filter () override;

  TAO_Notify_ETCL_Filter (TAO_Notify_ETCL_Filter const &) = delete;
  TAO_Notify_ETCL_Filter (TAO_Notify_ETCL_Filter &&) = delete;
  TAO_Notify_ETCL_Filter &operator= (TAO_Notify_ETCL_Filter const &) = delete;
  TAO_Notify_ETCL_Filter &operator= (TAO_Notify_ETCL_Filter &&) = delete;

  void save_persistent (TAO_Notify::Topology_Saver &saver) override;
  void load_attrs (TAO_Notify::NVPList const &attrs) override;
  TAO_Notify::Topology_Object* load_child (ACE_CString const &type,
    CORBA::Long id, TAO_Notify::NVPList const &attrs) override;

protected:
  char * constraint_grammar () override;

  CosNotifyFilter::ConstraintInfoSeq * add_constraints (
    CosNotifyFilter::ConstraintExpSeq const &constraint_list) override;

  void modify_constraints (CosNotifyFilter::ConstraintIDSeq const &del_list,
                           CosNotifyFilter::ConstraintInfoSeq const &modify_list) override;

  CosNotifyFilter::ConstraintInfoSeq * get_constraints (
    CosNotifyFilter::ConstraintIDSeq const &id_list) override;

  CosNotifyFilter::ConstraintInfoSeq * get_all_constraints () override;

  void remove_all_constraints () override;

  void destroy () override;

  CORBA::Boolean match (CORBA::Any const &filterable_data) override;

  CORBA::Boolean match_structured (CosNotification::StructuredEvent const &filterable_data) override;

  CORBA::Boolean match_typed (CosNotification::PropertySeq const &filterable_data) override;

  CosNotifyFilter::CallbackID attach_callback (CosNotifyComm::NotifySubscribe_ptr callback) override;

  void detach_callback (CosNotifyFilter::CallbackID callback) override;

  CosNotifyFilter::CallbackIDSeq * get_callbacks () override;

private:
  /// Release this object.
  void release () override;

  void add_constraints_i (const CosNotifyFilter::ConstraintInfoSeq& constraint_info_seq);
  void add_constraint_i (const CosNotifyFilter::ConstraintInfo& constraint,
    CosNotifyFilter::ConstraintID cnstr_id = 0);

  TAO_Notify_Constraint_Expr*
    add_constraint_i (CosNotifyFilter::ConstraintID cnstr_id);

  void remove_all_constraints_i ();

  /// Lock to serialize access to data members.
  TAO_SYNCH_MUTEX lock_;

  /// Id generator for ConstraintInfo's.
  CORBA::Long constraint_expr_ids_;

  /// A list of the constraints stored in this filter.
  using CONSTRAINT_EXPR_LIST = ACE_Hash_Map_Manager <CosNotifyFilter::ConstraintID,
                                                      TAO_Notify_Constraint_Expr*,
                                                      ACE_SYNCH_NULL_MUTEX>;

  CONSTRAINT_EXPR_LIST constraint_expr_list_;

  PortableServer::POA_var poa_;

  TAO_Notify_Object::ID id_;

  ACE_CString grammar_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_ETCL_FILTER_H */
