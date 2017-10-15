// ------------------------------------------
// Controlling Home Thermostat over the Internet
// v0.0.2
// ------------------------------------------


// Declare aliases for pins
//   temperature_plus is on pin D2
//   temperature_minus is on pin D6
int temperature_plus = D2;
int temperature_minus = D6;

// Declare global variables for setting temperature
int insideTemp = 76; // Inside temperature is assumed as 76° Fahrenheit
int setToTemp = 76;  // Default set to temperature is 76° Fahrenheit
int correction = 0;  // Positive difference between insideTemp and setToTemp

char setTime[6] = "HH:MM";


// Function for initial setup of the device
void setup() {
  // Set Time Zone = Arizona Time
  Time.zone(-7);

  // Set pin configurations to INPUT
  // INPUT consumes less power than OUTPUT
  pinMode(temperature_plus, INPUT);
  pinMode(temperature_minus, INPUT);

  // Register Particle.functions
  // When we ask the cloud for the function "SetTempTo", it will employ the function set_temperature_app() from this app.
  Particle.function("SetTempTo", set_temperature_app);

  // Register Particle.variables to access variables from the cloud
  // This variable is used for tracking current temperture inside the house
  Particle.variable("Inside", insideTemp);

  // This variable is used for tracking current set to temperture
  Particle.variable("SetTo", setToTemp);

  // This variable is used to read the timestamp
  Particle.variable("Time", setTime);
}


// Function to increase temperature by 1° Fahrenheit
void increase() {
  // Change the temperature plus pin mode to OUTPUT
  pinMode(temperature_plus, OUTPUT);

  // Send signal to the increase temperature pin
  // The pin is by default pulled-up i.e. 1 = OFF, 0 = ON
  digitalWrite(temperature_plus, LOW);

  // Wait for 500 milliseconds
  delay(500);

  // Change the temperature plus pin mode back to INPUT to conserve power
  pinMode(temperature_plus, INPUT);

  // Wait for 500 milliseconds
  delay(500);
}


// Function to decrease temperature by 1° Fahrenheit
void decrease() {
  // Change the temperature minus pin mode to OUTPUT
  pinMode(temperature_minus, OUTPUT);

  // Send signal to the decrease temperature pin
  // The pin is by default pulled-up i.e. 1 = OFF, 0 = ON
  digitalWrite(temperature_minus, LOW);

  // Wait for 500 milliseconds
  delay(500);

  // Change the temperature minus pin mode back to INPUT to conserve power
  pinMode(temperature_minus, INPUT);

  // Wait for 500 milliseconds
  delay(500);
}


// Do nothing on a loop
void loop() {}


// This is the set_temperature_app() function we registered to the "SetTempTo" Particle.function earlier.
// Particle.functions always take a string as an argument and return an integer.
int set_temperature_app(String command) {
  setToTemp = command.toInt();

  sprintf(setTime, "%02d:%02d", Time.hour(), Time.minute());

  if (setToTemp > insideTemp) {
    correction = setToTemp - insideTemp + 1;

    for (int i = 0; i < correction; i++) {
      increase();
    }

    insideTemp = setToTemp;
    return correction - 1;
  } else if (setToTemp < insideTemp) {
    correction = insideTemp - setToTemp + 1;

    for (int i = 0; i < correction; i++) {
      decrease();
    }

    insideTemp = setToTemp;
    return 1 - correction;
  } else {
    return 0;
  }
}
