TOPPLERDIR = ../toppler

all: m1.ttm m2.ttm ball1.ttm abc.ttm ball2.ttm ball3.ttm
	cp *.ttm ${TOPPLERDIR}

clean:
	rm -f *.ttm cremission

cremission: cremission.cc level.o
	g++ cremission.cc -I${TOPPLERDIR} -I/usr/include/SDL ${TOPPLERDIR}/keyb.o level.o \
    ${TOPPLERDIR}/points.o ${TOPPLERDIR}/decl.o ${TOPPLERDIR}/configuration.o \
    -lSDL -lSDL_mixer -lz -o cremission

level.o: ${TOPPLERDIR}/level.cc ${TOPPLERDIR}/level.h
	g++ ${TOPPLERDIR}/level.cc -I${TOPPLERDIR} -I/usr/include/SDL -D CREATOR -D TOP_DATADIR=\"./\" -c -o level.o

# RULE FOR MISSION 1

m1.ttm: mission1/m1t1 mission1/m1t2 mission1/m1t3 mission1/m1t4 \
        mission1/m1t5 mission1/m1t6 mission1/m1t7 mission1/m1t8 \
        cremission
	ln -s `pwd`/mission1 ${HOME}/.toppler/t
	./cremission "Mission 1" 10 t/m1t1 t/m1t2 t/m1t3 t/m1t4 t/m1t5 t/m1t6 t/m1t7 t/m1t8
	rm ${HOME}/.toppler/t
	mv "${HOME}/.toppler/Mission 1.ttm" m1.ttm


# RULE FOR MISSION 2

m2.ttm: mission2/m2t1 mission2/m2t2 mission2/m2t3 mission2/m2t4 \
        mission2/m2t5 mission2/m2t6 mission2/m2t7 mission2/m2t8 \
        cremission
	ln -s `pwd`/mission2 ${HOME}/.toppler/t
	./cremission "Mission 2" 11 t/m2t1 t/m2t2 t/m2t3 t/m2t4 t/m2t5 t/m2t6 t/m2t7 t/m2t8
	rm ${HOME}/.toppler/t
	mv "${HOME}/.toppler/Mission 2.ttm" m2.ttm

# RULE FOR CLARENCE MISSION 1

ball1.ttm: ball1/lev1 ball1/lev2 ball1/lev3 ball1/lev4 \
           ball1/lev5 ball1/lev6 ball1/lev7 ball1/lev8 \
           cremission
	ln -s `pwd`/ball1 ${HOME}/.toppler/t
	./cremission "Ball 1" 13 t/lev1 t/lev2 t/lev3 t/lev4 t/lev5 t/lev6 t/lev7 t/lev8
	rm ${HOME}/.toppler/t
	mv "${HOME}/.toppler/Ball 1.ttm" ball1.ttm

# RULE FOR CLARENCE MISSION 2

ball2.ttm: ball2/lev1 ball2/lev2 ball2/lev3 ball2/lev4 \
           ball2/lev5 ball2/lev6 ball2/lev7 ball2/lev8 \
           cremission
	ln -s `pwd`/ball2 ${HOME}/.toppler/t
	./cremission "Ball 2" 14 t/lev1 t/lev2 t/lev3 t/lev4 t/lev5 t/lev6 t/lev7 t/lev8
	rm ${HOME}/.toppler/t
	mv "${HOME}/.toppler/Ball 2.ttm" ball2.ttm

# RULE FOR CLARENCE MISSION 3

ball3.ttm: ball3/lev1 ball3/lev2 ball3/lev3 ball3/lev4 \
           ball3/lev5 ball3/lev6 ball3/lev7 ball3/lev8 \
           cremission
	ln -s `pwd`/ball3 ${HOME}/.toppler/t
	./cremission "Ball 3" 15 t/lev1 t/lev2 t/lev3 t/lev4 t/lev5 t/lev6 t/lev7 t/lev8
	rm ${HOME}/.toppler/t
	mv "${HOME}/.toppler/Ball 3.ttm" ball3.ttm

# RULE FOR PASIS MISSION 1

abc.ttm: kallinen1/t1 kallinen1/t2 kallinen1/t3 kallinen1/t4 \
         kallinen1/t5 kallinen1/t6 kallinen1/t7 kallinen1/t8 \
         cremission
	ln -s `pwd`/kallinen1 ${HOME}/.toppler/t
	./cremission "ABC Towers" 12 t/t1 t/t2 t/t3 t/t4 t/t5 t/t6 t/t7 t/t8
	rm ${HOME}/.toppler/t
	mv "${HOME}/.toppler/ABC Towers.ttm" abc.ttm


