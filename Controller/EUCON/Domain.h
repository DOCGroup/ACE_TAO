#ifndef DOMAIN_H
#define DOMAIN_H

#include <string>
#include <vector>

namespace CIAO
{
  namespace RACE
  {
    /// Generic structure that holds information about a resource on a
    /// node. Resources can be memory, CPU, n/w bandwidth, etc.
    struct Resource
    {
      /// Unique identifier of the resource.
      std::string UUID;

      /// Name of the resource - memory, CPU, n/w bandwidth.
      std::string label;

      /// Desired utilization set-point (max 1).
      double set_point;

      /// Current utilization (between 0 & 1).
      double util;
    };

    /// Structure of a logical node.
    struct Node
    {
      /// Unique identifier of the node.
      std::string UUID;

      /// Logical node name.
      std::string label;

      /// Various resources in this node.
      std::vector <Resource> resources;
    };

    struct Domain
    {
      std::vector <Node> nodes;
    };


  }
}

#endif /* DOMAIN_H */
