#include "AD5676.h"

AD5676::AD5676(uint8_t CSSelectPin, float InputVref)
{
	Vref = InputVref;
	SelectPin = CSSelectPin;
	pinMode(SelectPin, OUTPUT);
	digitalWrite(SelectPin, HIGH);
	ConnectionSettings = SPISettings(1000000, MSBFIRST, SPI_MODE2);
	for (uint8_t Index = 0; Index < AD5676NumberOfChannels; Index++)
	{
		Setting[Index] = 0;
	}
}

AD5676::AD5676(uint8_t CSSelectPin) : AD5676::AD5676(CSSelectPin,AD5676VrefDefault)
{
	
}

void AD5676::SetSPIData(commandName InputCommand, uint8_t Address, uint16_t Data)
{
	CommandByte = Address;
	if (CommandByte > 7)
	{
		CommandByte = 0;
	}
	switch( InputCommand )
	{
		case commandName::WriteToReg:
			CommandByte = CommandByte | B00010000;
			break;
		case commandName::UpdateDACFromReg:
			CommandByte = CommandByte | B00100000;
			break;
		case commandName::WriteToDAC:
			CommandByte = CommandByte | B00110000;
			break;
		case commandName::PowerUpDown:
			CommandByte = CommandByte | B01000000;
			break;
		case commandName::SoftReset:
			CommandByte = CommandByte | B01100000;
			break;
		case commandName::SetDCEN:
			CommandByte = CommandByte | B10000000;
			break;
		case commandName::SetReadback:
			CommandByte = CommandByte | B10010000;
			break;
		case commandName::UpdateAllReg:
			CommandByte = CommandByte | B10100000;
			break;
		case commandName::UpdateAllDAC:
			CommandByte = CommandByte | B10110000;
			break;
		default:
			break;
	}
	MSBByte = (uint8_t)(Data>>8);
	LSBByte = (uint8_t)(Data);
}

void AD5676::SendSPIData()
{
	SPI.beginTransaction(ConnectionSettings);
	SPI.transfer( 0 );
	//Serial.print(SelectPin);
	//Serial.print(":");
	//Serial.print(CommandByte,DEC);
	//Serial.print(":");
	//Serial.print(MSBByte,DEC);
	//Serial.print(":");
	//Serial.print(LSBByte,DEC);
	//Serial.println(";");
	digitalWrite(SelectPin,LOW);
	SPI.transfer( CommandByte );
	SPI.transfer( MSBByte );
	SPI.transfer( LSBByte );
	SPI.endTransaction();
	digitalWrite(SelectPin, HIGH);
}

void AD5676::Set(uint8_t Channel, uint16_t NewSetting)
{
	Setting[Channel] = NewSetting;
	SetSPIData(commandName::WriteToDAC, Channel, NewSetting);
	SendSPIData();
}

void AD5676::Set(uint8_t Channel, float NewVoltage)
{
	uint16_t NewSetting = (uint16_t)(NewVoltage / Vref * 65535);
	Set(Channel, NewSetting);
}

void AD5676::SetAll(uint16_t NewSetting)
{
	for (uint8_t Index = 0; Index < AD5676NumberOfChannels; Index++)
	{
		Setting[Index] = NewSetting;
	}
	SetSPIData(commandName::UpdateAllDAC,0,NewSetting);
	SendSPIData();
}

void AD5676::SetAll(float NewVoltage)
{
	uint16_t NewSetting = (uint16_t)(NewVoltage / Vref * 65535);
	SetAll(NewSetting);
}

void AD5676::SetVref(float NewVref)
{
	Vref = NewVref;
}

uint16_t AD5676::GetSetting(uint8_t Channel)
{
	return Setting[Channel];
}

float AD5676::GetVoltage(uint8_t Channel)
{
	return Vref * (float)(GetSetting(Channel)) / 65535.0;
}

float AD5676::GetVref()
{
	return Vref;
}

SPISettings* AD5676::GetSPISettings()
{
	return &ConnectionSettings;
}