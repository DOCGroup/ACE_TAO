from templet import stringfunction

@stringfunction
def template (component_name, config_values, impl, node) :
    """ 
    ${{
    if impl == "": 
       impl = component_name + "ComponentImplementation"
       
    if node == "":
       node = "NodeOne"
    
    }}

  <instance xmi:id="${component_name}ComponentInstance">
    <name>${component_name}Component</name>
    <node>${node}</node>
    <!-- hostname -->
    <source/>
    <implementation xmi:idref="${impl}" />

    ${config_values}
  </instance>
   
 """
