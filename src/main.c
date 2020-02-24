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

typedef enum {
    PLAYING_READY = 0,
    PLAYING_START = 1,
    PLAYING_PAUSE = 2,
    PLAYING_STOP = 3,
} playing_state_e;

typedef struct _playing_context_s {
    playing_state_e state;
    long time_ms;
    long start_time_ms;
    Evas_Object *layout;
    Ecore_Timer *timer;
} ply_ctx_s;

static void _set_playing_button(Evas_Object *layout, ply_ctx_s *ctx);
static Evas_Object *_get_timer_panel(Evas_Object *layout);
static void _play_button_clicked_cb(void *data, Evas_Object *obj, void *event_info);
static void _pause_button_clicked_cb(void *data, Evas_Object *obj, void *event_info);
static void _stop_button_clicked_cb(void *data, Evas_Object *obj, void *event_info);
static Eina_Bool _simulator_timer_cb(void *data);

static void _play_button_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
    ply_ctx_s *ctx = (ply_ctx_s *)data;
    Evas_Object *layout = ctx->layout;

    switch (ctx->state) {
        case PLAYING_STOP:
            elm_object_part_text_set(layout, "area/control/timer/timer", "00:00.000 ms");
            ctx->time_ms = 0L;
            ctx->start_time_ms = 0L;
        case PLAYING_PAUSE:
            if (ctx->timer) {
                ecore_timer_del(ctx->timer);
                ctx->timer = NULL;
            }
        case PLAYING_READY:
            ctx->timer = ecore_timer_add(0.1, _simulator_timer_cb, (void *)ctx);
            ctx->state = PLAYING_START;
            break;
        default:
            break;
    }
}

static void _pause_button_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
    ply_ctx_s *ctx = (ply_ctx_s *)data;
    Evas_Object *layout = ctx->layout;

    switch (ctx->state) {
        case PLAYING_START:
            if (ctx->timer) {
                ecore_timer_del(ctx->timer);
                ctx->timer = NULL;
            }
            ctx->state = PLAYING_PAUSE;
            break;
        deafult:
            break;
    }
}

static void _stop_button_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
    ply_ctx_s *ctx = (ply_ctx_s *)data;
    Evas_Object *layout = ctx->layout;

    switch (ctx->state) {
        case PLAYING_STOP:
            elm_object_part_text_set(layout, "area/control/timer/timer", "00:00.000 ms");
            ctx->time_ms = 0L;
            ctx->start_time_ms = 0L;
            if (ctx->timer) {
                ecore_timer_del(ctx->timer);
                ctx->timer = NULL;
            }
        case PLAYING_PAUSE:
            ctx->state = PLAYING_STOP;
            break;
        case PLAYING_START:
            if (ctx->timer) {
                ecore_timer_del(ctx->timer);
                ctx->timer = NULL;
            }
            ctx->state = PLAYING_STOP;
            break;
        default:
            break;
    }
}

static void _set_playing_button(Evas_Object *layout, ply_ctx_s *ctx)
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

    evas_object_smart_callback_add(playing_play, "clicked", _play_button_clicked_cb, (void *)ctx);
    evas_object_smart_callback_add(playing_pause, "clicked", _pause_button_clicked_cb, (void *)ctx);
    evas_object_smart_callback_add(playing_stop, "clicked", _stop_button_clicked_cb, (void *)ctx);

    evas_object_show(playing_play);
    evas_object_show(playing_pause);
    evas_object_show(playing_stop);
}

static Evas_Object *_get_timer_panel(Evas_Object *layout)
{
    return elm_object_part_content_get(layout, "area/control/timer/timer");
}

static Eina_Bool _simulator_timer_cb(void *data)
{
    ply_ctx_s *ctx = (ply_ctx_s *)data;
    Evas_Object *layout = ctx->layout;

    ctx->time_ms += 100;
    long tval = ctx->time_ms;
    char timer_str[40];
    snprintf(timer_str, 40, "%02ld:%02ld.%03ld ms", (tval / 60000), ((tval % 6000) / 1000), (tval % 1000));
    elm_object_part_text_set(layout, "area/control/timer/timer", timer_str);

    return ECORE_CALLBACK_RENEW;
}

int main(int argc, char *argv[])
{
    char *pkg = "LaySim Editor";
    ply_ctx_s ctx = { PLAYING_READY, 0L, 0L, NULL, NULL };

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
    evas_object_show(ly);

    ctx.layout = ly;

    _set_playing_button(ly, &ctx);

    log("hello");

    elm_run();	
    elm_exit();

    return 0;
}
