#declare turmot8 = union {
  cylinder { <0,0,0>, <0,0,768>, 46 texture { moertel } }
  object { scheibe8 rotate z* 0.00 translate z*8*0 }
  object { scheibe8 rotate z*11.25 translate z*8*1 }
  object { stufe rotate z*(360/16)*0 translate z*8*1 }
  object { stufe rotate z*(360/16)*1 translate z*8*1 }
  object { stufe rotate z*(360/16)*2 translate z*8*1 }
  object { stufe rotate z*(360/16)*3 translate z*8*1 }
  object { stufe rotate z*(360/16)*4 translate z*8*1 }
  object { stufe rotate z*(360/16)*5 translate z*8*1 }
  object { stufe rotate z*(360/16)*6 translate z*8*1 }
  object { stufe rotate z*(360/16)*7 translate z*8*1 }
  object { stufe rotate z*(360/16)*10 translate z*8*1 }
  object { scheibe8 rotate z* 0.00 translate z*8*2 }
  object { eleva rotate z*(360/16)*7 translate z*8*2 }
  object { eleva rotate z*(360/16)*10 translate z*8*2 }
  object { scheibe8 rotate z*11.25 translate z*8*3 }
  object { scheibe8 rotate z* 0.00 translate z*8*4 }
  object { scheibe8 rotate z*11.25 translate z*8*5 }
  object { scheibe8 rotate z* 0.00 translate z*8*6 }
  object { scheibe8 rotate z*11.25 translate z*8*7 }
  object { scheibe8 rotate z* 0.00 translate z*8*8 }
  object { scheibe8 rotate z*11.25 translate z*8*9 }
  object { stufe rotate z*(360/16)*13 translate z*8*9 }
  object { scheibe8 rotate z* 0.00 translate z*8*10 }
  object { stufe rotate z*(360/16)*3 translate z*8*10 }
  object { stufe rotate z*(360/16)*12 translate z*8*10 }
  object { stufe rotate z*(360/16)*15 translate z*8*10 }
  object { scheibe8 rotate z*11.25 translate z*8*11 }
  object { stufe rotate z*(360/16)*1 translate z*8*11 }
  object { stufe rotate z*(360/16)*5 translate z*8*11 }
  object { stufe rotate z*(360/16)*14 translate z*8*11 }
  object { scheibe8 rotate z* 0.00 translate z*8*12 }
  object { stufe rotate z*(360/16)*0 translate z*8*12 }
  object { scheibe8 rotate z*11.25 translate z*8*13 }
  object { stufe rotate z*(360/16)*2 translate z*8*13 }
  object { stufe rotate z*(360/16)*4 translate z*8*13 }
  object { scheibe8 rotate z* 0.00 translate z*8*14 }
  object { stufe rotate z*(360/16)*3 translate z*8*14 }
  object { scheibe8 rotate z*11.25 translate z*8*15 }
  object { stufe rotate z*(360/16)*5 translate z*8*15 }
  object { stufe rotate z*(360/16)*6 translate z*8*15 }
  object { stufe rotate z*(360/16)*10 translate z*8*15 }
  object { stufe rotate z*(360/16)*11 translate z*8*15 }
  object { stufe rotate z*(360/16)*12 translate z*8*15 }
  object { stufe rotate z*(360/16)*13 translate z*8*15 }
  difference {
    object { scheibe8 rotate z* 0.00 translate z*8*16 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*5 translate z*8*16 texture { moertel } }
  }
  difference {
    object { scheibe8 rotate z*11.25 translate z*8*17 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*5 translate z*8*17 texture { moertel } }
  }
  difference {
    object { scheibe8 rotate z* 0.00 translate z*8*18 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*5 translate z*8*18 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*0 translate z*8*18 }
  object { stufe rotate z*(360/16)*1 translate z*8*18 }
  object { stufe rotate z*(360/16)*8 translate z*8*18 }
  object { stufe rotate z*(360/16)*9 translate z*8*18 }
  difference {
    object { scheibe8 rotate z*11.25 translate z*8*19 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*0 translate z*8*19 texture { moertel } }
  }
  difference {
    object { stufe rotate z*(360/16)*2 translate z*8*19 }
    object { scheibe8 rotate z* 0.00 translate z*8*20 }
  }
  difference {
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*0 translate z*8*20 texture { moertel } }
    object { stufe rotate z*(360/16)*3 translate z*8*20 }
  }
  difference {
    object { scheibe8 rotate z*11.25 translate z*8*21 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*0 translate z*8*21 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*4 translate z*8*21 }
  object { stufe rotate z*(360/16)*7 translate z*8*21 }
  object { stufe rotate z*(360/16)*13 translate z*8*21 }
  object { scheibe8 rotate z* 0.00 translate z*8*22 }
  object { stufe rotate z*(360/16)*5 translate z*8*22 }
  object { stufe rotate z*(360/16)*9 translate z*8*22 }
  object { stufe rotate z*(360/16)*11 translate z*8*22 }
  object { eleva rotate z*(360/16)*13 translate z*8*22 }
  object { scheibe8 rotate z*11.25 translate z*8*23 }
  object { scheibe8 rotate z* 0.00 translate z*8*24 }
  object { scheibe8 rotate z*11.25 translate z*8*25 }
  object { scheibe8 rotate z* 0.00 translate z*8*26 }
  object { stufe rotate z*(360/16)*2 translate z*8*26 }
  object { stufe rotate z*(360/16)*3 translate z*8*26 }
  object { stufe rotate z*(360/16)*4 translate z*8*26 }
  object { stufe rotate z*(360/16)*8 translate z*8*26 }
  object { scheibe8 rotate z*11.25 translate z*8*27 }
  object { eleva rotate z*(360/16)*2 translate z*8*27 }
  object { eleva rotate z*(360/16)*4 translate z*8*27 }
  object { stufe rotate z*(360/16)*6 translate z*8*27 }
  object { stufe rotate z*(360/16)*10 translate z*8*27 }
  object { stufe rotate z*(360/16)*12 translate z*8*27 }
  object { scheibe8 rotate z* 0.00 translate z*8*28 }
  object { scheibe8 rotate z*11.25 translate z*8*29 }
  object { scheibe8 rotate z* 0.00 translate z*8*30 }
  object { scheibe8 rotate z*11.25 translate z*8*31 }
  object { stufe rotate z*(360/16)*0 translate z*8*31 }
  object { stufe rotate z*(360/16)*7 translate z*8*31 }
  object { stufe rotate z*(360/16)*15 translate z*8*31 }
  difference {
    object { scheibe8 rotate z* 0.00 translate z*8*32 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*7 translate z*8*32 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*3 translate z*8*32 }
  object { stufe rotate z*(360/16)*9 translate z*8*32 }
  object { stufe rotate z*(360/16)*11 translate z*8*32 }
  object { stufe rotate z*(360/16)*12 translate z*8*32 }
  difference {
    object { scheibe8 rotate z*11.25 translate z*8*33 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*33 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*7 translate z*8*33 texture { moertel } }
  }
  object { eleva rotate z*(360/16)*12 translate z*8*33 }
  difference {
    object { scheibe8 rotate z* 0.00 translate z*8*34 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*34 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*7 translate z*8*34 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*1 translate z*8*34 }
  object { stufe rotate z*(360/16)*12 translate z*8*34 }
  difference {
    object { scheibe8 rotate z*11.25 translate z*8*35 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*35 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*1 translate z*8*35 }
  object { stufe rotate z*(360/16)*12 translate z*8*35 }
  object { scheibe8 rotate z* 0.00 translate z*8*36 }
  object { stufe rotate z*(360/16)*12 translate z*8*36 }
  object { scheibe8 rotate z*11.25 translate z*8*37 }
  object { stufe rotate z*(360/16)*12 translate z*8*37 }
  object { scheibe8 rotate z* 0.00 translate z*8*38 }
  object { stufe rotate z*(360/16)*12 translate z*8*38 }
  object { scheibe8 rotate z*11.25 translate z*8*39 }
  object { stufe rotate z*(360/16)*13 translate z*8*39 }
  object { scheibe8 rotate z* 0.00 translate z*8*40 }
  object { stufe rotate z*(360/16)*0 translate z*8*40 }
  object { stufe rotate z*(360/16)*5 translate z*8*40 }
  object { stufe rotate z*(360/16)*6 translate z*8*40 }
  object { stufe rotate z*(360/16)*7 translate z*8*40 }
  object { stufe rotate z*(360/16)*8 translate z*8*40 }
  object { stufe rotate z*(360/16)*9 translate z*8*40 }
  object { stufe rotate z*(360/16)*10 translate z*8*40 }
  object { stufe rotate z*(360/16)*14 translate z*8*40 }
  difference {
    object { scheibe8 rotate z*11.25 translate z*8*41 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*0 translate z*8*41 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*15 translate z*8*41 }
  difference {
    object { scheibe8 rotate z* 0.00 translate z*8*42 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*0 translate z*8*42 texture { moertel } }
  }
  difference {
    object { scheibe8 rotate z*11.25 translate z*8*43 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*0 translate z*8*43 texture { moertel } }
  }
  object { scheibe8 rotate z* 0.00 translate z*8*44 }
  object { stufe rotate z*(360/16)*4 translate z*8*44 }
  object { stufe rotate z*(360/16)*5 translate z*8*44 }
  object { stufe rotate z*(360/16)*6 translate z*8*44 }
  object { stufe rotate z*(360/16)*7 translate z*8*44 }
  object { stufe rotate z*(360/16)*8 translate z*8*44 }
  object { scheibe8 rotate z*11.25 translate z*8*45 }
  object { scheibe8 rotate z* 0.00 translate z*8*46 }
  object { scheibe8 rotate z*11.25 translate z*8*47 }
  object { scheibe8 rotate z* 0.00 translate z*8*48 }
  object { scheibe8 rotate z*11.25 translate z*8*49 }
  object { stufe rotate z*(360/16)*3 translate z*8*49 }
  object { stufe rotate z*(360/16)*4 translate z*8*49 }
  object { stufe rotate z*(360/16)*5 translate z*8*49 }
  object { stufe rotate z*(360/16)*6 translate z*8*49 }
  object { stufe rotate z*(360/16)*7 translate z*8*49 }
  object { stufe rotate z*(360/16)*8 translate z*8*49 }
  object { stufe rotate z*(360/16)*9 translate z*8*49 }
  object { stufe rotate z*(360/16)*10 translate z*8*49 }
  object { stufe rotate z*(360/16)*11 translate z*8*49 }
  object { scheibe8 rotate z* 0.00 translate z*8*50 }
  object { eleva rotate z*(360/16)*3 translate z*8*50 }
  object { scheibe8 rotate z*11.25 translate z*8*51 }
  object { scheibe8 rotate z* 0.00 translate z*8*52 }
  object { scheibe8 rotate z*11.25 translate z*8*53 }
  object { scheibe8 rotate z* 0.00 translate z*8*54 }
  object { scheibe8 rotate z*11.25 translate z*8*55 }
  object { stufe rotate z*(360/16)*0 translate z*8*55 }
  object { stufe rotate z*(360/16)*1 translate z*8*55 }
  object { stufe rotate z*(360/16)*2 translate z*8*55 }
  object { stufe rotate z*(360/16)*4 translate z*8*55 }
  object { stufe rotate z*(360/16)*5 translate z*8*55 }
  object { stufe rotate z*(360/16)*6 translate z*8*55 }
  object { stufe rotate z*(360/16)*7 translate z*8*55 }
  object { stufe rotate z*(360/16)*8 translate z*8*55 }
  object { stufe rotate z*(360/16)*9 translate z*8*55 }
  object { stufe rotate z*(360/16)*10 translate z*8*55 }
  object { stufe rotate z*(360/16)*11 translate z*8*55 }
  object { stufe rotate z*(360/16)*13 translate z*8*55 }
  object { stufe rotate z*(360/16)*14 translate z*8*55 }
  object { stufe rotate z*(360/16)*15 translate z*8*55 }
  object { scheibe8 rotate z* 0.00 translate z*8*56 }
  object { eleva rotate z*(360/16)*7 translate z*8*56 }
  object { scheibe8 rotate z*11.25 translate z*8*57 }
  object { stufe rotate z*(360/16)*6 translate z*8*57 }
  object { scheibe8 rotate z* 0.00 translate z*8*58 }
  object { stufe rotate z*(360/16)*5 translate z*8*58 }
  object { stufe rotate z*(360/16)*8 translate z*8*58 }
  object { scheibe8 rotate z*11.25 translate z*8*59 }
  object { stufe rotate z*(360/16)*4 translate z*8*59 }
  object { stufe rotate z*(360/16)*9 translate z*8*59 }
  object { scheibe8 rotate z* 0.00 translate z*8*60 }
  object { scheibe8 rotate z*11.25 translate z*8*61 }
  object { scheibe8 rotate z* 0.00 translate z*8*62 }
  object { scheibe8 rotate z*11.25 translate z*8*63 }
  object { scheibe8 rotate z* 0.00 translate z*8*64 }
  object { scheibe8 rotate z*11.25 translate z*8*65 }
  object { stufe rotate z*(360/16)*4 translate z*8*65 }
  object { stufe rotate z*(360/16)*5 translate z*8*65 }
  object { stufe rotate z*(360/16)*6 translate z*8*65 }
  object { scheibe8 rotate z* 0.00 translate z*8*66 }
  object { scheibe8 rotate z*11.25 translate z*8*67 }
  object { scheibe8 rotate z* 0.00 translate z*8*68 }
  object { scheibe8 rotate z*11.25 translate z*8*69 }
  object { scheibe8 rotate z* 0.00 translate z*8*70 }
  object { scheibe8 rotate z*11.25 translate z*8*71 }
  object { scheibe8 rotate z* 0.00 translate z*8*72 }
  object { stufe rotate z*(360/16)*6 translate z*8*72 }
  object { scheibe8 rotate z*11.25 translate z*8*73 }
  object { stufe rotate z*(360/16)*5 translate z*8*73 }
  object { stufe rotate z*(360/16)*13 translate z*8*73 }
  object { scheibe8 rotate z* 0.00 translate z*8*74 }
  object { stufe rotate z*(360/16)*4 translate z*8*74 }
  object { eleva rotate z*(360/16)*13 translate z*8*74 }
  object { stufe rotate z*(360/16)*15 translate z*8*74 }
  object { scheibe8 rotate z*11.25 translate z*8*75 }
  object { stufe rotate z*(360/16)*3 translate z*8*75 }
  object { stufe rotate z*(360/16)*11 translate z*8*75 }
  difference {
    object { scheibe8 rotate z* 0.00 translate z*8*76 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*76 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*10 translate z*8*76 }
  difference {
    object { scheibe8 rotate z*11.25 translate z*8*77 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*77 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*9 translate z*8*77 }
  difference {
    object { scheibe8 rotate z* 0.00 translate z*8*78 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*78 texture { moertel } }
  }
  object { stufe rotate z*(360/16)*0 translate z*8*78 }
  object { stufe rotate z*(360/16)*2 translate z*8*78 }
  object { stufe rotate z*(360/16)*8 translate z*8*78 }
  object { stufe rotate z*(360/16)*14 translate z*8*78 }
  object { scheibe8 rotate z*11.25 translate z*8*79 }
  object { stufe rotate z*(360/16)*7 translate z*8*79 }
  object { scheibe8 rotate z* 0.00 translate z*8*80 }
  object { stufe rotate z*(360/16)*6 translate z*8*80 }
  object { scheibe8 rotate z*11.25 translate z*8*81 }
  object { stufe rotate z*(360/16)*5 translate z*8*81 }
  object { scheibe8 rotate z* 0.00 translate z*8*82 }
  object { stufe rotate z*(360/16)*1 translate z*8*82 }
  object { stufe rotate z*(360/16)*4 translate z*8*82 }
  object { scheibe8 rotate z*11.25 translate z*8*83 }
  object { stufe rotate z*(360/16)*3 translate z*8*83 }
  object { scheibe8 rotate z* 0.00 translate z*8*84 }
  object { scheibe8 rotate z*11.25 translate z*8*85 }
  object { scheibe8 rotate z* 0.00 translate z*8*86 }
  object { scheibe8 rotate z*11.25 translate z*8*87 }
  object { scheibe8 rotate z* 0.00 translate z*8*88 }
  object { scheibe8 rotate z*11.25 translate z*8*89 }
  object { scheibe8 rotate z* 0.00 translate z*8*90 }
  object { scheibe8 rotate z*11.25 translate z*8*91 }
  object { stufe rotate z*(360/16)*9 translate z*8*91 }
  object { stufe rotate z*(360/16)*10 translate z*8*91 }
  object { stufe rotate z*(360/16)*11 translate z*8*91 }
  object { stufe rotate z*(360/16)*12 translate z*8*91 }
  difference {
    object { scheibe8 rotate z* 0.00 translate z*8*92 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*92 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*2 translate z*8*92 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*92 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*92 texture { moertel } }
  }
  difference {
    object { scheibe8 rotate z*11.25 translate z*8*93 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*93 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*2 translate z*8*93 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*93 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*93 texture { moertel } }
  }
  difference {
    object { scheibe8 rotate z* 0.00 translate z*8*94 }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*1 translate z*8*94 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*2 translate z*8*94 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*3 translate z*8*94 texture { moertel } }
    box { <-50,-10,0>, <50,10,8.001> rotate z*(360/16)*4 translate z*8*94 texture { moertel } }
  }
  object { scheibe8 rotate z*11.25 translate z*8*95 }
  object { zinnen8 translate z*768 }
}

#declare turm8 = object {
  object { turmot8 }
  bounded_by { cylinder { <0, 0, 0> <0, 0, 800> 80 } }
}
