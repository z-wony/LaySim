#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdint.h>
#include <sys/time.h>

#include <glib.h>
#include <Elementary.h>

#define GETTID()	((long)syscall(SYS_gettid))

static char *font_color[6] = {
	"\033[0m", /* reset */
	"\033[31m", /* red */
	"\033[32m", /* green */
	"\033[34m", /* blue */
	"\033[35m", /* magenta */
	"\033[36m" /* cyan */
};

#define _color_reset	"\033[0m"
#define _color_red		"\033[31m"
#define _color_green	"\033[32m"
#define _color_blue		"\033[35m"
#define _color_cyan		"\033[36m"

#define log(fmt, arg...)	fprintf(stderr, _color_green"%s(%d)> "_color_reset fmt"\n", __FUNCTION__, (int)__LINE__, ##arg)

static void _set_playing_button(Evas_Object *layout)
{
    Evas_Object *playing_play = elm_button_add(layout);
    Evas_Object *playing_pause = elm_button_add(layout);
    Evas_Object *playing_stop = elm_button_add(layout);

    elm_object_text_set(playing_play, "Play");
    elm_object_text_set(playing_pause, "Pause");
    elm_object_text_set(playing_stop, "Stop");

    elm_object_part_content_set(layout, "area/control/playing/play", playing_play);
    elm_object_part_content_set(layout, "area/control/playing/pause", playing_pause);
    elm_object_part_content_set(layout, "area/control/playing/stop", playing_stop);

    evas_object_show(playing_play);
    evas_object_show(playing_pause);
    evas_object_show(playing_stop);
}

static void _set_timer_panel(Evas_Object *layout)
{
    Evas_Object *timer_label = elm_label_add(layout);

    elm_object_text_set(timer_label, "<font size=20>00:00:00.000 ms</font>");
    elm_object_part_content_set(layout, "area/control/timer/timer", timer_label);
    evas_object_show(timer_label);
}

int main(int argc, char *argv[])
{
	char *pkg = "LaySim Editor";

	elm_init(argc, argv);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	Evas_Object *win = elm_win_util_standard_add(pkg, pkg);
	elm_win_autodel_set(win, EINA_TRUE);
	evas_object_resize(win, 550, 550);
	evas_object_show(win);

    Evas_Object *ly = elm_layout_add(win);
    elm_layout_file_set(ly, "res/edc/theme.edj", "base");
    elm_win_resize_object_add(win, ly);
    evas_object_size_hint_weight_set(ly, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//    evas_object_resize(ly, 500, 500);
//    evas_object_move(ly, 0, 0);
    evas_object_show(ly);

    _set_playing_button(ly);
//    _set_timer_panel(ly);

	log("hello");

	elm_run();	
	elm_exit();

	return 0;
}
