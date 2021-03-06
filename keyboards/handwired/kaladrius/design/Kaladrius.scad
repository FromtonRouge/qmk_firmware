/* Copyright 2018 FromtonRouge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

assert(version()[0] >= 2019, "Invalid OpenSCAD version. At least version 2019.05 is needed");

use <Boxes.scad>
use <BezierScad.scad>
use <MCAD/nuts_and_bolts.scad>

fragments_number = $preview ? 0 : 60; // Use 0 for debugging, 60 for final rendering

/* [Keyboard Part To Design] */
Design_Mode = 0; // [0:Plate, 1:Case, 2:Link -> need fix with service.netfabb.com, 3:Link Plate, 4:Tenting Adjustment, 5:Case & Plate & Tent, 6:Test Nut Holes]

// Only valid for 'Plate' mode
Show_Plate_Helpers = false;

// Only valid for 'Plate' mode
Show_Plate_Keycaps = false;

/* [Switch Hole] */

// (mm)
Switch_Hole_Width = 14;

// (mm)
Switch_Hole_Tolerance = -0.1;

// (mm, 5.00mm for both MBK Choc and G20 Keycaps)
Space_Between_Switch_Holes_X = 5.00; // [3.5:0.05:6]

// (mm, 4.10mm for MBK Choc Keycaps, 5.00mm for G20 keycaps)
Space_Between_Switch_Holes_Y = 4.10; // [3.5:0.05:6]

// (mm)
Switch_Hole_Height = 22;

/* [Fingers Column Offsets] */

// (-2 for ergodox)
Middle_Finger_Offset = 0; // [-10:10]

// (2 for ergodox)
Ring_Finger_Offset = 0; // [-5:14]

// (3 for ergodox)
Pinky_Finger_Offset = 0; // [-10:10]

/* [Nut Slot] */

// (mm)
Nut_Slot_Tolerance = 0.0; // [0:0.01:0.2]

// (mm)
Nut_Slot_Diameter = 10; // [9:0.1:12]

// (mm)
Nut_Hole_3mm_Tolerance = 0.22; // [0:0.01:0.5]

// (mm)
Nut_Hole_2mm_Tolerance = 0.22; // [0:0.01:0.5]

/* [Thumb Zone Position] */

// (mm)
Thumb_Zone_Origin_X = 15; // [-20:0.1:20]

// (mm)
Thumb_Zone_Origin_Y = 10.3; // [0:0.1:20]

// (degree)
Thumb_Zone_Angle = -18; // [-45:0]

/* [Case] */

// Usefull for debugging
Case_Render_Shell = true;

// (mm)
Case_Shell_Thickness = 3; // [2:4]

// (mm)
Case_Height = 8.5; // [7:0.1:12]

// (mm)
Case_Outer_Border = 8; // [8:12]

// (mm)
Case_Top_Plate_Offset = -1; // [-2:0.1:0]

/* [Plate] */

// Generate a small plate to test finger positions
Plate_Test_Finger_Positions = false;

// (mm)
Plate_Height = 3; // [2:4]

// (mm)
Plate_Additional_Border_Height = 1; // [0:0.1:2]

Plate_Bezier_Controls_Mode = false;

Plate_Show_Bezier_Controls = false;

Plate_Control_Top_0 = 0; // [-20:0.1:20]
Plate_Control_Top_1 = 0.4; // [-20:0.1:20]
Plate_Control_Top_2 = 0; // [-20:0.1:20]
Plate_Control_Top_3 = -1; // [-20:0.1:20]
Plate_Control_Top_4 = -2.2; // [-20:0.1:20]
Plate_Control_Top_5 = -2.1; // [-20:0.1:20]
Plate_Control_Top_6 = 0; // [-20:0.1:20]

Plate_Control_Bottom_0 = 0; // [-20:0.1:20]
Plate_Control_Bottom_1 = 5.6; // [-20:0.1:20]
Plate_Control_Bottom_2 = 1.8; // [-20:0.1:20]
Plate_Control_Bottom_3 = -15; // [-20:0.1:20]
Plate_Control_Bottom_4 = 0; // [-20:0.1:20]
Plate_Control_Bottom_5 = 0; // [-20:0.1:20]
Plate_Control_Bottom_6 = 0; // [-20:0.1:20]

/* [Screws] */

// (mm)
Screws_Diameter = 3.5; // [3:0.1:4]

// (mm)
Electronic_Screw_Mount_Diameter = 6; // [5:0.1:7]

// (mm)
Electronic_Screws_Hole_Diameter = 2.7; // [2:0.1:3]

/* [Electronic and Teensy] */

// (mm)
Electronic_Pcb_Dimensions = [40, 60, 1.54]; // Real size of the pcb plate in mm
electronic_hole_to_hole_dim = [Electronic_Pcb_Dimensions[0] - 4, Electronic_Pcb_Dimensions[1] - 4, Electronic_Pcb_Dimensions[2]];

// (mm)
Electronic_Pcb_Mount_Height = 1;

// (mm)
Teensy_Plate_Size = [60.5, 61.8];

// (mm)
Teensy_Plate_Thickness = 3; // [2:4]

// (mm)
Teensy_Wall_Thickness = 3; // [2:4]

// (mm)
Teensy_Case_Roundness = 4.5; // [2:0.1:10]

teensy_case_parameters = get_box_parameters(Teensy_Plate_Size, Teensy_Case_Roundness, Teensy_Plate_Thickness, Teensy_Wall_Thickness);

/* [Tent Module] */

// (degree)
Tenting_Angle = 5.4; // [5.4:0.1:10.5]

// (degree)
Tilt_Angle = 5; // [0:0.1:10]

// (mm)
Tent_Distance_X = 103; // [95:0.1:110]
Tent_Distance_Y = -4; // [-5:0.1:5]

tent_pos = [86.5, Pinky_Finger_Offset-70];

Tent_Profile_Cube = [49, 70, 1];

Tent_Thumb_Scale = 0.75;

/* [Link Module] */

// (mm)
Link_Nut_Slot_Height = 12.2; // [10:0.1:20]

Link_Reinforcement = true;

// (mm)
Link_Reinforcement_Radius = 107; // [100:1:130]

Link_Plate_Screw_Holes = true;

function switch_hole_and_space_x() = Switch_Hole_Width + Space_Between_Switch_Holes_X;
function switch_hole_and_space_y() = Switch_Hole_Width + Space_Between_Switch_Holes_Y;

point_pinky_last = [get_kaladrius_origin()[0] + Case_Outer_Border, get_kaladrius_origin()[1]];
point_pinky = [point_pinky_last[0] + switch_hole_and_space_x(), point_pinky_last[1]];
point_ring = [point_pinky[0] +  switch_hole_and_space_x(), point_pinky[1] + Pinky_Finger_Offset];
point_middle = [point_ring[0] + switch_hole_and_space_x(), point_ring[1] + Ring_Finger_Offset];
point_index3 = [point_middle[0] + switch_hole_and_space_x(), point_middle[1] + Middle_Finger_Offset];
point_index2 = [point_index3[0] + switch_hole_and_space_x(), point_index3[1]];
point_index1 = [point_index2[0] + switch_hole_and_space_x(), point_index2[1]];
point_star_key = [point_index1[0], point_index1[1] - 2*(switch_hole_and_space_y())];

function switches(color, rows, cols, origin, vertical=false, special_key = "") = [color, rows, cols, origin, vertical, special_key];
function is_rect_plate() = Middle_Finger_Offset == 0 && Ring_Finger_Offset == 0 && Pinky_Finger_Offset == 0;

