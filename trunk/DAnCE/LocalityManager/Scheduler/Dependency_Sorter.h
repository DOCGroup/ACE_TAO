// $Id$
/**
 * @file Dependency_Sorter.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 */
#ifndef DEP_SORTER_H
#define DEP_SORTER_H

#include <map>
#include <set>
#include <string>
#include <vector>

namespace DAnCE
{
  class Dependency_Sorter
  {
  public:
    typedef std::vector < std::string > INSTALL_ORDER;
    typedef std::set < std::string > IH_DEPS;
    class Invalid_Install_Order : std::exception {};

    /// Add a handler which has no dependencies
    void add_nondependent (const std::string &subject);

    /// Add a dependency to the map.
    void add_dependency (const std::string &subject,
                         const IH_DEPS &deps);

    /// Add a dependency to the map.
    void add_dependency (const std::string &subject,
                         const std::string &install_after);
    
    void calculate_order (INSTALL_ORDER &);

  private:
    typedef std::map < std::string, IH_DEPS > DEP_MAP;
    
    DEP_MAP dep_map_;
  };
}

#endif
