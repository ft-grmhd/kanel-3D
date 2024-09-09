R"nzsl(

	[nzsl_version("1.0")]
	module;

	struct VertIn
	{
		[builtin(vertex_index)] vert_index: i32
	}

	struct VertOut
	{
		[location(0)] uv: vec2[f32],
		[builtin(position)] position: vec4[f32]
	}

	const vertices = array[vec2[f32]](
		vec2[f32](-1.0, -3.0),
		vec2[f32](-1.0, 1.0),
		vec2[f32]( 3.0, 1.0)
	);

	[entry(vert)]
	fn main(input: VertIn) -> VertOut
	{
		let position = vertices[input.vert_index];

		let output: VertOut;
		output.position = vec4[f32](position, 0.0, 1.0);
		output.uv = position * 0.5 + vec2[f32](0.5, 0.5);

		return output;
	}

)nzsl"
