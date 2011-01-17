// -*- C++ -*-
//
// $Id$

namespace DAnCE
{
  ACE_INLINE
  std::string ArtifactInstallationHandler::basename (const std::string& location, char dirsep)
  {
    std::string::size_type n = location.rfind (dirsep);
    return (n == std::string::npos) ? location : location.substr (n+1);
  }

  ACE_INLINE
  bool ArtifactInstallationProperties::has_property (const std::string& propname)
    {
      TPropertyMap::const_iterator it = this->propmap_.find (PROPERTY_BASE+propname);
      if (it == this->propmap_.end () && !this->protocol_.empty ())
        {
          it = this->propmap_.find (PROPERTY_BASE+this->protocol_+propname);
        }
      return it != this->propmap_.end ();
    }

  ACE_INLINE
  const std::string& ArtifactInstallationProperties::get_property (const std::string& propname)
    {
      TPropertyMap::const_iterator it = this->propmap_.find (PROPERTY_BASE+propname);
      if (it == this->propmap_.end () && !this->protocol_.empty ())
        {
          it = this->propmap_.find (PROPERTY_BASE+this->protocol_+propname);
        }
      if (it != this->propmap_.end ())
          return it->second;
      else
        return empty_;
    }
}
