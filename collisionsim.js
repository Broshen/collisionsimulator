var c = document.getElementById("myCanvas");
var ctx = c.getContext("2d");
var myTimer;

var object_1 ={ 
x:0,
y:0,
vx:0,
vy:0,
ax:0,
ay:0,
mass:0,
radius:0,
inertia:0,
w:0, //angular v
torque:0
};

var object_2 ={ 
x:0,
y:0,
vx:0,
vy:0,
ax:0,
ay:0,
mass:0,
radius:0,
inertia:0,
w:0, //angular v
torque:0
};

var springConstant=1400, interval=0.05, mu=0.05, F, cost, sint, alpha1, alpha2, d;

function resize(){
	c.width = window.innerWidth-20;
	c.height = 400;
	redraw();
}

//interval is in s
function drawBackground(){


//clears screen
 ctx.clearRect(0, 0, screen.width,screen.height);

 
 //draw the objects
 ctx.beginPath();
 ctx.arc(object_1.x,object_1.y,object_1.radius,0,2*Math.PI);
 ctx.stroke();
 ctx.beginPath();
 ctx.arc(object_2.x,object_2.y,object_2.radius,0,2*Math.PI);
 ctx.stroke();
 
 //draw their velocity vectors
 ctx.beginPath();
 ctx.moveTo(object_1.x, object_1.y);
 ctx.lineTo(object_1.x + object_1.vx, object_1.y + object_1.vy);
 ctx.stroke();
 
 ctx.beginPath();
 ctx.moveTo(object_2.x,object_2.y);
 ctx.lineTo(object_2.x + object_2.vx, object_2.y + object_2.vy);
 ctx.stroke();
 
 }

function getInputs(){
  object_1.x=Number(document.getElementById("x_1").value);
  object_1.y=Number(document.getElementById("y_1").value);
  object_1.vx=Number(document.getElementById("vx_1").value);
  object_1.vy=Number(document.getElementById("vy_1").value);
  object_1.radius=Number(document.getElementById("r_1").value);
  object_1.mass=Number(document.getElementById("m_1").value);
  object_1.inertia=0.4*object_1.mass*object_1.radius*object_1.radius;
  
  
  
  object_2.x=Number(document.getElementById("x_2").value);
  object_2.y=Number(document.getElementById("y_2").value);
  object_2.vx=Number(document.getElementById("vx_2").value);
  object_2.vy=Number(document.getElementById("vy_2").value);
  object_2.radius=Number(document.getElementById("r_2").value);
  object_2.mass=Number(document.getElementById("m_2").value);
  object_2.inertia=0.4*object_2.mass*object_2.radius*object_2.radius;
  
  
  springConstant=Number(document.getElementById("k").value);
  mu=Number(document.getElementById("mu").value);
  
  
  drawBackground();
  
	 }

function calculate(){
d = Math.sqrt(Math.pow(object_2.x-object_1.x,2) + Math.pow(object_2.y-object_1.y,2));

if (d < (object_1.radius + object_2.radius)){
	
	
  //Given the same spring constant, displacement and spring force should be the same
   F = springConstant*(object_1.radius + object_2.radius - d)/2;
   //Here, cos(theta) is delta-x/d and sin(theta) is delta-y/d
   cost = (object_2.x-object_1.x)/d;
   sint = (object_2.y-object_1.y)/d;
   
   //Using torque, given that T=fr and t=I(alpha), we can calculate (alpha)= fr/I where f = F(mu)
   alpha1 = (F*mu*object_1.radius)/object_1.inertia;
   alpha2 = (F*mu*object_2.radius)/object_2.inertia;
   
   //Accelerations are calculated by free-body diagram observation
   object_1.ax = sint*alpha1 - cost*F/object_1.mass;
   object_1.ay = cost*alpha1 - sint*F/object_1.mass;
   object_2.ax = cost*F/object_2.mass - sint*alpha2;
   object_2.ay = sint*F/object_2.mass - cost*alpha2;
   
   //Calculate the new velocities
   object_1.vx += object_1.ax * interval;
   object_1.vy += object_1.ay * interval;
   object_2.vx += object_2.ax * interval;
   object_2.vy += object_2.ay * interval;
   object_1.w += alpha1 * interval;
   object_2.w += alpha2 * interval;
   
   }
    //Calculate the new positions
    object_1.x += (object_1.vx * interval);
    object_1.y += (object_1.vy * interval);
    object_2.x += (object_2.vx * interval);
    object_2.y += (object_2.vy * interval);
    object_1.torque += (object_1.w * interval);
    object_2.torque += (object_2.w * interval);
}

function run(){
	calculate();
	drawBackground();
}



function start(){
	myTimer = setInterval(run, 50);
	document.getElementById("startbutton").disabled = true;
	document.getElementById("stopbutton").disabled = false;
}

function stop(){
	clearInterval(myTimer);
	document.getElementById("startbutton").disabled = false;
	document.getElementById("stopbutton").disabled = true;
}

function initialize(){

}
resize();
initialize();
stop();

								   