k = [

    // Fingers
    switches("red", 5, 1, point_pinky_last),
    switches("magenta", 5, 1, point_pinky),
    switches("green", 5, 1, point_ring),
    switches("lightGreen", 5, 1, point_middle, false, "alt_key"),
    switches("blue", 4, 1, point_index3),
    switches("lightBlue", 4, 1, point_index2),
    switches("cyan", 2, 1, point_index1),
    
    // Star key
    switches("green", 1, 1, point_star_key, true, "star_key"),

    // Thumbs
    switches("red", 1, 1, [0, 0]),
    switches("green", 1, 1, [-(switch_hole_and_space_x())/2 - (switch_hole_and_space_x()), -(switch_hole_and_space_y()) - (switch_hole_and_space_y())/2], true),
    switches("blue", 1, 2, [-(switch_hole_and_space_x())/2, -(switch_hole_and_space_y())]),
    switches("lightBlue", 1, 1, [0, -2*(switch_hole_and_space_y())]),
];

hole_positions = [
    k[0][3],    // Top left
    k[3][3] + [(Switch_Hole_Width + 2*Space_Between_Switch_Holes_X)/2, Space_Between_Switch_Holes_Y/2], // Top middle
    k[6][3] + [(Switch_Hole_Width + 2*Space_Between_Switch_Holes_X), 0], // Top right
    k[7][3] + [(Switch_Hole_Width + 2*Space_Between_Switch_Holes_X), -k[7][1]*Switch_Hole_Width - (k[7][1]+1)*Space_Between_Switch_Holes_Y], // Middle right
    k[0][3] - [0, k[0][1]*Switch_Hole_Width + (k[0][1]+1)*Space_Between_Switch_Holes_Y], // Bottom left

    // Thumb
    [-1.75*(switch_hole_and_space_x()) + 3*Switch_Hole_Width + 4*Space_Between_Switch_Holes_X, 0], // Top right
    [-1.75*(switch_hole_and_space_x()) + 3*Switch_Hole_Width + 4*Space_Between_Switch_Holes_X,  -3*Switch_Hole_Width - 4*Space_Between_Switch_Holes_Y], // Bottom right
    [-1.75*(switch_hole_and_space_x()), -3*Switch_Hole_Width - 4*Space_Between_Switch_Holes_Y], // Bottom left
];

function get_kaladrius_origin() = [0, 0, 0];
function get_thumb_anchor() = k[6][3] - [0, 3*(switch_hole_and_space_y())];
function get_thumb_origin() = get_thumb_anchor() + [Thumb_Zone_Origin_X, -Thumb_Zone_Origin_Y];
function get_tent_origin() = get_kaladrius_origin() + tent_pos;
function get_tenting_angle() = Tenting_Angle;
function get_cells(height, rows, columns, xy_scale = 1) = [xy_scale*(columns*Switch_Hole_Width + (columns+1)*Space_Between_Switch_Holes_X), xy_scale*(rows*Switch_Hole_Width + (rows+1)*Space_Between_Switch_Holes_Y), height];
function get_thumb_profile_cube() = get_cells(1, 3, 3, Tent_Thumb_Scale);
function get_points_from_cube(c) = [[0, 0], [c[0], 0], [c[0], c[1]], [0, c[1]]];
function get_tent_screw_locations() = [[Tent_Profile_Cube[0], Tent_Profile_Cube[1]], [0, Tent_Profile_Cube[1]]];
function get_tent_thumb_screw_locations() = [[0, 0], [get_thumb_profile_cube()[0], 0]];
function get_tilt_origin() = hole_positions[2] + [Case_Outer_Border + Case_Shell_Thickness, Case_Outer_Border + Case_Shell_Thickness];

module printable_nut_hole(size, tolerance, cone=true)
{
    if (cone)
    {
        hull()
        {
            scale([0.2,0.2,1.6]) nutHole(size = size, tolerance = tolerance);
            nutHole(size = size, tolerance = tolerance);
        }
    }
    else
    {
        nutHole(size = size, tolerance = tolerance);
    }
}

module case_hole(height, diameter=Screws_Diameter)
{
    cylinder(height-2, d=diameter, $fn = fragments_number);
    translate([0, 0, height-2])
    {
        cylinder(2, d=diameter, $fn = fragments_number);
    }
}

module transform_tilt()
{
    translate(get_tilt_origin())
    {
        rotate([0, 0, -Tilt_Angle])
        {
            translate(-get_tilt_origin())
            {
                children();
            }
        }
    }
}

module transform_star_key()
{
    translate([0, -(switch_hole_and_space_x())/4, 0])
    {
        children();
    }
}

module transform_thumb()
{
    translate(get_thumb_origin())
    {
        rotate([0, 0, Thumb_Zone_Angle])
        {
            children();
        }
    }
}

module transform_thumb_profile()
{
    transform_thumb()
    {
        original_width = 3*Switch_Hole_Width + 4*Space_Between_Switch_Holes_X;
        translate([(original_width-Tent_Thumb_Scale*original_width)/2, -(original_width-Tent_Thumb_Scale*original_width)/2, 0])
        {
            translate([-(switch_hole_and_space_x())/4, 0, 0])
            {
                translate([-1.5*(switch_hole_and_space_x()), 0, 0])
                {
                    translate([0, -get_thumb_profile_cube()[1], 0]) children();
                }
            }
        }
    }
}


module holes(height = Switch_Hole_Height, has_additional_border = true)
{
    module make_keyboard_hole(i)
    {
        color(k[i][0]) create_holes(height, k[i][1], k[i][2], k[i][3], has_additional_border, k[i][4], k[i][5]);
    }

    // Fingers
    for (index = [0:6])
    {
        make_keyboard_hole(index);
    }

    // Star key
    transform_star_key()
    {
        make_keyboard_hole(7);
    }

    // Thumbs
    transform_thumb()
    {
        make_keyboard_hole(8);
        translate([-(switch_hole_and_space_x())/4, 0, 0])
        {
            for (index = [9:11])
            {
                make_keyboard_hole(index);
            }
        }
    }
}

module transform_hole(index)
{
    translate(hole_positions[index]) children();
}

module transform_for_each_hole()
{
    transform_hole(0) children();
    transform_hole(1) children();
    transform_hole(2) children();
    transform_hole(4) children();

    transform_thumb()
    {
        transform_hole(5) children();
        transform_hole(6) children();
        transform_hole(7) children();
    }
}

module case_holes(offset=0, height=Switch_Hole_Height, diameter=Screws_Diameter)
{
    module make_hole()
    {
        case_hole(height, diameter);
        make_case_screw_hole();
    }

    color("red")
    {
        translate([0, 0, -1])
        {
            transform_for_each_hole() make_hole();
        }
    }
}

module screw_mounts()
{
    height = 8;
    module make_mount()
    {
        cylinder(h=height, d=Nut_Slot_Diameter, $fn = fragments_number);
    }

    module transform_for_each_mount()
    {
        translate([0, 0, Case_Height + Case_Shell_Thickness + Case_Top_Plate_Offset - height])
        {
            transform_hole(0) rotate([0, 0, -45]) children();
            transform_hole(1) rotate([0, 0, -90]) children();
            transform_hole(2) rotate([0, 0, -90]) children();
            transform_hole(4) rotate([0, 0, 45]) children();

            transform_thumb()
            {
                transform_hole(5) rotate([0, 0, 70]) children();
                transform_hole(6) rotate([0, 0, 90]) children();
                transform_hole(7) rotate([0, 0, 90]) children();
            }
        }
    }

    transform_for_each_mount() make_mount();
}

