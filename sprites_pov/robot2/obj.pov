#declare rot = clock;

#if (clock >= 1)
#declare white = 1;
#else
#declare white = 0;
#end

#include "colors.inc"
#include "textures.inc"
#include "txt.inc"


#declare weiss = texture {
  pigment { color rgb 1 }
  finish { ambient 1 }
}



global_settings { assumed_gamma 1.8 }

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
  sphere { <0,0,0> 1.3
#if (white = 1)
  texture { weiss }
#else

    texture { T2 }
#end
    }
  union {
    sphere { <0,2.2,0> 0.8 rotate z*180*clock rotate x*45}
    sphere { <0,-2.2,0> 0.8 rotate z*180*clock rotate -x*45 }
#if (white = 1)
  texture { weiss }
#else

    texture { T1 }
#end

}
  union {
    sphere { <0,0,2> 0.6 }
    sphere { <0,2,0> 0.6 }
    sphere { <0,0,-2> 0.6 }
    sphere { <0,-2,0> 0.6 }
#if (white = 1)
  texture { weiss }
#else

    texture { T3 }
#end
}
  rotate z*clock*180
  rotate x*clock*180

  scale 0.9
}
