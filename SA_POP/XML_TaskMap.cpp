#include "XML_TaskMap.hpp"

namespace SA_POP
{
  namespace XML
  {
    // TaskID
    // 

    TaskID::
    TaskID (::XMLSchema::int_ const& b__)
    : 
    Base__ (b__),
    regulator__ ()
    {
    }

    TaskID::
    TaskID (::SA_POP::XML::TaskID const& s)
    :
    Base__ (s),
    regulator__ ()
    {
    }

    ::SA_POP::XML::TaskID& TaskID::
    operator= (::SA_POP::XML::TaskID const& s)
    {
      static_cast< Base__& > (*this) = static_cast< Base__ const& > (s);

      return *this;
    }



    // TaskImplID
    // 

    TaskImplID::
    TaskImplID (::XMLSchema::string< wchar_t > const& b__)
    : 
    Base__ (b__),
    regulator__ ()
    {
    }

    TaskImplID::
    TaskImplID (::SA_POP::XML::TaskImplID const& s)
    :
    Base__ (s),
    regulator__ ()
    {
    }

    ::SA_POP::XML::TaskImplID& TaskImplID::
    operator= (::SA_POP::XML::TaskImplID const& s)
    {
      static_cast< Base__& > (*this) = static_cast< Base__ const& > (s);

      return *this;
    }



    // ResourceID
    // 

    ResourceID::
    ResourceID (::XMLSchema::string< wchar_t > const& b__)
    : 
    Base__ (b__),
    regulator__ ()
    {
    }

    ResourceID::
    ResourceID (::SA_POP::XML::ResourceID const& s)
    :
    Base__ (s),
    regulator__ ()
    {
    }

    ::SA_POP::XML::ResourceID& ResourceID::
    operator= (::SA_POP::XML::ResourceID const& s)
    {
      static_cast< Base__& > (*this) = static_cast< Base__ const& > (s);

      return *this;
    }



    // ResourceKind
    // 

    ResourceKind::Value ResourceKind::
    integral () const
    {
      return v_;
    }

    bool
    operator== (::SA_POP::XML::ResourceKind const& a, ::SA_POP::XML::ResourceKind const& b)
    {
      return a.v_ == b.v_;
    }

    bool
    operator!= (::SA_POP::XML::ResourceKind const& a, ::SA_POP::XML::ResourceKind const& b)
    {
      return a.v_ != b.v_;
    }

    ResourceKind::
    ResourceKind (ResourceKind::Value v)
    : v_ (v)
    {
    }

    // ResourceValue
    // 

    ResourceValue::
    ResourceValue (::XMLSchema::int_ const& b__)
    : 
    Base__ (b__),
    regulator__ ()
    {
    }

    ResourceValue::
    ResourceValue (::SA_POP::XML::ResourceValue const& s)
    :
    Base__ (s),
    regulator__ ()
    {
    }

    ::SA_POP::XML::ResourceValue& ResourceValue::
    operator= (::SA_POP::XML::ResourceValue const& s)
    {
      static_cast< Base__& > (*this) = static_cast< Base__ const& > (s);

      return *this;
    }



    // ImplParamID
    // 

    ImplParamID::
    ImplParamID (::XMLSchema::string< wchar_t > const& b__)
    : 
    Base__ (b__),
    regulator__ ()
    {
    }

    ImplParamID::
    ImplParamID (::SA_POP::XML::ImplParamID const& s)
    :
    Base__ (s),
    regulator__ ()
    {
    }

    ::SA_POP::XML::ImplParamID& ImplParamID::
    operator= (::SA_POP::XML::ImplParamID const& s)
    {
      static_cast< Base__& > (*this) = static_cast< Base__ const& > (s);

      return *this;
    }



    // ImplParamKind
    // 

    ImplParamKind::
    ImplParamKind (::XMLSchema::string< wchar_t > const& b__)
    : 
    Base__ (b__),
    regulator__ ()
    {
    }

    ImplParamKind::
    ImplParamKind (::SA_POP::XML::ImplParamKind const& s)
    :
    Base__ (s),
    regulator__ ()
    {
    }

    ::SA_POP::XML::ImplParamKind& ImplParamKind::
    operator= (::SA_POP::XML::ImplParamKind const& s)
    {
      static_cast< Base__& > (*this) = static_cast< Base__ const& > (s);

      return *this;
    }



    // ImplParamValue
    // 

    ImplParamValue::
    ImplParamValue (::XMLSchema::string< wchar_t > const& b__)
    : 
    Base__ (b__),
    regulator__ ()
    {
    }

    ImplParamValue::
    ImplParamValue (::SA_POP::XML::ImplParamValue const& s)
    :
    Base__ (s),
    regulator__ ()
    {
    }

    ::SA_POP::XML::ImplParamValue& ImplParamValue::
    operator= (::SA_POP::XML::ImplParamValue const& s)
    {
      static_cast< Base__& > (*this) = static_cast< Base__ const& > (s);

      return *this;
    }



    // TimeValue
    // 

    TimeValue::
    TimeValue (::XMLSchema::int_ const& b__)
    : 
    Base__ (b__),
    regulator__ ()
    {
    }

    TimeValue::
    TimeValue (::SA_POP::XML::TimeValue const& s)
    :
    Base__ (s),
    regulator__ ()
    {
    }

    ::SA_POP::XML::TimeValue& TimeValue::
    operator= (::SA_POP::XML::TimeValue const& s)
    {
      static_cast< Base__& > (*this) = static_cast< Base__ const& > (s);

      return *this;
    }



    // Resource
    // 

    Resource::
    Resource (::SA_POP::XML::ResourceID const& resourceID__,
              ::SA_POP::XML::ResourceKind const& kind__,
              ::SA_POP::XML::ResourceValue const& capacity__)
    : 
    ::XSCRT::Type (), 
    resourceID_ (new ::SA_POP::XML::ResourceID (resourceID__)),
    kind_ (new ::SA_POP::XML::ResourceKind (kind__)),
    capacity_ (new ::SA_POP::XML::ResourceValue (capacity__)),
    regulator__ ()
    {
      resourceID_->container (this);
      kind_->container (this);
      capacity_->container (this);
    }

    Resource::
    Resource (::SA_POP::XML::Resource const& s)
    :
    ::XSCRT::Type (),
    resourceID_ (new ::SA_POP::XML::ResourceID (*s.resourceID_)),
    kind_ (new ::SA_POP::XML::ResourceKind (*s.kind_)),
    capacity_ (new ::SA_POP::XML::ResourceValue (*s.capacity_)),
    regulator__ ()
    {
      resourceID_->container (this);
      kind_->container (this);
      capacity_->container (this);
    }

    ::SA_POP::XML::Resource& Resource::
    operator= (::SA_POP::XML::Resource const& s)
    {
      resourceID (s.resourceID ());

      kind (s.kind ());

      capacity (s.capacity ());

      return *this;
    }


    // Resource
    // 
    ::SA_POP::XML::ResourceID const& Resource::
    resourceID () const
    {
      return *resourceID_;
    }

    void Resource::
    resourceID (::SA_POP::XML::ResourceID const& e)
    {
      *resourceID_ = e;
    }

    // Resource
    // 
    ::SA_POP::XML::ResourceKind const& Resource::
    kind () const
    {
      return *kind_;
    }

