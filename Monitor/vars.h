#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations

typedef enum {
    StatoID1_RED = 0,
    StatoID1_GREEN = 1
} StatoID1;

// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_BAR_BAT1 = 0,
    FLOW_GLOBAL_VARIABLE_BAR_BAT2 = 1,
    FLOW_GLOBAL_VARIABLE_BAR_BAT3 = 2,
    FLOW_GLOBAL_VARIABLE_BAR_BAT4 = 3,
    FLOW_GLOBAL_VARIABLE_BAR_BAT5 = 4,
    FLOW_GLOBAL_VARIABLE_BAR_BAT6 = 5,
    FLOW_GLOBAL_VARIABLE_BAR_BAT7 = 6,
    FLOW_GLOBAL_VARIABLE_BAR_BAT8 = 7,
    FLOW_GLOBAL_VARIABLE_BAR_BAT9 = 8,
    FLOW_GLOBAL_VARIABLE_BAR_BAT10 = 9,
    FLOW_GLOBAL_VARIABLE_BAR_SOIL1 = 10,
    FLOW_GLOBAL_VARIABLE_BAR_SOIL2 = 11,
    FLOW_GLOBAL_VARIABLE_BAR_SOIL3 = 12,
    FLOW_GLOBAL_VARIABLE_BAR_SOIL4 = 13,
    FLOW_GLOBAL_VARIABLE_BAR_SOIL5 = 14,
    FLOW_GLOBAL_VARIABLE_BAR_SOIL6 = 15,
    FLOW_GLOBAL_VARIABLE_BAR_SOIL7 = 16,
    FLOW_GLOBAL_VARIABLE_BAR_SOIL8 = 17,
    FLOW_GLOBAL_VARIABLE_BAR_SOIL9 = 18,
    FLOW_GLOBAL_VARIABLE_BAR_SOIL10 = 19,
    FLOW_GLOBAL_VARIABLE_COLOR_LED1 = 20,
    FLOW_GLOBAL_VARIABLE_COLOR_LED2 = 21,
    FLOW_GLOBAL_VARIABLE_COLOR_LED3 = 22,
    FLOW_GLOBAL_VARIABLE_COLOR_LED4 = 23,
    FLOW_GLOBAL_VARIABLE_COLOR_LED5 = 24,
    FLOW_GLOBAL_VARIABLE_COLOR_LED6 = 25,
    FLOW_GLOBAL_VARIABLE_COLOR_LED7 = 26,
    FLOW_GLOBAL_VARIABLE_COLOR_LED8 = 27,
    FLOW_GLOBAL_VARIABLE_COLOR_LED9 = 28,
    FLOW_GLOBAL_VARIABLE_COLOR_LED10 = 29,
    FLOW_GLOBAL_VARIABLE_NODE_ID = 30,
    FLOW_GLOBAL_VARIABLE_DESCRIPTION = 31,
    FLOW_GLOBAL_VARIABLE_UPDATE_FREQ = 32,
    FLOW_GLOBAL_VARIABLE_UMIDITA = 33,
    FLOW_GLOBAL_VARIABLE_BATTERIA = 34
};

// Native global variables

extern int32_t get_var_bar_bat1();
extern void set_var_bar_bat1(int32_t value);
extern int32_t get_var_bar_bat2();
extern void set_var_bar_bat2(int32_t value);
extern int32_t get_var_bar_bat3();
extern void set_var_bar_bat3(int32_t value);
extern int32_t get_var_bar_bat4();
extern void set_var_bar_bat4(int32_t value);
extern int32_t get_var_bar_bat5();
extern void set_var_bar_bat5(int32_t value);
extern int32_t get_var_bar_bat6();
extern void set_var_bar_bat6(int32_t value);
extern int32_t get_var_bar_bat7();
extern void set_var_bar_bat7(int32_t value);
extern int32_t get_var_bar_bat8();
extern void set_var_bar_bat8(int32_t value);
extern int32_t get_var_bar_bat9();
extern void set_var_bar_bat9(int32_t value);
extern int32_t get_var_bar_bat10();
extern void set_var_bar_bat10(int32_t value);
extern int32_t get_var_bar_soil1();
extern void set_var_bar_soil1(int32_t value);
extern int32_t get_var_bar_soil2();
extern void set_var_bar_soil2(int32_t value);
extern int32_t get_var_bar_soil3();
extern void set_var_bar_soil3(int32_t value);
extern int32_t get_var_bar_soil4();
extern void set_var_bar_soil4(int32_t value);
extern int32_t get_var_bar_soil5();
extern void set_var_bar_soil5(int32_t value);
extern int32_t get_var_bar_soil6();
extern void set_var_bar_soil6(int32_t value);
extern int32_t get_var_bar_soil7();
extern void set_var_bar_soil7(int32_t value);
extern int32_t get_var_bar_soil8();
extern void set_var_bar_soil8(int32_t value);
extern int32_t get_var_bar_soil9();
extern void set_var_bar_soil9(int32_t value);
extern int32_t get_var_bar_soil10();
extern void set_var_bar_soil10(int32_t value);
extern int32_t get_var_color_led1();
extern void set_var_color_led1(int32_t value);
extern int32_t get_var_color_led2();
extern void set_var_color_led2(int32_t value);
extern int32_t get_var_color_led3();
extern void set_var_color_led3(int32_t value);
extern int32_t get_var_color_led4();
extern void set_var_color_led4(int32_t value);
extern int32_t get_var_color_led5();
extern void set_var_color_led5(int32_t value);
extern int32_t get_var_color_led6();
extern void set_var_color_led6(int32_t value);
extern int32_t get_var_color_led7();
extern void set_var_color_led7(int32_t value);
extern int32_t get_var_color_led8();
extern void set_var_color_led8(int32_t value);
extern int32_t get_var_color_led9();
extern void set_var_color_led9(int32_t value);
extern int32_t get_var_color_led10();
extern void set_var_color_led10(int32_t value);
extern const char *get_var_node_id();
extern void set_var_node_id(const char *value);
extern const char *get_var_description();
extern void set_var_description(const char *value);
extern const char *get_var_update_freq();
extern void set_var_update_freq(const char *value);
extern const char *get_var_umidita();
extern void set_var_umidita(const char *value);
extern const char *get_var_batteria();
extern void set_var_batteria(const char *value);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/