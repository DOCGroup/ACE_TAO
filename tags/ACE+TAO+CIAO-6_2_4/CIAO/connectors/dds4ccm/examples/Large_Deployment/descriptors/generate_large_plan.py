#!/usr/bin/python
#$Id$
import sys, os, math

sys.path.append (os.getenv ("DANCE_ROOT") + "/bin")

from PythonDAnCE import generator

nodes = 10
processes = 200
proc_per_node = processes / 10
single_service_components = 1000
dds_pub_components = 250
dds_pub_per_proc = 2
dds_sub_components = 375
dds_sub_per_proc = 2
topics = 250

namespace = ""


def create_names (prefix, number):
    retval = []
    
    for x in range(number):
        retval.append (prefix + "_" + str(x))
    return retval

def add_sub_connector (name, topic, node) :
    comp = ""
    conn = ""
    
    config_prop = ""
    config_prop += generator.config_prop ("topic_name", "string", topic)
    
    comp += generator.comp_inst (name + "DDSConnector", config_prop, )
    
    deploy_req = generator.deploy_requirement ("edu.dre.vanderbilt.DAnCE.ConnectionType",
                                               "Local_Interface")
    conn += generator.connection (name + "info_out_data",
                                  deploy_req,
                                  generator.internal_endpoint ("info_out_data",
                                                               "false",
                                                               "SimplexReceptacle",
                                                               name + "ComponentInstance"),
                                  generator.internal_endpoint ("push_consumer_data",
                                                               "true",
                                                               "Facet",
                                                               name + "DDSConnector" + "ComponentInstance"))
    
    conn += generator.connection (name + "listenercontrol",
                                  deploy_req,
                                  generator.internal_endpoint ("info_out_data_control",
                                                               "false",
                                                               "SimplexReceptacle",
                                                               name + "ComponentInstance"),
                                  generator.internal_endpoint ("push_consumer_data_control",
                                                               "true",
                                                               "Facet",
                                                               name + "DDSConnector" + "ComponentInstance"))

    conn += generator.connection (name + "portstatuslistener",
                                  deploy_req,
                                  generator.internal_endpoint ("info_out_status",
                                                               "false",
                                                               "SimplexReceptacle",
                                                               name + "ComponentInstance"),
                                  generator.internal_endpoint ("push_consumer_status",
                                                               "true",
                                                               "Facet",
                                                               name + "DDSConnector" + "ComponentInstance"))

    conn += generator.connection (name + "listener",
                                  deploy_req,
                                  generator.internal_endpoint ("info_out_data_listener",
                                                               "false",
                                                               "SimplexReceptacle",
                                                               name + "ComponentInstance"),
                                  generator.internal_endpoint ("push_consumer_data_listener",
                                                               "true",
                                                               "Facet",
                                                               name + "DDSConnector" + "ComponentInstance"))
    return (comp, conn)

def add_pub_connector (name, topic, node) :
    comp = ""
    conn = ""
    
    config_prop = ""
    config_prop += generator.config_prop.template ("topic_name", "string", topic)

    comp += generator.comp_inst.template (name + "DDSConnector", config_prop, "Large_ConnectorComponentImplementation", node)
    
    deploy_req = generator.deploy_requirement.template ("edu.dre.vanderbilt.DAnCE.ConnectionType",
                                                        "Local_Interface")

    conn += generator.connection.template (name + "writer",
                                           deploy_req,
                                           generator.internal_endpoint.template ("info_data",
                                                                                 "false",
                                                                                 "SimplexReceptacle",
                                                                                 name + "ComponentInstance"),
                                           generator.internal_endpoint.template ("supplier_data",
                                                                                 "true",
                                                                                 "Facet",
                                                                                 name + "DDSConnector" + "ComponentInstance"))
    
    return (comp, conn)

def add_provide_component (name, node) :
    retval = ""
    
    retval += generator.comp_inst.template (name, "", "Single_ServiceComponentImplementation", node)
    
    return retval

