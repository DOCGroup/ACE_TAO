// $Id$

#include "Containers_Info_Map.h"
#include "ciao/CIAO_Config.h"
#include "ciao/CIAO_common.h"
#include "DAnCE/Deployment/CIAO_ServerResourcesC.h"

//Added for HTTP
#include "URL_Parser.h"              //for parsing the URL
#include "tao/HTTP_Client.h"         //the HTTP client class to downloading packages
#include "ace/Message_Block.h"       //for ACE_Message_Block
#include "ace/OS_NS_fcntl.h"         //for open
#include "ace/OS_NS_unistd.h"        //for close
#include "ace/OS_NS_sys_stat.h"      //for filesize and mkdir
#include "ace/OS_NS_string.h"        //for string functions
#include "ace/streams.h"

namespace CIAO
{
  Containers_Info_Map::
  Containers_Info_Map (const Deployment::DeploymentPlan & plan,
          const Deployment::ComponentPlans & shared_components)
    : map_ (CIAO_DEFAULT_MAP_SIZE),
      plan_ (plan),
      shared_components_ (shared_components),
      HTTP_DOWNLOAD_PATH ()
  {
    char* temp = ACE_OS::getenv ("CIAO_ROOT");
    HTTP_DOWNLOAD_PATH += temp;

#if defined (ACE_WIN32)
    HTTP_DOWNLOAD_PATH += "\\";
#else
    HTTP_DOWNLOAD_PATH += "/";
#endif

    HTTP_DOWNLOAD_PATH += "HTTP_DOWNLOADED_LIBS";

    ACE_OS::mkdir(HTTP_DOWNLOAD_PATH.c_str ());
    //if dir already exists a -1 is returned
    //we ignore this, just need to make sure the directory exists

    this->initialize_map ();
    this->build_map ();
  }

  Deployment::ContainerImplementationInfos *
  Containers_Info_Map::containers_info (void)
  {
    // Fetch the information from the <containers_info_map_>
    // and return the corresponding sequence
    Deployment::ContainerImplementationInfos_var retv;

    ACE_NEW_RETURN (retv,
                    Deployment::ContainerImplementationInfos,
                    0);

    Iterator end = this->map_.end ();
    CORBA::ULong i = 0;

    for (Iterator b = this->map_.begin ();
         b != end;
         ++b)
      {
        retv->length (i + 1);
        (*retv)[i] = *((*b).int_id_);

        ++i;
      }
    return retv._retn ();
  }

