#ifndef KANEL_3D_NZSL_MODULES
#define KANEL_3D_NZSL_MODULES

#include <string>

namespace kbh
{
	const std::string viewer_data = R"nzsl(
		[nzsl_version("1.0")]
		module Engine.ViewerData;

		[export]
		[layout(std140)]
		struct ViewerData
		{
			projection_matrix: mat4[f32],
			inv_projection_matrix: mat4[f32],
			view_matrix: mat4[f32],
			inv_view_matrix: mat4[f32],
			view_proj_matrix: mat4[f32],
			inv_view_proj_matrix: mat4[f32],
		}
	)nzsl";
}

#endif
