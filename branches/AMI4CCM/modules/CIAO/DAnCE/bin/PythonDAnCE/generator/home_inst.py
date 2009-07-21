from templet import stringfunction

@stringfunction
def template (component_name) :
    """
  <instance xmi:id="${component_name}HomeInstance">
    <name>${component_name}Home</name>
    <node>NodeOne</node>
    <!-- hostname -->
    <source/>
    <implementation xmi:idref="${component_name}HomeImplementation" />
  </instance>
    """
