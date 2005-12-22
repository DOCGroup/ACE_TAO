// $Id$

#include "Config_Handlers/XML_File_Intf.h"
#include "ciao/DeploymentC.h"
#include "ciao/CIAO_common.h"

#include "CPK_Packager.h"
#include "PDL_Handler.h"

namespace CIAO
{
  namespace Component_Packager
  {
  int
  CPK_Packager::preparePackage (PACKAGE_PLAN &pkg_plan)
  {
    ACE_TString impl_inzip_path ("implementations");
    impl_inzip_path += ACE_DIRECTORY_SEPARATOR_STR;

    ACE_TString desc_inzip_path ("descriptors");
    desc_inzip_path += ACE_DIRECTORY_SEPARATOR_STR;

    CIAO::Config_Handlers::XML_File_Intf intf (this->cdp_url_);
    Deployment::DeploymentPlan_var obj_plan = intf.get_plan ();

    const Deployment::DeploymentPlan &plan = obj_plan;

    //@@ Prepare package plan for implementations
    for (CORBA::ULong i = 0;
         i < plan.implementation.length ();
         i++)
    {
       ACE_TString name (plan.implementation[i].name);

       this->desc_mf_ << name << " " << ACE_PLATFORM << "\n";
       this->desc_mf_ << "{\n";

       CORBA::ULong artifact_len = plan.implementation[i].artifactRef.length ();

       ACE_TCHAR dll_pathname[MAXPATHLEN + 1];

       for (CORBA::ULong iter = 0;
            iter < artifact_len;
            iter++)
       {
         // Transform the pathname into the appropriate dynamic link library
         // by searching the ACE_LD_SEARCH_PATH.
         int result = ACE::ldfind (plan.artifact[plan.implementation[i].artifactRef[iter]].location[0],
                                   dll_pathname,
                                   (sizeof (dll_pathname) / sizeof (ACE_TCHAR)));

         if (result != 0)
         {
           ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("Resource not available!")
                      ACE_TEXT ("Please make sure you have compiled the component.\n")));
           return -1;
         }

         ACE_TString path (dll_pathname);
         ACE_TString base (ACE::basename (dll_pathname));

         ACE_TString inzip_path = impl_inzip_path + base;

         // store the plan info to predefined struct
         Component_Packager::DESC_PLAN_CONST_ITER plan_iter =
           pkg_plan.implementations.find (inzip_path);
         if (plan_iter != pkg_plan.descriptors.end ())
         {
           pkg_plan.implementations[inzip_path] = path;
         }
         this->desc_mf_ << " " << base << "\n";
       }
       this->desc_mf_ << "}\n";
    }

    //@@ prepare package plan for descriptors
    ACE_TString cdp_path (this->cdp_url_);
    ACE_TString cdp_inzip_path = desc_inzip_path +
                                 ACE::basename (cdp_path.c_str ());
    pkg_plan.descriptors[cdp_inzip_path] = cdp_path;

    // return if separated descriptors are not required to be inluded
    // in the package
    if (!this->include_std_desc_ )
      return 0;

    //@@ retrieve location information for standard descriptors
    PDL_Handler pdl_handler;
    PDL_Handler::DESC_LIST desc_list;

    if (-1 == pdl_handler.process_pdl (this->pdl_url_,
                                       desc_list))
    {
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("Error processing the .pdl file\n")));
      return -1;
    }

    for (PDL_Handler::DESC_LIST::iterator iter = desc_list.begin ();
         iter != desc_list.end ();
         ++iter)
    {
      ACE_TString path ((*iter).c_str ());
      ACE_TString inzip_path = desc_inzip_path + path;
      pkg_plan.descriptors [inzip_path] = path;
    }

    return 0;
  }

  int
  CPK_Packager::createPackage (const PACKAGE_PLAN &pkg_plan)
  {
    this->cpk_zip_.Open (this->cpk_file_,
                         CZipArchive::zipCreate);

    //@@ :Archive description
    ACE_DEBUG ((LM_INFO,
               ACE_TEXT ("ComponentPackager: Adding description ....")));

    //@@ Write the description file to the archive
    if (this->cpk_zip_.AddNewFile (this->desc_mf_, "description"))
    {
      ACE_DEBUG ((LM_INFO,
                 ACE_TEXT ("[Done]\n")));
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("\nfail to add description, operation aborted.")));
      return -1;
    }

    //@@ Archive descriptors
    for (Component_Packager::DESC_PLAN_CONST_ITER iter = pkg_plan.descriptors.begin ();
         iter != pkg_plan.descriptors.end ();
         iter++)
    {
      ACE_TString inzip_path ((*iter).first);
      ACE_TString path ((*iter).second);

      CZipAddNewFileInfo desc_info (path.c_str (),
                                    inzip_path.c_str ());

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("ComponentPackager: Adding %s ...."),
                  inzip_path.c_str ()));

      if (this->cpk_zip_.AddNewFile (desc_info))
      {
        ACE_DEBUG ((LM_INFO,
                   ACE_TEXT ("[Done]\n")));
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("\nfail to add %s, the target file")
                    ACE_TEXT ("may be corrupted or does not exist,operation aborted.\n"),
                    inzip_path.c_str ()));
        return -1;
      }
    }

    //@@ Archive implementations
    for (Component_Packager::IMPL_PLAN_CONST_ITER iter = pkg_plan.implementations.begin ();
         iter != pkg_plan.implementations.end ();
         iter++)
    {
      ACE_TString inzip_path ((*iter).first);
      ACE_TString path ((*iter).second);

      CZipAddNewFileInfo impl_info (path.c_str (),
                                    inzip_path.c_str ());

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("ComponentPackager: Adding %s ...."),
                  inzip_path.c_str ()));

      if (this->cpk_zip_.AddNewFile (impl_info))
      {
        ACE_DEBUG ((LM_INFO,
                   ACE_TEXT ("[Done]\n")));
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("\nfail to add %s, the target file")
                    ACE_TEXT ("may be corrupted or does not exist,operation aborted.\n"),
                    inzip_path.c_str ()));
        return -1;
      }
    }

    this->cpk_zip_.Close ();
    return 0;
  }


  // overloaded std::out operator
  CZipMemFile& operator << (CZipMemFile &mf,
                            const ACE_TCHAR *str)
  {
    mf.Write (str, ACE_OS::strlen (str));
    return mf;
  }

  // overloaded std::out operator
  CZipMemFile& operator << (CZipMemFile &mf,
                            const ACE_TString &str)
  {
    mf.Write (str.c_str (), str.length ());
    return mf;
  }


  }
}
