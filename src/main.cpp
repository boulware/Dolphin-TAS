#include <iostream>
#include <fstream>
#include <cstdint>
#include <array>
#include <vector>
#include <map>

uint8_t
Clamp(uint8_t N, uint8_t Lower, uint8_t Upper)
{
    if(Lower > Upper) return N;

    if(N < Lower) N = Lower;
    if(N > Upper) N = Upper;

    return N;
}

struct game
{
    // Bit-field: XXXX4321, where X is unused and 1-4 refer to the respective activated controller ports.    
    static uint8_t ControllerField;
    
    const uint8_t GetControllerCount();
    const uint8_t GetControllerOffset(uint8_t Port);
};

uint8_t game::ControllerField = 0;

const uint8_t game::GetControllerCount()
{
    uint8_t Count =
        ((ControllerField >> 0) & 0x1) +
        ((ControllerField >> 1) & 0x1) +
        ((ControllerField >> 2) & 0x1) +
        ((ControllerField >> 3) & 0x1);

    return Count;
}    

const uint8_t game::GetControllerOffset(uint8_t Port)
{
    uint8_t ControllerOffset = 0;
    
    for(int port = 0; port < 4; port++)
    {
        if(((ControllerField >> port) & 0x1) == 1)
        {
            ControllerOffset++;

            if(port == Port) return ControllerOffset;
        }
    }

    std::cout << "ERROR: Tried to get controller offset for empty port." << std::endl;
    return 0;
}

game Game;

struct trigger
{
    uint8_t Pressure = 0;
};

struct stick
{
    uint8_t HPosition = 128;
    uint8_t VPosition = 128;
};

class raw_header_data
{
private:
    uint8_t Data[256];
public:
    const uint8_t* GetByteAddress(uint8_t Byte)
    {
        Clamp(Byte, 0, 255);
        return Data+Byte;
    }
};

enum class button : uint8_t
{
    Start = 0,
    A,
    B,
    X,
    Y,
    Z,
    Up,
    Down,
    Left,
    Right,
    L,
    R,
};

struct controller_state
{
    uint8_t Digitals[2];
    uint8_t LPressure;
    uint8_t RPressure;
    uint8_t ControlStickX;
    uint8_t ControlStickY;
    uint8_t CStickX;
    uint8_t CStickY;
};

void SetButton(controller_state& ControllerState, button Button, bool Pressed)
{
    
}

struct input
{
    uint8_t Port;
    
    button Start;
    button A;
    button B;
    button X;
    button Y;
    button Z;
    button DPadUp;
    button DPadDown;
    button DPadLeft;
    button DPadRight;
    button DigitalL;
    button DigitalR;

    trigger L;
    trigger R;

    stick ControlStick;
    stick CStick;

    input(uint8_t Port) : Port(Port) {}
};

class routine
{
private:
    uint32_t FrameCount = 0;
    std::multimap<uint32_t, input> Inputs;
public:
    void Insert(input& Input, uint32_t Frame);
    void WriteRoutineToFile(raw_header_data& Header, std::string FileName);
};

void
routine::Insert(input& Input, uint32_t Frame)
{
    Inputs.insert(std::pair<uint32_t, input>(Frame, Input));

    if(Frame >= FrameCount) FrameCount = Frame + 1;
}

void
LoadHeaderFromDTM(std::string FileName, raw_header_data& Destination)
{
    std::ifstream ConfigFile("config.dtm", std::ios::in|std::ios::binary|std::ios::ate);
    if(ConfigFile.is_open())
    {
        std::streampos Size = ConfigFile.tellg();
        uint8_t* Memory = new uint8_t[Size];
        ConfigFile.seekg(0, std::ios::beg);
        ConfigFile.read((char*)Memory, Size);
        ConfigFile.close();

        memcpy(&Destination, Memory, 256);
    }
    else
    {
        std::cout << "ERROR: Failed to load header from config.dtm" << std::endl;
    }

}

int main()
{
    raw_header_data Header;
    LoadHeaderFromDTM("config.dtm", Header);

    Game.ControllerField = *Header.GetByteAddress(0xB);

    
//        *(RawHeader + 0xB);
    
//    std::streampos size;
//    uint8_t*

    routine MainRoutine;
      
    input Input0(0);
    Input0.CStick.HPosition = 1;

    input Input1(1);
    Input1.CStick.HPosition = 255;

    input Input2(2);
    Input2.CStick.VPosition = 1;

    input Input3(3);
    Input3.CStick.VPosition = 255;

    MainRoutine.Insert(Input0, 60);
    MainRoutine.Insert(Input1, 0);
    MainRoutine.Insert(Input2, 0);
    MainRoutine.Insert(Input3, 0);
    
//    void* Memory;
//    MainRoutine.WriteRoutineToFile(Header, "Routine.dtm");      

/*
        std::ofstream RoutineFile("Routine.dtm", std::ios::out|std::ios::binary);
    if(RoutineFile.is_open())
    {

//        std::vector<void>
//        uint8_t* ControllerData = Memory + 256; // Controller data is offset 256 bytes from beginning of file.
        
//        RoutineFile.write((char*)ControllerData, 
    }
    else
    {
        std::cout << "ERROR: Failed to open routine.dtm for reading." << std::endl;
        }*/
            /*
    std::ifstream infile("FD4Foxes.dtm", std::ios::in|std::ios::binary|std::ios::ate);
    if (infile.is_open())
    {
        size = infile.tellg();
        Memory = new uint8_t[size];
        infile.seekg (0, std::ios::beg);
        infile.read ((char*)Memory, size);
        infile.close();

        std::cout << "the entire file content is in memory";


        


        controller C0(0);
        C0.CStick.HPosition = 1;

        controller C1(1);
        C1.CStick.HPosition = 255;

        controller C2(2);
        C2.CStick.VPosition = 1;

        controller C3(3);
        C3.CStick.VPosition = 255;

        WriteControllerToFrame(C0, ControllerData, 0);
        WriteControllerToFrame(C1, ControllerData, 0);
        WriteControllerToFrame(C2, ControllerData, 0);
        WriteControllerToFrame(C3, ControllerData, 0);

        std::ofstream outfile("NewFile.dtm", std::ios::out|std::ios::binary|std::ios::trunc);

        outfile.write((char*)Memory, size);

        outfile.close();
        
        delete[] Memory;
    }
    else std::cout << "Unable to open file";
            */
    // std::cout << sizeof(raw_controller_data) << std::endl;

    std::cout << sizeof(controller_state) << std::endl;
    
    return 0;
}
