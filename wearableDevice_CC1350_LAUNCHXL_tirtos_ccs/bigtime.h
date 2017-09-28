




#ifndef __BIGTIME_H__
#define __BIGTIME_H__

class Clock {
    private:
         // data
         int id;
         int microsecond;
         int millisecond;
         int second;
         int minute;
         int hour;
         int day;
         int month;
         int year;
         int century;
         int millenium;

    public:
        // methods
        Clock(int newId);  // Constructor
        ~Clock();          // Destructor

        void TimerDim(uint8_t count);
        int getId();
        int getMicrosecond();
        int getMillisecond();
        int getSecond();
        int getMinute();
        int getHour();
        int getDay();
        int getMonth();
        int getYear();
        int getCentury();
        int getMillenium();
        void setMicrosecond();
        void setMillisecond();
        void setMillisecond(int nMilliseconds);
        void setSecond();
        void setMinute();
        void setHour();
        void setDay();
        void setMonth();
        void setYear();
        void setCentury();
        void setMillenium();
};

#endif