module create_hole(height, has_additional_border = true, vertical = false)
{
    additional_border_width = 0.5;
    clip_width = 6;
    roundness = 0.5;
    x = Switch_Hole_Width - 2*roundness + 2*additional_border_width + Switch_Hole_Tolerance;
    y = Switch_Hole_Width - 2*roundness + Switch_Hole_Tolerance;
    translate([Space_Between_Switch_Holes_X + (Switch_Hole_Width-((vertical?y:x)+2*roundness))/2, -Space_Between_Switch_Holes_Y - (Switch_Hole_Width-((vertical?x:y)+2*roundness))/2, 0])
    {
        translate([roundness, -roundness, 0]) // minkowski compensation
        {
            translate([vertical?y/2:x/2, vertical?-x/2:-y/2, 0]) // move top left to [0,0]
            {
                rotate([0, 0, vertical?90:0])
                {
                    // External hole
                    ext_hole = 1;
                    translate([0, 0, height/2 + (has_additional_border?Plate_Additional_Border_Height:0)]) // The cube is centered, we want to move up the bottom of the cube
                    {
                        minkowski()
                        {
                            cube([Switch_Hole_Width + 2*ext_hole - 2*roundness, Switch_Hole_Width + 2*ext_hole - 2*roundness, height], center=true);
                            cylinder(r=roundness,  h=2, $fn = fragments_number);
                        }
                    }

                    translate([0, 0, -0.01]) // Small vertical offset to have clean holes
                    {
                        minkowski()
                        {
                            linear_extrude(height=height)
                            {
                                // Create a centered polygon, so we can rotate it after
                                points = [
                                    [-x/2, y/2], // top left
                                    [x/2, y/2], // top right
                                    [x/2, clip_width/2],
                                    [x/2-additional_border_width, clip_width/2],
                                    [x/2-additional_border_width, -clip_width/2],
                                    [x/2, -clip_width/2],
                                    [x/2, -y/2],
                                    [-x/2, -y/2],
                                    [-x/2, -clip_width/2],
                                    [-x/2 + additional_border_width, -clip_width/2],
                                    [-x/2 + additional_border_width, clip_width/2],
                                    [-x/2, clip_width/2],
                                ];
                                polygon(points);
                            }
                            cylinder(r=roundness,  h=2, $fn = fragments_number);
                        }
                    }
                }
            }
        }
    }
}

module create_cells(height, rows, columns, origin)
{
    translate(origin)
    {
        cells = get_cells(height, rows, columns);
        translate([0, -cells[1], 0]) cube(cells);
    }
}

module create_holes(height, rows, columns, origin, has_additional_border = true, vertical = false, special_key = "")
{
    translate(origin)
    {
        for (col = [0:columns-1])
        {
            col_offset = col*(switch_hole_and_space_x());
            for (row = [0:rows-1])
            {
                if (special_key == "alt_key" && row == 4)
                {
                    translate([col_offset + switch_hole_and_space_x()/4, -row*(switch_hole_and_space_y()), 0])
                    {
                        create_hole(height, has_additional_border, vertical);
                    }
                }
                else if (special_key == "star_key")
                {
                    translate([col_offset, -row*(switch_hole_and_space_y()) - 0.2, 0])
                    {
                        create_hole(height, has_additional_border, vertical);
                    }
                }
                else
                {
                    translate([col_offset, -row*(switch_hole_and_space_y()), 0])
                    {
                        create_hole(height, has_additional_border, vertical);
                    }
                }
            }
        }
    }
}

module plate(total_height = Plate_Height, chamfer = false)
{
    minkowski()
    { 
        basic_height = total_height/3;
        minkowski_height = 2*basic_height;
        union()
        {
            if (!is_rect_plate())
            {
                // Fingers plate
                hull()
                {
                    top_point_offset = [(Switch_Hole_Width+2*Space_Between_Switch_Holes_X)/2, Space_Between_Switch_Holes_Y];
                    top_control_points =  [
                        k[0][3] + [0, Plate_Control_Top_0],
                        k[1][3] + top_point_offset + [0, Plate_Control_Top_1],
                        k[2][3] + top_point_offset + [0, Plate_Control_Top_2],
                        k[3][3] + top_point_offset + [0, Plate_Control_Top_3],
                        k[4][3] + top_point_offset + [0, Plate_Control_Top_4],
                        k[5][3] + top_point_offset + [0, Plate_Control_Top_5],
                        k[6][3] + [Switch_Hole_Width + 2*Space_Between_Switch_Holes_X, Plate_Control_Top_6],
                    ];

                        bottom_control_points =  [
                            k[0][3] + [0, -k[0][1]*(switch_hole_and_space_y()) - Space_Between_Switch_Holes_Y + Plate_Control_Bottom_0],
                            k[1][3] + [Space_Between_Switch_Holes_X + Switch_Hole_Width/2, -k[1][1]*(switch_hole_and_space_y()) - 2*Space_Between_Switch_Holes_Y + Plate_Control_Bottom_1],
                            k[2][3] + [Space_Between_Switch_Holes_X + Switch_Hole_Width/2, -k[2][1]*(switch_hole_and_space_y()) - 2*Space_Between_Switch_Holes_Y + Plate_Control_Bottom_2],
                            k[3][3] + [Space_Between_Switch_Holes_X + Switch_Hole_Width/2, -k[3][1]*(switch_hole_and_space_y()) - 2*Space_Between_Switch_Holes_Y + Plate_Control_Bottom_3],
                            k[4][3] + [Space_Between_Switch_Holes_X + Switch_Hole_Width/2, -k[4][1]*(switch_hole_and_space_y()) - 2*Space_Between_Switch_Holes_Y + Plate_Control_Bottom_4],
                            k[5][3] + [Space_Between_Switch_Holes_X + Switch_Hole_Width/2, -k[5][1]*(switch_hole_and_space_y()) - 2*Space_Between_Switch_Holes_Y + Plate_Control_Bottom_5],
                            k[7][3]  + [2*Space_Between_Switch_Holes_X + Switch_Hole_Width, -k[7][1]*(switch_hole_and_space_y()) - Space_Between_Switch_Holes_Y + Plate_Control_Bottom_6],
                        ];

                    difference()
                    {
                        union()
                        {
                            for (i = [0:7])
                            {
                                create_cells(basic_height, k[i][1], k[i][2], k[i][3]);
                            }
                        }

                        BezWall(top_control_points, width = 20, height = basic_height, steps = 64, centered = true, showCtlR = Plate_Show_Bezier_Controls);
                        BezWall(bottom_control_points, width = 20, height = basic_height, steps = 64, centered = true, showCtlR = Plate_Show_Bezier_Controls);
                    }

                    BezWall(top_control_points, width = 1, height = basic_height, steps = 64, centered = true, showCtlR = Plate_Show_Bezier_Controls);
                    BezWall(bottom_control_points, width = 1, height = basic_height, steps = 64, centered = true, showCtlR = Plate_Show_Bezier_Controls);
                }
            }
            else
            {
                for (i = [0:7])
                {
                    create_cells(basic_height, k[i][1], k[i][2], k[i][3]);
                }
            }

            // Thumb plate
            if (!Plate_Bezier_Controls_Mode)
            {
                transform_thumb()
                {
                    translate([-(switch_hole_and_space_x())/4, 0, 0])
                    {
                        translate([-1.5*(switch_hole_and_space_x()), 0, 0]) create_cells(basic_height, 3, 3, [0,0]);
                    }
                }
            }
        }
        r1 = Case_Outer_Border;
        r2 = chamfer? r1 - minkowski_height : r1;

        if (!Plate_Bezier_Controls_Mode)
        {
            cylinder(r1=r1, r2=r2, h=minkowski_height, $fn = fragments_number);
        }
    }
}

