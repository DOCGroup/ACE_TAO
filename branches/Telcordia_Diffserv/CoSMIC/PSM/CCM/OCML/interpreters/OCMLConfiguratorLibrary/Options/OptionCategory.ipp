namespace OCML
{

  /*
   * OptionCategory inline methods.
   */

  inline
  OptionCategory::OptionCategory(const char* name, const char* description)
    : DescribedItem(name, description)
  {
  }

  /*
   * STL begin & end markers.
   */

  inline OptionCategory::category_iterator
  OptionCategory::begin_categories()
  {
    return categories_.begin();
  }

  inline OptionCategory::category_iterator
  OptionCategory::end_categories()
  {
    return categories_.end();
  }

  inline OptionCategory::option_iterator
  OptionCategory::begin_options()
  {
    return options_.begin();
  }

  inline OptionCategory::option_iterator
  OptionCategory::end_options()
  {
    return options_.end();
  }

  /*
   * STL begin & end markers. - const versions -
   */

  inline OptionCategory::const_category_iterator
  OptionCategory::begin_categories() const
  {
    return categories_.begin();
  }

  inline OptionCategory::const_category_iterator
  OptionCategory::end_categories() const
  {
    return categories_.end();
  }

  inline OptionCategory::const_option_iterator
  OptionCategory::begin_options() const
  {
    return options_.begin();
  }

  inline OptionCategory::const_option_iterator
  OptionCategory::end_options() const
  {
    return options_.end();
  }

  inline void
  OptionCategory::add_option(Option* option)
  {
    options_.push_back(option);
  }

  inline void
  OptionCategory::add_category(OptionCategory* category)
  {
    categories_.push_back(category);
  }

} // namespace OCML
