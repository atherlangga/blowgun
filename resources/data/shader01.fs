varying mediump vec2		v_vertex_texture;
uniform mediump sampler2D	u_texture;

void main (void)
{
	gl_FragColor = texture2D(u_texture, v_vertex_texture);
} 

