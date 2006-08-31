// $Id$

namespace OCML
{

/*
 *  Inline methods of the class DescribedItem
 */

/**
 *  Initializes the read only name and description attrubites.
 */
inline
DescribedItem::DescribedItem(const char* name, const char* description)
    : name_(name), description_(description)
{
}

inline const std::string&
DescribedItem::name() const
{
    return name_;
}

inline const std::string&
DescribedItem::description() const
{
    return description_;
}

inline bool
DescribedItem::operator==(const char* value) const
{
  return name_ == std::string(value);
}

} // namespace OCML
