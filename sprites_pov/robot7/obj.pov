//#declare trans

#include "colors.inc"
#include "textures.inc"
#include "txt.inc"

global_settings { assumed_gamma 1.8 }

#declare rot = clock;
#if (clock >= 1)
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

union {
  torus { 1.5 0.5
#if (white = 1)
  texture { weiss }
#else

  texture { T3 }
#end
    rotate y*90
    rotate z*clock*180
    rotate x*45
  }
  torus { 2.5 0.5
#if (white = 1)
  texture { weiss }
#else

    texture { T1 }
#end
    rotate y*90
    rotate z*clock*180+z*33
    rotate -x*33
  }
  sphere { <0,0,0> 1
#if (white = 1)
  texture { weiss }
#else
    texture { T2 }
#end
  }

  scale 0.9
}
