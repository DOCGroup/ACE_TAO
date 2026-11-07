#ifndef INCLUDE_ANALYZER_H
#define INCLUDE_ANALYZER_H

#include "ValueTypes_impl.h"
#include "tao/DynamicAny/DynAnyFactory.h"

#include <cstddef>
#include <vector>

//============= class DynAnyAnalyzer ============

class DynAnyAnalyzer
{
private:
  std::size_t level_ {1};
  CORBA::ORB_var orb_;
  DynamicAny::DynAnyFactory_var dany_fact_ {};

public:
  DynAnyAnalyzer (int argc, ACE_TCHAR *argv[]);
  DynAnyAnalyzer (DynAnyAnalyzer const &) = delete;
  DynAnyAnalyzer (DynAnyAnalyzer &&) = delete;
  DynAnyAnalyzer &operator= (DynAnyAnalyzer const &) = delete;
  DynAnyAnalyzer &operator= (DynAnyAnalyzer &&) = delete;
  ~DynAnyAnalyzer () { orb_->destroy (); }

  DynamicAny::DynAny_ptr DynAny_NoTrunc (CORBA::Any const &);
  DynamicAny::DynAny_ptr DynAny (CORBA::Any const &);
  DynamicAny::DynAny_ptr DynAny (CORBA::TypeCode_ptr const &);

  void register_factory (CORBA::ValueFactoryBase_var const &factory);

  void analyze (
    DynamicAny::DynAny_ptr da,
    bool newline = false);

private:
  /// List of base types.
  using BaseTypesList_t = std::vector<CORBA::TypeCode_var>;

  void tab (char const *p)
  {
    for (std::size_t i = 0; i < level_; ++i)
      ACE_DEBUG ((LM_DEBUG, ".."));
    ACE_DEBUG ((LM_DEBUG, "%C", p));
  }

  void analyze_basic_seq (
    CORBA::TypeCode_ptr tc,
    DynamicAny::DynAny_ptr da);

  static void get_base_types (
    CORBA::TypeCode_ptr tc,
    BaseTypesList_t &base_types,
    CORBA::ULong *total_member_count = nullptr);

  /// Return the unaliased valuetype typecode that corresponds to
  /// index (0..total_members-1) from the given hierarchical list of
  /// the derived type and it basetypes.
  static CORBA::TypeCode_ptr get_correct_base_type (
    BaseTypesList_t const &base_types,
    CORBA::ULong &index);
};

#endif // INCLUDE_ANALYZER_H
