#ifndef HASH_H
#define HASH_H

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
vec3 hashS(vec3 p) {
  p = vec3(dot(p,vec3(127.1,311.7, 74.7)),
           dot(p,vec3(269.5,183.3,246.1)),
           dot(p,vec3(113.5,271.9,124.6)));

  return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

#endif