#---------------------------------------------------------------------------------------#
# global make, this creates the final data file packing all things togethers using zlib #
#---------------------------------------------------------------------------------------#
toppler.dat: crearc cross.dat font.dat graphics.dat menu.dat scroller.dat sprites.dat titles.dat dude.dat
	./crearc dude cross font graphics sprites titles menu scroller
	cp toppler.dat ../toppler_highres

crearc: crearc.c
	gcc crearc.c -o crearc -lz

#---------------------------------------------------------------------------------------#
# global make, this creates the final data file packing all things togethers using zlib #
#---------------------------------------------------------------------------------------#
clean: cross.clean font.clean graphics.clean menu.clean scroller.clean sprites.clean titles.clean dude.clean
	rm -f toppler.dat crearc colorreduction assembler

#-------------------------------------------------------#
# rules to create the data files necesary for the cross #
#-------------------------------------------------------#
cross.dat: cross cross_colors.png cross_mask.png
	./cross

cross: cross.c
	gcc cross.c -o cross -lSDL -lSDL_image -I/usr/include/SDL

cross_colors.png: colorreduction cross_colors_rgb.png
	./colorreduction cross_colors_rgb.png 256 cross_colors.png

cross_mask.png: colorreduction cross_mask_rgb.png
	./colorreduction cross_mask_rgb.png 256 cross_mask.png

