// $Id$

#ifndef CONFIG_EXPORT_VISITOR_H
#define CONFIG_EXPORT_VISITOR_H

#include "PICML/PICML.h"

namespace PICML
{

  class ConfigExportVisitor: public Visitor
  {
  public:
    ConfigExportVisitor(const std::string& output_path);

    virtual void
      Visit_RootFolder(const RootFolder& rf);
    virtual void
      Visit_ImplementationArtifacts(const ImplementationArtifacts& ias);
    virtual void
      Visit_ArtifactContainer(const ArtifactContainer& ac);
    virtual void
      Visit_ImplementationArtifact(const ImplementationArtifact& ia);

  private:
    std::string output_path_;
  };

} // namespace PICML

#endif // CONFIG_EXPORT_VISITOR_H
