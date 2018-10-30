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

switch_hole_width = 14;
switch_spacing = 4.8;
offset_finger_middle = 2.2;
offset_finger_ring = 8.38;
offset_finger_pinky_1 = 18;
offset_finger_pinky_2 = offset_finger_pinky_1;
thumb_x = 9;
thumb_y = 18;
thumb_angle = -20;
case_shell_size = 3;
case_height = 5;
case_outer_border = 8;
plate_height = 3;
plate_padding = 1;
plate_additional_border_height = 1;
prototype_mode = false;
screws_diameter = 3.5;
screws_mount_height = 17;
screw_mount_diameter = 10;
electronic_screw_mount_diameter = 6;
electronic_screws_hole_diameter = 2.7;
electronic_pcb_dim = [50.14, 70.12, 1.54];
electronic_screw_mount_height = 2*(11-electronic_pcb_dim[2])-7.5;
electronic_teensy_hole = [0, 50];
pcb_case_pos = [-33, -92];
pcb_plate_size = [48.40-2.12, 68.10-2.12, 2];
mirror_translate = [60, 0, 0];
switch_hole_height = screws_mount_height + 5;

point_index = [-switch_hole_width, 0];
point_middle = [switch_hole_width + 2*switch_spacing, offset_finger_middle];
point_ring = [2*switch_hole_width + 3*switch_spacing, offset_finger_middle - offset_finger_ring];
point_pinky = [3*switch_hole_width + 4*switch_spacing, offset_finger_middle - offset_finger_ring - offset_finger_pinky_1];
point_pinky_last = [4*switch_hole_width + 5*switch_spacing, offset_finger_middle - offset_finger_ring - offset_finger_pinky_2];

hole_positions = [
    [-2*switch_hole_width - switch_spacing - switch_spacing, -3*(switch_hole_width + switch_spacing), 0],
    [-2*switch_hole_width - switch_spacing - switch_spacing, switch_spacing, 0],
    [2*(switch_hole_width+switch_spacing) - switch_hole_width/2, offset_finger_middle + switch_spacing + case_outer_border/2, 0],
    [5*(switch_hole_width+switch_spacing) + switch_spacing, offset_finger_middle - offset_finger_ring - switch_hole_width, 0],
    [5*(switch_hole_width+switch_spacing) + switch_spacing, -4*(switch_hole_width + switch_spacing) + offset_finger_middle - offset_finger_ring - switch_hole_width - switch_spacing, 0],
    [-switch_hole_width - 2*switch_spacing, switch_spacing, 0],
    [2*(switch_hole_width+switch_spacing)-((switch_hole_width+switch_spacing)*0.25) + switch_spacing, -2*(switch_hole_width + switch_spacing)*1.5, 0],
    [-switch_hole_width - 2*switch_spacing, -3*(switch_spacing+switch_hole_width), 0]
];

function get_case_right_extremity() = point_pinky_last[0] + switch_hole_width + plate_padding + case_outer_border + switch_spacing;
function get_pcb_case_bounding_box() = [pcb_plate_size[0] + electronic_screw_mount_diameter, pcb_plate_size[1] + electronic_screw_mount_diameter, electronic_screw_mount_height];
function get_pcb_case_right_extremity() = pcb_case_pos[0] + get_pcb_case_bounding_box()[0];
function get_tilt() = atan(electronic_screw_mount_height/(get_case_right_extremity()-get_pcb_case_right_extremity()));

module printable_nut_hole(size, tolerance = 0.25, height_factor = 1)
{
    METRIC_NUT_THICKNESS = [
        -1, //0 index is not used but reduces computation
        -1,
        1.6,//m2
        2.40,//m3
        3.20,//m4
        4.00,//m5
        5.00,//m6
        -1,
        6.50,//m8
        -1,
        8.00,//m10
        -1,
        10.00,//m12
        -1,
        -1,
        -1,
        13.00,//m16
        -1,
        -1,
        -1,
        16.00//m20
            -1,
        -1,
        -1,
        19.00,//m24
        -1,
        -1,
        -1,
        -1,
        -1,
        24.00,//m30
        -1,
        -1,
        -1,
        -1,
        -1,
        29.00//m36
            ];
    hull()
    {
        height = METRIC_NUT_THICKNESS[size]+tolerance;
        translate([0, 0, height_factor*height]) scale([0.2, 0.2, 1]) nutHole(size = size, tolerance = tolerance);
        scale([1,1,height_factor*1.2]) nutHole(size = size, tolerance = tolerance);
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
    $fn = 60; cylinder(height-2, d=diameter);
    translate([0, 0, height-2])
    {
        $fn = 60; cylinder(2, d=diameter);
    }
}

module hole(offset, height, has_additional_border = true, vertical = false)
{
    roundness = 0.5;
    ext_hole = offset+1;
    translate([-ext_hole-switch_hole_width + roundness, -ext_hole-switch_hole_width + roundness, has_additional_border?plate_additional_border_height:0])
    {
        minkowski()
        {
            cube([switch_hole_width + 2*ext_hole - 2*roundness, switch_hole_width + 2*ext_hole - 2*roundness, height]);
            $fn = 30; cylinder(r=roundness,  h=2);
        }
    }