    void Resource::
    kind (::SA_POP::XML::ResourceKind const& e)
    {
      *kind_ = e;
    }

    // Resource
    // 
    ::SA_POP::XML::ResourceValue const& Resource::
    capacity () const
    {
      return *capacity_;
    }

    void Resource::
    capacity (::SA_POP::XML::ResourceValue const& e)
    {
      *capacity_ = e;
    }


    // ImplParam
    // 

    ImplParam::
    ImplParam (::SA_POP::XML::ImplParamID const& paramID__,
               ::SA_POP::XML::ImplParamKind const& kind__,
               ::SA_POP::XML::ImplParamValue const& value__)
    : 
    ::XSCRT::Type (), 
    paramID_ (new ::SA_POP::XML::ImplParamID (paramID__)),
    kind_ (new ::SA_POP::XML::ImplParamKind (kind__)),
    value_ (new ::SA_POP::XML::ImplParamValue (value__)),
    regulator__ ()
    {
      paramID_->container (this);
      kind_->container (this);
      value_->container (this);
    }

    ImplParam::
    ImplParam (::SA_POP::XML::ImplParam const& s)
    :
    ::XSCRT::Type (),
    paramID_ (new ::SA_POP::XML::ImplParamID (*s.paramID_)),
    kind_ (new ::SA_POP::XML::ImplParamKind (*s.kind_)),
    value_ (new ::SA_POP::XML::ImplParamValue (*s.value_)),
    regulator__ ()
    {
      paramID_->container (this);
      kind_->container (this);
      value_->container (this);
    }

    ::SA_POP::XML::ImplParam& ImplParam::
    operator= (::SA_POP::XML::ImplParam const& s)
    {
      paramID (s.paramID ());

      kind (s.kind ());

      value (s.value ());

      return *this;
    }


    // ImplParam
    // 
    ::SA_POP::XML::ImplParamID const& ImplParam::
    paramID () const
    {
      return *paramID_;
    }

    void ImplParam::
    paramID (::SA_POP::XML::ImplParamID const& e)
    {
      *paramID_ = e;
    }

    // ImplParam
    // 
    ::SA_POP::XML::ImplParamKind const& ImplParam::
    kind () const
    {
      return *kind_;
    }

    void ImplParam::
    kind (::SA_POP::XML::ImplParamKind const& e)
    {
      *kind_ = e;
    }

    // ImplParam
    // 
    ::SA_POP::XML::ImplParamValue const& ImplParam::
    value () const
    {
      return *value_;
    }

    void ImplParam::
    value (::SA_POP::XML::ImplParamValue const& e)
    {
      *value_ = e;
    }


    // TaskImpl
    // 

    TaskImpl::
    TaskImpl (::SA_POP::XML::TaskImplID const& implID__)
    : 
    ::XSCRT::Type (), 
    implID_ (new ::SA_POP::XML::TaskImplID (implID__)),
    regulator__ ()
    {
      implID_->container (this);
    }

    TaskImpl::
    TaskImpl (::SA_POP::XML::TaskImpl const& s)
    :
    ::XSCRT::Type (),
    implID_ (new ::SA_POP::XML::TaskImplID (*s.implID_)),
    regulator__ ()
    {
      implID_->container (this);
      {
        for (param_const_iterator i (s.param_.begin ());i != s.param_.end ();++i) add_param (*i);
      }
    }

    ::SA_POP::XML::TaskImpl& TaskImpl::
    operator= (::SA_POP::XML::TaskImpl const& s)
    {
      implID (s.implID ());

      param_.clear ();
      {
        for (param_const_iterator i (s.param_.begin ());i != s.param_.end ();++i) add_param (*i);
      }

      return *this;
    }


    // TaskImpl
    // 
    ::SA_POP::XML::TaskImplID const& TaskImpl::
    implID () const
    {
      return *implID_;
    }

    void TaskImpl::
    implID (::SA_POP::XML::TaskImplID const& e)
    {
      *implID_ = e;
    }

    // TaskImpl
    // 
    TaskImpl::param_iterator TaskImpl::
    begin_param ()
    {
      return param_.begin ();
    }

    TaskImpl::param_iterator TaskImpl::
    end_param ()
    {
      return param_.end ();
    }

    TaskImpl::param_const_iterator TaskImpl::
    begin_param () const
    {
      return param_.begin ();
    }

    TaskImpl::param_const_iterator TaskImpl::
    end_param () const
    {
      return param_.end ();
    }

    void TaskImpl::
    add_param (::SA_POP::XML::ImplParam const& e)
    {
      param_.push_back (e);
    }

    size_t TaskImpl::
    count_param(void) const
    {
      return param_.size ();
    }


    // TaskToImpl
    // 

    TaskToImpl::
    TaskToImpl (::SA_POP::XML::TaskID const& taskID__,
                ::SA_POP::XML::TaskImplID const& implID__,
                ::SA_POP::XML::TimeValue const& duration__)
    : 
    ::XSCRT::Type (), 
    taskID_ (new ::SA_POP::XML::TaskID (taskID__)),
    implID_ (new ::SA_POP::XML::TaskImplID (implID__)),
    duration_ (new ::SA_POP::XML::TimeValue (duration__)),
    regulator__ ()
    {
      taskID_->container (this);
      implID_->container (this);
      duration_->container (this);
    }

    TaskToImpl::
    TaskToImpl (::SA_POP::XML::TaskToImpl const& s)
    :
    ::XSCRT::Type (),
    taskID_ (new ::SA_POP::XML::TaskID (*s.taskID_)),
    implID_ (new ::SA_POP::XML::TaskImplID (*s.implID_)),
    duration_ (new ::SA_POP::XML::TimeValue (*s.duration_)),
    regulator__ ()
    {
      taskID_->container (this);
      implID_->container (this);
      duration_->container (this);
    }

    ::SA_POP::XML::TaskToImpl& TaskToImpl::
    operator= (::SA_POP::XML::TaskToImpl const& s)
    {
      taskID (s.taskID ());

      implID (s.implID ());

      duration (s.duration ());

      return *this;
    }


    // TaskToImpl
    // 
    ::SA_POP::XML::TaskID const& TaskToImpl::
    taskID () const
    {
      return *taskID_;
    }

    void TaskToImpl::
    taskID (::SA_POP::XML::TaskID const& e)
    {
      *taskID_ = e;
    }

    // TaskToImpl
    // 
    ::SA_POP::XML::TaskImplID const& TaskToImpl::
    implID () const
    {
      return *implID_;
    }

    void TaskToImpl::
    implID (::SA_POP::XML::TaskImplID const& e)
    {
      *implID_ = e;
    }

    // TaskToImpl
    // 
    ::SA_POP::XML::TimeValue const& TaskToImpl::
    duration () const
    {
      return *duration_;
    }

    void TaskToImpl::
    duration (::SA_POP::XML::TimeValue const& e)
    {
      *duration_ = e;
    }


    // ImplToResource
    // 

    ImplToResource::
    ImplToResource (::SA_POP::XML::TaskImplID const& implID__,
                    ::SA_POP::XML::ResourceID const& resourceID__,
                    ::SA_POP::XML::ResourceValue const& utilization__)
    : 
    ::XSCRT::Type (), 
    implID_ (new ::SA_POP::XML::TaskImplID (implID__)),
    resourceID_ (new ::SA_POP::XML::ResourceID (resourceID__)),
    utilization_ (new ::SA_POP::XML::ResourceValue (utilization__)),
    regulator__ ()
    {
      implID_->container (this);
      resourceID_->container (this);
      utilization_->container (this);
    }

