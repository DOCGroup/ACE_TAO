// $Id$
#include "Dump_Obj.h"

ACE_RCSID (Config_Handlers,
           Dump_Obj,
           "$Id$")

  namespace Deployment
  {
    // static member initialization
    std::string Dump_Obj::indent_ = "-";
    std::map<std::string, CORBA::Any> Dump_Obj::desc_map_;

    Dump_Obj::Dump_Obj(const char* caption)
      : desc_()
    {
      ACE_DEBUG ((LM_DEBUG, "%s%s:\n", indent_.c_str(), caption));
      indent_.append("  ");
    }


    Dump_Obj::Dump_Obj(const char* caption,
                       CORBA::Any &val)
      : desc_(caption)
    {
      ACE_DEBUG ((LM_DEBUG, "%s%s:\n",
                  indent_.c_str(),
                  caption));

      indent_.append("   ");

      if (desc_map_.find (std::string (caption)) != desc_map_.end())
        {
          ACE_DEBUG ((LM_DEBUG, "DnC_Dump.cpp:Dump_Obj - \
The item with name %s is already in \
the node map.\n", caption));
          throw Node_Exist();
        }
      else
        {
          desc_map_[std::string(caption)] = val;
        }
    }

    Dump_Obj::~Dump_Obj()
    {
      indent_.erase(indent_.size() - 2, 2);
      if (desc_.size() != 0)
        {
          if (desc_map_.find(desc_) != desc_map_.end())
            {
              desc_map_.erase(desc_map_.find(desc_));
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG, "DnC_Dump.cpp:Dump_Obj - \
The item with name %s is not in the node map.\n",
                          desc_.c_str()));
              throw Node_Not_Exist();
            }
        }
    }

  }
