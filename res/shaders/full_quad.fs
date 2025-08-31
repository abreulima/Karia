#version 300 es

precision mediump float;

uniform sampler2D g_color;
uniform sampler2D g_normal;
uniform sampler2D g_depth;

uniform float nearPlane;
uniform float farPlane;
uniform vec2 resolution;

out vec4 FragColor;
in vec2 TexCoord;

float get_linear_depth(vec2 uv, sampler2D depth_texture)
{
    float depth = texture(depth_texture, uv).r;
    return (depth - nearPlane) / (farPlane - nearPlane);
}
vec3 get_normal(vec2 uv, sampler2D normalTexture)
{
    vec3 normal = texture(normalTexture, uv).rgb;
    //ec3 normal = texture2DAA(normalTexture, uv).rgb;
    normal = normal * 2.0 - 1.0;
    return normal;
}

float normal_edges_bias(vec3 edge_bias, vec3 normal, vec3 nNormal)
{
	float normal_diff = dot(normal - nNormal, edge_bias);
	float normalIndicator = clamp(smoothstep(-.01, .01, normal_diff), 0.0, 1.0);
	return distance(normal, nNormal) *  normalIndicator;
}

float depth_edge(float depth, float shifted_depth)
{
	float depthDifference = clamp(shifted_depth - depth, 0.0, 1.0);
	float invDepthDifference = depth - shifted_depth;
	return depthDifference;
}


void main()
{
    vec2 texelSize = vec2(1.0) / resolution;
    vec2 uv = gl_FragCoord.xy / resolution;
	ivec2 texelCoord = ivec2(gl_FragCoord.xy);

	vec2 UV_OFFSETS[4];
	UV_OFFSETS[0] = uv + vec2(-1.0, 0.0) * texelSize;
	UV_OFFSETS[1] = uv + vec2(+1.0, 0.0) * texelSize;
	UV_OFFSETS[2] = uv + vec2(0.0, -1.0) * texelSize;
	UV_OFFSETS[3] = uv + vec2(0.0, +1.0) * texelSize;

	// Edges By Depth
    //float depth_texture = texture(g_depth, uv).r;

	//float depth = get_linear_depth(uv, g_depth);

	float depth       = texelFetch(g_depth, texelCoord, 0).r;
	float depth_left  = texelFetch(g_depth, texelCoord + ivec2(-1, 0), 0).r;
	float depth_right = texelFetch(g_depth, texelCoord + ivec2( 1, 0), 0).r;
	float depth_top   = texelFetch(g_depth, texelCoord + ivec2( 0,-1), 0).r;
	float depth_bottom= texelFetch(g_depth, texelCoord + ivec2( 0, 1), 0).r;

	/*
    	for (int i = 0; i < UVOffsets.length(); i++){
    		float dOff = GetLinearDepth(UVOffsets[i],DEPTH_TEXTURE, INV_PROJECTION_MATRIX, outlineMask);
    		depthDifference += clamp(dOff - depth, 0.0, 1.0);
    		invDepthDifference += depth - dOff;
    	}
	*/

	float depth_diff = 0.0;
	depth_diff += depth_edge(depth, depth_left);
	depth_diff += depth_edge(depth, depth_right);
	depth_diff += depth_edge(depth, depth_top);
	depth_diff += depth_edge(depth, depth_bottom);

	depth_diff = step(0.015, depth_diff);

	// Edges By Normal
    vec3 normal_texture = texture(g_normal, uv).rgb;
	vec3 normal       = texelFetch(g_normal, texelCoord, 0).rgb;
	vec3 normal_left  = texelFetch(g_normal, texelCoord + ivec2(-1, 0), 0).rgb;
	vec3 normal_right = texelFetch(g_normal, texelCoord + ivec2( 1, 0), 0).rgb;
	vec3 normal_top   = texelFetch(g_normal, texelCoord + ivec2( 0,-1), 0).rgb;
	vec3 normal_bottom= texelFetch(g_normal, texelCoord + ivec2( 0, 1), 0).rgb;

    float normal_diff = 0.0;
    vec3 bias = vec3(1.0, 1.0, 1.0);

    normal_diff += normal_edges_bias(bias, normal, normal_left);
    normal_diff += normal_edges_bias(bias, normal, normal_right);
    normal_diff += normal_edges_bias(bias, normal, normal_top);
    normal_diff += normal_edges_bias(bias, normal, normal_bottom);

	//normal_diff = smoothstep(0.2, 0.2, normal_diff);
	normal_diff = smoothstep(0.15, 0.25, normal_diff);


	FragColor = vec4(vec3(normal_diff + depth_diff), 1.0);

	vec3 color_uv;

	//FragColor = vec4((depth_diff + normal_diff) * uv.xy, 0.0, 1.0);

}