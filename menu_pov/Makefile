
TOPPLERDIR = ../../toppler

LEVELDIR = ../../levels/mission1
LEVELS = m1t1 m1t2 m1t3 m1t4 m1t5 m1t6 m1t7 m1t8

#LEVELDIR = ../../levels/mission2
#LEVELS = m2t1 m2t2 m2t3 m2t4 m2t5 m2t6 m2t7 m2t8

#LEVELDIR = ../../levels/kallinen1
#LEVELS = t1 t2 t3 t4 t5 t6 t7 t8

#LEVELDIR = ../../levels/ball1
#LEVELS = lev1 lev2 lev3 lev4 lev5 lev6 lev7 lev8



OBJS = $(TOPPLERDIR)/level.o $(TOPPLERDIR)/decl.o \
	$(TOPPLERDIR)/keyb.o $(TOPPLERDIR)/points.o

all: tower2inc
	( ln -s `pwd`/$(LEVELDIR) $(HOME)/.toppler/t && \
	./tower2inc "turm%i.inc" t/ $(LEVELS) && \
	rm $(HOME)/.toppler/t && \
	povray menu.ini )

tower2inc: tower2inc.cc
	g++ tower2inc.cc -I$(TOPPLERDIR) -I/usr/include/SDL $(OBJS) -lSDL -o tower2inc

clean:
	rm -f tower2inc turm*.inc menu_rgb.png
