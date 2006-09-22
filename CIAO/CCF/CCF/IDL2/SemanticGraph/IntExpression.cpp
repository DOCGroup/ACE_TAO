// file      : CCF/IDL2/SemanticGraph/IntExpression.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/IntExpression.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      // IntExpression
      //
      namespace
      {
        TypeInfo
        int_expression_init_ ()
        {
          TypeInfo ti (typeid (IntExpression));
          ti.add_base (Access::PUBLIC, true, Expression::static_type_info ());
          return ti;
        }

        TypeInfo int_expression_ (int_expression_init_ ());
      }

      TypeInfo const& IntExpression::
      static_type_info () { return int_expression_; }

      IntExpression::
      ~IntExpression ()
      {
      }

      // IntLiteral
      //
      namespace
      {
        TypeInfo
        int_literal_init_ ()
        {
          TypeInfo ti (typeid (IntLiteral));
          ti.add_base (
            Access::PUBLIC, true, IntExpression::static_type_info ());
          return ti;
        }

        TypeInfo int_literal_ (int_literal_init_ ());
      }

      TypeInfo const& IntLiteral::
      static_type_info () { return int_literal_; }


      // IntConst
      //
      namespace
      {
        TypeInfo
        int_const_init_ ()
        {
          TypeInfo ti (typeid (IntConst));
          ti.add_base (Access::PUBLIC, true, Const::static_type_info ());
          ti.add_base (
            Access::PUBLIC, true, IntExpression::static_type_info ());
          return ti;
        }

        TypeInfo int_const_ (int_const_init_ ());
      }

      TypeInfo const& IntConst::
      static_type_info () { return int_const_; }


      // NEG
      //

      namespace
      {
        TypeInfo
        int_negates_init_ ()
        {
          TypeInfo ti (typeid (IntNegates));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo int_negates_ (int_negates_init_ ());
      }

      TypeInfo const& IntNegates::
      static_type_info () { return int_negates_; }

      namespace
      {
        TypeInfo
        int_neg_init_ ()
        {
          TypeInfo ti (typeid (IntNeg));
          ti.add_base (
            Access::PUBLIC, true, IntExpression::static_type_info ());
          return ti;
        }

        TypeInfo int_neg_ (int_neg_init_ ());
      }

      TypeInfo const& IntNeg::
      static_type_info () { return int_neg_; }


      // COM
      //

      namespace
      {
        TypeInfo
        int_complements_init_ ()
        {
          TypeInfo ti (typeid (IntComplements));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo int_complements_ (int_complements_init_ ());
      }

      TypeInfo const& IntComplements::
      static_type_info () { return int_complements_; }

      namespace
      {
        TypeInfo
        int_com_init_ ()
        {
          TypeInfo ti (typeid (IntCom));
          ti.add_base (
            Access::PUBLIC, true, IntExpression::static_type_info ());
          return ti;
        }

        TypeInfo int_com_ (int_com_init_ ());
      }

      TypeInfo const& IntCom::
      static_type_info () { return int_com_; }


      // MUL
      //

      namespace
      {
        TypeInfo
        int_multiplies_init_ ()
        {
          TypeInfo ti (typeid (IntMultiplies));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo int_multiplies_ (int_multiplies_init_ ());
      }

      TypeInfo const& IntMultiplies::
      static_type_info () { return int_multiplies_; }

      namespace
      {
        TypeInfo
        int_mul_init_ ()
        {
          TypeInfo ti (typeid (IntMul));
          ti.add_base (
            Access::PUBLIC, true, IntExpression::static_type_info ());
          return ti;
        }

        TypeInfo int_mul_ (int_mul_init_ ());
      }

      TypeInfo const& IntMul::
      static_type_info () { return int_mul_; }


      // DIV
      //

      namespace
      {
        TypeInfo
        int_divides_init_ ()
        {
          TypeInfo ti (typeid (IntDivides));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo int_divides_ (int_divides_init_ ());
      }

      TypeInfo const& IntDivides::
      static_type_info () { return int_divides_; }

      namespace
      {
        TypeInfo
        int_div_init_ ()
        {
          TypeInfo ti (typeid (IntDiv));
          ti.add_base (
            Access::PUBLIC, true, IntExpression::static_type_info ());
          return ti;
        }

        TypeInfo int_div_ (int_div_init_ ());
      }

      TypeInfo const& IntDiv::
      static_type_info () { return int_div_; }

      namespace
      {
        TypeInfo
        int_rem_init_ ()
        {
          TypeInfo ti (typeid (IntRem));
          ti.add_base (
            Access::PUBLIC, true, IntExpression::static_type_info ());
          return ti;
        }

        TypeInfo int_rem_ (int_rem_init_ ());
      }

      TypeInfo const& IntRem::
      static_type_info () { return int_rem_; }


      // ADD
      //

      namespace
      {
        TypeInfo
        int_adds_init_ ()
        {
          TypeInfo ti (typeid (IntAdds));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo int_adds_ (int_adds_init_ ());
      }

      TypeInfo const& IntAdds::
      static_type_info () { return int_adds_; }

      namespace
      {
        TypeInfo
        int_add_init_ ()
        {
          TypeInfo ti (typeid (IntAdd));
          ti.add_base (
            Access::PUBLIC, true, IntExpression::static_type_info ());
          return ti;
        }

        TypeInfo int_add_ (int_add_init_ ());
      }

      TypeInfo const& IntAdd::
      static_type_info () { return int_add_; }


      // SUB
      //

      namespace
      {
        TypeInfo
        int_subtracts_init_ ()
        {
          TypeInfo ti (typeid (IntSubtracts));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo int_subtracts_ (int_subtracts_init_ ());
      }

      TypeInfo const& IntSubtracts::
      static_type_info () { return int_subtracts_; }

      namespace
      {
        TypeInfo
        int_sub_init_ ()
        {
          TypeInfo ti (typeid (IntSub));
          ti.add_base (
            Access::PUBLIC, true, IntExpression::static_type_info ());
          return ti;
        }

        TypeInfo int_sub_ (int_sub_init_ ());
      }

      TypeInfo const& IntSub::
      static_type_info () { return int_sub_; }


      // RSH & LSH
      //

      namespace
      {
        TypeInfo
        int_shifts_init_ ()
        {
          TypeInfo ti (typeid (IntShifts));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo int_shifts_ (int_shifts_init_ ());
      }

      TypeInfo const& IntShifts::
      static_type_info () { return int_shifts_; }

      namespace
      {
        TypeInfo
        int_rsh_init_ ()
        {
          TypeInfo ti (typeid (IntRsh));
          ti.add_base (
            Access::PUBLIC, true, IntExpression::static_type_info ());
          return ti;
        }

        TypeInfo int_rsh_ (int_rsh_init_ ());
      }

      TypeInfo const& IntRsh::
      static_type_info () { return int_rsh_; }

      namespace
      {
        TypeInfo
        int_lsh_init_ ()
        {
          TypeInfo ti (typeid (IntLsh));
          ti.add_base (
            Access::PUBLIC, true, IntExpression::static_type_info ());
          return ti;
        }

        TypeInfo int_lsh_ (int_lsh_init_ ());
      }

      TypeInfo const& IntLsh::
      static_type_info () { return int_lsh_; }


      // AND
      //

      namespace
      {
        TypeInfo
        int_conjuncts_init_ ()
        {
          TypeInfo ti (typeid (IntConjuncts));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo int_conjuncts_ (int_conjuncts_init_ ());
      }

      TypeInfo const& IntConjuncts::
      static_type_info () { return int_conjuncts_; }

      namespace
      {
        TypeInfo
        int_and_init_ ()
        {
          TypeInfo ti (typeid (IntAnd));
          ti.add_base (
            Access::PUBLIC, true, IntExpression::static_type_info ());
          return ti;
        }

        TypeInfo int_and_ (int_and_init_ ());
      }

      TypeInfo const& IntAnd::
      static_type_info () { return int_and_; }


      // XOR
      //

      namespace
      {
        TypeInfo
        int_exclusively_disjuncts_init_ ()
        {
          TypeInfo ti (typeid (IntExclusivelyDisjuncts));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo int_exclusively_disjuncts_ (
          int_exclusively_disjuncts_init_ ());
      }

      TypeInfo const& IntExclusivelyDisjuncts::
      static_type_info () { return int_exclusively_disjuncts_; }

      namespace
      {
        TypeInfo
        int_xor_init_ ()
        {
          TypeInfo ti (typeid (IntXor));
          ti.add_base (
            Access::PUBLIC, true, IntExpression::static_type_info ());
          return ti;
        }

        TypeInfo int_xor_ (int_xor_init_ ());
      }

      TypeInfo const& IntXor::
      static_type_info () { return int_xor_; }


      // OR
      //

      namespace
      {
        TypeInfo
        int_inclusively_disjuncts_init_ ()
        {
          TypeInfo ti (typeid (IntInclusivelyDisjuncts));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo int_inclusively_disjuncts_ (
          int_inclusively_disjuncts_init_ ());
      }

      TypeInfo const& IntInclusivelyDisjuncts::
      static_type_info () { return int_inclusively_disjuncts_; }

      namespace
      {
        TypeInfo
        int_or_init_ ()
        {
          TypeInfo ti (typeid (IntOr));
          ti.add_base (
            Access::PUBLIC, true, IntExpression::static_type_info ());
          return ti;
        }

        TypeInfo int_or_ (int_or_init_ ());
      }

      TypeInfo const& IntOr::
      static_type_info () { return int_or_; }
    }
  }
}
