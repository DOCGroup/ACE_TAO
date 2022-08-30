/* -*- C++ -*- */

//=============================================================================
/**
 *  @file Constraint_Visitors.h
 *
 *  @author Seth Widoff <sbw1@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_CONSTRAINT_VISITOR_H
#define TAO_CONSTRAINT_VISITOR_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Trader/Interpreter_Utils.h"
#include "orbsvcs/Trader/trading_serv_export.h"
#include "ace/Containers.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_DynSequence_i;

class TAO_Constraint;
class TAO_Unary_Constraint;
class TAO_Binary_Constraint;
class TAO_Literal_Constraint;
class TAO_Property_Constraint;
class TAO_Noop_Constraint;

/**
 * @class TAO_Constraint_Visitor
 *
 * @brief This is the base class for all visitors who wish to preform
 * some operation from the state of the expression tree. Using
 * double dispatching, subclasses of Constraint expression call
 * back to the InterpreterVisitor subclass from the accept
 * method.
 *
 * Traversal of the expression tree uses the "Visitor"
 * pattern. To "visit" a node, a client invokes the "accept"
 * method on a subclass of ConstraintExpression, which, in turn,
 * invokes the appropriate method on the visitor passed to it,
 * based on its own type. So, the Constraint_Visitor has a
 * method to deal with each possible type of node in an
 * expression tree; one for each operator in the grammar.
 */
class TAO_Trading_Serv_Export TAO_Constraint_Visitor
{
public:
  virtual ~TAO_Constraint_Visitor () {}

  virtual int visit_constraint (TAO_Unary_Constraint* constraint) = 0;

  virtual int visit_with (TAO_Unary_Constraint* unary_with) = 0;
  virtual int visit_min (TAO_Unary_Constraint* unary_min) = 0;
  virtual int visit_max (TAO_Unary_Constraint* unary_max) = 0;
  virtual int visit_first (TAO_Noop_Constraint* noop_first) = 0;
  virtual int visit_random (TAO_Noop_Constraint* noop_random) = 0;

  virtual int visit_and (TAO_Binary_Constraint* boolean_and) = 0;
  virtual int visit_or (TAO_Binary_Constraint* boolean_or) = 0;
  virtual int visit_not (TAO_Unary_Constraint* unary_not) = 0;

  virtual int visit_exist (TAO_Unary_Constraint* unary_exist) = 0;
  virtual int visit_unary_minus (TAO_Unary_Constraint* unary_minus) = 0;

  virtual int visit_add (TAO_Binary_Constraint* boolean_add) = 0;
  virtual int visit_sub (TAO_Binary_Constraint* boolean_sub) = 0;
  virtual int visit_mult (TAO_Binary_Constraint* boolean_mult) = 0;
  virtual int visit_div (TAO_Binary_Constraint* boolean_div) = 0;

  virtual int visit_twiddle (TAO_Binary_Constraint* binary_twiddle) = 0;
  virtual int visit_in (TAO_Binary_Constraint* binary_in) = 0;

  virtual int visit_less_than (TAO_Binary_Constraint* boolean_lt) = 0;
  virtual int visit_less_than_equal (TAO_Binary_Constraint* boolean_lte) = 0;
  virtual int visit_greater_than (TAO_Binary_Constraint* boolean_gt) = 0;
  virtual int visit_greater_than_equal (TAO_Binary_Constraint* boolean_gte) = 0;
  virtual int visit_equal (TAO_Binary_Constraint* boolean_eq) = 0;
  virtual int visit_not_equal (TAO_Binary_Constraint* boolean_neq) = 0;

  virtual int visit_literal (TAO_Literal_Constraint* literal) = 0;
  virtual int visit_property (TAO_Property_Constraint* literal) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include "orbsvcs/Trader/Constraint_Nodes.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Constraint_Validator
 *
 * @brief TAO_Constraint_Validator ensures that in an expression tree
 * passed to it, the operands of each operation match the
 * correct types.
 *
 * TAO_Constraint_Validator uses the visitor pattern to
 * traverse all the nodes in an expression tree, checking that
 * for each operator node the operands are of the proper data
 * type it they're literals, or that they exist in the service
 * type definition _and_ have the proper type, if they're
 * property names. The algorithm for type
 * checking is as follows: ensure that operand expression(s)
 * return the correct types using expr_returns* methods. If they
 * (or it) return the correct types, call accept
 * on each operand until all return true or one returns false,
 * at which point we can back out of the traversal and indicate
 * failure.
 */
class TAO_Trading_Serv_Export TAO_Constraint_Validator : public TAO_Constraint_Visitor
{
public:
  /// Constructor.
  TAO_Constraint_Validator ();

