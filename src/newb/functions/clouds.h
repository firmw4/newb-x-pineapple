#ifndef CLOUDS_H
#define CLOUDS_H

#include "noise.h"

// simple clouds 2D noise
float cloudNoise2D(vec2 p, highp float t, float rain) {
  t *= NL_CLOUD1_SPEED;
  p += t;
  p.x += sin(p.y*0.4 + t);

  vec2 p0 = floor(p);
  vec2 u = p-p0;
  u *= u*(3.0-2.0*u);
  vec2 v = 1.0-u;

  // rain transition
  vec2 d = vec2(0.09+0.5*rain,0.089+0.5*rain*rain);

  return v.y*(randt(p0,d)*v.x + randt(p0+vec2(1.0,0.0),d)*u.x) +
         u.y*(randt(p0+vec2(0.0,1.0),d)*v.x + randt(p0+vec2(1.0,1.0),d)*u.x);
}

// simple clouds
vec4 renderCloudsSimple(vec3 pos, highp float t, float rain, vec3 zenithCol, vec3 horizonCol, vec3 fogCol) {
  pos.xz *= NL_CLOUD1_SCALE;

  float cloudAlpha = cloudNoise2D(pos.xz, t, rain);
  float cloudShadow = cloudNoise2D(pos.xz*0.91, t, rain);

  vec4 color = vec4(0.02,0.04,0.05,cloudAlpha);

  color.rgb += fogCol;
  color.rgb *= 1.0 - 0.5*cloudShadow*step(0.0, pos.y);

  color.rgb += zenithCol*0.7;
  color.rgb *= 1.0 - 0.4*rain;

  return color;
}

// rounded clouds

// 2d noise
float noise(vec2 p){
  vec2 p0 = floor(p);
  vec2 u = p-p0;

  u *= u*(3.0-2.0*u);
  vec2 v = 1.0 - u;

  float c1 = rand(p0);
  float c2 = rand(p0+vec2(1.0,0.0));
  float c3 = rand(p0+vec2(0.0,1.0));
  float c4 = rand(p0+vec2(1.0,1.0));

  float n = v.y*mix(c1,c2,u.x) + u.y*(c3*v.x+c4*u.x);
  return n;
}

//   apply bevel with radius r at at corner (1.0)
float bevel(float x, float r) {
	 //return smoothstep(0.5,1.5,x);
  float y = max(x-r,0.0);
  return r+sqrt(1.0-2.0*r+r*r-y*y);
}

#ifdef NL_3DNOISE
// rounded clouds 3D density map
float cloudDf(vec3 pos, float rain) {
  pos.x += 0.64*noise(6.0*pos.xz);
  pos.y += 0.2*noise(0.1*pos.xz);
  pos.z += 0.1*noise(0.0*pos.xz);

  vec2 p0 = floor(pos.xz);
  vec2 u = pos.xz - p0;
  //u = smoothstep(0.99*NL_CLOUD2_SHAPE,1.0,u);
  u = max((u-NL_CLOUD2_SHAPE)/(1.0-NL_CLOUD2_SHAPE),0.0);
  //u = 3.0*u*u - 2.0*u*u*u;
  vec2 v = 1.0 - u;

  // rain transition
  vec2 t = vec2(0.3001+0.2*rain, 0.2999+0.2*rain*rain);

  // mix noise gradients
  float n = v.y*(randt(p0,t)*v.x + randt(p0+vec2(1.0,0.0),t)*u.x) +
            u.y*(randt(p0+vec2(0.0,1.0),t)*v.x + randt(p0+vec2(1.0,1.0),t)*u.x);

  float b = bevel(2.0*abs(pos.y-0.5), 0.3);
  return n*b;
}

vec4 renderClouds(vec3 vDir, vec3 vPos, float rain, float time, vec3 fogCol, vec3 skyCol) {
  // scaled ray offset
  float height = 7.0*(NL_CLOUD2_THICKNESS + rain*(NL_CLOUD2_RAIN_THICKNESS - NL_CLOUD2_THICKNESS));
  vec3 deltaP;

  // local cloud pos
  vec3 pos = vPos;
  pos.y = 0.0;
  pos.xz = NL_CLOUD2_SCALE*(vPos.xz + vec2(1.0,0.5)*(time*NL_CLOUD2_VELOCIY));

  deltaP.xz = (NL_CLOUD2_SCALE*height)*vDir.xz;
  deltaP.y = vDir.y;
  deltaP /= float(NL_CLOUD2_STEPS)*(0.02+0.98*abs(vDir.y));

  // alpha, gradient, ray depth temp
  vec3 d = vec3(0.0,1.0,1.0);
  for (int i=0; i<NL_CLOUD2_STEPS; i++) {
    pos += deltaP;
    float m = cloudDf(pos.xyz, rain);
    d.x += m;
    d.y = mix(d.y, pos.y, d.z);
    d.z *= 1.0 - m;
  }
  d.x = d.x/(float(NL_CLOUD2_STEPS)/NL_CLOUD2_DENSITY + d.x);
  d.x = smoothstep(0.3,0.6,d.x);
  if (vPos.y > 0.0) {
  	     d.y = 1.0 - d.y;
  }

  d.y = 1.0-0.8*d.y*d.y;

// add custom cloud tint
  float night = smoothstep(0.3, 0.1, fogCol.r);
  float dusk = smoothstep(1.0, 0.0, fogCol.b)*(1.0 - night);
  vec3 cloudTint = mix(mix(vec3(0.32,0.34,0.42), vec3(0.63,0.417,0.3), dusk), vec3(0.05,0.071,0.1), night);

  vec4 col = vec4((cloudTint*1.4), d.x*0.6);
  //vec4 col = vec4(skyCol*1.2, d.x*0.8);
  col.rgb += rain + fogCol*d.y;
  col.rgb *= 1.0 - 0.73*rain;

  return col;
}