    ImplToResource::
    ImplToResource (::SA_POP::XML::ImplToResource const& s)
    :
    ::XSCRT::Type (),
    implID_ (new ::SA_POP::XML::TaskImplID (*s.implID_)),
    resourceID_ (new ::SA_POP::XML::ResourceID (*s.resourceID_)),
    utilization_ (new ::SA_POP::XML::ResourceValue (*s.utilization_)),
    regulator__ ()
    {
      implID_->container (this);
      resourceID_->container (this);
      utilization_->container (this);
    }

    ::SA_POP::XML::ImplToResource& ImplToResource::
    operator= (::SA_POP::XML::ImplToResource const& s)
    {
      implID (s.implID ());

      resourceID (s.resourceID ());

      utilization (s.utilization ());

      return *this;
    }


    // ImplToResource
    // 
    ::SA_POP::XML::TaskImplID const& ImplToResource::
    implID () const
    {
      return *implID_;
    }

    void ImplToResource::
    implID (::SA_POP::XML::TaskImplID const& e)
    {
      *implID_ = e;
    }

    // ImplToResource
    // 
    ::SA_POP::XML::ResourceID const& ImplToResource::
    resourceID () const
    {
      return *resourceID_;
    }

    void ImplToResource::
    resourceID (::SA_POP::XML::ResourceID const& e)
    {
      *resourceID_ = e;
    }

    // ImplToResource
    // 
    ::SA_POP::XML::ResourceValue const& ImplToResource::
    utilization () const
    {
      return *utilization_;
    }

    void ImplToResource::
    utilization (::SA_POP::XML::ResourceValue const& e)
    {
      *utilization_ = e;
    }


    // TaskMap
    // 

    TaskMap::
    TaskMap ()
    : 
    ::XSCRT::Type (), 
    regulator__ ()
    {
    }

    TaskMap::
    TaskMap (::SA_POP::XML::TaskMap const& s)
    :
    ::XSCRT::Type (),
    regulator__ ()
    {
      {
        for (taskImpl_const_iterator i (s.taskImpl_.begin ());i != s.taskImpl_.end ();++i) add_taskImpl (*i);
      }

      {
        for (resource_const_iterator i (s.resource_.begin ());i != s.resource_.end ();++i) add_resource (*i);
      }

      {
        for (taskToImpl_const_iterator i (s.taskToImpl_.begin ());i != s.taskToImpl_.end ();++i) add_taskToImpl (*i);
      }

      {
        for (implToResource_const_iterator i (s.implToResource_.begin ());i != s.implToResource_.end ();++i) add_implToResource (*i);
      }
    }

    ::SA_POP::XML::TaskMap& TaskMap::
    operator= (::SA_POP::XML::TaskMap const& s)
    {
      taskImpl_.clear ();
      {
        for (taskImpl_const_iterator i (s.taskImpl_.begin ());i != s.taskImpl_.end ();++i) add_taskImpl (*i);
      }

      resource_.clear ();
      {
        for (resource_const_iterator i (s.resource_.begin ());i != s.resource_.end ();++i) add_resource (*i);
      }

      taskToImpl_.clear ();
      {
        for (taskToImpl_const_iterator i (s.taskToImpl_.begin ());i != s.taskToImpl_.end ();++i) add_taskToImpl (*i);
      }

      implToResource_.clear ();
      {
        for (implToResource_const_iterator i (s.implToResource_.begin ());i != s.implToResource_.end ();++i) add_implToResource (*i);
      }

      return *this;
    }


    // TaskMap
    // 
    TaskMap::taskImpl_iterator TaskMap::
    begin_taskImpl ()
    {
      return taskImpl_.begin ();
    }

    TaskMap::taskImpl_iterator TaskMap::
    end_taskImpl ()
    {
      return taskImpl_.end ();
    }

    TaskMap::taskImpl_const_iterator TaskMap::
    begin_taskImpl () const
    {
      return taskImpl_.begin ();
    }

    TaskMap::taskImpl_const_iterator TaskMap::
    end_taskImpl () const
    {
      return taskImpl_.end ();
    }

    void TaskMap::
    add_taskImpl (::SA_POP::XML::TaskImpl const& e)
    {
      taskImpl_.push_back (e);
    }

    size_t TaskMap::
    count_taskImpl(void) const
    {
      return taskImpl_.size ();
    }

    // TaskMap
    // 
    TaskMap::resource_iterator TaskMap::
    begin_resource ()
    {
      return resource_.begin ();
    }

    TaskMap::resource_iterator TaskMap::
    end_resource ()
    {
      return resource_.end ();
    }

    TaskMap::resource_const_iterator TaskMap::
    begin_resource () const
    {
      return resource_.begin ();
    }

    TaskMap::resource_const_iterator TaskMap::
    end_resource () const
    {
      return resource_.end ();
    }

    void TaskMap::
    add_resource (::SA_POP::XML::Resource const& e)
    {
      resource_.push_back (e);
    }

    size_t TaskMap::
    count_resource(void) const
    {
      return resource_.size ();
    }

    // TaskMap
    // 
    TaskMap::taskToImpl_iterator TaskMap::
    begin_taskToImpl ()
    {
      return taskToImpl_.begin ();
    }

    TaskMap::taskToImpl_iterator TaskMap::
    end_taskToImpl ()
    {
      return taskToImpl_.end ();
    }

    TaskMap::taskToImpl_const_iterator TaskMap::
    begin_taskToImpl () const
    {
      return taskToImpl_.begin ();
    }

    TaskMap::taskToImpl_const_iterator TaskMap::
    end_taskToImpl () const
    {
      return taskToImpl_.end ();
    }

    void TaskMap::
    add_taskToImpl (::SA_POP::XML::TaskToImpl const& e)
    {
      taskToImpl_.push_back (e);
    }

    size_t TaskMap::
    count_taskToImpl(void) const
    {
      return taskToImpl_.size ();
    }

    // TaskMap
    // 
    TaskMap::implToResource_iterator TaskMap::
    begin_implToResource ()
    {
      return implToResource_.begin ();
    }

    TaskMap::implToResource_iterator TaskMap::
    end_implToResource ()
    {
      return implToResource_.end ();
    }

    TaskMap::implToResource_const_iterator TaskMap::
    begin_implToResource () const
    {
      return implToResource_.begin ();
    }

    TaskMap::implToResource_const_iterator TaskMap::
    end_implToResource () const
    {
      return implToResource_.end ();
    }

    void TaskMap::
    add_implToResource (::SA_POP::XML::ImplToResource const& e)
    {
      implToResource_.push_back (e);
    }

    size_t TaskMap::
    count_implToResource(void) const
    {
      return implToResource_.size ();
    }
  }
}

namespace SA_POP
{
  namespace XML
  {
    // TaskID
    //

