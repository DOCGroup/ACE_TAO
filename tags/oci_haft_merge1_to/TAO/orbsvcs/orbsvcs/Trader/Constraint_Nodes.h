/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Constraint_Nodes.h
 *
 *  $Id$
 *
 *  @author Seth Widoff <sbw1@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_CONSTRAINT_NODES_H
#define TAO_CONSTRAINT_NODES_H
#include "ace/pre.h"

#include "Constraint_Tokens.h"
#include "tao/corba.h"
#include "orbsvcs/Trader/trading_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning (disable:4250)
#endif /* _MSC_VER */

class TAO_Constraint_Visitor;
typedef unsigned short TAO_Expression_Type;

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
class TAO_Trading_Export TAO_Constraint
{
public:

  /**
   * Implementing the pattern of double dispatching, each subclass of
   * TAO_Constraint will call back on an InterpreterVisitor the
   * method to handle a node of its ExpressionType.
   */
  virtual int accept (TAO_Constraint_Visitor* visitor) = 0;

  /// Return the expression type represented by this node.
  virtual TAO_Expression_Type expr_type (void) const = 0;

  virtual ~TAO_Constraint (void) {}
};

/**
 * @class TAO_Noop_Constraint
 *
 * @brief A node that represents an operation with no operands.
 */
class TAO_Trading_Export TAO_Noop_Constraint : public TAO_Constraint
{
public:

  TAO_Noop_Constraint (TAO_Expression_Type type)
    : type_ (type) {}

  virtual int accept (TAO_Constraint_Visitor* visitor);

  virtual TAO_Expression_Type expr_type (void) const
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
class TAO_Trading_Export TAO_Binary_Constraint : public TAO_Constraint
{
public:

  TAO_Binary_Constraint (TAO_Expression_Type op_type,
                         TAO_Constraint* left,
                         TAO_Constraint* right);

  virtual int accept (TAO_Constraint_Visitor* visitor);

  virtual ~TAO_Binary_Constraint (void);

  virtual TAO_Expression_Type expr_type (void) const
    { return this->op_; }

  /// Return the left operand of the binary expression
  TAO_Constraint* left_operand (void) const;

  /// Return the right operand of the binary expression
  TAO_Constraint* right_operand (void) const;

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

  TAO_Binary_Constraint (const TAO_Binary_Constraint&);
  TAO_Binary_Constraint& operator= (const TAO_Binary_Constraint&);

  /// The operator type
  TAO_Expression_Type op_;

  /// The operands of the expression
  TAO_Constraint* left_;
  TAO_Constraint* right_;
};

/**
 * @class TAO_Unary_Constraint
 *
 * @brief TAO_Unary_Constraint represents an operation with only
 * one operand.
 */
class TAO_Trading_Export TAO_Unary_Constraint : public TAO_Constraint
{
public:

  TAO_Unary_Constraint (TAO_Expression_Type op_type,
                                 TAO_Constraint* operand);

  virtual ~TAO_Unary_Constraint (void);

  virtual int accept (TAO_Constraint_Visitor* visitor);

  virtual TAO_Expression_Type expr_type (void) const
    { return this->op_; }

  TAO_Constraint* operand (void);

private:

  TAO_Unary_Constraint (const TAO_Unary_Constraint&);
  TAO_Unary_Constraint& operator= (const TAO_Unary_Constraint&);

  /// The operator type
  TAO_Expression_Type op_;

  /// The operand of the expression
  TAO_Constraint* operand_;
};

/**
 * @class TAO_Property_Constraint
 *
 * @brief TAO_Property_Constraint represents a property whose
 * value is determined by the offer being evaluated.
 */
class TAO_Trading_Export TAO_Property_Constraint : public TAO_Constraint
{
public:

  TAO_Property_Constraint (const char* name);

  virtual ~TAO_Property_Constraint (void);

  virtual int accept (TAO_Constraint_Visitor* visitor);

  virtual TAO_Expression_Type expr_type (void) const
    { return TAO_IDENT; }

  /// Returns the name of the property.
  const char* name (void) const;

private:

