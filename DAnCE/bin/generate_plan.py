#!/usr/bin/python
import sys, os

sys.path.append (os.getenv ("DANCE_ROOT"))


def parse_args ():
    from optparse import OptionParser

    parser = OptionParser ("""usage %prog [options]
    Note that this script is very simple.  It assumes that the idl and mpc follow
    canonical and predictable patterns.""")

    parser.add_option ("-o", "--output", dest="output", action="store", type="string",
                       help="Output filename", default="Plan.cdp")
    parser.add_option ("--home", dest="homes", action="append", type="string",
                       help="Home types to be included")
    parser.add_option ("-c", "--component", dest="components", action="append",
                       type="string", help="Component types to be included")
    parser.add_option ("--homed-component", dest="homed_components", action="append",
                       type="string", help="Homed component types to be included")
    parser.add_option ("-u", "--uuid", dest="uuid", action="store",
                       type="string", help="UUID for the generated plan")
    parser.add_option ("-n", "--namespace", dest="namespace", action="store", default="",
                       type="string", help="IDL namespace any components may be in")
    
    (options, arguments) = parser.parse_args ()

    return (options, arguments)

def main ():
    (opts, args) = parse_args ()

    # compset = set (opts.homes) | set (opts.components) | set (homed_components)

    from PythonDAnCE import generator
    retval = ""
    retval += generator.header.template (opts.uuid)

    artifacts = {}
    
    if opts.namespace != "":
        opts.namespace += "_"

    #implementations
    if opts.homes is not None:
        for item in opts.homes:
            retval += generator.home_impl.template (item, opts.namespace)
            artifacts[item] = 1

    if opts.homed_components is not None:
        for item in opts.homed_components:
            retval += generator.homed_comp_impl.template (item)
            artifacts[item] = 1

    if opts.components is not None:
        for item in opts.components:
            retval += generator.comp_impl.template (item, opts.namespace)
            artifacts[item] = 1

    #instances
    if opts.homes is not None:
        for item in opts.homes:
            retval += generator.home_inst.template (item)
    
    if opts.homed_components is not None:
        for item in opts.homed_components:
            retval += generator.homed_comp_inst.template (item)
            
    if opts.components is not None:
        for item in opts.components:
            retval += generator.comp_inst.template (item, "", "", "NodeOne")
        
    #artifacts
    if artifacts is not None:
        for item in artifacts.keys():
            retval += generator.artifact.template(item)

    retval += generator.footer.template ()
    
    outfile = open (opts.output, 'w')
    outfile.write (retval)
    outfile.close ()
    

if __name__ == "__main__":
    main ()

    
    
