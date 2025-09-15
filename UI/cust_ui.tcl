package require Tk 8.6

namespace eval ttk::theme::cust_ui {

    variable version 1.0
    package provide ttk::theme::cust_ui $version
    variable colors
    array set colors {
        -fg             "#eeeeee"
        -bg             "#313131"
        -disabledfg     "#595959"
        -disabledbg     "#ffffff"
        -selectfg       "#ffffff"
        -selectbg       "#217346"
    }

    proc LoadImages {imgdir} {
        variable I
        foreach file [glob -directory $imgdir *.png] {
            set img [file tail [file rootname $file]]
            set I($img) [image create photo -file $file -format png]
        }
    }

    LoadImages [file join [file dirname [info script]] cust_ui_elems]


    ttk::style layout Red_Accent.TButton {
        Red_AccentButton.button -children {
            Red_AccentButton.padding -children {
                Red_AccentButton.label -side left -expand true
            }
        }
    }

    # Elements

    # AccentButton
    ttk::style configure Red_Accent.TButton -padding {8 4 8 4} -width -10 -anchor center -foreground #eeeeee

    ttk::style element create Red_AccentButton.button image \
        [list $I(red-rect-accent) \
            {selected disabled} $I(red-rect-accent-hover) \
            disabled $I(red-rect-accent-hover) \
            selected $I(red-rect-accent) \
            pressed $I(red-rect-accent) \
            active $I(red-rect-accent-hover) \
        ] -border 4 -sticky nsew
}
