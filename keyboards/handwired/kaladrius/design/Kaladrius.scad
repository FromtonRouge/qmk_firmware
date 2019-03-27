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

fragments_number = 60; // Use 0 for debugging, 60 for final rendering
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
function get_pcb_case_size() = [electronic_hole_to_hole_dim[0] + electronic_screw_mount_diameter, electronic_hole_to_hole_dim[1] + electronic_screw_mount_diameter, electronic_screw_mount_height];
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

module transform_pcb_case()
{
    translate([get_pcb_case_origin()[0], get_pcb_case_origin()[1], -electronic_screw_mount_height])  children();
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
        rotate([0, 0, 0])
        {
            translate(-get_pcb_case_origin())
            {
                children();
            }
        }
    }
}

module left_link()
{
    transform_link_system() left_tent();
    *difference()
    {
        small_radius = 1;
        roundness = 5;
        link_dim = [80, 50, 14];
        translate([-link_dim[0]/2, -20, link_dim[2]/2])
        {
            minkowski()
            {
                difference()
                {
                    union()
                    {
                        top_control_points =  [
                            [link_dim[0]/2, link_dim[1]/2],
                            [0, link_dim[1]/2],
                            [0, link_dim[1]/2 + 20],
                            [4, link_dim[1]/2+35],
                        ];

                        cube(link_dim, center=true);

                        difference()
                        {
                            union()
                            {
                                translate([0, 4, 0]) cube(link_dim, center=true);
                                translate([0, 40, 0])
                                {
                                    cube([45, 30, link_dim[2]], center=true);
                                }
                            }

                            translate([0, 0, -link_dim[2]/2])
                            {
                                BezWall(top_control_points, width = 15, height = link_dim[2], steps = 64, centered = false, showCtlR = false);
                            }
                        }
                    }

                    bottom_control_points =  [
                        [link_dim[0]/2, 0],
                        [10, 0],
                        [0, -link_dim[1]/40],
                        [-22, -link_dim[1]/2],
                    ];

                    translate([0, 0, -link_dim[2]/2])
                    {
                        BezWall(bottom_control_points, width = -20, height = link_dim[2], steps = 64, centered = false, showCtlR = false);
                    }

                    translate([20, 50, 0]) cube([20, 20, 20], center = true);
                    translate([25, -20, 0]) cube([50, 20, 20], center = true);

                    hull()
                    {
                        translate([0, 12.5, 0])
                        {
                            translate([10, 0, 0]) cylinder(r=8, h=link_dim[2], center=true, $fn=60);
                            translate([40, 0, 0]) cylinder(r=8, h=link_dim[2], center=true, $fn=60);
                        }
                    }
                }

                cylinder(r2=small_radius, r1=case_shell_size,  h=1, $fn=30);
            }
        }

        transform_link_system() left_tent(holes_only = true);
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
*left_tent(printable = false);
*left_tent();
*left_link();
*test_nut_holes();

mirror([1, 0, 0])
{
    *right_top_plate();
    *test_right_top_plate();
    *left_link();
    *%left_keycaps();
}

module electronic_pcb(plate_thickness, holes_only = false)
{
    // Center the pcb plate on x and y
    x_offset_to_recenter = 1;
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

            mount_height = electronic_screw_mount_height-electronic_pcb_dim[2]-0.1;
            for (pcb_hole_position = pcb_hole_positions)
            {
                translate(pcb_hole_position)
                {
                    if (holes_only)
                    {
                        cylinder(h=mount_height*1.5, d=electronic_screws_hole_diameter, $fn = fragments_number);
                        translate([0, 0, -1.6]) make_pcb_case_screw_hole(0.5);
                    }
                    else
                    {
                        cylinder(h=mount_height, d=electronic_screw_mount_diameter, $fn= fragments_number);
                    }
                }
            }

            if (holes_only)
            {
                // Led hole
                led_offset = [17, 7, 5];
                translate(pcb_hole_positions[0] + led_offset)
                {
                    cylinder(h=20, d=5, $fn = fragments_number);
                }

                // Usb hole
                roundness = 3;
                hole_port_width = 17;
                hole_port_height = 8;
                hole_ports_dim = [hole_port_width-2*roundness, 20-2*roundness, hole_port_height-2*roundness];
                usb_offset = [17, 0, mount_height + 6];
                translate(pcb_hole_positions[3] + usb_offset)
                {
                    minkowski()
                    {
                        cube(hole_ports_dim, center = true);
                        sphere(r=roundness, $fn = fragments_number);
                    }
                }
            }
        }
    }
}

