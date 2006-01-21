
#GCC = g++
GCC = g++ -W -Wimplicit -Wreturn-type -Wunused -Wformat -Wswitch -Wshadow -Wcast-qual -Wwrite-strings -DGCC_WARN
CFLAGS = -lz -lSDL -lSDL_image -lm -lpng -I/usr/include/SDL -I/usr/include/libpng

POVRAY = povray
GIMP = gimp-2.2


TARGETDIR = ../toppler

TOPPLERDAT = toppler.dat

DATFILES = cross.dat font.dat graphics.dat menu.dat scroller.dat \
	   sprites.dat titles.dat dude.dat \
       sounds/alarm.wav sounds/boing.wav sounds/bubbles.wav sounds/fanfare.wav \
       sounds/hit.wav sounds/honk.wav sounds/rumble.wav sounds/score.wav \
       sounds/sonar.wav sounds/splash.wav sounds/swoosh.wav sounds/tap.wav \
       sounds/tick.wav sounds/torpedo.wav sounds/water.wav

CLEANRULES = cross.clean font.clean graphics.clean menu.clean \
	     scroller.clean sprites.clean titles.clean dude.clean

#---------------------------------------------------------------------------------------#
# global make, this creates the final data file packing all things togethers using zlib #
#---------------------------------------------------------------------------------------#
$(TOPPLERDAT): crearc $(DATFILES)
	./crearc $(TOPPLERDAT) $(DATFILES)
	cp $(TOPPLERDAT) $(TARGETDIR)

crearc: crearc.c
	$(GCC) $(CFLAGS) crearc.c -o crearc

clean: $(CLEANRULES)
	rm -f $(TOPPLERDAT) crearc colorreduction assembler

#-------------------------------------------------------#
# rules to create the data files necesary for the cross #
#-------------------------------------------------------#
cross.dat: cross cross_colors.png cross_mask.png
	./cross

cross: cross.c
	$(GCC) $(CFLAGS) cross.c -o cross

cross_colors.png: colorreduction cross_colors_rgb.png
	./colorreduction cross_colors_rgb.png 256 cross_colors.png

cross_mask.png: colorreduction cross_mask_rgb.png
	./colorreduction cross_mask_rgb.png 256 cross_mask.png

