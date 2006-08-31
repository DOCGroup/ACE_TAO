/**
 * Contains the definitions of the Option_Definition and Option_Category.
 *
 * $Id$
 *
 * @file definitions.hpp
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef _DEFINITIONS_HPP_
#define _DEFINITIONS_HPP_

#include "OCML_Engine_Export.h"

#include <string>
#include <iostream>
#include <list>
#include "option_traits.hpp"

namespace OCML
{

  /**
   * class Described_Item provides name and description attributes for the
   * options definitions and options categories.
   *
   * The name and description attributes are specified at the initialization
   * time as the parameters of the constructor.  They cannot be changed after
   * initialization.
   */
  class OCML_Engine_Export Described_Item
  {
  public:
    /// Constructor.
    Described_Item(const char* name, const char* description);
    /// Desctructor.
    virtual ~Described_Item();

    /// Getter method for name attribute.
    const std::string& name() const;

    /// Getter method for description attribute.
    const std::string& description() const;

    /// Returns the type of the described item as a char sequence.
    virtual const char* type_string() const = 0;

    /// Returns true if name is equal to the item's name.
    bool operator==(const char* value) const;

  private:
    /// The name attribute.
    std::string name_;

    /// The description attribute.
    std::string description_;
  };

  /**
   * class Option_Definition is the base class for all the option definitions.
   *
   * Option definition simply defines a name for the options.
   */
  class Option_Definition: public Described_Item
  {
  protected:
    /// protected Constructor.
    Option_Definition(const char* name, const char* description);
  };

  /**
   * Basic_Option_Definition provides the interface for the default value.
   */
  template <typename OPTION_TRAIT>
  class Basic_Option_Definition: public Option_Definition
  {
  public:
    /// Typedef for the template parameter option type info.
    typedef OPTION_TRAIT option_trait;
    /// Typedef for the C++ type.
    typedef typename option_trait::value_type value_type;

  public:
    /// Constructor. 
    Basic_Option_Definition(const char* name, const char* description);

    /// Constructor. 
    Basic_Option_Definition(const char* name, const char* description,
                            value_type option_default_value);

    /// Get method for the default value.
    const value_type& default_value() const;

    /// Returns the default value as string.
    virtual const char* type_string() const;

  private:
    /// Member variable for holding default value.
    value_type default_value_;
  };

  typedef Basic_Option_Definition<Integer_Option_Trait>
    Integer_Option_Definition;

  typedef Basic_Option_Definition<Boolean_Option_Trait>
    Boolean_Option_Definition;

  typedef Basic_Option_Definition<String_Option_Trait>
    String_Option_Definition;

  /**
   * An option category is the container for other option categories and
   * option definitions. 
   *
   * All the option definitions are contained in an option category. Option
   * categories form a tree structure with only one root category.
   * <P>
   * This class provides STL compatible begin and end marks.
   *
   * @see Option_Definition.
   */
  class Option_Category: public Described_Item
  {
  private:
    /// The container type for the contained options.
    typedef std::list<Option_Definition*> options_type;
    /// The container type for the contained categories.
    typedef std::list<Option_Category*> categories_type;

  public:
    /// Iterator for the categories list.
    typedef categories_type::iterator category_iterator;
    /// Iterator for the options list.
    typedef options_type::iterator option_iterator;
    /// Const iterator for the categories list.
    typedef categories_type::const_iterator const_category_iterator;
    /// Const iterator fot the options list.
    typedef options_type::const_iterator const_option_iterator;
    
  public:
    /// Contructor.
    OCML_Engine_Export Option_Category(const char* name, const char* description);
    /// Destructor.
    OCML_Engine_Export virtual ~Option_Category();

    // Defined as abstract at DescribedItem.
    OCML_Engine_Export virtual const char* type_string() const;

    // First defined at DescribedItem.
//    virtual xercesc::DOMElement* write(xercesc::DOMDocument* doc) const;

    /*
     * STL begin & end markers.
     */

    /// STL compatible begin marker for the contained categories.
    OCML_Engine_Export category_iterator begin_categories();
    /// STL compatible end marker for the contained categories.
    OCML_Engine_Export category_iterator end_categories();
    /// STL compatible begin marker for the contained options
    OCML_Engine_Export option_iterator begin_options();
    /// STL compatible end marker for the contained options.
    OCML_Engine_Export option_iterator end_options();

    /*
     * STL begin & end markers. - const versions -
     */

    /// STL compatible *const* begin marker for the contained categories.
    OCML_Engine_Export const_category_iterator begin_categories() const;
    /// STL compatible *const* end marker for the contained categories.
    OCML_Engine_Export const_category_iterator end_categories() const;
    /// STL compatible *const* begin marker for the contained options
    OCML_Engine_Export const_option_iterator begin_options() const;
    /// STL compatible *const* end marker for the contained options.
    OCML_Engine_Export const_option_iterator end_options() const;

    /// Inserts a new category to the categories list.
    OCML_Engine_Export void add_category(Option_Category* category);
    /// Inserts a new option to the options list.
    OCML_Engine_Export void add_option(Option_Definition* option);

    /// Finds a category with given name in the category list and returns it.
    OCML_Engine_Export Option_Category* category(const char* name);
    /// Finds an option with given name in the options list and returns it.
    OCML_Engine_Export Option_Definition* option(const char* name);

    /// Returns true if any of the contained item is assigned
    /// -checks the recursively the contained librraries-.
//    bool items_assigned() const;

  private:
    /// The category container.
    categories_type categories_;
    /// The option container.
    options_type options_;
  };

} // namespace OCML

#include "definitions.ipp"
#include "definitions.tpp"

#endif // _OPTION_CATEGORY_HPP_