module right_top_plate()
{
    translate([0, 0, Case_Height + Case_Shell_Thickness])
    {
        difference()
        {
            union()
            {
                plate(chamfer=true);

                color("black")
                {
                    mark_height = Plate_Height + 0.5;
                    translate(get_thumb_anchor() + [0, -Thumb_Zone_Origin_Y]) cube([Thumb_Zone_Origin_X, 2, mark_height]);
                    translate(get_thumb_anchor() + [0, -Thumb_Zone_Origin_Y]) cube([2, Thumb_Zone_Origin_Y, mark_height]);
                }
            }

            holes();
            transform_for_each_hole() translate([0,0,-10]) case_hole(20);
        }
    }
}

module plate_supports()
{
    support_height = Case_Shell_Thickness + Case_Height;
    module create_support(row)
    {
        hull()
        {
            support_diameter = 3.5;
            y = -row*switch_hole_and_space_y();
            translate([0, y-support_diameter/2])
            {
                cylinder(h=support_height-3, d=3.5, $fn=30);
                cylinder(h=support_height, d=1, $fn=30);
            }

            translate([0, y-support_diameter])
            {
                cylinder(h=support_height-3, d=3.5, $fn=30);
                cylinder(h=support_height, d=1, $fn=30);
            }
        }
    }

    wide_support_size = 6;
    module create_wide_support()
    {
        hull()
        {
            translate([0, -wide_support_size]) cylinder(h=support_height-3, d=3.5, $fn=30);
            translate([0, -wide_support_size]) cylinder(h=support_height, d=1, $fn=30);
            translate([0, wide_support_size]) cylinder(h=support_height-3, d=3.5, $fn=30);
            translate([0, wide_support_size]) cylinder(h=support_height, d=1, $fn=30);
        }
    }

    supports_pos = k[2][3] + [3*Space_Between_Switch_Holes_X/2 + Switch_Hole_Width, 0];
    translate(supports_pos)
    {
        for (row = [1:4])
        {
            create_support(row);
        }
    }

    // Left plate support
    left_middle_point = (hole_positions[0] + hole_positions[4])/2;
    translate(left_middle_point)
    {
        create_wide_support();
    }

    // Specific supports when the plate is a rectangle
    if (is_rect_plate())
    {
        // Support on top left
        translate([left_middle_point[0] + (hole_positions[1][0]-left_middle_point[0])/2, hole_positions[0][1]])
        {
            rotate([0,0,90]) create_wide_support();
        }

        // Support on top right
        translate([hole_positions[1][0] + (hole_positions[2][0]-hole_positions[1][0])/2, hole_positions[2][1]])
        {
            rotate([0,0,90]) create_wide_support();
        }

        // Support on the bottom left 1
        translate([left_middle_point[0] + (supports_pos[0]-left_middle_point[0])/2, hole_positions[4][1]])
        {
            rotate([0,0,90]) create_wide_support();
        }

        // Support on the bottom left 2
        translate([supports_pos[0], hole_positions[4][1]])
        {
            rotate([0,0,90]) create_wide_support();
        }
    }

    // Right plate support
    translate(hole_positions[3] + [0, wide_support_size]) create_wide_support();
    translate((hole_positions[2] + hole_positions[3])/2 + [0, wide_support_size]) create_wide_support();

    transform_thumb()
    {
        right_middle_point = (hole_positions[5] + hole_positions[6])/2;
        translate (right_middle_point)
        {
            create_wide_support();
        }

        x = hole_positions[7][0] + (hole_positions[6][0]-hole_positions[7][0])/2;
        y = hole_positions[7][1];
        translate ([x, y])
        {
            rotate([0,0,90]) create_wide_support();
        }
    }
}

module left_case(printable = true)
{
    module transform_tent_holes()
    {
        translate(get_tent_origin())
        {
            tent_screws_points = get_tent_screw_locations();
            translate(tent_screws_points[0]) rotate([0, 0, -90]) children();
            translate(tent_screws_points[1]) rotate([0, 0, -90]) children();
        }

        transform_thumb_profile()
        {
            thumb_screw_points = get_tent_thumb_screw_locations();
            translate(thumb_screw_points[0]) rotate([0, 0, 0]) children();
            translate(thumb_screw_points[1]) rotate([0, 0, 180]) children();
        }
    }

    rotate([0, printable == false ? -get_tenting_angle():0, 0])
    {
        difference()
        {
            union()
            {
                transform_tent_holes() translate([0, 0, Plate_Height])
                {
                    cylinder(h=5, d=Nut_Slot_Diameter, $fn = fragments_number);
                }

                if (Case_Render_Shell)
                {
                    difference()
                    {
                        union()
                        {
                            small_radius = 1;
                            // Bottom
                            minkowski()
                            {
                                plate();
                                cylinder(r1=small_radius, r2=Case_Shell_Thickness,  h=Case_Shell_Thickness, $fn = fragments_number);
                            }

                            // Middle
                            translate([0, 0, Case_Shell_Thickness])
                            {
                                minkowski()
                                {
                                    plate();
                                    cylinder(r=Case_Shell_Thickness,  h=Case_Height-Case_Shell_Thickness, $fn = fragments_number);
                                }
                            }

                            // Top
                            translate([0, 0, Case_Height])
                            {
                                minkowski()
                                {
                                    plate();
                                    cylinder(r2=small_radius, r1=Case_Shell_Thickness,  h=Case_Shell_Thickness, $fn = fragments_number);
                                }
                            }
                        }

                        translate([0, 0, Case_Shell_Thickness]) scale([1, 1, 8]) plate();
                    }
                }

                // Add all bolt mounts
                screw_mounts();

                plate_supports();
            }

            // Top plate hole without chamfer
            translate([0, 0, Case_Height + Case_Shell_Thickness + Case_Top_Plate_Offset]) scale([1, 1, 8]) plate();

            // Case holes
            case_holes(height = 20);

            // Special holes that allows the user to remove the top plate by pushing it from bottom to top with a small screw driver
            left_middle_point = (hole_positions[0] + hole_positions[4])/2;
            left_middle_up_point = (hole_positions[0] + left_middle_point)/2;
            left_middle_down_point = (hole_positions[4] + left_middle_point)/2;
            translate([0, 0, -0.01]) translate(left_middle_up_point) case_hole(20, Screws_Diameter);
            translate([0, 0, -0.01]) translate(left_middle_down_point) case_hole(20, Screws_Diameter);

            // Nut holes to connect the tent system
            transform_tent_holes()
            {
                translate([0, 0, -1]) screw_hole();
                hull()
                {
                    translate([0,0, Case_Shell_Thickness + 8]) printable_nut_hole(3, Nut_Hole_3mm_Tolerance);
                    translate([0,0,Case_Shell_Thickness-1]) printable_nut_hole(3, Nut_Hole_3mm_Tolerance);
                }
            }

            translate(get_tent_origin()) translate([0, 10, -1])
            {
                minkowski()
                {
                    translate([2, 0, 0]) cube(Tent_Profile_Cube + [-2, -20, Case_Shell_Thickness]);
                    cylinder(h=10, r=4, $fn = fragments_number);
                }
            }

            transform_thumb_profile() translate([9, 9, -1])
            {
                minkowski()
                {
                    cube(get_thumb_profile_cube() + [-18, -18, Case_Shell_Thickness]);
                    cylinder(h=10, r=4, $fn = fragments_number);
                }
            }
        }
    }
}

