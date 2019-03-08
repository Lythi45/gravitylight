
hole=true;
radius=16.1;
thick=1.4;
width=1;

scale([10,10,10]) {
//a=0.02;
    a=0.01;
    {
    echo(a);
b=0; {
//translate([b*4,a*1000,0])
    difference(){
union(){ 
difference() {
rotate([0,0,-5]) rotate_extrude($fn=144,angle=100) translate([radius,0,0])polygon( points=[[0,0],[width,0],[width,thick],[0,thick]] );
rotate([0,0,-25+0.08*3.6]) rotate_extrude($fn=144,angle=30)  translate([radius-1,thick/2-0.03,0])polygon( points=[[0,0],[3,0],[3,1],[0,1]] ); 
rotate([0,0,85-0.08*3.6]) rotate_extrude($fn=144,angle=30)  translate([radius-1,thick/2-0.03,0])polygon( points=[[0,0],[3,0],[3,1],[0,1]] ); 
rotate ([0,0,2.5]) translate([radius+width/2,0,0])cylinder($fn=32,h=5,center=true,r=0.25+a) ; 
rotate ([0,0,87.5]) translate([radius+width/2,0,0])cylinder($fn=32,h=5,center=true,r=0.25+a) ; 

}
rotate ([0,0,-2.5]) translate([radius+width/2,0,thick/4])cylinder($fn=32,h=thick*3/4,r=0.25-a) ; 
rotate ([0,0,92.5]) translate([radius+width/2,0,thick/4])cylinder($fn=32,h=thick*3/4,r=0.25-a) ;
}
if (hole) {
rotate([0,0,45]) translate([radius-0.2,0,0]) rotate([0,0,-30]) rotate_extrude($fn=36,angle=60) polygon( points=[[0,0.2],[0,thick-0.2],[1.5,thick-0.4],[1.5,0.4]] );
}
 
rotate ([0,0,-2.5]) translate([radius+width/2,0,-thick/4])cylinder($fn=32,h=thick*2,r=0.1) ; 
rotate ([0,0,92.5]) translate([radius+width/2,0,-thick/4])cylinder($fn=32,h=thick*2,r=0.1) ;
//translate([1.3+b*0.8,-2,-3]) cube([20,20,20]);
}
}
}
}
echo(version=version());
