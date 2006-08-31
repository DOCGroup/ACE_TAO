/**
 * @file option.hpp
 *
 * $Id$
 *
 * Defines options with values.
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef _OPTION_HPP_
#define _OPTION_HPP_

#include <list>
#include <memory>
#include "option_traits.hpp"
#include "constraint_engine.hpp"

namespace OCML
{
  class Rule;
  class Option_Definition;
  class Option_Value;

  class Option_Selection_Rule;
  template <typename T> class Option_Equality_Rule;

  /**
   * class Option is the base class for all the options.
   *
   * Provides the methods for modifying the option values
   * with rule-checking.
   */
  class Option
  {
  protected:
    /// protected Constructor.
    Option(Option_Definition* option_definition,
           Constraint_Engine* constraint_manager);

  public:
    /// Clears the option so that assigned method return false.
    OCML_Engine_Export bool clear();

    /// Assigns the value of the option.
    OCML_Engine_Export bool value(const Option_Value* value);

    /// Assigns the value of the option by parsing the given string.
    OCML_Engine_Export virtual bool str_value(const char* str) = 0;

  private:
    Option_Definition* definition_;
    Constraint_Engine* constraint_manager_;
    bool assigned_;

    Constraint_Engine::rule_index equality_rule_;
  };

  template <typename OPTION_TRAIT>
  class Basic_Option: public Option
  {
  public:
    Basic_Option(Option_Definition* option_definition,
                 Constraint_Engine* constraint_engine);

    typedef OPTION_TRAIT option_trait;
    typedef typename option_trait::value_type value_type;

    virtual bool str_value(const char* str);
  };

  typedef Basic_Option<Boolean_Option_Trait> Boolean_Option;
  typedef Basic_Option<Integer_Option_Trait> Integer_Option;
  typedef Basic_Option<String_Option_Trait> String_Option;

} // namespace OCML

#include "option.ipp"
#include "option.tpp"

#endif // _OPTION_HPP_
