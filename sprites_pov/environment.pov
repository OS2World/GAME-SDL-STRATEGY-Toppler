#include "colors.inc"
#include "textures.inc"

#declare weiss = texture {
  pigment { color rgb 1 }
  finish { ambient 1 }
}

#if (clock >= 1)
  #declare T1 = texture { weiss }
  #declare T2 = texture { weiss }
  #declare T3 = texture { weiss }
#else
  #declare T1 = texture { Gold_Texture }
  #declare T2 = texture { Silver_Texture }
  #declare T3 = texture { Copper_Texture }
#end

camera {
  orthographic
  location  <16,0,0>
  sky       z
  up        <0,0,6>
  right     <6,0,0>
  look_at   <0,0,0>
}

global_settings { assumed_gamma 1.8 }

light_source { <16,-10,10> color 1 }

#if (clock >=1)
#declare rot = clock - 1;
#declare white = 1;
#else
#declare rot = clock;
#declare white = 0;
#end
