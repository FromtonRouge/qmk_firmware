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

module contour_shape(size, height, radius, centered, chamfer = false, chamfer_angle = 45)
{
    minkowski()
    {
        cube(size, centered);
        cylinder(h=height, r1=radius, r2=(chamfer==false)?radius:radius-(height/tan(chamfer_angle)));
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

module box_contour(size, roundness, thickness, centered = false)
{
    transform_to_box(roundness, centered)
    {
        difference()
        {
            contour_height = 0.01;
            base_size = [size[0]-2*roundness, size[1]-2*roundness, size[2]-contour_height];
            contour_shape(base_size, contour_height, roundness, centered);
            translate([0, 0, -0.01]) scale([1, 1, 2]) contour_shape(base_size, contour_height, roundness-thickness, centered);
        }
    }
}

function get_plate_base_size(size, roundness, plate_thickness, contour_height) = [size[0]-2*roundness, size[1]-2*roundness, plate_thickness-contour_height];

module box_bottom_plate(size, roundness, plate_thickness, contour_thickness, centered = false)
{
    transform_to_box(roundness, centered)
    {
        contour_height = 0.5;
        base_size = get_plate_base_size(size, roundness, plate_thickness, contour_height);
        translate((centered == false) ? [0, 0, 0]:[0, 0, (base_size[2] - size[2])/2]) contour_shape(base_size, contour_height, roundness-contour_thickness, centered, chamfer = true);
    }
}

module box_top_plate(size, roundness, plate_thickness, contour_height, contour_thickness, centered = false)
{
    transform_to_box(roundness, centered)
    {
        base_size = get_plate_base_size(size, roundness, plate_thickness, contour_height);
        translate((centered == false) ? [0, 0, size[2]] : [0, 0, (base_size[2] + size[2])/2]) contour_shape(base_size, contour_height, roundness, centered, chamfer = true);
    }
}
