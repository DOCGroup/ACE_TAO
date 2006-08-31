// -*- C++ -*-

//=============================================================================
/**
 *
 */
//=============================================================================

#ifndef _CUTS_BE_CIAO_PROJECT_GENERATOR_H_
#define _CUTS_BE_CIAO_PROJECT_GENERATOR_H_

#include "UDM_CIAO_Export.h"
#include "cuts/be/Project_Generator.h"
#include <iosfwd>

// Forward decl.
class CUTS_Dependency_Node;

class CUTS_UDM_CIAO_Export CUTS_CIAO_Project_Generator :
  public CUTS_BE_Project_Generator
{
public:
  /// Default constructor.
  CUTS_CIAO_Project_Generator (void);

  /// Destructor.
  virtual ~CUTS_CIAO_Project_Generator (void);

  /**
   * Signal to begin writing the project.
   *
   * @param[in]       container       Starting container.
   */
  void write_project (
    const PICML::ComponentImplementationContainer & container,
    const CUTS_BE_Preprocess_Data & ppd);

  /**
   * Write a stub project.
   *
   * @param[in]       node        Node for the stub project.
   * @retval          true        Successfully wrote file.
   * @retval          false       Failed to write file.
   */
  bool write_stub_project (CUTS_Dependency_Node * node);

private:
  void write_exec_project (
    std::ofstream & out,
    const PICML::ComponentImplementationContainer &,
    CUTS_Dependency_Node * node,
    const CUTS_BE_Preprocess_Data & ppd);

  void write_svnt_project (
    std::ofstream & out,
    const PICML::ComponentImplementationContainer &,
    CUTS_Dependency_Node * node);

  /**
   * Generates a listing of stubs.
   *
   * @param[in]     project     Target project stream.
   * @param[in]     node        Starting node.
   */
  void generate_stub_listing (std::ofstream & project,
                              CUTS_Dependency_Node * node);

  void generate_mpc_i (std::ofstream & project,
                       const CUTS_BE_Preprocess_Data & ppd);

  void generate_mpc_values (std::ofstream & project,
                            const std::string & heading,
                            const CUTS_String_Set & listing);
};

#endif  // !defined _CUTS_BE_CIAO_PROJECT_GENERATOR_H_
