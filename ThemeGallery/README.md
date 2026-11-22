# ThemeGallery

Small host application that shows the building blocks of the shared design foundation (`JUCEAppBasics::CustomLookAndFeel`) in the dark and the light palette side by side, with an accent selector. It serves as the visual check of the theme and the building block states (hover/pressed/focus/disabled/on), independent of any connection or audio device.

Build: open `ThemeGallery.jucer` with the Projucer (version matching the JUCE submodule) and export the Xcode (macOS) or Linux Makefile project. The module paths are `../../JUCE/modules`, so the target builds when JUCE-AppBasics is checked out as a submodule next to the JUCE submodule (as in Mema and Umsci).

Not shown yet:
- Rows of a `juce::ListBox` and a vertical `juce::ScrollBar`: both are created like the other components but do not appear in the panel. The cause has not been analysed yet. The `TreeView`, which uses the same list box colour ids, is shown.
- Hover, pressed and focus states cannot be forced from code; drive them with the mouse or the keyboard.
- The alert window uses the palette of its panel; popup menus and callouts are launched from the buttons of the row "Popups".
