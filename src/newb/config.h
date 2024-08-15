#ifndef NL_CONFIG_H
#define NL_CONFIG_H

/*
  NEWB SHADER MAIN CONFIG
  This part contains base configuration options for the shader.

  TOGGLES
  - If [toggle] is mentioned, then
    options can be commented to disable (by adding '//')
  - eg: #define PLANTS_WAVE    -> this is ON
        //#define PLANTS_WAVE  -> this is OFF

  COLOR VALUES
  - Color format: vec3(red,green,blue)
  - 1.0 means 100%, 0.0 means 0%
  - eg: vec3(1.0,1.0,0.0) = yellow

  VALUES
  - Values must be decimal
  - eg. 32 is wrong, 32.0 is correct

  TYPES
  - Should be integer. options to choose will be mentioned there
  - eg: #define NL_FOG_TYPE 2
  
  CUSTOM VALUE
  - NLC_.... = custom value for this variant

  Remember to rebuild the shader after making changes.
*/


/* Color correction */
// 1:Exponential, 2:Reinhard, 3:Extended Reinhard, 4:ACES
#define NL_TONEMAP_TYPE  3
#define NL_CONTRAST      0.826
#define NL_EXPOSURE      1.782
#define NL_SATURATION    1.413
//#define NL_TINT         vec3(1.0,0.75,0.5)

/* Terrain lighting */
#define NL_SUN_INTENSITY		 1.95 // 0.5 weak ~ 5.0 bright
#define NL_TORCH_INTENSITY		 0.82 // 0.5 weak ~ 3.0 bright
#define NL_NIGHT_BRIGHTNESS	     0.0 // 0.0 dark ~ 2.0 bright
#define NL_CAVE_BRIGHTNESS		 1.3 // 0.0 dark ~ 2.0 bright
#define NL_SHADOW_INTENSITY	     0.9 // 0.0 no shadow ~ 1.0 strong shadow
#define NL_BLINKING_TORCH                // [toggle] flickering light
//#define NL_CLOUD_SHADOW                // [toggle] cloud shadow (simple clouds only)
#define NLC_DARKEN_PLANT_BOTTOM  1.6  // 1.2 default ~ 2.0 darken (darken at bottom)

/* Sun/moon light color on terrain */
#define NL_MORNING_SUN_COL	    vec3(1.2, 0.46, 0.13)
#define NL_NOON_SUN_COL		    vec3(1.302, 0.6, 0.5)
#define NL_NIGHT_SUN_COL	    vec3(0.6, 0.67, 0.92)

/* Ambient light on terrain (light that is added everywhere) */
#define NL_NETHER_AMBIENT       vec3(0.501, 0.327, 0.412)//vec3(3.0,2.16,1.89)
#define NL_END_AMBIENT          vec3(0.237, 0.108, 0.649)//vec3(1.98,1.25,2.3)

/* Torch colors */
#define NL_OVERWORLD_TORCH_COL		vec3(0.87, 0.53, 0.28)
#define NL_UNDERWATER_TORCH_COL	    vec3(0.039, 0.7, 0.41)
#define NL_NETHER_TORCH_COL		    vec3(1.32, 0.54, 0.21)
#define NL_END_TORCH_COL		    vec3(0.32, 0.09, 0.61)

/* Fog */
#define NL_FOG_TYPE             2       // 0:no fog, 1:vanilla, 2:smoother vanilla
#define NL_MIST_DENSITY         1.26    // 0.0 no mist ~ 1.0 misty
#define NL_RAIN_MIST_OPACITY    0.41     // [toggle] 0.04 very subtle ~ 0.5 thick mist blow

/* Sky colors - zenith=top, horizon=bottom */
#define NL_DAY_ZENITH_COL    vec3(0.263, 0.44, 1.0)
#define NL_DAY_HORIZON_COL   vec3(0.73, 0.73, 1.0)

#define NL_NIGHT_ZENITH_COL  vec3(0.0, 0.001, 0.002)
#define NL_NIGHT_HORIZON_COL vec3(0.002, 0.2, 0.25)

#define NL_DAWN_ZENITH_COL   vec3(0.086, 0.15, 0.4)
#define NL_DAWN_HORIZON_COL  vec3(1.3, 0.301, 0.1)
#define NL_DAWN_EDGE_COL     vec3(1.4, 0.422, 0.93)

#define NL_RAIN_ZENITH_COL   vec3(0.25, 0.291, 0.38)
#define NL_RAIN_HORIZON_COL  vec3(0.37, 0.413, 0.58)

#define NL_END_ZENITH_COL    vec3(0.001, 0.0, 0.053)
#define NL_END_HORIZON_COL   vec3(0.43, 0.09, 0.92)

