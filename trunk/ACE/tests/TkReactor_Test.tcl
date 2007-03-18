# This file works in conjunction with the C++ source code in TkReactor_Test.cpp


# Create a frame.
set label_var "label for all events"

# Add the new dude button.
button .pressme -text "Press me" -command pressme

# Add a quit button.
button .goodbye -text "Quit" -command exit

label .label_for_event_one -textvariable label_var

pack .label_for_event_one .goodbye .pressme -side top -anchor w
