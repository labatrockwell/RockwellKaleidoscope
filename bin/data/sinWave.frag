#version 120

uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iGlobalTime;           // shader playback time (in seconds)
uniform sampler2DRect iChannel0;
uniform float waveAmplitude;

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

	// Get the pixel color at the index.
	vec4 color = texture2DRect(iChannel0,  uv);
    
//    vec3 hueShift = rgb2hsv( vec3(color.r,color.g,color.b) );
//    hueShift.x += (( sin(iGlobalTime)/2 ) + .5)/10;
//    vec3 colorShift = hsv2rgb(hueShift);
    
//    vec4 colorShift = vec4( (sin(iGlobalTime)/2) + .5 );
    
//    color.r = colorShift.r;
//    color.g = colorShift.g;
//    color.b = colorShift.b;
    
	gl_FragColor = color;
}

