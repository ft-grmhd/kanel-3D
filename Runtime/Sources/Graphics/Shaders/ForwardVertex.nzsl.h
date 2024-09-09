R"nzsl(

	[nzsl_version("1.0")]
	module;

	import ViewerData from Engine.ViewerData;

	struct VertIn
	{
		[location(0)] pos: vec4[f32],
		[location(1)] color: vec4[f32],
		[location(2)] normal: vec4[f32],
		[location(3)] uv: vec2[f32]
	}

	struct VertOut
	{
		[location(0)] color : vec4[f32],
		[location(1)] uv : vec2[f32],
		[location(2)] norm : vec4[f32],
		[location(3)] norm_mat : mat4[f32],
		[builtin(position)] pos: vec4[f32]
	}

	struct ModelData
	{
		matrix: mat4[f32],
		normal: mat4[f32],
	}

	external
	{
		[set(0), binding(0)] viewer_data: uniform[ViewerData],
		model: push_constant[ModelData]
	}

	[entry(vert)]
	fn main(input: VertIn) -> VertOut
	{
		let output: VertOut;
		output.color = input.color;
		output.uv = input.uv;
		output.norm = input.normal;
		output.norm_mat = model.normal;
		output.pos = viewer_data.view_proj_matrix * model.matrix * input.pos;
		return output;
	}

)nzsl"
