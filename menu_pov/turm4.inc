#declare turmot4 = union {
  cylinder { <0,0,0>, <0,0,528>, 46 texture { moertel } }
  object { scheibe4 rotate z* 0.00 translate z*8*0 }
  object { scheibe4 rotate z*11.25 translate z*8*1 }
  object { stufe rotate z*(360/16)*0 translate z*8*1 }
  object { stufe rotate z*(360/16)*4 translate z*8*1 }
  object { stufe rotate z*(360/16)*6 translate z*8*1 }
  object { stufe rotate z*(360/16)*11 translate z*8*1 }
  object { stufe rotate z*(360/16)*12 translate z*8*1 }
  object { stufe rotate z*(360/16)*13 translate z*8*1 }
  object { stufe rotate z*(360/16)*14 translate z*8*1 }
  object { stufe rotate z*(360/16)*15 translate z*8*1 }
  object { scheibe4 rotate z* 0.00 translate z*8*2 }
  object { stufe rotate z*(360/16)*1 translate z*8*2 }
  object { scheibe4 rotate z*11.25 translate z*8*3 }
  object { stufe rotate z*(360/16)*2 translate z*8*3 }
  object { stufe rotate z*(360/16)*7 translate z*8*3 }
  object { scheibe4 rotate z* 0.00 translate z*8*4 }
  object { stufe rotate z*(360/16)*3 translate z*8*4 }
  object { stufe rotate z*(360/16)*5 translate z*8*4 }
  object { eleva rotate z*(360/16)*7 translate z*8*4 }
  object { scheibe4 rotate z*11.25 translate z*8*5 }
  object { scheibe4 rotate z* 0.00 translate z*8*6 }
  object { scheibe4 rotate z*11.25 translate z*8*7 }
  object { stufe rotate z*(360/16)*0 translate z*8*7 }
  object { stufe rotate z*(360/16)*9 translate z*8*7 }
  object { stufe rotate z*(360/16)*10 translate z*8*7 }
  object { stufe rotate z*(360/16)*11 translate z*8*7 }
  object { stufe rotate z*(360/16)*12 translate z*8*7 }
  object { stufe rotate z*(360/16)*13 translate z*8*7 }
  object { stufe rotate z*(360/16)*14 translate z*8*7 }
  object { stufe rotate z*(360/16)*15 translate z*8*7 }
  object { scheibe4 rotate z* 0.00 translate z*8*8 }
  object { stufe rotate z*(360/16)*1 translate z*8*8 }
  object { eleva rotate z*(360/16)*9 translate z*8*8 }
  object { scheibe4 rotate z*11.25 translate z*8*9 }
  object { stufe rotate z*(360/16)*2 translate z*8*9 }
  object { stufe rotate z*(360/16)*3 translate z*8*9 }
  object { stufe rotate z*(360/16)*4 translate z*8*9 }
  object { stufe rotate z*(360/16)*5 translate z*8*9 }
  object { stufe rotate z*(360/16)*6 translate z*8*9 }
  object { scheibe4 rotate z* 0.00 translate z*8*10 }
  object { scheibe4 rotate z*11.25 translate z*8*11 }
  object { scheibe4 rotate z* 0.00 translate z*8*12 }
  object { stufe rotate z*(360/16)*0 translate z*8*12 }
  object { stufe rotate z*(360/16)*8 translate z*8*12 }
  object { stufe rotate z*(360/16)*14 translate z*8*12 }
  object { stufe rotate z*(360/16)*15 translate z*8*12 }
  difference {
    object { scheibe4 rotate z*11.25 translate z*8*13 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*0 translate z*8*13 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*3 translate z*8*13 }
  object { stufe rotate z*(360/16)*4 translate z*8*13 }
  object { stufe rotate z*(360/16)*5 translate z*8*13 }
  object { stufe rotate z*(360/16)*13 translate z*8*13 }
  difference {
    object { scheibe4 rotate z* 0.00 translate z*8*14 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*0 translate z*8*14 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*5 translate z*8*14 texture { moertel } }
  }
  object { eleva rotate z*(360/16)*3 translate z*8*14 }
  difference {
    object { scheibe4 rotate z*11.25 translate z*8*15 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*0 translate z*8*15 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*5 translate z*8*15 texture { moertel } }
  }
  difference {
    object { scheibe4 rotate z* 0.00 translate z*8*16 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*5 translate z*8*16 texture { moertel } }
  }
  object { scheibe4 rotate z*11.25 translate z*8*17 }
  object { scheibe4 rotate z* 0.00 translate z*8*18 }
  object { scheibe4 rotate z*11.25 translate z*8*19 }
  object { stufe rotate z*(360/16)*1 translate z*8*19 }
  object { scheibe4 rotate z* 0.00 translate z*8*20 }
  object { eleva rotate z*(360/16)*1 translate z*8*20 }
  object { stufe rotate z*(360/16)*4 translate z*8*20 }
  object { stufe rotate z*(360/16)*5 translate z*8*20 }
  object { stufe rotate z*(360/16)*6 translate z*8*20 }
  object { stufe rotate z*(360/16)*14 translate z*8*20 }
  difference {
    object { scheibe4 rotate z*11.25 translate z*8*21 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*6 translate z*8*21 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*15 translate z*8*21 }
  difference {
    object { scheibe4 rotate z* 0.00 translate z*8*22 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*6 translate z*8*22 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*0 translate z*8*22 }
  object { stufe rotate z*(360/16)*8 translate z*8*22 }
  difference {
    object { scheibe4 rotate z*11.25 translate z*8*23 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*0 translate z*8*23 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*6 translate z*8*23 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*9 translate z*8*23 }
  difference {
    object { scheibe4 rotate z* 0.00 translate z*8*24 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*0 translate z*8*24 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*2 translate z*8*24 }
  object { stufe rotate z*(360/16)*4 translate z*8*24 }
  object { stufe rotate z*(360/16)*10 translate z*8*24 }
  difference {
    object { scheibe4 rotate z*11.25 translate z*8*25 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*0 translate z*8*25 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*2 translate z*8*25 texture { moertel } }
  }
  object { stutz rotate z*(360/16)*4 translate z*8*25 }
  object { stufe rotate z*(360/16)*7 translate z*8*25 }
  difference {
    object { scheibe4 rotate z* 0.00 translate z*8*26 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*2 translate z*8*26 texture { moertel } }
  }
  object { stutz rotate z*(360/16)*4 translate z*8*26 }
  difference {
    object { scheibe4 rotate z*11.25 translate z*8*27 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*2 translate z*8*27 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*0 translate z*8*27 }
  object { stutz rotate z*(360/16)*4 translate z*8*27 }
  object { stufe rotate z*(360/16)*6 translate z*8*27 }
  object { scheibe4 rotate z* 0.00 translate z*8*28 }
  object { stutz rotate z*(360/16)*4 translate z*8*28 }
  object { stufe rotate z*(360/16)*5 translate z*8*28 }
  object { eleva rotate z*(360/16)*6 translate z*8*28 }
  object { scheibe4 rotate z*11.25 translate z*8*29 }
  object { stufe rotate z*(360/16)*2 translate z*8*29 }
  object { stufe rotate z*(360/16)*3 translate z*8*29 }
  object { stufe rotate z*(360/16)*4 translate z*8*29 }
  object { scheibe4 rotate z* 0.00 translate z*8*30 }
  object { scheibe4 rotate z*11.25 translate z*8*31 }
  object { scheibe4 rotate z* 0.00 translate z*8*32 }
  object { stufe rotate z*(360/16)*12 translate z*8*32 }
  object { scheibe4 rotate z*11.25 translate z*8*33 }
  object { stufe rotate z*(360/16)*11 translate z*8*33 }
  object { scheibe4 rotate z* 0.00 translate z*8*34 }
  object { stufe rotate z*(360/16)*3 translate z*8*34 }
  object { stufe rotate z*(360/16)*4 translate z*8*34 }
  object { stufe rotate z*(360/16)*5 translate z*8*34 }
  object { stufe rotate z*(360/16)*7 translate z*8*34 }
  object { stufe rotate z*(360/16)*8 translate z*8*34 }
  object { stufe rotate z*(360/16)*9 translate z*8*34 }
  object { stufe rotate z*(360/16)*10 translate z*8*34 }
  object { stufe rotate z*(360/16)*15 translate z*8*34 }
  difference {
    object { scheibe4 rotate z*11.25 translate z*8*35 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*7 translate z*8*35 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*14 translate z*8*35 }
  difference {
    object { scheibe4 rotate z* 0.00 translate z*8*36 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*7 translate z*8*36 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*13 translate z*8*36 }
  object { scheibe4 rotate z*11.25 translate z*8*37 }
  difference {
    object { stufe rotate z*(360/16)*12 translate z*8*37 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*7 translate z*8*37 texture { moertel } }
  }
  object { scheibe4 rotate z* 0.00 translate z*8*38 }
  object { stufe rotate z*(360/16)*11 translate z*8*38 }
  object { scheibe4 rotate z*11.25 translate z*8*39 }
  object { stufe rotate z*(360/16)*5 translate z*8*39 }
  object { stufe rotate z*(360/16)*7 translate z*8*39 }
  object { stufe rotate z*(360/16)*10 translate z*8*39 }
  object { scheibe4 rotate z* 0.00 translate z*8*40 }
  object { stufe rotate z*(360/16)*9 translate z*8*40 }
  object { scheibe4 rotate z*11.25 translate z*8*41 }
  object { stufe rotate z*(360/16)*8 translate z*8*41 }
  object { scheibe4 rotate z* 0.00 translate z*8*42 }
  object { stufe rotate z*(360/16)*7 translate z*8*42 }
  object { scheibe4 rotate z*11.25 translate z*8*43 }
  object { stufe rotate z*(360/16)*1 translate z*8*43 }
  object { stufe rotate z*(360/16)*2 translate z*8*43 }
  object { stufe rotate z*(360/16)*4 translate z*8*43 }
  object { stufe rotate z*(360/16)*5 translate z*8*43 }
  object { stufe rotate z*(360/16)*6 translate z*8*43 }
  object { scheibe4 rotate z* 0.00 translate z*8*44 }
  object { eleva rotate z*(360/16)*1 translate z*8*44 }
  object { stufe rotate z*(360/16)*3 translate z*8*44 }
  object { scheibe4 rotate z*11.25 translate z*8*45 }
  object { scheibe4 rotate z* 0.00 translate z*8*46 }
  object { scheibe4 rotate z*11.25 translate z*8*47 }
  object { stufe rotate z*(360/16)*11 translate z*8*47 }
  object { stufe rotate z*(360/16)*12 translate z*8*47 }
  object { scheibe4 rotate z* 0.00 translate z*8*48 }
  object { eleva rotate z*(360/16)*12 translate z*8*48 }
  object { scheibe4 rotate z*11.25 translate z*8*49 }
  object { scheibe4 rotate z* 0.00 translate z*8*50 }
  object { scheibe4 rotate z*11.25 translate z*8*51 }
  object { stufe rotate z*(360/16)*6 translate z*8*51 }
  object { stufe rotate z*(360/16)*7 translate z*8*51 }
  object { stufe rotate z*(360/16)*8 translate z*8*51 }
  object { stufe rotate z*(360/16)*9 translate z*8*51 }
  object { stufe rotate z*(360/16)*10 translate z*8*51 }
  object { scheibe4 rotate z* 0.00 translate z*8*52 }
  object { stufe rotate z*(360/16)*6 translate z*8*52 }
  object { scheibe4 rotate z*11.25 translate z*8*53 }
  object { stufe rotate z*(360/16)*6 translate z*8*53 }
  object { scheibe4 rotate z* 0.00 translate z*8*54 }
  object { stufe rotate z*(360/16)*6 translate z*8*54 }
  object { scheibe4 rotate z*11.25 translate z*8*55 }
  object { stufe rotate z*(360/16)*6 translate z*8*55 }
  object { scheibe4 rotate z* 0.00 translate z*8*56 }
  object { stufe rotate z*(360/16)*6 translate z*8*56 }
  object { scheibe4 rotate z*11.25 translate z*8*57 }
  object { stufe rotate z*(360/16)*2 translate z*8*57 }
  object { stufe rotate z*(360/16)*3 translate z*8*57 }
  object { stufe rotate z*(360/16)*4 translate z*8*57 }
  object { stufe rotate z*(360/16)*5 translate z*8*57 }
  object { stufe rotate z*(360/16)*6 translate z*8*57 }
  object { stufe rotate z*(360/16)*7 translate z*8*57 }
  object { stufe rotate z*(360/16)*8 translate z*8*57 }
  object { stufe rotate z*(360/16)*9 translate z*8*57 }
  object { stufe rotate z*(360/16)*10 translate z*8*57 }
  object { stufe rotate z*(360/16)*11 translate z*8*57 }
  object { scheibe4 rotate z* 0.00 translate z*8*58 }
  object { stutz rotate z*(360/16)*2 translate z*8*58 }
  object { stutz rotate z*(360/16)*11 translate z*8*58 }
  object { scheibe4 rotate z*11.25 translate z*8*59 }
  object { stutz rotate z*(360/16)*2 translate z*8*59 }
  object { stutz rotate z*(360/16)*11 translate z*8*59 }
  object { scheibe4 rotate z* 0.00 translate z*8*60 }
  object { stutz rotate z*(360/16)*2 translate z*8*60 }
  object { stutz rotate z*(360/16)*11 translate z*8*60 }
  object { scheibe4 rotate z*11.25 translate z*8*61 }
  object { stufe rotate z*(360/16)*2 translate z*8*61 }
  object { stufe rotate z*(360/16)*9 translate z*8*61 }
  object { stufe rotate z*(360/16)*10 translate z*8*61 }
  object { stufe rotate z*(360/16)*11 translate z*8*61 }
  object { scheibe4 rotate z* 0.00 translate z*8*62 }
  object { scheibe4 rotate z*11.25 translate z*8*63 }
  object { scheibe4 rotate z* 0.00 translate z*8*64 }
  object { scheibe4 rotate z*11.25 translate z*8*65 }
  object { zinnen4 translate z*528 }
}
#declare turm4 = object {
  object { turmot4 }

  bounded_by { cylinder { <0, 0, 0> <0, 0, 560> 80 } }
}
