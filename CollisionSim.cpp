/* Collision Simulator
 *
 * This program simulates a collision between two elastic spheres
 * of equal density. It takes into account glancing
 * collisions as well as rotation.
 *
 * Group Members:
 *  Nicholas Blake
 *  Boshen Cui
 *  Tony Liao
 *  Susie Liu
 */

#include<iostream>
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<math.h>
#include<fstream>
#include<string>

using namespace std;
static double m1, m2, r1, r2, i1, i2, k=1400, ti, dc, mu;

void menu(double &s1x,double &s1y,double &s2x,double &s2y,double &v1x,double &v1y,double &v2x,double &v2y){

    //Display main menu
    system("cls");
    cout<<"\nCollision Simulator\n\n";

    //Inputs
    //Spring constant are assumed to be the same for both balls
    cout<<"Please enter the initial conditions.\n";
    cout<<"Time interval per display (ms): ";
    cin>>ti;
    cout<<"Calculations per display: ";
    cin>>dc;
    cout<<"Coefficient of friction: ";
    cin>>mu;
    //Convert ti to time interval per calculation, in seconds
    ti /= (1000*dc);

    //Position and velocity inputs
    cout<<"\nProjectile\nMass (kg): ";
    cin>>m1;
    cout<<"Position (m)\nx: ";
    cin>>s1x;
    cout<<"y: ";
    cin>>s1y;
    cout<<"Velocity (m/s)\nx: ";
    cin>>v1x;
    cout<<"y: ";
    cin>>v1y;
    cout<<"\nTarget\nMass (kg): ";
    cin>>m2;
    cout<<"Position (m)\nx: ";
    cin>>s2x;
    cout<<"y: ";
    cin>>s2y;
    cout<<"Velocity (m/s)\nx: ";
    cin>>v2x;
    cout<<"y: ";
    cin>>v2y;

    //Output table
    system("cls");
    cout<<"\nProjectile\nt-Position\tt-Velocity\ta-position\ta-velocity\ttime";
    cout<<"\nTarget\nt-Position\tt-Velocity\ta-position\ta-velocity\n";

}

void tick(double &s1x,double &s1y,double &t1,double &w1,double &s2x,double &s2y,double &t2,double &w2,double &v1x,double &v1y,double &v2x,double &v2y,double &t){

    //Distance is taken between the centres of the balls
    double d = sqrt(((s2x-s1x)*(s2x-s1x))+((s2y-s1y)*(s2y-s1y)));

    //Check if balls are touching
    if (d < (r1+r2))
    {
        //Compute magnitude of F as kx
        //Given the same spring constant, displacement and spring force should be the same
        double F = (k*(r1+r2-d)/2);

        //Here, cos(theta) is delta-x/d and sin(theta) is delta-y/d
        double cost = ((s2x-s1x)/d);
        double sint = ((s2y-s1y)/d);

        //Using torque, given that T=fr and t=I(alpha), we can calculate (alpha)= fr/I where f = F(mu)
        double al1 = ((F*mu*r1)/i1);
        double al2 = ((F*mu*r2)/i2);

        //Accelerations are calculated by free-body diagram observation
        double a1x = ((sint*al1)-(cost*(F/m1)));
        double a1y = ((cost*al1)-(sint*(F/m1)));
        double a2x = ((cost*(F/m2))-(sint*al2));
        double a2y = ((sint*(F/m2))-(cost*al2));

        //Calculate the new velocities
        v1x += (a1x * ti);
        v1y += (a1y * ti);
        v2x += (a2x * ti);
        v2y += (a2y * ti);
        w1 += (al1 * ti);
        w2 += (al2 * ti);
    }
    //Calculate the new positions
    s1x += (v1x * ti);
    s1y += (v1y * ti);
    s2x += (v2x * ti);
    s2y += (v2y * ti);
    t1 += (w1 * ti);
    t2 += (w2 * ti);

    //Update time
    t += ti;
}

void display(double s1x,double s1y,double t1,double w1,double s2x,double s2y,double t2,double w2,double v1x,double v1y,double v2x,double v2y,double t){
    //displays the vectors at a certain time on screen
    cout<<"\n("<<s1x<<","<<s1y<<")\t";
    cout<<"("<<v1x<<","<<v1y<<")\t";
    cout<<t1<<"\t"<<w1<<"\t"<<t<<"\n";
    cout<<"("<<s2x<<","<<s2y<<")\t";
    cout<<"("<<v2x<<","<<v2y<<")\t";
    cout<<t2<<"\t"<<w2<<"\n";
}

int main(){

    //Open Excel file for data output
    cout << "\nPlease enter a file name to write: ";
    string name;
    getline(cin, name);
    name+=".csv";
    ofstream file;
    file.open(name.c_str());

    //Display header on .csv
    file<<"Projectile,,,,,,Target,,,,,,Time"<<endl;
    file<<"t-Position,,t-Velocity,,a-Position,a-Velocity,t-Position,,t-Velocity,,a-Position,a-Velocity"<<endl;
    file<<"x,y,x,y,,,x,y,x,y"<<endl;
    file<<endl;

    //The rotation of objects will be considered, but only as a result of a glancing collision
    double s1x,s1y,t1=0,w1=0,s2x,s2y,t2=0,w2=0,v1x,v1y,v2x,v2y,t=0;
    menu(s1x,s1y,s2x,s2y,v1x,v1y,v2x,v2y);

    //Assuming equal density, the radius is that of the measured stress ball
    //times the cube root of the ratio between their masses
    r1 = (cbrt(m1/0.043)*0.0325);
    r2 = (cbrt(m2/0.043)*0.0325);

    //For a solid ball, the rotational inertia is 2/5 mr^2
    i1 = (0.4*m1*r1*r1);
    i2 = (0.4*m2*r2*r2);

    while(true)
    {
        //Display 10 iterations, then wait for user input
        for(int i = 0; i < 10; i++)
        {
            //Calculates dc iterations before displaying once.
            for(int j = 0; j < dc; j++)
                tick(s1x,s1y,t1,w1,s2x,s2y,t2,w2,v1x,v1y,v2x,v2y,t);
            display(s1x,s1y,t1,w1,s2x,s2y,t2,w2,v1x,v1y,v2x,v2y,t);

            //saves data to the .csv file.
            file<<s1x<<","<<s1y<<",";
            file<<v1x<<","<<v1y<<",";
            file<<t1<<","<<w1<<",";
            file<<s2x<<","<<s2y<<",";
            file<<v2x<<","<<v2y<<",";
            file<<t2<<","<<w2<<",";
            file<<t<<endl;
        }
        getch(); //waits for user input
    }
    file.close();
    return 0;
}
