#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Main_Title
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.main_title = obj;
            lv_obj_set_pos(obj, 19, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff1baad4), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_INTERNAL, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_post(obj, false, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "BiblioLab    Green    Monitor");
        }
        {
            // MainLed1
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.main_led1 = obj;
            lv_obj_set_pos(obj, 52, 49);
            lv_obj_set_size(obj, 13, 13);
            lv_led_set_brightness(obj, 200);
            lv_obj_add_event_cb(obj, action_on_led_clicked1, LV_EVENT_CLICKED, (void *)0);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_width(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarBat1
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_bat1 = obj;
            lv_obj_set_pos(obj, 53, 64);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2196f3), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        }
        {
            // MainBarSoil1
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_soil1 = obj;
            lv_obj_set_pos(obj, 53, 146);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_max_width(obj, 8191, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_min_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainLed2
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.main_led2 = obj;
            lv_obj_set_pos(obj, 78, 49);
            lv_obj_set_size(obj, 13, 13);
            lv_led_set_brightness(obj, 200);
            lv_obj_add_event_cb(obj, action_on_led_clicked2, LV_EVENT_CLICKED, (void *)1);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_width(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 2, 87);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Batteria");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 1, 170);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff00), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Umidita'");
        }
        {
            // MainBarBat2
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_bat2 = obj;
            lv_obj_set_pos(obj, 80, 65);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2196f3), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarSoil2
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_soil2 = obj;
            lv_obj_set_pos(obj, 80, 146);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_max_width(obj, 8191, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_min_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj2 = obj;
            lv_obj_set_pos(obj, 56, 29);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "1");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj3 = obj;
            lv_obj_set_pos(obj, 81, 29);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "2");
        }
        {
            // MainLed3
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.main_led3 = obj;
            lv_obj_set_pos(obj, 105, 49);
            lv_obj_set_size(obj, 13, 13);
            lv_led_set_brightness(obj, 200);
            lv_obj_add_event_cb(obj, action_on_led_clicked3, LV_EVENT_CLICKED, (void *)3);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_width(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarBat3
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_bat3 = obj;
            lv_obj_set_pos(obj, 106, 65);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarSoil3
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_soil3 = obj;
            lv_obj_set_pos(obj, 106, 146);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_max_width(obj, 8191, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_min_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj4 = obj;
            lv_obj_set_pos(obj, 107, 29);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "3");
        }
        {
            // MainLed4
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.main_led4 = obj;
            lv_obj_set_pos(obj, 131, 49);
            lv_obj_set_size(obj, 13, 13);
            lv_led_set_brightness(obj, 200);
            lv_obj_add_event_cb(obj, action_on_led_clicked4, LV_EVENT_CLICKED, (void *)4);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_width(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarBat4
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_bat4 = obj;
            lv_obj_set_pos(obj, 133, 65);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarSoil4
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_soil4 = obj;
            lv_obj_set_pos(obj, 133, 146);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_max_width(obj, 8191, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_min_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj5 = obj;
            lv_obj_set_pos(obj, 133, 29);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "4");
        }
        {
            // MainLed5
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.main_led5 = obj;
            lv_obj_set_pos(obj, 158, 49);
            lv_obj_set_size(obj, 13, 13);
            lv_led_set_brightness(obj, 200);
            lv_obj_add_event_cb(obj, action_on_led_clicked5, LV_EVENT_CLICKED, (void *)5);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_width(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarBat5
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_bat5 = obj;
            lv_obj_set_pos(obj, 159, 65);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarSoil5
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_soil5 = obj;
            lv_obj_set_pos(obj, 159, 146);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_max_width(obj, 8191, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_min_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj6 = obj;
            lv_obj_set_pos(obj, 161, 29);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "5");
        }
        {
            // MainLed6
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.main_led6 = obj;
            lv_obj_set_pos(obj, 184, 49);
            lv_obj_set_size(obj, 13, 13);
            lv_led_set_brightness(obj, 200);
            lv_obj_add_event_cb(obj, action_on_led_clicked6, LV_EVENT_CLICKED, (void *)6);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_width(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarBat6
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_bat6 = obj;
            lv_obj_set_pos(obj, 186, 65);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarSoil6
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_soil6 = obj;
            lv_obj_set_pos(obj, 186, 146);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_max_width(obj, 8191, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_min_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj7 = obj;
            lv_obj_set_pos(obj, 187, 29);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "6");
        }
        {
            // MainLed7
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.main_led7 = obj;
            lv_obj_set_pos(obj, 211, 49);
            lv_obj_set_size(obj, 13, 13);
            lv_led_set_brightness(obj, 200);
            lv_obj_add_event_cb(obj, action_on_led_clicked7, LV_EVENT_CLICKED, (void *)7);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_width(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarBat7
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_bat7 = obj;
            lv_obj_set_pos(obj, 212, 65);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarSoil7
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_soil7 = obj;
            lv_obj_set_pos(obj, 212, 146);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_max_width(obj, 8191, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_min_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj8 = obj;
            lv_obj_set_pos(obj, 212, 29);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "7");
        }
        {
            // MainLed8
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.main_led8 = obj;
            lv_obj_set_pos(obj, 237, 49);
            lv_obj_set_size(obj, 13, 13);
            lv_led_set_brightness(obj, 200);
            lv_obj_add_event_cb(obj, action_on_led_clicked8, LV_EVENT_CLICKED, (void *)8);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_width(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarBat8
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_bat8 = obj;
            lv_obj_set_pos(obj, 239, 65);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarSoil8
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_soil8 = obj;
            lv_obj_set_pos(obj, 239, 146);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_max_width(obj, 8191, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_min_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj9 = obj;
            lv_obj_set_pos(obj, 238, 29);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "8");
        }
        {
            // MainLed9
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.main_led9 = obj;
            lv_obj_set_pos(obj, 264, 49);
            lv_obj_set_size(obj, 13, 13);
            lv_led_set_brightness(obj, 200);
            lv_obj_add_event_cb(obj, action_on_led_clicked9, LV_EVENT_CLICKED, (void *)9);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_width(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarBat9
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_bat9 = obj;
            lv_obj_set_pos(obj, 265, 65);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarSoil9
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_soil9 = obj;
            lv_obj_set_pos(obj, 265, 146);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_max_width(obj, 8191, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_min_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj10 = obj;
            lv_obj_set_pos(obj, 266, 29);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "9");
        }
        {
            // MainLed10
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.main_led10 = obj;
            lv_obj_set_pos(obj, 290, 49);
            lv_obj_set_size(obj, 13, 13);
            lv_led_set_brightness(obj, 200);
            lv_obj_add_event_cb(obj, action_on_led_clicked10, LV_EVENT_CLICKED, (void *)10);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_width(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // MainBarBat10
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_bat10 = obj;
            lv_obj_set_pos(obj, 292, 64);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff2196f3), LV_PART_MAIN | LV_STATE_SCROLLED);
        }
        {
            // MainBarSoil10
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.main_bar_soil10 = obj;
            lv_obj_set_pos(obj, 292, 146);
            lv_obj_set_size(obj, 10, 70);
            lv_obj_set_style_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_max_width(obj, 8191, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_min_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj11 = obj;
            lv_obj_set_pos(obj, 291, 29);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "10");
        }
    }
    
    tick_screen_main();
}

void delete_screen_main() {
    lv_obj_del(objects.main);
    objects.main = 0;
    objects.main_title = 0;
    objects.main_led1 = 0;
    objects.main_bar_bat1 = 0;
    objects.main_bar_soil1 = 0;
    objects.main_led2 = 0;
    objects.obj0 = 0;
    objects.obj1 = 0;
    objects.main_bar_bat2 = 0;
    objects.main_bar_soil2 = 0;
    objects.obj2 = 0;
    objects.obj3 = 0;
    objects.main_led3 = 0;
    objects.main_bar_bat3 = 0;
    objects.main_bar_soil3 = 0;
    objects.obj4 = 0;
    objects.main_led4 = 0;
    objects.main_bar_bat4 = 0;
    objects.main_bar_soil4 = 0;
    objects.obj5 = 0;
    objects.main_led5 = 0;
    objects.main_bar_bat5 = 0;
    objects.main_bar_soil5 = 0;
    objects.obj6 = 0;
    objects.main_led6 = 0;
    objects.main_bar_bat6 = 0;
    objects.main_bar_soil6 = 0;
    objects.obj7 = 0;
    objects.main_led7 = 0;
    objects.main_bar_bat7 = 0;
    objects.main_bar_soil7 = 0;
    objects.obj8 = 0;
    objects.main_led8 = 0;
    objects.main_bar_bat8 = 0;
    objects.main_bar_soil8 = 0;
    objects.obj9 = 0;
    objects.main_led9 = 0;
    objects.main_bar_bat9 = 0;
    objects.main_bar_soil9 = 0;
    objects.obj10 = 0;
    objects.main_led10 = 0;
    objects.main_bar_bat10 = 0;
    objects.main_bar_soil10 = 0;
    objects.obj11 = 0;
}

void tick_screen_main() {
    {
        uint32_t new_val = get_var_color_led1();
        new_val = lv_color_to32(lv_color_hex(new_val));
        uint32_t cur_val = lv_color_to32(((lv_led_t *)objects.main_led1)->color);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_led1;
            lv_led_set_color(objects.main_led1, lv_color_hex(new_val));
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_bat1();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_bat1);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_bat1;
            lv_bar_set_value(objects.main_bar_bat1, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_soil1();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_soil1);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_soil1;
            lv_bar_set_value(objects.main_bar_soil1, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        uint32_t new_val = get_var_color_led2();
        new_val = lv_color_to32(lv_color_hex(new_val));
        uint32_t cur_val = lv_color_to32(((lv_led_t *)objects.main_led2)->color);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_led2;
            lv_led_set_color(objects.main_led2, lv_color_hex(new_val));
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_bat2();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_bat2);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_bat2;
            lv_bar_set_value(objects.main_bar_bat2, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_soil2();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_soil2);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_soil2;
            lv_bar_set_value(objects.main_bar_soil2, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        uint32_t new_val = get_var_color_led3();
        new_val = lv_color_to32(lv_color_hex(new_val));
        uint32_t cur_val = lv_color_to32(((lv_led_t *)objects.main_led3)->color);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_led3;
            lv_led_set_color(objects.main_led3, lv_color_hex(new_val));
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_bat3();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_bat3);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_bat3;
            lv_bar_set_value(objects.main_bar_bat3, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_soil3();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_soil3);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_soil3;
            lv_bar_set_value(objects.main_bar_soil3, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        uint32_t new_val = get_var_color_led4();
        new_val = lv_color_to32(lv_color_hex(new_val));
        uint32_t cur_val = lv_color_to32(((lv_led_t *)objects.main_led4)->color);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_led4;
            lv_led_set_color(objects.main_led4, lv_color_hex(new_val));
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_bat4();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_bat4);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_bat4;
            lv_bar_set_value(objects.main_bar_bat4, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_soil4();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_soil4);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_soil4;
            lv_bar_set_value(objects.main_bar_soil4, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        uint32_t new_val = get_var_color_led5();
        new_val = lv_color_to32(lv_color_hex(new_val));
        uint32_t cur_val = lv_color_to32(((lv_led_t *)objects.main_led5)->color);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_led5;
            lv_led_set_color(objects.main_led5, lv_color_hex(new_val));
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_bat5();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_bat5);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_bat5;
            lv_bar_set_value(objects.main_bar_bat5, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_soil5();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_soil5);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_soil5;
            lv_bar_set_value(objects.main_bar_soil5, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        uint32_t new_val = get_var_color_led6();
        new_val = lv_color_to32(lv_color_hex(new_val));
        uint32_t cur_val = lv_color_to32(((lv_led_t *)objects.main_led6)->color);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_led6;
            lv_led_set_color(objects.main_led6, lv_color_hex(new_val));
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_bat6();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_bat6);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_bat6;
            lv_bar_set_value(objects.main_bar_bat6, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_soil6();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_soil6);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_soil6;
            lv_bar_set_value(objects.main_bar_soil6, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        uint32_t new_val = get_var_color_led7();
        new_val = lv_color_to32(lv_color_hex(new_val));
        uint32_t cur_val = lv_color_to32(((lv_led_t *)objects.main_led7)->color);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_led7;
            lv_led_set_color(objects.main_led7, lv_color_hex(new_val));
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_bat7();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_bat7);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_bat7;
            lv_bar_set_value(objects.main_bar_bat7, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_soil7();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_soil7);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_soil7;
            lv_bar_set_value(objects.main_bar_soil7, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        uint32_t new_val = get_var_color_led8();
        new_val = lv_color_to32(lv_color_hex(new_val));
        uint32_t cur_val = lv_color_to32(((lv_led_t *)objects.main_led8)->color);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_led8;
            lv_led_set_color(objects.main_led8, lv_color_hex(new_val));
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_bat8();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_bat8);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_bat8;
            lv_bar_set_value(objects.main_bar_bat8, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_soil8();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_soil8);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_soil8;
            lv_bar_set_value(objects.main_bar_soil8, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        uint32_t new_val = get_var_color_led9();
        new_val = lv_color_to32(lv_color_hex(new_val));
        uint32_t cur_val = lv_color_to32(((lv_led_t *)objects.main_led9)->color);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_led9;
            lv_led_set_color(objects.main_led9, lv_color_hex(new_val));
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_bat9();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_bat9);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_bat9;
            lv_bar_set_value(objects.main_bar_bat9, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_soil9();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_soil9);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_soil9;
            lv_bar_set_value(objects.main_bar_soil9, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        uint32_t new_val = get_var_color_led10();
        new_val = lv_color_to32(lv_color_hex(new_val));
        uint32_t cur_val = lv_color_to32(((lv_led_t *)objects.main_led10)->color);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_led10;
            lv_led_set_color(objects.main_led10, lv_color_hex(new_val));
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_bat10();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_bat10);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_bat10;
            lv_bar_set_value(objects.main_bar_bat10, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_bar_soil10();
        int32_t cur_val = lv_bar_get_value(objects.main_bar_soil10);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.main_bar_soil10;
            lv_bar_set_value(objects.main_bar_soil10, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_page1() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page1 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff15171a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(obj, &img_sfondo, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // BTPage1ToMain
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.bt_page1_to_main = obj;
            lv_obj_set_pos(obj, 234, 187);
            lv_obj_set_size(obj, 77, 41);
            lv_obj_add_event_cb(obj, action_to_main_page, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Bt1Back
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.bt1_back = obj;
                    lv_obj_set_pos(obj, 1, 1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Indietro");
                }
            }
        }
        {
            // ID
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.id = obj;
            lv_obj_set_pos(obj, 120, 54);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "ID :");
        }
        {
            // IDVAL
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.idval = obj;
            lv_obj_set_pos(obj, 149, 54);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // BAT
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.bat = obj;
            lv_obj_set_pos(obj, 120, 88);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Batteria  :");
        }
        {
            // BATVAL
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.batval = obj;
            lv_obj_set_pos(obj, 215, 88);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // UMI
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.umi = obj;
            lv_obj_set_pos(obj, 121, 110);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Umidita' :");
        }
        {
            // UMIVAL
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.umival = obj;
            lv_obj_set_pos(obj, 215, 110);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // FRE
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.fre = obj;
            lv_obj_set_pos(obj, 120, 136);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Cadenza :");
        }
        {
            // FREEVAL
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.freeval = obj;
            lv_obj_set_pos(obj, 215, 136);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj12 = obj;
            lv_obj_set_pos(obj, 259, 88);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "[%]");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj13 = obj;
            lv_obj_set_pos(obj, 259, 110);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "[%]");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj14 = obj;
            lv_obj_set_pos(obj, 259, 136);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "[min]");
        }
        {
            // DESRVAL
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.desrval = obj;
            lv_obj_set_pos(obj, 22, 10);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
    }
    
    tick_screen_page1();
}

void delete_screen_page1() {
    lv_obj_del(objects.page1);
    objects.page1 = 0;
    objects.bt_page1_to_main = 0;
    objects.bt1_back = 0;
    objects.id = 0;
    objects.idval = 0;
    objects.bat = 0;
    objects.batval = 0;
    objects.umi = 0;
    objects.umival = 0;
    objects.fre = 0;
    objects.freeval = 0;
    objects.obj12 = 0;
    objects.obj13 = 0;
    objects.obj14 = 0;
    objects.desrval = 0;
}

void tick_screen_page1() {
    {
        const char *new_val = get_var_node_id();
        const char *cur_val = lv_label_get_text(objects.idval);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.idval;
            lv_label_set_text(objects.idval, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_batteria();
        const char *cur_val = lv_label_get_text(objects.batval);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.batval;
            lv_label_set_text(objects.batval, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_umidita();
        const char *cur_val = lv_label_get_text(objects.umival);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.umival;
            lv_label_set_text(objects.umival, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_update_freq();
        const char *cur_val = lv_label_get_text(objects.freeval);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.freeval;
            lv_label_set_text(objects.freeval, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_description();
        const char *cur_val = lv_label_get_text(objects.desrval);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.desrval;
            lv_label_set_text(objects.desrval, new_val);
            tick_value_change_obj = NULL;
        }
    }
}



typedef void (*create_screen_func_t)();
create_screen_func_t create_screen_funcs[] = {
    create_screen_main,
    create_screen_page1,
};
void create_screen(int screen_index) {
    create_screen_funcs[screen_index]();
}
void create_screen_by_id(enum ScreensEnum screenId) {
    create_screen_funcs[screenId - 1]();
}

typedef void (*delete_screen_func_t)();
delete_screen_func_t delete_screen_funcs[] = {
    delete_screen_main,
    delete_screen_page1,
};
void delete_screen(int screen_index) {
    delete_screen_funcs[screen_index]();
}
void delete_screen_by_id(enum ScreensEnum screenId) {
    delete_screen_funcs[screenId - 1]();
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_page1,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
    create_screen_page1();
}
