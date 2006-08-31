/**
 * Definition of the option category.
 *
 * @file OptionCategory.hpp
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef OPTION_CATEGORY_HPP
#define OPTION_CATEGORY_HPP

#include <list>
#include "Option.hpp"

namespace OCML
{

  /**
   * An option category is the container for other option categories and options.
   *
   * All the options are contained in an option category. Option categories form
   * a tree structure with only one root category.
   * <P>
   * This class provides STL compatible begin and end marks.
   */
  class OptionCategory: public DescribedItem
  {
  private:
    /// The container type for the contained options.
    typedef std::list<Option*> options_type;
    /// The container type for the contained categories.
    typedef std::list<OptionCategory*> categories_type;

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
    OptionCategory(const char* name, const char* description);
    /// Destructor.
    virtual ~OptionCategory();

    // Defined as abstract at DescribedItem.
    virtual const char* type_string() const;

    // First defined at DescribedItem.
    virtual xercesc::DOMElement* write(xercesc::DOMDocument* doc) const;

    /*
     * STL begin & end markers.
     */

    /// STL compatible begin marker for the contained categories.
    category_iterator begin_categories();
    /// STL compatible end marker for the contained categories.
    category_iterator end_categories();
    /// STL compatible begin marker for the contained options
    option_iterator begin_options();
    /// STL compatible end marker for the contained options.
    option_iterator end_options();

    /*
     * STL begin & end markers. - const versions -
     */

    /// STL compatible *const* begin marker for the contained categories.
    const_category_iterator begin_categories() const;
    /// STL compatible *const* end marker for the contained categories.
    const_category_iterator end_categories() const;
    /// STL compatible *const* begin marker for the contained options
    const_option_iterator begin_options() const;
    /// STL compatible *const* end marker for the contained options.
    const_option_iterator end_options() const;

    /// Inserts a new category to the categories list.
    void add_category(OptionCategory* category);
    /// Inserts a new option to the options list.
    void add_option(Option* option);

    /// Finds a category with given name in the category list and returns it.
    OptionCategory* category(const char* name);
    /// Finds an option with given name in the options list and returns it.
    Option* option(const char* name);

    /// Returns true if any of the contained item is assigned
    /// -checks the recursively the contained librraries-.
    bool items_assigned() const;

  private:
    /// The category container.
    categories_type categories_;
    /// The option container.
    options_type options_;
  };

} // namespace OCML

#include "OptionCategory.ipp"

#endif // OPTION_CATEGORY_HPP
