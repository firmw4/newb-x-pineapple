#ifndef FOG_H
#define FOG_H

#include "tonemap.h"

float nlRenderFogFade(float relativeDist, vec3 FOG_COLOR, vec2 FOG_CONTROL) {
  #if NL_FOG_TYPE == 0
  // no fog
    return 0.0;

  #else
    float fade;
      if (NL_FOG_TYPE == 1) {
      // linear transition
         fade = clamp((relativeDist - FOG_CONTROL.x) / (FOG_CONTROL.y - FOG_CONTROL.x), 0.0, 1.0);
      } else if (NL_FOG_TYPE == 2) { 
         fade = smoothstep(FOG_CONTROL.x, FOG_CONTROL.y, relativeDist);
      } else if (NL_FOG_TYPE == 3) {
      // exponential fog
         float fogDensity = 0.01; 
         fade = smoothstep(FOG_CONTROL.x, FOG_CONTROL.y, relativeDist);
         fade = 1.0-exp(-relativeDist*fogDensity);
      } else if (NL_FOG_TYPE == 4) {
       // cubic interpolation
         float fogDensity = 0.01;
         float fogFactor = exp(-relativeDist*fogDensity);
         float t = (relativeDist - FOG_CONTROL.x) / (FOG_CONTROL.y - FOG_CONTROL.x);
         float smoothFade = smoothstep(0.0, 1.0, t);
         fade = mix(1.0-fogFactor, smoothFade, smoothFade);
      }

      // misty effect
      float mistDensity = NL_MIST_DENSITY * (19.0 - 18.0 * FOG_COLOR.g);
      fade += (1.0-fade)*(0.3-0.3 * exp(-relativeDist*relativeDist*mistDensity));

    return fade;
  #endif
}

float nlRenderGodRayIntensity(vec3 cPos, vec3 worldPos, float t, vec2 uv1, float relativeDist, vec3 FOG_COLOR) {
  // offset wPos (only works up to 16 blocks)
  vec3 offset = cPos - 16.0 * fract(worldPos * 0.0625);
  offset = abs(2.0 * fract(offset * 0.0625) - 1.0);
  offset = offset * offset * (3.0 - 2.0 * offset);

  vec3 nrmof = normalize(worldPos);

  // Calculate angular influence and diffusion
  float u = nrmof.z / length(nrmof.zy);
  float diff = dot(offset, vec3(0.1, 0.2, 1.0)) + 0.07 * t;
  float mask = nrmof.x * nrmof.x;

  // Enhanced volumetric light calculation for lower altitudes
  float vol = sin(7.0 * u + 1.5 * diff) * sin(3.0 * u + diff);
  vol *= vol * mask * uv1.y * (1.5 - mask * mask);
  vol *= relativeDist * relativeDist;

  vol *= clamp(3.5 * (FOG_COLOR.r - FOG_COLOR.b), 0.0, 1.0);

  vol = smoothstep(0.0, 0.1, vol);

  return vol;
}

#endif