  void
  Containers_Info_Map::
  initialize_map (void)
  {
    const CORBA::ULong instance_len = this->plan_.instance.length ();

    // Iterate over the instance list and look at the policy set id of each
    // component instance. For each policy set, we create a separate container
    // to host all the components with such policy set.
    // NOTE: all the component instances without policies are specified should
    // be hosted in the same container, and in our map the key is an empty
    // string ""
    int first_time = 0;
    for (CORBA::ULong i = 0; i < instance_len; ++i)
      {
        CORBA::String_var my_resource_id ("");
        const char *diff_policy_set_id = "";
        bool found = false;

        // @@ super hack here.  we are assuming RT policy set is
        // the only thing that will be specified.
        CORBA::ULong dp_length =
          this->plan_.instance[i].deployedResource.length ();

        if (dp_length == 0)
          {
            found = true;
          }

        for (CORBA::ULong x = 0; x < dp_length; ++x)
          {
            my_resource_id =
              this->plan_.instance[i].deployedResource[x].resourceName.in ();

            CORBA::ULong property_length =
              this->plan_.instance[i].deployedResource[x].property.length ();

            found = true;

            for (CORBA::ULong p = 0; p < property_length; ++p)
              {
                if (ACE_OS::strcmp (
                      this->plan_.instance[i].deployedResource[x].
                      property[p].name.in (),
                      "CIAO:InstancePolicy") == 0)
                  { // should be only one of them.
                    // each instance will have one one CIAO:InstancePolicy
                    // definition.
                    this->plan_.instance[i].deployedResource[x].property[p].
                    value >>= diff_policy_set_id;
                    if (this->map_.find (diff_policy_set_id) != 0)
                      {
                        // we found a policy that is different from what
                        // we have seen before.
                        found = true;
                      }
                    else
                      {
                        found = false;
                      }
                  }
              }
          }

        // If we find a existing policy_set_id, then do nothing.
        if (found == false)
          {
            // we did not find a new instance policy.
            // but the instance's receptacles could have their
            // own policies.
            continue;
          }

        if (ACE_OS::strcmp (diff_policy_set_id, "") == 0)
          {
            // no policy set id has been specified
            // but we could have some receptacle policies around.
            //
            if (first_time == 0)
              {
                Deployment::ContainerImplementationInfo * info;
                ACE_NEW (info, Deployment::ContainerImplementationInfo);
                this->map_.bind (diff_policy_set_id, info);
                first_time = 1;
                continue;
              }
          }
        else
          {
            Deployment::ContainerImplementationInfo * info;
            ACE_NEW (info, Deployment::ContainerImplementationInfo);

            // Fetch the actual policy_set_def from the infoProperty
            // Ugly due to the IDL data structure definition! :(
            CORBA::ULong j;
            CORBA::ULong infoProperty_length =
                this->plan_.infoProperty.length ();

            for (j = 0; j < infoProperty_length; ++j)
              {
                if (ACE_OS::strcmp (this->plan_.infoProperty[j].name.in (),
                                    "CIAOServerResources") != 0)
                  continue;

                CIAO::DAnCE::ServerResource *server_resource_def = 0;
                this->plan_.infoProperty[j].value >>= server_resource_def;

                if (ACE_OS::strcmp ((*server_resource_def).Id,
                                    my_resource_id.in ()) == 0)
                  {
                    // Iterate over the policy_sets
                    CORBA::ULong k;
                    CORBA::ULong policy_sets_length =
                      (*server_resource_def).orb_config.policy_set.length ();
                    const char *comparing_policy_set_id;
                    CORBA::ULong config_length;
                    CORBA::ULong new_config_length;
                    for (k = 0; k < policy_sets_length; ++k)
                      {
                        CORBA::ULong new_dp_length =
                          this->plan_.instance[i].deployedResource.length ();

                        for (CORBA::ULong z = 0; z < new_dp_length;
                             ++z)
                          {
                            CORBA::ULong new_property_length =
                              this->plan_.instance[i].deployedResource[z].
                                property.length ();
                            for (CORBA::ULong s = 0;
                                 s < new_property_length; ++s)
                              {
                                this->plan_.instance[i].deployedResource[z].
                                property[s].value >>= comparing_policy_set_id;
                                if (ACE_OS::strcmp (comparing_policy_set_id,
                                      (*server_resource_def).
                                      orb_config.policy_set[k].Id) == 0)
                                  {
                                    config_length =
                                      info->container_config.length ();
                                    new_config_length =
                                      config_length + 1;
                                    info->container_config.length (
                                      new_config_length);
                                    if (ACE_OS::strcmp (
                                         this->plan_.instance[i].
                                         deployedResource[z].
                                         property[s].name.in (),
                                         "CIAO:InstancePolicy") == 0)
                                      {
                                        info->
                                         container_config[config_length].name =
                                          CORBA::string_dup
                                            ("ContainerPolicySet");
                                      }
                                    else if (ACE_OS::strcmp (
                                              this->plan_.instance[i].
                                              deployedResource[z].
                                              property[s].name.in (),
                                              "CIAO:ReceptaclePolicy") == 0)
                                      {
                                        info->
                                         container_config[config_length].name =
                                          CORBA::string_dup
                                             ("ReceptaclePolicySet");
                                      }
                                    else
                                      {
                                        info->
                                          container_config[config_length].name =
                                            this->plan_.instance[i].
                                               deployedResource[z].
                                               property[s].name.in ();
                                      }
                                    info->
                                      container_config[config_length].value <<=
                                        comparing_policy_set_id;
                                  }
                              }
                          }
                      }
                    this->map_.bind (diff_policy_set_id, info);
                  }
              }
          }
      }
  }

