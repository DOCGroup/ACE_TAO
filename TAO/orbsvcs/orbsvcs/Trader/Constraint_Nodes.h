// -*- C++ -*-

//=============================================================================
/**
 *  @file    Constraint_Nodes.h
 *
 *  @author Seth Widoff <sbw1@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_CONSTRAINT_NODES_H
#define TAO_CONSTRAINT_NODES_H
#include /**/ "ace/pre.h"

#include "tao/Basic_Types.h"
#include "tao/String_Manager_T.h"

#include "orbsvcs/Trader/trading_serv_export.h"

#include <memory>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning (disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Constraint_Visitor;
using TAO_Expression_Type = unsigned short;

namespace CORBA
{
  class Any;
  using Any_ptr = Any *;

  class TypeCode;
  using TypeCode_ptr = TypeCode *;
}

/**
 * @class TAO_Constraint
 *
 * @brief TAO_Constraint is the base class of all nodes on the
 * constraint expression tree.
 *
 * An TAO_Constraint knows what type of operation or entity
 * it represents, and which method on TAO_Constraint_Visitor
 * correlates to its type. When the accept method is invoked, a
 * subclass dispatches the method on an TAO_Constraint_Visitor
 * correlating to its type.
 */
class TAO_Trading_Serv_Export TAO_Constraint
{
public:
  /**
   * Implementing the pattern of double dispatching, each subclass of
   * TAO_Constraint will call back on an InterpreterVisitor the
   * method to handle a node of its ExpressionType.
   */
  virtual int accept (TAO_Constraint_Visitor* visitor) = 0;

  /// Return the expression type represented by this node.
  virtual TAO_Expression_Type expr_type () const = 0;

  virtual ~TAO_Constraint () = default;
};

/**
 * @class TAO_Noop_Constraint
 *
 * @brief A node that represents an operation with no operands.
 */
class TAO_Trading_Serv_Export TAO_Noop_Constraint : public TAO_Constraint
{
public:
  explicit TAO_Noop_Constraint (TAO_Expression_Type type)
    : type_ (type) {}

  int accept (TAO_Constraint_Visitor* visitor) override;

  TAO_Expression_Type expr_type () const override
    { return this->type_; }

private:
  TAO_Expression_Type type_;
};

/**
 * @class TAO_Binary_Constraint
 *
 * @brief TAO_Binary_Constraint represents an operation with left
 * and right operands.
 */
class TAO_Trading_Serv_Export TAO_Binary_Constraint : public TAO_Constraint
{
public:
  TAO_Binary_Constraint (TAO_Binary_Constraint const&) = delete;
  TAO_Binary_Constraint& operator= (TAO_Binary_Constraint const&) = delete;
  TAO_Binary_Constraint (TAO_Binary_Constraint&&) = delete;
  TAO_Binary_Constraint& operator= (TAO_Binary_Constraint&&) = delete;

  TAO_Binary_Constraint (TAO_Expression_Type op_type,
                         TAO_Constraint* left,
                         TAO_Constraint* right);

  int accept (TAO_Constraint_Visitor* visitor) override;

  ~TAO_Binary_Constraint () override;

  TAO_Expression_Type expr_type () const override
    { return this->op_; }

  /// Return the left operand of the binary expression
  TAO_Constraint* left_operand () const;

  /// Return the right operand of the binary expression
  TAO_Constraint* right_operand () const;

