#ifndef TASK_MAP_HPP
#define TASK_MAP_HPP


// Fix for Borland compilers, which seem to have a broken
// <string> include.
#ifdef __BORLANDC__
# include <string.h>
#endif

// Forward declarations.
//
namespace  SA_POP
{
  namespace XML
  {
      class ResourceKind;
      class Resource;
      class ImplParam;
      class TaskImpl;
      class TaskToImpl;
      class ImplToResource;
      class TaskMap;
  }
  
}

#include <memory>
#include <list>
#include "XMLSchema/Types.hpp"

namespace  SA_POP
{
  namespace XML
  {
      class ResourceKind : public ::XSCRT::Type
      {
        public:
        ResourceKind (::XSCRT::XML::Element< wchar_t > const&);
        ResourceKind (::XSCRT::XML::Attribute< wchar_t > const&);

        static ResourceKind const UNARY;
        static ResourceKind const DISCRETE;
        static ResourceKind const RESERVOIR;

        enum Value
        {
          UNARY_l, DISCRETE_l, RESERVOIR_l
        };


        Value
        integral () const;

        friend bool 
        operator== (ResourceKind const& a, ResourceKind const& b);

        friend bool 
        operator!= (ResourceKind const& a, ResourceKind const& b);

        private:
        ResourceKind (Value v);

        Value v_;
      };

      bool operator== (ResourceKind const &a, ResourceKind const &b);

      bool operator!= (ResourceKind const &a, ResourceKind const &b);


      class Resource : public ::XSCRT::Type
      {
        typedef ::XSCRT::Type Base;

        // resourceID
        // 
        public:
        ::XMLSchema::string< wchar_t > const& resourceID () const;
        void resourceID (::XMLSchema::string< wchar_t > const& );

        protected:
        ::std::auto_ptr< ::XMLSchema::string< wchar_t > > resourceID_;

        // kind
        // 
        public:
        :: SA_POP::XML::ResourceKind const& kind () const;
        void kind (:: SA_POP::XML::ResourceKind const& );

        protected:
        ::std::auto_ptr< :: SA_POP::XML::ResourceKind > kind_;

        // capacity
        // 
        public:
        ::XMLSchema::int_ const& capacity () const;
        void capacity (::XMLSchema::int_ const& );

        protected:
        ::std::auto_ptr< ::XMLSchema::int_ > capacity_;

        public:
        Resource (::XMLSchema::string< wchar_t > const& resourceID__,
                  :: SA_POP::XML::ResourceKind const& kind__,
                  ::XMLSchema::int_ const& capacity__);

        Resource (::XSCRT::XML::Element< wchar_t > const&);
        Resource (Resource const& s);

        Resource&
        operator= (Resource const& s);

        private:
        char regulator__;
      };


      class ImplParam : public ::XSCRT::Type
      {
        typedef ::XSCRT::Type Base;

        // paramID
        // 
        public:
        ::XMLSchema::string< wchar_t > const& paramID () const;
        void paramID (::XMLSchema::string< wchar_t > const& );

        protected:
        ::std::auto_ptr< ::XMLSchema::string< wchar_t > > paramID_;

        // kind
        // 
        public:
        ::XMLSchema::string< wchar_t > const& kind () const;
        void kind (::XMLSchema::string< wchar_t > const& );

        protected:
        ::std::auto_ptr< ::XMLSchema::string< wchar_t > > kind_;

        // value
        // 
        public:
        ::XMLSchema::string< wchar_t > const& value () const;
        void value (::XMLSchema::string< wchar_t > const& );

        protected:
        ::std::auto_ptr< ::XMLSchema::string< wchar_t > > value_;

        public:
        ImplParam (::XMLSchema::string< wchar_t > const& paramID__,
                   ::XMLSchema::string< wchar_t > const& kind__,
                   ::XMLSchema::string< wchar_t > const& value__);

