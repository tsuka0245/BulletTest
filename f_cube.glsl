#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform bool selected;

varying vec2 v_uv;

void main()
{
    gl_FragColor = texture2D(texture, v_uv);
    if(selected == true)gl_FragColor += vec4(vec3(0.3),0.0);
}
