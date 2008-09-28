#include "XML_TaskMap.hpp"

namespace SA_POP
{
  namespace XML
  {
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

      // Resource
      // 

      Resource::
      Resource (::XMLSchema::string< wchar_t > const& resourceID__,
                ::SA_POP::XML::ResourceKind const& kind__,
                ::XMLSchema::int_ const& capacity__)
      : 
      ::XSCRT::Type (), 
      resourceID_ (new ::XMLSchema::string< wchar_t > (resourceID__)),
      kind_ (new ::SA_POP::XML::ResourceKind (kind__)),
      capacity_ (new ::XMLSchema::int_ (capacity__)),
      regulator__ ()
      {
        resourceID_->container (this);
        kind_->container (this);
        capacity_->container (this);
      }

      Resource::
      Resource (Resource const& s)
      :
      ::XSCRT::Type (),
      resourceID_ (new ::XMLSchema::string< wchar_t > (*s.resourceID_)),
      kind_ (new ::SA_POP::XML::ResourceKind (*s.kind_)),
      capacity_ (new ::XMLSchema::int_ (*s.capacity_)),
      regulator__ ()
      {
        resourceID_->container (this);
        kind_->container (this);
        capacity_->container (this);
      }

      Resource& Resource::
      operator= (Resource const& s)
      {
        resourceID (*s.resourceID_);

        kind (*s.kind_);

        capacity (*s.capacity_);

        return *this;
      }


      // Resource
      // 
      ::XMLSchema::string< wchar_t > const& Resource::
      resourceID () const
      {
        return *resourceID_;
      }

      void Resource::
      resourceID (::XMLSchema::string< wchar_t > const& e)
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
      ::XMLSchema::int_ const& Resource::
      capacity () const
      {
        return *capacity_;
      }

      void Resource::
      capacity (::XMLSchema::int_ const& e)
      {
        *capacity_ = e;
      }


      // ImplParam
      // 

      ImplParam::
      ImplParam (::XMLSchema::string< wchar_t > const& paramID__,
                 ::XMLSchema::string< wchar_t > const& kind__,
                 ::XMLSchema::string< wchar_t > const& value__)
      : 
      paramID_ (new ::XMLSchema::string< wchar_t > (paramID__)),
      kind_ (new ::XMLSchema::string< wchar_t > (kind__)),
      value_ (new ::XMLSchema::string< wchar_t > (value__)),
      regulator__ ()
      {
        paramID_->container (this);
        kind_->container (this);
        value_->container (this);
      }

      ImplParam::
      ImplParam (ImplParam const& s)
      :
      ::XSCRT::Type (),
      paramID_ (new ::XMLSchema::string< wchar_t > (*s.paramID_)),
      kind_ (new ::XMLSchema::string< wchar_t > (*s.kind_)),
      value_ (new ::XMLSchema::string< wchar_t > (*s.value_)),
      regulator__ ()
      {
        paramID_->container (this);
        kind_->container (this);
        value_->container (this);
      }

      ImplParam& ImplParam::
      operator= (ImplParam const& s)
      {
        paramID (*s.paramID_);

        kind (*s.kind_);

        value (*s.value_);

        return *this;
      }


      // ImplParam
      // 
      ::XMLSchema::string< wchar_t > const& ImplParam::
      paramID () const
      {
        return *paramID_;
      }

      void ImplParam::
      paramID (::XMLSchema::string< wchar_t > const& e)
      {
        *paramID_ = e;
      }

      // ImplParam
      // 
      ::XMLSchema::string< wchar_t > const& ImplParam::
      kind () const
      {
        return *kind_;
      }

      void ImplParam::
      kind (::XMLSchema::string< wchar_t > const& e)
      {
        *kind_ = e;
      }

      // ImplParam
      // 
      ::XMLSchema::string< wchar_t > const& ImplParam::
      value () const
      {
        return *value_;
      }

      void ImplParam::
      value (::XMLSchema::string< wchar_t > const& e)
      {
        *value_ = e;
      }


      // TaskImpl
      // 

      TaskImpl::
      TaskImpl (::XMLSchema::string< wchar_t > const& implID__,
                ::std::list< ::SA_POP::XML::ImplParam > const& param__)
      : 
      implID_ (new ::XMLSchema::string< wchar_t > (implID__)),
      param_ (param__),
      regulator__ ()
      {
        implID_->container (this);
      }

      TaskImpl::
      TaskImpl (TaskImpl const& s)
      :
      ::XSCRT::Type (),
      implID_ (new ::XMLSchema::string< wchar_t > (*s.implID_)),
      param_ (s.param_),
      regulator__ ()
      {
        implID_->container (this);
      }

