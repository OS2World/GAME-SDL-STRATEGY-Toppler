#declare klink=box { <40,-9,0>, <48,9,7.5> }

/*
global_settings{
  assumed_gamma 1.0
  radiosity{
    pretrace_start 0.04
    pretrace_end 0.01
    count 200
    recursion_limit 3
    nearest_count 10
    error_bound 0.5
  }
}
*/


#declare scheibe = union {
  object { klink rotate 00*(360/16)*z }
  object { klink rotate 01*(360/16)*z }
  object { klink rotate 02*(360/16)*z }
  object { klink rotate 03*(360/16)*z }
  object { klink rotate 04*(360/16)*z }
  object { klink rotate 05*(360/16)*z }
  object { klink rotate 06*(360/16)*z }
  object { klink rotate 07*(360/16)*z }
  object { klink rotate 08*(360/16)*z }
  object { klink rotate 09*(360/16)*z }
  object { klink rotate 10*(360/16)*z }
  object { klink rotate 11*(360/16)*z }
  object { klink rotate 12*(360/16)*z }
  object { klink rotate 13*(360/16)*z }
  object { klink rotate 14*(360/16)*z }
  object { klink rotate 15*(360/16)*z }
}

#declare zinne = difference {
  box { <61, -14, 0 >, <72, 14, 24> }
  box { <60, -15, 12>, <73, -7, 25> }
  box { <60,   7, 12>, <73, 15, 25> }
}

#declare zinnen = union {
  object { zinne rotate 00*z*(360/16) }
  object { zinne rotate 01*z*(360/16) }
  object { zinne rotate 02*z*(360/16) }
  object { zinne rotate 03*z*(360/16) }
  object { zinne rotate 04*z*(360/16) }
  object { zinne rotate 05*z*(360/16) }
  object { zinne rotate 06*z*(360/16) }
  object { zinne rotate 07*z*(360/16) }
  object { zinne rotate 08*z*(360/16) }
  object { zinne rotate 09*z*(360/16) }
  object { zinne rotate 10*z*(360/16) }
  object { zinne rotate 11*z*(360/16) }
  object { zinne rotate 12*z*(360/16) }
  object { zinne rotate 13*z*(360/16) }
  object { zinne rotate 14*z*(360/16) }
  object { zinne rotate 15*z*(360/16) }
  cylinder { <0,0,0> <0,0,12> 65 }
}
camera {
  location <700,-300,300>
  up z
  right 4/3*x
  sky z
  look_at <0,500,300>
}

#declare stufe=cylinder { <58,0,0>, <58,0,7>, 10 texture { pigment { color rgb 0.7 } } }
#declare stutz=cylinder { <58,0,0>, <58,0,7>, 4  texture { pigment { color rgb 0.7 } } }
#declare eleva=cylinder { <58,0,0>, <58,0,7>, 8  texture { pigment { color rgb 0.7 } } }

#declare moertel = texture { pigment { color rgb <0.7,0.7,0.7> } }

light_source { <1300,1700,300> color rgb <1,1,1> }

#declare farb1=pigment { color rgb <1,0,0> }
#declare farb2=pigment { color rgb <0.59,0.59,1> }
#declare farb3=pigment { color rgb 0.6666 }
#declare farb4=pigment { color rgb <0.64, 0.47, 0.345> }
#declare farb5=pigment { color rgb <0,0.6,0.6> }
#declare farb6=pigment { color rgb <1,0.39,0.39> }
#declare farb7=pigment { color rgb <0.588,1,0.588> }
#declare farb8=pigment { color rgb <1,0.6,0> }

#declare scheibe1 = object { scheibe pigment { farb1 } }
#declare scheibe2 = object { scheibe pigment { farb2 } }
#declare scheibe3 = object { scheibe pigment { farb3 } }
#declare scheibe4 = object { scheibe pigment { farb4 } }
#declare scheibe5 = object { scheibe pigment { farb5 } }
#declare scheibe6 = object { scheibe pigment { farb6 } }
#declare scheibe7 = object { scheibe pigment { farb7 } }
#declare scheibe8 = object { scheibe pigment { farb8 } }

#declare zinnen1 = object { zinnen pigment { farb1 } }
#declare zinnen2 = object { zinnen pigment { farb2 } }
#declare zinnen3 = object { zinnen pigment { farb3 } }
#declare zinnen4 = object { zinnen pigment { farb4 } }
#declare zinnen5 = object { zinnen pigment { farb5 } }
#declare zinnen6 = object { zinnen pigment { farb6 } }
#declare zinnen7 = object { zinnen pigment { farb7 } }
#declare zinnen8 = object { zinnen pigment { farb8 } }

#include "turm1.inc"
#include "turm2.inc"
#include "turm3.inc"
#include "turm4.inc"
#include "turm5.inc"
#include "turm6.inc"
#include "turm7.inc"
#include "turm8.inc"

plane {
  z, 0
  texture {
    pigment { color rgb <0,0,0> }
    finish {
      reflection 0.8
    }
    normal {
      waves
      scale 20
    }
  }
}
union {
  sphere { <0,0,0>,50000 
     pigment { color rgb 1 } 
     finish { ambient 0.9 }
  }
  intersection {
    sphere { <0,0,0>,50001 }
    box { <-50001,-50001,-50001>, <0,50001,50001>}
    pigment { color rgb <0,0,0.15> }
    finish { ambient 1 }
  }
  rotate z*50
  rotate -x*35
  translate <0,500000,60000>
  rotate z*14
  finish {
    diffuse 0
    phong 0
  }
}

object { turm1 translate <0,000,0> }
object { turm2 translate <0,200,0> }
object { turm3 translate <0,400,0> }
object { turm4 translate <0,600,0> }
object { turm5 translate <0,800,0> }
object { turm6 translate <0,1000,0> }
object { turm7 translate <0,1200,0> }
object { turm8 translate <0,1400,0> }

sky_sphere { pigment { color rgb <0,0,0.2> } }