module make_case_screw_hole()
{
    hull()
    {
        translate([0,0, 6.5]) printable_nut_hole(3, Nut_Hole_3mm_Tolerance);
        translate([0,0,-30]) printable_nut_hole(3, Nut_Hole_3mm_Tolerance, false);
    }
}

module make_pcb_case_screw_hole(height = 2)
{
    hull()
    {
        translate([0,0, height]) printable_nut_hole(2, Nut_Hole_2mm_Tolerance);
        translate([0,0,-height]) printable_nut_hole(2, Nut_Hole_2mm_Tolerance, false);
    }
}

module screw_hole(h = 20)
{
    cylinder(h, d=Screws_Diameter, $fn = fragments_number);
}

module head_screw_hole(h = 20)
{
    cylinder(h, d=6, $fn = fragments_number);
}

module left_tent(angle, printable = true, holes_only = false, plain = false)
{
    minkowski_height = 1;
    profile_height = Tent_Profile_Cube[2] + minkowski_height;

    module profile(minkowski_radius)
    {
        translate([0, 0, -profile_height/2])
        {
            translate(get_tent_origin())
            {
                minkowski()
                {
                    cube(Tent_Profile_Cube, center = false);
                    cylinder(h=minkowski_height, r=minkowski_radius, $fn = fragments_number);
                }
            }

            transform_thumb_profile()
            {
                minkowski()
                {
                    cube(get_thumb_profile_cube());
                    cylinder(h=minkowski_height, r=minkowski_radius, $fn = fragments_number);
                }
            }
        }
    }

    module extruded_profile(minkowski_radius, extrude_angle)
    {
        opposite = profile_height;

        adjacent_main_tent = get_tent_origin()[0] + Tent_Profile_Cube[0] + minkowski_radius;

        additional_length = Tent_Thumb_Scale*(-1.75*(switch_hole_and_space_x()) + 3*Switch_Hole_Width + 4*Space_Between_Switch_Holes_X);
        adjacent_thumb_tent = get_thumb_origin()[0] + additional_length + minkowski_radius;

        adjacent = max(adjacent_main_tent, adjacent_thumb_tent);

        step_angle = atan(opposite/adjacent);
        steps = floor(abs(extrude_angle)/step_angle) + 1;
        for (i = [0:steps])
        {
            rotate([0, i*step_angle, 0]) profile(minkowski_radius);
        }
    }

    module tent_screw_hole(h)
    {
        translate([0, 0, 1]) screw_hole(h);
        hull()
        {
            translate([0, 0, h-4]) screw_hole(1);
            head_screw_hole(h-4);
        }
    }

    rotate([0, printable?-angle:0, 0])
    {
        minkowski_external_radius = Teensy_Case_Roundness + 1;
        adjacent = get_tent_origin()[0] + Tent_Profile_Cube[0] + minkowski_external_radius;
        screw_location_height = adjacent*tan(angle) + 1;

        difference()
        {
            union()
            {
                difference()
                {
                    union()
                    {
                        if (plain)
                        {
                            extruded_profile(minkowski_external_radius, -angle);
                        }
                        else
                        {
                            difference()
                            {
                                extruded_profile(minkowski_external_radius, -angle);
                                extruded_profile(minkowski_external_radius - 2, -angle-1);
                            }
                        }

                        intersection()
                        {
                            extruded_profile(minkowski_external_radius, -angle);
                            translate([0, 0, -screw_location_height + 1])
                            {
                                screw_location_radius = 4;
                                translate(get_tent_origin())
                                {
                                    points = get_points_from_cube(Tent_Profile_Cube);
                                    for (p = [points[2], points[3]])
                                    {
                                        translate(p)
                                        {
                                            cylinder(h=screw_location_height, r=screw_location_radius, $fn = fragments_number);
                                        }
                                    }
                                }

                                transform_thumb_profile()
                                {
                                    thumb_points = get_points_from_cube(get_thumb_profile_cube());
                                    for (p = [thumb_points[0], thumb_points[1]])
                                    {
                                        translate(p)
                                        {
                                            cylinder(h=screw_location_height, r=screw_location_radius, $fn = fragments_number);
                                        }
                                    }
                                }
                            }
                        }
                    }

                    plane_to_remove = [180, 180, 40];
                    rotate([0, angle, 0]) translate([0, -160, -plane_to_remove[2]]) cube(plane_to_remove);
                    translate([0, -160, 0]) cube(plane_to_remove);
                }

                rotate([0, angle, 0])
                {
                    translate([0, 0, profile_height/2]) profile(Teensy_Case_Roundness + 0.5);
                }
            }

            if (!plain)
            {
                // Holes
                hole_height = screw_location_height;
                translate([0, 0, -hole_height])
                {
                    translate(get_tent_origin())
                    {
                        points = get_tent_screw_locations();
                        for (p = points)
                        {
                            translate(p)
                            {
                                tent_screw_hole(hole_height);
                            }
                        }
                    }

                    transform_thumb_profile()
                    {
                        thumb_points = get_tent_thumb_screw_locations();
                        for (p = thumb_points)
                        {
                            translate(p)
                            {
                                tent_screw_hole(hole_height);
                            }
                        }
                    }
                }
            }
        }
    }
}

module left_tilted_tent(angle, printable = true, holes_only = false, plain = false)
{
    transform_tilt()
    {
        left_tent(angle, printable = printable, holes_only = holes_only, plain = plain);
    }
}

module create_keycap(vertical = false, horizontal_stretch = 1)
{
    height = 4;
    roundness = 1.5;
    // Keycap size = base_size + 2*offset
    offset = 2.2;
    base_size = 14;
    x = horizontal_stretch*(base_size + 2*offset) - 2*roundness;
    y = (base_size + 2*offset - 2*roundness);
    translate([Space_Between_Switch_Holes_X + (base_size-((vertical?y:x)+2*roundness))/2, -Space_Between_Switch_Holes_Y - (base_size-((vertical?x:y)+2*roundness))/2, 0])
    {
        translate([roundness, -roundness, 0]) // minkowski compensation
        {
            translate([vertical?y/2:x/2, vertical?-x/2:-y/2, 0]) // move top left to [0,0]
            {
                rotate([0, 0, vertical?90:0])
                {
                    translate([0, 0, -0.01]) // Small vertical offset to have clean holes
                    {
                        minkowski()
                        {
                            linear_extrude(height=height)
                            {
                                // Create a centered polygon, so we can rotate it after
                                points = [
                                    [-x/2, y/2], // top left
                                    [x/2, y/2], // top right
                                    [x/2, 0],
                                    [x/2, 0],
                                    [x/2, 0],
                                    [x/2, 0],
                                    [x/2, -y/2],
                                    [-x/2, -y/2],
                                    [-x/2, 0],
                                    [-x/2, 0],
                                    [-x/2, 0],
                                    [-x/2, 0],
                                ];
                                polygon(points);
                            }
                            cylinder(r=roundness,  h=2, $fn = 20);
                        }
                    }
                }
            }
        }
    }
}

module create_keycaps(rows, columns, origin, vertical = false, horizontal_stretch = 1)
{
    translate(origin)
    {
        for (col = [0:columns-1])
        {
            col_offset = col*(switch_hole_and_space_x());
            for (row = [0:rows-1])
            {
                translate([col_offset, -row*(switch_hole_and_space_y()), 0])
                {
                    create_keycap(vertical, horizontal_stretch);
                }
            }
        }
    }
}

