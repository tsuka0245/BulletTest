#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

//uniform sampler2D texture;
uniform bool selected;

varying vec2 v_uv;

void main()
{
    gl_FragColor = vec4(0.2,0.6,0.2,1.0);
    if(selected == 1)gl_FragColor += vec4(vec3(0.3),0.0);
}
