//#declare trans

#include "colors.inc"
#include "textures.inc"
#include "txt.inc"

global_settings { assumed_gamma 1.8 }

#declare rot = clock;
#if (clock >= 1)
#declare white = 1;
#declare rot = clock - 1;
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
  scale (x+y)*0.3
  scale sin(rot*pi)*0.5+0.5

#if (white = 1)
  texture { weiss }
#else
  texture { T2 }
#end
}


union {
  sphere { <0,0,0> 1.5
#if (white = 1)
  texture { weiss }
#else
    texture { T3 }
#end
  }

  object { pyramide }

  object { pyramide rotate x*45 rotate z*72*0 }
  object { pyramide rotate x*45 rotate z*72*1 }
  object { pyramide rotate x*45 rotate z*72*2 }
  object { pyramide rotate x*45 rotate z*72*3 }
  object { pyramide rotate x*45 rotate z*72*4 }

  object { pyramide rotate x*90 rotate z*72*0   }
  object { pyramide rotate x*90 rotate z*72*0.5 }
  object { pyramide rotate x*90 rotate z*72*1   }
  object { pyramide rotate x*90 rotate z*72*1.5 }
  object { pyramide rotate x*90 rotate z*72*2   }
  object { pyramide rotate x*90 rotate z*72*2.5 }
  object { pyramide rotate x*90 rotate z*72*3   }
  object { pyramide rotate x*90 rotate z*72*3.5 }
  object { pyramide rotate x*90 rotate z*72*4   }
  object { pyramide rotate x*90 rotate z*72*4.5 }

  object { pyramide rotate x*135 rotate z*72*0 }
  object { pyramide rotate x*135 rotate z*72*1 }
  object { pyramide rotate x*135 rotate z*72*2 }
  object { pyramide rotate x*135 rotate z*72*3 }
  object { pyramide rotate x*135 rotate z*72*4 }

  object { pyramide rotate x*180 }

#if (white = 1)
#else
  pigment { color rgb <1,1,0> }
#end

  rotate z*rot*72

  scale 0.9
}

