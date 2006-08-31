namespace OCML
{

  /*
   *  class Option inline methods.
   */

  /**
   * Calls the DescribedItem constructor and initializes the value_kind
   * attribute.
   */
  inline
  Option::Option(value_kind_type value_kind,
                 const char* name, const char* description)
    : DescribedItem(name, description),
      value_kind_(value_kind)
  {
  }

  inline Option::value_kind_type
  Option::value_kind() const
  {
    return value_kind_;
  }

} // namespace OCML