/* Rainbow */
//#define NL_RAINBOW         // [toggle] enable rainbow in sky
#define NL_RAINBOW_CLEAR    1.0 // 0.3 subtle ~ 1.7 bright during clear
#define NL_RAINBOW_RAIN     0.0  // 0.5 subtle ~ 2.0 bright during rain

/* Ore glow intensity */
#define NL_GLOW_TEX			    6.0     // 0.4 weak ~ 8.0 bright
#define NL_GLOW_SHIMMER		    0.9     // [toggle] shimmer effect
#define NL_GLOW_LEAK		    0.5     // [toggle] 0.08 subtle ~ 1.0 100% brightness of NL_GLOW_TEX

/* Waving */
#define NL_PLANTS_WAVE      0.05    // [toggle] 0.02 gentle ~ 0.4 violent
#define NL_LANTERN_WAVE     0.16   // [toggle] 0.05 subtle ~ 0.4 large swing
#define NL_WAVE_SPEED       1.8      // 0.5 slow wave ~ 5.0 very fast wave
//#define NL_EXTRA_PLANTS_WAVE // [toggle] !dont use! wave using texture coords (1.21.0 vanilla 2048x1024)

/* Water */
#define NL_WATER_TRANSPARENCY           0.433    // 0.0 transparent ~ 1.0 normal
#define NL_WATER_BUMP                   0.012   // 0.001 plain ~ 0.2 bumpy water
#define NL_WATER_TEX_OPACITY            1.782    // 0.0 plain water ~ 1.0 vanilla water texture
#define NL_WATER_WAVE                           // [toggle] wave effect
#define NL_WATER_FOG_FADE                       // [toggle] fog fade for water
//#define NL_WATER_CLOUD_REFLECTION               // [toggle] simple clouds/aurora reflection + extra(rounded & box clouds)
#define NL_WATER_TINT                   vec3(0.24, 0.65, 1.0)

/* Underwater */
#define NL_UNDERWATER_BRIGHTNESS    2.0 // 0.0 dark ~ 3.0 bright
#define NL_CAUSTIC_INTENSITY        2.5     // 0.5 weak ~ 5.0 bright
#define NL_UNDERWATER_WAVE          0.08       // [toggle] 0.02 subtle ~ 0.6 trippy
#define NL_UNDERWATER_STREAKS       1.6    // [toggle] 0.8 subtle - 2.0 bright streaks from top
#define NL_UNDERWATER_TINT          vec3(0.2, 0.8, 0.9) // fog tint color when underwater

/* Cloud type */
#define NL_CLOUD_TYPE 0   // 0:vanilla, 1:soft, 2:rounded

/* Vanilla cloud settings - make sure to remove clouds.png when using this */
#define NL_CLOUD0_THICKNESS         2.0      // 0.5 slim ~ 8.0 fat
#define NL_CLOUD0_RAIN_THICKNESS    4.0 // 0.5 slim ~ 8.0 fat
#define NL_CLOUD0_OPACITY           1.0        // 0.0 invisible ~ 1.0 opaque

/* Soft cloud settings */
#define NL_CLOUD1_SCALE         vec2(0.016, 0.022)  // 0.003 large ~ 0.2 tiny
#define NL_CLOUD1_DEPTH         1.4                 // 0.0 no bump ~ 10.0 large bumps
#define NL_CLOUD1_SPEED         0.04                // 0.0 static ~ 0.4 fast moving
#define NL_CLOUD1_DENSITY       0.54                // 0.1 less clouds ~ 0.8 more clouds
#define NL_CLOUD1_OPACITY       1.0                 // 0.0 invisible ~ 1.0 opaque

/* Rounded cloud Settings */
#define NL_CLOUD2_THICKNESS			    2.3     // 0.5 slim ~ 5.0 fat
#define NL_CLOUD2_RAIN_THICKNESS		2.4     // 0.5 slim ~ 5.0 fat
#define NL_CLOUD2_STEPS					5       // 3 low quality ~ 16 high quality
#define NL_CLOUD2_SCALE					0.025   // 0.003 large ~ 0.3 tiny
#define NL_CLOUD2_SHAPE					0.65    // 0.0 round ~ 1.0 box
#define NL_CLOUD2_DENSITY				200.0    // 1.0 blurry ~ 100.0 sharp
#define NL_CLOUD2_VELOCITY				0.8     // 0.0 static ~ 4.0 very fast
//#define NL_CLOUD2_MULTILAYER                  // [toggle] extra cloud layer

