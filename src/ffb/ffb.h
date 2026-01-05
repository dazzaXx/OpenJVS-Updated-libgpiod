#ifndef FFB_H_
#define FFB_H_

#include <pthread.h>
#include <stdint.h>

#define FFB_MAX_EFFECTS 16
#define FFB_COMMAND_QUEUE_SIZE 64

typedef enum
{
    FFB_STATUS_SUCCESS,
    FFB_STATUS_ERROR,
    FFB_STATUS_ERROR_CONTROLLED_ALREADY_BOUND
} FFBStatus;

typedef enum
{
    FFB_EMULATION_TYPE_SEGA,
    FFB_EMULATION_TYPE_NAMCO
} FFBEmulationType;

typedef enum
{
    FFB_COMMAND_TYPE_CONSTANT,
    FFB_COMMAND_TYPE_SPRING,
    FFB_COMMAND_TYPE_DAMPER,
    FFB_COMMAND_TYPE_RUMBLE,
    FFB_COMMAND_TYPE_STOP_ALL
} FFBCommandType;

typedef struct
{
    FFBCommandType type;
    int direction;      // Direction in degrees (0-359) or left/right for rumble
    int strength;       // Strength 0-255
    int duration;       // Duration in milliseconds (0 = infinite)
    int leftMagnitude;  // For rumble: left motor strength
    int rightMagnitude; // For rumble: right motor strength
} FFBCommand;

typedef struct
{
    FFBCommand commands[FFB_COMMAND_QUEUE_SIZE];
    int head;
    int tail;
    int count;
    pthread_mutex_t mutex;
} FFBCommandQueue;

typedef struct
{
    FFBEmulationType type;
    int controller;
    int serial;
    pthread_t threadID;
    
    // Event device for force feedback
    int eventFd;
    char eventPath[256];
    
    // FFB capabilities
    int hasFFB;
    int hasConstant;
    int hasSpring;
    int hasDamper;
    int hasRumble;
    
    // Effect management
    int effectIds[FFB_MAX_EFFECTS];
    int effectCount;
    
    // Command queue
    FFBCommandQueue commandQueue;
    
    // Emulation for non-FFB controllers
    int emulationMode;           // 1 if emulating, 0 if real FFB
    long lastCommandTime;        // Timestamp of last command (milliseconds)
    int currentPosition;         // Emulated motor position (-100 to 100)
    int targetPosition;          // Target position from last command
    int motorStatus;             // Motor status flags
    unsigned char lastCommand[16]; // Last FFB command received
    int lastCommandLength;       // Length of last command
} FFBState;

FFBStatus initFFB(FFBState *state, FFBEmulationType type, char *serialPath);
FFBStatus bindController(FFBState *state, int controller);
FFBStatus closeFFB(FFBState *state);
FFBStatus queueFFBCommand(FFBState *state, FFBCommand *command);

// Emulation functions
void updateEmulatedPosition(FFBState *state);
int getEmulatedStatus(FFBState *state, unsigned char *response, int maxLen);
void trackFFBCommand(FFBState *state, const unsigned char *data, int length);

#endif // FFB_H_
