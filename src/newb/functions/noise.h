#ifndef NOISE_H
#define NOISE_H

#include "constants.h"

// hash function for noise (for highp only)
highp float rand(highp vec2 n) {
  return fract(sin(dot(n, vec2(12.9898, 6.1414))) * 43758.5453);
}

// random generate for shooting star
float hash(vec2 x) {
  vec2 y = vec2(3.1415926 * 1453.0, exp(1.0) * 3054.0);
  return fract(sin(dot(x, y) * 0.1) * 4323.0);
}

// hash function for star
vec3 sHash(vec3 p) {
  p = vec3(dot(p,vec3(127.1,311.7, 74.7)),
           dot(p,vec3(269.5,183.3,246.1)),
           dot(p,vec3(113.5,271.9,124.6)));

  return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

// 1D noise - used in plants,lantern wave
highp float noise1D(highp float x) {
  float x0 = floor(x);
  float t0 = x-x0;
  t0 *= t0*(3.0-2.0*t0);
  return mix(fract(sin(x0)*84.85), fract(sin(x0+1.0)*84.85), t0);
}

// simpler rand for disp, puddles
float fastRand(vec2 n){
  return fract(37.45*sin(dot(n, vec2(4.36, 8.28))));
}

// water displacement map (also used by caustic)
float disp(vec3 pos, highp float t) {
  float val = 0.5 + 0.5*sin(t*1.7 + (pos.x+pos.y)*NL_CONST_PI_HALF);
  return mix(fastRand(pos.xz), fastRand(pos.xz+vec2_splat(1.0)), val);
}

// rand with transition
float randt(vec2 n, vec2 t) {
  return smoothstep(t.x, t.y, rand(n));
}

// apply bevel with radius r at at corner (1.0)
float bevel(float x, float r) {
	 //return smoothstep(0.5,1.5,x);
  float y = max(x-r,0.0);
  return r+sqrt(1.0-2.0*r+r*r-y*y);
}

float mod289(float x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}
vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}
vec4 perm(vec4 x) {
  return mod289(((x * 34.0) + 1.0) * x);
}

// 3d noise for cloud2
float noise2D(vec3 p){
  vec3 a = floor(p);
  vec3 d = p - a;
  d = d * d * (3.0 - 2.0 * d);

  vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
  vec4 k1 = perm(b.xyxy);
  vec4 k2 = perm(k1.xyxy + b.zzww);

  vec4 c = k2 + a.zzzz;
  vec4 k3 = perm(c);
  vec4 k4 = perm(c + 1.0);

  vec4 o1 = fract(k3 * (1.0 / 41.0));
  vec4 o2 = fract(k4 * (1.0 / 41.0));

  vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
  vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

  return o4.y * d.y + o4.x * (1.0 - d.y);
}

// 3D star noise modeling
float sNoise(in vec3 p) {
  vec3 i = floor(p);
  vec3 f = fract(p);

  vec3 u = f*f*(3.0-2.0*f);

  return mix(mix(mix(dot(sHash(i + vec3(0.0,0.0,0.0)), f - vec3(0.0,0.0,0.0)),
                     dot(sHash(i + vec3(1.0,0.0,0.0)), f - vec3(1.0,0.0,0.0)), u.x),
                 mix(dot(sHash(i + vec3(0.0,1.0,0.0)), f - vec3(0.0,1.0,0.0)),
                     dot(sHash(i + vec3(1.0,1.0,0.0)), f - vec3(1.0,1.0,0.0)), u.x), u.y),
             mix(mix(dot(sHash(i + vec3(0.0,0.0,1.0)), f - vec3(0.0,0.0,1.0)),
                     dot(sHash(i + vec3(1.0,0.0,1.0)), f - vec3(1.0,0.0,1.0)), u.x),
                 mix(dot(sHash(i + vec3(0.0,1.0,1.0)), f - vec3(0.0,1.0,1.0)),
                     dot(sHash(i + vec3(1.0,1.0,1.0)), f - vec3(1.0,1.0,1.0)), u.x), u.y), u.z);
}

#endif