    TaskID::
    TaskID (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {
    }

    TaskID::
    TaskID (::XSCRT::XML::Attribute< wchar_t > const& a)
    :
    Base__ (a),
    regulator__ ()
    {
    }

    // TaskImplID
    //

    TaskImplID::
    TaskImplID (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {
    }

    TaskImplID::
    TaskImplID (::XSCRT::XML::Attribute< wchar_t > const& a)
    :
    Base__ (a),
    regulator__ ()
    {
    }

    // ResourceID
    //

    ResourceID::
    ResourceID (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {
    }

    ResourceID::
    ResourceID (::XSCRT::XML::Attribute< wchar_t > const& a)
    :
    Base__ (a),
    regulator__ ()
    {
    }

    // ResourceKind
    //

    ResourceKind::
    ResourceKind (::XSCRT::XML::Element< wchar_t > const& e)
    : ::XSCRT::Type (e)
    {
      ::std::basic_string< wchar_t > v (e.value ());

      if (v == L"UNARY") v_ = UNARY_l;
      else if (v == L"DISCRETE") v_ = DISCRETE_l;
      else if (v == L"RESERVOIR") v_ = RESERVOIR_l;
      else 
      {
      }
    }

    ResourceKind::
    ResourceKind (::XSCRT::XML::Attribute< wchar_t > const& a)
    : ::XSCRT::Type (a)
    {
      ::std::basic_string< wchar_t > v (a.value ());

      if (v == L"UNARY") v_ = UNARY_l;
      else if (v == L"DISCRETE") v_ = DISCRETE_l;
      else if (v == L"RESERVOIR") v_ = RESERVOIR_l;
      else 
      {
      }
    }

    ResourceKind const ResourceKind::UNARY (ResourceKind::UNARY_l);
    ResourceKind const ResourceKind::DISCRETE (ResourceKind::DISCRETE_l);
    ResourceKind const ResourceKind::RESERVOIR (ResourceKind::RESERVOIR_l);

    // ResourceValue
    //

    ResourceValue::
    ResourceValue (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {
    }

    ResourceValue::
    ResourceValue (::XSCRT::XML::Attribute< wchar_t > const& a)
    :
    Base__ (a),
    regulator__ ()
    {
    }

    // ImplParamID
    //

    ImplParamID::
    ImplParamID (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {
    }

    ImplParamID::
    ImplParamID (::XSCRT::XML::Attribute< wchar_t > const& a)
    :
    Base__ (a),
    regulator__ ()
    {
    }

    // ImplParamKind
    //

    ImplParamKind::
    ImplParamKind (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {
    }

    ImplParamKind::
    ImplParamKind (::XSCRT::XML::Attribute< wchar_t > const& a)
    :
    Base__ (a),
    regulator__ ()
    {
    }

    // ImplParamValue
    //

    ImplParamValue::
    ImplParamValue (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {
    }

    ImplParamValue::
    ImplParamValue (::XSCRT::XML::Attribute< wchar_t > const& a)
    :
    Base__ (a),
    regulator__ ()
    {
    }

    // TimeValue
    //

    TimeValue::
    TimeValue (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {
    }

    TimeValue::
    TimeValue (::XSCRT::XML::Attribute< wchar_t > const& a)
    :
    Base__ (a),
    regulator__ ()
    {
    }

    // Resource
    //

    Resource::
    Resource (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< wchar_t > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
        ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == L"resourceID")
        {
          resourceID_ = ::std::auto_ptr< ::SA_POP::XML::ResourceID > (new ::SA_POP::XML::ResourceID (e));
          resourceID_->container (this);
        }

        else if (n == L"kind")
        {
          kind_ = ::std::auto_ptr< ::SA_POP::XML::ResourceKind > (new ::SA_POP::XML::ResourceKind (e));
          kind_->container (this);
        }

        else if (n == L"capacity")
        {
          capacity_ = ::std::auto_ptr< ::SA_POP::XML::ResourceValue > (new ::SA_POP::XML::ResourceValue (e));
          capacity_->container (this);
        }

        else 
        {
        }
      }
    }

    // ImplParam
    //

    ImplParam::
    ImplParam (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< wchar_t > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
        ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == L"paramID")
        {
          paramID_ = ::std::auto_ptr< ::SA_POP::XML::ImplParamID > (new ::SA_POP::XML::ImplParamID (e));
          paramID_->container (this);
        }

        else if (n == L"kind")
        {
          kind_ = ::std::auto_ptr< ::SA_POP::XML::ImplParamKind > (new ::SA_POP::XML::ImplParamKind (e));
          kind_->container (this);
        }

        else if (n == L"value")
        {
          value_ = ::std::auto_ptr< ::SA_POP::XML::ImplParamValue > (new ::SA_POP::XML::ImplParamValue (e));
          value_->container (this);
        }

        else 
        {
        }
      }
    }

    // TaskImpl
    //

    TaskImpl::
    TaskImpl (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< wchar_t > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
        ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == L"implID")
        {
          implID_ = ::std::auto_ptr< ::SA_POP::XML::TaskImplID > (new ::SA_POP::XML::TaskImplID (e));
          implID_->container (this);
        }

        else if (n == L"param")
        {
          ::SA_POP::XML::ImplParam t (e);
          add_param (t);
        }

        else 
        {
        }
      }
    }

    // TaskToImpl
    //

    TaskToImpl::
    TaskToImpl (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< wchar_t > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
        ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == L"taskID")
        {
          taskID_ = ::std::auto_ptr< ::SA_POP::XML::TaskID > (new ::SA_POP::XML::TaskID (e));
          taskID_->container (this);
        }

        else if (n == L"implID")
        {
          implID_ = ::std::auto_ptr< ::SA_POP::XML::TaskImplID > (new ::SA_POP::XML::TaskImplID (e));
          implID_->container (this);
        }

        else if (n == L"duration")
        {
          duration_ = ::std::auto_ptr< ::SA_POP::XML::TimeValue > (new ::SA_POP::XML::TimeValue (e));
          duration_->container (this);
        }

        else 
        {
        }
      }
    }

    // ImplToResource
    //

    ImplToResource::
    ImplToResource (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< wchar_t > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
        ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == L"implID")
        {
          implID_ = ::std::auto_ptr< ::SA_POP::XML::TaskImplID > (new ::SA_POP::XML::TaskImplID (e));
          implID_->container (this);
        }

        else if (n == L"resourceID")
        {
          resourceID_ = ::std::auto_ptr< ::SA_POP::XML::ResourceID > (new ::SA_POP::XML::ResourceID (e));
          resourceID_->container (this);
        }

        else if (n == L"utilization")
        {
          utilization_ = ::std::auto_ptr< ::SA_POP::XML::ResourceValue > (new ::SA_POP::XML::ResourceValue (e));
          utilization_->container (this);
        }

        else 
        {
        }
      }
    }

    // TaskMap
    //

    TaskMap::
    TaskMap (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< wchar_t > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
        ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == L"taskImpl")
        {
          ::SA_POP::XML::TaskImpl t (e);
          add_taskImpl (t);
        }

        else if (n == L"resource")
        {
          ::SA_POP::XML::Resource t (e);
          add_resource (t);
        }

        else if (n == L"taskToImpl")
        {
          ::SA_POP::XML::TaskToImpl t (e);
          add_taskToImpl (t);
        }

        else if (n == L"implToResource")
        {
          ::SA_POP::XML::ImplToResource t (e);
          add_implToResource (t);
        }

        else 
        {
        }
      }
    }
  }
}

