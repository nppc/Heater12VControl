$fn=50;

Box();

translate([-7.5,-33,-5])case_mountinghole();
translate([-24,-8,-5])rotate([0,0,-90])case_mountinghole();
translate([0,33,-5])rotate([0,0,180])case_mountinghole();

translate([-15,-33,10.5])rotate([0,0,90])linear_extrude(height = 0.5){text("REFLOW", $fn = 20, size = 4.5, font = "ArialBlack");}

//cylinder(d=4, h=6);

module Box(){
difference(){
    union(){
        difference(){
            union(){
                generalBox(21.5,72.5,21,4);
                translate([-17,-18.5,0])generalBox(20.5,35.5,21,4);
            }
            translate([0,0,-0.7])generalBox(19,70,20,2.6);
            translate([-17,-18,-0.7])generalBox(18,32,20,2.6);

            // cutout for PowerJack
            translate([-17,-11,0.5])rotate([90,0,0])cylinder(d=11,h=30);

            // cutout for encoder pin
            translate([0.4,-23,9]){
                cylinder(d=7.2,h=20);
                translate([6,0,0])cube([2,4,3.5], center=true);
            }

            // Cutout for USB
            translate([1,-35,-4])cube([11.5,10,9], center=true);

            // cutout for cables
            translate([0.4,35,4])cube([14,10,6], center=true);
            //cube([50,50,50]);

            // cutout for text
            translate([-17,-18.5,11.5])cube([10,30,2], center=true);
        }
        // frame for OLED
        translate([0.4,-23+15+25/2+1,9.4])cube([10,27,1], center=true);
        
        // PCB holders
        translate([9,20,9.2-12]){
            rotate([90,0,0])cylinder(d=1.5,h=8,center=true);
            translate([0,-4,-0.75])cube([1,8,5]);
        }
        translate([-9,20,9.2-12]){
            rotate([90,0,0])cylinder(d=1.5,h=8,center=true);
            translate([-1,-4,-0.75])cube([1,8,5]);
        }
   }
    // cutout for OLED
    translate([0.4,-23+15+25/2+1,9]){
        
        cube([8,25,5], center=true);
        hull(){
            translate([0,0,3])cube([11,28,0.1], center=true);
            translate([0,0,2])cube([8,25,3], center=true);
        }
    }
}
}



module generalBox(sx,sy,sz,diam){
    hull(){
        translate([(sx-diam)/2,(sy-diam)/2,0])generalBoxCylinder(sz,diam);
        translate([(-sx+diam)/2,(sy-diam)/2,0])generalBoxCylinder(sz,diam);
        translate([(sx-diam)/2,-(sy-diam)/2,0])generalBoxCylinder(sz,diam);
        translate([-(sx-diam)/2,-(sy-diam)/2,0])generalBoxCylinder(sz,diam);
    }
}

module generalBoxCylinder(hght,diam){
    cylinder(d=diam, h=hght-1,center=true);
    translate([0,0,(hght-1)/2])cylinder(d1=diam,d2=diam-1.2, h=1);
}


module case_mountinghole(){
    rotate([0,180,0])difference(){
        hull(){
            cylinder(d=4, h=10, center=true);
            translate([0,-2,0])cube([4,2,10], center=true);
        }
        translate([0,0,3])cylinder(d=1.8, h=9, center=true);
        translate([0,2,-8])rotate([45,0,0])cube([15,15,10], center=true);
    }
}

