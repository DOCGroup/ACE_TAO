package gjt.test;

import java.applet.Applet;
import java.awt.*;

// The OccupationOracle class makes a guess at a person's occupation 
// within an engineering organization based on a few "key" traits.
// Invalid entries in numeric fields result in an "Unknown" occupation.
// This applet uses the awt.GridBagLayout class to structure the
// occupation form.  The awt.GridBagLayout class allows fields to
// be placed in rows and columns within a form.  Each component
// is given a "display area" based on the constraints in effect
// when it is added to the layout.

// Author:  Jerry Jackson (thanks, sifu)

public class OccupationOracle extends Panel {

    // Construct the form.  Create each component of the form and
    // add it to the layout.  Initialize the occupation to "Unknown". 

    public OccupationOracle() {

    // Use the GridBagLayout layout to construct rows and
    // columns.

    GridBagLayout      gridbag     = new GridBagLayout();

    // Create a new set of constraints to use when adding
    // a component to the layout.  The constraint values
    // in effect when a component is added to the layout
    // are cloned and stored in conjunction with the component 
    // by the layout.

    GridBagConstraints constraints = new GridBagConstraints();

    // Set the font for the form.

    //setFont(new Font("TimesRoman", Font.BOLD, 12));

    // Associate the GridBagLayout object with the applet.

    setLayout(gridbag);

    // The "anchor" constraint determines how a component
    // is justified within its display area.

    constraints.anchor  = GridBagConstraints.WEST;

    // Determines how much space should be given to this component.
    // if left at 0.0, all components clump up in the middle as the
    // padding is applied to the outside.

    constraints.weightx = 1.0;

    // Create a name label and text field.

    makeNameField();

    // Setting the "gridwidth" constraint to 1  will
    // cause the component to take up the minimum 
    // horizontal space in its row.
    
    constraints.gridwidth = 1;

    // "addFormComponent" will associate the current constraints 
    // with a component and add the component to the form.

    addFormComponent(gridbag, nameLabel, constraints);

    // Setting the "gridwidth" constraint to REMAINDER will
    // cause the component to fill up the remainder of its row. 
    // i.e. it will be the last entry in the row.

    constraints.gridwidth = GridBagConstraints.REMAINDER;

    // The "fill" constraint tells what to do if the item is in
    // a area larger than it is.  In this case we want to fill
    // any extra horizontal space.

    constraints.fill      = GridBagConstraints.HORIZONTAL;

    addFormComponent(gridbag, nameField, constraints);

    // Create and add an age label and text field.

    makeAgeField();

    constraints.gridwidth = 1;
    constraints.fill      = GridBagConstraints.NONE;
    constraints.weightx   = 0.0;
    addFormComponent(gridbag, ageLabel, constraints);
    constraints.gridwidth = GridBagConstraints.REMAINDER;
    constraints.weightx   = 1.0;
    addFormComponent(gridbag, ageField, constraints);

    // Create and add a world view label and a single checkbox
    // for a true/false value.

    makeWorldViewField();

    constraints.gridwidth = 1;
    constraints.weightx   = 0.0;
    addFormComponent(gridbag, worldViewLabel, constraints);
    constraints.gridwidth = GridBagConstraints.REMAINDER;
    constraints.weightx   = 1.0;
    addFormComponent(gridbag, worldViewField, constraints);


    // Create and add a coffee consumption label and text field.

    makeCoffeeField();

    constraints.gridwidth = 1;
    constraints.weightx   = 0.0;
    addFormComponent(gridbag, coffeeLabel, constraints);
    constraints.gridwidth = GridBagConstraints.REMAINDER;
    constraints.weightx   = 1.0;
    addFormComponent(gridbag, coffeeField, constraints);


    // Create and add a fashion sense label and a checkbox
    // group that has three mutually exclusive values.

    makeFashionField();

    constraints.gridwidth = GridBagConstraints.REMAINDER;
    constraints.weightx   = 0.0;
    constraints.weighty   = 0.0;
    addFormComponent(gridbag, fashionLabel, constraints);

    // The three checkboxes that represent fashion sense.

    addFormComponent(gridbag, low, constraints);
    addFormComponent(gridbag, medium, constraints);
    addFormComponent(gridbag, high, constraints);

    // The Occupation field is output only.

    makeOccupationField();

    constraints.gridwidth = 1;
    constraints.weightx   = 0.0;
    constraints.weighty   = 1.0;
    constraints.fill      = GridBagConstraints.NONE;
    addFormComponent(gridbag, occupationLabel, constraints);
    constraints.fill      = GridBagConstraints.HORIZONTAL;
    constraints.gridwidth = GridBagConstraints.REMAINDER;
    constraints.weightx   = 1.0;
    addFormComponent(gridbag, occupationField, constraints);

    // Display the initial "Unknown" occupation.

    recalculateOccupation();

    resize(400, 250);
    }

