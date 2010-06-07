// -*- C++ -*-
//
// $Id$

namespace DAnCE
{
  ACE_INLINE
  ArtifactRegistry::Version::Version () {}

  ACE_INLINE
  ArtifactRegistry::Version::Version (
      const std::string& protocol,
      const std::string& location)
    : protocol_ (protocol),
      location_ (location)
    {
    }

  ACE_INLINE
  ArtifactRegistry::Version::Version (const ArtifactRegistry::Version& version)
    {
      *this = version;
    }

  ACE_INLINE
  ArtifactRegistry::Version& ArtifactRegistry::Version::operator =(const ArtifactRegistry::Version& version)
    {
      this->protocol_ = version.protocol_;
      this->location_ = version.location_;
      return *this;
    }

  ACE_INLINE
  ArtifactRegistry& ArtifactRegistry::operator =(const ArtifactRegistry& ar)
  {
    this->versions_ = ar.versions ();
    return *this;
  }

  ACE_INLINE
  const std::string& ArtifactRegistry::location () const
    {
      return this->versions_.back ().location_;
    }

  ACE_INLINE
  ArtifactRegistry::TVersions& ArtifactRegistry::versions ()
  {
    return this->versions_;
  }

  ACE_INLINE
  const ArtifactRegistry::TVersions& ArtifactRegistry::versions () const
    {
      return this->versions_;
    }
};
