#declare turmot5 = union {
  cylinder { <0,0,0>, <0,0,768>, 46 texture { moertel } }
  object { scheibe5 rotate z* 0.00 translate z*8*0 }
  object { scheibe5 rotate z*11.25 translate z*8*1 }
  object { stufe rotate z*(360/16)*0 translate z*8*1 }
  object { stufe rotate z*(360/16)*1 translate z*8*1 }
  object { stufe rotate z*(360/16)*6 translate z*8*1 }
  object { stufe rotate z*(360/16)*7 translate z*8*1 }
  object { stufe rotate z*(360/16)*8 translate z*8*1 }
  object { stufe rotate z*(360/16)*9 translate z*8*1 }
  object { scheibe5 rotate z* 0.00 translate z*8*2 }
  object { eleva rotate z*(360/16)*1 translate z*8*2 }
  object { eleva rotate z*(360/16)*6 translate z*8*2 }
  object { scheibe5 rotate z*11.25 translate z*8*3 }
  object { scheibe5 rotate z* 0.00 translate z*8*4 }
  object { scheibe5 rotate z*11.25 translate z*8*5 }
  object { scheibe5 rotate z* 0.00 translate z*8*6 }
  object { stufe rotate z*(360/16)*2 translate z*8*6 }
  object { stufe rotate z*(360/16)*3 translate z*8*6 }
  object { stufe rotate z*(360/16)*4 translate z*8*6 }
  object { stufe rotate z*(360/16)*5 translate z*8*6 }
  object { stufe rotate z*(360/16)*7 translate z*8*6 }
  object { stufe rotate z*(360/16)*8 translate z*8*6 }
  object { stufe rotate z*(360/16)*9 translate z*8*6 }
  object { stufe rotate z*(360/16)*10 translate z*8*6 }
  object { stufe rotate z*(360/16)*11 translate z*8*6 }
  object { stufe rotate z*(360/16)*12 translate z*8*6 }
  object { stufe rotate z*(360/16)*13 translate z*8*6 }
  object { stufe rotate z*(360/16)*14 translate z*8*6 }
  object { scheibe5 rotate z*11.25 translate z*8*7 }
  object { stutz rotate z*(360/16)*14 translate z*8*7 }
  object { scheibe5 rotate z* 0.00 translate z*8*8 }
  object { stutz rotate z*(360/16)*14 translate z*8*8 }
  object { scheibe5 rotate z*11.25 translate z*8*9 }
  object { stufe rotate z*(360/16)*7 translate z*8*9 }
  object { stutz rotate z*(360/16)*14 translate z*8*9 }
  object { scheibe5 rotate z* 0.00 translate z*8*10 }
  object { stufe rotate z*(360/16)*0 translate z*8*10 }
  object { stufe rotate z*(360/16)*8 translate z*8*10 }
  object { stufe rotate z*(360/16)*14 translate z*8*10 }
  object { stufe rotate z*(360/16)*15 translate z*8*10 }
  object { scheibe5 rotate z*11.25 translate z*8*11 }
  object { stufe rotate z*(360/16)*9 translate z*8*11 }
  object { scheibe5 rotate z* 0.00 translate z*8*12 }
  object { stufe rotate z*(360/16)*10 translate z*8*12 }
  object { scheibe5 rotate z*11.25 translate z*8*13 }
  object { stufe rotate z*(360/16)*3 translate z*8*13 }
  object { stufe rotate z*(360/16)*11 translate z*8*13 }
  difference {
    object { scheibe5 rotate z* 0.00 translate z*8*14 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*14 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*4 translate z*8*14 }
  difference {
    object { scheibe5 rotate z*11.25 translate z*8*15 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*15 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*5 translate z*8*15 }
  difference {
    object { scheibe5 rotate z* 0.00 translate z*8*16 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*16 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*6 translate z*8*16 }
  object { scheibe5 rotate z*11.25 translate z*8*17 }
  object { stufe rotate z*(360/16)*7 translate z*8*17 }
  object { scheibe5 rotate z* 0.00 translate z*8*18 }
  object { stufe rotate z*(360/16)*8 translate z*8*18 }
  object { stufe rotate z*(360/16)*9 translate z*8*18 }
  object { scheibe5 rotate z*11.25 translate z*8*19 }
  object { eleva rotate z*(360/16)*9 translate z*8*19 }
  object { scheibe5 rotate z* 0.00 translate z*8*20 }
  object { scheibe5 rotate z*11.25 translate z*8*21 }
  object { scheibe5 rotate z* 0.00 translate z*8*22 }
  object { stufe rotate z*(360/16)*1 translate z*8*22 }
  object { stufe rotate z*(360/16)*2 translate z*8*22 }
  object { stufe rotate z*(360/16)*3 translate z*8*22 }
  object { scheibe5 rotate z*11.25 translate z*8*23 }
  object { eleva rotate z*(360/16)*1 translate z*8*23 }
  object { eleva rotate z*(360/16)*3 translate z*8*23 }
  object { scheibe5 rotate z* 0.00 translate z*8*24 }
  object { scheibe5 rotate z*11.25 translate z*8*25 }
  object { scheibe5 rotate z* 0.00 translate z*8*26 }
  object { scheibe5 rotate z*11.25 translate z*8*27 }
  object { stufe rotate z*(360/16)*1 translate z*8*27 }
  object { stufe rotate z*(360/16)*3 translate z*8*27 }
  object { scheibe5 rotate z* 0.00 translate z*8*28 }
  object { stufe rotate z*(360/16)*2 translate z*8*28 }
  object { stufe rotate z*(360/16)*10 translate z*8*28 }
  object { stufe rotate z*(360/16)*11 translate z*8*28 }
  object { scheibe5 rotate z*11.25 translate z*8*29 }
  object { eleva rotate z*(360/16)*11 translate z*8*29 }
  object { scheibe5 rotate z* 0.00 translate z*8*30 }
  object { stufe rotate z*(360/16)*0 translate z*8*30 }
  object { stufe rotate z*(360/16)*2 translate z*8*30 }
  object { stufe rotate z*(360/16)*4 translate z*8*30 }
  object { stufe rotate z*(360/16)*6 translate z*8*30 }
  object { stufe rotate z*(360/16)*8 translate z*8*30 }
  object { scheibe5 rotate z*11.25 translate z*8*31 }
  object { scheibe5 rotate z* 0.00 translate z*8*32 }
  object { scheibe5 rotate z*11.25 translate z*8*33 }
  object { stufe rotate z*(360/16)*10 translate z*8*33 }
  object { scheibe5 rotate z* 0.00 translate z*8*34 }
  object { scheibe5 rotate z*11.25 translate z*8*35 }
  object { scheibe5 rotate z* 0.00 translate z*8*36 }
  object { scheibe5 rotate z*11.25 translate z*8*37 }
  object { stufe rotate z*(360/16)*9 translate z*8*37 }
  object { scheibe5 rotate z* 0.00 translate z*8*38 }
  object { stufe rotate z*(360/16)*5 translate z*8*38 }
  object { stufe rotate z*(360/16)*6 translate z*8*38 }
  object { stufe rotate z*(360/16)*7 translate z*8*38 }
  object { scheibe5 rotate z*11.25 translate z*8*39 }
  object { eleva rotate z*(360/16)*5 translate z*8*39 }
  object { scheibe5 rotate z* 0.00 translate z*8*40 }
  object { stufe rotate z*(360/16)*0 translate z*8*40 }
  object { stufe rotate z*(360/16)*8 translate z*8*40 }
  object { stufe rotate z*(360/16)*10 translate z*8*40 }
  object { stufe rotate z*(360/16)*12 translate z*8*40 }
  object { stufe rotate z*(360/16)*14 translate z*8*40 }
  object { stufe rotate z*(360/16)*15 translate z*8*40 }
  object { scheibe5 rotate z*11.25 translate z*8*41 }
  object { scheibe5 rotate z* 0.00 translate z*8*42 }
  object { scheibe5 rotate z*11.25 translate z*8*43 }
  object { scheibe5 rotate z* 0.00 translate z*8*44 }
  object { stufe rotate z*(360/16)*7 translate z*8*44 }
  object { scheibe5 rotate z*11.25 translate z*8*45 }
  object { stufe rotate z*(360/16)*12 translate z*8*45 }
  object { stufe rotate z*(360/16)*13 translate z*8*45 }
  object { stufe rotate z*(360/16)*14 translate z*8*45 }
  difference {
    object { scheibe5 rotate z* 0.00 translate z*8*46 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*6 translate z*8*46 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*8 translate z*8*46 }
  object { stufe rotate z*(360/16)*9 translate z*8*46 }
  difference {
    object { scheibe5 rotate z*11.25 translate z*8*47 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*6 translate z*8*47 texture { moertel } }
  }
  difference {
    object { scheibe5 rotate z* 0.00 translate z*8*48 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*6 translate z*8*48 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*3 translate z*8*48 }
  object { scheibe5 rotate z*11.25 translate z*8*49 }
  object { scheibe5 rotate z* 0.00 translate z*8*50 }
  object { scheibe5 rotate z*11.25 translate z*8*51 }
  object { stufe rotate z*(360/16)*11 translate z*8*51 }
  object { scheibe5 rotate z* 0.00 translate z*8*52 }
  object { eleva rotate z*(360/16)*11 translate z*8*52 }
  object { stufe rotate z*(360/16)*12 translate z*8*52 }
  object { scheibe5 rotate z*11.25 translate z*8*53 }
  object { stufe rotate z*(360/16)*13 translate z*8*53 }
  object { scheibe5 rotate z* 0.00 translate z*8*54 }
  object { stufe rotate z*(360/16)*0 translate z*8*54 }
  object { stufe rotate z*(360/16)*14 translate z*8*54 }
  object { stufe rotate z*(360/16)*15 translate z*8*54 }
  object { scheibe5 rotate z*11.25 translate z*8*55 }
  object { stufe rotate z*(360/16)*4 translate z*8*55 }
  object { stufe rotate z*(360/16)*6 translate z*8*55 }
  object { scheibe5 rotate z* 0.00 translate z*8*56 }
  object { stufe rotate z*(360/16)*3 translate z*8*56 }
  object { stufe rotate z*(360/16)*7 translate z*8*56 }
  object { stufe rotate z*(360/16)*8 translate z*8*56 }
  object { scheibe5 rotate z*11.25 translate z*8*57 }
  object { stufe rotate z*(360/16)*2 translate z*8*57 }
  object { scheibe5 rotate z* 0.00 translate z*8*58 }
  object { stufe rotate z*(360/16)*1 translate z*8*58 }
  object { scheibe5 rotate z*11.25 translate z*8*59 }
  object { scheibe5 rotate z* 0.00 translate z*8*60 }
  object { stufe rotate z*(360/16)*4 translate z*8*60 }
  object { stufe rotate z*(360/16)*6 translate z*8*60 }
  object { scheibe5 rotate z*11.25 translate z*8*61 }
  object { scheibe5 rotate z* 0.00 translate z*8*62 }
  object { scheibe5 rotate z*11.25 translate z*8*63 }
  object { stufe rotate z*(360/16)*3 translate z*8*63 }
  difference {
    object { scheibe5 rotate z* 0.00 translate z*8*64 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*64 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*4 translate z*8*64 }
  difference {
    object { scheibe5 rotate z*11.25 translate z*8*65 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*65 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*5 translate z*8*65 }
  difference {
    object { scheibe5 rotate z* 0.00 translate z*8*66 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*66 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*5 translate z*8*66 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*6 translate z*8*66 }
  difference {
    object { scheibe5 rotate z*11.25 translate z*8*67 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*5 translate z*8*67 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*7 translate z*8*67 }
  difference {
    object { scheibe5 rotate z* 0.00 translate z*8*68 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*5 translate z*8*68 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*7 translate z*8*68 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*8 translate z*8*68 }
  difference {
    object { scheibe5 rotate z*11.25 translate z*8*69 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*7 translate z*8*69 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*1 translate z*8*69 }
  object { stufe rotate z*(360/16)*9 translate z*8*69 }
  difference {
    object { scheibe5 rotate z* 0.00 translate z*8*70 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*70 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*7 translate z*8*70 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*2 translate z*8*70 }
  difference {
    object { scheibe5 rotate z*11.25 translate z*8*71 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*71 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*3 translate z*8*71 }
  difference {
    object { scheibe5 rotate z* 0.00 translate z*8*72 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*72 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*72 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*4 translate z*8*72 }
  difference {
    object { scheibe5 rotate z*11.25 translate z*8*73 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*73 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*5 translate z*8*73 }
  object { stufe rotate z*(360/16)*13 translate z*8*73 }
  difference {
    object { scheibe5 rotate z* 0.00 translate z*8*74 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*74 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*5 translate z*8*74 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*6 translate z*8*74 }
  object { stufe rotate z*(360/16)*12 translate z*8*74 }
  difference {
    object { scheibe5 rotate z*11.25 translate z*8*75 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*5 translate z*8*75 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*7 translate z*8*75 }
  object { stufe rotate z*(360/16)*9 translate z*8*75 }
  object { stufe rotate z*(360/16)*10 translate z*8*75 }
  object { stufe rotate z*(360/16)*11 translate z*8*75 }
  difference {
    object { scheibe5 rotate z* 0.00 translate z*8*76 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*5 translate z*8*76 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*7 translate z*8*76 texture { moertel } }
  }
  object { eleva rotate z*(360/16)*9 translate z*8*76 }
  difference {
    object { scheibe5 rotate z*11.25 translate z*8*77 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*7 translate z*8*77 texture { moertel } }
  }
  difference {
    object { scheibe5 rotate z* 0.00 translate z*8*78 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*7 translate z*8*78 texture { moertel } }
  }
  object { scheibe5 rotate z*11.25 translate z*8*79 }
  object { scheibe5 rotate z* 0.00 translate z*8*80 }
  object { scheibe5 rotate z*11.25 translate z*8*81 }
  object { scheibe5 rotate z* 0.00 translate z*8*82 }
  object { scheibe5 rotate z*11.25 translate z*8*83 }
  object { scheibe5 rotate z* 0.00 translate z*8*84 }
  object { scheibe5 rotate z*11.25 translate z*8*85 }
  object { stufe rotate z*(360/16)*0 translate z*8*85 }
  object { stufe rotate z*(360/16)*1 translate z*8*85 }
  object { stufe rotate z*(360/16)*2 translate z*8*85 }
  object { stufe rotate z*(360/16)*3 translate z*8*85 }
  object { stufe rotate z*(360/16)*4 translate z*8*85 }
  object { stufe rotate z*(360/16)*5 translate z*8*85 }
  object { stufe rotate z*(360/16)*6 translate z*8*85 }
  object { stufe rotate z*(360/16)*7 translate z*8*85 }
  object { stufe rotate z*(360/16)*8 translate z*8*85 }
  object { stufe rotate z*(360/16)*10 translate z*8*85 }
  object { stufe rotate z*(360/16)*11 translate z*8*85 }
  object { stufe rotate z*(360/16)*12 translate z*8*85 }
  object { stufe rotate z*(360/16)*13 translate z*8*85 }
  object { stufe rotate z*(360/16)*14 translate z*8*85 }
  object { stufe rotate z*(360/16)*15 translate z*8*85 }
  object { scheibe5 rotate z* 0.00 translate z*8*86 }
  object { stutz rotate z*(360/16)*10 translate z*8*86 }
  object { eleva rotate z*(360/16)*11 translate z*8*86 }
  object { scheibe5 rotate z*11.25 translate z*8*87 }
  object { stutz rotate z*(360/16)*10 translate z*8*87 }
  object { scheibe5 rotate z* 0.00 translate z*8*88 }
  object { stutz rotate z*(360/16)*10 translate z*8*88 }
  object { scheibe5 rotate z*11.25 translate z*8*89 }
  object { stutz rotate z*(360/16)*10 translate z*8*89 }
  object { scheibe5 rotate z* 0.00 translate z*8*90 }
  object { stutz rotate z*(360/16)*10 translate z*8*90 }
  object { scheibe5 rotate z*11.25 translate z*8*91 }
  object { stufe rotate z*(360/16)*10 translate z*8*91 }
  object { scheibe5 rotate z* 0.00 translate z*8*92 }
  object { scheibe5 rotate z*11.25 translate z*8*93 }
  object { stufe rotate z*(360/16)*5 translate z*8*93 }
  object { scheibe5 rotate z* 0.00 translate z*8*94 }
  object { stufe rotate z*(360/16)*4 translate z*8*94 }
  object { scheibe5 rotate z*11.25 translate z*8*95 }
  object { zinnen5 translate z*768 }
}
#declare turm5 = object {
  object { turmot5 }
    
  bounded_by { cylinder { <0, 0, 0> <0, 0, 800> 80 } }
}
