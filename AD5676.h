#ifndef __ARDUINO_AD5676
#define __ARDUINO_AD5676

#include <Arduino.h>
#include <SPI.h>

#define AD5676NumberOfChannels 8
#define AD5676VrefInternal 2.5f
#define AD5676VrefDefault 5.0f

class AD5676
{
	public:
		enum class commandName{WriteToReg,UpdateDACFromReg,WriteToDAC,PowerUpDown,SoftReset,SetDCEN,SetReadback,UpdateAllReg,UpdateAllDAC};
		AD5676(uint8_t CSSelectPin);
		AD5676(uint8_t CSSelectPin, float InputVref);
		void Set(uint8_t Channel, uint16_t NewSetting);
		void Set(uint8_t Channel, float NewVoltage);
		void SetAll(uint16_t NewSetting);
		void SetAll(float NewVoltage);
		void SetVref(float NewVref);
		uint16_t GetSetting(uint8_t Channel);
		float GetVoltage(uint8_t Channel);
		float GetVref();
		SPISettings* GetSPISettings();
	private:
		SPISettings ConnectionSettings;
		uint8_t GetCommandAddressByte(commandName InputCommand, uint8_t Address);
		void SetSPIData(commandName InputCommand, uint8_t Address, uint16_t Data);
		void SendSPIData();
		uint8_t CommandByte;
		uint8_t MSBByte;
		uint8_t LSBByte;
		float Vref;
		uint16_t Setting[AD5676NumberOfChannels];
		uint8_t SelectPin;
};
#endif