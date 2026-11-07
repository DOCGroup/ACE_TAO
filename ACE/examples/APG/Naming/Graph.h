#ifndef GRAPH_H
#define GRAPH_H

#include "Graphable_Element.h"

class Graph
{
public:
  Graph () = default;

  void graph (char const *filename, Graphable_Element_List &data);
};

#endif /* GRAPH_H */
