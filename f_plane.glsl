#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

varying vec2 v_uv;
varying float pos_distance;

void main()
{
    vec2 scaled_uv = v_uv*10;
    float line_width = 0.01*pos_distance;
    if(fract(scaled_uv.x)>(1.0-line_width) || fract(scaled_uv.x) < line_width ||
       fract(scaled_uv.y)>(1.0-line_width) || fract(scaled_uv.y) < line_width)
    {
        gl_FragColor = vec4(1.0,1.0,1.0,1.0);
    }
    else gl_FragColor = vec4(0);
}
