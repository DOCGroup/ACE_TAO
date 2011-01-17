from templet import stringfunction

@stringfunction
def template (requirement_name, requirement_type) :
    """
    <deployRequirement>
      <name>${requirement_name}</name>
      <resourceType>${requirement_type}</resourceType>
    </deployRequirement>
    """
    
