// $Id$

namespace OCML
{

  /*
   *  Inline methods of the class Described_Item
   */

  /**
   *  Initializes the read only name and description attrubites.
   */
  inline
  Described_Item::Described_Item(const char* name, const char* description)
      : name_(name), description_(description)
  {
  }

  inline const std::string&
  Described_Item::name() const
  {
    return name_;
  }

  inline const std::string&
  Described_Item::description() const
  {
    return description_;
  }

  inline bool
  Described_Item::operator==(const char* value) const
  {
    return name_ == std::string(value);
  }

  /**
   * Calls the DescribedItem constructor and initializes the value_kind
   * attribute.
   */
  inline
  Option_Definition::Option_Definition(const char* name,
                                             const char* description)
    : Described_Item(name, description)
  {
  }

  /*
   * option_category inline methods.
   */

  inline
  Option_Category::Option_Category(const char* name, const char* description)
    : Described_Item(name, description)
  {
  }

  inline void
  Option_Category::add_option(Option_Definition* option)
  {
    options_.push_back(option);
  }

  inline void
  Option_Category::add_category(Option_Category* category)
  {
    categories_.push_back(category);
  }

  /*
   * STL begin & end markers.
   */

  inline Option_Category::category_iterator
  Option_Category::begin_categories()
  {
    return categories_.begin();
  }

  inline Option_Category::category_iterator
  Option_Category::end_categories()
  {
    return categories_.end();
  }

  inline Option_Category::option_iterator
  Option_Category::begin_options()
  {
    return options_.begin();
  }

  inline Option_Category::option_iterator
  Option_Category::end_options()
  {
    return options_.end();
  }

  /*
   * STL begin & end markers. - const versions -
   */

  inline Option_Category::const_category_iterator
  Option_Category::begin_categories() const
  {
    return categories_.begin();
  }

  inline Option_Category::const_category_iterator
  Option_Category::end_categories() const
  {
    return categories_.end();
  }

  inline Option_Category::const_option_iterator
  Option_Category::begin_options() const
  {
    return options_.begin();
  }

  inline Option_Category::const_option_iterator
  Option_Category::end_options() const
  {
    return options_.end();
  }

} // namespace OCML
