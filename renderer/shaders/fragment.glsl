#version 330 core
uniform sampler2D u_grid;
uniform float u_time;
uniform vec2 u_resolution;
out vec4 FragColor;

void main()
{
	vec2 uv = gl_FragCoord.xy / u_resolution;
	float cell = texture(u_grid, uv).r;  // 1.0 = vivant, 0.0 = mort
	vec2 st = gl_FragCoord.xy/u_resolution;

	if (cell > 0.5)
	{
		// cellule vivante → glow orange
		FragColor = vec4(1.0, 0.5 + 0.5 * sin(u_time * 4.0), 0.0, 1.0);
		//FragColor = vec4(st.x, st.y, abs(sin(u_time * 2.0 )), 1.0);
	}
	else
	{
		// cellule morte → bleu nuit
		FragColor = vec4(0.0, 0.05, 0.15, 1.0);
	}
}