from templet import stringfunction

@stringfunction
def template (component_name):
    """
  <artifact xmi:id="${component_name}_ExecArtifact">
    <name>${component_name}_exec</name>
    <source/>
    <node/>
    <location>${component_name}_exec</location>
  </artifact>
  <artifact xmi:id="${component_name}_SvntArtifact">
    <name>${component_name}_svnt</name>
    <source/>
    <node/>
    <location>${component_name}_svnt</location>
  </artifact>
  <artifact xmi:id="${component_name}_StubArtifact">
    <name>${component_name}_stub</name>
    <source/>
    <node/>
    <location>${component_name}_stub</location>
  </artifact>  
  """
