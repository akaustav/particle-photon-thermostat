
// ------------------------------------------
// Controlling Home Thermostat over the Internet
// ------------------------------------------

#define in true
#define out false
// Declare aliases for pins
// temperature_plus is D2, temperature_minus is D6
int temperature_plus = D2;
int temperature_minus = D6;
// int LED = D7;

// Declare variables for setting temperature
int tempValue = 76;
int curTemp = 76;
int correction = 0;

char setTime[6] = "HH:MM";

void setup() {
    
    // Set Time Zone
    Time.zone(-7);
    
    // Pin configuration
    pinMode(temperature_plus, INPUT);
    pinMode(temperature_minus, INPUT);
    //   pinMode(LED, OUTPUT);
    
    // Declare Spark.functions
    // When we ask the cloud for the function "changeTemp", it will employ the function change_temperature_app() from this app.
    Particle.function("ChangeTemp", temperature_change_app);

    // When we ask the cloud for the function "IFTTT", it will employ the function IFTTT_presence_app() from this app.
    // Spark.function("IFTTT", IFTTT_presence_app);


    // Declare Spark.variables to access variables from the cloud
    // This variable is used for sending temperture to be set at
    Particle.variable("currentTemp", curTemp);

    // This variable is used to read the timestamp
    Particle.variable("Time", setTime);
    
    // Initialize components
    // Set the button pins high
    // digitalWrite(temperature_plus, HIGH);
    // digitalWrite(temperature_minus, HIGH);
    // digitalWrite(LED, HIGH);

}


void increase (void) {
    
    pinMode(temperature_plus, OUTPUT);
    digitalWrite(temperature_plus, LOW);
    delay(500);
    pinMode(temperature_plus, INPUT);
    delay(500);
}
    
    
void decrease (void) {
    
    pinMode(temperature_minus, OUTPUT);
    digitalWrite(temperature_minus, LOW);
    delay(500);
    pinMode(temperature_minus, INPUT);
    delay(500);
}


void loop()
{

}


// This is the temperature_change_app() function we registered to the "ChangeTemp" Spark.function earlier.
int temperature_change_app (String command) {
    // Spark.functions always take a string as an argument and return an integer.

    tempValue = command.toInt();
    sprintf(setTime, "%02d:%02d", Time.hour(), Time.minute());

    // Particle.publish("Temperature", command, 60, PRIVATE);
    
    if (tempValue > curTemp)
    {
        
        correction = tempValue - curTemp + 1;
        for (int i = 0; i < correction; i++)
        {
            increase();
        }
        curTemp = tempValue;
        return correction - 1;
    }
    
    else if (tempValue < curTemp)
    {
        
        correction = curTemp - tempValue + 1;
        for (int i = 0; i < correction; i++)
        {
            decrease();
        }
        curTemp = tempValue;
        return 1 - correction;
    }
    
    else 
    {
        return 0;
    }
}



