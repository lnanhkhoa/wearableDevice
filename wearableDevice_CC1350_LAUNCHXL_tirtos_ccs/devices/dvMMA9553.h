
#ifndef __DVMMA9553_H__
#define __DVMMA9553_H__

#define MMA9553_Slave_Addr  0x4C
#define MMA9553_Sub_Addr    0x00

class pedometer{
public:
    pedometer();
    ~pedometer();
    void init(void);
    void reset(void);
    void enable(void);
    void disable(void);
    void active(void);
    void write_config(void);
    void afe_config(void);
    void afe_config_read(void);
    void wakeup(void);
    void cmd_readwakeup(void);
    void int0_enable(void);
    void cmd_readstatus(void);
    void cmd_readconfig(void);
    void cmd_readrawxyz(void);
    void cmd_readlpfxyz(void);
    void Read(uint8_t *buffer, uint8_t len);
    void Write(uint8_t *buffer, uint8_t len);
private:
    void dvMMA9553_Read(uint8_t deviceAddr, uint8_t regAddr, uint8_t *data, uint8_t len);
    void dvMMA9553_Write(uint8_t deviceAddr, uint8_t regAddr, uint8_t *data, uint8_t len);
    void delay_mma9553(void);
};

#endif