namespace SA_POP
{
  namespace XML
  {
    ::SA_POP::XML::TaskMap
    taskMap (xercesc::DOMDocument const* d)
    {
      ::XSCRT::XML::Element< wchar_t > e (d->getDocumentElement ());
      if (e.name () == L"taskMap")
      {
        ::SA_POP::XML::TaskMap r (e);
        return r;
      }

      else
      {
        throw 1;
      }
    }
  }
}

#include "XMLSchema/TypeInfo.hpp"

namespace SA_POP
{
  namespace XML
  {
    namespace
    {
      ::XMLSchema::TypeInfoInitializer < wchar_t > XMLSchemaTypeInfoInitializer_ (::XSCRT::extended_type_info_map ());

      struct TaskIDTypeInfoInitializer
      {
        TaskIDTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (TaskID));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XMLSchema::int_));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      TaskIDTypeInfoInitializer TaskIDTypeInfoInitializer_;

      struct TaskImplIDTypeInfoInitializer
      {
        TaskImplIDTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (TaskImplID));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XMLSchema::string< wchar_t >));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      TaskImplIDTypeInfoInitializer TaskImplIDTypeInfoInitializer_;

      struct ResourceIDTypeInfoInitializer
      {
        ResourceIDTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ResourceID));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XMLSchema::string< wchar_t >));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ResourceIDTypeInfoInitializer ResourceIDTypeInfoInitializer_;

      struct ResourceKindTypeInfoInitializer
      {
        ResourceKindTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ResourceKind));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));

          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ResourceKindTypeInfoInitializer ResourceKindTypeInfoInitializer_;

      struct ResourceValueTypeInfoInitializer
      {
        ResourceValueTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ResourceValue));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XMLSchema::int_));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ResourceValueTypeInfoInitializer ResourceValueTypeInfoInitializer_;

      struct ImplParamIDTypeInfoInitializer
      {
        ImplParamIDTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ImplParamID));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XMLSchema::string< wchar_t >));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ImplParamIDTypeInfoInitializer ImplParamIDTypeInfoInitializer_;

      struct ImplParamKindTypeInfoInitializer
      {
        ImplParamKindTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ImplParamKind));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XMLSchema::string< wchar_t >));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ImplParamKindTypeInfoInitializer ImplParamKindTypeInfoInitializer_;

      struct ImplParamValueTypeInfoInitializer
      {
        ImplParamValueTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ImplParamValue));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XMLSchema::string< wchar_t >));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ImplParamValueTypeInfoInitializer ImplParamValueTypeInfoInitializer_;

      struct TimeValueTypeInfoInitializer
      {
        TimeValueTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (TimeValue));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XMLSchema::int_));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      TimeValueTypeInfoInitializer TimeValueTypeInfoInitializer_;

      struct ResourceTypeInfoInitializer
      {
        ResourceTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (Resource));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ResourceTypeInfoInitializer ResourceTypeInfoInitializer_;

      struct ImplParamTypeInfoInitializer
      {
        ImplParamTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ImplParam));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ImplParamTypeInfoInitializer ImplParamTypeInfoInitializer_;

      struct TaskImplTypeInfoInitializer
      {
        TaskImplTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (TaskImpl));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      TaskImplTypeInfoInitializer TaskImplTypeInfoInitializer_;

      struct TaskToImplTypeInfoInitializer
      {
        TaskToImplTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (TaskToImpl));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      TaskToImplTypeInfoInitializer TaskToImplTypeInfoInitializer_;

      struct ImplToResourceTypeInfoInitializer
      {
        ImplToResourceTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ImplToResource));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ImplToResourceTypeInfoInitializer ImplToResourceTypeInfoInitializer_;

      struct TaskMapTypeInfoInitializer
      {
        TaskMapTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (TaskMap));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      TaskMapTypeInfoInitializer TaskMapTypeInfoInitializer_;
    }
  }
}

namespace SA_POP
{
  namespace XML
  {
    namespace Traversal
    {
      // TaskID
      //
      //

      void TaskID::
      traverse (Type& o)
      {
        pre (o);
        post (o);
      }

      void TaskID::
      traverse (Type const& o)
      {
        pre (o);
        post (o);
      }

      void TaskID::
      pre (Type&)
      {
      }

      void TaskID::
      pre (Type const&)
      {
      }

      void TaskID::
      post (Type&)
      {
      }

      void TaskID::
      post (Type const&)
      {
      }

      // TaskImplID
      //
      //

      void TaskImplID::
      traverse (Type& o)
      {
        pre (o);
        post (o);
      }

      void TaskImplID::
      traverse (Type const& o)
      {
        pre (o);
        post (o);
      }

      void TaskImplID::
      pre (Type&)
      {
      }

      void TaskImplID::
      pre (Type const&)
      {
      }

      void TaskImplID::
      post (Type&)
      {
      }

      void TaskImplID::
      post (Type const&)
      {
      }

      // ResourceID
      //
      //

      void ResourceID::
      traverse (Type& o)
      {
        pre (o);
        post (o);
      }

      void ResourceID::
      traverse (Type const& o)
      {
        pre (o);
        post (o);
      }

      void ResourceID::
      pre (Type&)
      {
      }

      void ResourceID::
      pre (Type const&)
      {
      }

      void ResourceID::
      post (Type&)
      {
      }

      void ResourceID::
      post (Type const&)
      {
      }

      // ResourceValue
      //
      //

      void ResourceValue::
      traverse (Type& o)
      {
        pre (o);
        post (o);
      }

      void ResourceValue::
      traverse (Type const& o)
      {
        pre (o);
        post (o);
      }

      void ResourceValue::
      pre (Type&)
      {
      }

      void ResourceValue::
      pre (Type const&)
      {
      }

      void ResourceValue::
      post (Type&)
      {
      }

      void ResourceValue::
      post (Type const&)
      {
      }

      // ImplParamID
      //
      //

      void ImplParamID::
      traverse (Type& o)
      {
        pre (o);
        post (o);
      }

      void ImplParamID::
      traverse (Type const& o)
      {
        pre (o);
        post (o);
      }

      void ImplParamID::
      pre (Type&)
      {
      }

      void ImplParamID::
      pre (Type const&)
      {
      }

      void ImplParamID::
      post (Type&)
      {
      }

      void ImplParamID::
      post (Type const&)
      {
      }

      // ImplParamKind
      //
      //

      void ImplParamKind::
      traverse (Type& o)
      {
        pre (o);
        post (o);
      }

      void ImplParamKind::
      traverse (Type const& o)
      {
        pre (o);
        post (o);
      }

      void ImplParamKind::
      pre (Type&)
      {
      }

      void ImplParamKind::
      pre (Type const&)
      {
      }

      void ImplParamKind::
      post (Type&)
      {
      }

      void ImplParamKind::
      post (Type const&)
      {
      }

      // ImplParamValue
      //
      //

      void ImplParamValue::
      traverse (Type& o)
      {
        pre (o);
        post (o);
      }

      void ImplParamValue::
      traverse (Type const& o)
      {
        pre (o);
        post (o);
      }

      void ImplParamValue::
      pre (Type&)
      {
      }

      void ImplParamValue::
      pre (Type const&)
      {
      }

      void ImplParamValue::
      post (Type&)
      {
      }

      void ImplParamValue::
      post (Type const&)
      {
      }

      // TimeValue
      //
      //