      TaskImpl& TaskImpl::
      operator= (TaskImpl const& s)
      {
        implID (*s.implID_);

        param_ = s.param_;

        return *this;
      }


      // TaskImpl
      // 
      ::XMLSchema::string< wchar_t > const& TaskImpl::
      implID () const
      {
        return *implID_;
      }

      void TaskImpl::
      implID (::XMLSchema::string< wchar_t > const& e)
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
      TaskToImpl (::XMLSchema::int_ const& taskID__,
                  ::XMLSchema::string< wchar_t > const& implID__,
                  ::XMLSchema::int_ const& duration__)
      : 
      taskID_ (new ::XMLSchema::int_ (taskID__)),
      implID_ (new ::XMLSchema::string< wchar_t > (implID__)),
      duration_ (new ::XMLSchema::int_ (duration__)),
      regulator__ ()
      {
        taskID_->container (this);
        implID_->container (this);
        duration_->container (this);
      }

      TaskToImpl::
      TaskToImpl (TaskToImpl const& s)
      :
      ::XSCRT::Type (),
      taskID_ (new ::XMLSchema::int_ (*s.taskID_)),
      implID_ (new ::XMLSchema::string< wchar_t > (*s.implID_)),
      duration_ (new ::XMLSchema::int_ (*s.duration_)),
      regulator__ ()
      {
        taskID_->container (this);
        implID_->container (this);
        duration_->container (this);
      }

      TaskToImpl& TaskToImpl::
      operator= (TaskToImpl const& s)
      {
        taskID (*s.taskID_);

        implID (*s.implID_);

        duration (*s.duration_);

        return *this;
      }


      // TaskToImpl
      // 
      ::XMLSchema::int_ const& TaskToImpl::
      taskID () const
      {
        return *taskID_;
      }

      void TaskToImpl::
      taskID (::XMLSchema::int_ const& e)
      {
        *taskID_ = e;
      }

      // TaskToImpl
      // 
      ::XMLSchema::string< wchar_t > const& TaskToImpl::
      implID () const
      {
        return *implID_;
      }

      void TaskToImpl::
      implID (::XMLSchema::string< wchar_t > const& e)
      {
        *implID_ = e;
      }

      // TaskToImpl
      // 
      ::XMLSchema::int_ const& TaskToImpl::
      duration () const
      {
        return *duration_;
      }

      void TaskToImpl::
      duration (::XMLSchema::int_ const& e)
      {
        *duration_ = e;
      }


      // ImplToResource
      // 

      ImplToResource::
      ImplToResource (::XMLSchema::string< wchar_t > const& implID__,
                      ::XMLSchema::string< wchar_t > const& resourceID__,
                      ::XMLSchema::int_ const& utilization__)
      : 
      implID_ (new ::XMLSchema::string< wchar_t > (implID__)),
      resourceID_ (new ::XMLSchema::string< wchar_t > (resourceID__)),
      utilization_ (new ::XMLSchema::int_ (utilization__)),
      regulator__ ()
      {
        implID_->container (this);
        resourceID_->container (this);
        utilization_->container (this);
      }

      ImplToResource::
      ImplToResource (ImplToResource const& s)
      :
      ::XSCRT::Type (),
      implID_ (new ::XMLSchema::string< wchar_t > (*s.implID_)),
      resourceID_ (new ::XMLSchema::string< wchar_t > (*s.resourceID_)),
      utilization_ (new ::XMLSchema::int_ (*s.utilization_)),
      regulator__ ()
      {
        implID_->container (this);
        resourceID_->container (this);
        utilization_->container (this);
      }

      ImplToResource& ImplToResource::
      operator= (ImplToResource const& s)
      {
        implID (*s.implID_);

        resourceID (*s.resourceID_);

        utilization (*s.utilization_);

        return *this;
      }


      // ImplToResource
      // 
      ::XMLSchema::string< wchar_t > const& ImplToResource::
      implID () const
      {
        return *implID_;
      }

      void ImplToResource::
      implID (::XMLSchema::string< wchar_t > const& e)
      {
        *implID_ = e;
      }

      // ImplToResource
      // 
      ::XMLSchema::string< wchar_t > const& ImplToResource::
      resourceID () const
      {
        return *resourceID_;
      }

      void ImplToResource::
      resourceID (::XMLSchema::string< wchar_t > const& e)
      {
        *resourceID_ = e;
      }

      // ImplToResource
      // 
      ::XMLSchema::int_ const& ImplToResource::
      utilization () const
      {
        return *utilization_;
      }

      void ImplToResource::
      utilization (::XMLSchema::int_ const& e)
      {
        *utilization_ = e;
      }


      // TaskMap
      // 