        ImplParam (::XSCRT::XML::Element< wchar_t > const&);
        ImplParam (ImplParam const& s);

        ImplParam&
        operator= (ImplParam const& s);

        private:
        char regulator__;
      };


      class TaskImpl : public ::XSCRT::Type
      {
        typedef ::XSCRT::Type Base;

        // implID
        // 
        public:
        ::XMLSchema::string< wchar_t > const& implID () const;
        void implID (::XMLSchema::string< wchar_t > const& );

        protected:
        ::std::auto_ptr< ::XMLSchema::string< wchar_t > > implID_;

        // param
        // 
        public:
        typedef ::std::list< :: SA_POP::XML::ImplParam >::iterator param_iterator;
        typedef ::std::list< :: SA_POP::XML::ImplParam >::const_iterator param_const_iterator;
        param_iterator begin_param ();
        param_iterator end_param ();
        param_const_iterator begin_param () const;
        param_const_iterator end_param () const;
        void add_param (:: SA_POP::XML::ImplParam const& );
        size_t count_param (void) const;

        protected:
        ::std::list< :: SA_POP::XML::ImplParam > param_;

        public:
        TaskImpl (::XMLSchema::string< wchar_t > const& implID__,
                  ::std::list< :: SA_POP::XML::ImplParam > const& param__);

        TaskImpl (::XSCRT::XML::Element< wchar_t > const&);
        TaskImpl (TaskImpl const& s);

        TaskImpl&
        operator= (TaskImpl const& s);

        private:
        char regulator__;
      };


      class TaskToImpl : public ::XSCRT::Type
      {
        typedef ::XSCRT::Type Base;

        // taskID
        // 
        public:
        ::XMLSchema::int_ const& taskID () const;
        void taskID (::XMLSchema::int_ const& );

        protected:
        ::std::auto_ptr< ::XMLSchema::int_ > taskID_;

        // implID
        // 
        public:
        ::XMLSchema::string< wchar_t > const& implID () const;
        void implID (::XMLSchema::string< wchar_t > const& );

        protected:
        ::std::auto_ptr< ::XMLSchema::string< wchar_t > > implID_;

        // duration
        // 
        public:
        ::XMLSchema::int_ const& duration () const;
        void duration (::XMLSchema::int_ const& );

        protected:
        ::std::auto_ptr< ::XMLSchema::int_ > duration_;

        public:
        TaskToImpl (::XMLSchema::int_ const& taskID__,
                    ::XMLSchema::string< wchar_t > const& implID__,
                    ::XMLSchema::int_ const& duration__);

        TaskToImpl (::XSCRT::XML::Element< wchar_t > const&);
        TaskToImpl (TaskToImpl const& s);

        TaskToImpl&
        operator= (TaskToImpl const& s);

        private:
        char regulator__;
      };


      class ImplToResource : public ::XSCRT::Type
      {
        typedef ::XSCRT::Type Base;

        // implID
        // 
        public:
        ::XMLSchema::string< wchar_t > const& implID () const;
        void implID (::XMLSchema::string< wchar_t > const& );

        protected:
        ::std::auto_ptr< ::XMLSchema::string< wchar_t > > implID_;

        // resourceID
        // 
        public:
        ::XMLSchema::string< wchar_t > const& resourceID () const;
        void resourceID (::XMLSchema::string< wchar_t > const& );

        protected:
        ::std::auto_ptr< ::XMLSchema::string< wchar_t > > resourceID_;

        // utilization
        // 
        public:
        ::XMLSchema::int_ const& utilization () const;
        void utilization (::XMLSchema::int_ const& );

        protected:
        ::std::auto_ptr< ::XMLSchema::int_ > utilization_;

        public:
        ImplToResource (::XMLSchema::string< wchar_t > const& implID__,
                        ::XMLSchema::string< wchar_t > const& resourceID__,
                        ::XMLSchema::int_ const& utilization__);

