// #include <Arduino.h>

// typedef enum
// {
//     BTN_UNCHANGE = 0,
//     BTN_PRESSED,
//     BTN_RELEASE,
//     BTN_LONG_PRESS,
//     BTN_LONG_RELEASE,
// } Tbutton_status_t;

// typedef struct Tbutton_item
// {
//     int gpio;
//     long long last_press_tick;
//     bool long_press;
//     struct Tbutton_item *entry;
// } Tbutton_item_t;

// struct Tbutton_struct
// {
//     int long_press_time_ms;
//     long long gpio_mask;
//     struct Tbutton_list
//     {
//         struct Tbutton_item *stqh_first; /* first element */
//         struct Tbutton_item **stqh_last; /* addr of last next element */
//     } btn_list;
// };

// typedef struct
// {
//     uint64_t press_mask;
//     uint64_t release_mask;
//     uint64_t long_press_mask;
//     uint64_t long_release_mask;
// } button_result_t;

// typedef struct Tbutton_struct *Tbutton_handle_t;
// typedef void (*gpio_intr_handler)(void *);

// typedef struct
// {
//     int long_press_time_ms;
//     uint64_t gpio_mask;
//     gpio_intr_handler button_intr_handler;
//     void *intr_context;
// } button_config_t;

// #define DEFAULT_LONG_PRESS_TIME_MS (2 * 1000)

// class Tbutton
// {

//     void begin()
//     {
//     }

//     Tbutton_status_t button_get_state(Tbutton_handle_t button, Tbutton_item_t *btn_item)
//     {
//         int level = digitalRead(item->gpio);
//         int active_level = 0;
//         int deactive_level = 1;
//         if (btn_item->last_press_tick == 0 && level == active_level)
//         {
//             btn_item->last_press_tick = tick_get();
//             btn_item->long_pressed = false;
//             return BTN_PRESSED;
//         }

//         if (level == deactive_level && btn_item->last_press_tick && tick_get() - btn_item->last_press_tick > button->long_press_time_ms)
//         {
//             btn_item->last_press_tick = 0;
//             btn_item->long_pressed = false;
//             return BTN_LONG_RELEASE;
//         }

//         if (level == deactive_level && btn_item->last_press_tick)
//         {
//             btn_item->last_press_tick = 0;
//             btn_item->long_pressed = false;
//             return BTN_RELEASE;
//         }

//         if (btn_item->long_pressed == false && level == active_level && tick_get() - btn_item->last_press_tick > button->long_press_time_ms)
//         {
//             btn_item->long_pressed = true;
//             return BTN_LONG_PRESS;
//         }
//         return BTN_UNCHANGE;
//     }

//     uint64_t tick_get()
//     {
//         return millis();
//     }

    

// }