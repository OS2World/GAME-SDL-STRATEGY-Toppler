#declare turmot3 = union {
  cylinder { <0,0,0>, <0,0,392>, 46 texture { moertel } }
  object { scheibe3 rotate z* 0.00 translate z*8*0 }
  object { scheibe3 rotate z*11.25 translate z*8*1 }
  object { stufe rotate z*(360/16)*0 translate z*8*1 }
  object { stufe rotate z*(360/16)*3 translate z*8*1 }
  object { stufe rotate z*(360/16)*4 translate z*8*1 }
  object { stufe rotate z*(360/16)*9 translate z*8*1 }
  object { stufe rotate z*(360/16)*10 translate z*8*1 }
  object { stufe rotate z*(360/16)*11 translate z*8*1 }
  object { stufe rotate z*(360/16)*12 translate z*8*1 }
  object { stufe rotate z*(360/16)*13 translate z*8*1 }
  object { stufe rotate z*(360/16)*14 translate z*8*1 }
  object { stufe rotate z*(360/16)*15 translate z*8*1 }
  difference {
    object { scheibe3 rotate z* 0.00 translate z*8*2 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*2 texture { moertel } }
  }
  object { eleva rotate z*(360/16)*3 translate z*8*2 }
  object { eleva rotate z*(360/16)*9 translate z*8*2 }
  difference {
    object { scheibe3 rotate z*11.25 translate z*8*3 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*3 texture { moertel } }
  }
  difference {
    object { scheibe3 rotate z* 0.00 translate z*8*4 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*4 texture { moertel } }
  }
  object { scheibe3 rotate z*11.25 translate z*8*5 }
  object { scheibe3 rotate z* 0.00 translate z*8*6 }
  object { stufe rotate z*(360/16)*4 translate z*8*6 }
  object { stufe rotate z*(360/16)*5 translate z*8*6 }
  object { stufe rotate z*(360/16)*6 translate z*8*6 }
  object { stufe rotate z*(360/16)*7 translate z*8*6 }
  object { stufe rotate z*(360/16)*8 translate z*8*6 }
  object { stufe rotate z*(360/16)*9 translate z*8*6 }
  object { stufe rotate z*(360/16)*12 translate z*8*6 }
  object { stufe rotate z*(360/16)*13 translate z*8*6 }
  object { stufe rotate z*(360/16)*14 translate z*8*6 }
  object { scheibe3 rotate z*11.25 translate z*8*7 }
  object { eleva rotate z*(360/16)*12 translate z*8*7 }
  object { scheibe3 rotate z* 0.00 translate z*8*8 }
  object { scheibe3 rotate z*11.25 translate z*8*9 }
  object { scheibe3 rotate z* 0.00 translate z*8*10 }
  object { scheibe3 rotate z*11.25 translate z*8*11 }
  object { stufe rotate z*(360/16)*2 translate z*8*11 }
  object { stufe rotate z*(360/16)*4 translate z*8*11 }
  object { stufe rotate z*(360/16)*5 translate z*8*11 }
  object { stufe rotate z*(360/16)*6 translate z*8*11 }
  object { stufe rotate z*(360/16)*7 translate z*8*11 }
  object { stufe rotate z*(360/16)*8 translate z*8*11 }
  difference {
    object { scheibe3 rotate z* 0.00 translate z*8*12 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*6 translate z*8*12 texture { moertel } }
  }
  object { stutz rotate z*(360/16)*5 translate z*8*12 }
  difference {
    object { scheibe3 rotate z*11.25 translate z*8*13 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*6 translate z*8*13 texture { moertel } }
  }
  object { stutz rotate z*(360/16)*5 translate z*8*13 }
  difference {
    object { scheibe3 rotate z* 0.00 translate z*8*14 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*6 translate z*8*14 texture { moertel } }
  }
  object { stutz rotate z*(360/16)*5 translate z*8*14 }
  object { scheibe3 rotate z*11.25 translate z*8*15 }
  object { stufe rotate z*(360/16)*5 translate z*8*15 }
  object { stufe rotate z*(360/16)*6 translate z*8*15 }
  object { stufe rotate z*(360/16)*7 translate z*8*15 }
  object { stufe rotate z*(360/16)*8 translate z*8*15 }
  object { stufe rotate z*(360/16)*9 translate z*8*15 }
  object { scheibe3 rotate z* 0.00 translate z*8*16 }
  object { eleva rotate z*(360/16)*5 translate z*8*16 }
  object { scheibe3 rotate z*11.25 translate z*8*17 }
  object { stufe rotate z*(360/16)*10 translate z*8*17 }
  object { scheibe3 rotate z* 0.00 translate z*8*18 }
  object { scheibe3 rotate z*11.25 translate z*8*19 }
  object { stufe rotate z*(360/16)*11 translate z*8*19 }
  object { scheibe3 rotate z* 0.00 translate z*8*20 }
  object { stufe rotate z*(360/16)*3 translate z*8*20 }
  object { stufe rotate z*(360/16)*4 translate z*8*20 }
  object { scheibe3 rotate z*11.25 translate z*8*21 }
  object { stufe rotate z*(360/16)*2 translate z*8*21 }
  object { scheibe3 rotate z* 0.00 translate z*8*22 }
  object { stufe rotate z*(360/16)*1 translate z*8*22 }
  object { stufe rotate z*(360/16)*8 translate z*8*22 }
  object { stufe rotate z*(360/16)*9 translate z*8*22 }
  object { scheibe3 rotate z*11.25 translate z*8*23 }
  object { stufe rotate z*(360/16)*0 translate z*8*23 }
  object { stufe rotate z*(360/16)*7 translate z*8*23 }
  object { eleva rotate z*(360/16)*9 translate z*8*23 }
  object { scheibe3 rotate z* 0.00 translate z*8*24 }
  object { stufe rotate z*(360/16)*6 translate z*8*24 }
  object { stufe rotate z*(360/16)*15 translate z*8*24 }
  object { scheibe3 rotate z*11.25 translate z*8*25 }
  object { stufe rotate z*(360/16)*14 translate z*8*25 }
  object { scheibe3 rotate z* 0.00 translate z*8*26 }
  object { stufe rotate z*(360/16)*13 translate z*8*26 }
  object { scheibe3 rotate z*11.25 translate z*8*27 }
  object { stufe rotate z*(360/16)*10 translate z*8*27 }
  object { stufe rotate z*(360/16)*11 translate z*8*27 }
  object { stufe rotate z*(360/16)*12 translate z*8*27 }
  object { scheibe3 rotate z* 0.00 translate z*8*28 }
  object { scheibe3 rotate z*11.25 translate z*8*29 }
  object { scheibe3 rotate z* 0.00 translate z*8*30 }
  object { scheibe3 rotate z*11.25 translate z*8*31 }
  object { scheibe3 rotate z* 0.00 translate z*8*32 }
  object { scheibe3 rotate z*11.25 translate z*8*33 }
  object { scheibe3 rotate z* 0.00 translate z*8*34 }
  object { stufe rotate z*(360/16)*3 translate z*8*34 }
  object { stufe rotate z*(360/16)*4 translate z*8*34 }
  object { stufe rotate z*(360/16)*12 translate z*8*34 }
  object { stufe rotate z*(360/16)*13 translate z*8*34 }
  difference {
    object { scheibe3 rotate z*11.25 translate z*8*35 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*35 texture { moertel } }
  }
  object { eleva rotate z*(360/16)*3 translate z*8*35 }
  object { stufe rotate z*(360/16)*14 translate z*8*35 }
  object { stufe rotate z*(360/16)*15 translate z*8*35 }
  difference {
    object { scheibe3 rotate z* 0.00 translate z*8*36 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*36 texture { moertel } }
  }
  difference {
    object { scheibe3 rotate z*11.25 translate z*8*37 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*37 texture { moertel } }
  }
  object { scheibe3 rotate z* 0.00 translate z*8*38 }
  object { stufe rotate z*(360/16)*7 translate z*8*38 }
  object { stufe rotate z*(360/16)*8 translate z*8*38 }
  difference {
    object { scheibe3 rotate z*11.25 translate z*8*39 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*7 translate z*8*39 texture { moertel } }
  }
  difference {
    object { scheibe3 rotate z* 0.00 translate z*8*40 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*7 translate z*8*40 texture { moertel } }
  }
  difference {
    object { scheibe3 rotate z*11.25 translate z*8*41 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*7 translate z*8*41 texture { moertel } }
  }
  object { scheibe3 rotate z* 0.00 translate z*8*42 }
  object { scheibe3 rotate z*11.25 translate z*8*43 }
  object { scheibe3 rotate z* 0.00 translate z*8*44 }
  object { stufe rotate z*(360/16)*4 translate z*8*44 }
  object { stufe rotate z*(360/16)*11 translate z*8*44 }
  object { stufe rotate z*(360/16)*12 translate z*8*44 }
  object { stufe rotate z*(360/16)*13 translate z*8*44 }
  difference {
    object { scheibe3 rotate z*11.25 translate z*8*45 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*45 texture { moertel } }
  }
  difference {
    object { scheibe3 rotate z* 0.00 translate z*8*46 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*46 texture { moertel } }
  }
  difference {
    object { scheibe3 rotate z*11.25 translate z*8*47 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*47 texture { moertel } }
  }
  object { scheibe3 rotate z* 0.00 translate z*8*48 }
  object { zinnen3 translate z*392 }
}
#declare turm3 = object {
  object { turmot3 }

  bounded_by { cylinder { <0, 0, 0> <0, 0, 425> 80 } }
}
