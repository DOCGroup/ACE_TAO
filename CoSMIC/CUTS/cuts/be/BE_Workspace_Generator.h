// -*- C++ -*-

//=============================================================================
/**
 * @file      BE_Workspace_Generator.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_BE_WORKSPACE_GENERATOR_H_
#define _CUTS_BE_WORKSPACE_GENERATOR_H_

#include "BE_export.h"
#include "PICML/PICML.h"
#include "Dependency_Graph.h"
#include <fstream>
#include <memory>

// Forward decl.
class CUTS_BE_Project_Generator;

//=============================================================================
/**
 * @class CUTS_BE_Workspace_Generator
 */
//=============================================================================

class CUTS_BE_Export CUTS_BE_Workspace_Generator :
  public PICML::Visitor
{
public:
  /**
   * Initializing constructor.
   *
   * @param[in]     outdir      Output directory for the workspace.
   * @param[in]     project     Project generator.
   */
  CUTS_BE_Workspace_Generator (const std::string & outdir,
                               CUTS_BE_Project_Generator * project = 0);

  /// Destructor.
  virtual ~CUTS_BE_Workspace_Generator (void);

protected:
  /// Visit a RootFolder object.
  void Visit_RootFolder (
    const PICML::RootFolder &);

  /// Visit a ComponentImplementations object.
  void Visit_ComponentImplementations (
    const PICML::ComponentImplementations &);

  /// Visit a ComponentImplementationContainer object.
  void Visit_ComponentImplementationContainer (
    const PICML::ComponentImplementationContainer &);

  void Visit_MonolithicImplementation (
    const PICML::MonolithicImplementation &);

  void Visit_Component (
    const PICML::Component &);

private:
  void write_stub_project (CUTS_Dependency_Node * node);

  void write_stub_project_i (CUTS_Dependency_Node * node);

  /// Output directory.
  std::string outdir_;

  /// Target project generator.
  std::auto_ptr <CUTS_BE_Project_Generator> project_;

  /// Dependency graph for the project.
  CUTS_Dependency_Graph graph_;

  /// Workspace output file stream.
  std::ofstream workspace_;

  /// Flag specifying if current component is a CoWorkEr.
  bool coworker_;

  // prevent the following operations
  CUTS_BE_Workspace_Generator (const CUTS_BE_Workspace_Generator &);
  const CUTS_BE_Workspace_Generator & operator = (const CUTS_BE_Workspace_Generator &);
};

#endif  // !defined _CUTS_BE_WORKSPACE_GENERATOR_H_
