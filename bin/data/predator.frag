#version 120
#define TESTS 25.0

uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iGlobalTime;           // shader playback time (in seconds)
uniform sampler2DRect iChannel0;
uniform float waveAmplitude;
uniform bool predatorMode;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    vec2 uv = gl_FragCoord.xy;
    uv.x += waveAmplitude * sin((uv.y+iGlobalTime*50.0)/50.0);
    
    if (predatorMode){
        
        float t = iGlobalTime;
        vec4 c = vec4(0.0);
        for(float i = 0.; i< TESTS; i++){
            c.rgb = max(c.rgb,
                        sin(i/25.+
                            6.*(vec3( 0.,.9,.95  )+
                                texture2DRect(
                                              iChannel0,vec2(
                                                             uv.x,uv.y-(i/iResolution.y))
                                              ).rgb
                                ))*.5+.5);
        }
        
        c.rgb = sin(( vec3(0.,.1,.2)+c.rgb+uv.y)*6.28)*.5+.5;
        c.a = 1.0;
        
        vec4 originalColor = texture2DRect(iChannel0, uv);
        gl_FragColor = mix(c, originalColor, (sin(iGlobalTime)/2)+.5);
        
//        gl_FragColor = c/2 + originalColor/2;
    
    } else {
        vec4 color = texture2DRect(iChannel0,  uv);
        gl_FragColor = color;
    }
    




//    vec2 uv = gl_FragCoord.xy;
//    uv.x += waveAmplitude * sin((uv.y+iGlobalTime*50.0)/50.0);
//
//	// Get the pixel color at the index.
//	vec4 color = texture2DRect(iChannel0,  uv);
//
//	gl_FragColor = color + c;
}

