/**
 * @file BasicOptions.hpp
 *
 * $Id$
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 *
 * Definition of the basic (integer, string, boolean) options.
 */

#ifndef BASIC_OPTIONS_HPP
#define BASIC_OPTIONS_HPP

#include <string>
#include <memory>
#include <exception>
#include <list>

#include "Option.hpp"

namespace OCML
{

  /**
   * class Base_Option is the base class for boolean, integer, string, and enum
   * options.
   *
   * Template argument VALUE_TYPE specifies the data type of the option value.
   */
  template <typename VALUE_TYPE>
  class Base_Option: public Option
  {
  protected:
    /// Constructor.
    Base_Option(value_kind_type value_kind,
                const char* name, const char* description);


    /// Template value type.
    typedef VALUE_TYPE value_type;

  public:
    /// Descructor.
    virtual ~Base_Option();

    // First defined at DescribedItem, reimplemented at Option.
    virtual xercesc::DOMElement* write(xercesc::DOMDocument* doc) const;

    // Defined as abstract at Option.
    virtual void clear();

    // Defined as abstract at Option.
    virtual bool assigned() const;

    /// Set method for the option value.
    virtual void set(value_type value);

    /// Getter method for the option value. Throws NotAssigned.
    virtual value_type get() const;

    /// Returns the value as string to export files or display via GUI widgets.
    virtual std::string get_str_value() const = 0;

  private:
    /// Pointer to the option value.
    std::auto_ptr<value_type> value_;
  };

  /**
   * class Basic_Option is the base class for boolean, integer, and string
   * options.
   *
   * Template argument VALUE_TYPE specifies the data type of the option value.
   */
  template <typename VALUE_TYPE>
  class Basic_Option: public Base_Option<VALUE_TYPE>
  {
  protected:
    /// Constructor.
    Basic_Option(value_kind_type value_kind,
                 const char* name, const char* description,
                 value_type def_value);

  public:
    /// Returns the default value.
    value_type default_value();

  private:
    value_type default_value_;
  };

  /**
   * Option for boolean values.
   */
  class BooleanOption: public Basic_Option<bool>
  {
  public:
    /// Constructor.
    BooleanOption(const char* name, const char* description,
                  value_type default_value);

    // Defined as abstract at DescribedItem.
    virtual const char* type_string() const;

    // Defined as abstract at Base_Option.
    virtual std::string get_str_value() const;
  };

  /**
   * Option for string values.
   */
  class StringOption: public Basic_Option<std::string>
  {
  public:
    /// Constructor.
    StringOption(const char* name, const char* description,
                 value_type default_value);

    // Defined as abstract at DescribedItem.
    const char* type_string() const;

    // Defined as abstract at Base_Option.
    virtual std::string get_str_value() const;
  };

  /**
   * Option for integer values.
   */
  class IntegerOption: public Basic_Option<int>
  {
  public:
    /// Constructor.
    IntegerOption(const char* name, const char* description,
                  value_type default_value);

    // Defined as abstract at DescribedItem.
    const char* type_string() const;

    // Defined as abstract at Base_Option.
    virtual std::string get_str_value() const;
  };

  /**
   * Option for enumerated values.
   */
  class EnumOption: public Base_Option<std::string>
  {
  private:
    typedef std::list<value_type> items_type;

  public:
    typedef items_type::iterator item_iterator;

    class InvalidItem: public std::exception
    {
    public:
      InvalidItem(const std::string& msg);

      virtual ~InvalidItem() throw();

      virtual const char* what() const throw();

    private:
      std::string msg_;
    };

  public:
    /// Constructor.
    EnumOption(const char* name, const char* description);

    /// Defined as abstract at DescribedItem.
    const char* type_string() const;

    /// Defined at Base_Option, reimplemented by EnumOption.
    virtual void set(value_type value);

    /// Defined as abstract at Base_Option.
    virtual std::string get_str_value() const;

    /// Appends a new item to the items list.
    void add_item(const char* item_name, bool default_value = false);

    /// Sets the given item as default.
    void set_default(item_iterator item);

    /// Returns the iterator for the default item.
    item_iterator get_default();

    /// Returns if this enum option has a default value.
    bool has_default();

    /// Checks if the given item is the default item.
    bool is_default(item_iterator item);

    /// Begin marker for the enum items
    item_iterator begin_items();
    /// End marker for the enum items
    item_iterator end_items();

  private:
    /// Items container.
    items_type items_;
    /// True if this enum option has a default value at all.
    bool has_default_;
    /// Points to the container element which is default.
    item_iterator default_;
  };

} // namespace OCML

#include "BasicOptions.ipp"
#include "BasicOptions.tpp"

#endif // BASIC_OPTIONS_HPP
