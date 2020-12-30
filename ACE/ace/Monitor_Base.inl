ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    ACE_INLINE
    const char*
    Monitor_Base::name () const
    {
      return this->name_.c_str ();
    }

    ACE_INLINE
    void
    Monitor_Base::name (const char* new_name)
    {
      this->name_ = new_name;
    }

    ACE_INLINE
    Monitor_Base::CONSTRAINTS&
    Monitor_Base::constraints ()
    {
      return this->constraints_;
    }

    ACE_INLINE
    void
    Monitor_Base::add_ref ()
    {
      (void) this->increment ();
    }

    ACE_INLINE
    void
    Monitor_Base::remove_ref ()
    {
      long const new_count = this->decrement ();

      if (new_count == 0)
        {
          delete this;
        }
    }

    ACE_INLINE
    Monitor_Control_Types::Information_Type
    Monitor_Base::type () const
    {
      return this->data_.type_;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

