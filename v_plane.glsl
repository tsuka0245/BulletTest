#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat4 model_mat;

attribute vec3 a_pos;
attribute vec2 a_uv;

varying vec2 v_uv;
varying float pos_distance;

void main()
{
    gl_Position = proj_mat * view_mat * model_mat * vec4(a_pos,1.0);
    v_uv = a_uv;
    pos_distance = length(a_pos);
}