  /// Destructor.
  ~TAO_Constraint_Validator () override;

  /**
   * Validate returns 1 if the expression tree whose root is <root>
   * makes semantic sense, in that the operands for each operation
   * comply with each other and the types accepted by the operator.
   */
  int validate (TAO_Constraint* root);

  // = Visitor Methods

  int visit_constraint (TAO_Unary_Constraint* constraint) override;

  int visit_with (TAO_Unary_Constraint* unary_with) override;
  int visit_min (TAO_Unary_Constraint* unary_min) override;
  int visit_max (TAO_Unary_Constraint* unary_max) override;
  int visit_first (TAO_Noop_Constraint* noop_first) override;
  int visit_random (TAO_Noop_Constraint* noop_random) override;

  /// The two operands must return a boolean value.
  int visit_and (TAO_Binary_Constraint* boolean_and) override;
  int visit_or (TAO_Binary_Constraint* boolean_or) override;

  /// The operand must return a boolean value.
  int visit_not (TAO_Unary_Constraint* unary_not) override;

  /// The operand must return a valid  (i.e., present in the service
  /// type description) property name.
  int visit_exist (TAO_Unary_Constraint* unary_exist) override;

  /// The operand must return a number to be negated.
  int visit_unary_minus (TAO_Unary_Constraint* unary_minus) override;

  /// Both operands must return numeric results.
  int visit_add (TAO_Binary_Constraint* boolean_add) override;
  int visit_sub (TAO_Binary_Constraint* boolean_sub) override;
  int visit_mult (TAO_Binary_Constraint* boolean_mult) override;
  int visit_div (TAO_Binary_Constraint* boolean_div) override;

  /// Both operands must return strings.
  int visit_twiddle (TAO_Binary_Constraint* binary_twiddle) override;

  /// The right operand must be a sequence of the same simple type as
  /// the left operand.
  int visit_in (TAO_Binary_Constraint* binary_in) override;

  /// The left and right operands must both be of the same simple type.
  int visit_less_than (TAO_Binary_Constraint* boolean_lt) override;
  int visit_less_than_equal (TAO_Binary_Constraint* boolean_lte) override;
  int visit_greater_than (TAO_Binary_Constraint* boolean_gt) override;
  int visit_greater_than_equal (TAO_Binary_Constraint* boolean_gte) override;
  int visit_equal (TAO_Binary_Constraint* boolean_eq) override;
  int visit_not_equal (TAO_Binary_Constraint* boolean_neq) override;

  /// The property must be defined in the service type description.
  int visit_literal (TAO_Literal_Constraint* literal) override;
  int visit_property (TAO_Property_Constraint* literal) override;

protected:
  /// A map gleaned from the ServiceTypeStruct, which correlates
  /// property names with their types.
  TAO_Typecode_Table type_map_;

private:
  CORBA::TypeCode* extract_type (TAO_Constraint* expr_type,
                                 TAO_Expression_Type& type);

  /// expr_returns_boolean returns 1 if <expr_type>, when evaluated, will
  /// return a boolean.
  int expr_returns_boolean (TAO_Expression_Type expr_type);

  /// expr_returns_boolean returns 1 if <expr_type>, when evaluated, will
  /// return a number.
  int expr_returns_number (TAO_Expression_Type expr_type);

  /// expr_returns_boolean returns 1 if <expr_type>, when evaluated, will
  /// return a string.
  int expr_returns_string (TAO_Expression_Type expr_type);

  TAO_Constraint_Validator (const TAO_Constraint_Validator&);
  TAO_Constraint_Validator& operator= (const TAO_Constraint_Validator&);
};

/**
 * @class TAO_Constraint_Evaluator
 *
 * @brief TAO_Constraint_Evaluator traverse a constraint expression
 * tree, and determines whether an offer fits the constraints
 * represented by the tree
 *
 * Using the Visitor pattern, the TAO_Constraint_Evaluator has
 * each node of the expression tree call back to it with the
 * method designated for its type. In that method, the visitor
 * will evaluate its operands and perform the operation
 * designated by that node's type, and return the result. Note:
 * the TAO_Constraint_Evaluator assumes the tree is semantically
 * correct, that is, the validate method on
 * TAO_Constraint_Validator return true. The only possible
 * evaluation time errors are a divide by a property whose value
 * is zero and undefined properties.
 */
class TAO_Trading_Serv_Export TAO_Constraint_Evaluator : public TAO_Constraint_Visitor
{
public:
  /// Constructor.
  TAO_Constraint_Evaluator ();

