#ifndef KANEL_3D_VIEWER_DATA
#define KANEL_3D_VIEWER_DATA

#include <Maths/Mat4.h>

namespace kbh
{
	struct ViewerData
	{
		Mat4f projection_matrix;
		Mat4f inv_projection_matrix;
		Mat4f view_matrix;
		Mat4f inv_view_matrix;
		Mat4f view_proj_matrix;
		Mat4f inv_view_proj_matrix;
	};
}

#endif
