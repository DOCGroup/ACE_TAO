from templet import stringfunction

@stringfunction
def template (component_name) :
    """
  <implementation xmi:id="${component_name}ComponentImplementation">
    <name>${component_name}ComponentImplementation</name>
    <source/>
    <artifact xmi:idref="${component_name}_ExecArtifact" />
    <artifact xmi:idref="${component_name}_SvntArtifact" />
    <artifact xmi:idref="${component_name}_StubArtifact" />
  </implementation>    
    """
