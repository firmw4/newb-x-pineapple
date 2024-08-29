#ifndef STARS_H
#define STARS_H

#include "hash.h"

// shooting star
highp float star(highp vec2 x, float time) {
    x = mul(x, mtxFromCols(vec2(cos(0.0), sin(0.0)), vec2(sin(0.0), -cos(0.5))));
    x.y += time * 5.0;

    highp float shape = (1.0 - length(fract(x - vec2(0, 0.5)) - 0.5));
    x *= vec2(1, 0.1);

    highp vec2 fr = fract(x);
    highp float random = step(hash(floor(x)), 0.01);
    highp float tall = (1.0 - (abs(fr.x - 0.5) + fr.y * 0.5)) * random;

    return clamp(clamp((shape - random) * step(hash(floor(x + vec2(0, 0.05))), 0.01), 0.0, 1.0) + tall, 0.0, 1.0);
}

#endif
    