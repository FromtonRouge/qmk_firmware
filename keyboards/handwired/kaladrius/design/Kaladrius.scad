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

use <BezierScad.scad>
use <MCAD/nuts_and_bolts.scad>

fragments_number = 100; // Use 0 for debugging, 50-100 for final rendering
switch_hole_width = 14;
switch_spacing = 4.8;

// For comparison, Ergodox settings are (in mm) :
//      offset_finger_middle = -2;
//      offset_finger_ring = 2;
//      offset_finger_pinky = 3;
// My personal settings :
offset_finger_middle = -3;
offset_finger_ring = 1;
offset_finger_pinky = 4;

thumb_x = 6.5;
thumb_y = 13;
thumb_angle = -20;
case_shell_size = 3;
case_height = 9;
case_outer_border = 8;
plate_height = 3;
plate_additional_border_height = 1;
screws_diameter = 3.5;
screws_mount_height = 17;
screw_mount_diameter = 10;
electronic_screw_mount_diameter = 6;
electronic_screws_hole_diameter = 2.7;
electronic_pcb_dim = [50.14, 70.12, 1.54];
electronic_screw_mount_height = 2*(11-electronic_pcb_dim[2])-5.5;
electronic_teensy_hole = [0, 50];
pcb_case_pos = [84, offset_finger_pinky-77];
pcb_plate_size = [48.40-2.12, 68.10-2.12, 2];
switch_hole_height = screws_mount_height + 5;

point_pinky_last = [get_kaladrius_origin()[0] + case_outer_border, get_kaladrius_origin()[1]];
point_pinky = [point_pinky_last[0] + switch_hole_width + switch_spacing, point_pinky_last[1]];
point_ring = [point_pinky[0] +  switch_hole_width + switch_spacing, point_pinky[1] + offset_finger_pinky];
point_middle = [point_ring[0] + switch_hole_width + switch_spacing, point_ring[1] + offset_finger_ring];
point_index3 = [point_middle[0] + switch_hole_width + switch_spacing, point_middle[1] + offset_finger_middle];
point_index2 = [point_index3[0] + switch_hole_width + switch_spacing, point_index3[1]];
point_index1 = [point_index2[0] + switch_hole_width + switch_spacing, point_index2[1]];

hole_positions = [
    point_pinky_last,
    point_middle + [(switch_hole_width + 2*switch_spacing)/2, switch_spacing/2],
    point_index1 + [(switch_hole_width + 2*switch_spacing), 0],
    point_index1 + [(switch_hole_width + 2*switch_spacing), -3*switch_hole_width - 4*switch_spacing],
    point_pinky_last - [0, 4*switch_hole_width + 5*switch_spacing],
    [-1.75*(switch_hole_width + switch_spacing) + 3*switch_hole_width + 4*switch_spacing, 0],
    [-1.75*(switch_hole_width + switch_spacing) + 3*switch_hole_width + 4*switch_spacing,  -3*switch_hole_width - 4*switch_spacing],
    [-1.75*(switch_hole_width + switch_spacing), -3*switch_hole_width - 4*switch_spacing],
];

function get_kaladrius_origin() = [0, 0, 0];
function get_thumb_anchor() = point_index1 - [0, 3*(switch_hole_width+switch_spacing) + switch_spacing];
function get_thumb_origin() = get_thumb_anchor() + [thumb_x, -thumb_y];
function get_pcb_case_bounding_box() = [pcb_plate_size[0] + electronic_screw_mount_diameter, pcb_plate_size[1] + electronic_screw_mount_diameter, electronic_screw_mount_height];
function get_pcb_case_origin() = get_kaladrius_origin() + pcb_case_pos;
function get_tenting_angle() = atan(electronic_screw_mount_height/(get_kaladrius_origin()[0]-get_pcb_case_origin()[0]));