module electronic_case(top = true, bottom = true)
{
    module lateral_hole()
    {
        lateral_hole_dim = [6, 30, 15];
        translate([-30, 0, lateral_hole_dim[2]/2])
        {
            roundness = 3;
            minkowski()
            {
                cube(lateral_hole_dim, center = true);
                sphere(r=roundness, $fn = fragments_number);
            }
        }
    }

    box_size = [65, 70, 20];
    roundness = 7;
    plate_thickness = 3;
    contour_height = 2.99999;

    base_size = get_plate_base_size(box_size, roundness, 1, 0.5);

    points = [
        [0, 0],
        [base_size[0], 0],
        [base_size[0], base_size[1]],
        [0, base_size[1]],
    ];

    // Create holes
    difference()
    {
        union()
        {
            // Center the case on x and y
            translate([-base_size[0]/2-roundness, -base_size[1]/2-roundness, 0])
            {
                if (top)
                {
                    difference()
                    {
                        union()
                        {
                            box_top_plate(box_size, roundness, plate_thickness, contour_height, 1, false, $fn = fragments_number);
                            box_contour(box_size, roundness, plate_thickness, contour_height, false, $fn = fragments_number);
                        }

                        transform_to_box(roundness, false)
                        {
                            for (point = points)
                            {
                                translate(point)
                                {
                                    translate([0, 0, -3]) case_hole(40);
                                }
                            }
                        }
                    }
                }

                if (bottom)
                {
                    difference()
                    {
                        union()
                        {
                            box_bottom_inner_plate(box_size, roundness, plate_thickness, contour_height, false, $fn = fragments_number);
                            transform_to_box(roundness, false)
                            {
                                nut_slot_height = box_size[2] - plate_thickness;
                                echo("nut_slot_height", nut_slot_height);
                                echo("plate_thickness", plate_thickness);
                                for (point = points)
                                {
                                    translate(point)
                                    {
                                        translate([0, 0, plate_thickness])
                                        {
                                            difference()
                                            {
                                                nut_slot(nut_slot_height);
                                                nut_slot(force_nut_slot_z = 0, hole_only = true);
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        transform_to_box(roundness, false)
                        {
                            for (point = points)
                            {
                                translate(point)
                                {
                                    translate([0, 0, -3]) case_hole(40);
                                }
                            }
                        }
                    }
                }
            }

            if (bottom)
            {
                electronic_pcb(plate_thickness);
            }
        }

        electronic_pcb(plate_thickness, holes_only = true);

        // Lateral holes
        lateral_hole();
        mirror([1, 0, 0]) lateral_hole();
    }
}

l_size = [60, 60];
plate_thickness = 3;
contour_height = 2.99999;
roundness = 9/2;
bottom_plate(l_size, roundness, plate_thickness, $fn = fragments_number);

base = get_bottom_plate_base_cube(l_size, roundness, plate_thickness);
bounding_box = get_bottom_plate_bounding_box(l_size, roundness, plate_thickness);
%translate([0, 0, bounding_box[2]/2])
{
	cube(bounding_box, center = true);
}

points = get_points_from_rect(base);
for (p = points)
{
    translate(p)
    {
        translate([0, 0, plate_thickness])
        difference()
        {
            nut_slot(20);
            nut_slot(force_nut_slot_z = 0, hole_only = true);
        }
    }
}

*electronic_case(top = false);
*electronic_case(bottom = false);
*translate([80, 0, 22]) rotate([0, 180, 0]) electronic_case(bottom = false);
