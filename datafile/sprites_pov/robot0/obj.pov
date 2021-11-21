#include "../environment.pov"

#local sphere_size = 15*(sin(rot*3)+0.5);
#local torus_thickness = 4;

union {
    union {
	sphere {
	    <0, 0, -sphere_size>, sphere_size
	    texture { T2 }
	}
	sphere {
	    <0, 0, sphere_size>, sphere_size
	    texture { T2 }
	}
	sphere {
	    <0, sphere_size, 0>, sphere_size
	    texture { T1 }
	}
	sphere {
	    <0, -sphere_size, 0>, sphere_size
	    texture { T1 }
	}
	rotate <0, 90*rot, 90*-rot>
    }
    union {
	torus {
	    (100-torus_thickness*2)/2, torus_thickness
	    texture { T3 }
	}
	torus {
	    (100-torus_thickness*2)/2, torus_thickness
	    texture { T3 }
	    rotate z*90
	}
	torus {
	    (100-torus_thickness*2)/2, torus_thickness
	    texture { T3 }
	    rotate x*90
	}
    }
    rotate <90*rot, 0, 90*rot>
    scale 0.06
}
