#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

groups_t groups;
static bool groups_created = false;

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

static void event_handler_cb_main_main(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_SCREEN_LOAD_START) {
        // group: group
        lv_group_remove_all_objs(groups.group);
        lv_group_add_obj(groups.group, objects.title);
        lv_group_add_obj(groups.group, objects.reader);
    }
}

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 384, 168);
    lv_obj_add_event_cb(obj, event_handler_cb_main_main, LV_EVENT_ALL, 0);
    {
        lv_obj_t *parent_obj = obj;
        {
            // title
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.title = obj;
            lv_obj_set_pos(obj, 5, 4);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "monoTerminal eBook Reader Demo");
        }
        {
            // reader
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.reader = obj;
            lv_obj_set_pos(obj, 5, 23);
            lv_obj_set_size(obj, 369, 136);
            lv_textarea_set_max_length(obj, 1024);
            lv_textarea_set_text(obj, "考公考编时，才读懂《范进中举》；找工作时，才读懂《孔乙己》；谈婚论嫁时，才读懂《孔雀东南飞》；十五六岁的我在地理试卷上写下: “此地的优势在于其丰富的廉价劳动力，十年后的我审视着下班后的疲惫自己，一颗子弹正中眉心”当年的老师叫不醒曾经的我们一样。也完美诠释了，教育具有长期性和滞后性，就像是一个闭环，多年后你有一个瞬间突然意识到什么，那就是子弹命中的瞬间。此时，才是教育的完成”不过花有重开日，人无少年时。我特别喜欢一段话；我们不能站在现在高度去批判当年的自己，这不公平，如果能重来一次，以当年的阅历和心理，还是会做出同样的选择。那么，不如放过自己和过去的你和解吧");
            lv_textarea_set_one_line(obj, false);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ACTIVE);
            lv_obj_set_scroll_dir(obj, LV_DIR_VER);
            lv_obj_set_style_text_font(obj, &ui_font_zh_16px, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}


void ui_create_groups() {
    if (!groups_created) {
        groups.group = lv_group_create();
        groups_created = true;
    }
}


typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    ui_create_groups();
    
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
}
