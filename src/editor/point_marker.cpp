//  SuperTux
//  Copyright (C) 2016 Hume2 <teratux.mail@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "editor/point_marker.hpp"

#include "supertux/globals.hpp"
#include "supertux/resources.hpp"
#include "video/color.hpp"
#include "video/renderer.hpp"
#include "video/video_system.hpp"

PointMarker::PointMarker (const Vector& pos)
{
  bbox.p1 = pos;
  bbox.set_size(16, 16);
}

PointMarker::PointMarker ()
{
  bbox.p1 = Vector(0, 0);
  bbox.p2 = Vector(16, 16);
}

PointMarker::~PointMarker() {

}

void PointMarker::draw(DrawingContext& context) {
  context.draw_filled_rect(bbox, Color(1, 1, 1, 0.5), LAYER_GUI-20);
}

/* EOF */
