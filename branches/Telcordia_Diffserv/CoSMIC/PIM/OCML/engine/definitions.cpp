#include "definitions.hpp"

namespace OCML
{

  Described_Item::~Described_Item()
  {
  }

  Option_Category::~Option_Category()
  {
    for (category_iterator c_iter = categories_.begin();
         c_iter != categories_.end(); ++c_iter)
      delete *c_iter;

    for (option_iterator o_iter = options_.begin();
         o_iter != options_.end(); ++o_iter)
      delete *o_iter;
  }

  /// @see DescribedItem::type_string
  const char*
  Option_Category::type_string() const
  {
    return "OptionCategory";
  }

  Option_Category*
  Option_Category::category(const char* name)
  {
    for (category_iterator iter = categories_.begin();
         iter != categories_.end(); ++iter)
      {
        if ((*iter)->name() == std::string(name))
          return *iter;
      }

    return NULL;
  }

  Option_Definition*
  Option_Category::option(const char* name)
  {
    for (option_iterator iter = options_.begin();
         iter != options_.end(); ++iter)
      {
        if ((*iter)->name() == std::string(name))
          return *iter;
      }

    return NULL;
  }

  /**
   * Traverses the contained options and categories, calls the write methods,
   * inserts the returning nodes to the current node as children.
   * 
   * @see DescribedItem::write.
   */
  //xercesc::DOMElement*
  //OptionCategory::write(xercesc::DOMDocument* doc) const
  //{
  //  if (!items_assigned())
  //    return NULL;
  //
  //  xercesc::DOMElement* element = DescribedItem::write(doc);
  //
  //  // Traverse the contained categories and call write methods.
  //  // 
  //  for (const_category_iterator c_iter = begin_categories();
  //       c_iter != end_categories(); ++c_iter)
  //    {
  //      xercesc::DOMElement* child = (*c_iter)->write(doc);
  //      // Append as child if the return value is not NULL.
  //      if (child)
  //        element->appendChild(child);
  //    }
  //
  //  // Traverse the contained options and call write methods.
  //  for (const_option_iterator o_iter = begin_options();
  //       o_iter != end_options(); ++o_iter)
  //    {
  //      xercesc::DOMElement* child = (*o_iter)->write(doc);
  //      // Append as child if the return value is not NULL.
  //      if (child)
  //        element->appendChild(child);
  //    }
  //  return element;
  //}

  //bool
  //Option_Category::items_assigned() const
  //{
  //  for (const_option_iterator c_iter = begin_options();
  //       c_iter != end_options(); ++c_iter)
  //    if ((*c_iter)->assigned())
  //      return true;
  //
  //  for (const_category_iterator o_iter = begin_categories();
  //       o_iter != end_categories(); ++o_iter)
  //    if ((*o_iter)->items_assigned())
  //      return true;
  //
  //  return false;
  //}

} // namespace OCML
