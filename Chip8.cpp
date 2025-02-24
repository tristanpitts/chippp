#include "Chip8.h"
#include <cstddef>
#include <fstream>
#include <cstring>

Chip8::Chip8()
{
  PC = 512;
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
    std::memcpy(&Memory[512], &fileContent[0], fileContent.size());
    rom.close();
    if(memcmp(&fileContent[0], &Memory[512], fileContent.size()) != 0)
    {
      std::cout<<"Error loading rom"<<std::endl;
      return 1;
    }
  }

  return 0;
}

void Chip8::step()
{
  printf("%X%X ", Memory[PC+1], Memory[PC]);
  PC+=2;
}