/* Custom cloud setting */
#define NLC_CLOUD2_TRANSITION1 0.1
#define NLC_CLOUD2_TRANSITION2 0.1001
//#define NLC_CLOUD_CUSTOM      // [toggle] volumetric cloud

/* Aurora settings */
#define NL_AURORA               4.0     // [toggle] 0.4 dim ~ 4.0 very bright
#define NL_AURORA_VELOCITY      0.012    // 0.0 static ~ 0.3 very fast
#define NL_AURORA_SCALE         0.04    // 0.002 large ~ 0.4 tiny
#define NL_AURORA_WIDTH         0.06     // 0.04 thin line ~ 0.4 thick lines

#define NL_AURORA_COL1 vec3(0.0, 0.8, 1.0)
#define NL_AURORA_COL2 vec3(0.4, 1.0, 0.7)
//#define NLC_AURORA_COL3 vec3(0.2, 0.9, 0.5)

/* Chunk loading slide in animation */
//#define NL_CHUNK_LOAD_ANIM    30.0 // [toggle] -600.0 fall from top ~ 600.0 rise from bottom

/* Sun/Moon */
#define NL_SUNMOON_ANGLE        35.0    // [toggle] 0.0 no tilt - 90.0 tilt of 90 degrees
#define NL_SUNMOON_SIZE         1.2     // 0.3 tiny - 4.0 massive

/* Fake godrays during sunrise/sunset */
#define NL_GODRAY       1.3     // [toggle] 0.1 subtle - 0.8 strong

/* Sky reflection */
//#define NL_GROUND_REFL                  0.6 // [toggle] 0.2 slightly reflective ~ 1.0 fully reflect sky 
#define NL_GROUND_RAIN_WETNESS          1.0 // 0.0 no wetness ~ 1.0 fully wet blocks when raining
#define NL_GROUND_RAIN_PUDDLES          0.7 // 0.0 no puddles ~ 1.0 puddles
//#define NL_GROUND_AURORA_REFL             // [toggle] aurora reflection on ground


/*
  NEWB SHADER SUBPACK CONFIG
  This part contains custom configuration options for each subpack.

  If a value is already defined,
  then you must undefine it before modifying:
  eg: #undef OPTION_NAME

  Subpack names and flags are inside `pack_config.toml`.
  Build tool will enable corresponding flags when compiling. 
*/


#ifdef NO_WAVE
  #undef NL_PLANTS_WAVE
  #undef NL_LANTERN_WAVE
  #undef NL_UNDERWATER_WAVE
  #undef NL_WATER_WAVE
  #undef NL_RAIN_MIST_OPACITY
  #undef NL_EXTRA_PLANTS_WAVE
#endif

#ifdef NIGHT_VISION
  #undef NL_NIGHT_BRIGHTNESS
  #undef NL_CAVE_BRIGHTNESS
  #define NL_NIGHT_BRIGHTNESS 10.0
  #define NL_CAVE_BRIGHTNESS 15.0
#endif

#ifdef RAINBOWS
  #define NL_RAINBOW
#endif

#ifdef SIMPLE_CLOUDS
  #undef NL_CLOUD_TYPE
  #define NL_CLOUD_TYPE 1
#endif

#ifdef ROUNDED_CLOUDS
  #undef NL_CLOUD_TYPE
  #define NL_CLOUD_TYPE 2
#endif

#ifdef ALWAYS_RELFECTION
  #define NL_GROUND_REFL 0.6
#endif

#ifdef VOLUMETRIC_CLOUDS
  #undef NL_CLOUD_TYPE
  #undef NL_CLOUD2_THICKNESS
  #undef NL_CLOUD2_RAIN_THICKNESS
  #undef NL_CLOUD2_STEPS
  #undef NL_CLOUD2_SCALE
  #undef NL_CLOUD2_SHAPE
  #undef NL_CLOUD2_DENSITY
  #undef NL_AURORA_SCALE
  #undef NLC_CLOUD2_TRANSITION1
  #undef NLC_CLOUD2_TRANSITION2
  #define NL_CLOUD_TYPE 2
  #define NL_CLOUD2_THICKNESS 2.3
  #define NL_CLOUD2_RAIN_THICKNESS 2.3
  #define NL_CLOUD2_STEPS 8
  #define NL_CLOUD2_SCALE 0.011
  #define NL_CLOUD2_SHAPE 0.01
  #define NL_CLOUD2_DENSITY 30.0
  #define NL_AURORA_SCALE 0.015
  #define NLC_CLOUD_CUSTOM
  #define NLC_CLOUD2_TRANSITION1 0.2
  #define NLC_CLOUD2_TRANSITION2 0.1999
#endif

#endif
