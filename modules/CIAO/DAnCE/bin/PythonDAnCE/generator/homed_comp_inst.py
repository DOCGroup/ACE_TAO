from templet import stringfunction

@stringfunction
def template (component_name) :
    """
  <instance xmi:id="${component_name}ComponentInstance">
    <name>${component_name}Component</name>
    <node>NodeOne</node>
    <!-- hostname -->
    <source/>
    <implementation xmi:idref="${component_name}ComponentImplementation" />
    <configProperty>
      <name>edu.vanderbilt.dre.DAnCE.ExplicitHome</name>
      <value>
        <type>
          <kind>tk_string</kind>
        </type>
        <value>
          <string>${component_name}Home</string>
        </value>
      </value>
    </configProperty>
  </instance>
    """
