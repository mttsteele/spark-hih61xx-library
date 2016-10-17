/*
Particle/Photon HIH81XX Temperature/Humidity Sensor Library
By: Geert wille - Appstrakt
Updated: Matthew Steele
Licence: GPL v2
*/
#ifndef HONEYWELL_HIH81XX_H
#define HONEYWELL_HIH81XX_H

#include "application.h"

#define HIH81XX_DEFAULT_ADDRESS     0x27

class HIH81XX {
    public:
        enum Error {
            NoError = 0,
            ConnectionError = 1,
            CommunicationError = 2,
            NotRunningError = 3,
            CommandModeError = 4,
            LoopTimeout = 5,
            ShitValue = 6,
            ErrorMask = 15
        };

        enum Flag {
            RunningFlag = 128,
            CommandModeFlag = 64,
            FlagsMask = ~ErrorMask
        };

        HIH61XX(uint8_t address = HIH61XX_DEFAULT_ADDRESS, TwoWire& MyWire = Wire, uint8_t powerPin = 255);

        uint8_t address() const {
            return a;
        }
        uint8_t powerPin() const {
            return p;
        }

        uint8_t error() const {
            return f & ErrorMask;
        }

        bool isRunning() const {
            return f & RunningFlag;
        }
        uint8_t flags() const {
            return f & FlagsMask;
        }

        //  return humidity / temperature
        float humidity() const {
            return rawToHumidity(h);
        }
        float temperature() const {
            return rawToTemperature(t);
        }
        uint16_t humidity_Raw() const {
            return h;
        }
        uint16_t temperature_Raw() const {
            return t;
        }

        //  start/stop the device
        virtual uint8_t start();
        virtual uint8_t stop();

        // update humidity / temperature
        uint8_t update();

        //  convert humidity / temperature
        static float rawToHumidity(uint16_t raw) {
            return float(raw) / 16382;
        }
        static float rawToTemperature(uint16_t raw) {
            return (float(raw) / 16382) * 165 - 40;
        }

        //  use a stream to control the sensor
        uint8_t commandRequest(Stream & stream);
    protected:
        uint8_t setError(uint8_t error) {
            f = (f & ~ErrorMask) | error;
            return error;
        }

        virtual uint8_t commandProcess(Stream & stream, uint8_t command);
        uint8_t commandReply(Stream & stream, uint8_t result) {
            stream.println(result);
            return result;
        }
        template < typename T > uint8_t commandReply(Stream & stream, uint8_t result,
            const T & data) {
            stream.println(data);
            return result;
        }
        uint8_t a;
        TwoWire& w;
        uint8_t p;
        uint8_t f;
        uint16_t h;
        uint16_t t;
};

#endif
