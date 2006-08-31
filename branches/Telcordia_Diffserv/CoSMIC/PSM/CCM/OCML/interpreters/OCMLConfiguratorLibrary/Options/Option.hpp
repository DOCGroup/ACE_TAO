/**
 * @file Option.hpp
 *
 * $Id$
 *
 * Definition of the options.
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef OPTION_HPP
#define OPTION_HPP

#include <exception>
#include "DescribedItem.hpp"

namespace OCML
{

  /**
   * class Option is the base class for all the options.
   *
   * Provides value type information for run-time option value type-checking.
   */
  class Option: public DescribedItem
  {
  public:
    /// enum value_kind_type defines the option value types.
    enum value_kind_type {
      ovk_boolean,
      ovk_string,
      ovk_integer,
      ovk_enum
    };

    /// Exception: NotAssigned.
    class NotAssigned: public std::exception
    {
    public:
      virtual const char* what() const throw()
      {
        return "Option:NotAssigned";
      }
    };

  protected:
    /// protected Constructor.
    Option(value_kind_type value_kind, 
           const char* name, const char* description);

  public:
    // First defined at DescribedItem.
    virtual xercesc::DOMElement* write(xercesc::DOMDocument* doc) const;

    /// The getter method for the option value kind.
    value_kind_type value_kind() const;

    /// Clears the option so that assigned method return false.
    virtual void clear() = 0;

    /// Returns true if the option is assigned, false otherwise.
    virtual bool assigned() const = 0;

  private:
    /// The value kind of the option.
    value_kind_type value_kind_;
  };

} // namespace OCML

#include "Option.ipp"

#endif // OPTION_HPP