module printable_nut_hole(size, tolerance = 0.25, cone=true)
{
    hull()
    {
        if (cone)
        {
            scale([0.2,0.2,2]) nutHole(size = size, tolerance = tolerance);
        }
        scale([1,1,1.2]) nutHole(size = size, tolerance = tolerance);
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
    // Pinky 2 and Pinky 1
    color("magenta")
    {
        create_holes(height, 4, 1, point_pinky_last, offset, has_additional_border);
        create_holes(height, 4, 1, point_pinky, offset, has_additional_border);
    }

    // Ring
    color("lightBlue")
    {
        create_holes(height, 5, 1, point_ring, offset, has_additional_border);
    }

    // Middle
    color("lightGreen")
    {
        create_holes(height, 5, 1, point_middle, offset, has_additional_border);
    }

    // Index
    color("cyan")
    {
        create_holes(height, 4, 1, point_index3, offset, has_additional_border);
        create_holes(height, 4, 1, point_index2, offset, has_additional_border);
        create_holes(height, 3, 1, point_index1, offset, has_additional_border);
    }

    // Thumbs
    color("yellow") transform_thumb()
    {
        create_holes(height, 1, 1, [0, 0], offset, has_additional_border);
        translate([-(switch_hole_width+switch_spacing)/4, 0, 0])
        {
            create_holes(height, 1, 1, [-(switch_hole_width + switch_spacing)/2 - (switch_hole_width + switch_spacing), -(switch_hole_width + switch_spacing) - (switch_hole_width + switch_spacing)/2], offset, has_additional_border, vertical = true);
            create_holes(height, 1, 2, [-(switch_hole_width + switch_spacing)/2, -(switch_hole_width + switch_spacing)], offset, has_additional_border);
            create_holes(height, 1, 1, [0, -2*(switch_hole_width + switch_spacing)], offset, has_additional_border);
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

module screw_mount(offset=0, height = switch_hole_height)
{
    for (index = [0:4])
    {
        transform_hole(index) case_hole(height, screw_mount_diameter);
    }

    transform_thumb()
    {
        for (index = [5:7])
        {
            transform_hole(index) case_hole(height, screw_mount_diameter);
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
    tolerance = 0.3;
    additional_border_width = 0.5;
    clip_width = 6;
    roundness = 0.5;
    x = switch_hole_width + 2*offset - 2*roundness + 2*additional_border_width - tolerance;
    y = switch_hole_width + 2*offset - 2*roundness - tolerance;
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
            hull()
            {
                top_point_offset = [(switch_hole_width+2*switch_spacing)/2, switch_spacing];
                top_control_points =  [
                    point_pinky_last,
                    point_pinky + top_point_offset,
                    point_ring + top_point_offset,
                    point_middle + top_point_offset,
                    point_index3 + top_point_offset,
                    point_index2 + top_point_offset,
                    point_index1 + [switch_hole_width + 2*switch_spacing, 0],
                ];

                bottom_control_points =  [
                    point_pinky_last + [0, -4*(switch_spacing+switch_hole_width) - switch_spacing],
                    point_pinky + [switch_spacing + switch_hole_width/2, -4*(switch_spacing+switch_hole_width) - 2*switch_spacing],
                    point_ring + [switch_spacing + switch_hole_width/2, -5*(switch_spacing+switch_hole_width) - 2*switch_spacing],
                    point_middle + [switch_spacing + switch_hole_width/2, -5*(switch_spacing+switch_hole_width) - 2*switch_spacing],
                    point_index3 + [switch_spacing + switch_hole_width/2, -4*(switch_spacing+switch_hole_width) - 2*switch_spacing],
                    point_index2 + [switch_spacing + switch_hole_width/2, -4*(switch_spacing+switch_hole_width) - 2*switch_spacing],
                    point_index1 + [2*switch_spacing + switch_hole_width, -3*(switch_spacing+switch_hole_width) - switch_spacing],
                ];

                difference()
                {
                    union()
                    {
                        // Create cells from pinky to the index finger
                        create_cells(basic_height, 4, 1, point_pinky_last);
                        create_cells(basic_height, 4, 1, point_pinky);
                        create_cells(basic_height, 5, 1, point_ring);
                        create_cells(basic_height, 5, 1, point_middle);
                        create_cells(basic_height, 4, 1, point_index3);
                        create_cells(basic_height, 4, 1, point_index2);
                    }

                    BezWall(top_control_points, width = 20, height = basic_height, steps = 64, centered = true, showCtlR = false);
                    BezWall(bottom_control_points, width = 20, height = basic_height, steps = 64, centered = true, showCtlR = false);
                }

                BezWall(top_control_points, width = 1, height = basic_height, steps = 64, centered = true, showCtlR = false);
                BezWall(bottom_control_points, width = 1, height = basic_height, steps = 64, centered = true, showCtlR = false);
            }

            // Thumb
            transform_thumb()
            {
                *create_cells(basic_height, 1, 1, [0, 0]);
                translate([-(switch_hole_width+switch_spacing)/4, 0, 0])
                {
                    *create_cells(basic_height, 1, 1, [-(switch_hole_width + switch_spacing)/2 - (switch_hole_width + switch_spacing), -(switch_hole_width + switch_spacing) - (switch_hole_width + switch_spacing)/2]);
                    *create_cells(basic_height, 1, 2, [-(switch_hole_width + switch_spacing)/2, -(switch_hole_width + switch_spacing)]);
                    *create_cells(basic_height, 1, 1, [0, -2*(switch_hole_width + switch_spacing)]);
                    translate([-1.5*(switch_hole_width+ switch_spacing), 0, 0]) create_cells(basic_height, 3, 3, [0,0]);
                }
            }

            // Index 1
            create_cells(basic_height, 3, 1, point_index1);
        }
        r1 = case_outer_border;
        r2 = chamfer? r1 - minkowski_height : r1;
        cylinder(r1=r1, r2=r2, h=minkowski_height, $fn = fragments_number);
    }
}

module top_plate()
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
                    translate(get_thumb_anchor()) cube([thumb_x, 2, mark_height]);
                    translate(get_thumb_anchor() + [thumb_x, -thumb_y]) cube([2, thumb_y, mark_height]);
                }
            }
            holes();
            case_holes();
        }
    }
}