module left_keycaps()
{
    translate([0, 0, Case_Height + Case_Shell_Thickness + Plate_Height])
    {
        for (i = [0:6])
        {
            create_keycaps(k[i][1], k[i][2], k[i][3]);
        }

        // Star key
        transform_star_key()
        {
            create_keycaps(k[7][1], k[7][2], k[7][3], k[7][4], horizontal_stretch=1.5);
        }

        transform_thumb()
        {
            create_keycaps(k[8][1], k[8][2], k[8][3], k[8][4], horizontal_stretch=1.5);
            translate([-(switch_hole_and_space_x())/4, 0, 0])
            {
                create_keycaps(1, 1, [-(switch_hole_and_space_x())/2 - (switch_hole_and_space_x()), -(switch_hole_and_space_y()) - (switch_hole_and_space_y())/2], vertical = true, horizontal_stretch = 2);
                create_keycaps(1, 2, [-(switch_hole_and_space_x())/2, -(switch_hole_and_space_y())]);
                create_keycaps(1, 1, [0, -2*(switch_hole_and_space_y())], horizontal_stretch = 2);
            }
        }
    }
}

// Kaladrius link system
module transform_link_system()
{
    translate([-Tent_Distance_X, Tent_Distance_Y, 0])
    {
        translate(-get_tent_origin())
        {
            children();
        }
    }
}

module test_nut_holes()
{
    module test_screw_hole(size, hole, tolerance)
    {
        difference()
        {
            height = 7;
            translate([0, 0, height/2]) {cylinder(h=height, d=15, center=true, $fn = fragments_number);}
            hull()
            {
                translate([0, 0, 3]) printable_nut_hole(size, tolerance);
                translate([0, 0, -0.1]) printable_nut_hole(size, tolerance);
            }
            cylinder(h=20, d=hole, $fn = fragments_number);
        }
    }

    test_screw_hole(3, Screws_Diameter, Nut_Hole_3mm_Tolerance);
    translate([12, 0, 0]) test_screw_hole(2, Electronic_Screws_Hole_Diameter, Nut_Hole_2mm_Tolerance);
}

module test_right_top_plate()
{
    intersection()
    {
        minkowski()
        {
            hull()
            {
                height = 20;
                translate([0, -2*(switch_hole_and_space_y())]) 
                {
                    for (i = [1:6])
                    {
                        create_holes(height, 3, k[i][2], k[i][3], true);
                    }
                }

                transform_thumb()
                {
                    create_holes(height, 1, 1, [0, 0], true);
                    translate([-(switch_hole_and_space_x())/4, 0, 0])
                    {
                        create_holes(height, 1, 1, [-(switch_hole_and_space_x())/2 - (switch_hole_and_space_x()), -(switch_hole_and_space_y()) - (switch_hole_and_space_y())/2], has_additional_border = true, vertical = true);
                        create_holes(height, 1, 2, [-(switch_hole_and_space_x())/2, -(switch_hole_and_space_y())], has_additional_border = true);
                        create_holes(height, 1, 1, [0, -2*(switch_hole_and_space_y())], has_additional_border=true);
                    }
                }
            }
            cylinder(h=0.001, r=2, $fn=30);
        }

        right_top_plate();
    }
}

module nut_slot(height = 11, diameter = Nut_Slot_Diameter, force_nut_slot_z = -1, smooth_junction = false, hole_only = false)
{
    tolerance = Nut_Slot_Tolerance;
    module nut_hole_rect()
    {
        translate([-3.5, 0, 0])
        {
            intersection()
            {
                scale([6, 2, 20]) translate([0, 0, 0]) printable_nut_hole(3, tolerance, cone = false);
                size = [diameter, diameter, 40];
                translate([-size[0], -size[1]/2, 0]) cube(size);
            }
        }
    }

    module nut_hole()
    {
        nut_slot_z = force_nut_slot_z >= 0 ? force_nut_slot_z : height - 5;
        translate([0, 0, nut_slot_z])
        {
            hull()
            {
                translate([-10, 0, 0]) printable_nut_hole(3, tolerance, cone = false);
                translate([10, 0, 0]) printable_nut_hole(3, tolerance, cone = false);
                translate([0, 0, 3.1]) cube([20, 4, 0.1], true);
            }

            nut_hole_rect();
            mirror([1, 0, 0])  nut_hole_rect();
        }
    }

    if (hole_only)
    {
        nut_hole();
    }
    else
    {
        difference()
        {
            union()
            {
                cylinder(h=height, d=diameter, $fn = fragments_number);
                if (smooth_junction)
                {
                    rotate_extrude($fn = fragments_number)
                    {
                        circle_radius = 4;
                        difference()
                        {
                            points = [
                                [0, 0],
                                [circle_radius + diameter/2, 0],
                                [circle_radius + diameter/2, circle_radius],
                                [0, circle_radius],
                            ];
                                polygon(points);
                                translate([circle_radius + diameter/2, circle_radius, 0]) circle(circle_radius, $fn = fragments_number);
                        }
                    }
                }
            }

            nut_hole();
        }
    }
}

module transform_recenter_pcb()
{
    translate([1, 0, 0])
    {
        children();
    }
}

module transform_place_pcb_on_mount()
{
    translate([0, 0, Teensy_Plate_Thickness + Electronic_Pcb_Mount_Height])
    {
        children();
    }
}

module electronic_pcb(holes_only = false)
{
    // Center the pcb plate on x and y
    transform_recenter_pcb()
    {
        translate([-(electronic_hole_to_hole_dim[0]+Electronic_Screw_Mount_Diameter)/2, -(electronic_hole_to_hole_dim[1]+Electronic_Screw_Mount_Diameter)/2, Teensy_Plate_Thickness])
        {
            pcb_hole_positions = [
                [Electronic_Screw_Mount_Diameter/2, Electronic_Screw_Mount_Diameter/2, 0],
                [electronic_hole_to_hole_dim[0] + Electronic_Screw_Mount_Diameter/2, Electronic_Screw_Mount_Diameter/2, 0],
                [electronic_hole_to_hole_dim[0]+Electronic_Screw_Mount_Diameter/2, electronic_hole_to_hole_dim[1]+Electronic_Screw_Mount_Diameter/2, 0],
                [Electronic_Screw_Mount_Diameter/2, electronic_hole_to_hole_dim[1]+Electronic_Screw_Mount_Diameter/2, 0]
                    ];

            for (pcb_hole_position = pcb_hole_positions)
            {
                translate(pcb_hole_position)
                {
                    if (holes_only)
                    {
                        cylinder(h=Electronic_Pcb_Mount_Height*1.5, d=Electronic_Screws_Hole_Diameter, $fn = fragments_number);
                        translate([0, 0, -3.9]) make_pcb_case_screw_hole();
                    }
                    else
                    {
                        cylinder(h=Electronic_Pcb_Mount_Height, d=Electronic_Screw_Mount_Diameter, $fn= fragments_number);
                    }
                }
            }
        }

        transform_place_pcb_on_mount()
        {
            pcb_prototype_board_and_teensy(holes_only);
        }
    }
}

module pcb_prototype_board_and_teensy(holes_only)
{
    x_offset = -1;
    led_diameter = 5;
    led_height = 12;