def add_dds_pub_component (name, topic, node) :
    comps = ""
    conns = ""
    
    comps += generator.comp_inst.template (name, "", "DDS_PubComponentImplementation", node)

    compname = name + "_prov1"
    comps += add_provide_component (compname, node)
    conns += generator.connection.template (name + "service1",
                                   "",
                                   generator.internal_endpoint.template ("ping_port",
                                                                "true",
                                                                "Facet",
                                                                compname+ "ComponentInstance"),
                                   generator.internal_endpoint.template ("ping_one",
                                                                "false",
                                                                "SimplexReceptacle",
                                                                name + "ComponentInstance"))
    compname = name + "_prov2"
    comps += add_provide_component (compname, node)
    conns += generator.connection.template (name + "service2",
                                   "",
                                   generator.internal_endpoint.template ("ping_port",
                                                                "true",
                                                                "Facet",
                                                                compname+ "ComponentInstance"),
                                   generator.internal_endpoint.template ("ping_two",
                                                                "false",
                                                                "SimplexReceptacle",
                                                                name + "ComponentInstance" ))
    
                                                                
    (tmpcomp, tmpconn) = add_pub_connector (name, topic, node)
    
    comps += tmpcomp
    conns += tmpconn

    return (comps, conns)

def add_dds_sub_component (name, topic, node) :
    comps = ""
    conns = ""
    
    comps += generator.comp_inst (name, "", "DDS_PubComponentImplementation", node)

    compname = name + "_prov"
    comps += add_provide_component (compname, node)
    conns += generator.connection (name + "service",
                                   "",
                                   generator.internal_endpoint ("ping_port",
                                                                "true",
                                                                "Facet",
                                                                compname + "ComponentInstance"),
                                   generator.internal_endpoint ("ping_one",
                                                                "false",
                                                                "SimplexReceptacle",
                                                                name + "ComponentInstance"))    
    
    (tmpcomp, tmpconn) = add_sub_connector (name, topic, node)
    
    comps += tmpcomp
    conns += tmpconn
    
    return (comps, conns)

def main ():
    
    import random

    retval = ""
    
    retval += generator.header.template ("Large_Plan")
    
    retval += generator.comp_impl.template ("Large_Connector", "")
    retval += generator.comp_impl.template ("Single_Service", "")
    retval += generator.comp_impl.template ("DDS_Pub", "")
    retval += generator.comp_impl.template ("DDS_Sub", "")
    
    conns = ""
    comps = ""
    
    nodenames = create_names ("Node", nodes)
    print nodenames
    topicnames = create_names ("Topic", topics)
    print topicnames

    pub_count = 0
    sub_count = 0
    service_count = 0

    for name in nodenames:
        for proc in range (proc_per_node):
            for comp in range (dds_pub_per_proc):
                (tmpcomp, tmpconn) = add_dds_pub_component ('Pub' + '_' + name + '_' + str(proc) + '_' + str(comp) + '_',
                                                            random.choice (topicnames),
                                                            name)
                conns += tmpconn
                comps += tmpcomp
                pub_count += 1
                service_count += 2

            for comp in range (dds_sub_per_proc):
                (tmpcomp, tmpconn) = add_dds_pub_component ('Sub' + '_' + name + '_' + str(proc) + '_' + str(comp) + '_',
                                                            random.choice (topicnames),
                                                            name)
                
                conns += tmpconn
                comps += tmpcomp
                
                sub_count += 1
                service_count += 1
    
    retval += comps
    retval += conns
    
    retval += generator.artifact.template ("Large_Connector")
    retval += generator.artifact.template ("Single_Service")
    retval += generator.artifact.template ("DDS_Pub")
    retval += generator.artifact.template ("DDS_Sub")
    retval += generator.footer.template ()
    
    print "Writing a plan with " + str(pub_count) + " publish components, " + str (sub_count) + " subscribe components, and " + str(service_count) + " service components."

    outfile = open ("LargePlan.cdp", 'w')
    outfile.write (retval)
    outfile.close ()
    
if __name__ == "__main__":
    main ()

            
                
