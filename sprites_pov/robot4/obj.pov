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
  box {<-3,-3,0> <3,3,3>}
  plane { z+y ,sqrt(4.5) rotate z*0 }
  plane { z+y ,sqrt(4.5) rotate z*72 }
  plane { z+y ,sqrt(4.5) rotate z*144 }
  plane { z+y ,sqrt(4.5) rotate z*216 }
  plane { z+y ,sqrt(4.5) rotate z*288 }
  scale 0.5
  scale z*2
#if (white = 1)
  texture { weiss }
#else
  texture { Gold_Texture }
#end
}


union {
  cylinder { <0,0,-0.1> <0,0,0.1> 3
#if (white = 1)
  texture { weiss }
#else

    texture { Silver_Texture }
#end
}
  object { pyramide }
  object { pyramide rotate x*180 rotate z*36 }
  union {
    cylinder { <0,2.5,-1> <0,2.5,1> 0.5 rotate z*360/10*0 }
    cylinder { <0,2.5,-1> <0,2.5,1> 0.5 rotate z*360/10*1 }
    cylinder { <0,2.5,-1> <0,2.5,1> 0.5 rotate z*360/10*2 }
    cylinder { <0,2.5,-1> <0,2.5,1> 0.5 rotate z*360/10*3 }
    cylinder { <0,2.5,-1> <0,2.5,1> 0.5 rotate z*360/10*4 }
    cylinder { <0,2.5,-1> <0,2.5,1> 0.5 rotate z*360/10*5 }
    cylinder { <0,2.5,-1> <0,2.5,1> 0.5 rotate z*360/10*6 }
    cylinder { <0,2.5,-1> <0,2.5,1> 0.5 rotate z*360/10*7 }
    cylinder { <0,2.5,-1> <0,2.5,1> 0.5 rotate z*360/10*8 }
    cylinder { <0,2.5,-1> <0,2.5,1> 0.5 rotate z*360/10*9 }

#if (white = 1)
  texture { weiss }
#else
    texture { Copper_Texture }
#end
}

#if (white = 1)
#else
  finish { phong 1 ambient 0.3 }
#end
  rotate z*clock*72
  rotate y*clock*180
  scale 0.9
}

