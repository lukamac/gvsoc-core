/*
 * Copyright (C) 2021  GreenWaves Technologies, SAS
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <functional>

#include "vp/itf/i2c.hpp"
#include "vp/component.hpp"
#include "vp/clock/clock_engine.hpp"
#include "vp/clock/clock_event.hpp"

typedef enum {
    MASTER_ACK, /*!< equivalent to OK for operations that have no ACK */
    MASTER_NACK,
    MASTER_ERROR_ARBITRATION,
    ANY_ERROR_FRAMING,
} i2c_status_e;

typedef enum {
    MASTER_START,
    MASTER_ADDR,
    MASTER_READ,
    MASTER_WRITE,
    MASTER_STOP
} i2c_operation_e;

typedef std::function<void(i2c_operation_e id, i2c_status_e status, int value)> i2c_callback_t;

typedef std::function<void(vp::clock_event* event,uint64_t time_ps)> i2c_enqueue_event_fn_t;

/**
 * \brief TODO
 *
 * connect to a clock
 * connect to the helpersical interface
 *
 * Needs to:
 * - duty_cycle, baudrate
 * - be able to send/receive I2C parts ((RE)START, STOP, ADDR+RW, DATA, etc)
 * - clock stretch
 * - clock synchronization
 * - report errors (nack, arbitration, framing, etc)
 */
class I2C_helper {
    public:
        I2C_helper(vp::component* parent, vp::i2c_master* itf, i2c_enqueue_event_fn_t event);

        //TODO
        void set_timings(int prescaler, int delay_low, int delay_high);

        //TODO
        void send_start(void);

        //TODO
        void send_address(int addr, bool is_write, bool is_10bits);

        //TODO
        void send_data(int byte);

        //TODO
        void read_data(void);

        //TODO
        void send_stop(void);

        //TODO
        bool is_busy(void);

        /**
         * \brief TODO register callback
         */
        void register_callback(i2c_callback_t callback);
    private:
        /******************/
        /* Static methods */
        /******************/
        static void st_master_event_handler(void* __this, vp::clock_event* event);
        static void i2c_sync(void *__this, int scl, int sda);

        /***********/
        /* Methods */
        /***********/
        void master_event_handler(vp::clock_event* event);

        void start_clock(void);
        void stop_clock(void);

        void fsm_step(int scl, int sda);

        /*************/
        /* Externals */
        /*************/
        vp::component* parent;
        vp::i2c_master* itf;
        //vp::clock_engine* clock;

        /*************/
        /* Callbacks */
        /*************/
        i2c_callback_t cb_master_operation;

        i2c_enqueue_event_fn_t enqueue_event;

        /*****************/
        /* Configuration */
        /*****************/
        int prescaler;
        int delay_low;
        int delay_high;

        /****************/
        /* Runtime data */
        /****************/
        vp::clock_event master_event;

        bool bus_is_busy;
        int scl;
        int sda;
};