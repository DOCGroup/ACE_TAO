/* -*- C++ -*- */

// =====================================================================
// $Id$
//
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME
//    Constraint_Nodes.h
// 
// = AUTHOR 
//    Seth Widoff <sbw1@cs.wustl.edu> 
//   
// =====================================================================

#ifndef TAO_CONSTRAINT_NODES_H
#define TAO_CONSTRAINT_NODES_H

#include "Constraint_Visitor.h"
#include "Constraint_Tokens.h"

#include "orbsvcs/CosTradingC.h"
#include "orbsvcs/SequencesC.h"

typedef unsigned short TAO_Expression_Type;

class TAO_Constraint
//
// = TITLE
//     TAO_Constraint is the base class of all nodes on the
//     constraint expression tree.
//
// = DESCRIPTION
//     An TAO_Constraint knows what type of operation or entity
//     it represents, and which method on TAO_Constraint_Visitor
//     correlates to its type. When the accept method is invoked, a
//     subclass dispatches the method on an InterpreterVisitor
//     correlating to its type.
{
public:

  virtual int accept(TAO_Constraint_Visitor* visitor) = 0;
  // Implementing the pattern of double dispatching, each subclass of
  // TAO_Constraint will call back on an InterpreterVisitor the
  // method to handle a node of its ExpressionType.
  
  virtual TAO_Expression_Type expr_type(void) const = 0;
  // Return the expression type represented by this node.
  
  virtual ~TAO_Constraint(void) {}  
};

class TAO_Noop_Constraint : public TAO_Constraint
{
public:

  TAO_Noop_Constraint(TAO_Expression_Type type)
    : type_ (type) {}
  
  virtual int accept(TAO_Constraint_Visitor* visitor);

  virtual TAO_Expression_Type expr_type(void) const
    { return this->type_; }

private:

  TAO_Expression_Type type_;
};

class TAO_Binary_Constraint : public TAO_Constraint
//
// = TITLE
//      TAO_Binary_Constraint represents an operation with left
//      and right operands.
{
public:
  
  TAO_Binary_Constraint(TAO_Expression_Type op_type,
				  TAO_Constraint* left,
				  TAO_Constraint* right);

  virtual int accept(TAO_Constraint_Visitor* visitor);

  virtual ~TAO_Binary_Constraint(void);

  virtual TAO_Expression_Type expr_type(void) const
    { return this->op_; }
  
  TAO_Constraint* left_operand(void);
  // Return the left operand of the binary expression
  
  TAO_Constraint* right_operand(void);
  // Return the right operand of the binary expression

  // Allow double dispatching without creating an inundation of
  // classes by using a dispatch table of static method pointers to
  // invoke the correct visitor method as efficiently as a virtual
  // method invocation.
  static int visit_or(TAO_Constraint_Visitor*, TAO_Binary_Constraint*);
  static int visit_and(TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_less_than(TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_less_than_equal(TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_greater_than(TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_greater_than_equal(TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_equal(TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_not_equal(TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_add(TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_sub(TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_mult(TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_div(TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_twiddle(TAO_Constraint_Visitor* , TAO_Binary_Constraint*);
  static int visit_in(TAO_Constraint_Visitor* , TAO_Binary_Constraint*);  
  
private:
  
  TAO_Expression_Type op_;
  // The operator type
  
  TAO_Constraint* left_;
  TAO_Constraint* right_;
  // The operands of the expression
};

class TAO_Unary_Constraint : public TAO_Constraint
//
// = TITLE
//      TAO_Unary_Constraint represents an operation with only
//      one operand.
{
public:
  
  TAO_Unary_Constraint(TAO_Expression_Type op_type,
				 TAO_Constraint* operand);
  
  virtual ~TAO_Unary_Constraint(void);
  
  virtual int accept(TAO_Constraint_Visitor* visitor);

  virtual TAO_Expression_Type expr_type(void) const
    { return this->op_; }
  
  TAO_Constraint* operand(void);
  
private:

  TAO_Expression_Type op_;
  // The operator type
  
  TAO_Constraint* operand_;
  // The operand of the expression
};

class TAO_Property_Constraint : public TAO_Constraint
//
// = TITLE
//      TAO_Property_Constraint represents a property whose
//      value is determined by the offer being evaluated.
{
public:
  
  TAO_Property_Constraint(const char* name);

  virtual ~TAO_Property_Constraint(void);
  
  virtual int accept(TAO_Constraint_Visitor* visitor);

  virtual TAO_Expression_Type expr_type(void) const
    { return TAO_IDENT; }
  
  const char* name(void) const;
  // Returns the name of the property.
  
private:

  char* name_;
  // The name of the property.
};

class TAO_Literal_Constraint : public TAO_Constraint
//
// = TITLE
//     TAO_Literal_Constraint represents a literal occuring in
//     the constraint expression tree. 
{
 public:

  TAO_Literal_Constraint(void)
    : type_ (TAO_UNKNOWN) {}

  // = Constructors for each of the various types of literals.
  
  TAO_Literal_Constraint(CORBA::Any* any);
  
  TAO_Literal_Constraint(CORBA::ULong uinteger);

  TAO_Literal_Constraint(CORBA::Long integer);

  TAO_Literal_Constraint(CORBA::Boolean boolean);

  TAO_Literal_Constraint(CORBA::Double doub);
  
  TAO_Literal_Constraint(const char* str);

  TAO_Literal_Constraint (const TAO_Literal_Constraint& lit);
  
  ~TAO_Literal_Constraint(void);
  
  virtual int accept(TAO_Constraint_Visitor* visitor);
  
  virtual TAO_Expression_Type expr_type(void) const
    { return type_; }

  void operator= (const TAO_Literal_Constraint& co);
  
  // Conversion routines.
  operator CORBA::Boolean(void) const;
  operator CORBA::ULong(void) const;
  operator CORBA::Long(void) const; 
  operator CORBA::Double(void) const;
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
    operator== (const TAO_Sequences::StringSeq::Manager& left,
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
  
  static TAO_Expression_Type
    widest_type (const TAO_Literal_Constraint& left,
		 const TAO_Literal_Constraint& right);
  // Ensure both operands are of the same simple numeric type.
  
  static TAO_Expression_Type
    comparable_type (CORBA::TypeCode_ptr type);
  // Determine the comparable Expression Type from the CORBA type
  
 private:

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
  
  TAO_Expression_Type type_;
  // The actual types of the TAO_Literal_Constraint.
  
};

#endif /* TAO_CONSTRAINT_NODES_H */
