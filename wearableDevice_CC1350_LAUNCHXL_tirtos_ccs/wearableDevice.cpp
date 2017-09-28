/*
 *    ======== wearableDevice.cpp ========
 */

#include "wearableDevice.h"
#include <stddef.h>
#include <unistd.h>
#include <cstring>

#include <ti/sysbios/BIOS.h>
#include <time.h>
#include <ti/sysbios/hal/Seconds.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Event.h>



/* Board Header files */
#include "Board.h"

/* Driver Header files */
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/PIN.h>
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>

/* Library Header files */
#include "bigtime.h"

/* Devices Header files */
#include "devices/dvMMA9553.h"
//#include "devices/heartrate_3_click.h"
//#include "SimpleKalmanFilter/SimpleKalmanFilter.h"
#include <OledDisplay/WDsDisplay.h>

//#define EVENT_INVALID_PACKET_RECEIVED (uint32_t)(1 << 1)



#define TASKSTACKSIZE           1024
#define MAINTASK_PRIORITY       4
#define PRIMARYTASK_PRIORITY    3
#define COUNTSLEEPTASK_PRIORITY 2

#define Display_ON true
#define Display_OFF false
// #define INACTIVE_PRIORITY       -1
// #define HIGH_PRIORITY           4

/***** Variable declarations *****/
static Task_Params mainTaskParams;
static Task_Params sleepModeTaskParams, countSleepTaskParams;
Task_Struct mainTaskStruct;
static uint8_t mainTaskStack[TASKSTACKSIZE];
Task_Handle mainTaskHandle;

Task_Struct primaryTaskStruct;
static uint8_t primaryTaskStack[TASKSTACKSIZE];
Task_Handle primaryTaskHandle;


Task_Struct countSleepTaskStruct;
static uint8_t countSleepTaskStack[TASKSTACKSIZE];

Clock_Struct mainClockStruct;
static Clock_Handle mainClockHandle;

Event_Struct operationEvent;
static Event_Handle operationEventHandle;

Event_Struct operationEvent1;
static Event_Handle operationEventHandle1;

/* Semaphore used to gate for shutdown */
Semaphore_Struct sem0Struct;
Semaphore_Handle sem0Handle;

Semaphore_Struct sem1Struct;
Semaphore_Handle sem1Handle;


uint8_t modeMain = 0;
bool onoff = true;
volatile UInt32 timestamp = 0;
volatile uint16_t heartRateNumber = 0;
bool taskExitIsSet = false, enableSleep = false, stateDisplay = Display_ON ;

pedometer Pedometer;
I2C_Handle i2c;
I2C_Params i2cParams;
I2C_Transaction i2cTransaction;

static Display_Handle display;
WDsDisplay displayOLED(OLED_RESET);
Clock clk(1);

extern "C" {

//void display_head(void);
//void clock_main(void);
//void heart_main(uint8_t rate);
//void pedometer_show(int16_t stepCount);
//int16_t pedometer_measurement(void);
//void pedometer_main(void);
//void checkDimOLED(uint8_t time);

} // end extern "C"


/*
 *  ======== gpioButtonFxn0 ========
 *  Callback function for the GPIO interrupt on Board_GPIO_BUTTON0.
 */
void gpioButtonFxn0(uint_least8_t index){

    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(Board_GPIO_LED0);
    enableSleep = false;
    if(stateDisplay == Display_OFF){
        modeMain=2;
        Event_post(operationEventHandle, EVENT_TURNON_DISPLAY | EVENT_UPDATE_OLED);
        // Event_post(operationEventHandle, EVENT_UPDATE_OLED);
    }
    modeMain++;
    modeMain%=3;

    if(modeMain == 1){
        heartRateNumber = 0;
        taskExitIsSet = false;
        Semaphore_post(sem0Handle);
    }
    else{
        taskExitIsSet = true;
    }
}

/*
 *  ======== gpioButtonFxn1 ========
 *  Callback function for the GPIO interrupt on Board_GPIO_BUTTON1.
 *  This may not be used for all boards.
 */
void gpioButtonFxn1(uint_least8_t index){
//    GPIO_toggle(Board_GPIO_LED1);
   // taskManager1.create_repeatThread(5);
   // seconds++;
   // if(seconds>59) seconds=0;
}

void display_head(void){
    displayOLED.Clear_head();
    displayOLED.setTextSize(1);
    displayOLED.setTextColor(WHITE);
    displayOLED.setCursor(40, 8);
    displayOLED.print((uint8_t*)elementHead.text);
    displayOLED.Bluetooth_icon(elementHead.bleIcon);
    displayOLED.Battery_set(elementHead.batteryLevel);
    eventChange.head = true;
}


void clock_main(uint32_t secs){
    static UInt32 seconds = 10; // different to Second_get() in initial running
    if(seconds != secs){
        seconds = secs;
        clk.setSecond();
        uint8_t hour = clk.getHour();
        uint8_t min = clk.getMinute();
        displayOLED.Clear_body();
        displayOLED.Clock_set(hour, min);
        displayOLED.Colon_toogle();
        eventChange.body = true;

        if(!enableSleep){
            Semaphore_post(sem1Handle);
            enableSleep = true;
        }

    }
}