    if (holes_only)
    {
        translate([x_offset, Electronic_Pcb_Dimensions[1]/2 - 4.66, Electronic_Pcb_Dimensions[2]-0.04])
        {
            teensy32(holes_only);

            hole_diameter = led_diameter + 2;
            hole_height = led_height + 1;
            translate([0, -35.56 - 12, 0])
            {
                translate([0, 0, hole_height-hole_diameter/2]) sphere(d=hole_diameter, $fn = fragments_number);
                cylinder(h=hole_height-hole_diameter/2, d=hole_diameter, $fn = fragments_number);
            }
        }

        // Board
        board_hole_dim = [Electronic_Pcb_Dimensions[0]-1, Electronic_Pcb_Dimensions[1]-Electronic_Screw_Mount_Diameter - 6, 1];
        minkowski_height = 1;
        translate([0, 0, -board_hole_dim[2]/2 - minkowski_height])
        {
            minkowski()
            {
                cube(board_hole_dim, true);
                sphere(d=2, $fn=fragments_number);
            }
        }
    }
    else
    {
        %union()
        {
            // Board
            translate([0, 0, Electronic_Pcb_Dimensions[2]/2])
            {
                cube(Electronic_Pcb_Dimensions, true);
            }

            translate([x_offset, Electronic_Pcb_Dimensions[1]/2 - 4.66, Electronic_Pcb_Dimensions[2]])
            {
                // Teensy
                teensy32();

                // Led
                translate([0, -35.56 - 12, 0])
                {
                    translate([0, 0, led_height-led_diameter/2]) sphere(d=led_diameter, $fn = fragments_number);
                    cylinder(h=led_height-led_diameter/2, d=led_diameter, $fn = fragments_number);
                }
            }
        }
    }
}

module teensy32(holes_only)
{
    // See https://www.pjrc.com/teensy/dimensions.html
    pcb = [17.78, 35.56, 1.57];
    pins_dim = [2.3, pcb[1], 4-pcb[2]];

    module make_pins()
    {
        translate([7.62, 0, -pins_dim[2]/2])
        {
            cube(pins_dim, true);

            pins_connectors_dim = [1, pcb[1]-2.54, pins_dim[2] + pcb[2] + 2.5];
            translate([0, 0, (pins_dim[2]-pins_connectors_dim[2])/2])
            {
                if (holes_only)
                {
                    minkowski()
                    {
                        cube(pins_connectors_dim, true);
                        sphere(r=1, $fn = fragments_number);
                    }
                }
                else
                {
                    cube(pins_connectors_dim, true);
                }
            }
        }
    }

    translate([0, -pcb[1]/2, pins_dim[2]])
    {
        usb = [7.5, 5, 2.5];
        if (!holes_only)
        {
            // Pcb
            translate([0, 0, pcb[2]/2])
            {
                cube(pcb, true);
            }

            // Reset button
            translate([0, pcb[1]/2 - 1.27 - 29.97, 0])
            {
                reset = [3, 2.2, 2.5];
                translate([0, 0, reset[2]/2 + pcb[2]])
                {
                    cube(reset, true);
                }
            }

            // Usb
            translate([0, pcb[1]/2 - usb[1]/2, 0])
            {
                translate([0, 0, usb[2]/2 + pcb[2]])
                {
                    cube(usb, true);
                }
            }
        }
        else
        {
            // Reset button hole
            translate([0, pcb[1]/2 - 1.27 - 29.97, 0])
            {
                reset = [3, 2.2, 2.5];
                translate([0, 0, reset[2] + pcb[2]])
                {
                    cylinder(h=20, d=3, $fn = fragments_number);
                }
            }

            // Usb hole
            usb_hole = usb + [6, 8, 3];
            translate([0, pcb[1]/2 + 2*usb[1], 0])
            {
                translate([0, 0, usb[2]/2 + pcb[2]])
                {
                    minkowski()
                    {
                        cube(usb_hole, true);
                        sphere(r=3, $fn = fragments_number);
                    }
                }
            }
        }

        // Pins
        make_pins();
        mirror([1, 0, 0]) make_pins();
    }
}

module electronic_case(top = true, bottom = true)
{
    p = teensy_case_parameters;
    nut_slot_height = 14;
    points = get_points_from_rect(p[0]);
    minkowski_height = p[4]*tan(45);

    module lateral_hole()
    {
        lateral_hole_dim = [10, 30, 15];
        translate([-30, 0, lateral_hole_dim[2]/2])
        {
            minkowski()
            {
                cube(lateral_hole_dim, center = true);
                sphere(r=3, $fn = fragments_number);
            }
        }
    }

    // Create holes
    difference()
    {
        // Bottom + Top case
        union()
        {
            // Bottom case
            if (bottom)
            {
                union()
                {
                    electronic_pcb();

                    bottom_plate(p, $fn = fragments_number);
                    for (p = points)
                    {
                        translate(p)
                        {
                            translate([0, 0, Teensy_Plate_Thickness])
                            {
                                difference()
                                {
                                    nut_slot(nut_slot_height - 1);
                                    nut_slot(force_nut_slot_z = 0, hole_only = true);
                                }
                            }
                        }
                    }
                }
            }

            // Top case
            if (top)
            {
                union()
                {
                    difference()
                    {
                        cube_size = [p[0][0], p[0][1], nut_slot_height + Teensy_Plate_Thickness - minkowski_height];
                        translate([0, 0, cube_size[2]/2])
                        {
                            minkowski()
                            {
                                cube(cube_size, true);
                                cylinder(h=minkowski_height, r1=p[3], r2=p[3]+p[4], $fn = fragments_number);
                            }
                        }

                        translate([0, 0, cube_size[2]/2])
                        {
                            minkowski()
                            {
                                cube(cube_size + [0, 0, 1], true);
                                cylinder(h=minkowski_height, r=p[3], $fn = fragments_number);
                            }
                        }
                    }

                    translate([0, 0, p[0][2]/2 + nut_slot_height + Teensy_Plate_Thickness])
                    {
                        minkowski()
                        {
                            cube(p[0], true);
                            cylinder(h=minkowski_height, r2=p[3], r1=p[3]+p[4], $fn = fragments_number);
                        }
                    }
                }
            }
        }

        // Holes for the pcb mount
        electronic_pcb(holes_only = true);

        // Holes for the link between the top and bottom cases
        for (p = points)
        {
            translate(p)
            {
                translate([0, 0, -3]) case_hole(40);
            }
        }

        // Lateral holes
        lateral_hole();
        mirror([1, 0, 0]) lateral_hole();
    }
}

module link_center_walls()
{
    p = teensy_case_parameters;
    points = get_points_from_rect(p[0]);
    minkowski_height = p[4]*tan(45);

    // Create holes
    electronic_pcb();

    difference()
    {
        cube_size = [p[0][0], p[0][1], Link_Nut_Slot_Height + Teensy_Plate_Thickness - minkowski_height];
        union()
        {
            // Wall
            translate([0, 0, cube_size[2]/2])
            {
                minkowski()
                {
                    cube(cube_size, true);
                    cylinder(h=minkowski_height, r=p[2]+p[4], $fn = fragments_number);
                }
            }

            // Top
            translate([0, 0, p[0][2]/2 + Link_Nut_Slot_Height + Teensy_Plate_Thickness])
            {
                minkowski()
                {
                    cube(p[0], true);
                    cylinder(h=minkowski_height, r2=p[2], r1=p[2]+p[4], $fn = fragments_number);
                }
            }
        }

        // Big hole
        translate([0, 0, cube_size[2]/2])
        {
            minkowski()
            {
                cube(cube_size + [0, 0, 20], true);
                cylinder(h=minkowski_height, r=p[2], $fn = fragments_number);
            }
        }
    }

    module make_mount()
    {
        height = Link_Nut_Slot_Height-1;
        cylinder(h=height, d=Nut_Slot_Diameter, $fn = fragments_number);
    }

