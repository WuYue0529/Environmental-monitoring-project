
int BspReadEeprom(int reg, int len, unsigned char *buff);
int BspWriteEeprom(int reg, int len, unsigned char *buff);
bool burnBoardInfoToEeprom(void);

