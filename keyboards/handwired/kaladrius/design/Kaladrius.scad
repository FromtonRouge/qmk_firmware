use <BezierScad.scad>;

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
case_height = 8;
case_outer_border = 8;
case_color = "red";
plate_height = 2;
plate_additional_border_height = 1;
prototype_mode = false;
screws_diameter = 3.4;
screws_mount_height = 17;
screw_mount_diameter = 8;
screw_nut_diameter = 6.5;
mirror_translate = [60, 0, 0];
switch_hole_height = screws_mount_height + 5;

module screw_hole(height, pos, diameter=screws_diameter)
{
    translate(pos)
    {
        $fn = 60; cylinder(height-2, d=diameter);
        translate([0, 0, height-2])
        {
            $fn = 60; cylinder(2, d2=diameter/2, d1=diameter);
        }
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

    additional_border_width = 0.5;
    clip_width = 6;
    if (vertical)
    {
        translate([-offset-switch_hole_width + roundness, -offset-switch_hole_width - additional_border_width + roundness, has_additional_border?-2:0])
        {
            minkowski()
            {
                linear_extrude(height=height)
                {
                    width = switch_hole_width + 2*offset - 2*roundness;
                    height = switch_hole_width + 2*offset - 2*roundness + 2*additional_border_width;
                    points = [
                        [0, 0],
                        [(width-clip_width)/2, 0],
                        [(width-clip_width)/2, additional_border_width],
                        [(width-clip_width)/2 + clip_width, additional_border_width],
                        [(width-clip_width)/2 + clip_width, 0],
                        [width, 0],
                        [width, height],
                        [(width-clip_width)/2 + clip_width, height],
                        [(width-clip_width)/2 + clip_width, height-additional_border_width],
                        [(width-clip_width)/2, height-additional_border_width],
                        [(width-clip_width)/2, height],
                        [0, height],
                    ];
                    polygon(points);
                }
                $fn = 30; cylinder(r=roundness,  h=2);
            }
        }
    }
    else
    {
        translate([-offset-switch_hole_width - additional_border_width+ roundness, -offset-switch_hole_width + roundness, has_additional_border?-2:0])
        {
            minkowski()
            {
                linear_extrude(height=height)
                {
                    width = switch_hole_width + 2*offset - 2*roundness + 2*additional_border_width;
                    height = switch_hole_width + 2*offset - 2*roundness;
                    points = [
                        [0, 0],
                        [width, 0],
                        [width, (height-clip_width)/2],
                        [width-additional_border_width, (height-clip_width)/2],
                        [width-additional_border_width, (height-clip_width)/2 + clip_width],
                        [width, (height-clip_width)/2 + clip_width],
                        [width, height],
                        [0, height],
                        [0, (height-clip_width)/2 + clip_width],
                        [additional_border_width, (height-clip_width)/2 + clip_width],
                        [additional_border_width, (height-clip_width)/2],
                        [0, (height-clip_width)/2],
                    ];
                    polygon(points);
                }
                $fn = 30; cylinder(r=roundness,  h=2);
            }
        }
    }
}

module thumb_transform()
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
	color("yellow") thumb_transform()
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

module screw_holes(offset=0, height=switch_hole_height, diameter=screws_diameter)
{
    if (!prototype_mode)
    {
		translate([0, 0, -1])
		{
			// Index
			color("cyan")
			{
				screw_hole(height, [-2*switch_hole_width - switch_spacing - switch_spacing, switch_spacing, 0], diameter);
				screw_hole(height, [-2*switch_hole_width - switch_spacing - switch_spacing, -3*(switch_hole_width + switch_spacing), 0], diameter);
			}

			// Middle
			color("lightGreen")
			{
				screw_hole(height, [2*(switch_hole_width+switch_spacing) - switch_hole_width/2, offset_finger_middle + switch_spacing + case_outer_border/2, 0], diameter);
			}

			// Pinky
			color("magenta")
			{
				screw_hole(height, [5*(switch_hole_width+switch_spacing) + switch_spacing, offset_finger_middle - offset_finger_ring - switch_hole_width, 0], diameter);
				screw_hole(height, [5*(switch_hole_width+switch_spacing) + switch_spacing, -4*(switch_hole_width + switch_spacing) + offset_finger_middle - offset_finger_ring - switch_hole_width - switch_spacing, 0], diameter);
			}

			// Thumbs
			color("yellow") thumb_transform()
			{
				x_adjust = (switch_hole_width+switch_spacing)*0.25;
				screw_hole(height, [-switch_hole_width - 2*switch_spacing, switch_spacing, 0], diameter);
				screw_hole(height, [2*(switch_hole_width+switch_spacing)-x_adjust + switch_spacing, -2*(switch_hole_width + switch_spacing)*1.5, 0], diameter);
				screw_hole(height, [-switch_hole_width - 2*switch_spacing, -3*(switch_spacing+switch_hole_width), 0], diameter);
			}
		}
    }
}

module screw_mount(offset=0, height = switch_hole_height)
{
    if (!prototype_mode)
    {
        // Index
        color("cyan")
        {
            screw_hole(height, [-2*switch_hole_width - switch_spacing - switch_spacing, switch_spacing, 0], screw_mount_diameter);
            screw_hole(height, [-2*switch_hole_width - switch_spacing - switch_spacing, -3*(switch_hole_width + switch_spacing), 0], screw_mount_diameter);
        }

		// Middle
		color("lightGreen")
		{
			screw_hole(height, [2*(switch_hole_width+switch_spacing) - switch_hole_width/2, offset_finger_middle + switch_spacing, 0], screw_mount_diameter);
		}

        // Pinky
        color("magenta")
        {
            screw_hole(height, [5*(switch_hole_width+switch_spacing) + switch_spacing, offset_finger_middle - offset_finger_ring - switch_hole_width, 0], screw_mount_diameter);
            screw_hole(height, [5*(switch_hole_width+switch_spacing) + switch_spacing, -4*(switch_hole_width + switch_spacing) + offset_finger_middle - offset_finger_ring - switch_hole_width - switch_spacing, 0], screw_mount_diameter);
        }

        // Thumbs
        color("yellow") thumb_transform()
        {
            x_adjust = (switch_hole_width+switch_spacing)*0.25;
            screw_hole(height, [-switch_hole_width - 2*switch_spacing, switch_spacing, 0], screw_mount_diameter);
            screw_hole(height, [2*(switch_hole_width+switch_spacing)-x_adjust + switch_spacing, -2*(switch_hole_width + switch_spacing)*1.5, 0], screw_mount_diameter);
            screw_hole(height, [-switch_hole_width - 2*switch_spacing, -3*(switch_spacing+switch_hole_width), 0], screw_mount_diameter);
        }
    }
}

module plate()
{
    module sub_plate(rows, columns, origin, padding)
    {
        size = switch_hole_width + switch_spacing;
        points = [
            [origin[0] - padding, origin[1] + padding],
            [origin[0] - padding, origin[1] - padding - rows*switch_hole_width - (rows-1)*switch_spacing],
            [origin[0] + columns*switch_hole_width + (columns-1)*switch_spacing + padding, origin[1] - padding - rows*switch_hole_width - (rows-1)*switch_spacing],
            [origin[0] + columns*switch_hole_width + (columns-1)*switch_spacing + padding, origin[1] + padding]
        ];

