#declare turmot1 = union {
  cylinder { <0,0,0>, <0,0,376>, 46 texture { moertel } }
  object { scheibe1 rotate z* 0.00 translate z*8*0 }
  object { scheibe1 rotate z*11.25 translate z*8*1 }
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
  difference {
    object { scheibe1 rotate z* 0.00 translate z*8*2 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*2 translate z*8*2 texture { moertel } }
  }
  object { eleva rotate z*(360/16)*6 translate z*8*2 }

  difference {
    object { scheibe1 rotate z*11.25 translate z*8*3 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*2 translate z*8*3 texture { moertel } }
  }
  difference {
    object { scheibe1 rotate z* 0.00 translate z*8*4 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*2 translate z*8*4 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*12 translate z*8*4 }
  object { stufe rotate z*(360/16)*13 translate z*8*4 }
  object { stufe rotate z*(360/16)*14 translate z*8*4 }

  difference {
    object { scheibe1 rotate z*11.25 translate z*8*5 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*6 translate z*8*5 texture { moertel } }
  }
  object { eleva rotate z*(360/16)*12 translate z*8*5 }

  difference {
    object { scheibe1 rotate z* 0.00 translate z*8*6 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*6 translate z*8*6 texture { moertel } }
  }
  difference {
    object { scheibe1 rotate z*11.25 translate z*8*7 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*6 translate z*8*7 texture { moertel } }
  }

  object { scheibe1 rotate z* 0.00 translate z*8*8 }
  object { scheibe1 rotate z*11.25 translate z*8*9 }
  object { stufe rotate z*(360/16)*8 translate z*8*9 }
  object { stufe rotate z*(360/16)*9 translate z*8*9 }
  object { stufe rotate z*(360/16)*10 translate z*8*9 }
  object { stufe rotate z*(360/16)*11 translate z*8*9 }
  object { stufe rotate z*(360/16)*13 translate z*8*9 }
  object { stufe rotate z*(360/16)*14 translate z*8*9 }
  object { scheibe1 rotate z* 0.00 translate z*8*10 }
  object { eleva rotate z*(360/16)*14 translate z*8*10 }
  object { scheibe1 rotate z*11.25 translate z*8*11 }
  object { scheibe1 rotate z* 0.00 translate z*8*12 }
  object { scheibe1 rotate z*11.25 translate z*8*13 }
  object { scheibe1 rotate z* 0.00 translate z*8*14 }
  object { stufe rotate z*(360/16)*9 translate z*8*14 }
  object { stufe rotate z*(360/16)*10 translate z*8*14 }
  object { stufe rotate z*(360/16)*11 translate z*8*14 }
  object { stufe rotate z*(360/16)*12 translate z*8*14 }
  object { stufe rotate z*(360/16)*13 translate z*8*14 }
  object { scheibe1 rotate z*11.25 translate z*8*15 }
  object { stufe rotate z*(360/16)*8 translate z*8*15 }
  object { scheibe1 rotate z* 0.00 translate z*8*16 }
  object { stufe rotate z*(360/16)*7 translate z*8*16 }
  object { scheibe1 rotate z*11.25 translate z*8*17 }
  object { stufe rotate z*(360/16)*6 translate z*8*17 }
  object { scheibe1 rotate z* 0.00 translate z*8*18 }
  object { stufe rotate z*(360/16)*4 translate z*8*18 }
  object { stufe rotate z*(360/16)*5 translate z*8*18 }
  object { scheibe1 rotate z*11.25 translate z*8*19 }
  object { stufe rotate z*(360/16)*1 translate z*8*19 }
  object { stufe rotate z*(360/16)*2 translate z*8*19 }
  object { stufe rotate z*(360/16)*3 translate z*8*19 }
  object { stufe rotate z*(360/16)*9 translate z*8*19 }
  object { stufe rotate z*(360/16)*10 translate z*8*19 }
  object { stufe rotate z*(360/16)*11 translate z*8*19 }
  object { stufe rotate z*(360/16)*12 translate z*8*19 }
  object { stufe rotate z*(360/16)*13 translate z*8*19 }
  object { stufe rotate z*(360/16)*14 translate z*8*19 }

  difference {
    object { scheibe1 rotate z* 0.00 translate z*8*20 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*20 texture { moertel } }
  }
  object { eleva rotate z*(360/16)*14 translate z*8*20 }

  difference {
    object { scheibe1 rotate z*11.25 translate z*8*21 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*21 texture { moertel } }
  }
  difference {
    object { scheibe1 rotate z* 0.00 translate z*8*22 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*22 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*5 translate z*8*22 }
  object { scheibe1 rotate z*11.25 translate z*8*23 }
  object { eleva rotate z*(360/16)*5 translate z*8*23 }
  object { scheibe1 rotate z* 0.00 translate z*8*24 }
  object { stufe rotate z*(360/16)*5 translate z*8*24 }
  object { scheibe1 rotate z*11.25 translate z*8*25 }
  object { stufe rotate z*(360/16)*5 translate z*8*25 }
  object { scheibe1 rotate z* 0.00 translate z*8*26 }
  object { stufe rotate z*(360/16)*4 translate z*8*26 }
  object { stufe rotate z*(360/16)*5 translate z*8*26 }
  object { stufe rotate z*(360/16)*6 translate z*8*26 }
  object { stufe rotate z*(360/16)*7 translate z*8*26 }
  object { stufe rotate z*(360/16)*8 translate z*8*26 }
  object { stufe rotate z*(360/16)*9 translate z*8*26 }
  object { stufe rotate z*(360/16)*10 translate z*8*26 }
  object { stufe rotate z*(360/16)*11 translate z*8*26 }
  object { stufe rotate z*(360/16)*12 translate z*8*26 }
  object { scheibe1 rotate z*11.25 translate z*8*27 }
  object { stutz rotate z*(360/16)*4 translate z*8*27 }
  object { stutz rotate z*(360/16)*12 translate z*8*27 }
  object { scheibe1 rotate z* 0.00 translate z*8*28 }
  object { stutz rotate z*(360/16)*4 translate z*8*28 }
  object { stutz rotate z*(360/16)*12 translate z*8*28 }
  object { scheibe1 rotate z*11.25 translate z*8*29 }
  object { stufe rotate z*(360/16)*4 translate z*8*29 }
  object { stufe rotate z*(360/16)*12 translate z*8*29 }
  object { stufe rotate z*(360/16)*13 translate z*8*29 }
  object { scheibe1 rotate z* 0.00 translate z*8*30 }
  object { scheibe1 rotate z*11.25 translate z*8*31 }
  object { scheibe1 rotate z* 0.00 translate z*8*32 }
  object { scheibe1 rotate z*11.25 translate z*8*33 }
  object { scheibe1 rotate z* 0.00 translate z*8*34 }
  object { stufe rotate z*(360/16)*6 translate z*8*34 }
  object { stufe rotate z*(360/16)*7 translate z*8*34 }
  object { stufe rotate z*(360/16)*8 translate z*8*34 }
  object { scheibe1 rotate z*11.25 translate z*8*35 }
  object { scheibe1 rotate z* 0.00 translate z*8*36 }
  object { scheibe1 rotate z*11.25 translate z*8*37 }
  object { scheibe1 rotate z* 0.00 translate z*8*38 }
  object { stufe rotate z*(360/16)*0 translate z*8*38 }
  object { stufe rotate z*(360/16)*1 translate z*8*38 }
  object { stufe rotate z*(360/16)*6 translate z*8*38 }
  object { stufe rotate z*(360/16)*7 translate z*8*38 }
  object { stufe rotate z*(360/16)*8 translate z*8*38 }
  object { stufe rotate z*(360/16)*10 translate z*8*38 }
  object { stufe rotate z*(360/16)*11 translate z*8*38 }
  object { stufe rotate z*(360/16)*12 translate z*8*38 }
  object { stufe rotate z*(360/16)*13 translate z*8*38 }
  object { stufe rotate z*(360/16)*14 translate z*8*38 }
  object { stufe rotate z*(360/16)*15 translate z*8*38 }

  difference {
    object { scheibe1 rotate z*11.25 translate z*8*39 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*0 translate z*8*39 texture { moertel } }
  }
  object { stutz rotate z*(360/16)*1 translate z*8*39 }
  object { stutz rotate z*(360/16)*10 translate z*8*39 }
  object { eleva rotate z*(360/16)*11 translate z*8*39 }

  difference {
    object { scheibe1 rotate z* 0.00 translate z*8*40 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*0 translate z*8*40 texture { moertel } }
  }
  object { stutz rotate z*(360/16)*1 translate z*8*40 }
  object { stutz rotate z*(360/16)*10 translate z*8*40 }

  difference {
    object { scheibe1 rotate z*11.25 translate z*8*41 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*0 translate z*8*41 texture { moertel } }
  }
  object { stutz rotate z*(360/16)*1 translate z*8*41 }
  object { stutz rotate z*(360/16)*10 translate z*8*41 }
  object { scheibe1 rotate z* 0.00 translate z*8*42 }
  object { stufe rotate z*(360/16)*1 translate z*8*42 }
  object { stufe rotate z*(360/16)*2 translate z*8*42 }
  object { stufe rotate z*(360/16)*3 translate z*8*42 }
  object { stufe rotate z*(360/16)*4 translate z*8*42 }
  object { stufe rotate z*(360/16)*9 translate z*8*42 }
  object { stufe rotate z*(360/16)*10 translate z*8*42 }

  difference {
    object { scheibe1 rotate z*11.25 translate z*8*43 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*43 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*43 texture { moertel } }
  }
  difference {
    object { scheibe1 rotate z* 0.00 translate z*8*44 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*44 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*44 texture { moertel } }
  }
  difference {
    object { scheibe1 rotate z*11.25 translate z*8*45 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*45 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*45 texture { moertel } }
  }
  object { scheibe1 rotate z* 0.00 translate z*8*46 }
  object { zinnen1 translate z*376 }
}
#declare turm1 = object {
  object { turmot1 }
  
  bounded_by { cylinder { <0, 0, 0>, <0, 0, 403>, 80 } }
}
