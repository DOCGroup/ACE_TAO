/**
 * @file Configurator.hpp
 *
 * Definition of the boolean edit control.
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef CONFIGURATOR_HPP
#define CONFIGURATOR_HPP

#include "Options/OptionCategory.hpp"

#include <string>
#include <memory>

#include "../Common/StringUtils.hpp"
#include "../Common/XercesHelpers.hpp"

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>

namespace OCML
{

  /**
   * Manages the Option tree.
   *
   * <UL>
   *   <LI> Reads and parses the XML Options file. </LI>
   *   <LI> Creates the option tree and destroys on self destruction. </LI>
   *   <LI> Exports the option tree into XML formatted string. </LI>
   * </UL>
   */
  class Configurator {
  public:
    /// Thrown on errors in constructor on XML errors.
    class InitializationError { };

    /// Parses the Options tree.
    Configurator(const char* tree_file);

    void initialize_options(const char* value_data, size_t size);

    /// Exports the option tree into the return value as XML tree.
    std::string write();
  
    /// Returns the root category.
    OptionCategory* root_category();

  private:
    /// Xerces environment. @see XercesSystem for more information.
    XercesSystem system_;
    /// The root of the option tree.
    std::auto_ptr<OptionCategory> root_;
  };

} // namespace OCML

#endif // CONFIGURATOR_HPP
