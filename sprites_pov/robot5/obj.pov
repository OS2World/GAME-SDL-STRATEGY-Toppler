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

#declare spiess = union {
  cylinder { <0,0,0> <2.5,5/3,0> 0.2 }
  cone { <2.5,5/3,0> 0.2 <3,2,0> 0 }
#if (white = 1)
  texture { weiss }
#else
  texture { Silver_Texture }
#end
}


union {
  cylinder { <-0.2,0,0> <0.2,0,0> 3 
#if (white = 1)
  texture { weiss }
#else
    texture { Gold_Texture } 
#end
  }

  union {
    object { spiess translate y rotate x*360/5*0 }
    object { spiess translate y rotate x*360/5*1 }
    object { spiess translate y rotate x*360/5*2 }
    object { spiess translate y rotate x*360/5*3 }
    object { spiess translate y rotate x*360/5*4 }

    object { spiess translate -2.8*y rotate x*360/5*0 }
    object { spiess translate -2.8*y rotate x*360/5*1 }
    object { spiess translate -2.8*y rotate x*360/5*2 }
    object { spiess translate -2.8*y rotate x*360/5*3 }
    object { spiess translate -2.8*y rotate x*360/5*4 }
  }

  union {
    object { spiess translate y rotate x*360/5*0 }
    object { spiess translate y rotate x*360/5*1 }
    object { spiess translate y rotate x*360/5*2 }
    object { spiess translate y rotate x*360/5*3 }
    object { spiess translate y rotate x*360/5*4 }

    object { spiess translate -2.8*y rotate x*360/5*0 }
    object { spiess translate -2.8*y rotate x*360/5*1 }
    object { spiess translate -2.8*y rotate x*360/5*2 }
    object { spiess translate -2.8*y rotate x*360/5*3 }
    object { spiess translate -2.8*y rotate x*360/5*4 }

    rotate z*180
    rotate x*180
  }

  rotate x*clock*180
  rotate z*180*clock+90*z

  scale 0.9
}
