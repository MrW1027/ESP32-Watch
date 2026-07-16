#include "my_max30102.h"

MAX30105 particleSensor;

#define MAX_BRIGHTNESS 255

#define MAX30102_SDA 48
#define MAX30102_SCL 47

uint32_t irBuffer[100];  // infrared LED sensor data
uint32_t redBuffer[100]; // red LED sensor data

int32_t bufferLength;  // data length
int32_t spo2;          // SPO2 value
int8_t validSPO2;      // indicator to show if the SPO2 calculation is valid
int32_t heartRate;     // heart rate value
int8_t validHeartRate; // indicator to show if the heart rate calculation is valid

int32_t my_spo2=0;
int32_t my_heartreat=0;


void my_max30102_init()
{
    Wire1.begin(MAX30102_SDA, MAX30102_SCL);
    if (!particleSensor.begin(Wire1, I2C_SPEED_FAST)) // Use default I2C port, 400kHz speed
    {
        Serial.println(F("MAX30105 was not found. Please check wiring/power."));
        while (1) ;
    }
    byte ledBrightness = 80; // Options: 0=Off to 255=50mA
    byte sampleAverage = 4;  // Options: 1, 2, 4, 8, 16, 32
    byte ledMode = 2;        // Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
    byte sampleRate = 100;   // Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
    int pulseWidth = 411;    // Options: 69, 118, 215, 411
    int adcRange = 4096;     // Options: 2048, 4096, 8192, 16384

    particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); // Configure sensor with these settings
    Serial.println(F("MAX30105 set done"));
}

void my_max30102_run()
{
    bufferLength = 100; // buffer length of 100 stores 4 seconds of samples running at 25sps

    // read the first 100 samples, and determine the signal range
    for (byte i = 0; i < bufferLength; i++)
    {
        while (particleSensor.available() == false) // do we have new data?
            particleSensor.check();                 // Check the sensor for new data

        redBuffer[i] = particleSensor.getRed();
        irBuffer[i] = particleSensor.getIR();
        particleSensor.nextSample(); // We're finished with this sample so move to next sample

        Serial.print(F("red="));
        Serial.print(redBuffer[i], DEC);
        Serial.print(F(", ir="));
        Serial.println(irBuffer[i], DEC);
    }

    // calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

    // Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
    while (1)
    {
        // dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
        for (byte i = 25; i < 100; i++)
        {
            redBuffer[i - 25] = redBuffer[i];
            irBuffer[i - 25] = irBuffer[i];
        }

        // take 25 sets of samples before calculating the heart rate.
        for (byte i = 75; i < 100; i++)
        {
            while (particleSensor.available() == false) // do we have new data?
                particleSensor.check();                 // Check the sensor for new data

            redBuffer[i] = particleSensor.getRed();
            irBuffer[i] = particleSensor.getIR();
            particleSensor.nextSample(); // We're finished with this sample so move to next sample

            // // send samples and calculation result to terminal program through UART
            // Serial.print(F("red="));
            // Serial.print(redBuffer[i], DEC);
            // Serial.print(F(", ir="));
            // Serial.print(irBuffer[i], DEC);

            // Serial.print(F(", HR="));
            // Serial.print(heartRate, DEC);

            // Serial.print(F(", HRvalid="));
            // Serial.print(validHeartRate, DEC);

            // Serial.print(F(", SPO2="));
            // Serial.print(spo2, DEC);

            // Serial.print(F(", SPO2Valid="));
            // Serial.println(validSPO2, DEC);
        }

        // After gathering 25 new samples recalculate HR and SP02
        maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
        
        if(validHeartRate==1&&heartRate>50&&heartRate<150)
        {
            my_heartreat=heartRate;
            Serial.printf("heartRate:%d\n",my_heartreat);
        }
        if(validSPO2==1&&spo2>40&&spo2<=100)
        {
            my_spo2=spo2;
            Serial.printf("spo2:%d\n",my_spo2);
        }
        delay(10);
    }
}
void max30102_task(void* para)
{
    my_max30102_init();
    my_max30102_run();
}
int32_t get_heart_rate()
{
    return my_heartreat;
}
int32_t get_spo2()
{
    return my_spo2;
}
