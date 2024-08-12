$input v_color0, v_worldPos, v_time

#include <bgfx_shader.sh>

#ifndef INSTANCING
uniform vec4 StarsColor;
#endif

void main() {
#ifndef INSTANCING
  vec4 color = v_color0;
  color.rgb *= StarsColor.rgb * v_color0.a;
  color.rgb *= sin(v_worldPos.x+v_time + cos(v_worldPos.y))*cos(3.*v_worldPos.y+v_time*1.4);
  gl_FragColor = color;
#else
  gl_FragColor = vec4(0.0,0.0,0.0,0.0);
#endif
}