#include "../environment.pov"

#declare HK=union {
  intersection {
    sphere { 0 2 }
    box { <-2,-2,0>, <2,2,2> }

    texture { T2 }
  }

  union {
    sphere { <0,2,0.4> 0.3 rotate z*360/6*0 }
    sphere { <0,2,0.4> 0.3 rotate z*360/6*1 }
    sphere { <0,2,0.4> 0.3 rotate z*360/6*2 }
    sphere { <0,2,0.4> 0.3 rotate z*360/6*3 }
    sphere { <0,2,0.4> 0.3 rotate z*360/6*4 }
    sphere { <0,2,0.4> 0.3 rotate z*360/6*5 }

    texture { T1 }
  }
}

union {
  object { HK translate  z*sin(rot*pi) rotate z*rot*360/6 }
  object { HK rotate x*180 translate -z*sin(rot*pi) rotate z*rot*360/6 }
  box { <-1,-1,-1> <1,1,1> rotate z*rot*90 texture { T3 } }
  scale 0.9
}