      void TimeValue::
      traverse (Type& o)
      {
        pre (o);
        post (o);
      }

      void TimeValue::
      traverse (Type const& o)
      {
        pre (o);
        post (o);
      }

      void TimeValue::
      pre (Type&)
      {
      }

      void TimeValue::
      pre (Type const&)
      {
      }

      void TimeValue::
      post (Type&)
      {
      }

      void TimeValue::
      post (Type const&)
      {
      }

      // Resource
      //
      //

      void Resource::
      traverse (Type& o)
      {
        pre (o);
        resourceID (o);
        kind (o);
        capacity (o);
        post (o);
      }

      void Resource::
      traverse (Type const& o)
      {
        pre (o);
        resourceID (o);
        kind (o);
        capacity (o);
        post (o);
      }

      void Resource::
      pre (Type&)
      {
      }

      void Resource::
      pre (Type const&)
      {
      }

      void Resource::
      resourceID (Type& o)
      {
        dispatch (o.resourceID ());
      }

      void Resource::
      resourceID (Type const& o)
      {
        dispatch (o.resourceID ());
      }

      void Resource::
      kind (Type& o)
      {
        dispatch (o.kind ());
      }

      void Resource::
      kind (Type const& o)
      {
        dispatch (o.kind ());
      }

      void Resource::
      capacity (Type& o)
      {
        dispatch (o.capacity ());
      }

      void Resource::
      capacity (Type const& o)
      {
        dispatch (o.capacity ());
      }

      void Resource::
      post (Type&)
      {
      }

      void Resource::
      post (Type const&)
      {
      }

      // ImplParam
      //
      //

      void ImplParam::
      traverse (Type& o)
      {
        pre (o);
        paramID (o);
        kind (o);
        value (o);
        post (o);
      }

      void ImplParam::
      traverse (Type const& o)
      {
        pre (o);
        paramID (o);
        kind (o);
        value (o);
        post (o);
      }

      void ImplParam::
      pre (Type&)
      {
      }

      void ImplParam::
      pre (Type const&)
      {
      }

      void ImplParam::
      paramID (Type& o)
      {
        dispatch (o.paramID ());
      }

      void ImplParam::
      paramID (Type const& o)
      {
        dispatch (o.paramID ());
      }

      void ImplParam::
      kind (Type& o)
      {
        dispatch (o.kind ());
      }

      void ImplParam::
      kind (Type const& o)
      {
        dispatch (o.kind ());
      }

      void ImplParam::
      value (Type& o)
      {
        dispatch (o.value ());
      }

      void ImplParam::
      value (Type const& o)
      {
        dispatch (o.value ());
      }

      void ImplParam::
      post (Type&)
      {
      }

      void ImplParam::
      post (Type const&)
      {
      }

      // TaskImpl
      //
      //

      void TaskImpl::
      traverse (Type& o)
      {
        pre (o);
        implID (o);
        param (o);
        post (o);
      }

      void TaskImpl::
      traverse (Type const& o)
      {
        pre (o);
        implID (o);
        param (o);
        post (o);
      }

      void TaskImpl::
      pre (Type&)
      {
      }

      void TaskImpl::
      pre (Type const&)
      {
      }

      void TaskImpl::
      implID (Type& o)
      {
        dispatch (o.implID ());
      }

      void TaskImpl::
      implID (Type const& o)
      {
        dispatch (o.implID ());
      }

      void TaskImpl::
      param (Type& o)
      {
        // VC6 anathema strikes again
        //
        TaskImpl::Type::param_iterator b (o.begin_param()), e (o.end_param());

        if (b != e)
        {
          param_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) param_next (o);
          }

          param_post (o);
        }
      }

      void TaskImpl::
      param (Type const& o)
      {
        // VC6 anathema strikes again
        //
        TaskImpl::Type::param_const_iterator b (o.begin_param()), e (o.end_param());

        if (b != e)
        {
          param_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) param_next (o);
          }

