// ------------------------------------------
// Controlling Home Thermostat over the Internet
// ------------------------------------------

// TODO - akaustav: Find if code is being used
#define in true
#define out false

// Declare aliases for pins
//   temperature_plus is on pin D2
//   temperature_minus is on pin D6
int temperature_plus = D2;
int temperature_minus = D6;

// Declare global variables for setting temperature
int tempValue = 76; // Default set temperature is 76° Fahrenheit
int curTemp = 76;   // Current temperature is assumed as 76° Fahrenheit
int correction = 0; // Positive difference between curTemp and tempValue

// TODO -akaustav: Check if array size should be set to 5 instead of 6
char setTime[6] = "HH:MM";


// Function for initial setup of the device
void setup() {
  // Set Time Zone = Arizona Time
  // TODO - akaustav: Find if code is being used
  Time.zone(-7);

  // Set pin configurations to INPUT
  // INPUT consumes less power than OUTPUT
  pinMode(temperature_plus, INPUT);
  pinMode(temperature_minus, INPUT);

  // Register Particle.functions
  // When we ask the cloud for the function "changeTemp", it will employ the function change_temperature_app() from this app.
  Particle.function("ChangeTemp", temperature_change_app);

  // Register Particle.variables to access variables from the cloud
  // This variable is used for sending temperture to be set at
  Particle.variable("currentTemp", curTemp);

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


// This is the temperature_change_app() function we registered to the "ChangeTemp" Particle.function earlier.
// Particle.functions always take a string as an argument and return an integer.
int temperature_change_app(String command) {
  tempValue = command.toInt();

  // TODO - akaustav: Find if code is being used
  sprintf(setTime, "%02d:%02d", Time.hour(), Time.minute());

  if (tempValue > curTemp) {
    correction = tempValue - curTemp + 1;

    for (int i = 0; i < correction; i++) {
      increase();
    }

    curTemp = tempValue;
    return correction - 1;
  } else if (tempValue < curTemp) {
    correction = curTemp - tempValue + 1;

    for (int i = 0; i < correction; i++) {
      decrease();
    }

    curTemp = tempValue;
    return 1 - correction;
  } else {
    return 0;
  }
}