        ImplToResource (::XSCRT::XML::Element< wchar_t > const&);
        ImplToResource (ImplToResource const& s);

        ImplToResource&
        operator= (ImplToResource const& s);

        private:
        char regulator__;
      };


      class TaskMap : public ::XSCRT::Type
      {
        typedef ::XSCRT::Type Base;

        // taskImpl
        // 
        public:
        typedef ::std::list< :: SA_POP::XML::TaskImpl >::iterator taskImpl_iterator;
        typedef ::std::list< :: SA_POP::XML::TaskImpl >::const_iterator taskImpl_const_iterator;
        taskImpl_iterator begin_taskImpl ();
        taskImpl_iterator end_taskImpl ();
        taskImpl_const_iterator begin_taskImpl () const;
        taskImpl_const_iterator end_taskImpl () const;
        void add_taskImpl (:: SA_POP::XML::TaskImpl const& );
        size_t count_taskImpl (void) const;

        protected:
        ::std::list< :: SA_POP::XML::TaskImpl > taskImpl_;

        // resource
        // 
        public:
        typedef ::std::list< :: SA_POP::XML::Resource >::iterator resource_iterator;
        typedef ::std::list< :: SA_POP::XML::Resource >::const_iterator resource_const_iterator;
        resource_iterator begin_resource ();
        resource_iterator end_resource ();
        resource_const_iterator begin_resource () const;
        resource_const_iterator end_resource () const;
        void add_resource (:: SA_POP::XML::Resource const& );
        size_t count_resource (void) const;

        protected:
        ::std::list< :: SA_POP::XML::Resource > resource_;

        // taskToImpl
        // 
        public:
        typedef ::std::list< :: SA_POP::XML::TaskToImpl >::iterator taskToImpl_iterator;
        typedef ::std::list< :: SA_POP::XML::TaskToImpl >::const_iterator taskToImpl_const_iterator;
        taskToImpl_iterator begin_taskToImpl ();
        taskToImpl_iterator end_taskToImpl ();
        taskToImpl_const_iterator begin_taskToImpl () const;
        taskToImpl_const_iterator end_taskToImpl () const;
        void add_taskToImpl (:: SA_POP::XML::TaskToImpl const& );
        size_t count_taskToImpl (void) const;

        protected:
        ::std::list< :: SA_POP::XML::TaskToImpl > taskToImpl_;

        // implToResource
        // 
        public:
        typedef ::std::list< :: SA_POP::XML::ImplToResource >::iterator implToResource_iterator;
        typedef ::std::list< :: SA_POP::XML::ImplToResource >::const_iterator implToResource_const_iterator;
        implToResource_iterator begin_implToResource ();
        implToResource_iterator end_implToResource ();
        implToResource_const_iterator begin_implToResource () const;
        implToResource_const_iterator end_implToResource () const;
        void add_implToResource (:: SA_POP::XML::ImplToResource const& );
        size_t count_implToResource (void) const;

        protected:
        ::std::list< :: SA_POP::XML::ImplToResource > implToResource_;

        public:
        TaskMap (::std::list< :: SA_POP::XML::TaskImpl > const& taskImpl__,
                 ::std::list< :: SA_POP::XML::Resource > const& resource__,
                 ::std::list< :: SA_POP::XML::TaskToImpl > const& taskToImpl__,
                 ::std::list< :: SA_POP::XML::ImplToResource > const& implToResource__);

        TaskMap (::XSCRT::XML::Element< wchar_t > const&);
        TaskMap (TaskMap const& s);

        TaskMap&
        operator= (TaskMap const& s);

        private:
        char regulator__;
      };
  }
}

namespace  SA_POP
{
  namespace XML
  {
      namespace reader
      {
        ::SA_POP::XML::TaskMap
        taskMap (xercesc::DOMDocument const*);
      }
  }
}

#endif // TASK_MAP_HPP
