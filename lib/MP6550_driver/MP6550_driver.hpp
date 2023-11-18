#ifndef MP6550_driver_hpp
#define MP6550_driver_hpp

#include <Arduino.h>

class motor{		
	private:

		//*Arguments for Motorcontrol:
		int speed_;		  //Integer for pwm-speed (255 = 0 speed | 0 = max Speed)
		int breakForce_;	//Integer for Brake_speed (255 = max Brake | 0 = No Break)

		//*Arguments for Control Pins:
		int IN1;				//Pin connected to IN1
		int IN2;				//Pin connected to IN2
		int SLP;			   //Pin connected to SLP
		
	public:
		motor(int IN1pin, int IN2pin, int SLPpin);	//Constructor with default: Sleep,In1,In2=0, speed = 0, br_speed = 0; standby = 1; dir_ = 0
		void run(int speed);				//Runs Motor with given speed (-255 to 255)
		void brake(int breakForce);				//Motor brakes with given speed br_speed 
		int getSpeed();						//Gets speed of Motor (same as line above)
		int getBrakeForce();	//Gets brake speed of Motor
		bool sleepState();				//Gets Sleep State of Motor (Status mentioned standby_)
		void sleep();					//Puts Driver to Sleep
		void printData();				//Prints all Data of Motor
		~motor();							//Destructor -> Brakes first then enables SLP
};

class motorVisen : public motor {
		
	private:
		//*Arguments for Control Pins:
		int VIS;				//Pin connected to VISEN (Current Measurement)

		//*Arguments for Current Measurement:
		double CuRatio_;		
		double Visen_;
	
	public:
		motorVisen(int IN1pin, int IN2pin, int SLPpin, int VISpin);	//Constructor with default: Sleep,In1,In2=0, speed = 0, br_speed = 0; standby = 1; dir_ = 0	
		void setCurrentRatio(int ratio);	//Sets current ratio (Resistance (default 2kOhm) * ISet = V_Isen in V/A)
		double getCurrent(); 				//Returns current in mA
		~motorVisen();						//Destructor -> Brakes first then enables SLP
};

#endif
