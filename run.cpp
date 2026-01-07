//define motor pins
#define ldir -
#define lpwm -
#define lbrk -
#define lcod -
#define rdir -
#define rpwm -
#define rbrk -
#define rcod - 

//define counts for movements 1:50 gear ratio
const int lowerc=306, const int upperc=612, const int turn=251, const int speed=150;
//zero encoders
long leftn=0, long ricghtn=0;
//count all encoder movements
void resetencoders(){
    noInterrupts();
    leftn=0;
    rightn=0;
    interrupts();}
void encodercounts(int leftg, int rightg, int velo){
    zeroencoders();
}