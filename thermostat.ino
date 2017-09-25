// ------------------------------------------
// Controlling Home Thermostat over the Internet
// ------------------------------------------

#define in true
#define out false

// Declare aliases for pins
//   temperature_plus is on pin D2,
//   temperature_minus is on pin D6
int temperature_plus = D2;
int temperature_minus = D6;

// Declare variables for setting temperature
int tempValue = 76;
int curTemp = 76;
int correction = 0;

char setTime[6] = "HH:MM";


void setup() {
  // Set Time Zone = Arizona Time
  Time.zone(-7);

  // Pin configuration
  pinMode(temperature_plus, INPUT);
  pinMode(temperature_minus, INPUT);

  // Register Particle.functions
  // When we ask the cloud for the function "changeTemp", it will employ the function change_temperature_app() from this app.
  Particle.function("ChangeTemp", temperature_change_app);

  // Declare Particle.variables to access variables from the cloud
  // This variable is used for sending temperture to be set at
  Particle.variable("currentTemp", curTemp);

  // This variable is used to read the timestamp
  Particle.variable("Time", setTime);
}

void increase() {
  pinMode(temperature_plus, OUTPUT);
  digitalWrite(temperature_plus, LOW);
  delay(500);
  pinMode(temperature_plus, INPUT);
  delay(500);
}


void decrease() {
  pinMode(temperature_minus, OUTPUT);
  digitalWrite(temperature_minus, LOW);
  delay(500);
  pinMode(temperature_minus, INPUT);
  delay(500);
}


void loop() {}


// This is the temperature_change_app() function we registered to the "ChangeTemp" Particle.function earlier.
// Particle.functions always take a string as an argument and return an integer.
int temperature_change_app(String command) {
  tempValue = command.toInt();
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