    tolerance = 0.3;
    additional_border_width = 0.5;
    clip_width = 6;
    if (vertical)
    {
        x = switch_hole_width + 2*offset - 2*roundness - tolerance;
        y = switch_hole_width + 2*offset - 2*roundness + 2*additional_border_width - tolerance;
        translate([-offset-switch_hole_width + roundness + tolerance/2, -offset-switch_hole_width - additional_border_width + roundness + tolerance/2, has_additional_border?-2:0])
        {
            minkowski()
            {
                linear_extrude(height=height)
                {
                    points = [
                        [0, 0],
                        [(x-clip_width)/2, 0],
                        [(x-clip_width)/2, additional_border_width],
                        [(x-clip_width)/2 + clip_width, additional_border_width],
                        [(x-clip_width)/2 + clip_width, 0],
                        [x, 0],
                        [x, y],
                        [(x-clip_width)/2 + clip_width, y],
                        [(x-clip_width)/2 + clip_width, y-additional_border_width],
                        [(x-clip_width)/2, y-additional_border_width],
                        [(x-clip_width)/2, y],
                        [0, y],
                    ];
                    polygon(points);
                }
                $fn = 30; cylinder(r=roundness,  h=2);
            }
        }
    }
    else
    {
        x = switch_hole_width + 2*offset - 2*roundness + 2*additional_border_width - tolerance;
        y = switch_hole_width + 2*offset - 2*roundness - tolerance;
        translate([-offset-switch_hole_width - additional_border_width + roundness + tolerance/2, -offset-switch_hole_width + roundness + tolerance/2, has_additional_border?-2:0])
        {
            minkowski()
            {
                linear_extrude(height=height)
                {
                    points = [
                        [0, 0],
                        [x, 0],
                        [x, (y-clip_width)/2],
                        [x-additional_border_width, (y-clip_width)/2],
                        [x-additional_border_width, (y-clip_width)/2 + clip_width],
                        [x, (y-clip_width)/2 + clip_width],
                        [x, y],
                        [0, y],
                        [0, (y-clip_width)/2 + clip_width],
                        [additional_border_width, (y-clip_width)/2 + clip_width],
                        [additional_border_width, (y-clip_width)/2],
                        [0, (y-clip_width)/2],
                    ];
                    polygon(points);
                }
                $fn = 30; cylinder(r=roundness,  h=2);
            }
        }
    }
}

module transform_thumb()
{
    translate([switch_spacing-thumb_x, -thumb_y, 0])
    {
        translate([0, -3*(switch_hole_width + switch_spacing), 0])
        {
            translate([-(switch_hole_width+switch_spacing)*0.25 - switch_hole_width, 0, 0])
            {
                rotate([0, 0, -thumb_angle])
                {
                    children();
                }
            }
        }
    }
}

module transform_pcb_case()
{
    translate([pcb_case_pos[0], pcb_case_pos[1], -electronic_screw_mount_height])  children();
}

module holes(offset=0, height = switch_hole_height, has_additional_border = true)
{
    // Index
    color("cyan")
    {
        range = prototype_mode? [2:3]: [0:3];
        for (row = range)
        {
            translate([0, -row*(switch_hole_width + switch_spacing), 0])
            {
                hole(offset, height, has_additional_border);
            }

            translate([switch_hole_width + switch_spacing, -row*(switch_hole_width + switch_spacing), 0])
            {
                hole(offset, height, has_additional_border);
            }
        }

        if (!prototype_mode)
        {
            for (row = [0:2])
            {
                translate([-(switch_hole_width + switch_spacing), -row*(switch_hole_width + switch_spacing), 0])
                {
                    hole(offset, height, has_additional_border);
                }
            }
        }
    }

