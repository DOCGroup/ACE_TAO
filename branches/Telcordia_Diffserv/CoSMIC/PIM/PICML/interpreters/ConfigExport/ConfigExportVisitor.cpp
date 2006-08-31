#include "StdAfx.h"

#include "ConfigExport/ConfigExportVisitor.h"

#include <fstream>

namespace PICML
{

  ConfigExportVisitor::
  ConfigExportVisitor(const std::string& output_path)
    : output_path_ (output_path)
  {
  }

  void ConfigExportVisitor::
  Visit_RootFolder(const RootFolder& rf)
  {
    std::set<ImplementationArtifacts>
      folders = rf.ImplementationArtifacts_kind_children();

    for (std::set<ImplementationArtifacts>::iterator iter = folders.begin();
         iter != folders.end();
         ++iter)
      {
        ImplementationArtifacts folder = *iter;
        folder.Accept (*this);
      }
  }

  void ConfigExportVisitor::
  Visit_ImplementationArtifacts(const ImplementationArtifacts& ias)
  {
    std::set<ArtifactContainer>
      afs = ias.ArtifactContainer_kind_children();

    for (std::set<ArtifactContainer>::iterator iter = afs.begin();
         iter != afs.end();
         ++iter)
      {
        ArtifactContainer ac = *iter;
        ac.Accept (*this);
      }
  }

  void ConfigExportVisitor::
  Visit_ArtifactContainer(const ArtifactContainer& ac)
  {
    const std::set<ImplementationArtifact>
      ias = ac.ImplementationArtifact_kind_children();

    for (std::set<ImplementationArtifact>::const_iterator iter = ias.begin();
         iter != ias.end();
         ++iter)
      {
        ImplementationArtifact ia = *iter;
        ia.Accept (*this);
      }
  }

  void ConfigExportVisitor::
  Visit_ImplementationArtifact(const ImplementationArtifact& ia)
  {
    if (std::string(ia.configuration()).size() != 0)
      {
        std::string file_name = output_path_ + std::string("\\");
        file_name += ia.name();
        file_name += std::string("_svc.conf");
        std::ofstream f(file_name.c_str());
        f << ia.configuration();
      }
  }

} // namespace PICML