  bool
  Containers_Info_Map::
  build_map (void)
  {
    const CORBA::ULong instance_len = this->plan_.instance.length ();

    for (CORBA::ULong i = 0; i < instance_len; ++i)
      {
        const Deployment::InstanceDeploymentDescription & instance =
          this->plan_.instance[i];

        // If this component instance happens to be in the "shared components
        // list", then we ignore it, otherwise we shall install it.
        ACE_CString name (instance.name.in ());
        if (this->is_shared_component (name))
          continue;

        if (! this->insert_instance_into_map (instance))
          return false;
      }

    return true;
  }

  bool
  Containers_Info_Map::insert_instance_into_map (
    const Deployment::InstanceDeploymentDescription & instance)
  {
    Deployment::ContainerImplementationInfo container_info;

    const char * policy_set_id = "";

    CORBA::ULong dp_length =
      instance.deployedResource.length ();

    for (CORBA::ULong x = 0; x < dp_length; ++x)
      {
        CORBA::ULong property_length =
          instance.deployedResource[x].property.length ();

        for (CORBA::ULong p = 0; p < property_length; ++p)
          {
            if (ACE_OS::strcmp (
                  instance.deployedResource[x].
                  property[p].name.in (),
                  "CIAO:InstancePolicy") == 0)
              {
                instance.deployedResource[x].property[p].value >>=
                  policy_set_id;
              }
          }
      }

    // Find the ContainerImplementationInfo entry from the map
    MAP::ENTRY *entry = 0;
    if (this->map_.find (policy_set_id, entry) != 0)
      {
        return false; //should never happen
      }
    else
      {
        this->insert_instance_into_container (
                instance,
                entry->int_id_->impl_infos);
      }

    return true;
  }

