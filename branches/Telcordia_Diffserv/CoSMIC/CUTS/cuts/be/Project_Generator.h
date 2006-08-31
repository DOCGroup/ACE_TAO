// $Id$

//=============================================================================
/**
 * @file      Project_Generator.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_PROJECT_GENERATOR_H_
#define _CUTS_PROJECT_GENERATOR_H_

#include "BE_File_Generator_Manager.h"

// Forward decl.
class CUTS_BE_File_Generator;

// Forward decl.
class CUTS_Dependency_Graph;

// Forward decl.
class CUTS_Dependency_Node;

// Forward decl.
class CUTS_BE_Preprocess_Data;

//=============================================================================
/**
 * @class CUTS_BE_Project_Generator
 *
 * Base class for project file generators. This class provides two
 * methods for generating projects. The first method is using the
 * visitor pattern for PICML. The second method is by explicity
 * invoking the write_project () method. The former method allows
 * control to be passed to the CUTS_BE_File_Generator_Manager for the
 * project generator to generate the necessary source files. The
 * latter method will only generate the project file, and none of
 * the source files.
 */
//=============================================================================

class CUTS_BE_Export CUTS_BE_Project_Generator :
  public PICML::Visitor
{
public:
  /// Destructor.
  virtual ~CUTS_BE_Project_Generator (void);

  /**
   * Initialize the project generator.
   *
   * @param[in]     outdir      Output directory for the project.
   * @param[in]     graph       Dependence graph for all projects.
   */
  void init (const std::string & outdir,
             CUTS_Dependency_Graph * graph = 0);

  /**
   * Signal to begin writing the project.
   *
   * @param[in]       container       Starting container.
   * @param[in]       ppd             Preprocess data for the container.
   */
  virtual void write_project (
    const PICML::ComponentImplementationContainer & container,
    const CUTS_BE_Preprocess_Data & ppd);

  /**
   * Write a stub project.
   *
   * @param[in]       node            Pointer to the source node.
   * @retval          true            Succeed to write file.
   * @retval          false           Failure to write file.
   */
  virtual bool write_stub_project (CUTS_Dependency_Node * node);

protected:
  /// Default constructor.
  CUTS_BE_Project_Generator (void);

  // @@ this needs to be in the private section w/ public acessor methods
  // @@ since we do not want this data to change.

  /// Output directory.
  std::string outdir_;

  /// Dependency graph for the entire project.
  CUTS_Dependency_Graph * graph_;

  /// Collection of file generators.
  CUTS_BE_File_Generator_Manager manager_;

private:
  /// Visit a ComponentImplementationContainer object.
  void Visit_ComponentImplementationContainer (
    const PICML::ComponentImplementationContainer &);

  /// Visit a MonolithicImplemenation object
  void Visit_MonolithicImplementation (
    const PICML::MonolithicImplementation &);

  /// Visit a Implements object.
  void Visit_Implements (const PICML::Implements &);

  /// Visit a Component object.
  void Visit_Component (const PICML::Component &);

  // prevent the following operations
  CUTS_BE_Project_Generator (const CUTS_BE_Project_Generator &);
  const CUTS_BE_Project_Generator & operator = (const CUTS_BE_Project_Generator &);
};

#if defined (__CUTS_INLINE__)
#include "Project_Generator.inl"
#endif

#endif  // !defined _CUTS_PROJECT_GENERATOR_H_
