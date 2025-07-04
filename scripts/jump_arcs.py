#!/usr/bin/python
import math

def lerp(fromval, toval, tval):
    return fromval + tval * (toval - fromval)

def ease_in(x):
    return 1.0 - math.cos((x * math.pi) / 2.0);

def ease_out(x):
    return math.sin((x * math.pi) / 2.0)

jumpArcX = [None]*64
jumpArcY = [None]*64
for x in range(0, 8):
    for y in range(0, 8):
        idx = x + (y*8)
        jumpArcX[idx] = [None]*30
        jumpArcY[idx] = [None]*30
        x_diff = (x - 4)*16
        y_diff = (y - 4)*16
        for n in range(0, 30):
            x_value_at_n = 0
            y_value_at_n = 0
            y_mid_target = y_diff-16;
            if(y - 4 >= 0): # downwards jump, so always jump up even more
                y_mid_target = -32;

            if(n < 15):
                y_value_at_n = math.floor(lerp(0, y_mid_target, ease_out(n/15)))
            else:
                y_value_at_n = math.floor(lerp(y_mid_target, y_diff, ease_in((n-15)/15)))
            x_value_at_n = math.floor(lerp(0, x_diff, n/30))
            jumpArcX[idx][n] = x_value_at_n + 127
            jumpArcY[idx][n] = y_value_at_n + 127

c_string = ""
c_string += "const unsigned char jumpLutX[1920] = {"
for i in range(0, 64):
    for n in range(0, 30):
        c_string += str(jumpArcX[i][n]) + ", "
    c_string += "\n"
c_string += "};\n"

c_string += "const unsigned char jumpLutY[1920] = {"
for i in range(0, 64):
    for n in range(0, 30):
        c_string += str(jumpArcY[i][n]) + ", "
    c_string += "\n"
c_string += "};\n"
print(c_string)