module plate_supports()
{
    module create_support(row)
    {
        hull()
        {
            support_height = case_height+3*case_shell_size;
            support_diameter = 3.5;
            y = -row*(switch_spacing + switch_hole_width);
            translate([0, y-support_diameter/2])
            {
                cylinder(h=support_height, d=3.5, $fn=fragments_number);
            }

            translate([0, y-support_diameter])
            {
                cylinder(h=support_height, d=3.5, $fn=fragments_number);
            }
        }
    }

    translate(point_ring + [3*switch_spacing/2 + switch_hole_width, 0])
    {
        for (row = [1:4])
        {
            create_support(row);
        }
    }
}

module case()
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
            screw_mount(height = case_height + 2*case_shell_size);

            plate_supports();
        }

        // Top plate hole without chamfer
        translate([0, 0, case_height + case_shell_size - 1]) scale([1, 1, 8]) plate();

        // Case holes
        translate([0, 0, -20]) case_holes(height = 200);

        // Nut holes
        for (index = [0:4])
        {
            transform_hole(index) translate([0,0,-0.001]) printable_nut_hole(3);
        }

        // Nut holes
        transform_thumb()
        {
            for (index = [5:7])
            {
                transform_hole(index) translate([0,0,-0.001]) printable_nut_hole(3);
            }
        }

        // Electronic holes
        transform_pcb_case() pcb_case(holes_only = true, with_trrs_hole= false);
        mini_thumb_holes();
    }
}