    // Middle
    color("lightGreen")
    {
        range = prototype_mode? [2:4]: [0:4];
        for (row = range)
        {
            translate([2*(switch_hole_width+switch_spacing), -row*(switch_hole_width + switch_spacing) + offset_finger_middle, 0])
            {
                hole(offset, height, has_additional_border);
            }
        }
    }

    // Ring
    color("lightBlue")
    {
        range = prototype_mode? [2:4]: [0:4];
        for (row = range)
        {
            translate([3*(switch_hole_width+switch_spacing), -row*(switch_hole_width + switch_spacing) + offset_finger_middle - offset_finger_ring, 0])
            {
                hole(offset, height, has_additional_border);
            }
        }
    }

    // Pinky
    color("magenta")
    {
        range = prototype_mode? [2:4]: [0:3];
        for (row = range)
        {
            translate([4*(switch_hole_width+switch_spacing), -row*(switch_hole_width + switch_spacing) + offset_finger_middle - offset_finger_ring - offset_finger_pinky_1, 0])
            {
                hole(offset, height, has_additional_border);
            }
        }

        if (!prototype_mode)
        {
            for (row = [0:3])
            {
                translate([5*(switch_hole_width+switch_spacing), -row*(switch_hole_width + switch_spacing) + offset_finger_middle - offset_finger_ring - offset_finger_pinky_2, 0])
                {
                    hole(offset, height, has_additional_border);
                }
            }
        }
    }

    // Thumbs
    color("yellow") transform_thumb()
    {
        // Top
        hole(offset, height, has_additional_border);

        // Middle
        x_adjust = (switch_hole_width+switch_spacing)*0.25;
        translate([-x_adjust, -switch_spacing-switch_hole_width, 0])
        {
            for (col = [0:1])
            {
                translate([col*(switch_hole_width+switch_spacing), 0, 0])
                {
                    hole(offset, height, has_additional_border);
                }
            }

        }

        // Right
        translate([2*(switch_hole_width+switch_spacing)-x_adjust, -(switch_hole_width + switch_spacing)*1.5, 0])
        {
            hole(offset, height, has_additional_border, true);
        }

        // Bottom
        translate([(switch_hole_width + switch_spacing)/2-x_adjust, -2*(switch_spacing+switch_hole_width), 0])
        {
            hole(offset, height, has_additional_border);
        }
    }
}

module transform_hole(index)
{
    translate(hole_positions[index]) children();
}

module case_holes(offset=0, height=switch_hole_height, diameter=screws_diameter)
{
    if (!prototype_mode)
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
    if (!prototype_mode)
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
}

module plate(chamfer = false)
{
    module sub_plate(rows, columns, origin, padding)
    {
        size = switch_hole_width + switch_spacing;
        points = [
            [origin[0] - padding, origin[1] + padding], // top left
            [origin[0] - padding, origin[1] - padding - rows*switch_hole_width - (rows-1)*switch_spacing], // bottom left
            [origin[0] + columns*switch_hole_width + (columns-1)*switch_spacing + padding, origin[1] - padding - rows*switch_hole_width - (rows-1)*switch_spacing], // bottom right
            [origin[0] + columns*switch_hole_width + (columns-1)*switch_spacing + padding, origin[1] + padding] // top right
        ];

        polygon(points);
    }

    module final_plate()
    {
        minkowski()
        { 
            height1 = plate_height/3;
            height2 = 2*height1;
            union()
            {
                size = switch_hole_width + switch_spacing;
                linear_extrude(height=height1)
                {
                    // Thumb
                    transform_thumb()
                    {
                        sub_plate(3, 3, [-switch_hole_width-switch_spacing, 0], plate_padding);
                    }
                }

                // Index
                linear_extrude(height=height1) sub_plate(3, 1, point_index - [switch_hole_width+switch_spacing, 0], plate_padding);

                hull()
                {
                    linear_extrude(height=height1)
                    {
                        // Index, Middle, Ring, Pinky
                        sub_plate(4, 2, point_index, plate_padding);
                        sub_plate(5, 1, point_middle, plate_padding);
                        sub_plate(5, 1, point_ring, plate_padding);
                        sub_plate(4, 1, point_pinky, plate_padding);
                        sub_plate(4, 1, point_pinky_last, plate_padding);
                    }

                    y_offset = 6;
                    point_offset = [switch_hole_width, 2*switch_spacing];

                    control_points =  [
                        point_index - [switch_hole_width + switch_spacing, plate_padding],
                        point_middle + point_offset,
                        point_ring + point_offset,
                        point_pinky_last + [switch_hole_width-plate_padding, 0] 
                    ];

                    BezWall(control_points, width = 5, height = height1, steps = 64, centered = true, showCtlR = false);
                }
            }
            r1 = case_outer_border+switch_spacing;
            r2 = chamfer? r1 - height2 : r1;
            $fn = 60; cylinder(r1=r1, r2=r2, h=height2);
        }
    }