        polygon(points);
    }

    module final_plate()
    {
        minkowski()
        { 
            union()
            {
                plate_padding = 1;
                size = switch_hole_width + switch_spacing;
                linear_extrude(height=plate_height)
                {
                    // Thumb
                    thumb_transform()
                    {
                        sub_plate(3, 3, [-switch_hole_width-switch_spacing, 0], plate_padding);
                    }
                }

                // Index
                point_index = [-switch_hole_width, 0];
                linear_extrude(height=plate_height) sub_plate(3, 1, point_index - [switch_hole_width+switch_spacing, 0], plate_padding);

				hull()
				{
                    point_middle = [switch_hole_width + 2*switch_spacing, offset_finger_middle];
                    point_ring = [2*switch_hole_width + 3*switch_spacing, offset_finger_middle - offset_finger_ring];
                    point_pinky = [3*switch_hole_width + 4*switch_spacing, offset_finger_middle - offset_finger_ring - offset_finger_pinky_1];
                    point_pinky_last = [4*switch_hole_width + 5*switch_spacing, offset_finger_middle - offset_finger_ring - offset_finger_pinky_2];
					linear_extrude(height=plate_height)
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
                        point_index - [switch_hole_width + switch_spacing, 0],
                        point_index,
                        point_middle + point_offset,
                        point_ring + point_offset,
                        point_pinky + point_offset,
                        point_pinky_last + [switch_hole_width, 0] 
                    ];

					BezWall(control_points, width = 5, height = plate_height, steps = 32, centered = true, showCtlR = false);
				}
            }
            $fn = 60; cylinder(r=case_outer_border+switch_spacing, h=plate_height);
        }
    }

    color(case_color)
    {
        if (prototype_mode)
        {
            holes(4, 2, false);
        }
        else
        {
            final_plate();
        }
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
                    plate();
                }
                holes();
            }
            screw_holes();
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
                difference()
                {
                    union()
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

                                    translate([0, 0, case_shell_size])
                                    {
                                        scale([1, 1, 8])
                                        {
                                            plate();
                                        }
                                    }
                                }

                                screw_mount(height = case_height + 2*case_shell_size);
                            }
                            translate([0, 0, -20]) screw_holes(height = 200);
                        }
                    }

                    translate([0, 0, case_height+case_shell_size]) plate();
                }

                screw_holes(0, 4, screw_nut_diameter);
            }
        }
    }
}

*plate();
*holes();
top_plate();
*mirror([1, 0, 0]) top_plate();
*case();
*mirror([1, 0, 0]) case();
