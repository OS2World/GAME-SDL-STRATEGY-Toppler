#include "colors.inc"
#include "textures.inc"

global_settings { assumed_gamma 1.8 }

#declare rot = clock;
#if (clock >= 1)
#declare rot = clock - 1;
#declare white = 1;
#else
#declare white = 0;
#end
#declare weiss = texture {
  pigment { color rgb 1 }
  finish { ambient 1 }
}



camera {  //  Camera Camera01
  orthographic
  location  <16,0,0>
  sky       z  
  up        <0,0,6>  
  right     <6,0,0>
  look_at   <0,0,0>
}

light_source { <16,-10,10> color 1 }

#declare HK=union {
  intersection {
    sphere { 0 2 }
    box { <-2,-2,0>, <2,2,2> }
#if (white = 1)
  texture { weiss }
#else

    texture { Silver_Texture }
#end
}
  union {
    sphere { <0,2,0.4> 0.3 rotate z*360/6*0 }
    sphere { <0,2,0.4> 0.3 rotate z*360/6*1 }
    sphere { <0,2,0.4> 0.3 rotate z*360/6*2 }
    sphere { <0,2,0.4> 0.3 rotate z*360/6*3 }
    sphere { <0,2,0.4> 0.3 rotate z*360/6*4 }
    sphere { <0,2,0.4> 0.3 rotate z*360/6*5 }
#if (white = 1)
  texture { weiss }
#else

    texture { Gold_Texture }
#end
}
}

union {
  object { HK translate  z*sin(rot*pi) rotate z*rot*360/6 }
  object { HK rotate x*180 translate -z*sin(rot*pi) rotate z*rot*360/6 }
  box { <-1,-1,-1> <1,1,1> rotate z*rot*90
#if (white = 1)
  texture { weiss } }
#else
    texture { Copper_Texture }
    finish { ambient 0.7 } }
  finish { ambient 0.3 phong 1 }
#end
  scale 0.9
}
