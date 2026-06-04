#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *page1;
    lv_obj_t *main_title;
    lv_obj_t *main_led1;
    lv_obj_t *main_bar_bat1;
    lv_obj_t *main_bar_soil1;
    lv_obj_t *main_led2;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *main_bar_bat2;
    lv_obj_t *main_bar_soil2;
    lv_obj_t *obj2;
    lv_obj_t *obj3;
    lv_obj_t *main_led3;
    lv_obj_t *main_bar_bat3;
    lv_obj_t *main_bar_soil3;
    lv_obj_t *obj4;
    lv_obj_t *main_led4;
    lv_obj_t *main_bar_bat4;
    lv_obj_t *main_bar_soil4;
    lv_obj_t *obj5;
    lv_obj_t *main_led5;
    lv_obj_t *main_bar_bat5;
    lv_obj_t *main_bar_soil5;
    lv_obj_t *obj6;
    lv_obj_t *main_led6;
    lv_obj_t *main_bar_bat6;
    lv_obj_t *main_bar_soil6;
    lv_obj_t *obj7;
    lv_obj_t *main_led7;
    lv_obj_t *main_bar_bat7;
    lv_obj_t *main_bar_soil7;
    lv_obj_t *obj8;
    lv_obj_t *main_led8;
    lv_obj_t *main_bar_bat8;
    lv_obj_t *main_bar_soil8;
    lv_obj_t *obj9;
    lv_obj_t *main_led9;
    lv_obj_t *main_bar_bat9;
    lv_obj_t *main_bar_soil9;
    lv_obj_t *obj10;
    lv_obj_t *main_led10;
    lv_obj_t *main_bar_bat10;
    lv_obj_t *main_bar_soil10;
    lv_obj_t *obj11;
    lv_obj_t *bt_page1_to_main;
    lv_obj_t *bt1_back;
    lv_obj_t *id;
    lv_obj_t *idval;
    lv_obj_t *bat;
    lv_obj_t *batval;
    lv_obj_t *umi;
    lv_obj_t *umival;
    lv_obj_t *fre;
    lv_obj_t *freeval;
    lv_obj_t *obj12;
    lv_obj_t *obj13;
    lv_obj_t *obj14;
    lv_obj_t *desrval;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_PAGE1 = 2,
};

void create_screen_main();
void delete_screen_main();
void tick_screen_main();

void create_screen_page1();
void delete_screen_page1();
void tick_screen_page1();

void create_screen_by_id(enum ScreensEnum screenId);
void delete_screen_by_id(enum ScreensEnum screenId);
void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/