  bool
  Containers_Info_Map::insert_instance_into_container (
      const Deployment::InstanceDeploymentDescription & instance,
      Deployment::ComponentImplementationInfos & impl_infos)
  {
    // Increase the length of the ComponentImplementationInfos by one
    CORBA::ULong i = impl_infos.length ();
    impl_infos.length (i + 1);

    // Fill in the information about this component instance
    // Get the component instance name.
    impl_infos[i].component_instance_name = instance.name.in ();

    const Deployment::MonolithicDeploymentDescription & impl =
      this->plan_.implementation[instance.implementationRef];

    const CORBA::ULong artifact_num = impl.artifactRef.length ();

    // Copy Component instance related Properties if there is any.
    if (instance.configProperty.length () > 0)
      {
        impl_infos[i].component_config = instance.configProperty;
      }

    CORBA::ULong config_length =
      impl_infos[i].component_config.length ();

    CORBA::ULong dp_length =
      instance.deployedResource.length ();

    const char* policy_set_id = "";

    for (CORBA::ULong x = 0; x < dp_length; ++x)
      {
        CORBA::ULong property_length =
          instance.deployedResource[x].property.length ();

        for (CORBA::ULong p = 0; p < property_length; ++p)
          {
            if (ACE_OS::strcmp (
                  instance.deployedResource[x].
                  property[p].name.in (),
                  "CIAO:InstancePolicy") != 0)
              {
                if (ACE_OS::strcmp (
                      instance.deployedResource[x].
                      property[p].name.in (),
                      "CIAO:ReceptaclePolicy") != 0)
                  {
                    config_length =
                      impl_infos[i].component_config.length ();
                    impl_infos[i].component_config.length (config_length + 1);
                    impl_infos[i].component_config[config_length].name =
                      instance.deployedResource[x].property[p].name.in ();
                    instance.deployedResource[x].property[p].value >>=
                      policy_set_id;
                    impl_infos[i].component_config[config_length].value <<=
                      policy_set_id;
                  }
              }
          }
      }

    bool svnt_found = false;
    bool exec_found = false;

    // For svnt/exec artifacts
    for (CORBA::ULong j = 0; j < artifact_num; ++j)
      {
        Deployment::ArtifactDeploymentDescription arti =
          this->plan_.artifact[ impl.artifactRef[j] ];

        for (size_t loc_num = 0;
             loc_num < arti.location.length ();
             ++loc_num)
          {
            if (ACE_OS::strstr (arti.location[loc_num], "http://"))
              {
                ACE_CString path;
                ACE_CString arti_name;
                if (!this->resolve_http_reference (arti.location[loc_num],
                                                   arti_name,
                                                   path))
                  {
                    ACE_ERROR
                         ((LM_ERROR,
                         "CIAO (%P|%t) Containers_Info_Map.cpp -"
                         "Containers_Info_Map::insert_instance_into_container -"
                         "ERROR: Unable to resolve HTTP ref to location[%d] of %s\n",
                         loc_num, arti.name.in ()));

                    //No need to throw an exception here!
                    //Actually it is not desirable.
                    //This is handled in a different part of DAnCE
                    arti.location[loc_num] = "HTTP_failure";
                  }
                else
                  {
                    arti.location[loc_num] = arti_name.c_str ();
                    //enque for cleanup
                  }
              }
          }

        ACE_CString tmp = arti.name.in ();
        ACE_CString::size_type pos;

        //@@ Note: I am not checking for redundancy here. Maybe
        //         the modeling tool should make sure of
        //         uniqueness, i.e., one component implementation
        //         should have only 1 _svnt and 1 _exec libs.
        if ((pos  = tmp.find ("_stub")) != ACE_CString::npos ||
            (pos  = tmp.find ("_Stub")) != ACE_CString::npos)
          continue; // We ignore _stub artifact since it's not used.

        if (!svnt_found &&
                ((pos  = tmp.find ("_svnt")) != ACE_CString::npos ||
                 (pos  = tmp.find ("_Svnt")) != ACE_CString::npos))
          {
            if (arti.location.length() < 1 )
              {
                ACE_DEBUG ((LM_DEBUG, "Servant Artifact must have a location!\n"));
                return  0;
              }

            svnt_found = true;
            // Copy the servant dll/so name.
            // @@ Note: I ignore all the other locations except the first one.
            impl_infos[i].servant_dll =
              CORBA::string_dup (arti.location[0]);

            // Get the entry point.
            const CORBA::ULong prop_length = arti.execParameter.length ();

            for (CORBA::ULong prop_num = 0;
                 prop_num < prop_length;
                 ++prop_num)
              {
                ACE_CString name (arti.execParameter[prop_num].name.in ());
                if (name == ACE_CString ("entryPoint"))
                  {
                    const char * entry = 0;
                    (arti.execParameter[prop_num].value) >>= entry;
                    impl_infos[i].servant_entrypt = CORBA::string_dup (entry);
                  }
                else
                  {
                    ACE_DEBUG ((LM_DEBUG, "Found unknown property in the artifact!\n"));
                    ACE_DEBUG ((LM_DEBUG, "We only support entrypoint at this point in CIAO.\n"));
                  }
              }

            continue; // continue for the next artifact
          }

        // As one can see, code is duplicated here. I will come back for this later.
        // For exec artifact
        if (!exec_found &&
                ((pos  = tmp.find ("_exec")) != ACE_CString::npos ||
                 (pos  = tmp.find ("_Exec")) != ACE_CString::npos))
          {
            if (arti.location.length() < 1 )
              {
                ACE_DEBUG ((LM_DEBUG, "Executor Artifact must have a location!\n"));
                return 0;
              }

            exec_found = true;
            // Copy the servant dll/so name.
            // @@ Note: I ignore all the other locations except the first one.
            exec_found = true;
            impl_infos[i].executor_dll =
              CORBA::string_dup (arti.location[0]);

            // Get the entry point.
            const CORBA::ULong prop_length = arti.execParameter.length ();
            for (CORBA::ULong prop_num = 0;
                 prop_num < prop_length;
                 ++prop_num)
              {
                ACE_CString name (arti.execParameter[prop_num].name.in ());
                if (name == ACE_CString ("entryPoint"))
                  {
                    const char * entry = 0;
                    (arti.execParameter[prop_num].value) >>= entry;
                    impl_infos[i].executor_entrypt = CORBA::string_dup (entry);
                  }
                else
                  {
                    ACE_DEBUG ((LM_DEBUG, "Found unknown property in the artifact!\n"));
                    ACE_DEBUG ((LM_DEBUG, "We only support entrypoint at this point in CIAO.\n"));
                  }
              }
          }
        else
          // We see artifact other than servant/executor and we ignore them.
          continue;
      }
    return true;
  }
}

