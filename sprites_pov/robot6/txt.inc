#declare b = texture {
  pigment { color rgb <0,0,0> }
  finish {
    reflection 0
    ambient 0
    phong 0
  }
}

#ifdef (trans)
  #declare T1 = texture { b }
  #declare T2 = texture { b }
  #declare T3 = texture { b } =
  background { color rgb 1 }
#else
  #declare T1 = texture { Gold_Texture }
  #declare T2 = texture { Silver_Texture }
  #declare T3 = texture { Copper_Texture }
#end
