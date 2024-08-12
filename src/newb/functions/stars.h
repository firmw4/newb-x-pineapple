#ifndef STARS_H
#define STARS_H
  
  float hashS(vec2 x){
    vec2 y = vec2(3.1415926*1453.0,exp(1.)*3054.0);
    return fract(sin(dot(x,y)*0.1)*4323.0);
  }
  
  float star(vec2 x, float time){
    x = mul(x, mtxFromCols(vec2(cos(0.9), sin(0.9)), vec2(sin(0.9), -cos(0.9))));
    x.y += time*12.0;
  
    float shape = (1.0-length(fract(x-vec2(0,0.5))-0.5));
    x *= vec2(1,0.1);
    
    vec2 fr = fract(x);
    float random = step(hashS(floor(x)),0.01), tall = (1.0-(abs(fr.x-0.5)+fr.y*0.5))*random;
    
    return clamp(clamp((shape-random)*step(hashS(floor(x+vec2(0,0.05))),.01),0.0,1.0)+tall,0.0,1.0);
  }
  
#endif
