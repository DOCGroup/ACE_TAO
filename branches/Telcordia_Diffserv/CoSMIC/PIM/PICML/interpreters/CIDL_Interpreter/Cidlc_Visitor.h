// -*- C++ -*-

//=============================================================================
/**
 * @file      Cidlc_Visitor.h
 *
 * $Id$
 *
 * @author    Arvind Krishna
 */
//=============================================================================

#ifndef _CIDLC_VISITOR_H_
#define _CIDLC_VISITOR_H_

#include "PICML/PICML.h"
#include "CIDL_Export.h"

namespace PICML
{
  //===========================================================================
  /**
   * @class Cidlc_Visitor
   *
   * Backend generator of CIDL files from PICML models.
   */
  //===========================================================================

  class CIDL_Export Cidlc_Visitor: public Visitor
  {
  public:
    /**
     * Constructor
     *
     * @param[in]     output        Ouput path for the CIDL files.
     */
    Cidlc_Visitor (const std::string & output);

    /// Destructor.
    virtual ~Cidlc_Visitor (void);

  protected:
    /// Visit a RootFolder element.
    virtual void Visit_RootFolder (const PICML::RootFolder &);

    /// Visit a ComponentImplemenations element.
    virtual void Visit_ComponentImplementations (
      const PICML::ComponentImplementations &);

    /// Visit a ComponentImplemenationContainer element.
    virtual void Visit_ComponentImplementationContainer (
      const PICML::ComponentImplementationContainer &);

    /// Visit a MonolithicImplementation element.
    virtual void Visit_MonolithicImplementation (
      const PICML::MonolithicImplementation &);

    /// Visit a Component element.
    virtual void Visit_Component (const Component&);

    /// Visit a ComponentFactory element.
    virtual void Visit_ComponentFactory (const PICML::ComponentFactory &);

    virtual void Visit_ManagesComponent (const PICML::ManagesComponent &);

    virtual void Visit_ComponentRef (const PICML::ComponentRef &);

  private:
    /// Generate the scope of a named type.
    void generate_scope (const PICML::NamedType &);

    /// Generate the CIDL file based on the collected information.
    void generate_cidl_file (void);

    /// Output directory for the cidl files.
    std::string output_;

    /// Collection of information needed to generate the
    /// composition of a monolithic implemenation.
    struct cidl_t
    {
      /// Name of the CIDL file.
      std::string cidlfile_;

      /// IDL file containing the home.
      std::string idlfile_;

      /// Name of the monolithic implemenation.
      std::string monolithic_;

      /// Name of the component type.
      std::string component_;

      /// Name of the home managing the component.
      std::string home_;

      /// Scope of the home.
      std::string scope_;

      /// Flag contained the validity of the information.
      bool valid_;
    } cidl_;
  };
}

#endif  // !define _CIDLC_VISITOR_H_
