#ifndef I2C160502
#define I2C160502

struct i2c;

typedef struct i2c i2c_t;

int i2c_sz();

void i2c_init(i2c_t * o, int port, unsigned freq, int timeout);

int i2c_start(i2c_t * o);

int i2c_stop(i2c_t * o);

void i2c_write(i2c_t * o, char byte);

int i2c_read(i2c_t * o);

#endif