#else
// rounded clouds 3D density map
float cloudDf(vec3 pos, float rain) {
  vec2 p0 = floor(pos.xz);
  vec2 u = smoothstep(0.999*NL_CLOUD2_SHAPE, 1.0, pos.xz-p0);
  
  // rain transition
  vec2 t = vec2(0.3001+0.2*rain, 0.2999+0.2*rain*rain);

  float n = mix(
    mix(randt(p0, t),randt(p0+vec2(1.0,0.0), t), u.x),
    mix(randt(p0+vec2(0.0,1.0), t),randt(p0+vec2(1.0,1.0), t), u.x),
    u.y
  );
	
  // round y
  float b = 1.0 - 1.9*smoothstep(NL_CLOUD2_SHAPE, 2.0 - NL_CLOUD2_SHAPE, 2.0*abs(pos.y-0.5));
  return smoothstep(0.2, 1.0, n * b);
}

vec4 renderClouds(vec3 vDir, vec3 vPos, float rain, float time, vec3 fogCol, vec3 skyCol) {

  float height = 7.0*mix(NL_CLOUD2_THICKNESS, NL_CLOUD2_RAIN_THICKNESS, rain);

  // scaled ray offset
  vec3 deltaP;
  deltaP.y = 1.0;
  deltaP.xz = (NL_CLOUD2_SCALE*height)*vDir.xz/(0.02+0.98*abs(vDir.y));
  //deltaP.xyz = (NL_CLOUD2_SCALE*height)*vDir.xyz;
  //deltaP.y = abs(deltaP.y);
  
  // local cloud pos
  vec3 pos;
  pos.y = 0.0;
  pos.xz = NL_CLOUD2_SCALE*(vPos.xz + vec2(1.0,0.5)*(time*NL_CLOUD2_VELOCIY));
  pos += deltaP;

  deltaP /= -float(NL_CLOUD2_STEPS);
  
  // alpha, gradient
  vec2 d = vec2(0.0,1.0);
  for (int i=1; i<=NL_CLOUD2_STEPS; i++) {
    float m = cloudDf(pos, rain);
    
    d.x += m;
    d.y = mix(d.y, pos.y, m);
    
    //if (d.x == 0.0 && i > NL_CLOUD2_STEPS/2) {
    //	break;
    //} 

    pos += deltaP;
  }
  //d.x *= vDir.y*vDir.y; 
  d.x *= smoothstep(0.03, 0.1, d.x);
  d.x = d.x / ((float(NL_CLOUD2_STEPS)/NL_CLOUD2_DENSITY) + d.x);
  
  if (vPos.y > 0.0) { // view from bottom
    d.y = 1.0 - d.y;
  }

  d.y = 1.0 - 0.7*d.y*d.y;
 
// add custom cloud tint
  float night = smoothstep(0.3, 0.1, fogCol.r);
  float dusk = smoothstep(1.0, 0.0, fogCol.b)*(1.0 - night);
  vec3 cloudTint = mix(mix(vec3(0.42,0.61,0.82), vec3(1.6,0.45,0.14), dusk), vec3(0.0,0.021,0.08), night);

  vec4 col = vec4((cloudTint*1.4), d.x*0.6);
  //vec4 col = vec4(skyCol*1.2, d.x*0.8);
  col.rgb += rain + fogCol*d.y;
  col.rgb *= 1.0 - 0.7*rain;

  return col;
}
#endif


// aurora is rendered on clouds layer
#ifdef NL_AURORA
vec4 renderAurora(vec3 p, float t, float rain, vec3 FOG_COLOR) {
  p.xz *= NL_AURORA_SCALE;
  t *= NL_AURORA_VELOCITY;
  p.xz += 0.05 * sin(p.x * 8.0 * t);

  // Introduce more complex wave patterns for realistic aurora
  float wave1 = cos(p.x * 0.15 + t + cos(p.z * 0.28));
  float wave2 = cos(p.z * 0.18 - t + cos(p.x * 0.25));
  float wave3 = sin(p.z * 0.16 - t + cos(p.x * 0.26));
  float wave4 = sin(p.x * 0.17 + 1.0 * sin(wave2 + wave1 + wave3 * 1.5) + wave3 * 1.0 + wave1 * 1.5 + wave2 * 1.0);

  // Square the wave values to create sharp contrasts
  wave1 *= wave1; wave2 *= wave2; wave3 *= wave3; wave4 *= wave4;
  wave3 = wave1 / (1.0 + wave3 * wave4 / NL_AURORA_WIDTH);

  // Modify fade mask for more subtle blending
  float fadeMask = 1.0 / (1.0 + 48.0 * FOG_COLOR.b * FOG_COLOR.b);
  //float fadeMask = (1.0-0.8*rain)*max(1.0 - 4.0*max(FOG_COLOR.b, FOG_COLOR.g), 0.0);

  // Blend colors dynamically for a more vibrant aurora
  return vec4(NL_AURORA * mix(mix(NL_AURORA_COL1, NL_AURORA_COL2, wave1), NL_AURORA_COL3, wave2), 1.0) * wave3 * fadeMask;
}
#endif

#endif
