#if (clock >= 1)
#declare white = 1;
#declare cl = clock - 1;
#else
#declare white = 0;
#declare cl = clock;
#end

camera { 
  orthographic
  location  <16,0,0>
  sky       z  
  up        <0,0,6>  
  right     <6,0,0>
  look_at   <0,0,0>
}

light_source { <16,-10,10> color 1 }

union {  
  union {
    sphere { z*2  1 }
    sphere { -z*2 1 }
    sphere { y*2  1 }
    sphere { -y*2 1 }

#if (white = 1)
    pigment { color rgb 1 }
    finish { ambient 1 }
#else
    pigment { color rgb <0,0,1> }
    finish { phong 1 ambient 0.3 }
#end    
  }
  union {
    cylinder { z*2, -z*2, 0.3 }
    cylinder { y*2, -y*2, 0.3 }
#if (white = 1)
    pigment { color rgb 1 }
    finish { ambient 1 }
#else
    pigment { color rgb <1,0,0> }
    finish { phong 1 ambient 0.3 }
#end
  }
  rotate x*(cl+0.5)*90*5
  rotate y*(cl+0.5)*90*6
  rotate z*90*4*sin((cl+0.5)*2*pi)
  
  scale 0.9
}
