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

use <Boxes.scad>
use <BezierScad.scad>
use <MCAD/nuts_and_bolts.scad>

fragments_number = 40; // Use 0 for debugging, 60 for final rendering
switch_hole_width = 14;
switch_hole_tolerance = -0.1;
switch_spacing = 4.8;

// For comparison, Ergodox settings are (in mm) :
//      offset_finger_middle = -2;
//      offset_finger_ring = 2;
//      offset_finger_pinky = 3;
// My personal settings :
offset_finger_middle = 0;
offset_finger_ring = 0;
offset_finger_pinky = 5;

thumb_x = -switch_hole_width;
thumb_y = switch_hole_width+0.5;
thumb_angle = -21;
case_shell_size = 3;
case_height = 9;
case_outer_border = 8;
plate_height = 3;
plate_additional_border_height = 1;
screws_diameter = 3.5;
screws_mount_height = 17;
screw_mount_diameter = 10;
nut_3_tolerance = 0.25;
nut_2_tolerance = 0.3;
electronic_screw_mount_diameter = 6;
electronic_screws_hole_diameter = 2.7;
electronic_pcb_dim = [40, 60, 1.54]; // Real size of the pcb plate in mm
electronic_hole_to_hole_dim = [electronic_pcb_dim[0] - 4, electronic_pcb_dim[1] - 4, electronic_pcb_dim[2]];
electronic_screw_mount_height = 2*(11-electronic_pcb_dim[2])-10.5;
teensy_reset_button_y = 35.5; // distance from the front of the plate to the teensy reset button in mm
teensy_plate_size = [60, 63];
teensy_plate_thickness = 3;
teensy_wall_thickness = 3;
teensy_case_roundness = 9/2;
teensy_case_parameters = get_box_parameters(teensy_plate_size, teensy_case_roundness, teensy_plate_thickness, teensy_wall_thickness);
pcb_case_pos = [84, offset_finger_pinky-71];
tent_size = [46, 66, 2];
switch_hole_height = screws_mount_height + 5;

point_pinky_last = [get_kaladrius_origin()[0] + case_outer_border, get_kaladrius_origin()[1]];
point_pinky = [point_pinky_last[0] + switch_hole_width + switch_spacing, point_pinky_last[1]];
point_ring = [point_pinky[0] +  switch_hole_width + switch_spacing, point_pinky[1] + offset_finger_pinky];
point_middle = [point_ring[0] + switch_hole_width + switch_spacing, point_ring[1] + offset_finger_ring];
point_index3 = [point_middle[0] + switch_hole_width + switch_spacing, point_middle[1] + offset_finger_middle];
point_index2 = [point_index3[0] + switch_hole_width + switch_spacing, point_index3[1]];
point_index1 = [point_index2[0] + switch_hole_width + switch_spacing, point_index2[1]];

function switches(color, rows, cols, origin, vertical=false) = [color, rows, cols, origin, vertical];
k = [

    // Fingers
    switches("red", 5, 1, point_pinky_last),
    switches("magenta", 5, 1, point_pinky),
    switches("green", 5, 1, point_ring),
    switches("lightGreen", 4, 1, point_middle),
    switches("blue", 4, 1, point_index3),
    switches("lightBlue", 4, 1, point_index2),
    switches("cyan", 3, 1, point_index1),

    // Thumbs
    switches("red", 1, 1, [0, 0]),
    switches("green", 1, 1, [-(switch_hole_width + switch_spacing)/2 - (switch_hole_width + switch_spacing), -(switch_hole_width + switch_spacing) - (switch_hole_width + switch_spacing)/2], true),
    switches("blue", 1, 2, [-(switch_hole_width + switch_spacing)/2, -(switch_hole_width + switch_spacing)]),
    switches("lightBlue", 1, 1, [0, -2*(switch_hole_width + switch_spacing)]),
];

