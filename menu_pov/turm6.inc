#declare turmot6 = union {
  cylinder { <0,0,0>, <0,0,512>, 46 texture { moertel } }
  object { scheibe6 rotate z* 0.00 translate z*8*0 }
  object { scheibe6 rotate z*11.25 translate z*8*1 }
  object { stufe rotate z*(360/16)*0 translate z*8*1 }
  object { stufe rotate z*(360/16)*1 translate z*8*1 }
  object { stufe rotate z*(360/16)*2 translate z*8*1 }
  object { stufe rotate z*(360/16)*3 translate z*8*1 }
  object { stufe rotate z*(360/16)*4 translate z*8*1 }
  object { stufe rotate z*(360/16)*5 translate z*8*1 }
  object { stufe rotate z*(360/16)*6 translate z*8*1 }
  object { stufe rotate z*(360/16)*7 translate z*8*1 }
  object { stufe rotate z*(360/16)*8 translate z*8*1 }
  object { stufe rotate z*(360/16)*9 translate z*8*1 }
  object { stufe rotate z*(360/16)*10 translate z*8*1 }
  object { stufe rotate z*(360/16)*11 translate z*8*1 }
  object { scheibe6 rotate z* 0.00 translate z*8*2 }
  object { eleva rotate z*(360/16)*3 translate z*8*2 }
  object { stutz rotate z*(360/16)*4 translate z*8*2 }
  object { eleva rotate z*(360/16)*5 translate z*8*2 }
  object { eleva rotate z*(360/16)*11 translate z*8*2 }
  object { scheibe6 rotate z*11.25 translate z*8*3 }
  object { stutz rotate z*(360/16)*4 translate z*8*3 }
  object { scheibe6 rotate z* 0.00 translate z*8*4 }
  object { stutz rotate z*(360/16)*4 translate z*8*4 }
  object { scheibe6 rotate z*11.25 translate z*8*5 }
  object { stufe rotate z*(360/16)*1 translate z*8*5 }
  object { stufe rotate z*(360/16)*4 translate z*8*5 }
  object { stufe rotate z*(360/16)*6 translate z*8*5 }
  object { stufe rotate z*(360/16)*7 translate z*8*5 }
  object { stufe rotate z*(360/16)*13 translate z*8*5 }
  object { stufe rotate z*(360/16)*14 translate z*8*5 }
  object { stufe rotate z*(360/16)*15 translate z*8*5 }
  object { scheibe6 rotate z* 0.00 translate z*8*6 }
  object { eleva rotate z*(360/16)*14 translate z*8*6 }
  object { scheibe6 rotate z*11.25 translate z*8*7 }
  object { scheibe6 rotate z* 0.00 translate z*8*8 }
  object { stufe rotate z*(360/16)*2 translate z*8*8 }
  object { stufe rotate z*(360/16)*10 translate z*8*8 }
  object { scheibe6 rotate z*11.25 translate z*8*9 }
  object { eleva rotate z*(360/16)*2 translate z*8*9 }
  object { stufe rotate z*(360/16)*3 translate z*8*9 }
  object { stufe rotate z*(360/16)*4 translate z*8*9 }
  object { stufe rotate z*(360/16)*9 translate z*8*9 }
  object { scheibe6 rotate z* 0.00 translate z*8*10 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*10 texture { moertel } }
  object { scheibe6 rotate z*11.25 translate z*8*11 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*11 texture { moertel } }
  object { scheibe6 rotate z* 0.00 translate z*8*12 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*12 texture { moertel } }
  object { scheibe6 rotate z*11.25 translate z*8*13 }
  object { stufe rotate z*(360/16)*0 translate z*8*13 }
  object { stufe rotate z*(360/16)*3 translate z*8*13 }
  object { stufe rotate z*(360/16)*4 translate z*8*13 }
  object { stufe rotate z*(360/16)*5 translate z*8*13 }
  object { stufe rotate z*(360/16)*15 translate z*8*13 }
  object { scheibe6 rotate z* 0.00 translate z*8*14 }
  object { stufe rotate z*(360/16)*6 translate z*8*14 }
  object { scheibe6 rotate z*11.25 translate z*8*15 }
  object { stufe rotate z*(360/16)*7 translate z*8*15 }
  object { scheibe6 rotate z* 0.00 translate z*8*16 }
  object { stufe rotate z*(360/16)*1 translate z*8*16 }
  object { stufe rotate z*(360/16)*8 translate z*8*16 }
  object { scheibe6 rotate z*11.25 translate z*8*17 }
  object { stufe rotate z*(360/16)*0 translate z*8*17 }
  object { stufe rotate z*(360/16)*1 translate z*8*17 }
  object { stufe rotate z*(360/16)*2 translate z*8*17 }
  object { stufe rotate z*(360/16)*3 translate z*8*17 }
  object { stufe rotate z*(360/16)*4 translate z*8*17 }
  object { stufe rotate z*(360/16)*9 translate z*8*17 }
  object { stufe rotate z*(360/16)*10 translate z*8*17 }
  object { stufe rotate z*(360/16)*11 translate z*8*17 }
  object { stufe rotate z*(360/16)*12 translate z*8*17 }
  object { stufe rotate z*(360/16)*15 translate z*8*17 }
  difference {
    object { scheibe6 rotate z* 0.00 translate z*8*18 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*18 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*18 texture { moertel } }
  }
  object { eleva rotate z*(360/16)*2 translate z*8*18 }
  object { eleva rotate z*(360/16)*3 translate z*8*18 }
  difference {
    object { scheibe6 rotate z*11.25 translate z*8*19 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*19 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*19 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*14 translate z*8*19 }
  difference {
    object { scheibe6 rotate z* 0.00 translate z*8*20 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*20 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*20 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*2 translate z*8*20 }
  object { scheibe6 rotate z*11.25 translate z*8*21 }
  object { stufe rotate z*(360/16)*4 translate z*8*21 }
  object { stufe rotate z*(360/16)*5 translate z*8*21 }
  object { stufe rotate z*(360/16)*6 translate z*8*21 }
  object { stufe rotate z*(360/16)*7 translate z*8*21 }
  object { stufe rotate z*(360/16)*13 translate z*8*21 }
  object { scheibe6 rotate z* 0.00 translate z*8*22 }
  object { stufe rotate z*(360/16)*2 translate z*8*22 }
  object { eleva rotate z*(360/16)*7 translate z*8*22 }
  object { scheibe6 rotate z*11.25 translate z*8*23 }
  object { stufe rotate z*(360/16)*0 translate z*8*23 }
  object { stufe rotate z*(360/16)*1 translate z*8*23 }
  object { stufe rotate z*(360/16)*9 translate z*8*23 }
  object { stufe rotate z*(360/16)*10 translate z*8*23 }
  object { stufe rotate z*(360/16)*12 translate z*8*23 }
  difference {
    object { scheibe6 rotate z* 0.00 translate z*8*24 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*24 texture { moertel } }
  }
  object { eleva rotate z*(360/16)*0 translate z*8*24 }
  object { stufe rotate z*(360/16)*2 translate z*8*24 }
  difference {
    object { scheibe6 rotate z*11.25 translate z*8*25 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*25 texture { moertel } }
  }
  difference {
    object { scheibe6 rotate z* 0.00 translate z*8*26 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*26 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*2 translate z*8*26 }
  object { stufe rotate z*(360/16)*8 translate z*8*26 }
  object { scheibe6 rotate z*11.25 translate z*8*27 }
  object { stufe rotate z*(360/16)*6 translate z*8*27 }
  object { scheibe6 rotate z* 0.00 translate z*8*28 }
  object { stufe rotate z*(360/16)*5 translate z*8*28 }
  object { stufe rotate z*(360/16)*8 translate z*8*28 }
  object { stufe rotate z*(360/16)*9 translate z*8*28 }
  object { stufe rotate z*(360/16)*10 translate z*8*28 }
  object { stufe rotate z*(360/16)*11 translate z*8*28 }
  object { stufe rotate z*(360/16)*12 translate z*8*28 }
  object { stufe rotate z*(360/16)*13 translate z*8*28 }
  object { scheibe6 rotate z*11.25 translate z*8*29 }
  object { eleva rotate z*(360/16)*13 translate z*8*29 }
  object { scheibe6 rotate z* 0.00 translate z*8*30 }
  object { scheibe6 rotate z*11.25 translate z*8*31 }
  object { scheibe6 rotate z* 0.00 translate z*8*32 }
  object { scheibe6 rotate z*11.25 translate z*8*33 }
  object { scheibe6 rotate z* 0.00 translate z*8*34 }
  object { scheibe6 rotate z*11.25 translate z*8*35 }
  object { scheibe6 rotate z* 0.00 translate z*8*36 }
  object { stufe rotate z*(360/16)*9 translate z*8*36 }
  object { scheibe6 rotate z*11.25 translate z*8*37 }
  object { eleva rotate z*(360/16)*9 translate z*8*37 }
  object { stufe rotate z*(360/16)*10 translate z*8*37 }
  object { scheibe6 rotate z* 0.00 translate z*8*38 }
  object { stufe rotate z*(360/16)*11 translate z*8*38 }
  object { stufe rotate z*(360/16)*12 translate z*8*38 }
  object { scheibe6 rotate z*11.25 translate z*8*39 }
  object { scheibe6 rotate z* 0.00 translate z*8*40 }
  object { scheibe6 rotate z*11.25 translate z*8*41 }
  object { scheibe6 rotate z* 0.00 translate z*8*42 }
  object { scheibe6 rotate z*11.25 translate z*8*43 }
  object { stufe rotate z*(360/16)*3 translate z*8*43 }
  object { stufe rotate z*(360/16)*4 translate z*8*43 }
  object { scheibe6 rotate z* 0.00 translate z*8*44 }
  object { stufe rotate z*(360/16)*5 translate z*8*44 }
  object { scheibe6 rotate z*11.25 translate z*8*45 }
  object { stufe rotate z*(360/16)*6 translate z*8*45 }
  object { stufe rotate z*(360/16)*11 translate z*8*45 }
  object { scheibe6 rotate z* 0.00 translate z*8*46 }
  object { scheibe6 rotate z*11.25 translate z*8*47 }
  object { stufe rotate z*(360/16)*12 translate z*8*47 }
  object { scheibe6 rotate z* 0.00 translate z*8*48 }
  object { stufe rotate z*(360/16)*7 translate z*8*48 }
  object { scheibe6 rotate z*11.25 translate z*8*49 }
  object { stufe rotate z*(360/16)*10 translate z*8*49 }
  object { stufe rotate z*(360/16)*13 translate z*8*49 }
  object { scheibe6 rotate z* 0.00 translate z*8*50 }
  object { scheibe6 rotate z*11.25 translate z*8*51 }
  object { stufe rotate z*(360/16)*14 translate z*8*51 }
  object { scheibe6 rotate z* 0.00 translate z*8*52 }
  object { scheibe6 rotate z*11.25 translate z*8*53 }
  object { stufe rotate z*(360/16)*15 translate z*8*53 }
  object { scheibe6 rotate z* 0.00 translate z*8*54 }
  object { scheibe6 rotate z*11.25 translate z*8*55 }
  object { stufe rotate z*(360/16)*0 translate z*8*55 }
  object { scheibe6 rotate z* 0.00 translate z*8*56 }
  object { scheibe6 rotate z*11.25 translate z*8*57 }
  object { stufe rotate z*(360/16)*1 translate z*8*57 }
  object { scheibe6 rotate z* 0.00 translate z*8*58 }
  object { scheibe6 rotate z*11.25 translate z*8*59 }
  object { stufe rotate z*(360/16)*2 translate z*8*59 }
  difference {
    object { scheibe6 rotate z* 0.00 translate z*8*60 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*2 translate z*8*60 texture { moertel } }
  }
  difference {
    object { scheibe6 rotate z*11.25 translate z*8*61 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*2 translate z*8*61 texture { moertel } }
  }
  difference {
    object { scheibe6 rotate z* 0.00 translate z*8*62 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*2 translate z*8*62 texture { moertel } }
  }
  object { scheibe6 rotate z*11.25 translate z*8*63 }
  object { zinnen6 translate z*512 }
}
#declare turm6 = object {
  object { turmot6 }

  bounded_by { cylinder { <0, 0, 0> <0, 0, 540> 80 } }
}