cross_colors_rgb.png: assembler cross_pov/cross000.png
	./assembler vm cross_rgb cross_pov/*.png
	mv cross_rgb_colors.png cross_colors_rgb.png
	mv cross_rgb_mask.png cross_mask_rgb.png

cross_pov/cross000.png: cross_pov/cross.pov cross_pov/cross.ini
	sh -c "cd cross_pov; povray cross.ini"

cross.clean:
	rm -f cross.dat cross cross_colors_rgb.png cross_colors.png cross_mask_rgb.png cross_mask.png
	rm -f cross_pov/*.png

#------------------------------------------------------#
# rules to create the data files necesary for the font #
#------------------------------------------------------#
font.dat: font font_colors.png font_mask.png
	./font

font: font.c
	gcc font.c -o font -lSDL -lSDL_image -I/usr/include/SDL

font_colors.png: font_colors_rgb.png colorreduction
	./colorreduction font_colors_rgb.png 256 font_colors.png

font_mask.png: font_mask_rgb.png colorreduction
	./colorreduction font_mask_rgb.png 256 font_mask.png

font.clean:
	rm -f font.dat font_colors.png font_mask.png font

#----------------------------------------------------------#
# rules to create the data files necesary for the graphics #
#----------------------------------------------------------#
graphics.dat: graphics graphics_brick.png graphics_pinacle.png
	./graphics

graphics: graphics.c colorreduction.h pngsaver.h
	gcc graphics.c -o graphics -lSDL -lSDL_image -I/usr/include/SDL

graphics.clean:
	rm -f graphics.dat graphics

#-------------------------------------------------------#
# rules to create the data files necesary for the mennu #
#-------------------------------------------------------#
menu.dat: menu menu.png
	./menu

menu: menu.c
	gcc menu.c -o menu -lSDL -lSDL_image -I/usr/include/SDL

#menu.png: menu_rgb.png colorreduction
#	./colorreduction menu_rgb.png 256 menu.png

#menu_rgb.png: menu_pov/menu.pov menu_pov/turm1.inc menu_pov/turm2.inc \
#              menu_pov/turm3.inc menu_pov/turm4.inc \
#              menu_pov/turm5.inc menu_pov/turm6.inc \
#              menu_pov/turm7.inc menu_pov/turm8.inc
#	cd menu_pov
#	povray menu.ini
#	mv menu_rgb.png ..

menu.clean:
	rm -f menu.dat menu

#----------------------------------------------------------#
# rules to create the data files necesary for the scroller #
#----------------------------------------------------------#
scroller.dat: scroller scroller1.png scroller2.png scroller3.png
	./scroller 3 2 1/1 scroller1.png 1/2 scroller2.png 1/1 scroller3.png 2/1

scroller: scroller.c
	gcc scroller.c -o scroller -lSDL -lSDL_image -I/usr/include/SDL

scroller.clean:
	rm -f scroller scroller.dat

#---------------------------------------------------------#
# rules to create the data files necesary for the sprites #
#---------------------------------------------------------#
sprites.dat: sprites sprites_robots_colors.png sprites_robots_mask.png \
             sprites_balls_colors.png sprites_balls_mask.png \
             sprites_box_colors.png sprites_box_mask.png \
             sprites_snowball_colors.png sprites_snowball_mask.png \
             sprites_star.png sprites_bonus.png
	./sprites

sprites: sprites.c
	gcc sprites.c -o sprites -lSDL -lSDL_image -I/usr/include/SDL

sprites_robots_colors.png: colorreduction sprites_robots_colors_rgb.png
	./colorreduction sprites_robots_colors_rgb.png 256 sprites_robots_colors.png

sprites_robots_mask.png: colorreduction sprites_robots_mask_rgb.png
	./colorreduction sprites_robots_mask_rgb.png 256 sprites_robots_mask.png

sprites_balls_colors.png: colorreduction sprites_balls_rgb_colors.png
	./colorreduction sprites_balls_rgb_colors.png 256 sprites_balls_colors.png

sprites_balls_mask.png: colorreduction sprites_balls_rgb_mask.png
	./colorreduction sprites_balls_rgb_mask.png 256 sprites_balls_mask.png

sprites_box_colors.png: colorreduction sprites_box_rgb_colors.png
	./colorreduction sprites_box_rgb_colors.png 256 sprites_box_colors.png

sprites_box_mask.png: colorreduction sprites_box_rgb_mask.png
	./colorreduction sprites_box_rgb_mask.png 256 sprites_box_mask.png

sprites_snowball_colors.png: colorreduction sprites_pov/snowball/obj0.png
	./colorreduction sprites_pov/snowball/obj0.png 256 sprites_snowball_colors.png

sprites_snowball_mask.png: colorreduction sprites_pov/snowball/obj0.png
	./colorreduction sprites_pov/snowball/obj1.png 256 sprites_snowball_mask.png

sprites_box_rgb_colors.png: assembler sprites_pov/box/obj00.png
	./assembler hm sprites_box_rgb sprites_pov/box/obj*.png

sprites_balls_rgb_colors.png: assembler sprites_pov/balls/obj0.png
	./assembler hm sprites_balls_rgb sprites_pov/balls/obj*.png

sprites_pov/box/obj00.png: sprites_pov/box/obj.pov sprites_pov/box/obj.ini
	sh -c "cd sprites_pov/box; povray obj.ini"

sprites_pov/balls/obj0.png: sprites_pov/balls/obj.pov sprites_pov/balls/obj.ini
	sh -c "cd sprites_pov/balls; povray obj.ini"

sprites_pov/snowball/obj0.png: sprites_pov/snowball/obj.pov sprites_pov/snowball/obj.ini
	sh -c "cd sprites_pov/snowball; povray obj.ini"

sprites_robots_colors_rgb.png: assembler sprites_pov/robot0_rgb_colors.png \
                               sprites_pov/robot1_rgb_colors.png \
                               sprites_pov/robot2_rgb_colors.png \
                               sprites_pov/robot3_rgb_colors.png \
                               sprites_pov/robot4_rgb_colors.png \
                               sprites_pov/robot5_rgb_colors.png \
                               sprites_pov/robot6_rgb_colors.png \
                               sprites_pov/robot7_rgb_colors.png
	./assembler v sprites_robots_rgb sprites_pov/robot*_rgb_colors.png sprites_pov/robot*_rgb_mask.png
	mv sprites_robots_rgb_colors.png sprites_robots_colors_rgb.png
	mv sprites_robots_rgb_mask.png sprites_robots_mask_rgb.png

sprites_pov/robot0_rgb_colors.png: sprites_pov/robot0/obj00.png assembler
	./assembler hm sprites_pov/robot0_rgb sprites_pov/robot0/*.png

sprites_pov/robot0/obj00.png: sprites_pov/robot0/obj.pov sprites_pov/robot0/obj.ini
	sh -c "cd sprites_pov/robot0; povray obj.ini"

sprites_pov/robot1_rgb_colors.png: sprites_pov/robot1/obj00.png assembler
	./assembler hm sprites_pov/robot1_rgb sprites_pov/robot1/*.png

sprites_pov/robot1/obj00.png: sprites_pov/robot1/obj.pov sprites_pov/robot1/obj.ini
	sh -c "cd sprites_pov/robot1; povray obj.ini"

sprites_pov/robot2_rgb_colors.png: sprites_pov/robot2/obj00.png assembler
	./assembler hm sprites_pov/robot2_rgb sprites_pov/robot2/*.png

sprites_pov/robot2/obj00.png: sprites_pov/robot2/obj.pov sprites_pov/robot2/obj.ini
	sh -c "cd sprites_pov/robot2; povray obj.ini"

sprites_pov/robot3_rgb_colors.png: sprites_pov/robot3/obj00.png assembler
	./assembler hm sprites_pov/robot3_rgb sprites_pov/robot3/*.png

sprites_pov/robot3/obj00.png: sprites_pov/robot3/obj.pov sprites_pov/robot3/obj.ini
	sh -c "cd sprites_pov/robot3; povray obj.ini"

sprites_pov/robot4_rgb_colors.png: sprites_pov/robot4/obj00.png assembler
	./assembler hm sprites_pov/robot4_rgb sprites_pov/robot4/*.png

sprites_pov/robot4/obj00.png: sprites_pov/robot4/obj.pov sprites_pov/robot4/obj.ini
	sh -c "cd sprites_pov/robot4; povray obj.ini"

sprites_pov/robot5_rgb_colors.png: sprites_pov/robot5/obj00.png assembler
	./assembler hm sprites_pov/robot5_rgb sprites_pov/robot5/*.png

sprites_pov/robot5/obj00.png: sprites_pov/robot5/obj.pov sprites_pov/robot5/obj.ini
	sh -c "cd sprites_pov/robot5; povray obj.ini"

sprites_pov/robot6_rgb_colors.png: sprites_pov/robot6/obj00.png assembler
	./assembler hm sprites_pov/robot6_rgb sprites_pov/robot6/*.png

sprites_pov/robot6/obj00.png: sprites_pov/robot6/obj.pov sprites_pov/robot6/obj.ini
	sh -c "cd sprites_pov/robot6; povray obj.ini"

sprites_pov/robot7_rgb_colors.png: sprites_pov/robot7/obj00.png assembler
	./assembler hm sprites_pov/robot7_rgb sprites_pov/robot7/*.png

sprites_pov/robot7/obj00.png: sprites_pov/robot0/obj.pov sprites_pov/robot7/obj.ini
	sh -c "cd sprites_pov/robot7; povray obj.ini"

sprites.clean:
	rm -f sprites sprites.dat
	rm -f sprites_robots_colors.png sprites_robots_colors_rgb.png sprites_robots_mask.png sprites_robots_mask_rgb.png
	rm -f sprites_pov/robot*_rgb_*.png
	rm -f sprites_pov/robot0/obj*.png
	rm -f sprites_pov/robot1/obj*.png
	rm -f sprites_pov/robot2/obj*.png
	rm -f sprites_pov/robot3/obj*.png
	rm -f sprites_pov/robot4/obj*.png
	rm -f sprites_pov/robot5/obj*.png
	rm -f sprites_pov/robot6/obj*.png
	rm -f sprites_pov/robot7/obj*.png
	rm -f sprites_balls_colors.png sprites_balls_rgb_colors.png sprites_balls_mask.png sprites_balls_rgb_mask.png
	rm -f sprites_pov/balls/obj*.png
	rm -f sprites_box_colors.png sprites_box_rgb_colors.png sprites_box_mask.png sprites_box_rgb_mask.png
	rm -f sprites_pov/box/obj*.png
	rm -f sprites_snowball_colors.png sprites_snowball_mask.png
	rm -f sprites_pov/snowball/obj*.png

#-------------------------------------------------------#
# rules to create the data files necesary for the title #
#-------------------------------------------------------#
titles.dat: titles titles_colors.png titles_mask.png
	./titles

titles: titles.c
	gcc titles.c -o titles -lSDL -lSDL_image -I/usr/include/SDL

titles_colors.png: titles_colors_rgb.png colorreduction
	./colorreduction titles_colors_rgb.png 256 titles_colors.png

titles_mask.png: titles_mask_rgb.png colorreduction
	./colorreduction titles_mask_rgb.png 256 titles_mask.png

titles.clean:
	rm -f titles.dat titles titles_colors.png titles_mask.png

#-------------------------------------------------------#
# rules to create the data files necesary for the dude  #
#-------------------------------------------------------#
dude.dat: dude dude_colors.png dude_mask.png
	./dude

dude: dude.c
	gcc dude.c -o dude -lSDL -lSDL_image -I/usr/include/SDL

dude_colors.png: dude_colors_rgb.png colorreduction
	./colorreduction dude_colors_rgb.png 256 dude_colors.png

dude_mask.png: dude_mask_rgb.png colorreduction
	./colorreduction dude_mask_rgb.png 256 dude_mask.png

dude.clean:
	rm -f dude.dat dude dude_mask.png dude_colors.png

#-------------------------------------------------------#
# rules to create the tool programs                     #
#-------------------------------------------------------#
colorreduction: colorreduction.c colorreduction.h pngsaver.h
	gcc colorreduction.c -o colorreduction -lSDL -lSDL_image -lpng -I/usr/include/SDL -I/usr/include/libpng

assembler: assembler.c
	gcc assembler.c -o assembler -lSDL -lSDL_image -lpng -I/usr/include/SDL -I/usr/include/libpng
