#include "Dependency_Sorter.h"

#include <algorithm>
#include <vector>

namespace DAnCE
{
  /// Add a handler which has no dependencies
  void
  Dependency_Sorter::add_nondependent (const std::string &subject)
  {
    this->dep_map_[subject] = IH_DEPS ();
  }
  
  /// Add a dependency to the map.
  void
  Dependency_Sorter::add_dependency (const std::string &subject,
                                     const IH_DEPS &deps)
  {
    this->dep_map_[subject] = deps;
  }
  

  void
  Dependency_Sorter::add_dependency (const std::string &subject,
                                     const std::string &install_after)
  {
    DEP_MAP::iterator after = this->dep_map_.find (install_after);
    
    if (after == this->dep_map_.end ())
      {
        IH_DEPS tmp;
        tmp.insert (install_after);
        this->dep_map_[subject] = tmp;
      }
    else
      {
        after->second.insert (install_after);
      }
  }
  
  void
  Dependency_Sorter::calculate_order (Dependency_Sorter::INSTALL_ORDER &retval)
  {
    // Deps which have been added to the install order
    IH_DEPS selected;
    
    DEP_MAP tmp_dep = this->dep_map_;
    
    while (retval.size () != this->dep_map_.size ())
      {
        bool updated (false);

        for (DEP_MAP::iterator i = tmp_dep.begin (); 
             i != tmp_dep.end ();
             ++i)
          {
            if (i->second.size () == 0) // i.e., no dependencies
              {
                retval.push_back (i->first);
                selected.insert (i->first);
                updated = true;
                tmp_dep.erase (i);
                break;
              }
            
            if (selected.size () >= i->second.size ())
              {
                std::vector< std::string > tmp (i->second.size ());

                std::set_intersection (i->second.begin (), i->second.end (),
                                       selected.begin (), selected.end (),
                                       tmp.begin ());
                
                if (tmp.size () == i->second.size ()) // i.e., all deps satisfied
                  {
                    retval.push_back (i->first);
                    selected.insert (i->first);
                    updated = true;
                    tmp_dep.erase (i);
                    break;
                  }
              }
          }
        
        if (!updated) // i.e., we weren't able to add a new dep
          {
            throw Invalid_Install_Order ();
          }
      }
  }
}
