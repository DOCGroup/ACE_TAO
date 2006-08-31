#ifndef _OPTION_RULES_HPP_
#define _OPTION_RULES_HPP_

#include "rule.hpp"
#include "option_reference.hpp"

namespace OCML
{

  class OCML_Engine_Export Option_Selection_Rule
    : public Basic_Rule<Option_Selection_Rule>
  {
  public:
    Option_Selection_Rule(Rule* root, Option_Reference* option);

    virtual bool test() const;

    virtual bool apply(bool negate);

  private:
    /// (not negate) Tries to set the required flag to true.
    bool make_select();

    /// (negate) Tries to set the allowed flag to false.
    bool make_not_select();

  protected:
    Option_Reference* option_;
  };

  /**
   * Value_Equality_Rule is used to check/enforce the equality of the given
   * option to a constant value.
   */
  template <typename OPTION_TRAIT>
  class Value_Equality_Rule
    : public Basic_Rule< Value_Equality_Rule<OPTION_TRAIT> >
  {
  public:
    typedef OPTION_TRAIT option_trait;
    typedef typename option_trait::value_type value_type;

  public:
    Value_Equality_Rule(Rule* root,
                        Basic_Option_Reference<option_trait>* option,
                        value_type value);
    virtual ~Value_Equality_Rule();

    virtual bool test() const;
    virtual bool apply(bool negate);

  private:
    /// (not negate) Tries to set the assigned flag and set option value to
    /// the given value.
    bool make_equal();

    /// (negate) Tries to add the value into the ban list.
    bool make_not_equal();
    
  private:
    Basic_Option_Reference<option_trait>* option_;
    value_type value_;
  };

  /**
   * Option_Equality_Rule is used to check/enforce the equality of the given
   * option to another option value.
   */

  template <typename OPTION_TRAIT>
  class Option_Equality_Rule
    : public Basic_Rule< Option_Equality_Rule<OPTION_TRAIT> >
  {
  public:
    typedef OPTION_TRAIT option_trait;
    typedef typename option_trait::value_type value_type;

  public:
    Option_Equality_Rule(Basic_Option_Reference<option_trait>* option1,
                       Basic_Option_Reference<option_trait>* option2);

    virtual bool test() const;
    virtual bool apply(bool negate);

  private:
    // Enforce: lvalue = rvalue;
    bool equate(Basic_Option_Reference<option_trait>* rvalue,
                Basic_Option_Reference<option_trait>* lvalue);

    // Prevent: lvalue = rvalue;
    bool unequate(Basic_Option_Reference<option_trait>* rvalue,
                  Basic_Option_Reference<option_trait>* lvalue);

    // (not negate) If one of the options has a value assigned
    // tries to set the other one to the same value.
    bool make_equal();

    // (negate) If one of the options has a value tries to
    // add that value into the ban list of the other option.
    bool make_not_equal();

  private:
    Basic_Option_Reference<option_trait>* option1_;
    Basic_Option_Reference<option_trait>* option2_;
  };

} // namespace OCML

#include "option_rules.ipp"
#include "option_rules.tpp"

#endif //  _OPTION_RULES_HPP_
