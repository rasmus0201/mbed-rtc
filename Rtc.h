/**
 * @file Rtc.h
 * @author Rasmus Sørensen (bundsgaard.rasmus@gmail.com)
 * @brief Rtc definitions
 * @version 0.1
 * @date 2020-08-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef BUNDSGAARD_RTC_H
#define BUNDSGAARD_RTC_H

#import "mbed.h"

namespace Bundsgaard
{
    /**
     * @brief Error codes
     * 
     */
    enum RtcErrors
    {
        NO_ERROR = 0,
        NO_INTERFACE = 1,
        CONNECTION_FAILED = 2
    };

    /**
     * @brief Rtc class to sync with NTP server
     * 
     */
    class Rtc
    {
        public:
            /**
             * @brief Construct a new Rtc object
             *
             * @param syncInterval 
             */
            Rtc(int syncInterval);

            /**
             * @brief Destruct object
             * 
             */
            ~Rtc();

            /**
             * @brief Start the thread
             * 
             */
            void Start();

            /**
             * @brief Get last NTP time
             * 
             * @return time_t 
             */
            time_t GetNTPTime();

            /**
             * @brief Alias of time(NULL)
             * 
             * @return time_t 
             */
            time_t GetTime();

            /**
             * @brief Get the current UNIX timestamp 
             * 
             * @return unsigned int 
             */
            unsigned int GetTimestamp();

            /**
             * @brief Get the current UNIX timestamp in milliseconds 
             * 
             * @return uint64_t
             */
            uint64_t GetTimestampMS();
        
        private:
            /**
             * @brief Worker thread
             * 
             */
            void Worker();

            /**
             * @brief Tick
             */
            void Tick();
            
        private:
            RtcErrors error = Bundsgaard::RtcErrors::NO_ERROR;
            Ticker ticker;
            Thread thread;
            NetworkInterface *net;
            time_t ntpTime = 0;
            int syncInterval;
            int ms;
    };
}

#endif