          param_post (o);
        }
      }

      void TaskImpl::
      param_pre (Type&)
      {
      }

      void TaskImpl::
      param_pre (Type const&)
      {
      }

      void TaskImpl::
      param_next (Type&)
      {
      }

      void TaskImpl::
      param_next (Type const&)
      {
      }

      void TaskImpl::
      param_post (Type&)
      {
      }

      void TaskImpl::
      param_post (Type const&)
      {
      }

      void TaskImpl::
      post (Type&)
      {
      }

      void TaskImpl::
      post (Type const&)
      {
      }

      // TaskToImpl
      //
      //

      void TaskToImpl::
      traverse (Type& o)
      {
        pre (o);
        taskID (o);
        implID (o);
        duration (o);
        post (o);
      }

      void TaskToImpl::
      traverse (Type const& o)
      {
        pre (o);
        taskID (o);
        implID (o);
        duration (o);
        post (o);
      }

      void TaskToImpl::
      pre (Type&)
      {
      }

      void TaskToImpl::
      pre (Type const&)
      {
      }

      void TaskToImpl::
      taskID (Type& o)
      {
        dispatch (o.taskID ());
      }

      void TaskToImpl::
      taskID (Type const& o)
      {
        dispatch (o.taskID ());
      }

      void TaskToImpl::
      implID (Type& o)
      {
        dispatch (o.implID ());
      }

      void TaskToImpl::
      implID (Type const& o)
      {
        dispatch (o.implID ());
      }

      void TaskToImpl::
      duration (Type& o)
      {
        dispatch (o.duration ());
      }

      void TaskToImpl::
      duration (Type const& o)
      {
        dispatch (o.duration ());
      }

      void TaskToImpl::
      post (Type&)
      {
      }

      void TaskToImpl::
      post (Type const&)
      {
      }

      // ImplToResource
      //
      //

      void ImplToResource::
      traverse (Type& o)
      {
        pre (o);
        implID (o);
        resourceID (o);
        utilization (o);
        post (o);
      }

      void ImplToResource::
      traverse (Type const& o)
      {
        pre (o);
        implID (o);
        resourceID (o);
        utilization (o);
        post (o);
      }

      void ImplToResource::
      pre (Type&)
      {
      }

      void ImplToResource::
      pre (Type const&)
      {
      }

      void ImplToResource::
      implID (Type& o)
      {
        dispatch (o.implID ());
      }

      void ImplToResource::
      implID (Type const& o)
      {
        dispatch (o.implID ());
      }

      void ImplToResource::
      resourceID (Type& o)
      {
        dispatch (o.resourceID ());
      }

      void ImplToResource::
      resourceID (Type const& o)
      {
        dispatch (o.resourceID ());
      }

      void ImplToResource::
      utilization (Type& o)
      {
        dispatch (o.utilization ());
      }

      void ImplToResource::
      utilization (Type const& o)
      {
        dispatch (o.utilization ());
      }

      void ImplToResource::
      post (Type&)
      {
      }

      void ImplToResource::
      post (Type const&)
      {
      }

      // TaskMap
      //
      //

      void TaskMap::
      traverse (Type& o)
      {
        pre (o);
        taskImpl (o);
        resource (o);
        taskToImpl (o);
        implToResource (o);
        post (o);
      }

      void TaskMap::
      traverse (Type const& o)
      {
        pre (o);
        taskImpl (o);
        resource (o);
        taskToImpl (o);
        implToResource (o);
        post (o);
      }

      void TaskMap::
      pre (Type&)
      {
      }

      void TaskMap::
      pre (Type const&)
      {
      }

      void TaskMap::
      taskImpl (Type& o)
      {
        // VC6 anathema strikes again
        //
        TaskMap::Type::taskImpl_iterator b (o.begin_taskImpl()), e (o.end_taskImpl());

        if (b != e)
        {
          taskImpl_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) taskImpl_next (o);
          }

          taskImpl_post (o);
        }
      }

      void TaskMap::
      taskImpl (Type const& o)
      {
        // VC6 anathema strikes again
        //
        TaskMap::Type::taskImpl_const_iterator b (o.begin_taskImpl()), e (o.end_taskImpl());

        if (b != e)
        {
          taskImpl_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) taskImpl_next (o);
          }

          taskImpl_post (o);
        }
      }

      void TaskMap::
      taskImpl_pre (Type&)
      {
      }

      void TaskMap::
      taskImpl_pre (Type const&)
      {
      }

      void TaskMap::
      taskImpl_next (Type&)
      {
      }

      void TaskMap::
      taskImpl_next (Type const&)
      {
      }

      void TaskMap::
      taskImpl_post (Type&)
      {
      }

      void TaskMap::
      taskImpl_post (Type const&)
      {
      }

      void TaskMap::
      resource (Type& o)
      {
        // VC6 anathema strikes again
        //
        TaskMap::Type::resource_iterator b (o.begin_resource()), e (o.end_resource());

        if (b != e)
        {
          resource_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) resource_next (o);
          }

          resource_post (o);
        }
      }

      void TaskMap::
      resource (Type const& o)
      {
        // VC6 anathema strikes again
        //
        TaskMap::Type::resource_const_iterator b (o.begin_resource()), e (o.end_resource());

        if (b != e)
        {
          resource_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) resource_next (o);
          }

          resource_post (o);
        }
      }

      void TaskMap::
      resource_pre (Type&)
      {
      }

      void TaskMap::
      resource_pre (Type const&)
      {
      }

      void TaskMap::
      resource_next (Type&)
      {
      }

      void TaskMap::
      resource_next (Type const&)
      {
      }

      void TaskMap::
      resource_post (Type&)
      {
      }

      void TaskMap::
      resource_post (Type const&)
      {
      }

      void TaskMap::
      taskToImpl (Type& o)
      {
        // VC6 anathema strikes again
        //
        TaskMap::Type::taskToImpl_iterator b (o.begin_taskToImpl()), e (o.end_taskToImpl());

        if (b != e)
        {
          taskToImpl_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) taskToImpl_next (o);
          }

          taskToImpl_post (o);
        }
      }

      void TaskMap::
      taskToImpl (Type const& o)
      {
        // VC6 anathema strikes again
        //
        TaskMap::Type::taskToImpl_const_iterator b (o.begin_taskToImpl()), e (o.end_taskToImpl());

        if (b != e)
        {
          taskToImpl_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) taskToImpl_next (o);
          }

          taskToImpl_post (o);
        }
      }

      void TaskMap::
      taskToImpl_pre (Type&)
      {
      }

      void TaskMap::
      taskToImpl_pre (Type const&)
      {
      }

      void TaskMap::
      taskToImpl_next (Type&)
      {
      }

      void TaskMap::
      taskToImpl_next (Type const&)
      {
      }

      void TaskMap::
      taskToImpl_post (Type&)
      {
      }

      void TaskMap::
      taskToImpl_post (Type const&)
      {
      }

      void TaskMap::
      implToResource (Type& o)
      {
        // VC6 anathema strikes again
        //
        TaskMap::Type::implToResource_iterator b (o.begin_implToResource()), e (o.end_implToResource());

        if (b != e)
        {
          implToResource_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) implToResource_next (o);
          }

          implToResource_post (o);
        }
      }

      void TaskMap::
      implToResource (Type const& o)
      {
        // VC6 anathema strikes again
        //
        TaskMap::Type::implToResource_const_iterator b (o.begin_implToResource()), e (o.end_implToResource());

        if (b != e)
        {
          implToResource_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) implToResource_next (o);
          }

          implToResource_post (o);
        }
      }

      void TaskMap::
      implToResource_pre (Type&)
      {
      }

      void TaskMap::
      implToResource_pre (Type const&)
      {
      }

      void TaskMap::
      implToResource_next (Type&)
      {
      }

      void TaskMap::
      implToResource_next (Type const&)
      {
      }

      void TaskMap::
      implToResource_post (Type&)
      {
      }

      void TaskMap::
      implToResource_post (Type const&)
      {
      }

      void TaskMap::
      post (Type&)
      {
      }

      void TaskMap::
      post (Type const&)
      {
      }
    }
  }
}

namespace SA_POP
{
  namespace XML
  {
    namespace Writer
    {
      // TaskID
      //
      //