void heart_show(uint8_t rate){
    static uint8_t _rate;
    displayOLED.Clear_body();
    displayOLED.Heartrate_status();
    if(_rate != rate){
        _rate = rate;
    }
    displayOLED.Heartrate_number(_rate);
    eventChange.body = true;
}

int16_t heart_measurement(uint16_t _next){
    static uint16_t _heartRate = _next;
    uint32_t count = 2000000;
    while(count--);
    _heartRate = Seconds_get()%100;
    return _heartRate;

}

void heart_main(){
    //Start measure heart rate
    heart_show(heartRateNumber);
}


void pedometer_show(int16_t stepCount) {
    static uint8_t _stepCount;
    displayOLED.Clear_body();
    displayOLED.Footsteps_status();
    if(_stepCount != stepCount){
        _stepCount = stepCount;
    }
    displayOLED.Footsteps_number(_stepCount);
    eventChange.body = true;
}


int16_t pedometer_measurement(){
    uint8_t Buf[20];
    static int16_t StepCount;
    Pedometer.cmd_readstatus();
    Pedometer.Read(Buf, 2);
    if (Buf[1] == 0x80)
    {
        Pedometer.Read(Buf, 16);
        StepCount = (Buf[6] <<8) | Buf[7];
    }
    return StepCount;
}

void pedometer_main(){
    int16_t steps;
    steps = pedometer_measurement();
    pedometer_show(steps);
    if(!enableSleep){
        Semaphore_post(sem1Handle);
        enableSleep = true;
    }
}

void checkDimOLED(uint8_t time){
    static uint8_t _time = time;
}


class hienthi{
public:
    hienthi(){
        displayOLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
        displayOLED.clearDisplay();
        displayOLED.display();
    }

    void showHead(void){
        display_head();
        if(eventChange.head){
            displayOLED.Display_head();
            eventChange.head = false;
        }
    }

    /*
     * Mode 0: Clock
     * Mode 1: StepCount
     *
     * */
    void showBody(uint8_t mode){
        switch(mode){
            case 0:  clock_main(timestamp); break;
            case 1:  heart_main(); break;
            case 2:  pedometer_main(); break;
            case 3:  break;
            default: break;
        }
        if(eventChange.body){
            displayOLED.Display_body();
            eventChange.body = false;
        }
    }

    void TurnONOFFDisplay(bool turn){
        displayOLED.onoff(turn);
        onoff = turn;
    }
};




/*
 *  ======== mainTaskStructFunction ========
 */
static void mainTaskStructFunction(UArg arg0, UArg arg1){
    /* Open GPIO for usage */
    GPIO_init();
    GPIO_PinConfig pinConfig = Board_GPIO_BUTTON0 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING;
    GPIO_setConfig(Board_GPIO_BUTTON0, pinConfig);
    /* install Button callback and Enable interrupts */
    GPIO_setCallback(Board_GPIO_BUTTON0, gpioButtonFxn0);
    GPIO_setCallback(Board_GPIO_BUTTON1, gpioButtonFxn1);
    GPIO_enableInt(Board_GPIO_BUTTON0);
    GPIO_enableInt(Board_GPIO_BUTTON1);
    
    /* Open the HOST display for output */
    Display_init();
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) while (1);
    Display_printf(display, 0, 0, "Starting the i2c connect device \n");

    /* Create I2C for usage */
    I2C_init();
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2c = I2C_open(Board_I2C_TMP, &i2cParams);
    if (i2c == NULL){
        Display_printf(display, 0, 0, "Error Initializing I2C\n");
        while (1);
    }else
        Display_printf(display, 0, 0, "I2C Initialized!\n");

    Pedometer.init();
    hienthi HienThi;
    elementHead.bleIcon = true;
    std::strcpy( elementHead.text, "MEMSITECH");
    elementHead.batteryLevel = 50;

    while(1){
        /* Wait for event */

      // uint32_t events1 = Event_pend(operationEventHandle1, 0, EVENT_ALL, BIOS_NO_WAIT);
       uint32_t events = Event_pend(operationEventHandle, 0, EVENT_ALL, BIOS_WAIT_FOREVER);

      if(events & EVENT_SLEEPMODE_COMPLETE){
           HienThi.TurnONOFFDisplay(false);
           stateDisplay = Display_OFF;
           Clock_stop(mainClockHandle);
//           Semaphore_reset(sem0Handle,0);
//           Semaphore_reset(sem1Handle,0);
       }

      if(events & EVENT_TURNON_DISPLAY){
          HienThi.TurnONOFFDisplay(true);
          stateDisplay = Display_ON;
          Clock_start(mainClockHandle);
      }

         if(events & EVENT_UPDATE_OLED && stateDisplay == Display_ON){
            HienThi.showHead();
            HienThi.showBody(modeMain);
           // Event_post(operationEventHandle1, EVENT_SLEEPMODE_START);
        }
    }
}