    if (prototype_mode)
    {
        holes(4, 2, false);
    }
    else
    {
        final_plate();
    }
}

module top_plate()
{
    translate([0, 0, case_height + case_shell_size])
        translate(mirror_translate) rotate([0, 0, 0])
        {
            difference()
            {
                difference()
                {
                    union()
                    {
                        plate(chamfer=true);
                    }
                    holes();
                }
                case_holes();
            }
        }
}

module case()
{
    if (!prototype_mode)
    {
        translate(mirror_translate)
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
                            big_radius = case_shell_size;
                            // Bottom
                            minkowski()
                            {
                                plate();
                                $fn = 30; cylinder(r1=small_radius, r2=big_radius,  h=case_shell_size);
                            }

                            // Middle
                            translate([0, 0, case_shell_size])
                            {
                                minkowski()
                                {
                                    plate();
                                    $fn = 30; cylinder(r=case_shell_size,  h=case_height-case_shell_size);
                                }
                            }

                            // Top
                            translate([0, 0, case_height])
                            {
                                minkowski()
                                {
                                    plate();
                                    $fn = 30; cylinder(r2=small_radius, r1=big_radius,  h=case_shell_size);
                                }
                            }
                        }

                        translate([0, 0, case_shell_size]) scale([1, 1, 8]) plate();
                    }

                    // Add all bolt mounts
                    screw_mount(height = case_height + 2*case_shell_size);
                }

                // Top plate hole without chamfer
                translate([0, 0, case_height+case_shell_size - 1]) scale([1, 1, 8]) plate();

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
                transform_pcb_case() pcb_case(holes_only = true);
            }
        }
    }
}

module pcb_case(holes_only = false)
{
    module mount(h, d1, d2, holes_only)
    {
        if (holes_only)
        {
            translate([0, 0, -h/4])
            {
                $fn = 60; cylinder(h=h*2.5, d=d1);
            }
        }
        else
        {
            $fn= 60; cylinder(h=h, d=d2);
        }
    }

    full_plate_dim = [pcb_plate_size[0] + electronic_screw_mount_diameter, pcb_plate_size[1] + electronic_screw_mount_diameter];
    positions = [
        [electronic_screw_mount_diameter/2, electronic_screw_mount_diameter/2, 0],
        [pcb_plate_size[0] + electronic_screw_mount_diameter/2, electronic_screw_mount_diameter/2, 0],
        [pcb_plate_size[0]+electronic_screw_mount_diameter/2, pcb_plate_size[1]+electronic_screw_mount_diameter/2, 0],
        [electronic_screw_mount_diameter/2, pcb_plate_size[1]+electronic_screw_mount_diameter/2, 0]
    ];

    for (index = [0:3])
    {
        translate(positions[index])
        {
            mount(electronic_screw_mount_height, electronic_screws_hole_diameter, electronic_screw_mount_diameter, holes_only);

            if (holes_only)
            {
                translate([0,0,-0.001]) printable_nut_hole(2, tolerance=0.4);
            }
        }
    }

