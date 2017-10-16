// ------------------------------------------
// Controlling Home Thermostat over the Internet
// v0.0.2
// ------------------------------------------

#define BUTTON_PRESSED_TIMEOUT 7000 // 7 seconds
#define DEBOUNCE_DELAY_PLUS  500000 // 500 milliseconds
#define DEBOUNCE_DELAY_MINUS 500000 // 500 milliseconds

// Declare aliases for pins
//   temperature_plus is on pin D2
//   temperature_minus is on pin D6
int temperature_plus = D2;
int temperature_minus = D6;

// Declare global variables
// Variables for button presses
int changeWindow = 0; // Are you in the window change? Is the timer running?
int counter = 76;

// Variable for setting temperature
int oldSetToTemp = 76;  // Previous set to temperature value is assumed as 76° Fahrenheit
int newSetToTemp = 76;  // Default set to temperature value is 76° Fahrenheit
int correction = 0;  // Positive difference between oldSetToTemp and newSetToTemp

// char setTime[6] = "HH:MM";

// Initialize a timer
Timer buttonPressedTimer(BUTTON_PRESSED_TIMEOUT, resetButtonPressedFlag);

// Function for initial setup of the device
void setup() {
  // Set Time Zone = Arizona Time
  // Time.zone(-7);

  // Set pin configurations to INPUT
  // INPUT consumes less power than OUTPUT
  pinMode(temperature_plus, INPUT);
  pinMode(temperature_minus, INPUT);

  // Setting up interrupts
  attachInterrupt(temperature_plus, plus_button_pressed, FALLING);
  attachInterrupt(temperature_minus, minus_button_pressed, FALLING);

  // Register Particle.functions
  // When we ask the cloud for the function "SetTempTo", it will employ the function set_temperature_app() from this app.
  Particle.function("SetTempTo", set_temperature_app);

  // Register Particle.variables to access variables from the cloud
  // This variable is used for tracking current temperture inside the house
  Particle.variable("OldSetTo", oldSetToTemp);

  // This variable is used for tracking current set to temperture
  Particle.variable("NewSetTo", newSetToTemp);

  Particle.variable("Counter", counter);
  Particle.variable("changeWindow", changeWindow);

  // This variable is used to read the timestamp
  // Particle.variable("Time", setTime);

  // Enable all interrupts
  interrupts();
}


// Callback function for plus button pressed interrupt
void plus_button_pressed() {
  // noInterrupts();
  buttonPressedTimer.reset();
  ATOMIC_BLOCK() {
    delayMicroseconds(DEBOUNCE_DELAY_PLUS); // Wait for DEBOUNCE_DELAY
  }
  counter++;

  if (changeWindow == 0) {
    changeWindow = 1;
  } else if (changeWindow == 1) {
    oldSetToTemp++;
  }

  // interrupts();
}

// Callback function for plus button pressed interrupt
void minus_button_pressed() {
  // noInterrupts();
  buttonPressedTimer.reset();
  ATOMIC_BLOCK() {
    delayMicroseconds(DEBOUNCE_DELAY_MINUS); // Wait for DEBOUNCE_DELAY
  }
  counter--;

  if (changeWindow == 0) {
    changeWindow = 1;
  } else if (changeWindow == 1) {
    oldSetToTemp--;
  }

  // interrupts();
}

// Timer reset callback
void resetButtonPressedFlag() {
  buttonPressedTimer.stop();
  changeWindow = 0;
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
  newSetToTemp = command.toInt();

  // sprintf(setTime, "%02d:%02d", Time.hour(), Time.minute());

  if (newSetToTemp > oldSetToTemp) {
    correction = newSetToTemp - oldSetToTemp + 1;

    for (int i = 0; i < correction; i++) {
      increase();
    }

    oldSetToTemp = newSetToTemp;
    return correction - 1;
  } else if (newSetToTemp < oldSetToTemp) {
    correction = oldSetToTemp - newSetToTemp + 1;

    for (int i = 0; i < correction; i++) {
      decrease();
    }

    oldSetToTemp = newSetToTemp;
    return 1 - correction;
  } else {
    return 0;
  }
}
