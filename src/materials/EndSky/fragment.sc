#ifndef INSTANCING
$input v_texcoord0, v_posTime
#endif

#include <bgfx_shader.sh>

#ifndef INSTANCING
  #include <newb/main.sh>

  SAMPLER2D_AUTOREG(s_SkyTexture);
#endif

void main() {
  #ifndef INSTANCING
    vec4 diffuse = texture2D(s_SkyTexture, v_texcoord0);

    vec3 color = renderEndSky(getEndHorizonCol(), getEndZenithCol(), normalize(v_posTime.xyz), v_posTime.w);
    color += mix(vec3(0.0, 0.0, 0.0), 1.8*diffuse.rgb, 0.4*smoothstep(-0.5, 1.0, normalize(v_posTime.y))); // stars

    color = colorCorrection(color);

    gl_FragColor = vec4(color, 1.0);
  #else
    gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  #endif
}