    if (!holes_only)
    {
        minkowski()
        {
            translate([electronic_screw_mount_diameter/2, electronic_screw_mount_diameter/2, 0]) cube([pcb_plate_size[0], pcb_plate_size[1], pcb_plate_size[2]/2]);
            $fn = 60; cylinder(h=pcb_plate_size[2]/2, r=electronic_screw_mount_diameter/2);
        }

        *difference()
        {
            minkowski()
            {
                translate([electronic_screw_mount_diameter/2, electronic_screw_mount_diameter/2, 0]) cube([pcb_plate_size[0], pcb_plate_size[1], electronic_screw_mount_height]);
                $fn = 60; cylinder(h=pcb_plate_size[2]/2, r1=electronic_screw_mount_diameter/2, r2=electronic_screw_mount_diameter/2 + pcb_plate_size[2]/2);
            }

            // TODO
            wall_width = 2;
            translate([electronic_screw_mount_diameter/2, electronic_screw_mount_diameter/2, 0]) cube([pcb_plate_size[0], pcb_plate_size[1], electronic_screw_mount_height]);
        }
    }
    else
    {
        // Hole for 3.5 trrs and mini usb
        roundness = 3;
        y_offset = -6;
        z_offset = -1;
        usb_height = 4;
        hole_port_width = pcb_plate_size[0] - electronic_screw_mount_diameter - 4;
        hole_ports_dim = [hole_port_width-2*roundness, 20-2*roundness, electronic_screw_mount_height-2*roundness];
        translate([(pcb_plate_size[0] - hole_port_width + electronic_screw_mount_diameter)/2,  full_plate_dim[1] + y_offset, z_offset])
        {
            translate([roundness, roundness, roundness])
            {
                minkowski()
                {
                    cube(hole_ports_dim);
                    $fn = 60; sphere(r=roundness);
                }
            }
        }

        // Usb and trrs room
        usb_and_trrs_room = [34-2*roundness, 24-2*roundness, 10];
        translate([(full_plate_dim[0]-usb_and_trrs_room[0])/2, full_plate_dim[1]-usb_and_trrs_room[1]-roundness - 2, electronic_screw_mount_height-usb_and_trrs_room[2]])
        {
            minkowski()
            {
                cube(usb_and_trrs_room);
                $fn = 60; cylinder(h=1, r=roundness);
            }
        }

        // Hole for pcb
        translate([0, 0, electronic_screw_mount_height-electronic_pcb_dim[2]]) 
        {
            translation = [(full_plate_dim[0]-electronic_pcb_dim[0])/2, (full_plate_dim[1]-electronic_pcb_dim[1])/2, 0];
            translate(translation) cube(electronic_pcb_dim);
        }

        // Teensy hole
        teensy_hole_rect = [pcb_plate_size[0]/1.5, 1, 10];
        minkowski()
        {
            translate([full_plate_dim[0]/2, electronic_pcb_dim[1]-electronic_teensy_hole[1], 0]) cube(teensy_hole_rect, center=true);
            $fn = 60; cylinder(h=1, r=1.5);
        }

        // Hole for the bottom of the case
        difference()
        {
            translation = [(full_plate_dim[0]-electronic_pcb_dim[0])/2, (full_plate_dim[1]-electronic_pcb_dim[1])/2, electronic_screw_mount_height-0.001];
            translate(translation)
            {
                cube([electronic_pcb_dim[0], electronic_pcb_dim[1], 5*electronic_pcb_dim[2]]);
            }

            for (index = [0:3])
            {
                translate(positions[index])
                {
                    $fn= 60; cylinder(h=electronic_screw_mount_height*2, d=electronic_screw_mount_diameter);
                }
            }
        }

        // Additional holes
        translate([0, 0, electronic_screw_mount_height + case_shell_size])
        {
            for (index = [0:3])
            {
                translate(positions[index])
                {
                    $fn= 60; cylinder(h=electronic_screw_mount_height, d=electronic_screw_mount_diameter);
                }
            }
        }
    }
}

*plate(chamfer=true);
*holes();
*top_plate();
*mirror([1, 0, 0]) top_plate();

*union()
{
    difference()
    {
        pcb_case();
        pcb_case(holes_only=true);
    }
    %pcb_case(holes_only=true);
}

*mirror([1, 0, 0])
{
    intersection()
    {
        translate([40, -3, 0]) transform_pcb_case() cube([90, 80, 40]);
        case();
    }
}

*intersection()
{
    translate([40, -3, 0]) transform_pcb_case() cube([90, 80, 40]);
    top_plate();
}

*translate(mirror_translate)
{
    difference()
    {
        transform_pcb_case() pcb_case();
        transform_pcb_case() pcb_case(holes_only = true);
    }
}

*case();
*mirror([1, 0, 0]) case();

%plate();

{
    transform_pcb_case() pcb_case();

    distance = get_case_right_extremity() - get_pcb_case_right_extremity();
    %translate([pcb_case_pos[0], pcb_case_pos[1], 0]) translate([distance + get_pcb_case_bounding_box()[0], get_pcb_case_bounding_box()[1], 0]) rotate([0, 0, 180]) rotate_hull_around_y(angle=get_tilt()) translate([distance, 0, 0])
    {
        minkowski()
        {
            cube([get_pcb_case_bounding_box()[0], get_pcb_case_bounding_box()[1], 0.0001]);
            $fn = 60; cylinder(r=5,  h=0.001);
        }
    }
}

*difference()
{
    translate([0, 0, 2.5]) {$fn = 60;cylinder(h=5, d=15, center=true);}
    translate([0, 0, -0.001]) printable_nut_hole(3);
}
