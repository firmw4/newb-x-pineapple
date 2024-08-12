$input a_color0, a_position, a_texcoord0
$output v_color0, v_worldPos, v_time

#include <bgfx_shader.sh>

#ifndef INSTANCING
uniform vec4 ViewPositionAndTime;
#endif

void main() {
#ifndef INSTANCING
  mat4 model = u_model[0];
  vec3 v_worldPos = a_position;
  vec3 pos = (u_model[0] * vec4(v_worldPos, 1.0)).xyz;
  v_color0 = a_color0;
  v_worldPos = pos;
  v_time = ViewPositionAndTime.w;
  gl_Position = mul(u_viewProj, vec4(v_worldPos, 1.0));
#else
  gl_Position = vec4(0.0,0.0,0.0,0.0);
#endif
}