$fn=40;
difference(){
    cube([21,72,20], center=true);
    translate([0,0,-1])cube([19,70,20], center=true);
}

translate([-18,-16,0])rotate([90,0,0])cylinder(d=15,h=20);