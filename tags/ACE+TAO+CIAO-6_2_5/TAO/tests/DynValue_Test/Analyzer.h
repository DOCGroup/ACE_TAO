// $Id$

#ifndef INCLUDE_ANALYZER_H
#define INCLUDE_ANALYZER_H

#include "ValueTypes_impl.h"
#include "tao/DynamicAny/DynAnyFactory.h"

//============= class DynAnyAnalyzer ============

class DynAnyAnalyzer
{
private:
  unsigned int level_;
  CORBA::ORB_var orb_;
  DynamicAny::DynAnyFactory_var dany_fact_;

public:
  DynAnyAnalyzer (int argc, ACE_TCHAR *argv[]);
  ~DynAnyAnalyzer (void) {orb_->destroy ();}

  DynamicAny::DynAny_ptr DynAny_NoTrunc (const CORBA::Any &);
  DynamicAny::DynAny_ptr DynAny (const CORBA::Any &);
  DynamicAny::DynAny_ptr DynAny (const CORBA::TypeCode_ptr &);

  void register_factory (CORBA::ValueFactoryBase_var factory);

  void analyze (
    DynamicAny::DynAny_ptr da,
    CORBA::Boolean newline = false);

private:
  /// List of base types.
  typedef ACE_Array_Base<CORBA::TypeCode_var> BaseTypesList_t;

  void tab (const char p[])
  {
    for (unsigned int i = 0u; i < level_ ; ++i)
      ACE_DEBUG ((LM_DEBUG, ".."));
    ACE_DEBUG ((LM_DEBUG, "%C", p));
  }

  void analyze_basic_seq (
    CORBA::TypeCode_ptr tc,
    DynamicAny::DynAny_ptr da);

  static void get_base_types (
    CORBA::TypeCode_ptr tc,
    BaseTypesList_t &base_types,
    CORBA::ULong *total_member_count = 0);

  /// Return the unaliased valuetype typecode that corresponds to
  /// index (0..total_members-1) from the given hiarchical list of
  /// the derived type and it basetypes.
  static CORBA::TypeCode_ptr get_correct_base_type (
    const BaseTypesList_t &base_types,
    CORBA::ULong &index);
};

#endif // INCLUDE_ANALYZER_H
