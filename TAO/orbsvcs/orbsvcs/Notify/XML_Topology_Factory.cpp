#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/XML_Topology_Factory.h"
#include "orbsvcs/Notify/XML_Saver.h"
#include "orbsvcs/Notify/XML_Loader.h"

#include "tao/debug.h"
#include "ace/OS_NS_strings.h"

#include <memory>


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  const char TOPOLOGY_ID_NAME[] = "TopologyID";

  XML_Topology_Factory::XML_Topology_Factory () = default;

  // virtual
  XML_Topology_Factory::~XML_Topology_Factory () = default;

  // virtual
  Topology_Saver*
  XML_Topology_Factory::create_saver ()
  {
    XML_Saver *saver = {};

    ACE_NEW_RETURN (saver, XML_Saver (this->timestamp_), nullptr);
    std::unique_ptr<XML_Saver> saver_guard (saver);

    if (! saver_guard->open (this->save_base_path_.c_str (), this->backup_count_))
    {
      return nullptr;
    }
    return saver_guard.release ();
  }

  // virtual
  Topology_Loader*
  XML_Topology_Factory::create_loader()
  {
    XML_Loader *loader = {};
    ACE_NEW_RETURN (loader, XML_Loader, nullptr);
    std::unique_ptr<XML_Loader> loader_guard (loader);

    if (! loader_guard->open (this->load_base_path_))
    {
      return nullptr;
    }
    return loader_guard.release ();
  }

  // virtual
  int
  XML_Topology_Factory::init (int argc, ACE_TCHAR *argv[])
  {
    int result = 0;
    bool verbose = false;
    for (int narg = 0; narg < argc; ++narg)
    {
      ACE_TCHAR * av = argv[narg];
      if (ACE_OS::strcasecmp (av, ACE_TEXT("-v")) == 0)
      {
        verbose = true;
        ORBSVCS_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Standard_Event_Persistence: -verbose\n")
          ));
      }
      else if (ACE_OS::strcasecmp (av, ACE_TEXT("-base_path")) == 0 && narg + 1 < argc)
      {
        this->save_base_path_ = argv[narg + 1];
        this->load_base_path_ = argv[narg + 1];
        if (TAO_debug_level > 0 || verbose)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
            ACE_TEXT ("(%P|%t) XML_TopologyFactory: Setting -base_path: %s\n"),
            this->save_base_path_.c_str ()
          ));
        }
        narg += 1;
      }
      else if (ACE_OS::strcasecmp (av, ACE_TEXT("-save_base_path")) == 0 && narg + 1 < argc)
      {
        this->save_base_path_ = argv[narg + 1];
        if (TAO_debug_level > 0 || verbose)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
            ACE_TEXT ("(%P|%t) XML_TopologyFactory: Setting -save_base_path: %s\n"),
            this->save_base_path_.c_str ()
          ));
        }
        narg += 1;
      }
      else if (ACE_OS::strcasecmp (av, ACE_TEXT("-load_base_path")) == 0 && narg + 1 < argc)
      {
        this->load_base_path_ = argv[narg + 1];
        if (TAO_debug_level > 0 || verbose)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
            ACE_TEXT ("(%P|%t) XML_TopologyFactory: Setting -load_base_path: %s\n"),
            this->load_base_path_.c_str ()
          ));
        }
        narg += 1;
      }
      else if (ACE_OS::strcasecmp (av, ACE_TEXT("-backup_count")) == 0 && narg + 1 < argc)
      {
        this->backup_count_ = ACE_OS::atoi(argv[narg + 1]);
        if (TAO_debug_level > 0 || verbose)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
            ACE_TEXT ("(%P|%t) XML_TopologyFactory: Setting -file_count: %B\n"),
            this->backup_count_
          ));
        }
        narg += 1;
      }
      else if (ACE_OS::strcasecmp (av, ACE_TEXT("-no_timestamp")) == 0)
      {
        this->timestamp_ = false;
        if (TAO_debug_level > 0 || verbose)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
            ACE_TEXT ("(%P|%t) XML_TopologyFactory: Setting -no_timestamp\n")
          ));
        }
      }
      else
      {
        ORBSVCS_ERROR ((LM_ERROR,
          ACE_TEXT ("(%P|%t) Unknown parameter to XML Topology Factory: %s\n"),
          argv[narg]
          ));
        result = -1;
      }
    }
    return result;
  }

  // virtual
  int
  XML_Topology_Factory::fini ()
  {
    // nothing to do yet
    return 0;
  }
} /* namespace TAO_Notify */

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_NAMESPACE_DEFINE (TAO_Notify_Persist,
                              TAO_Notify_XML_Topology_Factory,
                              TAO_Notify::XML_Topology_Factory)
