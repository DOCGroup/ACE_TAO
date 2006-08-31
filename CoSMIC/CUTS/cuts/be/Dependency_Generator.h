// -*- C++ -*-

//=============================================================================
/**
 * @file        Dependency_Generator.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _CUTS_DEPENDENCY_GENERATOR_H_
#define _CUTS_DEPENDENCY_GENERATOR_H_

#include "BE_export.h"
#include "PICML/PICML.h"

// Forward decl.
class CUTS_Dependency_Graph;

// Forward decl.
class CUTS_Dependency_Node;

//=============================================================================
/**
 * @class CUTS_Dependency_Generator
 *
 * @brief Walks a PICML model and creates a dependency graph.
 */
//=============================================================================

class CUTS_BE_Export CUTS_Dependency_Generator :
  public PICML::Visitor
{
public:
  /**
   * Initializing constructor.
   *
   * @param[in]     graph     Reference to target graph.
   */
  CUTS_Dependency_Generator (CUTS_Dependency_Graph & graph);

  // Destructor.
  ~CUTS_Dependency_Generator (void);

protected:
  /// Visit the RootFolder of a PICML model.
  void Visit_RootFolder (const PICML::RootFolder &);

  /// Visit InterfaceDefinitions folder in a PICML model.
  void Visit_InterfaceDefinitions (const PICML::InterfaceDefinitions &);

  /// Visit a File in a PICML model.
  void Visit_File (const PICML::File &);

  /// Visit a Package in a PICML model.
  void Visit_Package (const PICML::Package &);

  /// Visit a Component in a PICML model.
  void Visit_Component (const PICML::Component &);

  /// Visit an Object in a PICML model.
  void Visit_Object (const PICML::Object &);

  /// Visit an InEventPort of a PICML model.
  void Visit_InEventPort (const PICML::InEventPort &);

  /// Visit an OutEventPort of a PICML model.
  void Visit_OutEventPort (const PICML::OutEventPort &);

  /// Visit an ProvidedRequestPort of a PICML model.
  void Visit_ProvidedRequestPort (const PICML::ProvidedRequestPort &);

  /// Visit an RequiredRequestPort of a PICML model.
  void Visit_RequiredRequestPort (const PICML::RequiredRequestPort &);

  /// Visit a NameType element in a PICML model.
  void Visit_NamedType (const PICML::NamedType &);

  /// Visit an Event in a PICML model.
  void Visit_Event (const PICML::Event &);

  /// Visit a Providable element in a PICML model.
  void Visit_Providable (const PICML::Provideable &);

  /// Visit a Supports element in a PICML model.
  void Visit_Supports (const PICML::Supports &);

  /// Visit a ReadonlyAttribute in a PICML model.
  void Visit_ReadonlyAttribute (const PICML::ReadonlyAttribute &);

private:
  /**
   * Visit the contents of a File/Package element in a PICML
   * model.
   *
   * @param[in]     object        The source object.
   */
  void visit_file_and_package_contents (const Udm::Object & object);

  /// Target dependency graph.
  CUTS_Dependency_Graph & graph_;

  /// The current node in use.
  CUTS_Dependency_Node * current_node_;

  // prevent the following operations
  CUTS_Dependency_Generator (const CUTS_Dependency_Generator &);
  const CUTS_Dependency_Generator & operator = (const CUTS_Dependency_Generator &);
};

#endif  // !defined _CUTS_DEPENDENCY_GENERATOR_H_