hole_positions = [
    k[0][3],
    k[3][3] + [(switch_hole_width + 2*switch_spacing)/2, switch_spacing/2],
    k[6][3] + [(switch_hole_width + 2*switch_spacing), 0],
    k[6][3]+ [(switch_hole_width + 2*switch_spacing), -k[6][1]*switch_hole_width - (k[6][1]+1)*switch_spacing],
    k[0][3] - [0, k[0][1]*switch_hole_width + (k[0][1]+1)*switch_spacing],

    // Thumb
    [-1.75*(switch_hole_width + switch_spacing) + 3*switch_hole_width + 4*switch_spacing, 0],
    [-1.75*(switch_hole_width + switch_spacing) + 3*switch_hole_width + 4*switch_spacing,  -3*switch_hole_width - 4*switch_spacing],
    [-1.75*(switch_hole_width + switch_spacing), -3*switch_hole_width - 4*switch_spacing],
];

function get_kaladrius_origin() = [0, 0, 0];
function get_thumb_anchor() = k[6][3] - [0, 3*(switch_hole_width+switch_spacing) + switch_spacing];
function get_thumb_origin() = get_thumb_anchor() + [thumb_x, -thumb_y];
function get_tent_bounding_box() = [tent_size[0] + electronic_screw_mount_diameter, tent_size[1] + electronic_screw_mount_diameter, electronic_screw_mount_height];
function get_pcb_case_origin() = get_kaladrius_origin() + pcb_case_pos;
function get_tenting_angle() = atan(electronic_screw_mount_height/(get_kaladrius_origin()[0]-get_pcb_case_origin()[0]));

module printable_nut_hole(size, tolerance, cone=true)
{
    if (cone)
    {
        hull()
        {
            scale([0.2,0.2,2]) nutHole(size = size, tolerance = tolerance);
            nutHole(size = size, tolerance = tolerance);
        }
    }
    else
    {
        nutHole(size = size, tolerance = tolerance);
    }
}

module rotate_hull_around_y(angle, steps = 30)
{
    hull()
    {
        angle_step = angle/steps;
        for (i = [0:steps])
        {
            rotate([0, i*angle_step, 0]) children();
        }
    }
}

module case_hole(height, diameter=screws_diameter)
{
    cylinder(height-2, d=diameter, $fn = fragments_number);
    translate([0, 0, height-2])
    {
        cylinder(2, d=diameter, $fn = fragments_number);
    }
}

module transform_pcb_case_to_center()
{
    centered = [-electronic_hole_to_hole_dim[0] - electronic_screw_mount_diameter, -electronic_hole_to_hole_dim[1] - electronic_screw_mount_diameter, 0]/2;
    translate(centered)
    {
        children();
    }
}

module transform_thumb()
{
    translate(get_thumb_origin())
    {
        rotate([0, 0, thumb_angle])
        {
            children();
        }
    }
}

module holes(offset=0, height = switch_hole_height, has_additional_border = true)
{
    module make_keyboard_hole(i)
    {
        color(k[i][0]) create_holes(height, k[i][1], k[i][2], k[i][3], offset, has_additional_border, k[i][4]);
    }

    // Fingers
    for (index = [0:6])
    {
        make_keyboard_hole(index);
    }

