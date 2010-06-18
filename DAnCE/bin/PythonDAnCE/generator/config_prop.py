from templet import stringfunction

@stringfunction
def template (name, typename, value) :
    """
    <configProperty>
      <name>${name}</name>
      <value>
        <type>
          <kind>tk_${typename}</kind>
        </type>
        <value>
          <${typename}>${value}</${typename}>
        </value>
      </value>
    </configProperty>
    """