      TaskMap::
      TaskMap (::std::list< ::SA_POP::XML::TaskImpl > const& taskImpl__,
               ::std::list< ::SA_POP::XML::Resource > const& resource__,
               ::std::list< ::SA_POP::XML::TaskToImpl > const& taskToImpl__,
               ::std::list< ::SA_POP::XML::ImplToResource > const& implToResource__)
      : 
      taskImpl_ (taskImpl__),
      resource_ (resource__),
      taskToImpl_ (taskToImpl__),
      implToResource_ (implToResource__),
      regulator__ ()
      {
      }

      TaskMap::
      TaskMap (TaskMap const& s)
      :
      ::XSCRT::Type (),
      taskImpl_ (s.taskImpl_),
      resource_ (s.resource_),
      taskToImpl_ (s.taskToImpl_),
      implToResource_ (s.implToResource_),
      regulator__ ()
      {
      }

      TaskMap& TaskMap::
      operator= (TaskMap const& s)
      {
        taskImpl_ = s.taskImpl_;

        resource_ = s.resource_;

        taskToImpl_ = s.taskToImpl_;

        implToResource_ = s.implToResource_;

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

      // Resource
      //

      Resource::
      Resource (::XSCRT::XML::Element< wchar_t > const& e)
      :Base (e), regulator__ ()
      {

        ::XSCRT::Parser< wchar_t > p (e);

        while (p.more_elements ())
        {
          ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
          ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

          if (n == L"resourceID")
          {
            resourceID_ = ::std::auto_ptr< ::XMLSchema::string< wchar_t > > (new ::XMLSchema::string< wchar_t > (e));
            resourceID_->container (this);
          }

          else if (n == L"kind")
          {
            kind_ = ::std::auto_ptr< ::SA_POP::XML::ResourceKind > (new ::SA_POP::XML::ResourceKind (e));
            kind_->container (this);
          }

          else if (n == L"capacity")
          {
            capacity_ = ::std::auto_ptr< ::XMLSchema::int_ > (new ::XMLSchema::int_ (e));
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
      :Base (e), regulator__ ()
      {

        ::XSCRT::Parser< wchar_t > p (e);

        while (p.more_elements ())
        {
          ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
          ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

          if (n == L"paramID")
          {
            paramID_ = ::std::auto_ptr< ::XMLSchema::string< wchar_t > > (new ::XMLSchema::string< wchar_t > (e));
            paramID_->container (this);
          }

          else if (n == L"kind")
          {
            kind_ = ::std::auto_ptr< ::XMLSchema::string< wchar_t > > (new ::XMLSchema::string< wchar_t > (e));
            kind_->container (this);
          }

          else if (n == L"value")
          {
            value_ = ::std::auto_ptr< ::XMLSchema::string< wchar_t > > (new ::XMLSchema::string< wchar_t > (e));
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
      :Base (e), regulator__ ()
      {

        ::XSCRT::Parser< wchar_t > p (e);

        while (p.more_elements ())
        {
          ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
          ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

          if (n == L"implID")
          {
            implID_ = ::std::auto_ptr< ::XMLSchema::string< wchar_t > > (new ::XMLSchema::string< wchar_t > (e));
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
      :Base (e), regulator__ ()
      {

        ::XSCRT::Parser< wchar_t > p (e);

        while (p.more_elements ())
        {
          ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
          ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

          if (n == L"taskID")
          {
            taskID_ = ::std::auto_ptr< ::XMLSchema::int_ > (new ::XMLSchema::int_ (e));
            taskID_->container (this);
          }

          else if (n == L"implID")
          {
            implID_ = ::std::auto_ptr< ::XMLSchema::string< wchar_t > > (new ::XMLSchema::string< wchar_t > (e));
            implID_->container (this);
          }

          else if (n == L"duration")
          {
            duration_ = ::std::auto_ptr< ::XMLSchema::int_ > (new ::XMLSchema::int_ (e));
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
      :Base (e), regulator__ ()
      {

        ::XSCRT::Parser< wchar_t > p (e);

        while (p.more_elements ())
        {
          ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
          ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

          if (n == L"implID")
          {
            implID_ = ::std::auto_ptr< ::XMLSchema::string< wchar_t > > (new ::XMLSchema::string< wchar_t > (e));
            implID_->container (this);
          }

          else if (n == L"resourceID")
          {
            resourceID_ = ::std::auto_ptr< ::XMLSchema::string< wchar_t > > (new ::XMLSchema::string< wchar_t > (e));
            resourceID_->container (this);
          }

          else if (n == L"utilization")
          {
            utilization_ = ::std::auto_ptr< ::XMLSchema::int_ > (new ::XMLSchema::int_ (e));
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
      :Base (e), regulator__ ()
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
      namespace reader
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
}

