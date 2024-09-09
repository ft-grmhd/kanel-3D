R"nzsl(

	[nzsl_version("1.0")]
	module;

	struct VertOut
	{
		[location(0)] color : vec4[f32],
		[location(1)] uv : vec2[f32],
		[location(2)] norm : vec4[f32],
		[location(3)] norm_mat : mat4[f32],
		[builtin(position)] pos: vec4[f32]
	}

	struct FragOut
	{
		[location(0)] color: vec4[f32]
	}

	external
	{
		[set(1), binding(0)] u_albedo: sampler2D[f32],
	}

	[entry(frag)]
	fn main(input: VertOut) -> FragOut
	{
		if(input.color.a == 0.0)
			discard;

		const ambient = vec3[f32](0.3, 0.3, 0.3);
		const directional_color = vec3[f32](5.0, 5.0, 5.0);
		const intensity = 1.0;

		let directional_vector = normalize(vec3[f32](0.85, 0.8, 0.75));

		let transformed_normal = mat3[f32](input.norm_mat) * input.norm.xyz;

		let directional : f32 = max(dot(transformed_normal.xyz, directional_vector), 0.0);
		let lighting : vec3[f32] = (ambient + (directional_color * directional)) * intensity;

		let output: FragOut;
		output.color = input.color * u_albedo.Sample(input.uv) * vec4[f32](lighting, 1.0);
		return output;
	}

)nzsl"
