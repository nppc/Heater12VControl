$fn=100;
difference(){
    union(){
        hull(){
            cube([21,72,20-1], center=true);
            translate([0,0,(20)/2])cube([21-2,72-2,1], center=true);
        }
        translate([0,-12.5,2+9.5])PowerJackHolderSub();
        }
    scale([0.9,0.97,0.9])hull(){
        translate([0,0,-1])cube([21,72,21], center=true);
        translate([0,0,(20)/2])cube([21-2,72-2,1], center=true);
        
    }
    translate([0-1.5,-12.5,1+9.5]){
        scale([0.9,0.9,0.9])PowerJackHolderSub();
    }

    translate([-17,-11,0.5])rotate([90,0,0])cylinder(d=11,h=30);
    translate([0,0,-15-9.5])cube([60,100,30],center=true);

    translate([0,-23,9]){
        cylinder(d=7.2,h=20);
        cube([14,14,2], center=true);
    }

    translate([0,-23+15+25/2,9]){
        cube([8,25,3], center=true);
    }

    translate([0,-35,-4]){
        cube([13,10,11], center=true);
    }

    translate([0,35,4]){
        cube([14,10,6], center=true);
    }
    //cube([50,50,50]);

    //translate([0,0,-1])cube([19,70,20], center=true);
}


module PowerJackHolderSub(){
    rotate([90,0,0])hull(){
        translate([-17,-11,-1.5]){
            translate([0,0,24])cylinder(d1=20, d2=18,h=1);
            sphere(d=20);
        }
        translate([-10,-11,-1.5]){
            translate([0,0,24])cylinder(d1=20, d2=18,h=1);
            sphere(d=20);
        }
        translate([-17,-25,-1.5]){
            translate([0,0,24])cylinder(d1=20, d2=18,h=1);
            sphere(d=20);
        }
        translate([-10,-25,-1.5]){
            translate([0,0,24])cylinder(d1=20, d2=18,h=1);
            sphere(d=20);
        }
    }
}