cross_colors_rgb.png: assembler cross_pov/cross239.png
	./assembler vm cross_rgb cross_pov/*.png
	mv cross_rgb_colors.png cross_colors_rgb.png
	mv cross_rgb_mask.png cross_mask_rgb.png

cross_pov/cross239.png: cross_pov/cross.pov cross_pov/cross.ini
	( cd cross_pov && $(POVRAY) cross.ini )

cross.clean:
	rm -f cross.dat cross cross_colors_rgb.png cross_colors.png cross_mask_rgb.png cross_mask.png
	rm -f cross_pov/*.png

#------------------------------------------------------#
# rules to create the data files necesary for the font #
#------------------------------------------------------#
font.dat: font font.xcf
	$(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"font.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 0) \"font_mask_rgb.png\" \"ttt\")(gimp-quit 1))"
	$(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"font.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 1) \"font_colors_rgb.png\" \"ttt\")(gimp-quit 1))"
	./colorreduction font_colors_rgb.png 256 font_colors.png
	./colorreduction font_mask_rgb.png 256 font_mask.png
	./font
	rm font*.png

font: font.c
	$(GCC) $(CFLAGS) font.c -o font

font.clean:
	rm -f font.dat font

#----------------------------------------------------------#
# rules to create the data files necesary for the graphics #
#----------------------------------------------------------#
graphics.dat: graphics graphics_brick.png graphics_pinacle.png
	./graphics

graphics: graphics.c colorreduction.h pngsaver.h
	$(GCC) $(CFLAGS) graphics.c -o graphics

graphics_brick.png: graphics_brick.xcf
	$(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"graphics_brick.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 0) \"graphics_brick.png\" \"ttt\")(gimp-quit 1))"

graphics_pinacle.png: graphics_pinacle.xcf
	$(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"graphics_pinacle.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 0) \"graphics_pinacle.png\" \"ttt\")(gimp-quit 1))"

graphics.clean:
	rm -f graphics.dat graphics graphics_brick.png graphics_pinacle.png

#-------------------------------------------------------#
# rules to create the data files necesary for the mennu #
#-------------------------------------------------------#
menu.dat: menu menu.png
	./menu

menu: menu.c
	$(GCC) $(CFLAGS) menu.c -o menu

menu.png: menu_rgb.png colorreduction
	./colorreduction menu_rgb.png 256 menu.png

menu_rgb.png:
	( cd menu_pov && make all && mv menu_rgb.png .. )

menu.clean:
	( cd menu_pov && make clean )
	rm -f menu.dat menu menu_rgb.png menu.png

#----------------------------------------------------------#
# rules to create the data files necesary for the scroller #
#----------------------------------------------------------#
scroller.dat: scroller scroller.xcf
	$(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"scroller.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 0) \"scroller1_colors_rgb.png\" \"ttt\")(gimp-quit 1))"
	$(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"scroller.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 1) \"scroller2_colors_rgb.png\" \"ttt\")(gimp-quit 1))"
	$(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"scroller.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 2) \"scroller3_colors_rgb.png\" \"ttt\")(gimp-quit 1))"
	$(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"scroller.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 3) \"scroller1_mask_rgb.png\" \"ttt\")(gimp-quit 1))"
	$(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"scroller.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 4) \"scroller2_mask_rgb.png\" \"ttt\")(gimp-quit 1))"
	$(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"scroller.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 5) \"scroller3_mask_rgb.png\" \"ttt\")(gimp-quit 1))"
	./colorreduction scroller3_mask_rgb.png 256 scroller3_mask.png
	./colorreduction scroller2_mask_rgb.png 256 scroller2_mask.png
	./colorreduction scroller1_mask_rgb.png 256 scroller1_mask.png
	./colorreduction scroller3_colors_rgb.png 256 scroller3_colors.png
	./colorreduction scroller2_colors_rgb.png 256 scroller2_colors.png
	./colorreduction scroller1_colors_rgb.png 256 scroller1_colors.png
	./scroller 3 2 1/1 scroller3_colors.png scroller3_mask.png 0/0/640 1/2 \
                       scroller2_colors.png scroller2_mask.png 0/0/640 1/1 \
                       scroller1_colors.png scroller1_mask.png 0/0/640 2/1
	rm -f scroller1_colors.png scroller1_mask.png
	rm -f scroller2_colors.png scroller2_mask.png
	rm -f scroller3_colors.png scroller3_mask.png
	rm -f scroller1_colors_rgb.png scroller1_mask_rgb.png
	rm -f scroller2_colors_rgb.png scroller2_mask_rgb.png
	rm -f scroller3_colors_rgb.png scroller3_mask_rgb.png

scroller: scroller.c
	$(GCC) $(CFLAGS) scroller.c -o scroller

scroller.clean:
	rm -f scroller scroller.dat

#---------------------------------------------------------#
# rules to create the data files necesary for the sprites #
#---------------------------------------------------------#
sprites.dat: sprites sprites_robots_colors.png sprites_robots_mask.png \
             sprites_balls_colors.png sprites_balls_mask.png \
             sprites_box_colors.png sprites_box_mask.png \
             sprites_snowball_colors.png sprites_snowball_mask.png \
             sprites_torpedo_colors.png sprites_torpedo_mask.png
	./sprites

sprites: sprites.c
	$(GCC) $(CFLAGS) sprites.c -o sprites

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
	( cd sprites_pov/box && $(POVRAY) obj.ini )

sprites_pov/balls/obj0.png: sprites_pov/balls/obj.pov sprites_pov/balls/obj.ini
	( cd sprites_pov/balls && $(POVRAY) obj.ini )

sprites_pov/snowball/obj0.png: sprites_pov/snowball/obj.pov sprites_pov/snowball/obj.ini
	( cd sprites_pov/snowball && $(POVRAY) obj.ini )

sprites_robots_colors_rgb.png: assembler \
                               sprites_pov/robot0_rgb_colors.png \
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

sprites_pov/robot0/obj00.png: sprites_pov/robot0/obj.pov sprites_pov/robot0/obj.ini sprites_pov/environment.pov
	( cd sprites_pov/robot0 && $(POVRAY) obj.ini )

sprites_pov/robot1_rgb_colors.png: sprites_pov/robot1/obj00.png assembler
	./assembler hm sprites_pov/robot1_rgb sprites_pov/robot1/*.png

sprites_pov/robot1/obj00.png: sprites_pov/robot1/obj.pov sprites_pov/robot1/obj.ini sprites_pov/environment.pov
	( cd sprites_pov/robot1 && $(POVRAY) obj.ini )

sprites_pov/robot2_rgb_colors.png: sprites_pov/robot2/obj00.png assembler
	./assembler hm sprites_pov/robot2_rgb sprites_pov/robot2/*.png

sprites_pov/robot2/obj00.png: sprites_pov/robot2/obj.pov sprites_pov/robot2/obj.ini sprites_pov/environment.pov
	( cd sprites_pov/robot2 && $(POVRAY) obj.ini )

sprites_pov/robot3_rgb_colors.png: sprites_pov/robot3/obj00.png assembler
	./assembler hm sprites_pov/robot3_rgb sprites_pov/robot3/*.png

sprites_pov/robot3/obj00.png: sprites_pov/robot3/obj.pov sprites_pov/robot3/obj.ini sprites_pov/environment.pov
	( cd sprites_pov/robot3 && $(POVRAY) obj.ini )

sprites_pov/robot4_rgb_colors.png: sprites_pov/robot4/obj00.png assembler
	./assembler hm sprites_pov/robot4_rgb sprites_pov/robot4/*.png

sprites_pov/robot4/obj00.png: sprites_pov/robot4/obj.pov sprites_pov/robot4/obj.ini sprites_pov/environment.pov
	( cd sprites_pov/robot4 && $(POVRAY) obj.ini )

sprites_pov/robot5_rgb_colors.png: sprites_pov/robot5/obj00.png assembler
	./assembler hm sprites_pov/robot5_rgb sprites_pov/robot5/*.png

sprites_pov/robot5/obj00.png: sprites_pov/robot5/obj.pov sprites_pov/robot5/obj.ini sprites_pov/environment.pov
	( cd sprites_pov/robot5 && $(POVRAY) obj.ini )

sprites_pov/robot6_rgb_colors.png: sprites_pov/robot6/obj00.png assembler
	./assembler hm sprites_pov/robot6_rgb sprites_pov/robot6/*.png

sprites_pov/robot6/obj00.png: sprites_pov/robot6/obj.pov sprites_pov/robot6/obj.ini sprites_pov/environment.pov
	( cd sprites_pov/robot6 && $(POVRAY) obj.ini )

sprites_pov/robot7_rgb_colors.png: sprites_pov/robot7/obj00.png assembler
	./assembler hm sprites_pov/robot7_rgb sprites_pov/robot7/*.png

sprites_pov/robot7/obj00.png: sprites_pov/robot7/obj.pov sprites_pov/robot7/obj.ini sprites_pov/environment.pov
	( cd sprites_pov/robot7 && $(POVRAY) obj.ini )

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
titles.dat: titles titles.xcf
	$(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"titles.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 0) \"titles_mask_rgb.png\" \"ttt\")(gimp-quit 1))"
	$(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"titles.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 1) \"titles_colors_rgb.png\" \"ttt\")(gimp-quit 1))"
	./colorreduction titles_colors_rgb.png 256 titles_colors.png
	./colorreduction titles_mask_rgb.png 256 titles_mask.png
	./titles
	rm titles*.png

titles: titles.c
	$(GCC) $(CFLAGS) titles.c -o titles

titles.clean:
	rm -f titles.dat titles 

#-------------------------------------------------------#
# rules to create the data files necesary for the dude  #
#-------------------------------------------------------#
dude.dat: dude dude.xcf
	$(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"dude.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 1) \"dude_colors_rgb.png\" \"ttt\")(gimp-quit 1))"
	$(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"dude.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 0) \"dude_mask_rgb.png\" \"ttt\")(gimp-quit 1))"
	./colorreduction dude_colors_rgb.png 256 dude_colors.png
	./colorreduction dude_mask_rgb.png 256 dude_mask.png
	./dude
	rm dude*.png

dude: dude.c
	$(GCC) $(CFLAGS) dude.c -o dude

dude.clean:
	rm -f dude.dat dude

#-------------------------------------------------------#
# rules to create the tool programs                     #
#-------------------------------------------------------#
colorreduction: colorreduction.c colorreduction.h pngsaver.h
	$(GCC) $(CFLAGS) colorreduction.c -o colorreduction

assembler: assembler.c
	$(GCC) $(CFLAGS) assembler.c -o assembler
