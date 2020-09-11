#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

varying vec2 v_uv;

void main()
{
    if(v_uv == vec2(0.0,0.0))
    {
        gl_FragColor = vec4(1.0,0.0,0.0,1.0);
    }
    else if(v_uv == vec2(1.0,1.0))
    {
        gl_FragColor = vec4(0.0,1.0,0.0,1.0);
    }
    else if(v_uv == vec2(0.0,1.0))
    {
        gl_FragColor = vec4(0.0,0.0,1.0,1.0);
    }
}
