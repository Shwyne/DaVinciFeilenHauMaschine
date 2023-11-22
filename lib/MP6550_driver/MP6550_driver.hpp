#ifndef MP6550_driver_hpp
#define MP6550_driver_hpp

#include <Arduino.h>

class MP6550{		
	private:

		//*Arguments for MP6550control:
		int speed_;		  //Integer for pwm-speed (255 = 0 speed | 0 = max Speed)

		//*Arguments for Control Pins:
		uint8_t  IN1;				//Pin connected to IN1
		uint8_t  IN2;				//Pin connected to IN2
		uint8_t  SLP;			   //Pin connected to SLP
		
	public:
		MP6550(uint8_t IN1pin, uint8_t  IN2pin, uint8_t  SLPpin);	//Constructor with default: Sleep,In1,In2=0, speed = 0, br_speed = 0; standby = 1; dir_ = 0
		void run(int speed);				//Runs MP6550 with given speed (-255 to 255)
		void brake();						//MP6550 brakes the motor
		void coast();						//MP6550 coasts the motor
		int getSpeed();						//Gets speed of MP6550 (same as line above)
		bool sleepState();				//Gets Sleep State of MP6550 (Status mentioned standby_)
		void sleep();					//Puts Driver to Sleep
		void printData();				//Prints all Data of MP6550
		~MP6550();							//Destructor -> Brakes first then enables SLP
};

#endif