    // Nut slots
    difference()
    {
        union()
        {
            translate(points[0]) translate([0, 0, Teensy_Plate_Thickness]) rotate([0, 0, 45]) make_mount();
            translate(points[1]) translate([0, 0, Teensy_Plate_Thickness]) rotate([0, 0, 135]) make_mount();
            translate(points[2]) translate([0, 0, Teensy_Plate_Thickness]) rotate([0, 0, 225]) make_mount();
            translate(points[3]) translate([0, 0, Teensy_Plate_Thickness]) rotate([0, 0, -45]) make_mount();
        }

        // Board clearance
        transform_recenter_pcb()
        {
            transform_place_pcb_on_mount()
            {
                clearance = [Electronic_Pcb_Dimensions[0]+1, Electronic_Pcb_Dimensions[1], 20];
                translate([0, 0, clearance[2]/2])
                {
                    cube(clearance, true);
                }
            }
        }
    }
}

module link_center_holes()
{
    p = teensy_case_parameters;

    // Holes for the pcb mount
    electronic_pcb(holes_only = true);

    module make_hole()
    {
        case_hole(40);
        make_case_screw_hole();
    }

    // Holes for the link between the top and bottom cases
    points = get_points_from_rect(p[0]);
    if (Link_Plate_Screw_Holes)
    {
        for (p = points)
        {
            translate(p)
            {
                translate([0, 0, Teensy_Plate_Thickness]) make_hole();
            }
        }
    }

    // Special holes that allows the user to remove the top plate by pushing it from bottom to top with a small screw driver
    if (Design_Mode != 3)
    {
        translate([0, 0, -0.01])
        {
            translate(points[0] + [0, 8]) case_hole(20, Screws_Diameter);
            translate(points[1] + [0, 8]) case_hole(20, Screws_Diameter);
        }
    }
}

module link_center()
{
    difference()
    {
        union()
        {
            minkowski()
            {
                p = teensy_case_parameters;
                base_cube = p[0];
                translate([0, 0, base_cube[2]/2]) cube([base_cube[0], base_cube[1], base_cube[2]], center = true);
                cylinder(h=p[1], r=p[2], $fn = fragments_number);
            }
            link_center_walls();
        }

        link_center_holes();
    }
}

module link_center_top()
{
    p = teensy_case_parameters;
    minkowski_height = p[4]*tan(45);
    cube_size = [p[0][0], p[0][1], p[0][2]];
    difference()
    {
        translate([0, 0, cube_size[2]/2 + Teensy_Plate_Thickness + Link_Nut_Slot_Height - 1])
        {
            minkowski()
            {
                cube(cube_size, true);
                cylinder(h=minkowski_height, r1=p[2]-0.5, r2=p[2], $fn = fragments_number);
            }
        }
        link_center_holes();
    }
}

module link_plate()
{
    rotate([0, 180, 0]) link_center_top();
}

module link_system(plain = false)
{
    p = teensy_case_parameters;
    base_cube = p[0];
    link_offset = [0, 39, 0];

    module left_wing()
    {
        difference()
        {
            wing_dim = [60, 30, 10];
            translate([-(wing_dim[0] + base_cube[0])/2 - p[2], 0, wing_dim[2]/2])
            {
                minkowski()
                {
                    cube(wing_dim, center = true);
                    cylinder(h=Teensy_Plate_Thickness, r1=p[4], $fn = fragments_number);
                }
            }

            remove_cube = [20, 30 + 2*p[4], 2*wing_dim[2]];
            translate([-22, 0, remove_cube[2]/2-1]) cube(remove_cube, center = true);
            translate(-link_offset) transform_link_system() left_tilted_tent(2*get_tenting_angle(), plain = true);
        }
    }

    module left_wing_hole()
    {
        minkowski_radius = 4.5;
        wing_dim = [55, 27 - Teensy_Wall_Thickness, 1];
        translate([-(wing_dim[0]/2 + base_cube[0])/2 - p[2], 0, wing_dim[2]/2 + minkowski_radius + Teensy_Plate_Thickness/2])
        {
            minkowski()
            {
                cube(wing_dim, center = true);
                sphere(r=minkowski_radius, $fn = fragments_number);
            }
        }
    }

    module link_plain()
    {
        translate(link_offset)
        {
            if (plain)
            {
                hull() link_center_walls();
            }
            else
            {
                link_center();
            }

            if (plain)
            {
                hull()
                {
                    left_wing();
                    mirror([1, 0, 0]) left_wing();
                }
            }
            else
            {
                left_wing();
                mirror([1, 0, 0]) left_wing();
            }
        }

        // Left and right tents 
        transform_link_system() left_tilted_tent(get_tenting_angle(), plain = plain);
        mirror([1, 0, 0]) transform_link_system() left_tilted_tent(get_tenting_angle(), plain = plain);
    }

    if (plain)
    {
        link_plain();
    }
    else
    {
        difference()
        {
            link_plain();

            translate(link_offset)
            {
                left_wing_hole();
                mirror([1, 0, 0]) left_wing_hole();
            }
        }
    }
}

module link_reinforcement()
{
    difference()
    {
        p = teensy_case_parameters;
        intersection()
        {
            hull()
            {
                transform_link_system() left_tilted_tent(get_tenting_angle(), plain = true);
                mirror([1, 0, 0]) transform_link_system() left_tilted_tent(get_tenting_angle(), plain = true);
            }

            minkowski()
            {
                difference()
                {
                    link_reinforcement_size = [150, 90, 10-Teensy_Plate_Thickness];
                    translate([0, 0, link_reinforcement_size[2]/2]) cube(link_reinforcement_size, center = true);
                    translate([0, -110, -1]) cylinder(h=link_reinforcement_size[2]+2, r=Link_Reinforcement_Radius + Tilt_Angle, $fn = $preview ? 0 : 400);
                }
                cylinder(h=Teensy_Plate_Thickness, r1=p[4], $fn = fragments_number);
            }
        }

        link_system(true);
    }
}

echo("Tenting Angle", get_tenting_angle());

// Show helpers
module show_point(p)
{
    %translate(p) cylinder(h=100, r=1, $fn=60);
}

if (Show_Plate_Helpers)
{
    show_point(get_kaladrius_origin());
    show_point(get_tent_origin());
    for (i = [0:7])
    {
        show_point(k[i][3]);
    }
    show_point(get_thumb_anchor());
    show_point(get_thumb_origin());
}

if (Show_Plate_Keycaps)
{
    %left_keycaps();
}

if (Design_Mode == 0)
{
    if (Plate_Test_Finger_Positions)
    {
        test_right_top_plate();
    }
    else
    {
        right_top_plate();
    }
}
else if (Design_Mode == 1)
{
    left_case(printable = true);
}
else if (Design_Mode == 2)
{
    union()
    {
        link_system();

        if (Link_Reinforcement)
        {
            link_reinforcement();
        }
    }
}
else if (Design_Mode == 3)
{
    link_plate();
}
else if (Design_Mode == 4)
{
    union()
    {
        link_system();

        if (Link_Reinforcement)
        {
            link_reinforcement();
        }
    }

    transform_link_system() transform_tilt() left_case(printable = false);
    mirror([1, 0, 0]) transform_link_system() transform_tilt() left_case(printable = false);
}
else if (Design_Mode == 5)
{
    translate([0, 0, 2*(Case_Height + Case_Shell_Thickness) + Plate_Height + Case_Top_Plate_Offset])
    {
        mirror([0,0,1]) right_top_plate();
    }
    left_case(printable = true);
    left_tent(get_tenting_angle(), printable = false);
}
else if (Design_Mode == 6)
{
    test_nut_holes();
}
