// -*- C++ -*-

//=============================================================================
/**
 * @class     Dependency_Graph.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_BE_DEPENDENCY_GRAPH_H_
#define _CUTS_BE_DEPENDENCY_GRAPH_H_

#include "BE_export.h"
#include "String_Set.h"
#include <map>

// Forward decl.
class CUTS_Dependency_Node;

/// Type definition for a collection of unique nodes.
typedef std::set <CUTS_Dependency_Node *> CUTS_Reference_Set;

//=============================================================================
/**
 * @class CUTS_Dependency_Node
 *
 * Dependency information about a particular IDL file.
 */
//=============================================================================

class CUTS_BE_Export CUTS_Dependency_Node
{
public:
  /// Flags for the dependency node.
  enum Node_Flags
  {
    /// None of the flags are set.
    DNF_NONE        = 0x0000,

    /// The stub project is necessary (value 1).
    DNF_STUB        = 0x0001,

    /// The node has been visited.
    DNF_VISITED     = 0x0002
  };

  /**
   * Constructor.
   *
   * @param[in]     name        Name of the node.
   */
  CUTS_Dependency_Node (const std::string & name);

  /// Default destructor.
  ~CUTS_Dependency_Node (void);

  /**
   * Get the name of the node.
   *
   * @return      Name of the node.
   */
  const std::string & name (void) const;

  /**
   * Get the basename of the node.
   *
   * @return      Basename of the node.
   */
  const char * basename (void) const;

  /// Node flags.
  long flags_;

  /// Reference set for the node.
  CUTS_Reference_Set references_;

  /// Collection of include paths.
  CUTS_String_Set includes_;

  /// Collection of library paths.
  CUTS_String_Set libpaths_;

  /// Collection of import libraries.
  CUTS_String_Set libs_;

  /// Reset the node.
  void reset (void);

private:
  // Name of the node.
  std::string name_;

  /// Pointer to the basename of the <name_>.
  size_t basename_;
};

//=============================================================================
/**
 * @class CUTS_Dependency_Graph
 *
 * Collection of interconnected CUTS_Dependency_Node elements. The
 * interconnection between the all nodes represents the dependency of
 * a node on another node.
 */
//=============================================================================

class CUTS_BE_Export CUTS_Dependency_Graph
{
public:
  /// Type definition for the dependency graph.
  typedef std::map <std::string,
                    CUTS_Dependency_Node *> Dependency_Graph;

  /// Constructor.
  CUTS_Dependency_Graph (void);

  /// Destructor.
  ~CUTS_Dependency_Graph (void);

  /**
   * Create a new node in the dependency graph. If the node already
   * exists then it returns a pointer to the existing node.
   *
   * @param[in]       name        Name of the node.
   * @return          Pointer to the node.
   */
  CUTS_Dependency_Node * create_node (const std::string & name);

  /**
   * Find a node in the graph. If the node is not found then
   * one is created.
   *
   * @param[in]     name          Name of the node to find.
   * @param[out]    node          Pointer to the node if found.
   * @retval        true          The node was found/created.
   * @retval        false         The node was not found.
   */
  bool find_node (const std::string & name,
                  CUTS_Dependency_Node * &node) const;

  /**
   * Remove a node from the graph.
   *
   * @param[in]     name      Name of the node to remove.
   */
  void remove_node (const std::string & name);

  /**
   * Get the contained dependency graph.
   *
   * @return      Read-only reference to internal graph.
   */
  const Dependency_Graph & graph (void) const;

  /// Reset the visit flag for all the nodes.
  void reset_visit_flag (void);

private:
  /// The dependency graph.
  Dependency_Graph graph_;
};

#endif  // !defined _CUTS_BE_DEPENDENCY_GRAPH_H_