    // Thumbs
    transform_thumb()
    {
        make_keyboard_hole(7);
        translate([-(switch_hole_width+switch_spacing)/4, 0, 0])
        {
            for (index = [8:10])
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

module case_holes(offset=0, height=switch_hole_height, diameter=screws_diameter)
{
    color("red")
    {
        translate([0, 0, -1])
        {
            for (index = [0:4])
            {
                transform_hole(index) case_hole(height, diameter);
            }

            transform_thumb()
            {
                for (index = [5:7])
                {
                    transform_hole(index) case_hole(height, diameter);
                }
            }
        }
    }
}

module screw_mounts()
{
    height = case_shell_size + case_height;
    for (index = [0:4])
    {
        transform_hole(index) 
        {
            hull()
            {
                case_hole(height-3, screw_mount_diameter);
                case_hole(height, screw_mount_diameter/2);
            }
        }
    }

    transform_thumb()
    {
        for (index = [5:7])
        {
            transform_hole(index)
            {
                hull()
                {
                    case_hole(height-3, screw_mount_diameter);
                    case_hole(height, screw_mount_diameter/2);
                }
            }
        }
    }
}

module create_cell(height)
{
    linear_extrude(height = height)
    {
        points = [
            [0, 0], // top left
            [0, -switch_hole_width], // bottom left
            [switch_hole_width, -switch_hole_width], // bottom right
            [switch_hole_width, 0] // top right
        ];

        polygon(points);
    }
}

module create_hole(height, offset, has_additional_border = true, vertical = false)
{
    additional_border_width = 0.5;
    clip_width = 6;
    roundness = 0.5;
    x = switch_hole_width + 2*offset - 2*roundness + 2*additional_border_width + switch_hole_tolerance;
    y = switch_hole_width + 2*offset - 2*roundness + switch_hole_tolerance;
    translate([switch_spacing + (switch_hole_width-((vertical?y:x)+2*roundness))/2, -switch_spacing - (switch_hole_width-((vertical?x:y)+2*roundness))/2, 0])
    {
        translate([roundness, -roundness, 0]) // minkowski compensation
        {
            translate([vertical?y/2:x/2, vertical?-x/2:-y/2, 0]) // move top left to [0,0]
            {
                rotate([0, 0, vertical?90:0])
                {
                    // External hole
                    ext_hole = offset+1;
                    translate([0, 0, height/2 + (has_additional_border?plate_additional_border_height:0)]) // The cube is centered, we want to move up the bottom of the cube
                    {
                        minkowski()
                        {
                            cube([switch_hole_width + 2*ext_hole - 2*roundness, switch_hole_width + 2*ext_hole - 2*roundness, height], center=true);
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
        for (col = [0:columns-1])
        {
            col_offset = col*(switch_hole_width + switch_spacing);
            for (row = [0:rows-1])
            {
                translate([col_offset, -row*(switch_hole_width+switch_spacing), 0])
                {
                    color("lightGreen") translate([switch_spacing, -switch_spacing, 0]) cube([switch_hole_width, switch_spacing, height]);
                    translate([switch_spacing, -switch_spacing, 0]) create_cell(height = height);
                }
            }
            color("lightGreen") translate([switch_spacing + col_offset, -rows*(switch_hole_width+switch_spacing)-switch_spacing, 0]) cube([switch_hole_width, switch_spacing, height]);
            color("lightGreen") translate([col_offset, -rows*(switch_hole_width+switch_spacing) - switch_spacing, 0]) cube([switch_spacing, rows*(switch_hole_width+switch_spacing) + switch_spacing, height]);
            color("lightGreen") translate([switch_hole_width + switch_spacing + col_offset, -rows*(switch_hole_width+switch_spacing) - switch_spacing, 0]) cube([switch_spacing, rows*(switch_hole_width+switch_spacing) + switch_spacing, height]);
        }
    }
}

module create_holes(height, rows, columns, origin, offset, has_additional_border = true, vertical = false)
{
    translate(origin)
    {
        for (col = [0:columns-1])
        {
            col_offset = col*(switch_hole_width + switch_spacing);
            for (row = [0:rows-1])
            {
                translate([col_offset, -row*(switch_hole_width+switch_spacing), 0])
                {
                    create_hole(height, offset, has_additional_border, vertical);
                }
            }
        }
    }
}

module plate(total_height = plate_height, chamfer = false)
{
    minkowski()
    { 
        basic_height = total_height/3;
        minkowski_height = 2*basic_height;
        union()
        {
            // Fingers plate
            hull()
            {
                top_point_offset = [(switch_hole_width+2*switch_spacing)/2, switch_spacing];
                top_control_points =  [
                    k[0][3],
                    k[1][3] + top_point_offset,
                    k[2][3] + top_point_offset,
                    k[3][3] + top_point_offset,
                    k[4][3] + top_point_offset,
                    k[5][3] + top_point_offset,
                    k[6][3] + [switch_hole_width + 2*switch_spacing, 0],
                ];

                bottom_control_points =  [
                    k[0][3] + [0, -k[0][1]*(switch_spacing+switch_hole_width) - switch_spacing],
                    k[1][3] + [switch_spacing + switch_hole_width/2, -k[1][1]*(switch_spacing+switch_hole_width) - 2*switch_spacing],
                    k[2][3] + [switch_spacing + switch_hole_width/2, -k[2][1]*(switch_spacing+switch_hole_width) - 2*switch_spacing],
                    k[3][3] + [switch_spacing + switch_hole_width/2, -k[3][1]*(switch_spacing+switch_hole_width) - 2*switch_spacing],
                    k[4][3] + [switch_spacing + switch_hole_width/2, -k[4][1]*(switch_spacing+switch_hole_width) - 2*switch_spacing],
                    k[5][3] + [switch_spacing + switch_hole_width/2, -k[5][1]*(switch_spacing+switch_hole_width) - 2*switch_spacing],
                    k[6][3]  + [2*switch_spacing + switch_hole_width, -k[6][1]*(switch_spacing+switch_hole_width) - switch_spacing],
                ];

                difference()
                {
                    union()
                    {
                        for (i = [0:6])
                        {
                            create_cells(basic_height, k[i][1], k[i][2], k[i][3]);
                        }
                    }

                    BezWall(top_control_points, width = 20, height = basic_height, steps = 64, centered = true, showCtlR = false);
                    BezWall(bottom_control_points, width = 20, height = basic_height, steps = 64, centered = true, showCtlR = false);
                }

                BezWall(top_control_points, width = 1, height = basic_height, steps = 64, centered = true, showCtlR = false);
                BezWall(bottom_control_points, width = 1, height = basic_height, steps = 64, centered = true, showCtlR = false);
            }

            // Thumb plate
            transform_thumb()
            {
                translate([-(switch_hole_width+switch_spacing)/4, 0, 0])
                {
                    translate([-1.5*(switch_hole_width+ switch_spacing), 0, 0]) create_cells(basic_height, 3, 3, [0,0]);
                }
            }
        }
        r1 = case_outer_border;
        r2 = chamfer? r1 - minkowski_height : r1;
        cylinder(r1=r1, r2=r2, h=minkowski_height, $fn = fragments_number);
    }
}

module right_top_plate()
{
    translate([0, 0, case_height + case_shell_size])
    {
        difference()
        {
            union()
            {
                plate(chamfer=true);

                color("black")
                {
                    mark_height = plate_height + 0.5;
                    translate(get_thumb_anchor() + [0, -thumb_y]) cube([thumb_x, 2, mark_height]);
                    translate(get_thumb_anchor() + [0, -thumb_y]) cube([2, thumb_y, mark_height]);
                }
            }
            holes();
            case_holes();
        }
    }
}

module plate_supports()
{
    support_height = case_shell_size + case_height;
    module create_support(row)
    {
        hull()
        {
            support_diameter = 3.5;
            y = -row*(switch_spacing + switch_hole_width);
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

    translate(k[2][3] + [3*switch_spacing/2 + switch_hole_width, 0])
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
        hull()
        {
            size = 6;
            translate([0, -size]) cylinder(h=support_height-3, d=3.5, $fn=30);
            translate([0, -size]) cylinder(h=support_height, d=1, $fn=30);
            translate([0, size]) cylinder(h=support_height-3, d=3.5, $fn=30);
            translate([0, size]) cylinder(h=support_height, d=1, $fn=30);
        }
    }
}

module left_case()
{
    difference()
    {
        union()
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
                        cylinder(r1=small_radius, r2=case_shell_size,  h=case_shell_size, $fn = fragments_number);
                    }

                    // Middle
                    translate([0, 0, case_shell_size])
                    {
                        minkowski()
                        {
                            plate();
                            cylinder(r=case_shell_size,  h=case_height-case_shell_size, $fn = fragments_number);
                        }
                    }

                    // Top
                    translate([0, 0, case_height])
                    {
                        minkowski()
                        {
                            plate();
                            cylinder(r2=small_radius, r1=case_shell_size,  h=case_shell_size, $fn = fragments_number);
                        }
                    }
                }

                translate([0, 0, case_shell_size]) scale([1, 1, 8]) plate();
            }

            // Add all bolt mounts
            screw_mounts();

            plate_supports();
        }

        // Top plate hole without chamfer
        translate([0, 0, case_height + case_shell_size - 1]) scale([1, 1, 8]) plate();

        // Case holes
        translate([0, 0, -20]) case_holes(height = 200);

        // Nut holes
        for (index = [0:4])
        {
            transform_hole(index) make_case_screw_hole();
        }

        // Nut holes
        transform_thumb()
        {
            for (index = [5:7])
            {
                transform_hole(index) make_case_screw_hole();
            }
        }

        // Holes to connect the tent system
        mini_thumb_holes();

        // Special holes that allows the user to remove the top plate by pushing it from bottom to top with a small screw driver
        left_middle_point = (hole_positions[0] + hole_positions[4])/2;
        left_middle_up_point = (hole_positions[0] + left_middle_point)/2;
        left_middle_down_point = (hole_positions[4] + left_middle_point)/2;
        translate([0, 0, -0.01]) translate(left_middle_up_point) case_hole(20, screws_diameter);
        translate([0, 0, -0.01]) translate(left_middle_down_point) case_hole(20, screws_diameter);
    }
}

module make_case_screw_hole()
{
    hull()
    {
        translate([0,0, 2]) printable_nut_hole(3, nut_3_tolerance);
        translate([0,0,-30]) printable_nut_hole(3, nut_3_tolerance, false);
    }
}

module make_pcb_case_screw_hole(height = 2)
{
    hull()
    {
        translate([0,0, height]) printable_nut_hole(2, nut_2_tolerance);
        translate([0,0,-height]) printable_nut_hole(2, nut_2_tolerance, false);
    }
}

module pcb_case(mount_height = electronic_screw_mount_height, holes_only = false)
{
    module mount(h, d1, d2, holes_only)
    {
        if (holes_only)
        {
            cylinder(h=h*1.5, d=d1, $fn = fragments_number);
        }
        else
        {
            cylinder(h=h, d=d2, $fn= fragments_number);
        }
    }

    pcb_hole_positions = [
        [electronic_screw_mount_diameter/2, electronic_screw_mount_diameter/2, 0],
        [electronic_hole_to_hole_dim[0] + electronic_screw_mount_diameter/2, electronic_screw_mount_diameter/2, 0],
        [electronic_hole_to_hole_dim[0]+electronic_screw_mount_diameter/2, electronic_hole_to_hole_dim[1]+electronic_screw_mount_diameter/2, 0],
        [electronic_screw_mount_diameter/2, electronic_hole_to_hole_dim[1]+electronic_screw_mount_diameter/2, 0]
            ];

    for (index = [0:3])
    {
        translate(pcb_hole_positions[index])
        {
            mount(mount_height-electronic_pcb_dim[2]-0.1, electronic_screws_hole_diameter, electronic_screw_mount_diameter, holes_only);

            if (holes_only)
            {
                make_pcb_case_screw_hole();
            }
        }
    }

    if (holes_only)
    {
        // Hole for usb port
        roundness = 3;
        y_offset = -6;
        z_offset = 10;
        hole_port_width = electronic_hole_to_hole_dim[0] - electronic_screw_mount_diameter - 4;
        hole_ports_dim = [hole_port_width-2*roundness, 20-2*roundness, mount_height-2*roundness+0.01];
        translate([(electronic_hole_to_hole_dim[0] - hole_port_width + electronic_screw_mount_diameter)/2,  electronic_hole_to_hole_dim[1] + y_offset, z_offset])
        {
            translate([roundness, roundness, roundness])
            {
                minkowski()
                {
                    cube(hole_ports_dim);
                    sphere(r=roundness, $fn = fragments_number);
                }
            }
        }

        /*
        // Hole to access the Teensy reset button
        teensy_hole_rect = [electronic_hole_to_hole_dim[0]/1.5, 1, 4];
        minkowski()
        {
        translate([electronic_pcb_dim[0]/2, electronic_pcb_dim[1]-teensy_reset_button_y, 0]) cube(teensy_hole_rect, center=true);
        cylinder(h=1, r=1.5, $fn = fragments_number);
        }
         */
    }
    else
    {
        minkowski()
        {
            translate([electronic_screw_mount_diameter/2, electronic_screw_mount_diameter/2, 0]) cube([electronic_hole_to_hole_dim[0], electronic_hole_to_hole_dim[1], electronic_hole_to_hole_dim[2]/2]);
            cylinder(h=electronic_hole_to_hole_dim[2]/2, r=electronic_screw_mount_diameter/2, $fn = fragments_number);
        }
    }
}

module mini_thumb_holes()
{
    module make_hole()
    {
        translate([0,0,-electronic_screw_mount_height])
        {
            make_pcb_case_screw_hole();
            cylinder(h=2*electronic_screw_mount_height, d=electronic_screws_hole_diameter, $fn = fragments_number);
        }
    }

    transform_thumb()
    {
        factor = 0.75;
        original_width = 3*switch_hole_width + 4*switch_spacing;
        translate([(original_width-factor*original_width)/2, -(original_width-factor*original_width)/2, 0])
        {
            translate([-(switch_hole_width+switch_spacing)/4, 0, 0])
            {
                translate([-1.5*(switch_hole_width+ switch_spacing), 0, 0])
                {
                    translate([factor*original_width, 0]) make_hole();
                    translate([factor*original_width, -factor*original_width]) make_hole();
                }
            }
        }
    }
}

module left_tent(printable = true, holes_only = false)
{
    module mini_thumb(factor)
    {
        original_width = 3*switch_hole_width + 4*switch_spacing;
        translate([(original_width-factor*original_width)/2, -(original_width-factor*original_width)/2, 0])
        {
            translate([-(switch_hole_width+switch_spacing)/4, 0, 0])
            {
                translate([-1.5*(switch_hole_width+ switch_spacing), 0, 0])
                {
                    scale([factor, factor,1]) create_cells(0.0001, 3, 3, [0,0]);
                }
            }
        }
    }

    mini_thumb_scale = 0.8;
    module tent_holes()
    {
        rotate([0, 10, 0]) rotate_hull_around_y(angle=-(get_tenting_angle()+20))
        {
            transform_thumb()
            {
                minkowski()
                {
                    new_scale = 0.65;
                    mini_thumb(mini_thumb_scale*new_scale);
                    cylinder(r=5*new_scale,  h=0.001, $fn = fragments_number);
                }
            }
        }

        mini_thumb_holes();
    }

    rotate([0, printable?get_tenting_angle():0, 0])
    {
        if (!holes_only)
        {
            difference()
            {
                union()
                {
                    rotate_hull_around_y(angle=-get_tenting_angle())
                    {
                        translate(get_pcb_case_origin())
                        {
                            minkowski()
                            {
                                cube([get_tent_bounding_box()[0], get_tent_bounding_box()[1], 0.0001]);
                                cylinder(r=5,  h=0.001, $fn = fragments_number);
                            }
                        }
                    }

                    rotate_hull_around_y(angle=-get_tenting_angle())
                    {
                        transform_thumb()
                        {
                            minkowski()
                            {
                                mini_thumb(mini_thumb_scale);
                                cylinder(r=5,  h=0.001, $fn = fragments_number);
                            }
                        }
                    }
                }

                tent_holes();
            }
        }
        else
        {
            tent_holes();
        }
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
    translate([switch_spacing + (base_size-((vertical?y:x)+2*roundness))/2, -switch_spacing - (base_size-((vertical?x:y)+2*roundness))/2, 0])
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
            col_offset = col*(switch_hole_width + switch_spacing);
            for (row = [0:rows-1])
            {
                translate([col_offset, -row*(switch_hole_width+switch_spacing), 0])
                {
                    create_keycap(vertical, horizontal_stretch);
                }
            }
        }
    }
}

module left_keycaps()
{
    translate([0, 0, case_height + case_shell_size + plate_height])
    {
        for (i = [0:6])
        {
            create_keycaps(k[i][1], k[i][2], k[i][3]);
        }

        transform_thumb()
        {
            create_keycaps(1, 1, [0, 0], horizontal_stretch=1.5);
            translate([-(switch_hole_width+switch_spacing)/4, 0, 0])
            {
                create_keycaps(1, 1, [-(switch_hole_width + switch_spacing)/2 - (switch_hole_width + switch_spacing), -(switch_hole_width + switch_spacing) - (switch_hole_width + switch_spacing)/2], vertical = true, horizontal_stretch = 2);
                create_keycaps(1, 2, [-(switch_hole_width + switch_spacing)/2, -(switch_hole_width + switch_spacing)]);
                create_keycaps(1, 1, [0, -2*(switch_hole_width + switch_spacing)], horizontal_stretch = 2);
            }
        }
    }
}

// Kaladrius link system
module transform_link_system()
{
    translate([-95, 0, 0])
    {
        translate(-get_pcb_case_origin())
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
            translate([0, 0, 2.5]) {cylinder(h=5, d=15, center=true, $fn = fragments_number);}
            translate([0, 0, -0.001]) printable_nut_hole(size, tolerance);
            cylinder(h=20, d=hole, $fn = fragments_number);
        }
    }

    test_screw_hole(3, screws_diameter, nut_3_tolerance);
    translate([12, 0, 0]) test_screw_hole(2, electronic_screws_hole_diameter, nut_2_tolerance);
}

module test_right_top_plate()
{
    intersection()
    {
        minkowski()
        {
            hull()
            {
                offset = 0;
                height = 20;
                translate([0, -2*(switch_spacing+switch_hole_width)]) 
                {
                    for (i = [1:5])
                    {
                        create_holes(height, 2, k[i][2], k[i][3], offset, true);
                    }
                }

                transform_thumb()
                {
                    create_holes(height, 1, 1, [0, 0], offset, true);
                    translate([-(switch_hole_width+switch_spacing)/4, 0, 0])
                    {
                        create_holes(height, 1, 1, [-(switch_hole_width + switch_spacing)/2 - (switch_hole_width + switch_spacing), -(switch_hole_width + switch_spacing) - (switch_hole_width + switch_spacing)/2], offset, has_additional_border = true, vertical = true);
                        create_holes(height, 1, 2, [-(switch_hole_width + switch_spacing)/2, -(switch_hole_width + switch_spacing)], offset, has_additional_border = true);
                        create_holes(height, 1, 1, [0, -2*(switch_hole_width + switch_spacing)], offset, has_additional_border=true);
                    }
                }
            }
            cylinder(h=0.001, r=2, $fn=30);
        }

        right_top_plate();
    }
}

module nut_slot(height = 11, diameter = 9, force_nut_slot_z = -1, smooth_junction = false, hole_only = false)
{
    tolerance = 0.0;
    module nut_hole_rect()
    {
        translate([-3.5, 0, 0])
        {
            intersection()
            {
                scale([2, 2, 20]) translate([0, 0, 0]) printable_nut_hole(3, tolerance, cone = false);
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
                translate([0, 0, 3]) cube([20, 0.1, 0.1], true);
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

            /*
               translate([0, 0, -1]) case_hole(height+2);

               hull()
               {
               case_hole(5);
               case_hole(4, 6.2);
               }
             */
        }
    }
}

module electronic_pcb(plate_thickness, holes_only = false)
{
    // Center the pcb plate on x and y
    x_offset_to_recenter = 1;
    mount_height = 3;
    translate([x_offset_to_recenter, 0, 0])
    {
        translate([-(electronic_hole_to_hole_dim[0]+electronic_screw_mount_diameter)/2, -(electronic_hole_to_hole_dim[1]+electronic_screw_mount_diameter)/2, plate_thickness])
        {
            pcb_hole_positions = [
                [electronic_screw_mount_diameter/2, electronic_screw_mount_diameter/2, 0],
                [electronic_hole_to_hole_dim[0] + electronic_screw_mount_diameter/2, electronic_screw_mount_diameter/2, 0],
                [electronic_hole_to_hole_dim[0]+electronic_screw_mount_diameter/2, electronic_hole_to_hole_dim[1]+electronic_screw_mount_diameter/2, 0],
                [electronic_screw_mount_diameter/2, electronic_hole_to_hole_dim[1]+electronic_screw_mount_diameter/2, 0]
                    ];

            for (pcb_hole_position = pcb_hole_positions)
            {
                translate(pcb_hole_position)
                {
                    if (holes_only)
                    {
                        cylinder(h=mount_height*1.5, d=electronic_screws_hole_diameter, $fn = fragments_number);
                        translate([0, 0, -3.9]) make_pcb_case_screw_hole();
                    }
                    else
                    {
                        cylinder(h=mount_height, d=electronic_screw_mount_diameter, $fn= fragments_number);
                    }
                }
            }
        }

        translate([0, 0, plate_thickness + mount_height])
        {
            pcb_prototype_board_and_teensy(holes_only);
        }
    }
}

module pcb_prototype_board_and_teensy(holes_only)
{
    // Mine is an Elegoo (40 x 60 mm)
    board_dim = [40, 60, 1.5];
    x_offset = -1;
    led_diameter = 5;
    led_height = 12;

    if (holes_only)
    {
        translate([x_offset, board_dim[1]/2 - 4.66, board_dim[2]])
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
    }
    else
    {
        %union()
        {
            // Board
            translate([0, 0, board_dim[2]/2])
            {
                cube(board_dim, true);
            }

            translate([x_offset, board_dim[1]/2 - 4.66, board_dim[2]])
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

            pins_connectors_dim = [1, pcb[1]-2.54, 10-pcb[2]];
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
                    electronic_pcb(teensy_plate_thickness);

                    bottom_plate(p, $fn = fragments_number);
                    for (p = points)
                    {
                        translate(p)
                        {
                            translate([0, 0, teensy_plate_thickness])
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
                        cube_size = [p[0][0], p[0][1], nut_slot_height + teensy_plate_thickness - minkowski_height];
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

                    translate([0, 0, p[0][2]/2 + nut_slot_height + teensy_plate_thickness])
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
        electronic_pcb(teensy_plate_thickness, holes_only = true);

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

echo("Tenting Angle", abs(get_tenting_angle()));

// Show helpers
module show_point(p)
{
    %translate(p) cylinder(h=100, r=1, $fn=60);
}

*union()
{
    show_point(get_kaladrius_origin());
    for (i = [0:6])
    {
        show_point(k[i][3]);
    }
    show_point(get_pcb_case_origin());
    show_point(get_thumb_anchor());
    show_point(get_thumb_origin());
}

*plate(total_height=1, chamfer=false);
*holes();

*right_top_plate();
*%left_keycaps();
*left_case();
%transform_link_system() left_tent();
*test_nut_holes();

mirror([1, 0, 0])
{
    *right_top_plate();
    *test_right_top_plate();
    %transform_link_system() left_tent();
    *%left_keycaps();
}

*electronic_case(bottom = false);
*electronic_case(top = false);

module link_system()
{
    p = teensy_case_parameters;
    base_cube = p[0];
    height = base_cube[2];

    cross_arm_width = 5;
    cross_width = 60;
    cross_roundness = 10;
    module cross_part()
    {
        cube([cross_width/2, cross_arm_width/2, height]);
        cube([cross_arm_width/2, cross_width/2, height]);
        radius = cross_roundness;
        difference()
        {
            translate([cross_arm_width/2, cross_arm_width/2, 0])
            {
                cube([radius, radius, height]);
            }

            translate([radius + cross_arm_width/2, radius + cross_arm_width/2, -0.01])
            {
                cylinder(h=height*2, r=radius, $fn = fragments_number);
            }
        }
    }

    module cross()
    {
        cross_part();
        mirror([1, 0, 0]) cross_part();
        mirror([1, 1, 0]) cross_part();
        mirror([0, 1, 0]) cross_part();
    }

    module left_wing()
    {
        left_arm = [40, cross_arm_width, height];

        module left_extension()
        {
            translate([-left_arm[0], 0, 0])
            {
                cube([10, 30, height]);

                translate([10, left_arm[1]/2, 0])
                {
                    difference()
                    {
                        radius = 10;
                        cube([radius, radius, height]);

                        translate([radius, radius, 0])
                        {
                            cylinder(h=height, r=radius, $fn = fragments_number);
                        }
                    }
                }
            }
        }

        union()
        {
            translate([-base_cube[0]/2, 0, 0])
            {
                translate([0, -base_cube[1]/2, 0])
                {
                    cube([base_cube[0]/2, base_cube[1], height], center = false);
                }

                translate([-cross_width/2, 0, 0])
                {
                    cross();
                }
            }
        }

        module round_corner()
        {
            translate([-base_cube[0]/2, left_arm[1]/2, 0])
            {
                difference()
                {
                    translate([-10, 0, 0]) cube([10, 10, height]);

                    union()
                    {
                        radius = cross_roundness;
                        translate([-radius, radius, 0])
                        {
                            cylinder(h=height, r=radius, $fn = fragments_number);
                        }

                        translate([-left_arm[0], 0, 0])
                        {
                            cube([radius, radius, height]);
                        }

                        radius2 = 20;
                        translate([-left_arm[0]/2 - radius2/2, left_arm[1], 0])
                        {
                            cube([radius2, radius2, height]);
                        }
                    }
                }
            }
        }

        round_corner();
        mirror([0, 1, 0]) round_corner();
    }

    translate([0, 35, 0])
    {
        minkowski()
        {
            union()
            {
                left_wing();
                mirror([1, 0, 0]) left_wing();
            }
            cylinder(h=p[1], r1=p[3], r2=p[3]);
        }
    }
}

link_system();
*translate([80, 0, 22]) rotate([0, 180, 0]) electronic_case(bottom = false);