bool
CIAO::Containers_Info_Map::
is_shared_component (ACE_CString & name)
{
  for (CORBA::ULong i = 0; i < this->shared_components_.length (); ++i)
    {
      if (ACE_OS::strcmp (this->shared_components_[i].name.in (),
                          name.c_str ()) == 0)
        return true;
    }

  return false;
}


/*---------------------------------------------------------------------
 * functions to support HTTP capabilities of the NodeApplicationManager
 * @author Stoyan Paunov
 *
 * Purpose: Adding the HTTP access code which will resove
 * any references to HTTP URLs
 */

  // This function checks if the HTTP_DOWNLOAD_PATH is
  // in the library load path
  void
  CIAO::Containers_Info_Map::update_loader_path (void)
  {
#if defined (ACE_WIN32)
    char* path = ACE_OS::getenv ("PATH");
#else
    char* path = ACE_OS::getenv ("LD_LIBRARY_PATH");
#endif

    if (ACE_OS::strstr (path, this->HTTP_DOWNLOAD_PATH.c_str ()))
       return;

#if defined (ACE_WIN32)
    ACE_CString new_path = "PATH=";
#else
    ACE_CString new_path = "LD_LIBRARY_PATH=";
#endif

    new_path += this->HTTP_DOWNLOAD_PATH;

#if defined (ACE_WIN32)
    new_path += ";";
#else
    new_path += ":";
#endif

    new_path += path;

   ACE_OS::putenv (new_path.c_str ());

  }

  //This function resolves any http location references
  //in the name of the implementation artifacts
  //It returns true on success and false on failure

  bool
  CIAO::Containers_Info_Map::resolve_http_reference (const char* location,
                                                     ACE_CString &arti_name,
                                                     ACE_CString &path)
  {
    // Figure out the file name.
    char* name = const_cast<char*> (location);
    char* p = 0;

    while (true)
      {
        p = ACE_OS::strstr (name, "/");

        if (0 == p)
          {
            p = ACE_OS::strstr (name, "\\");
          }

        if (0 == p)
          {
            break;
          }
        else
          {
            name = ++p;
            continue;
          }
      }

    arti_name = name;

    // Get the file.
    ACE_Message_Block* mb = 0;
    ACE_NEW_RETURN (mb, ACE_Message_Block (0,0), false);

    if (!this->retrieve_via_HTTP (location, *mb))
    {
      mb->release ();
      return false;
    }

#if defined (ACE_WIN32) && defined (ACE_LD_DECORATOR_STR) && !defined (ACE_DISABLE_DEBUG_DLL_CHECK)
    ACE_TString decorator (ACE_LD_DECORATOR_STR);
#endif
    ACE_TString prefix (ACE_DLL_PREFIX);
    ACE_TString suffix (ACE_DLL_SUFFIX);

    ACE_CString new_name (name);
    new_name = prefix + new_name;
#if defined (ACE_WIN32) && defined (ACE_LD_DECORATOR_STR) && !defined (ACE_DISABLE_DEBUG_DLL_CHECK)
    new_name += decorator;
#endif
    new_name += suffix;

    path = HTTP_DOWNLOAD_PATH;
    path += "/";
    path += new_name;

    if (!this->write_to_disk (path.c_str (), *mb))
    {
      mb->release ();
      return false;
    }

    mb->release ();
    return true;
  }

  //function to retvieve a file via HTTP
  //stores the file in the passed preallocated ACE_Message_Block
  //returns 1 on success
  //        0 on error

  bool
  CIAO::Containers_Info_Map::retrieve_via_HTTP (const char* URL,
                                                ACE_Message_Block &mb)
  {
    ACE_CString loc (URL);

    // Figure out the file name.
    char* name_ = const_cast<char*> (URL);
    char* p = 0;

    while (true)
      {
        p = ACE_OS::strstr (name_, "/");

        if (0 == p)
          {
            p = ACE_OS::strstr (name_, "\\");
          }

        if (0 == p)
          {
            break;
          }
        else
          {
            name_ = ++p;
            continue;
          }
      }

    ACE_CString name (name_);
    loc = loc.substr (0, loc.length() - name.length ());

#if defined (ACE_WIN32) && defined (ACE_LD_DECORATOR_STR) && !defined (ACE_DISABLE_DEBUG_DLL_CHECK)
    ACE_TString decorator (ACE_LD_DECORATOR_STR);
#endif
    ACE_TString prefix (ACE_DLL_PREFIX);
    ACE_TString suffix (ACE_DLL_SUFFIX);

    ACE_CString new_name (name);
    new_name = prefix + new_name;
#if defined (ACE_WIN32) && defined (ACE_LD_DECORATOR_STR) && !defined (ACE_DISABLE_DEBUG_DLL_CHECK)
    new_name += decorator;
#endif
    new_name += suffix;

    loc = loc + new_name;

    ACE_DEBUG ((LM_INFO,
                "Attempting to download %s\n",
                loc.c_str ()));

    URL_Parser *parser = TheURL_Parser::instance ();
    if (!parser->parseURL (const_cast<char*> (loc.c_str ())))
      return false;

    // Create a client
    TAO_HTTP_Client client;

    // Open the client
    if (client.open (parser->filename_,
                     parser->hostname_,
                     parser->port_) == -1)
    {
        client.close ();
        return false;
    }

    // Read from it
    if (client.read (&mb) <= 0)
    {
        client.close ();
        return false;
    }

    return true;
  }

  //This function attempts to write a sequence of bytes from an
  //ACE_Message_Block to a specified location. A 0 is returned
  //in the case of an error and a 1 upon success

  bool
  CIAO::Containers_Info_Map::write_to_disk (const char* full_path,
                                            ACE_Message_Block& mb,
                                            bool replace)
  {
    ACE_stat stat;

    if (ACE_OS::stat(full_path, &stat) != -1 && !replace)
       return false;

    // Open a file handle to the local filesystem
    ACE_HANDLE handle = ACE_OS::open (full_path, O_CREAT | O_TRUNC | O_WRONLY);
    if (handle == ACE_INVALID_HANDLE)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("[CIAO::Containers_Info_Map::write_to_disk]"),
                           ACE_TEXT (" file creation error")),
                           false);

    //write the data to the file
    for (ACE_Message_Block * curr = &mb; curr != 0; curr = curr->cont ())
    if (ACE_OS::write_n (handle, curr->rd_ptr(), curr->length()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("[CIAO::Containers_Info_Map::write_to_disk]"),
                           ACE_TEXT (" write error")),
                           false);

    // Close the file handle
    ACE_OS::close (handle);

    return true;
  }