void countSleepTaskStructFunction(UArg arg0, UArg arg1){
    while(true){
        Semaphore_pend(sem1Handle, BIOS_WAIT_FOREVER);
        for(int i=0; i<50; i++){
            if (!enableSleep) break;
            Task_sleep(10000);
            if(i==49)
                Event_post(operationEventHandle, EVENT_SLEEPMODE_COMPLETE);
        }
    }
}

void mainClockTaskStructFunction(UArg arg0, UArg arg1){
    UInt32 _time = Seconds_get();
    timestamp = _time;
    Event_post(operationEventHandle, EVENT_UPDATE_OLED);

}

void primaryTaskStructFunction(UArg arg0, UArg arg1){
    static uint16_t _temp = 50;
    while(true){
        if (Semaphore_pend(sem0Handle, BIOS_WAIT_FOREVER))
        {
          if (taskExitIsSet) continue;
          // do something in 1sec
          _temp = heart_measurement(_temp);
          if (taskExitIsSet) continue;
          // do something in 1sec
          _temp = heart_measurement(_temp);
          if (taskExitIsSet) continue;
          // do something in 1sec
          _temp = heart_measurement(_temp);
          if (taskExitIsSet) continue;
          // do something in 1sec
          _temp = heart_measurement(_temp);
          if (taskExitIsSet) continue;
          // do something in 1sec
          _temp = heart_measurement(_temp);
          if (taskExitIsSet) continue;
          // do something in 1sec
          heartRateNumber = heart_measurement(_temp);
        }
        if(!enableSleep){
            Semaphore_post(sem1Handle);
            enableSleep = true;
        }
    }
}

void sleepModeTaskStructFunction(UArg arg0, UArg arg1){

}


void wearableDevice_init(){

    
    /* Create event used internally for state changes */
    Event_Params eventParam;
    Event_Params_init(&eventParam);
    Event_construct(&operationEvent, &eventParam);
    operationEventHandle = Event_handle(&operationEvent);
    
    Event_Params eventParam1;
    Event_Params_init(&eventParam1);
    Event_construct(&operationEvent1, &eventParam1);
    operationEventHandle1 = Event_handle(&operationEvent1);
    
    /* Create the main task */
    Task_Params_init(&mainTaskParams);
    mainTaskParams.stackSize = TASKSTACKSIZE;
    mainTaskParams.priority = MAINTASK_PRIORITY;
    mainTaskParams.stack = &mainTaskStack;
    Task_construct(&mainTaskStruct, mainTaskStructFunction, &mainTaskParams, NULL);
    mainTaskHandle = Task_Handle(&mainTaskStruct);

    mainTaskParams.priority = PRIMARYTASK_PRIORITY;
    mainTaskParams.stack = &primaryTaskStack;
    Task_construct(&primaryTaskStruct, primaryTaskStructFunction, &mainTaskParams, NULL);    
    primaryTaskHandle = Task_Handle(&primaryTaskStruct);
   
    /* Create the count sleep task */ 
    Task_Params_init(&countSleepTaskParams);
    countSleepTaskParams.stackSize = TASKSTACKSIZE;
    countSleepTaskParams.priority = COUNTSLEEPTASK_PRIORITY;
    countSleepTaskParams.stack = &countSleepTaskStack;
    countSleepTaskParams.arg0 = (UArg)5;
    Task_construct(&countSleepTaskStruct, countSleepTaskStructFunction, &countSleepTaskParams, NULL);

    /* Create clock object which is used for ble bootload button check */
    Clock_Params clkParams;
    Clock_Params_init(&clkParams);
    clkParams.period = 20000;
   // clkParams.arg = (UArg)&HienThi;
    clkParams.startFlag = true;
    Clock_construct(&mainClockStruct, (Clock_FuncPtr)mainClockTaskStructFunction, 1, &clkParams);
    mainClockHandle = Clock_handle(&mainClockStruct);
    Clock_start(mainClockHandle);

    /* Create the sleep mode task */ 
    // Task_Params_init(&sleepModeTaskParams);
    // sleepModeTaskParams.stackSize = TASKSTACKSIZE;
    // sleepModeTaskParams.priority = MAINTASK_PRIORITY;
    // sleepModeTaskParams.stack = &sleepModeTaskStack;
    // Task_construct(&sleepModeTaskStruct, sleepModeTaskStructFunction, &sleepModeTaskParams, NULL);


     /* Configure shutdown semaphore. */
    Semaphore_Params semParams;
    Semaphore_Params_init(&semParams);
    Semaphore_construct(&sem0Struct, 0, &semParams);
    sem0Handle = Semaphore_handle(&sem0Struct);

    Semaphore_construct(&sem1Struct, 0, &semParams);
    sem1Handle = Semaphore_handle(&sem1Struct);

}
