#include "colors.inc"
#include "textures.inc"

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

declare pyramide = intersection {
  box {<-3,-3,-3> <3,3,3>}
  plane { z+y ,sqrt(4.5) }
  plane { z-y ,sqrt(4.5) }
  plane { z+x ,sqrt(4.5) }
  plane { z-x ,sqrt(4.5) }
//  pigment { color rgb <1,0,0> }
//  scale 1/sqrt(3)
}

union {
  difference {
    box { <-3,-3,-3> <3,3,3> }
    object { pyramide translate -z*3.5 }
    object { pyramide rotate x*90  translate y*3.5 }
    object { pyramide rotate x*180 translate z*3.5 }
    object { pyramide rotate -x*90 translate -y*3.5 }
    object { pyramide rotate y*90  translate -x*3.5 }
    object { pyramide rotate -y*90 translate x*3.5 }
#if (white = 1)
  texture { weiss }
#else

    texture { Silver_Texture }
#end
}
  union {
    torus { 3, 0.2 }
    torus { 3, 0.2 rotate x*90 }
    torus { 3, 0.2 rotate z*90 }
#if (white = 1)
  texture { weiss }
#else
    texture { Gold_Texture }
#end
}
  scale 1/sqrt(3)
  scale 0.9

  rotate x*45
  rotate y*asin(1/sqrt(3))*180/pi
  rotate z*120*clock

}
