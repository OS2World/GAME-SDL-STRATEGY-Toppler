
/* Trap of tricks */

#declare farb = color rgb <170/255,170/255,170/255>;

#declare turm = union {
  layer()
  layer()
  step(0)
  step(3)
  step(4)
  step(9)
  step(10)
  step(11)
  step(12)
  step(13)
  step(14)
  step(15)
  difference {
    layer()
    door(4)
    door(12)
  }
  elev(3)
  elev(9)
  difference {
    layer()
    door(4)
    door(12)
  }
  difference {
    layer()
    door(4)
    door(12)
  }
  layer()
  layer()
  step(4)
  step(5)
  step(6)
  step(7)
  step(8)
  step(9)
  step(12)
  step(13)
  step(14)
  layer()
  elev(12)
  layer()
  layer()
  layer()
  layer()
  step(2)
  step(4)
  step(5)
  step(6)
  step(7)
  step(8)
  difference {
    layer()
    door(6)
    door(14)
  }
  pillar(5)
  difference {
    layer()
    door(6)
    door(14)
  }
  pillar(5)
  difference {
    layer()
    door(6)
    door(14)
  }
  pillar(5)
  layer()
  step(5)
  step(6)
  step(7)
  step(8)
  step(9)
  layer()
  elev(5)
  layer()
  step(10)
  layer()
  layer()
  step(11)
  layer()
  step(3)
  step(4)
  layer()
  step(2)
  layer()
  step(1)
  step(8)
  step(9)
  layer()
  step(0)
  step(7)
  elev(9)
  layer()
  step(6)
  step(15)
  layer()
  step(14)
  layer()
  step(13)
  layer()
  step(10)
  step(11)
  step(12)
  layer()
  layer()
  layer()
  layer()
  layer()
  layer()
  layer()
  step(3)
  step(4)
  step(12)
  step(13)
  difference {
    layer()
    door(4)
    door(12)
  }
  elev(3)
  step(14)
  step(15)
  difference {
    layer()
    door(4)
    door(12)
  }
  difference {
    layer()
    door(4)
    door(12)
  }
  layer()
  step(7)
  step(8)
  difference {
    layer()
    door(7)
    door(15)
  }
  difference {
    layer()
    door(7)
    door(15)
  }
  difference {
    layer()
    door(7)
    door(15)
  }
  layer()
  layer()
  layer()
  step(4)
  step(11)
  step(12)
  step(13)
  difference {
    layer()
    door(4)
  }
  difference {
    layer()
    door(4)
  }
  difference {
    layer()
    door(4)
  }
  layer()
  layer()
}
