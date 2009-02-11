from templet import stringfunction

@stringfunction
def template (component_name) :
    """
  <instance xmi:id="${component_name}ComponentInstance">
    <name>${component_name}Component</name>
    <node>Node</node>
    <!-- hostname -->
    <source/>
    <implementation xmi:idref="${component_name}ComponentImplementation" />
  </instance>
    """
