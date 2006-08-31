/**
 * Defines the class DescribedItem.
 *
 * @file DescribedItem.hpp
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef DESCRIBED_ITEM_HPP
#define DESCRIBED_ITEM_HPP

#include <string>
#include <iostream>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include "../../Common/StringUtils.hpp"

namespace OCML
{

  /**
   * class DescribedItem provides name and description attributes for the
   * options, categories, and option items.
   *
   * The name and description attributes are specified at the initialization time
   * as the parameters of the constructor.  They cannot be changed after
   * initialization.
   */
  class DescribedItem
  {
  public:
    /// Constructor.
    DescribedItem(const char* name, const char* description);
    /// Desctructor.
    virtual ~DescribedItem();

    /// Getter method for name attribute.
    const std::string& name() const;

    /// Getter method for description attribute.
    const std::string& description() const;

    /// Returns the type of the described item as a char sequence.
    virtual const char* type_string() const = 0;

    /// Creates the corresponding DOMElement for saving the value.
    virtual xercesc::DOMElement* write(xercesc::DOMDocument* doc) const;

    /// Returns true if name is equal to the item's name.
    bool operator==(const char* value) const;

  private:
    /// The name attribute.
    std::string name_;

    /// The description attribute.
    std::string description_;
  };

} // namespace OCML

#include "DescribedItem.ipp"

#endif // DESCRIBED_ITEM_HPP
