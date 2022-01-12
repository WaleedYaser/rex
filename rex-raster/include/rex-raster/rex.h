#pragma once

#include "rex-raster/canvas.h"
#include "rex-raster/mesh.h"
#include "rex-raster/camera.h"

#include <rex-core/api.h>

namespace rex::raster
{
	struct Rex: Rex_Api
	{
		Canvas canvas;
		Camera cam;
		Mesh mesh;
		// TODO: use image instead of canvas
		Canvas texture;
		math::Color_F32 mesh_color;
	};
}