module pcb_case(mount_height = electronic_screw_mount_height, bounding_box = false, holes_only = false, with_trrs_hole = true)
{
    module mount(h, d1, d2, holes_only)
    {
        if (holes_only)
        {
            cylinder(h=h*2, d=d1, $fn = fragments_number);
        }
        else
        {
            cylinder(h=h, d=d2, $fn= fragments_number);
        }
    }

    full_plate_dim = [pcb_plate_size[0] + electronic_screw_mount_diameter, pcb_plate_size[1] + electronic_screw_mount_diameter];
    positions = [
        [electronic_screw_mount_diameter/2, electronic_screw_mount_diameter/2, 0],
        [pcb_plate_size[0] + electronic_screw_mount_diameter/2, electronic_screw_mount_diameter/2, 0],
        [pcb_plate_size[0]+electronic_screw_mount_diameter/2, pcb_plate_size[1]+electronic_screw_mount_diameter/2, 0],
        [electronic_screw_mount_diameter/2, pcb_plate_size[1]+electronic_screw_mount_diameter/2, 0]
    ];

    if (!bounding_box)
    {
        for (index = [0:3])
        {
            translate(positions[index])
            {
                mount(mount_height-electronic_pcb_dim[2]-0.1, electronic_screws_hole_diameter, electronic_screw_mount_diameter, holes_only);

                if (holes_only)
                {
                    hull()
                    {
                        translate([0,0,-0.001]) printable_nut_hole(2, tolerance=0.4);
                        translate([0,0,-30]) printable_nut_hole(2, tolerance=0.4, cone= false);
                    }
                }
            }
        }
    }

    if (!holes_only)
    {
        if (bounding_box)
        {
            minkowski()
            {
                translate([electronic_screw_mount_diameter/2, electronic_screw_mount_diameter/2, 0]) cube([pcb_plate_size[0], pcb_plate_size[1], pcb_plate_size[2]]);
                cylinder(h=mount_height-pcb_plate_size[2], r=electronic_screw_mount_diameter/2, $fn = fragments_number);
            }
        }
        else
        {
            minkowski()
            {
                translate([electronic_screw_mount_diameter/2, electronic_screw_mount_diameter/2, 0]) cube([pcb_plate_size[0], pcb_plate_size[1], pcb_plate_size[2]/2]);
                cylinder(h=pcb_plate_size[2]/2, r=electronic_screw_mount_diameter/2, $fn = fragments_number);
            }
        }
    }
    else
    {
        // Hole for 3.5 trrs and mini usb
        roundness = 3;
        if (with_trrs_hole)
        {
            y_offset = -6;
            z_offset = -1;
            hole_port_width = pcb_plate_size[0] - electronic_screw_mount_diameter - 4;
            hole_ports_dim = [hole_port_width-2*roundness, 20-2*roundness, mount_height-2*roundness-z_offset+0.01];
            translate([(pcb_plate_size[0] - hole_port_width + electronic_screw_mount_diameter)/2,  full_plate_dim[1] + y_offset, z_offset])
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
        }

        // Usb and trrs room
        usb_and_trrs_room = [34-2*roundness, 24-2*roundness, 10];
        translate([(full_plate_dim[0]-usb_and_trrs_room[0])/2, full_plate_dim[1]-usb_and_trrs_room[1]-roundness - 2, mount_height-electronic_pcb_dim[2]-usb_and_trrs_room[2]])
        {
            minkowski()
            {
                cube(usb_and_trrs_room);
                cylinder(h=1, r=roundness, $fn = fragments_number);
            }
        }

        // Teensy hole
        teensy_hole_rect = [pcb_plate_size[0]/1.5, 1, 40];
        minkowski()
        {
            translate([full_plate_dim[0]/2, electronic_pcb_dim[1]-electronic_teensy_hole[1], -10]) cube(teensy_hole_rect, center=true);
            cylinder(h=1, r=1.5, $fn = fragments_number);
        }

        // Hole for the bottom of the case
        difference()
        {
            translation = [(full_plate_dim[0]-electronic_pcb_dim[0])/2, (full_plate_dim[1]-electronic_pcb_dim[1])/2, mount_height-0.001];
            translate(translation)
            {
                cube([electronic_pcb_dim[0], electronic_pcb_dim[1], 5*electronic_pcb_dim[2]]);
            }

            for (index = [0:3])
            {
                translate(positions[index])
                {
                    cylinder(h=mount_height*2, d=electronic_screw_mount_diameter, $fn = fragments_number);
                }
            }
        }

        // Additional holes for the bottom of the case
        translate([0, 0, mount_height + case_shell_size])
        {
            for (index = [0:3])
            {
                translate(positions[index])
                {
                    cylinder(h=mount_height, d=electronic_screw_mount_diameter, $fn = fragments_number);
                }
            }
        }
    }
}

