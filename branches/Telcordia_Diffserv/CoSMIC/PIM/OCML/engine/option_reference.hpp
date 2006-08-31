#ifndef _OPTION_REFERENCE_HPP_
#define _OPTION_REFERENCE_HPP_

#include <string>
#include <list>
#include "option_value.hpp"
#include "definitions.hpp"

namespace OCML
{

  class OCML_Engine_Export Option_Reference
  {
  public:
    Option_Reference();

    /*
     * Set methods.
     */

    /// sets the assigned flag. @see assigned().
    void assigned(bool val);
    /// sets the required flag. @see required().
    void required(bool val);
    /// sets the allowed flag. @see allowed().
    void allowed(bool val);
    /// clear the option (assigned, required: returns false, allowed: true)
    void clear();

    /*
     * Get methods.
     */

    /// assigned() indicates if any value is assigned to this.
    bool assigned() const;
    /// required() means if this option needs to have a value.
    bool required() const;
    /// allowed() means if this option van have a value.
    bool allowed() const;
    /// if an options is required() but not assigned() flagged() returns true.
    bool flagged() const;
    /// creates and returns an option_value for the current value.
    virtual Option_Value* option_value() const = 0;

  private:
    bool assigned_;
    bool allowed_;
    bool required_;
  };

  template <typename OPTION_TRAIT>
  class Basic_Option_Reference: public Option_Reference
  {
  public:
    virtual ~Basic_Option_Reference();

    typedef OPTION_TRAIT option_trait;
    typedef typename option_trait::value_type value_type;

    /// Set method for the value.
    virtual void value(value_type val);

    /// Get method for the value.
    value_type value() const;

    /// Clears the present value.
    void clear();

    /// Checks if the given value is banned.
    bool value_banned(value_type value);

    /// Adds the value into the ban list.
    void ban_value(value_type value);

    /// Removes the value from the ban list.
    void allow_value(value_type value);

    /// creates and returns an option_value for the current value.
    virtual Option_Value* option_value() const;

  private:
    value_type value_;
    std::list<value_type> banned_values_;
  };

  typedef Basic_Option_Reference<Boolean_Option_Trait>
    Boolean_Option_Reference;
  typedef Basic_Option_Reference<Integer_Option_Trait>
    Integer_Option_Reference;
  typedef Basic_Option_Reference<String_Option_Trait>
    String_Option_Reference;

  class Option_Reference_Factory
  {
  public:
    class Unknown_Option_Kind {};

    template <typename OPTION_TRAIT>
    static Basic_Option_Reference<OPTION_TRAIT>*
    create_basic_option_reference();

    static Boolean_Option_Reference* create_boolean_option_reference();

    static Integer_Option_Reference* create_integer_option_reference();

    static String_Option_Reference* create_string_option_reference();

    static Option_Reference*
    create_option_reference(Option_Definition* definition);

  private:
    template <typename OPTION_TRAIT>
    static Option_Reference* try_create(Option_Definition* defn);
  };

} // namespace OCML

#include "option_reference.ipp"
#include "option_reference.tpp"

#endif // _OPTION_REFERENCE_HPP_
