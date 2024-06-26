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

// apply bevel with radius r at at corner (1.0)
float bevel(float x, float r) {
	 //return smoothstep(0.5,1.5,x);
  float y = max(x-r,0.0);
  return r+sqrt(1.0-2.0*r+r*r-y*y);
}

// rounded clouds 3D density map
float cloudDf(vec3 pos, float rain) {
  vec2 p0 = floor(pos.xz);
  vec2 u = pos.xz - p0;
  //u = smoothstep(0.99*NL_CLOUD2_SHAPE,1.0,u);
  u = max((u-NL_CLOUD2_SHAPE)/(1.0-NL_CLOUD2_SHAPE),0.0);
  u = 3.0*u*u - 2.0*u*u*u;
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
  // local cloud pos
  vec3 pos = vPos;
  pos.y = 0.0;
  pos.xz = NL_CLOUD2_SCALE*(vPos.xz + vec2(1.0,0.5)*(time*NL_CLOUD2_VELOCIY));

  // scaled ray offset
  float height = 7.0*(NL_CLOUD2_THICKNESS + rain*(NL_CLOUD2_RAIN_THICKNESS - NL_CLOUD2_THICKNESS));
  vec3 deltaP;
  deltaP.xyz = (NL_CLOUD2_SCALE*height/float(NL_CLOUD2_STEPS))*vDir.xyz/(0.02+0.98*abs(vDir.y));
  deltaP.y = abs(deltaP.y);

  // alpha, gradient, ray depth temp
  vec3 d = vec3(0.0,1.0,1.0);
  for (int i=0; i<NL_CLOUD2_STEPS; i++) {
    pos += deltaP;
    float m = cloudDf(pos.xyz, rain);
    d.x += m;
    d.y = mix(d.y, pos.y, d.z);
    d.z *= 1.0 - m;
  }

  d.x = smoothstep(0.2,0.4,d.x);

  if (vPos.y > 0.0) {
    d.y = 1.0 - d.y;
  }

  d.y = 1.0 - 1.06*d.y*d.x;

// add custom clouds tint
  float night = smoothstep(0.3, 0.1, fogCol.r);
  float dusk = smoothstep(1.0, 0.0, fogCol.b)*(1.0 - night);
  vec3 cloudTint = mix(mix(vec3(0.23,0.41,1.0), vec3(0.7,0.417,0.3), dusk), vec3(0.0,0.021,0.08), night);

  vec4 col = vec4((cloudTint*1.4), d.x*0.6);
  //vec4 col = vec4(skyCol*1.2, d.x*0.8);
  col.rgb += rain + fogCol*d.y;
  col.rgb *= 1.0 - 0.7*rain;

  return col;
}

// aurora is rendered on clouds layer
#ifdef NL_AURORA
vec4 renderAurora(vec3 p, float t, float rain, vec3 FOG_COLOR) {
  t *= NL_AURORA_VELOCITY;
  p.xz *= NL_AURORA_SCALE;
  p.xz += 0.05*sin(p.x*4.0 + 20.0*t);

  float d0 = sin(p.x*0.1 + t + sin(p.z*0.2));
  float d1 = sin(p.z*0.1 - t + sin(p.x*0.2));
  float d2 = sin(p.z*0.1 + 1.0*sin(d0 + d1*2.0) + d1*2.0 + d0*1.0);
  d0 *= d0; d1 *= d1; d2 *= d2;
  d2 = d0/(1.0 + d2/NL_AURORA_WIDTH);

  float mask = (1.0-0.8*rain)*max(1.0 - 3.0*max(FOG_COLOR.b, FOG_COLOR.g), 0.0);
  return vec4(NL_AURORA*mix(NL_AURORA_COL1,NL_AURORA_COL2,d1),1.0)*d2*mask;
}
#endif

#endif
