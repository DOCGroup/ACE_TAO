// $Id$

ACE_INLINE
TAO_EC_Object_Deactivator::TAO_EC_Object_Deactivator (void)
  : poa_ ()
  , id_ ()
  , deactivate_ (0)
{
}

ACE_INLINE
TAO_EC_Object_Deactivator::TAO_EC_Object_Deactivator (
                                        PortableServer::POA_ptr poa,
                                        PortableServer::ObjectId const & id)
  : poa_ (PortableServer::POA::_duplicate (poa))
  , id_ (id)
  , deactivate_ (1)
{
}

ACE_INLINE void
TAO_EC_Object_Deactivator::deactivate (void)
{
  if (this->deactivate_ && !CORBA::is_nil (this->poa_.in ()))
  {
    ACE_DECLARE_NEW_CORBA_ENV;
    ACE_TRY
    {
      this->deactivate_ = 0;
      this->poa_->deactivate_object (id_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
    ACE_CATCHANY
    {
      // The deactivation can throw...
    }
    ACE_ENDTRY;
  }
}

ACE_INLINE
TAO_EC_Object_Deactivator::~TAO_EC_Object_Deactivator (void)
{
  this->deactivate ();
}

ACE_INLINE PortableServer::POA_var
TAO_EC_Object_Deactivator::poa (void) const
{
  return this->poa_;
}

ACE_INLINE void
TAO_EC_Object_Deactivator::allow_deactivation (void)
{
  this->deactivate_ = 1;
}

ACE_INLINE void
TAO_EC_Object_Deactivator::disallow_deactivation (void)
{
  this->deactivate_ = 0;
}

ACE_INLINE void
TAO_EC_Object_Deactivator::set_values (PortableServer::POA_ptr poa,
                                       PortableServer::ObjectId const & id)
{
  this->poa_ = PortableServer::POA::_duplicate (poa);
  this->id_ = id;
  this->deactivate_ = 1;
}

ACE_INLINE void
TAO_EC_Object_Deactivator::set_values (TAO_EC_Object_Deactivator & deactivator)
{
  if (this == &deactivator)
    return;

  this->poa_ = deactivator.poa_._retn ();
  this->id_ = deactivator.id_;
  this->deactivate_ = deactivator.deactivate_;
  deactivator.deactivate_ = 0;
}

// ****************************************************************

ACE_INLINE
TAO_EC_Deactivated_Object::TAO_EC_Deactivated_Object (void)
  : deactivator_ ()
{
}

ACE_INLINE
TAO_EC_Deactivated_Object::~TAO_EC_Deactivated_Object (void)
{
  // Prevent automatic deactivation in deactivator's destructor.
  this->deactivator_.disallow_deactivation ();
}

ACE_INLINE void
TAO_EC_Deactivated_Object::set_deactivator (
                              TAO_EC_Object_Deactivator & deactivator)
{
  this->deactivator_.set_values (deactivator);
}

// ****************************************************************

ACE_INLINE
TAO_EC_ORB_Holder::TAO_EC_ORB_Holder (void)
  : orb_ ()
{
}

ACE_INLINE
TAO_EC_ORB_Holder::~TAO_EC_ORB_Holder (void)
{
  if (!CORBA::is_nil (this->orb_.in ()))
  {
    ACE_DECLARE_NEW_CORBA_ENV;
    ACE_TRY
      {
        this->orb_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    ACE_CATCHANY
      {
        // Ignore.
      }
    ACE_ENDTRY;
  }
}

ACE_INLINE void
TAO_EC_ORB_Holder::init (CORBA::ORB_var orb_var)
{
  this->orb_ = orb_var;
}

// ****************************************************************

ACE_INLINE
TAO_EC_Event_Channel_Holder::TAO_EC_Event_Channel_Holder (void)
  : ec_ ()
{
}

ACE_INLINE
TAO_EC_Event_Channel_Holder::~TAO_EC_Event_Channel_Holder (void)
{
  if (!CORBA::is_nil (this->ec_.in ()))
  {
    ACE_DECLARE_NEW_CORBA_ENV;
    ACE_TRY
      {
        this->ec_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    ACE_CATCHANY
      {
        // Ignore.
      }
    ACE_ENDTRY;
  }
}

ACE_INLINE void
TAO_EC_Event_Channel_Holder::init (
                    RtecEventChannelAdmin::EventChannel_var ec_var)
{
  this->ec_ = ec_var;
}
