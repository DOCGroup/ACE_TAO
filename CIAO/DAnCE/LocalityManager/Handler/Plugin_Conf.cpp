// $Id$

#include "Plugin_Conf.h"
#include "ace/Read_Buffer.h"
#include "ace/Tokenizer_T.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/DAnCE_PropertiesC.h"

#include "Inst_Handler_Impl.h"
#include "Interceptor_Handler_Impl.h"
#include "Config_Handler_Impl.h"

#include <vector>
#include <iostream>

namespace DAnCE
{
  void 
  Plugin_Configurator::load_from_text_file (const ACE_TCHAR *file)
  {
    DANCE_TRACE ("Plugin_Configurator::load_from_text_file");
    
    if (!file)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plugin_Configurator::load_from_text_file - ")
                         ACE_TEXT("Error: Provided with nil filename\n")));
        return;
      }

    FILE *inf = ACE_OS::fopen (file, ACE_TEXT("r"));

    if (!inf)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plugin_Configurator::load_from_text_file - ")
                         ACE_TEXT("Fail to open plugin data file: <%s>\n"),
                         file));
        return;
      }
    
    ::Deployment::DeploymentPlan plan;

    ACE_Read_Buffer reader (inf, true);

    char * string = 0;
    CORBA::ULong pos (0);

    // Read from the file line by line
    while ((string = reader.read ('\n')) != 0)
      {
        try
          {
            size_t len = ACE_OS::strlen (string);

            if (len == 0) continue;
        
            std::istringstream tokenizer (string);
            std::vector< std::string > tokens;
            std::copy (std::istream_iterator< std::string > (tokenizer),
                       std::istream_iterator <std::string > (),
                       std::back_inserter < std::vector < std::string > > (tokens));
        
            if (tokens.size () != 3)
              {
                DANCE_ERROR (1, (LM_ERROR, DLINFO
                                 ACE_TEXT ("Plugin_Configurator::load_from_text_file - ")
                                 ACE_TEXT ("Skipping line <%C>, wrong format.  Got %u tokens instead of 3\n"),
                                 string,
                                 tokens.size ()));
              }
            else
              {
                DANCE_DEBUG (7, (LM_DEBUG, DLINFO
                                 ACE_TEXT ("Plugin_Configurator::load_from_text_file - ")
                                 ACE_TEXT ("Attempting to load plugin of type <%C>, artifact <%C>, entrypoint <%C>\n"),
                                 tokens[0].c_str (),
                                 tokens[1].c_str (),
                                 tokens[2].c_str ()));
                             

                if (ACE_OS::strcmp (tokens[0].c_str (), 
                                    DAnCE::DANCE_INSTALLATIONHANDLER) == 0)
                  {
                    this->create_entry (tokens[1].c_str (), tokens[2].c_str (),
                                        DAnCE::DANCE_INSTALLATIONHANDLER,
                                        plan, pos++);
                  }
                else if (ACE_OS::strcmp (tokens[0].c_str (), 
                                         DAnCE::DANCE_DEPLOYMENTINTERCEPTOR) == 0)
                  {
                    this->create_entry (tokens[1].c_str (), tokens[2].c_str (),
                                        DAnCE::DANCE_DEPLOYMENTINTERCEPTOR,
                                        plan, pos++);
                  }
                else if (ACE_OS::strcmp (tokens[0].c_str (), 
                                         DAnCE::DANCE_CONFIGPLUGIN) == 0)
                  {
                    this->create_entry (tokens[1].c_str (), tokens[2].c_str (),
                                        DAnCE::DANCE_CONFIGPLUGIN,
                                        plan, pos++);
                  }
                else
                  {
                    DANCE_ERROR (1, (LM_ERROR, DLINFO
                                     ACE_TEXT ("Plugin_Configurator::load_from_text_file - ")
                                     ACE_TEXT ("Skipping line <%C>, bad type name\n"),
                                     string));
                  }
              }
        
            reader.alloc ()->free (string);
          }
        catch (...)
          {
            reader.alloc ()->free (string);
            throw;
          }
      }

    this->deploy_plan (plan);
  }
  
  void
  Plugin_Configurator::deploy_plan (::Deployment::DeploymentPlan &plan)
  {
    DANCE_TRACE ("Plugin_Configurator::deploy_plan");
    
    DAnCE::Inst_Handler_Impl inst_handler;
    DAnCE::Interceptor_Handler_Impl interceptor_handler;
    DAnCE::Config_Handler_Impl config_handler;
    
    for (CORBA::ULong i = 0; i < plan.instance.length (); ++i)
      {
        if (plan.instance[i].implementationRef >= plan.implementation.length ())
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("Plugin_Configurator::deploy_plan - ")
                             ACE_TEXT ("Invalid implementationRef for instance <%C>\n"),
                             plan.instance[i].name.in ()));
            continue;
          }
        
        ::Deployment::MonolithicDeploymentDescription &mdd = 
            plan.implementation[plan.instance[i].implementationRef];

        CORBA::String_var inst_type = 
          DAnCE::Utility::get_instance_type (mdd.execParameter);
        
        if (ACE_OS::strcmp (inst_type.in (), 
                            DAnCE::DANCE_INSTALLATIONHANDLER) == 0)
          {
            CORBA::Any_var any;
            inst_handler.install_instance (plan,
                                           i,
                                           any.out ());
          }
        else if (ACE_OS::strcmp (inst_type.in (), 
                                 DAnCE::DANCE_DEPLOYMENTINTERCEPTOR) == 0)
          {
            CORBA::Any_var any;
            interceptor_handler.install_instance (plan,
                                                  i,
                                                  any.out ());
          }
        else if (ACE_OS::strcmp (inst_type.in (), 
                                 DAnCE::DANCE_CONFIGPLUGIN) == 0)
          {
            CORBA::Any_var any;
            config_handler.install_instance (plan,
                                             i,
                                             any.out ());
          }
      }
  }

  void 
  Plugin_Configurator::create_entry (const char *artifact,
                                     const char *entrypoint,
                                     const char *type,
                                     ::Deployment::DeploymentPlan &plan,
                                     CORBA::ULong pos)
  {
    DANCE_TRACE ("Plugin_Configurator::create_entry");

    plan.implementation.length (pos + 1);
    plan.implementation[pos].name = artifact;
    plan.implementation[pos].execParameter.length (3);

    plan.implementation[pos].execParameter[0].name = DAnCE::DANCE_PLUGIN_ARTIFACT;
    plan.implementation[pos].execParameter[0].value <<= 
      CORBA::Any::from_string (artifact, 0);

    plan.implementation[pos].execParameter[1].name = DAnCE::DANCE_PLUGIN_ENTRYPT;
    plan.implementation[pos].execParameter[1].value <<= 
      CORBA::Any::from_string (entrypoint, 0);

    plan.implementation[pos].execParameter[2].name = DAnCE::IMPL_TYPE;
    plan.implementation[pos].execParameter[2].value <<= 
      CORBA::Any::from_string (type, 0);
    
    plan.instance.length (pos + 1);
    plan.instance[pos].name = entrypoint;
    plan.instance[pos].implementationRef = pos;
  }
}
