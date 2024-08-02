#ifndef INSTANCING
  $input v_fogColor, v_worldPos, v_underwaterRainTime, sPos
#endif

#include <bgfx_shader.sh>

// falling stars, code by i11212
highp float hashS(highp vec2 x){
  return fract(sin(dot(x,vec2(10,47)))*4e3);
}

highp float star(highp vec2 x, float time){
  //x *= mat2(cos(0.9), -sin(0.9), sin(0.9), cos(0.9));
  x = mul(x, mtxFromCols(vec2(cos(0.9), sin(0.9)), vec2(sin(0.9), -cos(0.9))));
  x.y += time*12.0;
  
  highp float shape = (1.0-length(fract(x-vec2(0,0.5))-0.5));
  x *= vec2(1,0.1);
    
  highp vec2 fr = fract(x);
  highp float random = step(hashS(floor(x)),0.01), tall = (1.0-(abs(fr.x-0.5)+fr.y*0.5))*random;
    
  return clamp(clamp((shape-random)*step(hashS(floor(x+vec2(0,0.05))),.01),0.0,1.0)+tall,0.0,1.0);
}

#ifndef INSTANCING
  #include <newb/main.sh>
  uniform vec4 FogAndDistanceControl;
#endif

void main() {
  #ifndef INSTANCING
    vec3 viewDir = normalize(v_worldPos);
    bool underWater = v_underwaterRainTime.x > 0.5;
    float rainFactor = v_underwaterRainTime.y;
    
    float mask = (1.0-1.0*rainFactor)*max(1.0 - 3.0 * max(v_fogColor.b, v_fogColor.g), 0.0);
    
    vec3 zenithCol;
    vec3 horizonCol;
    vec3 horizonEdgeCol;
    if (underWater) {
      vec3 fogcol = getUnderwaterCol(v_fogColor);
      zenithCol = fogcol;
      horizonCol = fogcol;
      horizonEdgeCol = fogcol;
    } else {
      vec3 fs = getSkyFactors(v_fogColor);
      zenithCol = getZenithCol(rainFactor, v_fogColor, fs);
      horizonCol = getHorizonCol(rainFactor, v_fogColor, fs);
      horizonEdgeCol = getHorizonEdgeCol(horizonCol, rainFactor, v_fogColor);
    }

    vec3 skyColor = nlRenderSky(horizonEdgeCol, horizonCol, zenithCol, -viewDir, v_fogColor, v_underwaterRainTime.z, rainFactor, false, underWater, false)*1.0;
    skyColor = colorCorrection(skyColor);
    
    skyColor += pow(vec3_splat(star(sPos.zx*100.0, v_underwaterRainTime.z))*1.1, vec3(8.0, 6.0, 2.0))*mask;
    
    gl_FragColor = vec4(skyColor, 1.0);
  #else
    gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  #endif
}
