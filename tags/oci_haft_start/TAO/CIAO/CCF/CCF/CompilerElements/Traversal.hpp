#ifndef TRAVERSAL_HPP
#define TRAVERSAL_HPP

#include "MSVC_Pragmas.hpp"

namespace CCF
{
  namespace Traversal
  {
    struct Visitor
    {
      virtual
      ~Visitor () /*@@= 0*/ {}
    };


    template <typename VisitorType>
    bool
    try_visit (
      void (VisitorType::*visit_func)(typename VisitorType::NodePtr const&),
      Visitor* v,
      typename VisitorType::NodePtr const& n)
    {
      VisitorType* vt = dynamic_cast<VisitorType*> (v);
      if(vt != 0)
      {
        (vt->*visit_func)(n);
        return true;
      }
      else
      {
        return false;
      }
    }
  }
}

#endif // TRAVERSAL_HPP
