

/**************************************************************/
/* define pin setup, what sensor/fan is connected to what pin */
/**************************************************************/

int LM35_1_PIN = A0;
int LM35_2_PIN = A1;
int FAN_PWM_PIN = 9;

// only needed when measuring temperature time series values
#define SEND_TEMP_VALUES_TO_HOST true
#define SEND_TO_HOST_INTERVALL_IN_MILLIS 1000

#define SENSOR_1_TEMP_THRESHOLD_in_CELCIUS 40.0

/***************************************************************/

#define RING_BUFFER_SIZE 10
#define INVALID_VALUE -273.0
#define AVERAGE_INTERVAL_IN_MILLIS 60*1000 // e.g. one minute = 60*1000

/***************************************************************/

struct TemperatureRingBuffer {
    float values[RING_BUFFER_SIZE];
    int writeIndex = 0;
};

TemperatureRingBuffer tempRingBuf_1;
TemperatureRingBuffer tempRingBuf_2;
unsigned long lastSend = 0;

/**************************************************************/

float readTemperatureInCelcius(int lm35_pin) {
    return (5.0 * 100.0 * analogRead(lm35_pin)) / 1024;
}

void buffer_write(TemperatureRingBuffer *tempRingBuf, float value) {
    tempRingBuf->values[tempRingBuf->writeIndex] = value;
    tempRingBuf->writeIndex = (tempRingBuf->writeIndex++) % RING_BUFFER_SIZE;
}

void initRingBuffer(TemperatureRingBuffer *tempRingBuf) {
    for (int i=0; i<RING_BUFFER_SIZE; i++) {
        tempRingBuf->values[i] = INVALID_VALUE;
    }
}

float averageValues(TemperatureRingBuffer *tempRingBuf) {
    float sum = 0.0;
    int numberOfValidValues = 0;
    for (int i=0; i<RING_BUFFER_SIZE; i++) {
        if (tempRingBuf->values[i] != INVALID_VALUE) {
            numberOfValidValues ++;
            sum += tempRingBuf->values[i];
        }
    }
    return sum / (float)numberOfValidValues;
}

void sendValuesToHost() {
    unsigned long currentMillis = millis();
    if ((currentMillis - lastSend) > SEND_TO_HOST_INTERVALL_IN_MILLIS) {
        lastSend = currentMillis;

        Serial.print("(1) ");
        Serial.print(averageValues(&tempRingBuf_1), 1);
        Serial.println("Grad Celsius");

        Serial.print("(2) ");
        Serial.print(averageValues(&tempRingBuf_2), 1);
        Serial.println("Grad Celsius");

        Serial.println("");
    }
}

void setup() {
    if (SEND_TEMP_VALUES_TO_HOST) {
        Serial.begin(9600);
    }

    pinMode(FAN_PWM_PIN, OUTPUT);

    initRingBuffer(&tempRingBuf_1);
    initRingBuffer(&tempRingBuf_2);
}

void loop() {
    buffer_write(&tempRingBuf_1, readTemperatureInCelcius(LM35_1_PIN));
    buffer_write(&tempRingBuf_2, readTemperatureInCelcius(LM35_2_PIN));
    delay(AVERAGE_INTERVAL_IN_MILLIS / RING_BUFFER_SIZE); // e.g. one minute = 60*1000

    float lastIntervalTempAverage = averageValues(&tempRingBuf_1);
    if (lastIntervalTempAverage > SENSOR_1_TEMP_THRESHOLD_in_CELCIUS) {
        analogWrite(FAN_PWM_PIN, 255);
    } else {
        analogWrite(FAN_PWM_PIN, 0);
    }

    if (SEND_TEMP_VALUES_TO_HOST) {
        sendValuesToHost();
    }
}
