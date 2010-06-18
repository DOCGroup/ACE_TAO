from templet import stringfunction

@stringfunction
def template (name, provider, kind, instance) :
    """
    <internalEndpoint>
      <portName>${name}</portName>
      <provider>${provider}</provider>
      <kind>${kind}</kind>
      <instance xmi:idref="${instance}" />
    </internalEndpoint>
    """
