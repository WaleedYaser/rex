#pragma once

#include "rex-raster/exports.h"

#include <rex-core/vec.h>
#include <rex-math/types.h>

namespace rex::raster
{
	struct Mesh
	{
		rc::Vec<math::V3> position;
		rc::Vec<math::V3> normal;
		rc::Vec<math::V2> uv;
		rc::Vec<math::Color_F32> color;
		rc::Vec<unsigned> indices;
		rc::Vec<unsigned> uv_indices;

		math::V3 bb_min;
		math::V3 bb_max;
	};

	REX_RASTER_EXPORT Mesh mesh_init();
	REX_RASTER_EXPORT void mesh_deinit(Mesh& self);

	REX_RASTER_EXPORT Mesh mesh_triangle();
	REX_RASTER_EXPORT Mesh mesh_quad();
	REX_RASTER_EXPORT Mesh mesh_cube();

	REX_RASTER_EXPORT Mesh mesh_from_stl(const char* path);
	REX_RASTER_EXPORT Mesh mesh_from_obj(const char* path);
}
