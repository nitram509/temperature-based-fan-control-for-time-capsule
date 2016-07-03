

/**************************************************************/
/* define pin setup, what sensor/fan is connected to what pin */
/**************************************************************/

int LM35_1_PIN = A0;
int LM35_2_PIN = A1;
int FAN_PWM_PIN = 9;

// only needed when measuring temperature time series values
#define SEND_TEMP_VALUES_TO_HOST true
#define SEND_TO_HOST_INTERVALL_IN_MILLIS 6000

#define SENSOR_1_TEMP_THRESHOLD_in_CELCIUS 40.0
#define SENSOR_2_TEMP_THRESHOLD_in_CELCIUS 37.0

/***************************************************************/

#define RING_BUFFER_SIZE 10
#define INVALID_VALUE -273.0
#define AVERAGE_INTERVAL_IN_MILLIS 60000 // e.g. one minute = 60000

/***************************************************************/

struct TemperatureRingBuffer {
    float values[RING_BUFFER_SIZE];
    int writeIndex = 0;
};

TemperatureRingBuffer tempRingBuf_1;
TemperatureRingBuffer tempRingBuf_2;
unsigned long lastSendMillis = 0;
unsigned long lastRecordedMillis = 0;

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
    bool isIntervalPassed = (currentMillis - lastSendMillis) > (unsigned long)SEND_TO_HOST_INTERVALL_IN_MILLIS;
    if ((unsigned long)0 == lastSendMillis || isIntervalPassed) {
        lastSendMillis = currentMillis;

        Serial.print("Sensor (1) ");
        Serial.print(averageValues(&tempRingBuf_1), 1);
        Serial.println(" Grad Celsius");

        Serial.print("Sensor (2) ");
        Serial.print(averageValues(&tempRingBuf_2), 1);
        Serial.println(" Grad Celsius");

        Serial.println("");
    }
}

void adjustFanSpeed() {
    float lastIntervalTempAverage_1 = averageValues(&tempRingBuf_1);
    float lastIntervalTempAverage_2 = averageValues(&tempRingBuf_2);
    if (lastIntervalTempAverage_1 > SENSOR_1_TEMP_THRESHOLD_in_CELCIUS
       || lastIntervalTempAverage_2 > SENSOR_2_TEMP_THRESHOLD_in_CELCIUS) {
        analogWrite(FAN_PWM_PIN, 255);
    } else {
        analogWrite(FAN_PWM_PIN, 0);
    }
}

void recordSensorMeasurements() {
    unsigned long currentMillis = millis();
    bool isIntervalPassed = (currentMillis - lastRecordedMillis) > ((unsigned long)AVERAGE_INTERVAL_IN_MILLIS / (unsigned long)RING_BUFFER_SIZE);
    if ((unsigned long)0 == lastRecordedMillis || isIntervalPassed) {
        lastRecordedMillis = currentMillis;
        buffer_write(&tempRingBuf_1, readTemperatureInCelcius(LM35_1_PIN));
        buffer_write(&tempRingBuf_2, readTemperatureInCelcius(LM35_2_PIN));
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
    recordSensorMeasurements();
    adjustFanSpeed();
    if (SEND_TEMP_VALUES_TO_HOST) {
        sendValuesToHost();
    }
}
