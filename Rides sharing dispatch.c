#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define MAX_DRIVERS 50
#define MAX_RIDERS 50
#define MAX_RIDES 100

// ------------ Structures ------------
typedef struct { double x, y; } Point;

typedef struct {
    int id;
    char name[20];
    double rating;
    Point location;
    bool available;
} Driver;

typedef struct {
    int id;
    char name[20];
    Point pickup, dropoff;
} Rider;

typedef struct {
    int id;
    int riderId;
    int driverId;
    double pickupDist;
    char status[15]; // "ongoing" or "completed"
} Ride;

// ------------ Globals ------------
Driver drivers[MAX_DRIVERS]; int driverCount=0;
Rider riderQueue[MAX_RIDERS]; int front=0, rear=0;
Ride rides[MAX_RIDES]; int rideCount=0;

int nextDriverId=1, nextRiderId=1, nextRideId=1;

// ------------ Helpers ------------
double distance(Point a, Point b) {
    return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}
bool isQueueEmpty() { return front==rear; }
void enqueue(Rider r){ riderQueue[rear++]=r; }
Rider dequeue(){ return riderQueue[front++]; }

// ------------ Core ------------
void addDriver() {
    char name[20]; double rating,x,y;
    printf("Enter driver name, rating, x, y: ");
    scanf("%s %lf %lf %lf", name,&rating,&x,&y);
    drivers[driverCount]=(Driver){nextDriverId++, "", rating,{x,y},true};
    strcpy(drivers[driverCount].name,name);
    driverCount++;
    printf("Driver added.\n");
}

void addRider() {
    char name[20]; double px,py,dx,dy;
    printf("Enter rider name, pickupX pickupY dropX dropY: ");
    scanf("%s %lf %lf %lf %lf", name,&px,&py,&dx,&dy);
    Rider r={nextRiderId++,"",{px,py},{dx,dy}};
    strcpy(r.name,name);
    enqueue(r);
    printf("Rider request added.\n");
}

void dispatch() {
    if(isQueueEmpty()){ printf("No riders waiting.\n"); return; }
    Rider r=riderQueue[front]; // peek
    int chosen=-1; double best=1e9;
    for(int i=0;i<driverCount;i++) if(drivers[i].available){
        double d=distance(drivers[i].location,r.pickup);
        if(chosen==-1 || d<best || 
          (fabs(d-best)<1e-6 && drivers[i].rating>drivers[chosen].rating)){
            chosen=i; best=d;
        }
    }
    if(chosen==-1){ printf("No drivers available.\n"); return; }
    Ride ride={nextRideId++,r.id,drivers[chosen].id,best,"ongoing"};
    rides[rideCount++]=ride;
    drivers[chosen].available=false;
    dequeue();
    printf("Driver %s assigned to Rider %s (dist=%.2f)\n",
        drivers[chosen].name,r.name,best);
}

void completeRide() {
    int did; printf("Enter driver ID: "); scanf("%d",&did);
    for(int i=0;i<rideCount;i++)
        if(rides[i].driverId==did && strcmp(rides[i].status,"ongoing")==0){
            strcpy(rides[i].status,"completed");
            for(int j=0;j<driverCount;j++) if(drivers[j].id==did) drivers[j].available=true;
            printf("Ride %d completed.\n",rides[i].id);
            return;
        }
    printf("No ongoing ride for this driver.\n");
}

// ------------ Display ------------
void showDrivers(){
    for(int i=0;i<driverCount;i++)
        printf("Driver %d - %s (rating %.1f) %s\n",
          drivers[i].id,drivers[i].name,drivers[i].rating,
          drivers[i].available?"Available":"Busy");
}
void showRiders(){
    for(int i=front;i<rear;i++)
        printf("Rider %d - %s pickup(%.1f,%.1f)\n",
          riderQueue[i].id,riderQueue[i].name,
          riderQueue[i].pickup.x,riderQueue[i].pickup.y);
}
void showRides(){
    for(int i=0;i<rideCount;i++)
        printf("Ride %d rider=%d driver=%d dist=%.2f status=%s\n",
          rides[i].id,rides[i].riderId,rides[i].driverId,
          rides[i].pickupDist,rides[i].status);
}

// ------------ Main ------------
int main(){
    int choice;
    while(1){
        printf("\n1.Add Driver 2.Add Rider 3.Dispatch 4.Complete Ride 5.Show Drivers 6.Show Riders 7.Show Rides 0.Exit\n> ");
        scanf("%d",&choice);
        if(choice==0) break;
        if(choice==1) addDriver();
        else if(choice==2) addRider();
        else if(choice==3) dispatch();
        else if(choice==4) completeRide();
        else if(choice==5) showDrivers();
        else if(choice==6) showRiders();
        else if(choice==7) showRides();
    }
    return 0;
}