      TaskID::
      TaskID (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      TaskID::
      TaskID ()
      {
      }

      void TaskID::
      traverse (Type const& o)
      {
        ::XMLSchema::Writer::FundamentalType< ::XMLSchema::int_, wchar_t >::traverse (o);
        Traversal::TaskID::traverse (o);
      }

      // TaskImplID
      //
      //

      TaskImplID::
      TaskImplID (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      TaskImplID::
      TaskImplID ()
      {
      }

      void TaskImplID::
      traverse (Type const& o)
      {
        ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< wchar_t >, wchar_t >::traverse (o);
        Traversal::TaskImplID::traverse (o);
      }

      // ResourceID
      //
      //

      ResourceID::
      ResourceID (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      ResourceID::
      ResourceID ()
      {
      }

      void ResourceID::
      traverse (Type const& o)
      {
        ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< wchar_t >, wchar_t >::traverse (o);
        Traversal::ResourceID::traverse (o);
      }

      // ResourceKind
      //
      //

      ResourceKind::
      ResourceKind (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      ResourceKind::
      ResourceKind ()
      {
      }

      void ResourceKind::
      traverse (Type const& o)
      {
        ::std::basic_string< wchar_t > s;

        if (o == ::SA_POP::XML::ResourceKind::UNARY) s = L"UNARY";
        else if (o == ::SA_POP::XML::ResourceKind::DISCRETE) s = L"DISCRETE";
        else if (o == ::SA_POP::XML::ResourceKind::RESERVOIR) s = L"RESERVOIR";
        else 
        {
        }

        if (::XSCRT::XML::Attribute< wchar_t >* a = attr_ ())
        {
          a->value (s);
        }

        else
        {
          top_().value (s);
        }
      }

      // ResourceValue
      //
      //

      ResourceValue::
      ResourceValue (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      ResourceValue::
      ResourceValue ()
      {
      }

      void ResourceValue::
      traverse (Type const& o)
      {
        ::XMLSchema::Writer::FundamentalType< ::XMLSchema::int_, wchar_t >::traverse (o);
        Traversal::ResourceValue::traverse (o);
      }

      // ImplParamID
      //
      //

      ImplParamID::
      ImplParamID (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      ImplParamID::
      ImplParamID ()
      {
      }

      void ImplParamID::
      traverse (Type const& o)
      {
        ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< wchar_t >, wchar_t >::traverse (o);
        Traversal::ImplParamID::traverse (o);
      }

      // ImplParamKind
      //
      //

      ImplParamKind::
      ImplParamKind (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      ImplParamKind::
      ImplParamKind ()
      {
      }

      void ImplParamKind::
      traverse (Type const& o)
      {
        ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< wchar_t >, wchar_t >::traverse (o);
        Traversal::ImplParamKind::traverse (o);
      }

      // ImplParamValue
      //
      //

      ImplParamValue::
      ImplParamValue (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      ImplParamValue::
      ImplParamValue ()
      {
      }

      void ImplParamValue::
      traverse (Type const& o)
      {
        ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< wchar_t >, wchar_t >::traverse (o);
        Traversal::ImplParamValue::traverse (o);
      }

      // TimeValue
      //
      //

      TimeValue::
      TimeValue (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      TimeValue::
      TimeValue ()
      {
      }

      void TimeValue::
      traverse (Type const& o)
      {
        ::XMLSchema::Writer::FundamentalType< ::XMLSchema::int_, wchar_t >::traverse (o);
        Traversal::TimeValue::traverse (o);
      }

      // Resource
      //
      //

      Resource::
      Resource (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      Resource::
      Resource ()
      {
      }

      void Resource::
      traverse (Type const& o)
      {
        Traversal::Resource::traverse (o);
      }

      void Resource::
      resourceID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"resourceID", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
        Traversal::Resource::resourceID (o);
        pop_ ();
      }

      void Resource::
      kind (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"kind", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
        Traversal::Resource::kind (o);
        pop_ ();
      }

      void Resource::
      capacity (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"capacity", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
        Traversal::Resource::capacity (o);
        pop_ ();
      }

      // ImplParam
      //
      //

      ImplParam::
      ImplParam (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      ImplParam::
      ImplParam ()
      {
      }

      void ImplParam::
      traverse (Type const& o)
      {
        Traversal::ImplParam::traverse (o);
      }

      void ImplParam::
      paramID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"paramID", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
        Traversal::ImplParam::paramID (o);
        pop_ ();
      }

      void ImplParam::
      kind (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"kind", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
        Traversal::ImplParam::kind (o);
        pop_ ();
      }

      void ImplParam::
      value (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"value", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
        Traversal::ImplParam::value (o);
        pop_ ();
      }

      // TaskImpl
      //
      //

      TaskImpl::
      TaskImpl (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      TaskImpl::
      TaskImpl ()
      {
      }

      void TaskImpl::
      traverse (Type const& o)
      {
        Traversal::TaskImpl::traverse (o);
      }

      void TaskImpl::
      implID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"implID", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
        Traversal::TaskImpl::implID (o);
        pop_ ();
      }

      void TaskImpl::
      param_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"param", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
      }

      void TaskImpl::
      param_next (Type const& o)
      {
        param_post (o);
        param_pre (o);
      }

      void TaskImpl::
      param_post (Type const&)
      {
        pop_ ();
      }

      // TaskToImpl
      //
      //

      TaskToImpl::
      TaskToImpl (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      TaskToImpl::
      TaskToImpl ()
      {
      }

      void TaskToImpl::
      traverse (Type const& o)
      {
        Traversal::TaskToImpl::traverse (o);
      }

      void TaskToImpl::
      taskID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"taskID", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
        Traversal::TaskToImpl::taskID (o);
        pop_ ();
      }

      void TaskToImpl::
      implID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"implID", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
        Traversal::TaskToImpl::implID (o);
        pop_ ();
      }

      void TaskToImpl::
      duration (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"duration", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
        Traversal::TaskToImpl::duration (o);
        pop_ ();
      }

      // ImplToResource
      //
      //

      ImplToResource::
      ImplToResource (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      ImplToResource::
      ImplToResource ()
      {
      }

      void ImplToResource::
      traverse (Type const& o)
      {
        Traversal::ImplToResource::traverse (o);
      }

      void ImplToResource::
      implID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"implID", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
        Traversal::ImplToResource::implID (o);
        pop_ ();
      }

      void ImplToResource::
      resourceID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"resourceID", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
        Traversal::ImplToResource::resourceID (o);
        pop_ ();
      }

      void ImplToResource::
      utilization (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"utilization", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
        Traversal::ImplToResource::utilization (o);
        pop_ ();
      }

      // TaskMap
      //
      //

      TaskMap::
      TaskMap (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      TaskMap::
      TaskMap ()
      {
      }

      void TaskMap::
      traverse (Type const& o)
      {
        Traversal::TaskMap::traverse (o);
      }

      void TaskMap::
      taskImpl_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"taskImpl", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
      }

      void TaskMap::
      taskImpl_next (Type const& o)
      {
        taskImpl_post (o);
        taskImpl_pre (o);
      }

      void TaskMap::
      taskImpl_post (Type const&)
      {
        pop_ ();
      }

      void TaskMap::
      resource_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"resource", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
      }

      void TaskMap::
      resource_next (Type const& o)
      {
        resource_post (o);
        resource_pre (o);
      }

      void TaskMap::
      resource_post (Type const&)
      {
        pop_ ();
      }

      void TaskMap::
      taskToImpl_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"taskToImpl", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
      }

      void TaskMap::
      taskToImpl_next (Type const& o)
      {
        taskToImpl_post (o);
        taskToImpl_pre (o);
      }

      void TaskMap::
      taskToImpl_post (Type const&)
      {
        pop_ ();
      }

      void TaskMap::
      implToResource_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"implToResource", L"http://www.vanderbilt.edu/SA-POP", top_ ()));
      }

      void TaskMap::
      implToResource_next (Type const& o)
      {
        implToResource_post (o);
        implToResource_pre (o);
      }

      void TaskMap::
      implToResource_post (Type const&)
      {
        pop_ ();
      }
    }
  }
}

namespace SA_POP
{
  namespace XML
  {
    void
    taskMap (::SA_POP::XML::TaskMap const& s, xercesc::DOMDocument* d)
    {
      ::XSCRT::XML::Element< wchar_t > e (d->getDocumentElement ());
      if (e.name () != L"taskMap")
      {
        throw 1;
      }

      struct W : virtual ::SA_POP::XML::Writer::TaskMap,
      virtual ::SA_POP::XML::Writer::TaskImpl,
      virtual ::SA_POP::XML::Writer::TaskImplID,
      virtual ::SA_POP::XML::Writer::ImplParam,
      virtual ::SA_POP::XML::Writer::ImplParamID,
      virtual ::SA_POP::XML::Writer::ImplParamKind,
      virtual ::SA_POP::XML::Writer::ImplParamValue,
      virtual ::SA_POP::XML::Writer::Resource,
      virtual ::SA_POP::XML::Writer::ResourceID,
      virtual ::SA_POP::XML::Writer::ResourceKind,
      virtual ::SA_POP::XML::Writer::ResourceValue,
      virtual ::SA_POP::XML::Writer::TaskToImpl,
      virtual ::SA_POP::XML::Writer::TaskID,
      virtual ::SA_POP::XML::Writer::TimeValue,
      virtual ::SA_POP::XML::Writer::ImplToResource,
      virtual ::XSCRT::Writer< wchar_t >
      {
        W (::XSCRT::XML::Element< wchar_t >& e)
        : ::XSCRT::Writer< wchar_t > (e)
        {
        }
      };

      W w (e);
      w.dispatch (s);
    }
  }
}