  TAO_Property_Constraint (const TAO_Property_Constraint&);
  TAO_Property_Constraint& operator= (const TAO_Property_Constraint&);

  /// The name of the property.
  char* name_;
};

/**
 * @class TAO_Literal_Constraint
 *
 * @brief TAO_Literal_Constraint represents a literal occuring in
 * the constraint expression tree.
 */
class TAO_Trading_Export TAO_Literal_Constraint : public TAO_Constraint
{
 public:

  TAO_Literal_Constraint (void)
    : type_ (TAO_UNKNOWN) {}

  // = Constructors for each of the various types of literals.

  TAO_Literal_Constraint (CORBA::Any* any);
  TAO_Literal_Constraint (CORBA::ULong uinteger);
  TAO_Literal_Constraint (CORBA::Long integer);
  TAO_Literal_Constraint (CORBA::Boolean boolean);
  TAO_Literal_Constraint (CORBA::Double doub);
  TAO_Literal_Constraint (const char* str);

  /// Copy constructor
  TAO_Literal_Constraint (const TAO_Literal_Constraint& lit);

  /// Destructor.
  ~TAO_Literal_Constraint(void);

  /// Visitor accept methods.
  virtual int accept (TAO_Constraint_Visitor* visitor);

  virtual TAO_Expression_Type expr_type (void) const
    { return type_; }

  /// Assignment operator.
  void operator= (const TAO_Literal_Constraint& co);

  // Conversion routines.
  operator CORBA::Boolean (void) const;
  operator CORBA::ULong (void) const;
  operator CORBA::Long (void) const;
  operator CORBA::Double (void) const;
  operator const char* (void) const;
  operator const CORBA::Any* (void) const;

  // Return the type represented by this MysteryOperand.

  // = Comparison operators.

  friend int
    operator< (const TAO_Literal_Constraint& left,
               const TAO_Literal_Constraint& right);

  friend int
    operator<= (const TAO_Literal_Constraint& left,
                const TAO_Literal_Constraint& right);

  friend int
    operator> (const TAO_Literal_Constraint& left,
               const TAO_Literal_Constraint& right);

  friend int
    operator>= (const TAO_Literal_Constraint& left,
                const TAO_Literal_Constraint& right);

  friend int
    operator== (const TAO_Literal_Constraint& left,
                const TAO_Literal_Constraint& right);

  friend int
    operator!= (const TAO_Literal_Constraint& left,
                const TAO_Literal_Constraint& right);

  friend int
    operator== (double left,
                const TAO_Literal_Constraint& right);

  friend int
    operator== (const TAO_String_Manager& left,
                const TAO_Literal_Constraint& right);

  // = Arithmetic operators.

  friend TAO_Literal_Constraint
    operator+ (const TAO_Literal_Constraint& left,
               const TAO_Literal_Constraint& right);

  friend TAO_Literal_Constraint
    operator- (const TAO_Literal_Constraint& left,
               const TAO_Literal_Constraint& right);

  friend TAO_Literal_Constraint
    operator* (const TAO_Literal_Constraint& left,
               const TAO_Literal_Constraint& right);

  friend TAO_Literal_Constraint
    operator/ (const TAO_Literal_Constraint& left,
               const TAO_Literal_Constraint& right);

  friend TAO_Literal_Constraint
    operator- (const TAO_Literal_Constraint& operand);

  /// Ensure both operands are of the same simple numeric type.
  static TAO_Expression_Type
    widest_type (const TAO_Literal_Constraint& left,
                 const TAO_Literal_Constraint& right);

  /// Determine the comparable Expression Type from the CORBA type
  static TAO_Expression_Type
    comparable_type (CORBA::TypeCode_ptr type);

 private:

  /// Private copy method.
  void copy (const TAO_Literal_Constraint& co);

  union
  {
    char* str_;
    CORBA::Any_ptr any_;
    CORBA::ULong uinteger_;
    CORBA::Long integer_;
    CORBA::Boolean bool_;
    CORBA::Double double_;
  } op_;
  // Union of the possible literal types.

  /// The actual types of the TAO_Literal_Constraint.
  TAO_Expression_Type type_;

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_CONSTRAINT_NODES_H */
