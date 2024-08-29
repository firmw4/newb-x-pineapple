#ifndef INSTANCING
  $input v_fogColor, v_worldPos, v_underwaterRainTime, sPos
#endif

#include <bgfx_shader.sh>

#ifndef INSTANCING
  #include <newb/main.sh>
  uniform vec4 FogAndDistanceControl;
  uniform vec4 ViewPositionAndTime;
#endif

void main() {
  #ifndef INSTANCING
    vec3 viewDir = normalize(v_worldPos);
    bool underWater = v_underwaterRainTime.x > 0.5;
    float rainFactor = v_underwaterRainTime.y;
    float mask = (1.0 - 1.0 * rainFactor) * max(1.0 - 3.0 * max(v_fogColor.b, v_fogColor.g), 0.0);

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

    vec3 skyColor = nlRenderSky(horizonEdgeCol, horizonCol, zenithCol, -viewDir, v_fogColor, v_underwaterRainTime.z, rainFactor, false, underWater, false) * 1.0;
    skyColor = colorCorrection(skyColor);

    #ifdef NLC_SHOOTING_STAR
    skyColor += pow(vec3_splat(star(sPos.xz*160.0, v_underwaterRainTime.z))*1.1, vec3(16. 6, 4)) * mask;
    #endif

    vec3 starDir = normalize(v_worldPos);
    float starTh = 8.0;
    float starEx = 200.0;
    float stars = pow(clamp(noiseS(starDir * 200.0), 0.0, 1.0), starTh) * starEx;
    stars *= mix(0.4, 1.4, noiseS(starDir * 100.0 + vec3(ViewPositionAndTime.w, ViewPositionAndTime.w, ViewPositionAndTime.w)));
    skyColor += vec3(stars*2.0, stars*2.0, stars*2.0)*mask;

    gl_FragColor = vec4(skyColor, 1.0);
  #else
    gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  #endif
}