    // The paint() method for this applet just calls the paintComponents()
    // method which is defined by the Container class.  It causes all
    // the components visible within the Container to get painted.

    public void paint(Graphics g) {
    paintComponents(g);
    }

    // When any action occurs within the form we do the same thing:
    // recalculate the person's occupation.

    public boolean action(Event event, Object arg) {
    recalculateOccupation();
    return true;
    }

    // A helper function that associates constraints with a component
    // and adds it to the form.

    private void addFormComponent(GridBagLayout grid, Component comp, 
                  GridBagConstraints c) {
    grid.setConstraints(comp, c);
    add(comp);
    }


    // recalculateOccupation() fetches the values of each component
    // and computes an occupation based on some truly stupid heuristics. 

    private void recalculateOccupation() {

    // If we don't have a name yet we might incorrectly categorize
    // the CEO!

    if (nameField.getText() == "") {
        occupationField.setText("Unknown");
    }

    // Fetch other important values that we'll use in our 
    // calculations.

    int     age;
    int     coffeeConsumption;
    boolean binaryView = worldViewField.getState();


    // Try to fetch integer values for age and coffeeConsumption.
    // If the values in the fields can't be parsed as integers,
    // set the occupation to "Unknown".

    try {
        age               = Integer.parseInt(ageField.getText());
        coffeeConsumption = Integer.parseInt(coffeeField.getText());
    } catch (Exception e) {
        occupationField.setText("Unknown");
        return;
    }

    // Check for the CEO.

    String name = nameField.getText();

    if (name.endsWith("II")  || 
        name.endsWith("III") || 
        name.endsWith("IV")) {

        if (age < 35 || coffeeConsumption < 4) {
        occupationField.setText("Junior Executive");
        } else {
        occupationField.setText("CEO");
        }

        return;
    }

    // Fashion sense is a critical piece of information.  
    // The getCurrent() method of CheckboxGroup returns whichever
    // Checkbox in the group is currently selected.  Only one
    // can be selected at a time.

    Checkbox fashionValue = fashionGroup.getCurrent();

    if (fashionValue == low || fashionValue == medium) {

        // There are two kinds of people in the world: those who
        // divide people into two kinds and those who don't.

        if (binaryView && coffeeConsumption >= 4) {
        occupationField.setText("Engineer");

        } else if ((age > 40 && binaryView) || 
               (age < 40 && coffeeConsumption >= 4)) {
        occupationField.setText("Engineering Manager");

        } else {
        occupationField.setText("Product Manager");
        }

    } else {

        // High fashion sense.  Not an engineer!

        if (binaryView || coffeeConsumption >= 4) {
        occupationField.setText("Vice President");

        } else {
        occupationField.setText("Product Marketing");
        }
    }
    }

    // Helper functions to create form components.

    private void makeNameField() {
    nameLabel = new Label("Name: ");
    nameField = new TextField(40);
    }

    private void makeAgeField() {
    ageLabel = new Label("Age: ");
    ageField = new TextField(3);
    }

    private void makeOccupationField() {
    occupationLabel = new Label("Occupation: ");
    occupationField = new TextField(40);
    }

    private void makeWorldViewField() {
    worldViewLabel = new Label("Binary World View: ");
    worldViewField = new Checkbox();
    }

    private void makeCoffeeField() {
    coffeeLabel = new Label("Coffee consumption: ");
    coffeeField = new TextField(3);
    }

    private void makeFashionField() {
    fashionLabel = new Label("Fashion sense:");

    fashionGroup = new CheckboxGroup();
    low      = new Checkbox("Low   ", fashionGroup, false);
    medium       = new Checkbox("Medium", fashionGroup, true);
    high         = new Checkbox("High  ", fashionGroup, false);
    }

    // Text fields.

    private TextField     nameField;
    private TextField     ageField;
    private TextField     coffeeField;
    private TextField     occupationField;

    // Labels.

    private Label         nameLabel;
    private Label         ageLabel;
    private Label         coffeeLabel;
    private Label         fashionLabel;
    private Label         worldViewLabel;
    private Label         occupationLabel;

    // Checkboxes.

    private Checkbox      worldViewField;
    private Checkbox      low;
    private Checkbox      medium;
    private Checkbox      high;

    // The fashion sense checkbox group.

    private CheckboxGroup fashionGroup;
}

    
