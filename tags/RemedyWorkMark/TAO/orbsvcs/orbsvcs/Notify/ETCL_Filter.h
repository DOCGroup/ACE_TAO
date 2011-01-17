/* -*- C++ -*- */
/**
 *  @file ETCL_Filter.h
 *
 *  $Id$
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

  TAO_Notify_Constraint_Expr (void);
  virtual ~TAO_Notify_Constraint_Expr ();

  void save_persistent (
    TAO_Notify::Topology_Saver& saver);


  void load_attrs(
    const TAO_Notify::NVPList& attrs);

  TAO_Notify::Topology_Object* load_child (
    const ACE_CString &type,
    CORBA::Long id,
    const TAO_Notify::NVPList& attrs);


private:
  /// Release this object.
  virtual void release (void);

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
 *
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
  virtual ~TAO_Notify_ETCL_Filter (void);

  virtual void save_persistent (TAO_Notify::Topology_Saver& saver);
  void load_attrs(const TAO_Notify::NVPList& attrs);
  TAO_Notify::Topology_Object* load_child (const ACE_CString &type,
    CORBA::Long id, const TAO_Notify::NVPList& attrs);

protected:
  virtual char * constraint_grammar (void);

  virtual CosNotifyFilter::ConstraintInfoSeq * add_constraints (const CosNotifyFilter::ConstraintExpSeq & constraint_list);

  virtual void modify_constraints (const CosNotifyFilter::ConstraintIDSeq & del_list,
                                   const CosNotifyFilter::ConstraintInfoSeq & modify_list);

  virtual CosNotifyFilter::ConstraintInfoSeq * get_constraints (const CosNotifyFilter::ConstraintIDSeq & id_list);

  virtual CosNotifyFilter::ConstraintInfoSeq * get_all_constraints (void);

  virtual void remove_all_constraints (void);

  virtual void destroy (void);

  virtual CORBA::Boolean match (const CORBA::Any & filterable_data);

  virtual CORBA::Boolean match_structured (const CosNotification::StructuredEvent & filterable_data);

  virtual CORBA::Boolean match_typed (const CosNotification::PropertySeq & filterable_data);

  virtual CosNotifyFilter::CallbackID attach_callback (CosNotifyComm::NotifySubscribe_ptr callback);

  virtual void detach_callback (CosNotifyFilter::CallbackID callback);

  virtual CosNotifyFilter::CallbackIDSeq * get_callbacks (void);

private:

  /// Release this object.
  virtual void release (void);

  void add_constraints_i (const CosNotifyFilter::ConstraintInfoSeq& constraint_info_seq);
  void add_constraint_i (const CosNotifyFilter::ConstraintInfo& constraint,
    CosNotifyFilter::ConstraintID cnstr_id = 0);

  TAO_Notify_Constraint_Expr*
    add_constraint_i (CosNotifyFilter::ConstraintID cnstr_id);

  void remove_all_constraints_i (void);

  /// Lock to serialize access to data members.
  TAO_SYNCH_MUTEX lock_;

  /// Id generator for ConstraintInfo's.
  CORBA::Long constraint_expr_ids_;

  /// A list of the constraints stored in this filter.
  typedef ACE_Hash_Map_Manager <CosNotifyFilter::ConstraintID,
                                TAO_Notify_Constraint_Expr*,
                                ACE_SYNCH_NULL_MUTEX>
  CONSTRAINT_EXPR_LIST;

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
