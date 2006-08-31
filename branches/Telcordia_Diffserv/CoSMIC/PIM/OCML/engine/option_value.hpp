#ifndef _OPTION_VALUE_HPP_
#define _OPTION_VALUE_HPP_

#include <memory>
#include <sstream>
#include <string>
#include <set>

#include "option_traits.hpp"

namespace OCML
{
  class Null_Value_Exception { };

  class Value_Type_Mismatch { };

  class Option_Value
  {
  public:
    virtual ~Option_Value() { }

    /// Parses the given string and reads the value.
    void str_value(const char* value);

    /// Writes the value into given string.
    std::string str_value() const;

    /*
     * Pure virtual methods.
     */

    /// Returns true if the value is not assigned.
    virtual bool is_null() const = 0;
    /// Clears the value (consequent is_null's return true.)
    virtual void clear() = 0;
    /// Reads the value from given stream.
    virtual void read(std::istream& is) = 0;
    /// Writes the value into given stream.
    virtual void write(std::ostream& os) const = 0;
    /// Assigns the value by with type-checking.
    virtual void set(const Option_Value* option_value) = 0;
    /// Compares the given option value with this.
    virtual bool compare(const Option_Value* option_value) const = 0;
    /// Creates an appropriate Option_Value type and sets the value the returns.
    virtual Option_Value* duplicate() const = 0;

    bool operator==(const Option_Value& option_value) const;
    bool operator!=(const Option_Value& option_value) const;
  };

  template <typename OPTION_TRAIT>
  class Basic_Option_Value: public Option_Value
  {
  public:
    typedef OPTION_TRAIT option_trait;
    typedef typename option_trait::value_type value_type;

    /// Default constructor.
    Basic_Option_Value();

    /// Copy constructor.
    Basic_Option_Value(const Basic_Option_Value<option_trait>& value);

    /// Constructor accepting a value.
    Basic_Option_Value(const value_type& value);

    /// Constructor accepting a string (range).
    Basic_Option_Value(const char* first, const char* last);

    virtual ~Basic_Option_Value();

    /// Setter method.
    void value(const value_type& value);

    /// Getter method.
    value_type value() const;

    /*
     * Virtual method implementations.
     */
    
    virtual bool is_null() const;

    virtual void clear();

    virtual void read(std::istream& is);

    virtual void write(std::ostream& os) const;

    virtual void set(const Option_Value* option_value);
 
    virtual bool compare(const Option_Value* option_value) const;

    virtual Option_Value* duplicate() const;

  private:
    std::auto_ptr<value_type> value_;
  };

  typedef Basic_Option_Value<Boolean_Option_Trait> Boolean_Option_Value;
  typedef Basic_Option_Value<Integer_Option_Trait> Integer_Option_Value;
  typedef Basic_Option_Value<String_Option_Trait>  String_Option_Value;
  
} // namespace OCML

#include "option_value.ipp"
#include "option_value.tpp"

#endif // _OPTION_VALUE_HPP_
