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

  ACE_INLINE
  u_long ArtifactRegistry::install_count () const
    {
      return this->install_count_;
    }

  ACE_INLINE
  void ArtifactRegistry::increment_install_count ()
    {
      ++this->install_count_;
    }

  ACE_INLINE
  void ArtifactRegistry::decrement_install_count ()
    {
      --this->install_count_;
    }

  ACE_INLINE
  void ArtifactRegistry::set_locked ()
    {
      this->locked_ = true;
    }

  ACE_INLINE
  void ArtifactRegistry::set_unlocked ()
    {
      if (this->locked_)
        {
          this->locked_ = false;
          this->condition_.broadcast ();
        }
    }

  ACE_INLINE
  bool ArtifactRegistry::is_locked () const
    {
      return this->locked_;
    }

  ACE_INLINE
  ArtifactRegistry::Guard::Guard (ArtifactRegistry* ar)
    : arp_ (ar)
    {
    }

  ACE_INLINE
  ArtifactRegistry::Guard::~Guard ()
    {
      if (this->arp_)
        {
          this->arp_->set_unlocked ();
        }
      this->arp_ = 0;
    }

  ACE_INLINE
  ArtifactRegistry* ArtifactRegistry::Guard::operator ->(void)
    {
      return this->arp_;
    }

  ACE_INLINE
  ArtifactRegistry* ArtifactRegistry::Guard::operator &(void)
    {
      return this->arp_;
    }

  ACE_INLINE
  ArtifactRegistry& ArtifactRegistry::Guard::operator *(void)
    {
      return *this->arp_;
    }
}
