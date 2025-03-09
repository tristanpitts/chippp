#include "Chip8.h"
#include <cstddef>
#include <fstream>
#include <cstring>

Chip8::Chip8()
{
  PC = PROGRAM_BASE_ADDRESS;
  I = 0;
  SP = 0;
  memset(Registers, 0, 16);
  memset(Memory, 0, 4096);

}

Chip8::~Chip8()
{

}

int Chip8::init(std::string filePath)
{
  std::ifstream rom(filePath, std::ios::in | std::ios::binary);
  if (rom.is_open())
  {
    std::string fileContent((std::istreambuf_iterator<char>(rom)), std::istreambuf_iterator<char>());
    std::memcpy(&Memory[PROGRAM_BASE_ADDRESS], &fileContent[0], fileContent.size());
    rom.close();
    if(memcmp(&fileContent[0], &Memory[PROGRAM_BASE_ADDRESS], fileContent.size()) != 0)
    {
      std::cout<<"Error loading rom"<<std::endl;
      return 1;
    }
  }

  return 0;
}

void Chip8::step()
{
  //printf("%X%X\n", Memory[PC],Memory[PC+1]) ;

  uint8_t instruction[4] =
  {
    (Memory[PC+1] & 0x0F),
    (Memory[PC+1] & 0xF0) >> 4,
    (Memory[PC] & 0x0F),
    (Memory[PC] & 0xF0) >> 4
  };

  printf("%X%X%X%X, ", instruction[3], instruction[2], instruction[1], instruction[0]);

  switch(instruction[3])
  {
    case 0x0:
    {
        switch(instruction[1] << 4 | instruction[0])
        {
            case 0xE0:
            {
                std::cout<<"Clear Screen"<<std::endl;
                break;
            }
            case 0xEE:
            {
                std::cout<<"Return from subroutine"<<std::endl;
                break;
            }
            default:
            {
                break;
            }
        }
    }

    case 0x1:
    {
        PC = instruction[2] << 8 | instruction[1] << 4 | instruction[0];
        std::cout<<"Jump to " << std::hex << PC <<std::endl;
        break;
    }

    case 0x2:
    {
        uint16_t addr = instruction[2] << 8 | instruction[1] << 4 | instruction[0];
        std::cout << "Call Subroutine at " << std::hex << addr << std::endl;
        break;
    }

    case 0x3:
    {
        uint8_t value = instruction[1] << 4 | instruction[0];
        printf("Skip next instruction if register %X == %X\n", instruction[2], value);
        if(Registers[instruction[2]] == value)
        {
          PC+=4;
        }
        else
        {
          PC+=2;
        }
        break;
    }

    case 0x4:
    {
        uint8_t value = instruction[1] << 4 | instruction[0];
        printf("Skip next instruction if register %X != %X\n", instruction[2], value);
        if(Registers[instruction[2]] != value)
        {
          PC+=4;
        }
        else
        {
          PC+=2;
        }
        break;
    }

    case 0x5:
    {
        printf("Skip next instruction if register %X != register %X\n", instruction[2], instruction[1]);
        if(Registers[instruction[2]] != Registers[instruction[1]])
        {
          PC+=4;
        }
        else
        {
          PC+=2;
        }
        break;
    }

    case 0x6:
    {
        uint8_t value = instruction[1] << 4 | instruction[0];
        printf("Set register %X to %X\n", instruction[2], value);
        Registers[instruction[2]] = value;
        PC+=2;
        break;
    }

    case 0x7:
    {
        uint8_t value = instruction[1] << 4 | instruction[0];
        printf("Set register %X = register %X + %X\n", instruction[2], instruction[2], value);
        Registers[instruction[2]]+=value;
        PC+=2;
        break;
    }

    case 0x8:
    {
        printf("register operations, todo\n");
        break;
    }

    default:
    {
        printf("Unimplemented or unknown\n");
    }
  }

  PC+=2;
}