  // Allow double dispatching without creating an inundation of
  // classes by using a dispatch table of static method pointers to
  // invoke the correct visitor method as efficiently as a virtual
  // method invocation.
  static int visit_or (TAO_Constraint_Visitor*, TAO_Binary_Constraint*);
  static int visit_and (TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_less_than (TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_less_than_equal (TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_greater_than (TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_greater_than_equal (TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_equal (TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_not_equal (TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_add (TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_sub (TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_mult (TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_div (TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_twiddle (TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_in (TAO_Constraint_Visitor* , TAO_Binary_Constraint*);

private:
  /// The operator type
  TAO_Expression_Type op_;

  /// The operands of the expression
  std::unique_ptr<TAO_Constraint> left_;
  std::unique_ptr<TAO_Constraint> right_;
};

/**
 * @class TAO_Unary_Constraint
 *
 * @brief TAO_Unary_Constraint represents an operation with only
 * one operand.
 */
class TAO_Trading_Serv_Export TAO_Unary_Constraint : public TAO_Constraint
{
public:
  TAO_Unary_Constraint (TAO_Unary_Constraint const&) = delete;
  TAO_Unary_Constraint& operator= (TAO_Unary_Constraint const&) = delete;
  TAO_Unary_Constraint (TAO_Unary_Constraint&&) = delete;
  TAO_Unary_Constraint& operator= (TAO_Unary_Constraint&&) = delete;

  TAO_Unary_Constraint (TAO_Expression_Type op_type,
                        TAO_Constraint* operand);

  ~TAO_Unary_Constraint () override;

  int accept (TAO_Constraint_Visitor* visitor) override;

  TAO_Expression_Type expr_type () const override
    { return this->op_; }

  TAO_Constraint* operand ();

private:
  /// The operator type
  TAO_Expression_Type op_;

  /// The operand of the expression
  std::unique_ptr<TAO_Constraint> operand_;
};

/**
 * @class TAO_Property_Constraint
 *
 * @brief TAO_Property_Constraint represents a property whose
 * value is determined by the offer being evaluated.
 */
class TAO_Trading_Serv_Export TAO_Property_Constraint : public TAO_Constraint
{
public:
  TAO_Property_Constraint (TAO_Property_Constraint const&) = delete;
  TAO_Property_Constraint& operator= (TAO_Property_Constraint const&) = delete;
  TAO_Property_Constraint (TAO_Property_Constraint&&) = delete;
  TAO_Property_Constraint& operator= (TAO_Property_Constraint&&) = delete;

  explicit TAO_Property_Constraint (char const* name);

  ~TAO_Property_Constraint () override;

  int accept (TAO_Constraint_Visitor* visitor) override;

  TAO_Expression_Type expr_type () const override;

  /// Returns the name of the property.
  const char* name () const;

private:
  /// The name of the property.
  char* name_;
};

/**
 * @class TAO_Literal_Constraint
 *
 * @brief TAO_Literal_Constraint represents a literal occuring in
 * the constraint expression tree.
 */
class TAO_Trading_Serv_Export TAO_Literal_Constraint : public TAO_Constraint
{
public:
  TAO_Literal_Constraint ();

  explicit TAO_Literal_Constraint (CORBA::Any* any);
  explicit TAO_Literal_Constraint (CORBA::ULongLong uinteger);
  explicit TAO_Literal_Constraint (CORBA::LongLong integer);
  explicit TAO_Literal_Constraint (CORBA::Boolean boolean);
  explicit TAO_Literal_Constraint (CORBA::Double doub);
  explicit TAO_Literal_Constraint (char const* str);

  TAO_Literal_Constraint (TAO_Literal_Constraint const& lit);

  ~TAO_Literal_Constraint () override;

  /// Visitor accept methods.
  int accept (TAO_Constraint_Visitor* visitor) override;

  TAO_Expression_Type expr_type () const override
    { return type_; }

  void operator= (TAO_Literal_Constraint const& co);

  // Conversion routines.
  operator CORBA::Boolean () const;
  operator CORBA::ULongLong () const;
  operator CORBA::LongLong () const;
  operator CORBA::Double () const;
  operator const char* () const;
  operator const CORBA::Any* () const;

  // Return the type represented by this MysteryOperand.

  friend TAO_Trading_Serv_Export bool
    operator< (TAO_Literal_Constraint const& left,
               TAO_Literal_Constraint const& right);

  friend TAO_Trading_Serv_Export bool
    operator<= (TAO_Literal_Constraint const& left,
                TAO_Literal_Constraint const& right);

  friend TAO_Trading_Serv_Export bool
    operator> (TAO_Literal_Constraint const& left,
               TAO_Literal_Constraint const& right);

  friend TAO_Trading_Serv_Export bool
    operator>= (TAO_Literal_Constraint const& left,
                TAO_Literal_Constraint const& right);

  friend TAO_Trading_Serv_Export bool
    operator== (TAO_Literal_Constraint const& left,
                TAO_Literal_Constraint const& right);

  friend TAO_Trading_Serv_Export bool
    operator!= (TAO_Literal_Constraint const& left,
                TAO_Literal_Constraint const& right);

  friend TAO_Trading_Serv_Export bool
    operator== (double left,
                TAO_Literal_Constraint const& right);

  friend TAO_Trading_Serv_Export bool
    operator== (const TAO::String_Manager& left,
                TAO_Literal_Constraint const& right);

  friend TAO_Trading_Serv_Export TAO_Literal_Constraint
    operator+ (TAO_Literal_Constraint const& left,
               TAO_Literal_Constraint const& right);

  friend TAO_Trading_Serv_Export TAO_Literal_Constraint
    operator- (TAO_Literal_Constraint const& left,
               TAO_Literal_Constraint const& right);

  friend TAO_Trading_Serv_Export TAO_Literal_Constraint
    operator* (TAO_Literal_Constraint const& left,
               TAO_Literal_Constraint const& right);

  friend TAO_Trading_Serv_Export TAO_Literal_Constraint
    operator/ (TAO_Literal_Constraint const& left,
               TAO_Literal_Constraint const& right);

  friend TAO_Trading_Serv_Export TAO_Literal_Constraint
    operator- (TAO_Literal_Constraint const& operand);

  /// Ensure both operands are of the same simple numeric type.
  static TAO_Expression_Type
    widest_type (TAO_Literal_Constraint const& left,
                 TAO_Literal_Constraint const& right);

  /// Determine the comparable Expression Type from the CORBA type
  static TAO_Expression_Type
    comparable_type (CORBA::TypeCode_ptr type);

 private:
  /// Private copy method.
  void copy (TAO_Literal_Constraint const& co);

  union
  {
    char* str_;
    CORBA::Any_ptr any_;
    CORBA::ULongLong uinteger_;
    CORBA::LongLong integer_;
    CORBA::Boolean bool_;
    CORBA::Double double_;
  } op_;
  // Union of the possible literal types.

  /// The actual types of the TAO_Literal_Constraint.
  TAO_Expression_Type type_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "Constraint_Nodes.inl"
#endif /* __ACE_INLINE */

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_CONSTRAINT_NODES_H */
