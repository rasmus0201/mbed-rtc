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
        
        private:
            /**
             * @brief Worker thread
             * 
             */
            void Worker();
            
        private:
            RtcErrors error = Bundsgaard::RtcErrors::NO_ERROR;
            Thread _thread;
            NetworkInterface *net;
            time_t ntpTime = 0;
            int syncInterval;
    };
}

#endif
