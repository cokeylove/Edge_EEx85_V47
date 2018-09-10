#ifndef OEM_Debug_H
#define OEM_Debug_H
#define BASE_MASK   0xFF00
#define LINE_MASK   0xFFF0

void Ec2iWrite (
  BYTE  RegOffset,
  BYTE  RegValue
);

BYTE Ec2iRead (
  BYTE  RegOffset
);

void DumpLpcReg (
  unsigned char Ldn
);

void DumpReg (
  unsigned short RegBase
);

void DumpAllReg (
  void
);

#endif
