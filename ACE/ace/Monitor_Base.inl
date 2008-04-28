// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    ACE_INLINE
    const char*
    Monitor_Base::name (void) const
    {
      return this->name_.c_str ();
    }

    ACE_INLINE
    void
    Monitor_Base::name (const char* new_name)
    {
      this->name_ = new_name;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