module mini_thumb_holes()
{
    module make_hole()
    {
        translate([0,0,-electronic_screw_mount_height])
        {
            hull()
            {
                translate([0,0,-0.001]) printable_nut_hole(2, tolerance=0.4);
                translate([0,0,-30]) printable_nut_hole(2, tolerance=0.4, cone= false);
            }

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
                    translate([0, -factor*original_width]) make_hole();
                }
            }
        }
    }
}

module printable_pcb_case(printable = true, holes_only = false)
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
    module pcb_case_holes()
    {
        transform_pcb_case() 
        {
            difference()
            {
                pcb_case(mount_height = electronic_screw_mount_height+0.01, bounding_box=true);
                pcb_case();
            }

            pcb_case(holes_only = true);
        }

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
                                cube([get_pcb_case_bounding_box()[0], get_pcb_case_bounding_box()[1], 0.0001]);
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

                pcb_case_holes();
            }
        }
        else
        {
            pcb_case_holes();
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
    show_point(point_pinky_last);
    show_point(point_pinky);
    show_point(point_ring);
    show_point(point_middle);
    show_point(point_index3);
    show_point(point_index2);
    show_point(point_index1);
    show_point(get_pcb_case_origin());
    show_point(get_thumb_anchor());
    show_point(get_thumb_origin());
}

*plate(total_height=1, chamfer=false);
*holes();

*top_plate();
*case();
*printable_pcb_case(printable=false);
*printable_pcb_case();

mirror([1, 0, 0])
{
    top_plate();
    *case();
    *printable_pcb_case(printable=false);
    *printable_pcb_case();
}

// Kaladrius link system
module transform_link_system()
{
    translate([-100, 0, 0])
    {
        rotate([0, 0, -20])
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
    transform_link_system() printable_pcb_case();
    difference()
    {
        small_radius = 1;
        roundness = 5;
        link_dim = [60, 50, 15];
        translate([-30, -20, link_dim[2]/2])
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
                    translate([20, -20, 0]) cube([30, 20, 20], center = true);

                    hull()
                    {
                        translate([0, 12.5, 0])
                        {
                            translate([10, 0, 0]) cylinder(r=8, h=link_dim[2], center=true, $fn=60);
                            translate([40, 0, 0]) cylinder(r=8, h=link_dim[2], center=true, $fn=60);
                        }
                    }
                }

                cylinder(r2=small_radius, r1=case_shell_size,  h=case_shell_size, $fn=30);
            }
        }

        transform_link_system() printable_pcb_case(holes_only = true);
    }
}

*left_link();
*mirror([1, 0, 0])
{
    left_link();
}

*transform_pcb_case() 
{
    difference()
    {
        pcb_case();
        pcb_case(holes_only = true);
    }
    %pcb_case(holes_only = true);
}

*difference()
{
    translate([0, 0, 2.5]) {cylinder(h=5, d=15, center=true, $fn = fragments_number);}
    translate([0, 0, -0.001]) printable_nut_hole(3);
}
