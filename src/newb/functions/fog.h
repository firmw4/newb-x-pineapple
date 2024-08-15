#ifndef FOG_H
#define FOG_H

#include "tonemap.h"

float nlRenderFogFade(float relativeDist, vec3 FOG_COLOR, vec2 FOG_CONTROL) {
  #if NL_FOG_TYPE == 0
    // no fog
    return 0.0;
  #endif

  #if NL_FOG_TYPE == 1
    // linear transition
    float fade = clamp((relativeDist - FOG_CONTROL.x) / (FOG_CONTROL.y - FOG_CONTROL.x), 0.0, 1.0);
  #else
    // smoother transition using smoothstep
    float fade = smoothstep(FOG_CONTROL.x, FOG_CONTROL.y, relativeDist);
  #endif

  // Optimized misty effect for smoother transitions
  float density = NL_MIST_DENSITY * (5.0 - FOG_COLOR.g); // Simplified density calculation
  float mistFactor = exp(-relativeDist * relativeDist * density); // More efficient calculation
  fade += (1.0 - fade) * (0.4 - 0.4 * mistFactor);
  
  /*
  // misty effect
  float density = NL_MIST_DENSITY*(20.0 - 18.0*FOG_COLOR.g);
  fade += (1.0-fade)*(0.3-0.3*exp(-relativeDist*relativeDist*density));
  */

  return fade;
}

float nlRenderGodRayIntensity(vec3 cPos, vec3 worldPos, float t, vec2 uv1, float relativeDist, vec3 FOG_COLOR) {
  // offset wPos (only works up to 16 blocks)
  vec3 offset = cPos - 16.0 * fract(worldPos * 0.0625);
  offset = abs(2.0 * fract(offset * 0.0625) - 1.0);
  offset = offset * offset * (3.0 - 2.0 * offset);

  // Normalize world position
  vec3 nrmof = normalize(worldPos);

  // Calculate angular influence and diffusion
  float u = nrmof.z / length(nrmof.zy);
  float diff = dot(offset, vec3(0.1, 0.2, 1.0)) + 0.07 * t;
  float mask = nrmof.x * nrmof.x;

  // Enhanced volumetric light calculation for lower altitudes
  float vol = sin(7.0 * u + 1.5 * diff) * sin(3.0 * u + diff);
  vol *= vol * mask * uv1.y * (1.2 - mask * mask); // Increased intensity at lower altitudes
  vol *= relativeDist * relativeDist;

  // Dawn/dusk mask enhanced for brighter and lower-altitude flares
  vol *= clamp(3.5 * (FOG_COLOR.r - FOG_COLOR.b), 0.0, 1.0); // Increased brightness factor
  
  // Increase overall brightness and visibility
  vol = smoothstep(0.0, 0.05, vol); // Reduced threshold for stronger flare effect
  
  /*
  //offset = 0.5 + 0.5*cos(offset*0.392699082);

  //vec3 ofPos = wPos+offset;
  vec3 nrmof = normalize(worldPos);

  float u = nrmof.z/length(nrmof.zy);
  float diff = dot(offset,vec3(0.1,0.2,1.0)) + 0.07*t;
  float mask = nrmof.x*nrmof.x;

  float vol = sin(7.0*u + 1.5*diff)*sin(3.0*u + diff);
  vol *= vol*mask*uv1.y*(1.0-mask*mask);
  vol *= relativeDist*relativeDist;

  // dawn/dusk mask
  vol *= clamp(3.0*(FOG_COLOR.r-FOG_COLOR.b), 0.0, 1.0);
  
  vol = smoothstep(0.0, 0.1, vol);
  */
  return vol;
}

#endif
