/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <assert.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "hsm.h"
#include "queue.h"


/**  hsmtest.c -- Hierarchical State Machine test harness.
 *   This is an implementation of the example found in
 *   Practical StateCharts in C/C++ by Miro Samek.
 *   Intent of program is to exercise the state machine implementation.
 *   An earlier implementation published in ESP August 2000 had errors
 *   that were identified by kevin.fleming@philips.com
 *   M. Samek 02-11-25
 */



static Queue q;
static Queue *qp = &q;
static int msg_idx;

typedef struct HsmTest HsmTest;
struct HsmTest {
    Hsm super;
    State s1;
      State s11;
    State s2;
      State s21;
        State s211;
    int foo;
};

enum HsmTestEvents {
    A_SIG, B_SIG, C_SIG, D_SIG, E_SIG, F_SIG, G_SIG, H_SIG
};

Msg const *HsmTest_top(HsmTest *me, Msg const *msg) {
    switch (msg->evt) {
    case START_EVT:
        printf("top-INIT;");
        STATE_START(me, &me->s1);
        return 0;
    case ENTRY_EVT:
        printf("top-ENTRY;");
        return 0;
    case EXIT_EVT:
        printf("top-EXIT;");
        return 0;
    case E_SIG:
        printf("top-E;");
        STATE_TRAN(me, &me->s211);
        return 0;
    }
    return msg;
}

Msg const *HsmTest_s1(HsmTest *me, Msg const *msg) {
    switch (msg->evt) {
    case START_EVT:
        printf("s1-INIT;");
        STATE_START(me, &me->s11);
        return 0;
    case ENTRY_EVT:
        printf("s1-ENTRY;");
        return 0;
    case EXIT_EVT:
        printf("s1-EXIT;");
        return 0;
    case A_SIG:
        printf("s1-A;");
        STATE_TRAN(me, &me->s1);
        return 0;
    case B_SIG:
        printf("s1-B;");
        STATE_TRAN(me, &me->s11);
        return 0;
    case C_SIG:
        printf("s1-C;");
        STATE_TRAN(me, &me->s2);
        return 0;
    case D_SIG:
        printf("s1-D;");
        STATE_TRAN(me, &((Hsm *)me)->top);
        return 0;
    case F_SIG:
        printf("s1-F;");
        STATE_TRAN(me, &me->s211);
        return 0;
    }
    return msg;
}

Msg const *HsmTest_s11(HsmTest *me, Msg const *msg) {
    switch (msg->evt) {
    case ENTRY_EVT:
        printf("s11-ENTRY;");
        return 0;
    case EXIT_EVT:
        printf("s11-EXIT;");
        return 0;
    case G_SIG:
        printf("s11-G;");
        STATE_TRAN(me, &me->s211);
        return 0;
    case H_SIG:
        if (me->foo) {
            printf("s11-H;");
            me->foo = 0;
            return 0;
        }
        break;
    }
    return msg;
}

Msg const *HsmTest_s2(HsmTest *me, Msg const *msg) {
    switch (msg->evt) {
    case START_EVT:
        printf("s2-INIT;");
        STATE_START(me, &me->s21);
        return 0;
    case ENTRY_EVT:
        printf("s2-ENTRY;");
        return 0;
    case EXIT_EVT:
        printf("s2-EXIT;");
        return 0;
    case C_SIG:
        printf("s2-C;");
        STATE_TRAN(me, &me->s1);
        return 0;
    case F_SIG:
        printf("s2-F;");
        STATE_TRAN(me, &me->s11);
        return 0;
    }
    return msg;
}

Msg const *HsmTest_s21(HsmTest *me, Msg const *msg) {
    switch (msg->evt) {
    case START_EVT:
        printf("s21-INIT;");
        STATE_START(me, &me->s211);
        return 0;
    case ENTRY_EVT:
        printf("s21-ENTRY;");
        return 0;
    case EXIT_EVT:
        printf("s21-EXIT;");
        return 0;
    case B_SIG:
        printf("s21-B;");
        STATE_TRAN(me, &me->s211);
        return 0;
    case H_SIG:
        if (!me->foo) {
            printf("s21-H;");
            me->foo = 1;
            STATE_TRAN(me, &me->s21);
            return 0;
        }
        break;
    }
    return msg;
}

Msg const *HsmTest_s211(HsmTest *me, Msg const *msg) {
    switch (msg->evt) {
    case ENTRY_EVT:
        printf("s211-ENTRY;");
        return 0;
    case EXIT_EVT:
        printf("s211-EXIT;");
        return 0;
    case D_SIG:
        printf("s211-D;");
        STATE_TRAN(me, &me->s21);
        return 0;
    case G_SIG:
        printf("s211-G;");
        STATE_TRAN(me, &((Hsm *)me)->top);
        return 0;
    }
    return msg;
}

void HsmTestCtor(HsmTest *me) {
    HsmCtor((Hsm *)me, "HsmTest", (EvtHndlr)HsmTest_top);
    StateCtor(&me->s1, "s1", &((Hsm *)me)->top, (EvtHndlr)HsmTest_s1);
      StateCtor(&me->s11, "s11", &me->s1, (EvtHndlr)HsmTest_s11);
    StateCtor(&me->s2, "s2", &((Hsm *)me)->top, (EvtHndlr)HsmTest_s2);
      StateCtor(&me->s21, "s21", &me->s2, (EvtHndlr)HsmTest_s21);
    StateCtor(&me->s211, "s211", &me->s21, (EvtHndlr)HsmTest_s211);
    me->foo = 0;
}

const Msg HsmTestMsg[] = {
    {A_SIG},{B_SIG},{C_SIG},{D_SIG},{E_SIG},{F_SIG},{G_SIG},{H_SIG},
};

void app_main(void)
{
    HsmTest hsmTest;

    QueueInit(qp,10);
    QueueSendToBack(qp,0);


    HsmTestCtor(&hsmTest);
    HsmOnStart((Hsm *)&hsmTest);

    /* Main Loop */
    for (;;) {

        QueuePrint(qp);

        QueueReceive(qp, &msg_idx);
        HsmOnEvent((Hsm *)&hsmTest, &HsmTestMsg[msg_idx]);

        asm("nop");
    }

    /* Never reach this line */
}