  /**
   * Evaluate returns 1 if the offer satisfies the constraints
   * represented by the the expression tree rooted at <root>, 0 if it
   * doesn't. If an error occurs during process, the constraint
   * automatically fails.
   */
  CORBA::Boolean evaluate_constraint (TAO_Constraint* root);

  /// The result of the preference evaluation is stored in result. The
  /// method returns 0 upon success, -1 upon failure.
  int evaluate_preference (TAO_Constraint* root,
                           TAO_Literal_Constraint& result);

  // = Visitor Methods

  int visit_constraint (TAO_Unary_Constraint* constraint) override;

  int visit_with (TAO_Unary_Constraint* unary_with) override;
  int visit_min (TAO_Unary_Constraint* unary_min) override;
  int visit_max (TAO_Unary_Constraint* unary_max) override;
  int visit_first (TAO_Noop_Constraint* noop_first) override;
  int visit_random (TAO_Noop_Constraint* noop_random) override;

  /**
   * Takes the logical and of the results of both operands. Note that
   * in the case where the left operand returns zero, the result is
   * immediately known.
   */
  int visit_and (TAO_Binary_Constraint* boolean_and) override;

  /**
   * Takes the logical or of the results of both operands. Note that
   * in the case where the left operand returns one, the result is
   * immediately known.
   */
  int visit_or (TAO_Binary_Constraint* boolean_or) override;

  /// Logically negates the value of the operand.
  int visit_not (TAO_Unary_Constraint* unary_not) override;

  /// The property exists if its name is bound to a value in the
  /// <props_> map.
  int visit_exist (TAO_Unary_Constraint* unary_exist) override;

  /// Mathematically negates the return value the operand.
  int visit_unary_minus (TAO_Unary_Constraint* unary_minus) override;

  /// Add the results of evaluating the left and right operands.
  int visit_add (TAO_Binary_Constraint* boolean_add) override;

  /// Subtract the results of evaluating the left and right operands.
  int visit_sub (TAO_Binary_Constraint* boolean_sub) override;

  /// Multiply the results of evaluating the left and right operands.
  int visit_mult (TAO_Binary_Constraint* boolean_mult) override;

  /// Divide the results of evaluating the left and right operands.
  int visit_div (TAO_Binary_Constraint* boolean_div) override;

  /// Determines if the right operand is a substring of the left.
  int visit_twiddle (TAO_Binary_Constraint* binary_twiddle) override;

  /// Determines if the sequence represented by the right operand
  /// contains the left operand.
  int visit_in (TAO_Binary_Constraint* binary_in) override;

  // = Compare the results of evaluating left and right operands.
  int visit_less_than (TAO_Binary_Constraint* boolean_lt) override;
  int visit_less_than_equal (TAO_Binary_Constraint* boolean_lte) override;
  int visit_greater_than (TAO_Binary_Constraint* boolean_gt) override;
  int visit_greater_than_equal (TAO_Binary_Constraint* boolean_gte) override;
  int visit_equal (TAO_Binary_Constraint* boolean_eq) override;
  int visit_not_equal (TAO_Binary_Constraint* boolean_neq) override;

  /// Copy the value of the literal into the result container.
  int visit_literal (TAO_Literal_Constraint* literal) override;

  /// Copy the value of the property into the result container.
  int visit_property (TAO_Property_Constraint* literal) override;

private:
  class TAO_Trading_Serv_Export Operand_Queue :
    public ACE_Unbounded_Queue <TAO_Literal_Constraint>
  // = TITLE
  // A queue adapter with methods to setting and getting operands
  // from the expression evaluation results.
  {
  public:
    Operand_Queue  ();

    /// In a binary operation, obtain the left operand.
    TAO_Literal_Constraint& get_left_operand ();

    /// In a binary operation, obtain the right operand.
    TAO_Literal_Constraint& get_right_operand ();

    /// In a unary operation, obtain the only operand.
    TAO_Literal_Constraint& get_operand ();

    /// Remove an operand from the queue.
    void dequeue_operand ();
  };

