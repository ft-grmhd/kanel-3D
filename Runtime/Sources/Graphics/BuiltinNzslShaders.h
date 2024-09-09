#ifndef KANEL_3D_BUILTIN_SHADERS
#define KANEL_3D_BUILTIN_SHADERS

#include <string>

namespace kbh
{
	const std::string forward_vertex_shader =
		#include <Graphics/Shaders/ForwardVertex.nzsl.h>
	;

	const std::string forward_fragment_shader =
		#include <Graphics/Shaders/ForwardFragment.nzsl.h>
	;

	const std::string screen_vertex_shader =
		#include <Graphics/Shaders/ScreenVertex.nzsl.h>
	;

	const std::string screen_fragment_shader =
		#include <Graphics/Shaders/ScreenFragment.nzsl.h>
	;
}

#endif
