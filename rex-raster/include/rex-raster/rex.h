#pragma once

#include "rex-raster/canvas.h"
#include "rex-raster/mesh.h"

#include <rex-core/api.h>

namespace rex::raster
{
	struct Rex: Rex_Api
	{
		Canvas canvas;
		Mesh mesh;
		math::Color_F32 mesh_color;
	};
}
