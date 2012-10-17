attribute mediump vec4	a_vertex_position;
attribute mediump vec2	a_vertex_texture;
varying mediump vec2	v_vertex_texture;
uniform mediump mat4	u_PMV_matrix;

void main(void)
{
	v_vertex_texture = a_vertex_texture;
	gl_Position = u_PMV_matrix * a_vertex_position;
}