  /// Method for performing a arithmetic or comparison operation.
  void do_the_op (int operation);

  /// Method for evaluating a binary operation.
  int visit_bin_op (TAO_Binary_Constraint* op, int operation);

  /// Determine if sequence contains @a element, a literal of the same
  /// simple type as <sequence_type>. Return true in this eventuality.
  CORBA::Boolean sequence_does_contain (CORBA::Any* sequence,
                                        TAO_Literal_Constraint& element);

  TAO_Constraint_Evaluator (const TAO_Constraint_Evaluator&) = delete;
  TAO_Constraint_Evaluator& operator= (const TAO_Constraint_Evaluator&) = delete;
  TAO_Constraint_Evaluator (TAO_Constraint_Evaluator&&) = delete;
  TAO_Constraint_Evaluator& operator= (TAO_Constraint_Evaluator&&) = delete;

protected:
  /// The map of property names to their values for a property.
  TAO_Lookup_Table props_;

  /// The result of a non_boolean operation.
  Operand_Queue queue_;
};

// Forward declaration
template <class ELEMENT_TYPE> class TAO_Element_Equal;

template<>
class TAO_Element_Equal<CORBA::Short>
{
public:
  /// Calls the correct method on dyn_seq to extract the element type,
  /// then uses the appropriate form of equals comparison.
  int operator () (TAO_DynSequence_i& dyn_any,
                   CORBA::Short element) const;
};

template<>
class TAO_Element_Equal<CORBA::UShort>
{
public:
  /// Calls the correct method on dyn_seq to extract the element type,
  /// then uses the appropriate form of equals comparison.
  int operator () (TAO_DynSequence_i& dyn_any,
                   CORBA::UShort element) const;
};

template<>
class TAO_Element_Equal<CORBA::Long>
{
public:
  /// Calls the correct method on dyn_seq to extract the element type,
  /// then uses the appropriate form of equals comparison.
  int operator () (TAO_DynSequence_i& dyn_any,
                   CORBA::Long element) const;
};

template<>
class TAO_Element_Equal<CORBA::ULong>
{
public:
  /// Calls the correct method on dyn_seq to extract the element type, then
  /// uses the appropriate form of equals comparison.
  int operator () (TAO_DynSequence_i& dyn_any,
                   CORBA::ULong element) const;

};

template<>
class TAO_Element_Equal<CORBA::LongLong>
{
public:
  /// Calls the correct method on dyn_seq to extract the element type,
  /// then uses the appropriate form of equals comparison.
  int operator () (TAO_DynSequence_i& dyn_any,
                   CORBA::LongLong element) const;
};

template<>
class TAO_Element_Equal<CORBA::ULongLong>
{
public:
  /// Calls the correct method on dyn_seq to extract the element type, then
  /// uses the appropriate form of equals comparison.
  int operator () (TAO_DynSequence_i& dyn_any,
                   CORBA::ULongLong element) const;
};

template<>
class TAO_Element_Equal<CORBA::Float>
{
public:
  /// Calls the correct method on dyn_seq to extract the element type,
  /// then uses the appropriate form of equals comparison.
  int operator () (TAO_DynSequence_i& dyn_any,
                   CORBA::Float element) const;
};

template<>
class TAO_Element_Equal<CORBA::Double>
{
public:
  /// Calls the correct method on dyn_seq to extract the element type,
  /// then uses the appropriate form of equals comparison.
  int operator () (TAO_DynSequence_i& dyn_any,
                   CORBA::Double element) const;
};

template<>
class TAO_Element_Equal<CORBA::Boolean>
{
public:
  /// Calls the correct method on dyn_seq to extract the element type,
  /// then uses the appropriate form of equals comparison.
  int operator () (TAO_DynSequence_i& dyn_any,
                   CORBA::Boolean element) const;
};

template<>
class TAO_Element_Equal<const char*>
{
public:
  /// Calls the correct method on dyn_seq to extract the element type,
  /// then uses the appropriate form of equals comparison.
  int operator () (TAO_DynSequence_i& dyn_any,
                   const char* element) const;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* CONSTRAINT_VISITORS_H */
