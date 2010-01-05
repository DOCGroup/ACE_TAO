from templet import stringfunction

@stringfunction
def template (component_name, namespace) :
    """
  <implementation xmi:id="${component_name}HomeImplementation">
    <name>${component_name}HomeImplementation</name>
    <source/>
    <artifact xmi:idref="${component_name}_ExecArtifact" />
    <artifact xmi:idref="${component_name}_SvntArtifact" />
    <execParameter>
      <!-- entrypoint as exec parameter (see 10.6.1) -->
      <name>home factory</name>
      <value>
        <type>
          <kind>tk_string</kind>
        </type>
        <value>
          <string>create_${namespace}${component_name}Home_Impl</string>
        </value>
      </value>
    </execParameter>
    <execParameter>
      <name>edu.vanderbilt.dre.CIAO.ServantEntrypoint</name>
      <value>
        <type>
          <kind>tk_string</kind>
        </type>
        <value>
          <string>create_${namespace}${component_name}Home_Servant</string>
        </value>
      </value>
    </execParameter>
    <execParameter>
      <name>edu.vanderbilt.dre.CIAO.ServantArtifact</name>
      <value>
        <type>
          <kind>tk_string</kind>
        </type>
        <value>
          <string>${component_name}_svnt</string>
        </value>
      </value>
    </execParameter>
    <execParameter>
      <name>edu.vanderbilt.dre.CIAO.ExecutorArtifact</name>
      <value>
        <type>
          <kind>tk_string</kind>
        </type>
        <value>
          <string>${component_name}_exec</string>
        </value>
      </value>
    </execParameter>
  </implementation>
  """
