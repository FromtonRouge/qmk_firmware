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

// chamfer: 0 = no chamfer, 1 = top chamfer, 2 = bottom chamfer
module contour_shape(size, minkowski_height, radius, centered, chamfer = 0, chamfer_angle = 45)
{
    minkowski()
    {
        cube(size, centered);

        if (chamfer == 0)
        {
            cylinder(h=minkowski_height, r1=radius, r2=radius);
        }
        else if (chamfer == 1)
        {
            cylinder(h=minkowski_height, r1=radius, r2=radius-(minkowski_height/tan(chamfer_angle)));
        }
        else if (chamfer == 2)
        {
            cylinder(h=minkowski_height, r2=radius, r1=radius-(minkowski_height/tan(chamfer_angle)));
        }
    }
}

module transform_to_box(roundness, centered = false)
{
    translation = (centered == false) ? [roundness, roundness, 0]:[0, 0, 0];
    translate(translation)
    {
        children();
    }
}

module box_contour(size, roundness, plate_thickness, contour_height, centered = false)
{
    transform_to_box(roundness, centered)
    {
        difference()
        {
            minkowski_height = 0.01;
            base_size = [size[0]-2*roundness, size[1]-2*roundness, size[2]-minkowski_height];
            union()
            {
                translate([0, 0, plate_thickness]) contour_shape(base_size - [0, 0, plate_thickness], minkowski_height, roundness, centered);
                bottom_size = get_plate_base_size(size, roundness, plate_thickness, contour_height);
                contour_shape(bottom_size, contour_height, roundness, centered, chamfer = 2);
            }

            chamfer_angle = 45;
            radius = roundness - contour_height/tan(chamfer_angle);
            translate([0, 0, -0.01]) scale([1, 1, 2]) contour_shape(base_size, contour_height, radius, centered, chamfer_angle = chamfer_angle);
        }
    }
}

function get_plate_base_size(size, roundness, plate_thickness, contour_height) = [size[0]-2*roundness, size[1]-2*roundness, plate_thickness-contour_height];

module box_bottom_inner_plate(size, roundness, plate_thickness, contour_thickness, centered = false)
{
    transform_to_box(roundness, centered)
    {
        contour_height = 0.5;
        base_size = get_plate_base_size(size, roundness, plate_thickness, contour_height);
        translate((centered == false) ? [0, 0, 0]:[0, 0, (base_size[2] - size[2])/2]) contour_shape(base_size, contour_height, roundness-contour_thickness, centered, chamfer = 1);
    }
}

module box_top_plate(size, roundness, plate_thickness, contour_height, centered = false)
{
    transform_to_box(roundness, centered)
    {
        base_size = get_plate_base_size(size, roundness, plate_thickness, contour_height);
        translate((centered == false) ? [0, 0, size[2]] : [0, 0, (base_size[2] + size[2])/2]) contour_shape(base_size, contour_height, roundness, centered, chamfer = 1);
    }
}

module bottom_plate(size_xy, roundness, plate_thickness)
{
    minkowski_height = plate_thickness/2;
    size_xyz = [size_xy[0], size_xy[1], plate_thickness-minkowski_height];
    translate([0, 0, size_xyz[2]/2]) contour_shape(size_xyz, minkowski_height, roundness, centered = true, chamfer = 1);
}

function get_points_from_rect(rect) = [[-rect[0]/2, -rect[1]/2], [rect[0]/2, -rect[1]/2], [rect[0]/2, rect[1]/2], [-rect[0]/2, rect[1]/2]];
