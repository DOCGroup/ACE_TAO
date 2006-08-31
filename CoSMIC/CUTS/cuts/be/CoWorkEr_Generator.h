#ifndef _UDM_COWORKER_GENERATOR_H_
#define _UDM_COWORKER_GENERATOR_H_

#include "BE_export.h"
#include "PICML/PICML.h"

//=============================================================================
/**
 * @class CUTS_UDM_CoWorkEr_Generator
 */
//=============================================================================

class CUTS_BE_Export CUTS_UDM_CoWorkEr_Generator :
  public PICML::Visitor
{
public:
  /// Default constructor.
  CUTS_UDM_CoWorkEr_Generator (void);

  /// Destructor.
  virtual ~CUTS_UDM_CoWorkEr_Generator (void);

private:
  //===========================================================================
  /**
   * @class Package_Visitor
   *
   * Specialized helper class for generating CoWorkErs located in
   * packages.
   */
  //===========================================================================

  class Package_Visitor : public PICML::Visitor
  {
  public:
    /**
     * Initializing constructor.
     *
     * @param[in]     parent      Parent of the CoWorkErs.
     */
    Package_Visitor (const PICML::Package & parent);

    /// Destructor.
    virtual ~Package_Visitor (void);

    /// Visit a PICML::Package element.
    void Visit_Package (const PICML::Package &);

    /// Visit a PICML::Component element.
    void Visit_Component (const PICML::Component &);

    /**
     * Get the number of CoWorkErs in the package.
     *
     * @return      Number of CoWorkErs.
     */
    size_t coworker_count (void) const;

  private:
    /// Parent package for all the CoWorkEr components.
    const PICML::Package & parent_;

    /// The current coworker.
    PICML::Component coworker_;

    /// Number of CoWorkErs in the package.
    size_t coworkers_;
  };

  /// Visit a PICML::RootFolder element.
  void Visit_RootFolder (const PICML::RootFolder &);

  /// Visit a PICML::InterfaceDefinitions element.
  void Visit_InterfaceDefinitions (const PICML::InterfaceDefinitions &);

  /// Visit a PICML::File element.
  void Visit_File (const PICML::File &);

  /// Visit a PICML::Component element.
  void Visit_Component (const PICML::Component &);

  PICML::InterfaceDefinitions coworker_idefs_;

  PICML::File parent_;

  PICML::Component coworker_;

  size_t coworkers_;
};

#endif  /* !defined _UDM_COWORKER_GENERATOR_H_ */
